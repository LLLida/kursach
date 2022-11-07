#include <vector>
#include <stdio.h>

static int partition(std::vector<int>& vec, int p, int r) {
  int x = vec[p];
  int i = r-1;
  int j = r-1;
  while (i >= p) {
    if (vec[i] < x) {
      i--;
    } else {
      std::swap(vec[i], vec[j]);
      i--;
      j--;
    }
  }
  return j+1;
}

static void sort(std::vector<int>& vec, int p, int r) {
  if (p < r) {
    int q = partition(vec, p, r);
    // printf("%d\n", q);
    sort(vec, p, q);
    sort(vec, q+1, r);
  }
}

void quick_sort(std::vector<int>& vec, int size) {
  return sort(vec, 0, size);
}
