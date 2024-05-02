// INFO: https://www.davidpriver.com/ctemplates.html
// INFO: https://godbolt.org/z/WKEMMbnzK
// INFO: STC uses this concept thoroughly https://github.com/stclib/STC

// INFO: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3037.pdf
#include <stdio.h>
#include <stdlib.h>

// https://www.reddit.com/r/C_Programming/comments/w5hl80/comment/ih8jxi6/?context=3
#define Vec(T)                                                                 \
  struct Vec__##T {                                                            \
    T *at;                                                                     \
    size_t _len;                                                               \
  }

#define vec_push(a, v)                                                         \
  ((a)->at = realloc((a)->at, ++(a)->_len * sizeof *(a)->at),                  \
   (a)->at[(a)->_len - 1] = (v))

#define vec_len(a) ((a)._len)

void fill(Vec(int) * vec) {
  for (int i = 0; i < 10; i += 2) {
    vec_push(vec, i);
  }
}

int main() {
  Vec(int) x = {0}; // or = {} in C2x
  // pre C2x you'd need to typedef Vec(int) to make the pointers compatible and
  // use it for `x` and in fill:
  // --v
  fill(&x);
  for (size_t i = 0; i < vec_len(x); ++i) {
    printf("%d\n", x.at[i]);
  }
}

// https://www.reddit.com/r/C_Programming/comments/w5hl80/comment/ih8jxi6/?context=6
#define i_val int
#include "templates.h"

#define i_val                                                                  \
  struct Pair {                                                                \
    int a, b;                                                                  \
  }
#define i_tag pair
#include "templates.h"

// THIS WILL BE FINE IN C23
#define i_val int
#include "templates.h"

void fill_int(Vec_int *vec) {
  for (int i = 0; i < 10; i += 2) {
    Vec_int_push(vec, i);
  }
}

void templates_main() {
  Vec_int iv = {0}; // or = {} in C2x
  fill_int(&iv);

  for (size_t i = 0; i < iv.len; ++i) {
    printf("%d ", iv.at[i]);
  }
}
