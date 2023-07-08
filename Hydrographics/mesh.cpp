#include "mesh.h"

Polygon::Polygon(int vert0, int vert1, int vert2, vec3 normal) {
    v0 = vert0;
    v1 = vert1;
    v2 = vert2;

    n = normal;
}

uint64_t Mesh::add_vertex(vec3 vert) {
    vertices.push_back(vert);
    return vertices.size() - 1;
}

void Mesh::add_polygon(int v0, int v1, int v2, vec3 n) {
    polygons.push_back(Polygon(v0, v1, v2, n));
}

void Mesh::select_polygons(std::vector < int > target) {
    for(auto poly : target){
        selected[poly] = true;
    }
}

void Mesh::deselect_all_polygons() {
    selected.clear();
}