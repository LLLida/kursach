#include <vector>
#include <cstdio>
#include <cstdlib>
#include <chrono>

#include "imgui/implot.h"
#include "imgui/imgui.h"

#include "app.h"
#include "util.h"

#define MAX_NUM 5000
#define WINDOW_SIZE_X 1536
#define WINDOW_SIZE_Y 1024
#define GREEN_COLOR ImVec4(0.1f, 1.0f, 0.1f, 1.0f)
#define RED_COLOR ImVec4(1.0f, 0.1f, 0.1f, 1.0f)

bool regenerate_vector_for_every_size = false;
std::vector<int> vector;
std::vector<SortBencmarkInfo> benchmarks;
// std::vector<float> sort_sizes = { 300, 500, 750 };

void do_benchmark(SortBencmarkInfo& benchmark);
void update_benchmarks();
void edit_vec_size();

int main(int argc, char** argv) {

  init_app(WINDOW_SIZE_X, WINDOW_SIZE_Y);

  sort_sizes = { 400, 500, 600, 700, 800, 1000, 2000, 3000, 4000, 5000 };

  benchmarks.push_back({ "Insertion sort", insertion_sort, {} });
  benchmarks.push_back({ "Bubble sort", bubble_sort, {} });
  benchmarks.push_back({ "Quick sort", quick_sort, {} });
  benchmarks.push_back({ "Choice sort", choice_sort, {} });
  benchmarks.push_back({ "Heap sort",  heap_sort, {} });
  generate_random_numbers(vector, sort_sizes.back(), MAX_NUM);
  update_benchmarks();

  while (true) {
    if (!new_frame())
      break;

    ImGuiWindowFlags flags = /*ImGuiWindowFlags_NoDecoration|*/ ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);

    ImGui::Begin("Kursovaya. Mokhammad Adil UPM-211", NULL, flags);

    ImGui::Text("Enable/disable sorting algorithms:");
    for (auto& bench: benchmarks) {
      if (ImGui::Checkbox(bench.name, &bench.enabled) && bench.enabled)
        do_benchmark(bench);
    }

    edit_vec_size();

    bool redo_benchmarks = false;
    if (ImGui::Button("Recalculate with same vector"))
      redo_benchmarks = true;
    if (ImGui::Button("Regenerate")) {
      redo_benchmarks = true;
      generate_random_numbers(vector, sort_sizes.back(), MAX_NUM);
    }
    if (redo_benchmarks) {
      update_benchmarks();
    }

    if (ImPlot::BeginPlot("Time")) {
      std::vector<float> fsizes(sort_sizes.size());
      for (int i = 0; i < sort_sizes.size(); i++) {
        fsizes[i] = (float)sort_sizes[i];
      }
      ImPlot::SetupAxes("Vector size", "Time in ms");
      for (auto& benchmark: benchmarks) {
        if (benchmark.enabled)
          ImPlot::PlotLine(benchmark.name,
                           fsizes.data(), benchmark.measurements.data(), sort_sizes.size());
      }
      ImPlot::EndPlot();
    }

    save_benchmarks_button(benchmarks);
    sort_user_vector_button(vector, benchmarks);

    ImGui::End();

    render_frame();
  }

  free_app();

  return 0;
}

void generate_random_numbers(std::vector<int>& vec, int size) {
  vec.resize(size);
  for (int& a: vec) {
    a = rand() % MAX_NUM;
  }
}

void do_benchmark(SortBencmarkInfo& benchmark) {
  benchmark.measurements.clear();
  std::vector<int> temp(vector.size());
  for (int size: sort_sizes) {
    memcpy(temp.data(), vector.data(), size*sizeof(int));
    float time = measure_sort_time(&temp, 1, benchmark.func, size);
    benchmark.measurements.push_back(time);
  }
}

void update_benchmarks() {
  for (auto& bench: benchmarks) {
    if (bench.enabled)
      do_benchmark(bench);
  }
}

void edit_vec_size() {
  ImGui::Separator();
  ImGui::Text("WARNING: passing big N might freeze the program, be sure to disable slow algorithms");
  static int max_size = sort_sizes.back();
  if (ImGui::InputInt("Sort size", &max_size, 0)) {
    sort_sizes.back() = max_size;
    for (int i = sort_sizes.size()-2; i >= 0; i--) {
      sort_sizes[i] = sort_sizes[i+1]-max_size/10;
    }
    generate_random_numbers(vector, sort_sizes.back(), MAX_NUM);
    update_benchmarks();
  }
  ImGui::Separator();
}
