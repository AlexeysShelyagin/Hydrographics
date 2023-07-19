#include "slicer.h"

#include <vector>
#include <algorithm>
#include <iostream>
#include <stack>
#include <map>

#define DOUBLE_VERTEX_PRECISION 1e-9
#define MAX_JOIN_ITERATIONS 100
#define MAX_DIMENSION 1000.0

using namespace glm;
using namespace Slice;

Slice_edge::Slice_edge(int polygon_i, int edge0, int edge1) {
    poly_i = polygon_i;
    e0 = edge0;
    e1 = edge1;
}

Slice_unique_vertex::Slice_unique_vertex(dvec3 vertex, int edge_){
    vert = vertex;
    edge = edge_;
}

Slice_connect_data::Slice_connect_data(){
    v0 = v1 = -1;
    face0 = face1 = -1;
}

Slice_connect_data::Slice_connect_data(int _v0, int _v1, int _face0, int _face1) {
    v0 = _v0;
    v1 = _v1;
    face0 = _face0;
    face1 = _face1;
}

bool vec3_unique_comp(Slice_unique_vertex a, Slice_unique_vertex b){
    if(abs(a.vert.x - b.vert.x) < DOUBLE_VERTEX_PRECISION) return a.vert.y < b.vert.y;
    return a.vert.x < b.vert.x;
}

dvec2 edges_intersect(dvec3 e0p0, dvec3 e0p1, dvec3 e1p0, dvec3 e1p1){
    dvec2 a = e0p1 - e0p0;
    dvec2 b = e1p0 - e1p1;
    dvec2 r_ab = e1p0 - e0p0;

    double D = determinant(mat2x2(a.x, a.y, b.x, b.y));
    double D_t = determinant(mat2x2(a.x, a.y, r_ab.x, r_ab.y));
    double D_k = determinant(mat2x2(r_ab.x, r_ab.y, b.x, b.y));

    if (D == 0) return dvec2(-1, -1);
    return dvec2 (D_t / D, D_k / D);
}

bool is_intersect(dvec2 res, bool allow_corners = true){
    if(allow_corners) return (0 <= res.x && res.x <= 1 && 0 <= res.y && res.y <= 1);
    return (0 < res.x && res.x < 1 && 0 < res.y && res.y < 1);
}

std::vector < dvec3 > vertices_by_edge(Mesh &mesh, int poly_i, int edge_i){
    std::vector < dvec3 > v(2);
    Face face = mesh.faces[poly_i];

    v[0] = mesh.vertices[face.verts[edge_i]];
    v[1] = mesh.vertices[face.verts[(edge_i + 1) % face.verts.size()]];

    return v;
}

dvec3 point_on_height(Mesh &mesh, int poly_i, int edge_i, double h) {
    std::vector<dvec3> edge = vertices_by_edge(mesh, poly_i, edge_i);
    dvec3 v0 = edge[0], v1 = edge[1];
    if (v0.z > v1.z) std::swap(v0, v1);

    dvec3 v01 = v1 - v0;
    double k = (h - v0.z) / v01.z;
    dvec3 p = v0 + v01 * k;

    return p;
}

int normal_by_vertices(std::vector < std::vector < int > > &normal_indices, int v0, int v1){
    int n00 = normal_indices[v0][0], n01 = normal_indices[v0][1];
    int n10 = normal_indices[v1][0], n11 = normal_indices[v1][1];

    if(n00 == n10) return n00;
    if(n00 == n11) return n00;
    if(n01 == n10) return n01;
    if(n01 == n11) return n01;
}

int closest_intersection(Mesh &slice, dvec3 &st, dvec3 &en, Face &face){
    std::vector < dvec2 > face_res;
    std::vector < int > face_res_i;
    for(int i = 0; i < face.verts.size(); i++){
        dvec2 res = edges_intersect(st, en, slice.vertices[face.verts[i]], slice.vertices[face.verts[(i + 1) % face.verts.size()]]);
        if(is_intersect(res, false)){
            face_res.push_back(res);
            face_res_i.push_back(i);
        }
    }

    double max_t = 0, max_t_i = -1;
    for(int i = 0; i < face_res.size(); i++){
        if(face_res[i].x > max_t){
            max_t = face_res[i].x;
            max_t_i = face_res_i[i];
        }
    }

    return max_t_i;
}

