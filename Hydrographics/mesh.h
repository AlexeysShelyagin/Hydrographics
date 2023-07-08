#ifndef HYDROGRAPHICS_MESH_H
#define HYDROGRAPHICS_MESH_H

#include <vector>
#include <stdint.h>
#include <unordered_map>


#include "vectors.h"

class Polygon{
public:
    int v0, v1, v2;
    vec3 n;

    Polygon() {};
    Polygon(int vert0, int vert1, int vert2, vec3 normal);
};

class Mesh{
public:
    std::vector < vec3 > vertices;
    std::vector < Polygon > polygons;

    std::unordered_map < int, bool > selected;

    Mesh() {};

    uint64_t add_vertex(vec3 vert);
    void add_polygon(int v0, int v1, int v2, vec3 n);

    void select_polygons(std::vector < int > target);
    void deselect_all_polygons();
};

#endif //HYDROGRAPHICS_MESH_H
