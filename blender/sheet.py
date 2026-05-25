bl_info = {
    "name": "My Test Add-on",
    "blender": (5, 1, 0),
    "category": "Export",
}

import bpy
from mathutils import Vector, Matrix
import math
import tempfile
import os
import numpy as np

def orbit_camera(camera, center, radius, theta, phi):
    matrix = Matrix.Rotation(theta, 4, 'Z') @ Matrix.Rotation(phi, 4, 'X')
    camera.location = center + matrix @ Vector((0, 0, radius / math.sin(camera.data.angle / 2)))
    camera.rotation_euler = matrix.to_euler()

class SpritesheetOperator(bpy.types.Operator):
    """stellarlib spritesheet"""
    bl_idname = "stellarlib.spritesheet"
    bl_label = "Generate spritesheet"

    filepath: bpy.props.StringProperty(subtype="FILE_PATH")
    resolution: bpy.props.IntProperty(name="Resolution", default=128, min=32, max=512)
    steps: bpy.props.IntProperty(name="Steps", default=4, min=4, max=12)

    def execute(self, context):
        # Prevent errors if there are no meshes in the scene
        vertices = [o.matrix_world @ Vector(v) for o in context.scene.objects if o.type == "MESH" for v in o.bound_box]
        if not vertices:
            self.report({'ERROR'}, "No mesh objects found.")
            return {'CANCELLED'}

        original_camera = context.scene.camera

        # Create temporary camera
        temp_cam_data = bpy.data.cameras.new("TempCamera")
        temp_cam_obj = bpy.data.objects.new("TempCamera", temp_cam_data)
        context.scene.camera = temp_cam_obj

        center = sum(vertices, Vector()) / len(vertices)
        radius = max(((v - center).length for v in vertices))

        # Access background safely
        if "Background" in context.scene.world.node_tree.nodes:
            light = context.scene.world.node_tree.nodes["Background"]
            light.inputs[0].default_value = (1, 1, 1, 1)

        context.scene.render.image_settings.file_format = "PNG"
        context.scene.render.film_transparent = True
        context.scene.render.resolution_x = self.resolution
        context.scene.render.resolution_y = self.resolution
        context.scene.render.filter_size = 0

        steps_x = self.steps * 4
        steps_y = self.steps * 2 + 1
        angle = math.radians(360 / steps_x)

        # --- Spritesheet Dimensions & Memory Allocation ---
        res = self.resolution
        sheet_width = steps_x * res
        sheet_height = steps_y * res

        # Pre-allocate numpy arrays for performance
        # Blender images use Float32 RGBA. Array shape: [Height, Width, RGBA]
        sheet_np = np.zeros((sheet_height, sheet_width, 4), dtype=np.float32)
        temp_img_np = np.zeros(res * res * 4, dtype=np.float32)

        # Process frames in a temporary directory
        with tempfile.TemporaryDirectory() as temp_dir:
            temp_path = os.path.join(temp_dir, "temp_frame.png")
            context.scene.render.filepath = temp_path

            for j in range(steps_y):
                for i in range(steps_x):
                    # Position camera
                    orbit_camera(context.scene.camera, center, radius, i * angle, j * angle)

                    # Render the current frame to the temp file
                    bpy.ops.render.render(write_still=True)

                    # Load temp frame into memory
                    img = bpy.data.images.load(temp_path)

                    # Fast-extract pixels
                    img.pixels.foreach_get(temp_img_np)
                    bpy.data.images.remove(img)

                    # Reshape flat array into a 2D matrix (Width x Height x 4 Channels)
                    tile = temp_img_np.reshape((res, res, 4))

                    # Map to the master sheet array
                    # Standard spritesheets read Top-to-Bottom, Left-to-Right.
                    # Blender coordinates are Bottom-Up, so we invert the Y-axis calculation:
                    y_start = (steps_y - 1 - j) * res
                    y_end = y_start + res
                    x_start = i * res
                    x_end = x_start + res

                    # Paste tile into the master numpy array
                    sheet_np[y_start:y_end, x_start:x_end] = tile

        # --- Save the Final Spritesheet ---
        # Ensure the filename ends with .png
        final_filepath = self.filepath if self.filepath.lower().endswith(".png") else self.filepath + ".png"

        # Create the final Blender image block
        sheet_img = bpy.data.images.new("Spritesheet", width=sheet_width, height=sheet_height, alpha=True)

        # Flatten the master numpy array and inject it into the image fast
        sheet_img.pixels.foreach_set(sheet_np.ravel())

        # Save to disk and clear from memory
        sheet_img.filepath_raw = final_filepath
        sheet_img.save()
        bpy.data.images.remove(sheet_img)

        # Cleanup the temporary camera
        context.scene.camera = original_camera
        bpy.data.objects.remove(temp_cam_obj)
        bpy.data.cameras.remove(temp_cam_data)

        self.report({'INFO'}, f"Spritesheet saved to {final_filepath}")
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
