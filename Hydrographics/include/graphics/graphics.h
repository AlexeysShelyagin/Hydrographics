#ifndef HYDROGRAPHICS_GRAPHICS_H
#define HYDROGRAPHICS_GRAPHICS_H

#include "UI.h"
#include <string>

#include "glm/glm.hpp"

using namespace glm;

void sf_init_window(int w, int h, std::string title, double scale);

bool sf_window_opened();
void sf_close_window();


std::string sf_window_event();

dvec2 pos_on_view(std::string view_name, dvec3 coord);

void draw_circle(std::string surf_name, dvec2 pos = dvec2());
void draw_text(std::string surf_name, std::string txt, dvec2 pos = dvec2());
void draw_line(std::string surf_name, dvec2 st = dvec2(), dvec2 en = dvec2(), dvec3 color = dvec3(255, 255, 255));
void sf_render_window();
void sf_clear_window();

#endif //HYDROGRAPHICS_GRAPHICS_H
