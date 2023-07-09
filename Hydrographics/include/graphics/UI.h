#ifndef N_BODY_PROBLEM_UI_H
#define N_BODY_PROBLEM_UI_H

#include <string>

#include "glm/glm.hpp"

using namespace glm;

class base_view{
public:
    int type;
    double scale;
    dvec2 pos;

    base_view(int _type, double _scale = 1, dvec2 _pos = dvec2());

    dvec2 coords(dvec3 obj_pos);
};

class window_surface{
public:
    int x, y, w, h;
    std::string name;

    window_surface(std::string _name);
    window_surface(int _width = 0, int _height = 0, int _x = 0, int _y = 0);

    bool inside(dvec2 pos);
    void change_size(dvec2 size, dvec2 pos = dvec2());
};
#endif //N_BODY_PROBLEM_UI_H
