#include "ScreenManager.h"

#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl2.h"
#include "imguiThemes.h"
#include <iostream>

ScreenManager::ScreenManager() {
    std::string title = "Test Window";
    _init_sdl_window(title, 100, 100, 680, 480);
    _init_opengl();
    _init_imgui();
    _init_gl2d();
}

ScreenManager::~ScreenManager() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool ScreenManager::handle_sdl_events() {
    int w = 0, h = 0;
    SDL_GetWindowSize(window, &w, &h);

    renderer2d.updateWindowMetrics(w, h);
    glViewport(0, 0, w, h);

    bool running = true;
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT) {
            running = false;
        }

        if (event.type == SDL_WINDOWEVENT) {
            if (event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window)) {
                running = false;
            }
        }
    }

    return running;
}

void ScreenManager::render_gl2d() {
    renderer2d.renderRectangle({100, 100, 100, 100}, Colors_Orange);
    renderer2d.flush();
}

void ScreenManager::populate_imgui() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(window);
    ImGui::NewFrame();
    // Create a docking space
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

    // Example ImGui window
    ImGui::Begin("Test Window");

    ImGui::Text("Hello, world!");
    ImGui::Button("I am Pressy the button!");
    float values[5] = {0.5, 0.4, 0.3, 0.56, 0.46};
    ImGui::PlotHistogram("I am a plot!", values, 5);

    ImGui::End();
}

void ScreenManager::render_imgui() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        SDL_Window *backup_current_window = SDL_GL_GetCurrentWindow();
        SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
    }
}

void ScreenManager::clear_screen() {
    glClear(GL_COLOR_BUFFER_BIT);
}

void ScreenManager::swap_window() {
    SDL_GL_SwapWindow(window);
}

void ScreenManager::_init_sdl_window(std::string title, int x, int y, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    window = SDL_CreateWindow(title.c_str(), x, y, width, height,
                              SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

    if (window == nullptr) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return;
    }
}

void ScreenManager::_init_opengl() {
    glContext = SDL_GL_CreateContext(window);
    if (glContext == nullptr) {
        std::cerr << "SDL_GL_CreateContext Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }
}

void ScreenManager::_init_imgui() {

    ImGui::CreateContext();

    imguiThemes::yellow();

    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGuiStyle &style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        // style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 0.f;
        style.Colors[ImGuiCol_DockingEmptyBg].w = 0.f;
    }

    ImGui_ImplSDL2_InitForOpenGL(window, glContext);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void ScreenManager::_init_gl2d() {
    gl2d::init();
    renderer2d.create();
}