int face_intersection(Mesh &slice, dvec3 &st, dvec3 &en, Face &face){
    int n = 0;
    for(int i = 0; i < face.verts.size(); i++){
        dvec2 res = edges_intersect(st, en, slice.vertices[face.verts[i]], slice.vertices[face.verts[(i + 1) % face.verts.size()]]);
        if(is_intersect(res, true)){
            n++;
        }
    }

    return n;
}

double face_intersection_dist(Mesh &slice, dvec3 &st, dvec3 &en, Face &face){
    double min_dist = MAX_DIMENSION;
    for(int i = 0; i < face.verts.size(); i++){
        dvec2 res = edges_intersect(st, en, slice.vertices[face.verts[i]], slice.vertices[face.verts[(i + 1) % face.verts.size()]]);
        if(is_intersect(res, true)){
            if(res.x < min_dist) min_dist = res.x;
        }
    }

    if(min_dist > 1 || min_dist < 0) return -1;
    return min_dist;
}

std::pair < int, int > find_connection(Mesh &slice, int face1_i, int face2_i){
    Face face1 = slice.faces[face1_i];
    Face face2 = slice.faces[face2_i];

    int i1 = 0, i2 = 0, i1_new, i2_new;
    dvec3  st = slice.vertices[face1.verts[i1]], en = slice.vertices[face2.verts[i2]];
    for (int i = 0; i < MAX_JOIN_ITERATIONS; i++) {
        if (i1 != -1) {
            st = slice.vertices[face1.verts[i1]];
            i1_new = closest_intersection(slice, st, en, face1);
        }
        if (i2 != -1) {
            en = slice.vertices[face2.verts[i2]];
            i2_new = closest_intersection(slice, en, st, face2);
        }

        if(i1_new == -1 && i2_new == -1) break;
        if(i1_new != -1) i1 = i1_new;
        if(i2_new != -1) i2 = i2_new;
    }

    // TODO: there's an error

    return std::make_pair(face1.verts[i1], face2.verts[i2]);
}

std::vector < int > outline_inside(Mesh &slice, int outline_i, std::vector < dvec3 > normals, std::vector < std::vector < int > > normal_indices){
    int v0 = slice.faces[outline_i].verts[0];
    int v1 = slice.faces[outline_i].verts[1];
    dvec3 n = normals[normal_by_vertices(normal_indices, v0, v1)];

    dvec3 st = slice.vertices[v0] + (slice.vertices[v1] - slice.vertices[v0]) * 0.5;
    dvec3 en = st + n * MAX_DIMENSION;

    std::vector < int > inside;
    for(int i = 0; i < slice.faces.size(); i++){
        int num = face_intersection(slice, st, en, slice.faces[i]);
        if(num % 2 == 1 && i != outline_i){
            inside.push_back(i);
        }
    }

    return inside;
}

Face merge_outlines(Mesh &slice, std::vector < std::vector < Slice_connect_data > > &connection_tree, std::vector < bool > &merged, int face_i){
    merged[face_i] = true;
    Face face = slice.faces[face_i];
    Slice_connect_data parent_data;

    for(Slice_connect_data i : connection_tree[face_i]){
        if(!merged[i.face1]){
            Face face_to_merge = merge_outlines(slice, connection_tree, merged, i.face1);

            int merge_i = 0;
            while (face.verts[merge_i] != i.v0 && face.verts[merge_i] != i.v1) merge_i++;
            face.verts.insert(face.verts.begin() + merge_i + 1, face.verts[merge_i]);
            face.verts.insert(face.verts.begin() + merge_i + 1, face_to_merge.verts.begin(), face_to_merge.verts.end());
        }
        else parent_data = i;
    }

    if(parent_data.v0 == -1) return face;

    for(int i = 0; i < face.verts.size(); i++){
        if(face.verts[i] == parent_data.v0) std::swap(parent_data.v0, parent_data.v1);
        if(face.verts[i] == parent_data.v1){
            face.verts.insert(face.verts.end(), face.verts.begin(), face.verts.begin() + i);
            face.verts.erase(face.verts.begin(), face.verts.begin() + i);
            break;
        }
    }
    face.verts.push_back(face.verts[0]);

    std::cout << face_i << ": ";
    std::cout << "(";
    for(int i: face.verts){
        std::cout << i << ", ";
    }
    std::cout << ") (" << parent_data.v0 << ", " << parent_data.v1 << ")\n";

    return face;
}

