#include <vector>
#include <cstdlib>
#include <cstdio>

static void print_heap(std::vector<int>& vec, int size) {
  int k = 1;
  int m = 1;
  for (int i = 0; i < size; i++) {
    printf("%d ", vec[i]);
    if (i+1 == k) {
      printf("\n");
      k += m*2;
      m *= 2;
    }
  }
  printf("\n");
}

static void restore(std::vector<int>& vec, int size, int i) {
  int maxi = i;
  // индекс левого потомка
  int l = 2 * i + 1;
  // индекс правого потомка
  int r = 2 * i + 2;
  // ищем максимального потомка
  if (l < size && vec[l] > vec[maxi]) {
    maxi = l;
  }
  if (r < size && vec[r] > vec[maxi]) {
    maxi = r;
  }
  // если нарушается условие кучи(потомки должны всегда быть меньше родителя)
  // то нужно восстановить ветку
  if (maxi != i) {
    std::swap(vec[maxi], vec[i]);
    restore(vec, size, maxi);
  }
}

void heap_sort(std::vector<int>& vec, int size) {
  // создаем кучу
  for (int j = size/2-1; j >= 0; j--) {
      restore(vec, size, j);
  }
  for (int i = 0; i < size-1; i++) {
    // поскольку мы идеем дело с кучей, то макс элемент - первый элемент
    int maxi = 0;
    // ставим его в конец
    std::swap(vec[maxi], vec[size-1-i]);
    // восстанавливаем кучу(она теперь меньше)
    restore(vec, size-i-1, 0);
  }
}
