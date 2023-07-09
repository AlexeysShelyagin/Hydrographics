#ifndef HYDROGRAPHICS_MESH_H
#define HYDROGRAPHICS_MESH_H

#include <vector>
#include <stdint.h>
#include <unordered_map>
#include "include/glm/vec3.hpp"

using namespace glm;

class Polygon{
public:
    int v0, v1, v2;
    dvec3 n;

    Polygon() {};
    Polygon(int vert0, int vert1, int vert2, dvec3 normal);
};

class Mesh{
public:
    std::vector < dvec3 > vertices;
    std::vector < Polygon > polygons;

    std::unordered_map < int, bool > selected;

    Mesh() {};

    uint64_t add_vertex(dvec3 vert);
    void add_polygon(int v0, int v1, int v2, dvec3 n);

    void select_polygons(std::vector < int > target);
    void deselect_all_polygons();
};

#endif //HYDROGRAPHICS_MESH_H
