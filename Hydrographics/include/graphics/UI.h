#ifndef N_BODY_PROBLEM_UI_H
#define N_BODY_PROBLEM_UI_H

#include <string>

#include "vectors.h"

class base_view{
public:
    int type;
    double scale;
    vec2 pos;

    base_view(int _type, double _scale = 1, vec2 _pos = vec2());

    vec2 coords(vec3 obj_pos);
};

class window_surface{
public:
    int x, y, w, h;
    std::string name;

    window_surface(std::string _name);
    window_surface(int _width = 0, int _height = 0, int _x = 0, int _y = 0);

    bool inside(vec2 pos);
    void change_size(vec2 size, vec2 pos = vec2());
};
#endif //N_BODY_PROBLEM_UI_H
