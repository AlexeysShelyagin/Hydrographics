import bpy
import mathutils

def main(context):
    for ob in context.scene.objects:
        print(ob)


class HydrographicsOperator(bpy.types.Operator):
    bl_idname = "object.hydrographics_operator"
    bl_label = "Hydrographics Operator"

    @classmethod
    def poll(cls, context):
        return context.active_object is not None

    def execute(self, context):
        main(context)
        return {'FINISHED'}


def menu_func(self, context):
    self.layout.operator(HydrographicsOperator.bl_idname, text=HydrographicsOperator.bl_label)


# Register and add to the "object" menu (required to also use F3 search "Simple Object Operator" for quick access).
def register():
    bpy.utils.register_class(HydrographicsOperator)
    bpy.types.VIEW3D_MT_object.append(menu_func)


def unregister():
    bpy.utils.unregister_class(HydrographicsOperator)
    bpy.types.VIEW3D_MT_object.remove(menu_func)


if __name__ == "__main__":
    register()



def create_cube():
    bpy.ops.mesh.primitive_cube_add(size=5, location=(0, 0, -2.5))
    return bpy.context.object

def select_object(name):
    bpy.ops.object.select_all(action='DESELECT')
    bpy.data.objects[name].select_set(True)

def create_slice(height):
    select_object('Body')
    bpy.ops.object.duplicate(linked=False)
    bpy.data.objects['Body.001'].name = f'z:{height}'
    
    select_object(f'z:{height}')
    bpy.ops.object.mode_set(mode='EDIT')
    bpy.ops.mesh.bisect(plane_co=(0, 0, height), plane_no=(0, 0, 1), use_fill=True, clear_inner=True, clear_outer=True)
    bpy.ops.object.mode_set(mode='OBJECT')

'''
N = 100

body = bpy.data.objects['Body']
body.location.z = 0
for i in range(N):
    create_slice(i*0.01)

    #body.location.z -= 0.01

    print(f'{round(i / N * 100)}%')

print('\nok')
'''