Face connect_group(Mesh &slice, std::vector < int > faces){
    std::stack < std::pair < int, int > > connect;
    std::vector < bool > connected(slice.faces.size());

    for(int i = 0; i < faces.size() - 1; i++){
        connect.push(std::make_pair(faces[i], faces[i + 1]));
    }

    std::map < std::pair < int, int >, bool > verts_to_connect;
    std::vector < Slice_connect_data > connection_data;

    while(!connect.empty()){
        int from = connect.top().first;
        int to = connect.top().second;
        connect.pop();
        if(connected[from] && connected[to]) continue;

        //std::cout << from << " -> " << to << '\n';
        std::pair < int, int > verts = find_connection(slice, from, to);
        std::vector < std::pair < double, int > > intersected;
        for(int i : faces){
            if(i != from && i != to){
                double dist = face_intersection_dist(
                    slice,
                    slice.vertices[verts.first],
                    slice.vertices[verts.second],
                    slice.faces[i]
                );
                if(dist != -1){
                    intersected.push_back(std::make_pair(dist, i));
                }
            }
        }

        if(intersected.empty()){
            if(!verts_to_connect[verts]){
                connection_data.push_back(Slice_connect_data(
                        std::min(verts.first, verts.second),
                        std::max(verts.first, verts.second),
                        from,
                        to
                ));
            }
            verts_to_connect[verts] = true;
            connected[from] = true;
            connected[to] = true;
        }
        else{
            std::sort(intersected.begin(), intersected.end());

            intersected.insert(intersected.begin(), std::make_pair(0, from));
            intersected.insert(intersected.end(), std::make_pair(0, to));

            for(int i = 0; i < intersected.size() - 1; i++){
                connect.push(std::make_pair(intersected[i].second, intersected[i + 1].second));
            }

        }
    }


    std::vector < std::vector < Slice_connect_data > > connection_tree(slice.faces.size(), std::vector < Slice_connect_data > ());
    for(auto i : connection_data){
        //connect_outlines(slice, i.v0, i.v1);
        connection_tree[i.face0].push_back(Slice_connect_data(i.v0, i.v1, i.face0, i.face1));
        connection_tree[i.face1].push_back(Slice_connect_data(i.v1, i.v0, i.face1, i.face0));
    }
/*
    std::cout << "\n--------" << faces[faces.size() - 1] << "--------\n";
    for(int i = 0; i < connection_tree.size(); i++){
       std::cout << i << ": ";
       for(auto j : connection_tree[i]){
           std::cout << j.face1 << "(" << j.v0 << ", " << j.v1 << ") ";
       }
       std::cout << '\n';
    }
*/
    std::vector < bool > merged(slice.faces.size());
    return merge_outlines(slice, connection_tree, merged, faces[faces.size() - 1]);
}

