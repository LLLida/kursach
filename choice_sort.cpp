#include <vector>

void choice_sort(std::vector<int>& vec, int size) {
  for (int i = 0; i < size; i++) {
    int maxi = 0;
    for (int j = 0; j < size-i; j++) {
      if (vec[j] > vec[maxi]) {
        maxi = j;
      }
    }
    std::swap(vec[maxi], vec[size-i-1]);
  }
}
