#include "model_loader.h"

#define TINYOBJLOADER_IMPLEMENTATION

#include <iostream>
#include "tiny_obj_loader.h"
#include "include/glm/vec3.hpp"

using namespace glm;

Mesh load_model(std::string filepath, std::string filename){
    std::cout << "Loading " << filename << "... ";

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warn;
    std::string err;

    bool ret = tinyobj::LoadObj(
        &attrib, &shapes, &materials,
        &warn, &err,
        (filepath + filename).c_str(), filepath.c_str()
    );

    if (!warn.empty()) std::cout << warn << std::endl;
    if (!err.empty()) std::cerr << err << std::endl;
    if (!ret) throw std::runtime_error(warn + err);

    Mesh model;

    for(int i = 0; i < attrib.vertices.size(); i += 3){
        model.add_vertex(dvec3(
            attrib.vertices[i + 0],
            attrib.vertices[i + 1],
            attrib.vertices[i + 2]
        ));
    }

    // Loop over shapes
    for (const auto& shape : shapes) {
        // check if mesh not triangulated
        for(const auto& num : shape.mesh.num_face_vertices){
            if(num != 3){
                throw std::runtime_error("mesh is not triangulated");

                // TODO: add triangulation
            }
        }

        for (uint64_t vertex_i = 0; vertex_i < shape.mesh.indices.size(); vertex_i += 3) {
            uint64_t normal_i = shape.mesh.indices[vertex_i].normal_index;
            dvec3 n = dvec3(
                attrib.normals[normal_i + 0],
                attrib.normals[normal_i + 1],
                attrib.normals[normal_i + 2]
            );
            model.add_face(
                    shape.mesh.indices[vertex_i + 0].vertex_index,
                    shape.mesh.indices[vertex_i + 1].vertex_index,
                    shape.mesh.indices[vertex_i + 2].vertex_index,
                    n
            );
        }
    }

    std::cout << "Load\n";
    return model;
}