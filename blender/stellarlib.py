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

class SpritesheetOperator(bpy.types.Operator):
	"""stellarlib spritesheet"""
	bl_idname = "stellarlib.spritesheet"
	bl_label = "Generate spritesheet"

	filepath: bpy.props.StringProperty(subtype="FILE_PATH")
	resolution: bpy.props.IntProperty(name="Resolution", default=128, min=32, max=512)
	steps: bpy.props.IntProperty(name="Steps", default=4, min=4, max=12)

	def execute(self, context):
		original = context.scene.camera
		context.scene.camera = bpy.data.objects.new("Camera", bpy.data.cameras.new("Camera"))
		vertices = [o.matrix_world @ Vector(v) for o in context.scene.objects if o.type == "MESH" for v in o.bound_box]
		center = sum(vertices, Vector()) / len(vertices)
		radius = max(((v - center).length for v in vertices))
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
		for i in range(steps_x):
			for j in range(steps_y):
				orbit_camera(context.scene.camera, center, radius, i * angle, j * angle)
				context.scene.render.filepath = self.filepath + str(i + 1) + str(j + 1)
				bpy.ops.render.render(write_still=True)
		context.scene.camera = original
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
