#ifndef HYDROGRAPHICS_RENDERER_H
#define HYDROGRAPHICS_RENDERER_H

#include "mesh.h"

#include <string>

void init_render_window(std::string title = "Hydrographics", double scale = 1);
bool window_not_closed();
void render_show();
void render_clear();

void render_mesh(Mesh &mesh, dvec3 default_color = dvec3(255, 255, 255), bool draw_indices = false, bool draw_face_number = false);

#endif //HYDROGRAPHICS_RENDERER_H
