#include "graphics.h"
#include "UI.h"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>

#define FONT_HEIGHT 14

using namespace sf;

Font font;

base_view top(0);
base_view front(1);
base_view side(2);

window_surface screen;
window_surface surf_top;
window_surface surf_front;
window_surface surf_side;

std::map < std::string, window_surface* > surfaces;

RenderWindow* window;

dvec2 pressed_pos;
int mouse_window;

dvec2 window_size;

bool window_opened = false;

Color HSV(double H, double S, double V){
    S /= 100;
    V /= 100;

    double f, p, t, q;
    double r, g, b;

    if(H >= 360) H = 0;
    H = H / 60;
    int i = (int) trunc(H);
    f = H - i;

    p = V * (1.0 - S);
    q = V * (1.0 - (S * f));
    t = V * (1.0 - (S * (1.0 - f)));

    switch (i) {
        case 0:
            r = V;
            g = t;
            b = p;
            break;
        case 1:
            r = q;
            g = V;
            b = p;
            break;
        case 2:
            r = p;
            g = V;
            b = t;
            break;
        case 3:
            r = p;
            g = q;
            b = V;
            break;
        case 4:
            r = t;
            g = p;
            b = V;
            break;
        default:
            r = V;
            g = p;
            b = q;
    }

    int R = r * 255;
    int G = g * 255;
    int B = b * 255;

    return Color(R, G, B);
}

void sf_init_window(int w, int h, std::string title, double scale){
    font.loadFromFile("../Fonts/consolas.ttf");

    screen = window_surface(w, h);
    surf_top = window_surface(screen.w / 2, screen.h / 2, screen.w / 2, 0);
    surf_front = window_surface(screen.w / 2, screen.h / 2, screen.w / 2, screen.h / 2);
    surf_side = window_surface(screen.w / 2, screen.h / 2,0, screen.h / 2);

    surfaces["screen"] = &screen;
    surfaces["top"] = &surf_top;
    surfaces["front"] = &surf_front;
    surfaces["side"] = &surf_side;

    window_size = dvec2(w, h);

    top.scale = scale;
    front.scale = scale;
    side.scale = scale;

    window_opened = true;
    window = new RenderWindow(VideoMode(w, h), title);
}

bool sf_window_opened(){
    return window_opened;
}

void sf_close_window(){
    window -> close();
    delete window;
    window_opened = false;
}

std::string sf_window_event(){
    std::string responce = "None";
    dvec2 mouse_pos(Mouse::getPosition(*window).x, Mouse::getPosition(*window).y);

    Event event;
    while (window -> pollEvent(event)){
        if (event.type == Event::Closed) sf_close_window();

        if (event.type == Event::MouseWheelScrolled){
            if (event.mouseWheelScroll.wheel == Mouse::VerticalWheel) {
                if (surf_top.inside(mouse_pos) || surf_front.inside(mouse_pos) || surf_side.inside(mouse_pos)) {
                    double scale = top.scale * (1.001 + event.mouseWheelScroll.delta);
                    top.scale = front.scale = side.scale = scale;
                }
            }
            responce = "wheel_scrolled";
        }

        if(event.type == Event::KeyPressed){
            if(event.key.code == Keyboard::Space){
                responce = "space";
            }
        }

        if (event.type == Event::MouseButtonPressed){
            pressed_pos = mouse_pos;

            if(surf_top.inside(mouse_pos)) mouse_window = 1;
            if(surf_front.inside(mouse_pos)) mouse_window = 2;
            if(surf_side.inside(mouse_pos)) mouse_window = 3;

        }
        if (event.type == Event::MouseButtonReleased){
            mouse_window = -1;
        }
        if (Mouse::isButtonPressed(sf::Mouse::Left)){
            dvec2 mouse_delta = mouse_pos - pressed_pos;
            pressed_pos = mouse_pos;

            if(mouse_window == 1) top.pos += mouse_delta;
            if(mouse_window == 2) front.pos += mouse_delta;
            if(mouse_window == 3) side.pos += mouse_delta;
        }
    }

    return responce;
}

dvec2 pos_on_view(std::string view_name, dvec3 coord){
    if(view_name == "top") return top.coords(coord);
    if(view_name == "front") return front.coords(coord);
    if(view_name == "side") return side.coords(coord);
}

void draw_circle(std::string surf_name, dvec2 pos){
    window_surface* surface = surfaces[surf_name];

    CircleShape circle(1);

    int half_w = surface -> w / 2, half_h = surface -> h / 2;

    int screen_x = 0, screen_y = 0, num = 0;
    if(pos.x > -half_w && pos.y > -half_h && pos.x < half_w && pos.y < half_h) {
        screen_x = pos.x + surface -> x + half_w;
        screen_y = pos.y + surface -> y + half_h;
        circle.setPosition(Vector2f(screen_x, screen_y));
    }
    circle.setFillColor(Color(HSV(5, int(55 * exp(-num * 0.5)) + 10, 95)));

    window -> draw(circle);
}

void draw_text(std::string surf_name, std::string txt, dvec2 pos){
    window_surface* surface = surfaces[surf_name];

    Text text;
    text.setFont(font);
    text.setCharacterSize(FONT_HEIGHT);

    int half_w = surface -> w / 2, half_h = surface -> h / 2;

    text.setString(txt);
    text.setPosition(Vector2f(pos.x + surface -> x + half_w, pos.y + surface -> y + half_h));
    window -> draw(text);
}

void draw_line(std::string surf_name, dvec2 st, dvec2 en, dvec3 color){
    window_surface* surface = surfaces[surf_name];

    int half_w = surface -> w / 2, half_h = surface -> h / 2;

    Vertex line[] = {
            Vertex(Vector2f(st.x + surface -> x + half_w, st.y + surface -> y + half_h), Color(color.x, color.y, color.z)),
            Vertex(Vector2f(en.x + surface -> x + half_w, en.y + surface -> y + half_h), Color(color.x, color.y, color.z)),
    };
    window -> draw(line, 2, Lines);
}

void sf_render_window(){
    window -> display();
}

void sf_clear_window(){
    window -> clear();
}