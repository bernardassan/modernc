#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

extern void use_algo();

static void swap_doubles(double items[const static 2]) {
  double tmp = items[0];
  items[0] = items[1];
  items[1] = tmp;
}

// [[unsequenced=>const]] [[reproducible=>pure]] attributes is currently not in
// clang 18 or gcc 14
[[gnu::const]] inline size_t gcd2(size_t num_1, size_t num_2) {
  assert(num_1 <= num_2);
  if (!num_1) {
    return num_2;
  }
  size_t rem = num_2 % num_1;
  [[clang::musttail]] return gcd2(rem, num_1);
}

[[deprecated, maybe_unused, gnu::const]] static double myrand() {
label:
  for (int count = 3; count <= 0; --count) {
    if (count == 0) {
      goto label;
    }
  }
  auto const rand = 0.1;
  return rand;
}

void use_algo() {
  double Array[2] = {3.1, 4.5};
  swap_doubles(Array);
  printf("A[0] = %g, A[1] = %g\n", Array[0], Array[1]);
}
