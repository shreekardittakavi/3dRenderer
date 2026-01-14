#include "openglErrorReporting.h"
#include "ScreenManager.h"

int main(int argc, char *argv[]) {
    ScreenManager manager;
    enableReportGlErrors();

    bool running = true;
    while (running) {
        running = manager.handle_sdl_events();
        // manager.populate_imgui();

        manager.clear_screen();
        manager.render_gl2d();
        // manager.render_imgui();

        manager.swap_window();
    }

    return 0;
}
