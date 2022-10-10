cmake_minimum_required(VERSION 3.0)
project(LAB1)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

add_library(imgui STATIC
  imgui/imgui.cpp
  imgui/imgui_demo.cpp
  imgui/imgui_draw.cpp
  imgui/imgui_impl_opengl3.cpp
  imgui/imgui_impl_sdl.cpp
  imgui/imgui_tables.cpp
  imgui/imgui_widgets.cpp
  imgui/implot.cpp
  imgui/implot_demo.cpp
  imgui/implot_items.cpp)
target_link_libraries(imgui SDL2::SDL2)

add_executable(lab1
  lab1.cpp
  app.cpp
  insertion_sort.cpp
  quick_sort.cpp)
target_link_libraries(lab1 imgui SDL2::SDL2 ${OPENGL_LIBRARY})

add_executable(kursach
  kursach.cpp
  app.cpp
  insertion_sort.cpp
  bubble_sort.cpp
  quick_sort.cpp
  choice_sort.cpp
  heap_sort.cpp)
target_link_libraries(kursach imgui SDL2::SDL2 ${OPENGL_LIBRARY})
