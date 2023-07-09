#include "UI.h"

#include <iostream>

base_view::base_view(int _type, double _scale, dvec2 _pos) {
    type = _type;
    scale = _scale;
    pos = _pos;
}

dvec2 base_view::coords(dvec3 obj_pos) {
    switch (type) {
        case 0: return dvec2(obj_pos.x, obj_pos.y) * scale + pos;
        case 1: return dvec2(obj_pos.x, obj_pos.z) * scale + pos;
        case 2: return dvec2(obj_pos.y, obj_pos.z) * scale + pos;
    }
    return dvec2();
}

window_surface::window_surface(std::string _name) {
    name = _name;
}

window_surface::window_surface(int _width, int _height, int _x, int _y) {
    w = _width;
    h = _height;
    x = _x;
    y = _y;
}

bool window_surface::inside(dvec2 pos) {
    int dx = pos.x - x, dy = pos.y - y;

    return (dx > 0 && dx < w && dy > 0 && dy < h);
}

void window_surface::change_size(dvec2 size, dvec2 pos) {
    w += size.x;
    h += size.y;
    x += pos.x;
    y += pos.y;
}