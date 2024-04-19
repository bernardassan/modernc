#include "dynamic_memory.h"
#include <stdio.h>

int main() {
  size_t length = 10;
  double *array = nullptr;
  printf("size of malloc is %zu\n", sizeof(typeof(array)[length]));
}
