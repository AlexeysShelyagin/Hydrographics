#include "renderer.h"

#include "include/graphics/graphics.h"
#include <iostream>
#include "include/glm/vec3.hpp"

using namespace glm;

void init_render_window(std::string title, double scale){
    sf_init_window(1920, 1080, title, scale);
}

bool window_not_closed(){
    std::string event = sf_window_event();
    if(event == "space"){
        while(sf_window_event() != "space") {}
    }
    return sf_window_opened();
}

void render_show(){
    sf_render_window();
}

void render_clear(){
    sf_clear_window();
}

void draw_face(std::vector < dvec3 > poly, dvec3 color = dvec3(255, 255, 255)){
    int v_n = poly.size();
    for(int j = 0; j < v_n; j++){
        draw_line("top",
                  pos_on_view("top", poly[j]),
                  pos_on_view("top", poly[(j + 1) % v_n]),
                  color
        );
        draw_line("front",
                  pos_on_view("front", poly[j]),
                  pos_on_view("front", poly[(j + 1) % v_n]),
                  color
        );
        draw_line("side",
                  pos_on_view("side", poly[j]),
                  pos_on_view("side", poly[(j + 1) % v_n]),
                  color
        );
    }
}

void render_mesh(Mesh &mesh, dvec3 default_color, bool draw_indices){

    for(int i = 0; i < mesh.vertices.size(); i++) {
        //draw_circle("top", pos_on_view("top", mesh.vertices[i]));
        //draw_circle("front", pos_on_view("front", mesh.vertices[i]));
        //draw_circle("side", pos_on_view("side", mesh.vertices[i]));

        if(draw_indices) {
            draw_text("top", std::to_string(i), pos_on_view("top", mesh.vertices[i]));
            draw_text("front", std::to_string(i), pos_on_view("front", mesh.vertices[i]));
            draw_text("side", std::to_string(i), pos_on_view("side", mesh.vertices[i]));
        }
    }

    for(int i = 0; i < mesh.faces.size(); i++){
        draw_face(mesh.face_vertices(i), default_color);

        draw_text("top", std::to_string(i), pos_on_view("top", mesh.vertices[mesh.faces[i].verts[0]]));
    }
    for(auto s : mesh.selected){
        if(s.second) {
            draw_face(mesh.face_vertices(s.first), dvec3(255, 100, 100));
        }
    }
}