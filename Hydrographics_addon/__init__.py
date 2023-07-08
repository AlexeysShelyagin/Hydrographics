# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTIBILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.

bl_info = {
    "name" : "Hydrographics",
    "author" : "Devel_team",
    "description" : "",
    "blender" : (2, 80, 0),
    "version" : (0, 0, 1),
    "location" : "",
    "warning" : "",
    "category" : "Physics"
}

import bpy

def main(context):
    def select_object(name):
        bpy.ops.object.select_all(action='DESELECT')
        bpy.data.objects[name].select_set(True)

    def create_slice(height):
        name = f'z:{round(height, 3)}'

        select_object('Form')
        bpy.ops.object.duplicate(linked=False)
        bpy.data.objects['Form.001'].name = name
        
        select_object(name)
        bpy.ops.object.mode_set(mode='EDIT')
        bpy.ops.mesh.select_all(action = 'SELECT')
        bpy.ops.mesh.bisect(plane_co=(0, 0, height), plane_no=(0, 0, 1), use_fill=True, clear_inner=True, clear_outer=True)
        bpy.ops.object.mode_set(mode='OBJECT')
        
    def create_cube(name='Cube'):
        bpy.ops.mesh.primitive_cube_add(size=5)
        bpy.context.object.name = name
        return bpy.context.object

    def create_form():
        print('Creating form... ', end='')

        form = create_cube('Form')
        form.location.z = 2.49

        form_modifier = form.modifiers.new(type='BOOLEAN', name='form_create')
        form_modifier.object = bpy.data.objects['Body']
        form_modifier.solver = 'FAST'
        bpy.ops.object.modifier_apply(modifier='form_create')

        print('form created')


    N = 5

    body = bpy.data.objects['Body']
    body.location.z = 1.33
    create_form()
    for i in range(N):
        create_slice(i*0.01)

        #body.location.z -= 0.01

        print(f'{round(i / N * 100)}%', end='\r')

    print('\nok')


class HydrographicsOperator(bpy.types.Operator):
    """Tooltip"""
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

def register():
    bpy.utils.register_class(HydrographicsOperator)
    bpy.types.VIEW3D_MT_object.append(menu_func)


def unregister():
    bpy.utils.unregister_class(HydrographicsOperator)
    bpy.types.VIEW3D_MT_object.remove(menu_func)

if __name__ == "__main__":
    register()