Mesh slice_mesh(Mesh &mesh, double h, dvec2 border_st, dvec2 border_en){
    //std::cout << "\n-------------------\n";

    //std::vector < int > polygons_to_cut;
    std::vector < Slice_edge > intersections;
    std::vector < dvec3 > normals;

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
        // TODO: fix colision with parallel surface

        if(edge0 != -1){
            intersections.push_back(Slice_edge(i, edge0, edge1));
            normals.push_back(mesh.faces[i].n);
            //polygons_to_cut.push_back(i);
        }
    }

    if(intersections.size() == 0){
        Mesh empty_slice;
        return empty_slice;
    }

    std::vector < Slice_unique_vertex > all_verts;
    for(int i = 0; i < intersections.size(); i++){
        dvec3 v0 = point_on_height(mesh, intersections[i].poly_i, intersections[i].e0, h);
        dvec3 v1 = point_on_height(mesh, intersections[i].poly_i, intersections[i].e1, h);

        all_verts.push_back(Slice_unique_vertex(v0, i));
        all_verts.push_back(Slice_unique_vertex(v1, i));
    }

    std::sort(all_verts.begin(), all_verts.end(), vec3_unique_comp);

    Mesh slice;
    bool merged = false;
    std::vector < std::vector < int > > edge_groups;
    for(int i = 0; i < all_verts.size(); i++){
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

    std::vector < std::vector < int > > normal_indices(slice.vertices.size(), std::vector < int > ());
    std::vector < std::vector < int > > outline_graph(slice.vertices.size(), std::vector < int > ());
    for(int i = 0; i < edges.size(); i++){
        if (edges[i].size() != 2){
            throw std::runtime_error("Invalid mesh: unable to slice");
        }
        outline_graph[edges[i][0]].push_back(edges[i][1]);
        outline_graph[edges[i][1]].push_back(edges[i][0]);

        normal_indices[edges[i][0]].push_back(i);
        normal_indices[edges[i][1]].push_back(i);
    }

    std::vector < bool > used(slice.vertices.size());
    int st;
    for(int i = 0; i < slice.vertices.size(); i++){
        if (!used[i]){
            st = i;
            std::vector < int > outline_seq;
            while(!used[st] && !outline_graph[st].empty()){
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

    int outline_n = slice.faces.size() - 1;
    std::vector < std::vector < int > > inside(outline_n, std::vector < int > ());
    int layers_n = 0;
    for(int i = 0; i < outline_n; i++){
        inside[i] = outline_inside(slice, i, normals, normal_indices);
        if(layers_n < inside[i].size()) layers_n = inside[i].size();
    }
    layers_n++;

    std::vector < std::vector < int > > layers(layers_n, std::vector < int > ());
    for(int i = 0; i < outline_n; i++){
        layers[inside[i].size()].push_back(i);
    }
    layers[0].push_back(outline_n);

    std::vector < bool > connected(outline_n);
    std::unordered_map < int, std::vector < int > > connection_groups(outline_n);
    for(int i = 0; i < outline_n; i++){
        if(!connected[i]) {
            int layer = inside[i].size();

            if (layer % 2 != 0) {
                for (int per_find: layers[layer - 1]) {
                    for (int per_match: inside[i]) {
                        if (per_find == per_match) {
                            connection_groups[per_match].push_back(i);
                            connected[i] = true;
                            connected[per_match] = true;
                            break;
                        }
                    }
                }
            }
        }
    }

    Mesh slice_ready_to_triangulate;
    slice_ready_to_triangulate.vertices = slice.vertices;

    for(auto el : connection_groups){
        std::vector < int > to_connect = el.second;
        to_connect.push_back(el.first);

        slice_ready_to_triangulate.add_face(connect_group(slice, to_connect));
    }
/*
    std::cout << "\n-------------------\n";
    for(int i = 0; i < slice.faces[0].verts.size(); i++){
        dvec3 n = normals[normal_by_vertices(
            normal_indices,
            slice.faces[0].verts[i],
            slice.faces[0].verts[(i + 1) % slice.faces[0].verts.size()]
        )];
        std::cout << slice.faces[0].verts[i] << ' ' << slice.faces[0].verts[(i + 1) % slice.faces[0].verts.size()] << ": ";
        std::cout << n.x << ", " << n.y << ", " << n.z << '\n';
    }

    for(Face seq : slice.faces){
        for(int vert : seq.verts){
            std::cout << vert << " -> ";
        }
        std::cout << seq.verts[0] << '\n';
    }
    std::cout << '\n';


    std::cout << '\n';
    for(int i = 0; i < slice.vertices.size(); i++){
        std::cout << i << ": " << slice.vertices[i].x << ' ' << slice.vertices[i].y << ' ' <<slice.vertices[i].z;
        //for(int j = 0; j < edge_groups[i].size(); j++) std::cout << edge_groups[i][j] << ' ';
        std::cout << '\n';
    }
*/
    return slice_ready_to_triangulate;
}