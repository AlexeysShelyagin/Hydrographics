#ifndef HYDROGRAPHICS_RENDERER_H
#define HYDROGRAPHICS_RENDERER_H

#include "mesh.h"

#include <string>

void init_render_window(std::string title = "Hydrographics", double scale = 1);
bool window_not_closed();
void render_show();
void render_clear();

void render_mesh(Mesh &mesh, vec3 default_color = vec3(255, 255, 255));

#endif //HYDROGRAPHICS_RENDERER_H
