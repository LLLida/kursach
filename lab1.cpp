#include <cstring>

#include "imgui/implot.h"
#include "imgui/imgui.h"

#include "app.h"
#include "util.h"

#define VEC_SIZE 1000
#define MAX_NUM 5000
#define WINDOW_SIZE_X 1536
#define WINDOW_SIZE_Y 1024

const std::vector<int> default_sort_sizes = { 50, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000 };

bool regenerate_vector_for_every_size = true;
int sort_times = 10;
std::vector<int> vector;
std::vector<SortBencmarkInfo> benchmarks;

void do_benchmark(SortBencmarkInfo& benchmark);
void do_benchmark_custom(SortBencmarkInfo& benchmark, bool overwrite);

int main(int argc, char** argv) {

  init_app(WINDOW_SIZE_X, WINDOW_SIZE_Y);

  sort_sizes = default_sort_sizes;

  benchmarks.push_back({ "Insertion sort", insertion_sort, {} });
  benchmarks.push_back({ "Quick sort", quick_sort, {} });
  for (auto& bench: benchmarks) {
    do_benchmark(bench);
  }

  while (true) {
    if (!new_frame())
      break;

    ImGuiWindowFlags flags = /*ImGuiWindowFlags_NoDecoration|*/ ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);

    ImGui::Begin("Sorts. Mokhammad Adil UPM-211", NULL, flags);

    bool redo_benchmarks = false;
    if (ImGui::Button("Recalculate"))
      redo_benchmarks = true;
    if (ImGui::Checkbox("Regenerate vector for every size", &regenerate_vector_for_every_size))
      redo_benchmarks = true;
    if (ImGui::InputInt("Number of sort iterations", &sort_times))
      redo_benchmarks = true;
    if (redo_benchmarks) {
      for (auto& bench: benchmarks) {
        do_benchmark(bench);
      }
    }
    ImGui::Separator();
    if (ImPlot::BeginPlot("Time")) {
      std::vector<float> fsizes(sort_sizes.size());
      for (int i = 0; i < sort_sizes.size(); i++) {
        fsizes[i] = (float)sort_sizes[i];
      }
      ImPlot::SetupAxes("Vector size", "Time in ms");
      for (auto& benchmark: benchmarks) {
        ImPlot::PlotLine(benchmark.name, fsizes.data(), benchmark.measurements.data(), sort_sizes.size());
      }
      ImPlot::EndPlot();
    }
    ImGui::Separator();

    save_benchmarks_button(benchmarks);
    sort_user_vector_button(vector, benchmarks);

    ImGui::End();

    render_frame();
  }

  free_app();
  return 0;
}

void do_benchmark(SortBencmarkInfo& benchmark) {
  benchmark.measurements.clear();
  sort_sizes = default_sort_sizes;
  std::vector<std::vector<int>> vecs(sort_times);
  if (!regenerate_vector_for_every_size) {
    generate_random_numbers(vector, VEC_SIZE, MAX_NUM);
    for (auto& vec: vecs) {
      vec = vector;
    }
  }
  for (int size: sort_sizes) {
    if (regenerate_vector_for_every_size) {
      generate_random_numbers(vector, size, MAX_NUM);
      for (auto& vec: vecs) {
        vec = vector;
      }
    }
    float time = measure_sort_time(vecs.data(), vecs.size(), benchmark.func, size);
    benchmark.measurements.push_back(time);
  }
  // vector = std::move(vecs[0]);
}

void do_benchmark_custom(SortBencmarkInfo& benchmark, bool overwrite) {
  benchmark.measurements.clear();
  std::vector<std::vector<int>> vecs(sort_times);
  if (!regenerate_vector_for_every_size) {
    for (auto& vec: vecs) {
      vec = vector;
    }
  }
  sort_sizes.clear();
  for (size_t i = 0; i < default_sort_sizes.size(); i++) {
    sort_sizes.push_back(vector.size() * (i+1) / default_sort_sizes.size());
  }
  for (int size: sort_sizes) {
    if (size > vector.size())
      break;
    if (regenerate_vector_for_every_size) {
      for (auto& vec: vecs) {
        vec = vector;
      }
    }
    float time = measure_sort_time(vecs.data(), vecs.size(), benchmark.func, size);
    benchmark.measurements.push_back(time);
  }
  if (overwrite) {
    vector = std::move(vecs[0]);
  }
}
