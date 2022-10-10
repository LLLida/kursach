#include <vector>

void insertion_sort(std::vector<int>& vec, int n) {
  for (int i = 1; i < n; i++) {
    int j = i-1;
    int R = vec[i];
    while (j >= 0 && R <= vec[j]) {
      vec[j+1] = vec[j];
      j--;
    }
    vec[j+1] = R;
  }
}
