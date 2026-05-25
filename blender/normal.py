bl_info = {
    "name": "My Test Add-on",
    "blender": (5, 1, 0),
    "category": "Export",
}

import bpy
from mathutils import Vector, Matrix
import math

def orbit_camera(camera, center, radius, theta, phi):
    matrix = Matrix.Rotation(theta, 4, 'Z') @ Matrix.Rotation(phi, 4, 'X')
    camera.location = center + matrix @ Vector((0, 0, radius / math.sin(camera.data.angle / 2)))
    camera.rotation_euler = matrix.to_euler()

def create_camera_normal_material():
    """Generates a material that outputs camera-space normals mapped to RGB."""
    mat_name = "CameraSpaceNormal_Override"
    mat = bpy.data.materials.get(mat_name)

    if mat is None:
        mat = bpy.data.materials.new(name=mat_name)
        mat.use_nodes = True
        nodes = mat.node_tree.nodes
        links = mat.node_tree.links

        # Clear default nodes
        for node in nodes:
            nodes.remove(node)

        # Create Nodes
        node_geom = nodes.new(type="ShaderNodeNewGeometry")

        node_transform = nodes.new(type="ShaderNodeVectorTransform")
        node_transform.vector_type = 'NORMAL'
        node_transform.convert_from = 'WORLD'
        node_transform.convert_to = 'CAMERA'

        # Map Normal [-1, 1] to RGB [0, 1] -> (N * 0.5) + 0.5
        # CRITICAL FIX: Blender cameras face -Z. We multiply Z by -0.5 to invert it,
        # ensuring faces pointing at the camera render as +Z (Blue/Purple).
        node_mul = nodes.new(type="ShaderNodeVectorMath")
        node_mul.operation = 'MULTIPLY'
        node_mul.inputs[1].default_value = (0.5, 0.5, -0.5)

        node_add = nodes.new(type="ShaderNodeVectorMath")
        node_add.operation = 'ADD'
        node_add.inputs[1].default_value = (0.5, 0.5, 0.5)

        node_emission = nodes.new(type="ShaderNodeEmission")
        node_output = nodes.new(type="ShaderNodeOutputMaterial")

        # Link Nodes
        links.new(node_geom.outputs['Normal'], node_transform.inputs['Vector'])
        links.new(node_transform.outputs['Vector'], node_mul.inputs[0])
        links.new(node_mul.outputs['Vector'], node_add.inputs[0])
        links.new(node_add.outputs['Vector'], node_emission.inputs['Color'])
        links.new(node_emission.outputs['Emission'], node_output.inputs['Surface'])

    return mat

class SpritesheetOperator(bpy.types.Operator):
    """stellarlib spritesheet normal map generator"""
    bl_idname = "stellarlib.spritesheet"
    bl_label = "Generate Normal Spritesheet"

    filepath: bpy.props.StringProperty(subtype="FILE_PATH")

    def execute(self, context):
        # 1. Store original states to restore later
        original_camera = context.scene.camera
        original_override = context.view_layer.material_override
        original_view_transform = context.scene.view_settings.view_transform

        # 2. Setup the override material for camera-space normals
        normal_mat = create_camera_normal_material()
        context.view_layer.material_override = normal_mat

        # 3. Force Color Management to 'Raw' to prevent AgX/Filmic from distorting normal map vectors
        context.scene.view_settings.view_transform = 'Raw'

        # 4. Create and link dummy camera (Modern Blender requires linking to a collection)
        cam_data = bpy.data.cameras.new("SpritesheetCamera")
        cam_obj = bpy.data.objects.new("SpritesheetCamera", cam_data)
        context.scene.collection.objects.link(cam_obj)
        context.scene.camera = cam_obj

        # 5. Calculate framing bounds
        vertices = [o.matrix_world @ Vector(v) for o in context.scene.objects if o.type == "MESH" for v in o.bound_box]

        if not vertices:
            self.report({'WARNING'}, "No meshes found in scene.")
            return {'CANCELLED'}

        center = sum(vertices, Vector()) / len(vertices)
        radius = max(((v - center).length for v in vertices))

        # 6. Adjust Render Settings
        context.scene.render.image_settings.file_format = "PNG"
        context.scene.render.image_settings.color_mode = 'RGBA'
        context.scene.render.film_transparent = True
        context.scene.render.resolution_x = 256
        context.scene.render.resolution_y = 256
        context.scene.render.filter_size = 0.01

        # 7. Render Loop
        for i in range(32):
            for j in range(17):
                orbit_camera(cam_obj, center, radius, math.radians(i * 11.25), math.radians(j * 11.25))
                context.scene.render.filepath = f"{self.filepath}_{i + 1}_{j + 1}.png"
                bpy.ops.render.render(write_still=True)

        # 8. Cleanup and Restore
        context.scene.camera = original_camera
        context.view_layer.material_override = original_override
        context.scene.view_settings.view_transform = original_view_transform

        # Remove dummy camera
        bpy.data.objects.remove(cam_obj, do_unlink=True)
        bpy.data.cameras.remove(cam_data, do_unlink=True)

        return {'FINISHED'}

    def invoke(self, context, event):
        context.window_manager.fileselect_add(self)
        return {'RUNNING_MODAL'}

def menu_func(self, context):
    self.layout.operator(SpritesheetOperator.bl_idname)

def register():
    bpy.utils.register_class(SpritesheetOperator)
    bpy.types.TOPBAR_MT_file_export.append(menu_func)

def unregister():
    bpy.types.TOPBAR_MT_file_export.remove(menu_func)
    bpy.utils.unregister_class(SpritesheetOperator)

if __name__ == "__main__":
    register()
