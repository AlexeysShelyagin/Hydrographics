#include "slicer.h"

#include <vector>
#include <algorithm>
#include <iostream>

using namespace glm;

Slice_polygon::Slice_polygon(int polygon_i, int edge0, int edge1, dvec3 n_) {
    poly_i = polygon_i;
    e0 = edge0;
    e1 = edge1;
    n = n_;
}

std::vector < dvec3 > vertices_by_edge(Mesh &mesh, int poly_i, int edge_i){
    std::vector < dvec3 > v(2);
    Polygon poly = mesh.polygons[poly_i];

    switch (edge_i) {
        case 0:
            v[0] = mesh.vertices[poly.v0];
            v[1] = mesh.vertices[poly.v1];
            break;
        case 1:
            v[0] = mesh.vertices[poly.v1];
            v[1] = mesh.vertices[poly.v2];
            break;
        case 2:
            v[0] = mesh.vertices[poly.v2];
            v[1] = mesh.vertices[poly.v0];
            break;
        default: break;
    }

    return v;
}

dvec3 point_on_height(Mesh &mesh, int poly_i, int edge_i, double h){
    std::vector < dvec3 > edge = vertices_by_edge(mesh, poly_i, edge_i);
    dvec3 v0 = edge[0], v1 = edge[1];
    if(v0.z > v1.z) std::swap(v0, v1);

    dvec3 v01 = v1 - v0;
    double k = (h - v0.z) / v01.z;
    dvec3 p = v0 + v01 * k;

    return p;
}

Mesh slice_mesh(Mesh &mesh, double h, dvec2 border_st, dvec2 border_en){
    std::vector < int > polygons_to_cut;
    std::vector < Slice_polygon > intersections;

    for(int i = 0; i < mesh.polygons.size(); i++){
        double poly_h[3] = {
                mesh.vertices[mesh.polygons[i].v0].z,
                mesh.vertices[mesh.polygons[i].v1].z,
                mesh.vertices[mesh.polygons[i].v2].z
        };
        int edge0 = -1, edge1 = -1;
        for(int j = 0; j < 3; j++){
            int v0 = j, v1 = (j + 1) % 3;
            if(std::min(poly_h[v0], poly_h[v1]) <= h && h <= std::max(poly_h[v0], poly_h[v1])){
                if(edge0 == -1) edge0 = j;
                else edge1 = j;
            }
        }
        if(edge0 != -1){
            intersections.push_back(Slice_polygon(i, edge0, edge1, mesh.polygons[i].n));
            polygons_to_cut.push_back(i);
        }
    }

    Mesh slice;
    for(Slice_polygon intersect : intersections){
        dvec3 p0 = point_on_height(mesh, intersect.poly_i, intersect.e0, h);
        dvec3 p1 = point_on_height(mesh, intersect.poly_i, intersect.e1, h);

        slice.add_vertex(p0);
        slice.add_vertex(p1);
        int last = slice.vertices.size() - 1;
        slice.add_polygon(last, last - 1, last, intersect.n);
    }

    return slice;
}