#ifndef HYDROGRAPHICS_SLICER_H
#define HYDROGRAPHICS_SLICER_H

#include "mesh.h"

class Slice_polygon{
public:
    int poly_i;
    int e0, e1;
    vec3 n;

    Slice_polygon(int polygon_i, int edge0, int edge1, vec3 n_);
};

Mesh slice_mesh(Mesh &mesh, double h = 0, vec2 border_st = vec2(-1, -1), vec2 border_en = vec2(1, 1));

#endif //HYDROGRAPHICS_SLICER_H
