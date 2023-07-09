#ifndef HYDROGRAPHICS_MESH_H
#define HYDROGRAPHICS_MESH_H

#include <vector>
#include <stdint.h>
#include <unordered_map>
#include "include/glm/vec3.hpp"

using namespace glm;

class Face{
public:
    std::vector < int > verts;
    dvec3 n;

    Face() {};
    Face(int v0, int v1, int v2, dvec3 normal);
    Face(std::vector < int > vertices, dvec3 normal);
};

class Mesh{
public:
    std::vector < dvec3 > vertices;
    std::vector < Face > faces;

    std::unordered_map < int, bool > selected;

    Mesh() {};

    uint64_t add_vertex(dvec3 vert);
    void add_face(int v0, int v1, int v2, dvec3 n = dvec3(0, 0, 1));
    void add_face(std::vector < int > vertices, dvec3 n = dvec3(0, 0, 1));

    std::vector < dvec3 > face_vertices(int face_i);

    void select_polygons(std::vector < int > target);
    void deselect_all_polygons();
};

#endif //HYDROGRAPHICS_MESH_H
