#include <iostream>
#include "model_loader.h"
#include "tiny_obj_loader.h"
#include "renderer.h"
#include "slicer.h"

int main() {
    Mesh cube = load_model("../../Tests/", "Perimeter_test.obj");
    //std::cout << cube.faces.size();

    init_render_window("Hydrographics", 200);
    for(double h = 0.1; h < 1; h += 0.00005) {
        //h = 0.1;
        //std::cout << h << '\n';
        //std::vector < int > cut = slice_mesh(cube, h);
        //cube.deselect_all_polygons();
        //cube.select_polygons(cut);
        Mesh slice = slice_mesh(cube, h, dvec2(-1.3, -1), dvec2(1.3, 1.3));

        Mesh plane;
        plane.add_vertex(dvec3(-2, 1, h));
        plane.add_vertex(dvec3(2, 1, h));
        plane.add_vertex(dvec3(0, -2, h));
        plane.add_face(0, 1, 2, vec3(0, 0, 1));

        render_clear();
        render_mesh(cube, dvec3(50, 50, 50));
        //render_mesh(plane, vec3(232, 54, 235));
        render_mesh(slice, dvec3(232, 54, 235));
        render_show();
        //while (window_not_closed()) {}
        if(!window_not_closed()) break;
    }
    return 0;
}
