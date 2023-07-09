#include <iostream>
#include "model_loader.h"
#include "tiny_obj_loader.h"
#include "renderer.h"
#include "slicer.h"

int main() {
    Mesh cube = load_model("../../Tests/", "Monkey.obj");
    //std::cout << cube.polygons.size();

    init_render_window("Hydrographics", 200);
    for(double h = -1; h < 1; h += 0.0005) {
        //std::cout << h << '\n';
        //std::vector < int > cut = slice_mesh(cube, h);
        //cube.deselect_all_polygons();
        //cube.select_polygons(cut);
        Mesh slice = slice_mesh(cube, h);

        Mesh plane;
        plane.add_vertex(dvec3(-2, 1, h));
        plane.add_vertex(dvec3(2, 1, h));
        plane.add_vertex(dvec3(0, -2, h));
        plane.add_polygon(0, 1, 2, vec3(0, 0, 1));

        render_clear();
        //render_mesh(cube);
        //render_mesh(plane, vec3(232, 54, 235));
        render_mesh(slice, vec3(232, 54, 235));
        render_show();
        //while (window_not_closed()) {}
        if(!window_not_closed()) break;
    }
    return 0;
}
