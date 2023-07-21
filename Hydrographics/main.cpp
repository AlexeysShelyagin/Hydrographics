#include <iostream>
#include "model_loader.h"
#include "tiny_obj_loader.h"
#include "renderer.h"
#include "slicer.h"
#include "triangulation.h"

int main() {
    Mesh cube = load_model("../../Tests/", "Perimeter_test.obj");
    //std::cout << cube.faces.size();

    init_render_window("Hydrographics", 200);
    for(double h = 0.1; h < 1; h += 0.00005) {
        //h = 0.1;
        Mesh slice = slice_mesh(cube, h, dvec2(-1.3, -1), dvec2(1.3, 1.3));
        Mesh slice_triangulated = triangulate(slice);

        render_clear();
        render_mesh(cube, dvec3(50, 50, 50));
        render_mesh(slice, dvec3(232, 54, 235));
        render_show();

        //while (window_not_closed()) {}
        if(!window_not_closed()) break;
    }
    return 0;
}
