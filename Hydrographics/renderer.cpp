#include "renderer.h"

#include "include/graphics/graphics.h"
#include <iostream>

void init_render_window(std::string title, double scale){
    sf_init_window(1920, 1080, title, scale);
}

bool window_not_closed(){
    std::string event = sf_window_event();
    //if(event != "None") sf_render_window();
    return sf_window_opened();
}

void render_show(){
    sf_render_window();
}

void render_clear(){
    sf_clear_window();
}

void draw_polygon(vec3 poly[3], vec3 color = vec3(255, 255, 255)){
    for(int j = 0; j < 3; j++){
        draw_line("top",
                  pos_on_view("top", poly[j]),
                  pos_on_view("top", poly[(j + 1) % 3]),
                  color
        );
        draw_line("front",
                  pos_on_view("front", poly[j]),
                  pos_on_view("front", poly[(j + 1) % 3]),
                  color
        );
        draw_line("side",
                  pos_on_view("side", poly[j]),
                  pos_on_view("side", poly[(j + 1) % 3]),
                  color
        );
    }
}

void render_mesh(Mesh &mesh, vec3 default_color){
    /*
    for(int i = 0; i < mesh.vertices.size(); i++) {
        draw_circle("top", pos_on_view("top", mesh.vertices[i]));
        draw_circle("front", pos_on_view("front", mesh.vertices[i]));
        draw_circle("side", pos_on_view("side", mesh.vertices[i]));
    }
     */

    for(int i = 0; i < mesh.polygons.size(); i++){
        vec3 poly[3] = {
                mesh.vertices[mesh.polygons[i].v0],
                mesh.vertices[mesh.polygons[i].v1],
                mesh.vertices[mesh.polygons[i].v2]
        };
        draw_polygon(poly, default_color);
    }
    for(auto s : mesh.selected){
        if(s.second) {
            vec3 poly[3] = {
                    mesh.vertices[mesh.polygons[s.first].v0],
                    mesh.vertices[mesh.polygons[s.first].v1],
                    mesh.vertices[mesh.polygons[s.first].v2]
            };
            draw_polygon(poly, vec3(255, 100, 100));
        }
    }
}