#include "slicer.h"

#include <vector>
#include <algorithm>
#include <iostream>

#define DOUBLE_VERTEX_PRECISION 1e-9

using namespace glm;

Slice_face::Slice_face(int polygon_i, int edge0, int edge1, dvec3 n_) {
    poly_i = polygon_i;
    e0 = edge0;
    e1 = edge1;
    n = n_;
}

Slice_unique_vertex::Slice_unique_vertex(dvec3 vertex, int edge_){
    vert = vertex;
    edge = edge_;
}

bool vec3_unique_comp(Slice_unique_vertex a, Slice_unique_vertex b){
    if(abs(a.vert.x - b.vert.x) < DOUBLE_VERTEX_PRECISION) return a.vert.y < b.vert.y;
    return a.vert.x < b.vert.x;
}

dvec2 edges_intersect(dvec2 e0p0, dvec2 e0p1, dvec2 e1p0, dvec2 e1p1){
    dvec2 a = e0p1 - e0p0;
    dvec2 b = e1p1 - e1p0;
    dvec2 r_ab = e1p0 - e0p0;

    double D = determinant(mat2x2(a.x, a.y, b.x, b.y));
    double D_t = determinant(mat2x2(a.x, a.y, r_ab.x, r_ab.y));
    double D_k = determinant(mat2x2(r_ab.x, r_ab.y, b.x, b.y));

    double t = D_t / D;
    double k = D_k / D;

    return dvec2(t, k);
}

std::vector < dvec3 > vertices_by_edge(Mesh &mesh, int poly_i, int edge_i){
    std::vector < dvec3 > v(2);
    Face face = mesh.faces[poly_i];

    v[0] = mesh.vertices[face.verts[edge_i]];
    v[1] = mesh.vertices[face.verts[(edge_i + 1) % face.verts.size()]];

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
    //std::vector < int > polygons_to_cut;
    std::vector < Slice_face > intersections;

    for(int i = 0; i < mesh.faces.size(); i++){
        std::vector < dvec3 > face = mesh.face_vertices(i);
        if(face.size() != 3){
            throw std::runtime_error("mesh is not triangulated");
        }

        int edge0 = -1, edge1 = -1;
        for(int j = 0; j < 3; j++){
            int v0 = j, v1 = (j + 1) % 3;
            if(std::min(face[v0].z, face[v1].z) <= h && h <= std::max(face[v0].z, face[v1].z)){
                if(edge0 == -1) edge0 = j;
                else edge1 = j;
            }
        }
        if(edge0 != -1){
            intersections.push_back(Slice_face(i, edge0, edge1, mesh.faces[i].n));
            //polygons_to_cut.push_back(i);
        }
    }

    if(intersections.size() == 0){
        Mesh empty_slice;
        return empty_slice;
    }

    Mesh slice_img;
    std::vector < Slice_unique_vertex > all_verts;
    for(int i = 0; i < intersections.size(); i++){
        dvec3 v0 = point_on_height(mesh, intersections[i].poly_i, intersections[i].e0, h);
        dvec3 v1 = point_on_height(mesh, intersections[i].poly_i, intersections[i].e1, h);

        all_verts.push_back(Slice_unique_vertex(v0, i));
        all_verts.push_back(Slice_unique_vertex(v1, i));

        dvec3 ph = v0 + (v1 - v0) * 0.5;
        ph.z += 0.01;
        slice_img.add_vertex(ph);
        slice_img.add_vertex(v0);
        slice_img.add_vertex(v1);
        int last = slice_img.vertices.size() - 1;
        slice_img.add_face(last, last - 1, last - 2, intersections[i].n);

    }

    std::sort(all_verts.begin(), all_verts.end(), vec3_unique_comp);

    Mesh slice;
    bool merged = false;
    std::vector < std::vector < int > > edge_groups;
    for(int i = 0; i < all_verts.size(); i++){
        //std::cout << all_verts[i].vert.x << ' ' <<  all_verts[i].vert.y << ' ' << all_verts[i].vert.z << ":   " << all_verts[i].edge << '\n';

        if (!merged){
            slice.add_vertex(all_verts[i].vert);
            edge_groups.push_back(std::vector < int > (1));
            edge_groups[edge_groups.size() - 1][0] = all_verts[i].edge;
        }
        merged = false;


        if(i >= all_verts.size() - 1) continue;
        dvec3 dv = all_verts[i + 1].vert - all_verts[i].vert;
        if(abs(dv.x) < DOUBLE_VERTEX_PRECISION && abs(dv.y) < DOUBLE_VERTEX_PRECISION){
            merged = true;
            edge_groups[edge_groups.size() - 1].push_back(all_verts[i + 1].edge);
        }

    }

    std::vector < std::vector < int > > edges(intersections.size(), std::vector < int > ());
    for(int i = 0; i < slice.vertices.size(); i++){
        for(int j = 0; j < edge_groups[i].size(); j++){
            edges[edge_groups[i][j]].push_back(i);
        }
    }

    std::vector < std::vector < int > > outline_graph(slice.vertices.size(), std::vector < int > ());
    for(int i = 0; i < edges.size(); i++){
        if (edges[i].size() != 2){
            throw std::runtime_error("Invalid mesh: unable to slice");
        }
        outline_graph[edges[i][0]].push_back(edges[i][1]);
        outline_graph[edges[i][1]].push_back(edges[i][0]);
    }

    std::vector < bool > used(slice.vertices.size());
    int st;
    for(int i = 0; i < slice.vertices.size(); i++){
        if (!used[i]){
            st = i;
            std::vector < int > outline_seq;
            while(!used[st]){
                outline_seq.push_back(st);
                used[st] = true;
                if(!used[outline_graph[st][0]]) st = outline_graph[st][0];
                else st = outline_graph[st][1];
            }
            slice.add_face(outline_seq, dvec3(0, 0, 1));
        }
    }

    slice.add_vertex(dvec3(border_st.x, border_st.y, h));
    slice.add_vertex(dvec3(border_st.x, border_en.y, h));
    slice.add_vertex(dvec3(border_en.x, border_en.y, h));
    slice.add_vertex(dvec3(border_en.x, border_st.y, h));
    int last_v = slice.vertices.size() - 1;
    std::vector < int > plane = {last_v, last_v - 1, last_v - 2, last_v - 3};
    slice.add_face(plane, dvec3(0, 0, 1));

/*
    std::cout << "\n-------------------\n";

    for(Face seq : slice.faces){
        for(int vert : seq.verts){
            std::cout << vert << " -> ";
        }
        std::cout << seq.verts[0] << '\n';
    }
    std::cout << '\n';

    for(int i = 0; i < edges.size(); i++){
        std::cout << i << ": ";
        for(int j = 0; j < edges[i].size(); j++){
            std::cout << edges[i][j] << ' ';
        }
        std::cout << '\n';
    }

    std::cout << '\n';
    for(int i = 0; i < slice.vertices.size(); i++){
        std::cout << i << ": " << slice.vertices[i].x << ' ' << slice.vertices[i].y << ' ' <<slice.vertices[i].z;
        //for(int j = 0; j < edge_groups[i].size(); j++) std::cout << edge_groups[i][j] << ' ';
        std::cout << '\n';
    }
*/
    return slice;
}