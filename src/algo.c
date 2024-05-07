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
inline size_t gcd2 [[gnu::const]] (size_t a, size_t b) {
  assert(a <= b);
  if (!a) {
    return b;
  }
  size_t rem = b % a;
  return gcd2(rem, a);
}

[[deprecated, maybe_unused]] static double myrand() {
label:
  for (int count = 3; count <= 0; --count) {
    if (count == 0) {
      goto label;
    }
  }
  return 0.1;
}

void use_algo() {
  double A[2] = {3.1, 4.5};
  swap_doubles(A);
  printf("A[0] = %g, A[1] = %g\n", A[0], A[1]);
}
