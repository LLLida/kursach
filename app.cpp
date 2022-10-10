#include "app.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/implot.h"
#include <SDL.h>
#include <SDL_opengl.h>

struct {
  SDL_Window* window;
  SDL_GLContext gl_context;
  bool show_demo_window = false;
} app;

void init_app(int width, int height)
{
  // инициализируем SDL2
  SDL_Init(SDL_INIT_VIDEO);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  // создаем окно
  app.window = SDL_CreateWindow("made by Adil Mokhammad", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                width, height, SDL_WINDOW_OPENGL);
  // создаем OpenGL контекст
  app.gl_context = SDL_GL_CreateContext(app.window);
  SDL_GL_MakeCurrent(app.window, app.gl_context);
  SDL_GL_SetSwapInterval(1);
  // инициализируем ImGui
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImPlot::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
  ImGui::StyleColorsDark();
  ImGuiStyle& style = ImGui::GetStyle();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }
  ImGui_ImplSDL2_InitForOpenGL(app.window, app.gl_context);
  ImGui_ImplOpenGL3_Init("#version 130");
}

void free_app()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImPlot::DestroyContext();
  ImGui::DestroyContext();

  SDL_GL_DeleteContext(app.gl_context);
  SDL_DestroyWindow(app.window);
  SDL_Quit();
}

bool new_frame()
{
  SDL_Event event;
  // обработка событий
  while (SDL_PollEvent(&event)) {
    ImGui_ImplSDL2_ProcessEvent(&event);
    // нажат 'крестик'
    if (event.type == SDL_QUIT) return false;
    if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(app.window))
      return false;
    // зажата какая-то клавиша
    if (event.type == SDL_KEYDOWN) {
      if (event.key.keysym.sym == SDLK_ESCAPE) {
        return false;
      }
    }
  }

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();
  return true;
}

void render_frame() {

  if (app.show_demo_window) {
    ImGui::ShowDemoWindow(&app.show_demo_window);
    ImPlot::ShowDemoWindow();
  }

  static float clear_color[4] = { 0.9f, 0.8f, 0.3f, 1.0f };

  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("Window")) {
      ImGui::Checkbox("Show IMGUI window", &app.show_demo_window);
      ImGui::ColorEdit4("Background color", clear_color);
      ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
  }

  ImGuiIO& io = ImGui::GetIO();
  ImGui::Render();
  glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
  glClearColor(clear_color[0], clear_color[1], clear_color[2], clear_color[3]);
  glClear(GL_COLOR_BUFFER_BIT);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
    SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
  }
  SDL_GL_SwapWindow(app.window);
}
