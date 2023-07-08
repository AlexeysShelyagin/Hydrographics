#ifndef HYDROGRAPHICS_GRAPHICS_H
#define HYDROGRAPHICS_GRAPHICS_H

#include "UI.h"
#include <string>

void sf_init_window(int w, int h, std::string title, double scale);

bool sf_window_opened();
void sf_close_window();


std::string sf_window_event();

vec2 pos_on_view(std::string view_name, vec3 coord);

void draw_circle(std::string surf_name, vec2 pos = vec2());
void draw_text(std::string surf_name, std::string txt, int x = 0, int y = 0);
void draw_line(std::string surf_name, vec2 st = vec2(), vec2 en = vec2(), vec3 color = vec3(255, 255, 255));
void sf_render_window();
void sf_clear_window();

#endif //HYDROGRAPHICS_GRAPHICS_H
