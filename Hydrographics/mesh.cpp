#include "mesh.h"

Face::Face(int v0, int v1, int v2, dvec3 normal) {
    verts.resize(3);
    verts[0] = v0;
    verts[1] = v1;
    verts[2] = v2;

    n = normal;
}

Face::Face(std::vector < int > vertices, dvec3 normal) {
    verts = vertices;

    n = normal;
}

uint64_t Mesh::add_vertex(dvec3 vert) {
    vertices.push_back(vert);
    return vertices.size() - 1;
}

void Mesh::add_face(int v0, int v1, int v2, dvec3 n) {
    faces.push_back(Face(v0, v1, v2, n));
}

void Mesh::add_face(std::vector < int > vertices, dvec3 n) {
    faces.push_back(Face(vertices, n));
}

void Mesh::add_face(Face face) {
    faces.push_back(face);
}

void Mesh::delete_face(int n){
    faces[n].verts.clear();
    face_to_delete.push_back(n);

    // TODO: create func to delete empty faces
}

std::vector < dvec3 > Mesh::face_vertices(int face_i) {
    int v_n = faces[face_i].verts.size();
    std::vector < dvec3 > verts(v_n);
    for(int i = 0; i < v_n; i++){
        verts[i] = vertices[faces[face_i].verts[i]];
    }

    return verts;
}

void Mesh::select_polygons(std::vector < int > target) {
    for(auto poly : target){
        selected[poly] = true;
    }
}

void Mesh::deselect_all_polygons() {
    selected.clear();
}