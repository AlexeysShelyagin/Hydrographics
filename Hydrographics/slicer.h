#ifndef HYDROGRAPHICS_SLICER_H
#define HYDROGRAPHICS_SLICER_H

#include "mesh.h"

#include "include/glm/vec3.hpp"
#include "include/glm/vec2.hpp"
#include "include/glm/mat2x2.hpp"

namespace Slice{
    class Slice_edge{
    public:
        int poly_i;
        int e0, e1;

        Slice_edge(int polygon_i, int edge0, int edge1);
    };

    class Slice_unique_vertex{
    public:
        dvec3 vert;
        int edge;

        Slice_unique_vertex() = default;
        Slice_unique_vertex(dvec3 vertex, int edge_);
    };

    class Slice_connect_data{
    public:
        int v0, v1;
        int face0, face1;

        Slice_connect_data();
        Slice_connect_data(int _v0, int _v1, int _face0, int _face1);
    };
}

Mesh slice_mesh(Mesh &mesh, double h = 0, dvec2 border_st = dvec2(-1, -1), dvec2 border_en = dvec2(1, 1));

#endif //HYDROGRAPHICS_SLICER_H
