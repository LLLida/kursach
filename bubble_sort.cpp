#include <vector>

void bubble_sort(std::vector<int>& vec, int size) {
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size-i-1; j++) {
      if (vec[j] > vec[j+1]) {
        std::swap(vec[j], vec[j+1]);
      }
    }
  }
}
