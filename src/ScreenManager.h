#pragma once

#include "imgui.h"
#include <SDL2/SDL.h>
#include <gl2d/gl2d.h>
#include <glad/glad.h>
#include <string>

class ScreenManager {
public:
    ScreenManager();
    ~ScreenManager();

    bool handle_sdl_events();
    void populate_imgui();
    void render_gl2d();
    void render_imgui();
    void clear_screen();
    void swap_window();

private:
    void _init_sdl_window(std::string title, int x, int y, int width, int height);
    void _init_opengl();
    void _init_imgui();
    void _init_gl2d();

private:
    SDL_Window *window;
    SDL_GLContext glContext;
    gl2d::Renderer2D renderer2d;
};
