#include <vector>
#include <stdio.h>

static int partition(std::vector<int>& vec, int p, int r) {
  // int x = vec[p];
  // int i = p-1;
  // int j = r;
  // while (true) {
  //   // идем справа-налево
  //   do {
  //     j = j - 1;
  //   } while (vec[j] <= x);
  //   // while (j > 0 && vec[j] <= x) {
  //   //   j--;
  //   // }
  //   // идема слева-направо
  //   do {
  //     i = i + 1;
  //   } while (vec[i] >= x);
  //   // while (vec[i] >= x) {
  //   //   i++;
  //   // }
  //   if (i < j) {
  //     // меняем местами a[i] и a[j]
  //     int c = vec[i];
  //     vec[i] = vec[j];
  //     vec[j] = c;
  //   } else {
  //     //Номер разделителя левой и правой части
  //     return j;
  //   }
  // }

  // int i = p - 1;
  // for (int j = p; j < r-1; j++) {
  //   if (vec[j] < x) {
  //     i++;
  //     std::swap(vec[i], vec[j]);
  //   }
  // }
  // std::swap(vec[i+1], vec[r-1]);

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

  // int x = vec[r-1];
  // int i = p;
  // int j = p;
  // while( i < r){
  //   if(vec[i] > x){
  //     i++;
  //   }
  //   else{
  //     std::swap(vec[i], vec[j]);
  //     i++;
  //     j++;
  //   }
  // }
  // return j-1;
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
