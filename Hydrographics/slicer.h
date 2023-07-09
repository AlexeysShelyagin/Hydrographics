#ifndef HYDROGRAPHICS_SLICER_H
#define HYDROGRAPHICS_SLICER_H

#include "mesh.h"

#include "include/glm/vec3.hpp"
#include "include/glm/vec2.hpp"

class Slice_polygon{
public:
    int poly_i;
    int e0, e1;
    dvec3 n;

    Slice_polygon(int polygon_i, int edge0, int edge1, dvec3 n_);
};

Mesh slice_mesh(Mesh &mesh, double h = 0, dvec2 border_st = dvec2(-1, -1), dvec2 border_en = dvec2(1, 1));

#endif //HYDROGRAPHICS_SLICER_H
