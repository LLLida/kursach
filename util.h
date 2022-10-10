#pragma once

#include <chrono>
#include <vector>
#include <cstdio>
#include <cstdlib>

#include "imgui/implot.h"
#include "imgui/imgui.h"

void insertion_sort(std::vector<int>& vec, int n);
void bubble_sort(std::vector<int>& vec, int n);
void quick_sort(std::vector<int>& vec, int n);
void choice_sort(std::vector<int>& vec, int n);
void heap_sort(std::vector<int>& vec, int n);

#define RESULT_SUCCESS 1
#define RESULT_ERROR_READING -1
#define RESULT_ERROR_WRITING -2
#define GREEN_COLOR ImVec4(0.1f, 1.0f, 0.1f, 1.0f)
#define RED_COLOR ImVec4(1.0f, 0.1f, 0.1f, 1.0f)

using sort_function_t = void(*)(std::vector<int>&, int);

struct SortBencmarkInfo {
  const char* name;
  sort_function_t func;
  // in ms
  std::vector<float> measurements;
  bool enabled = true;
};

std::vector<int> sort_sizes;

void generate_random_numbers(std::vector<int>& vec, int size, int max) {
  vec.resize(size);
  for (int& a: vec) {
    a = rand() % max;
  }
}

float measure_sort_time(std::vector<int>* vecs, int numvecs, sort_function_t sort, int size) {
  using namespace std::chrono;
  using milliseconds = duration<float, std::milli>;
  // начинаем отсчет
  auto start_time = steady_clock::now();
  // делаем сортировки
  for (int i = 0; i < numvecs; i++) {
    sort(vecs[i], size);
  }
  // заканчиваем отсчет
  auto end_time = steady_clock::now();
  auto elapsed = duration_cast<milliseconds>(end_time - start_time);
  float ms = elapsed.count() / (float)sort_sizes.size();
  return ms;
}

void write_vec(FILE* file, const std::vector<int>& vec, int n) {
  for (int i = 0; i < n; i++) {
    fprintf(file, "%d%c", vec[i], " \n"[i==n-1]);
  }
}

bool save_vector(const std::vector<int>& vec, const char* filename) {
  FILE* file = fopen(filename, "w");
  if (file == NULL) {
    return false;
  }
  write_vec(file, vec, vec.size());
  fprintf(file, "\n");
  fclose(file);
  return true;
}

bool read_vector(std::vector<int>& vec, const char* filename) {
  FILE* file = fopen(filename, "r");
  if (file == NULL) {
    return false;
  }
  vec.clear();
  int value;
  while (fscanf(file, "%d ", &value) != EOF) {
    vec.push_back(value);
  }
  fclose(file);
  return true;
}

bool save_benchmarks(const std::vector<SortBencmarkInfo>& benchmarks, const char* filename) {
  FILE* file = fopen(filename, "w");
  if (file == NULL) {
    return false;
  }
  fprintf(file, "Sorts:\n\n");
  for (auto& bench: benchmarks) {
    if (!bench.enabled)
      continue;
    fprintf(file, "%s:\n", bench.name);
    for (int i = 0; i < sort_sizes.size(); i++) {
      fprintf(file, "  %d -> %.4f ms\n", (int)sort_sizes[i], bench.measurements[i]);
    }
  }
  fclose(file);
  return true;
}

void save_benchmarks_button(const std::vector<SortBencmarkInfo>& benchmarks) {
  static char filename[32] = "info.txt";
  static int status = 0;
  ImGui::SetNextItemWidth(150);
  if (ImGui::InputText("Filename", filename, sizeof(filename)))
    status = 0;
  ImGui::SameLine();
  if (ImGui::Button("Save results")) {
    if (!save_benchmarks(benchmarks, filename)) {
      printf("ERROR: failed to open file '%s' for writing\n", filename);
      status = RESULT_ERROR_WRITING;
    } else {
      status = RESULT_SUCCESS;
    }
  }
  if (status == RESULT_SUCCESS) {
    ImGui::SameLine();
    ImGui::TextColored(GREEN_COLOR, "SAVED time results to file '%s'", filename);
  } else if (status == RESULT_ERROR_WRITING) {
    ImGui::SameLine();
    ImGui::TextColored(RED_COLOR, "FAILED to open file '%s' for writing", filename);
  }
}

void sort_user_vector_button(std::vector<int>& vec, const std::vector<SortBencmarkInfo>& benchmarks) {
  static int item_index = 0;
  static char read_filename[32] = "vector.txt";
  static char write_filename[32] = "vector.txt";
  static int status = 0;
  static float time = 0.0f;

  ImGui::Separator();
  std::vector<const char*> items(benchmarks.size());
  for (int i = 0; i < benchmarks.size(); i++)
    items[i] = benchmarks[i].name;

  ImGui::SetNextItemWidth(400);
  if (ImGui::BeginCombo("Sort method", items[item_index])) {
    for (int i = 0; i < items.size(); i++) {
      bool is_selected = (item_index == i);
      if (ImGui::Selectable(items[i], is_selected)) {
        item_index = i;
        status = 0;
      }
      if (is_selected)
        ImGui::SetItemDefaultFocus();
    }
    ImGui::EndCombo();
  }

  ImGui::SetNextItemWidth(150);
  if (ImGui::InputText("Read file", read_filename, sizeof(read_filename)))
    status = 0;
  ImGui::SameLine();
  ImGui::SetNextItemWidth(150);
  if (ImGui::InputText("Write file", write_filename, sizeof(write_filename)))
    status = 0;
  ImGui::SameLine();

  if (ImGui::Button("Sort")) {

    if (!read_vector(vec, read_filename)) {

      printf("ERROR: failed to open file '%s' for reading\n", read_filename);
      status = RESULT_ERROR_READING;

    } else {

      time = measure_sort_time(&vec, 1, benchmarks[item_index].func, vec.size());
      if (!save_vector(vec, write_filename)) {
        printf("ERROR: failed to open file '%s' for writing\n", write_filename);
        status = RESULT_ERROR_WRITING;
      } else {
        status = RESULT_SUCCESS;
      }

    }

  }

  switch (status) {
  case RESULT_SUCCESS:
    ImGui::SameLine();
    ImGui::TextColored(GREEN_COLOR, "SUCCESS sorted with '%s' in %.3f ms and saved to '%s'",
                       items[item_index], time, write_filename);
    break;
  case RESULT_ERROR_READING:
    ImGui::SameLine();
    ImGui::TextColored(RED_COLOR, "FAILED to open file '%s' for reading", read_filename);
    break;
  case RESULT_ERROR_WRITING:
    ImGui::SameLine();
    ImGui::TextColored(RED_COLOR, "FAILED to open file '%s' for writing", write_filename);
    break;
  }
}
