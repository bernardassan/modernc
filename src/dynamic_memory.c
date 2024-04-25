#include "dynamic_memory.h"
#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>

circular *circular_init(circular *circle, size_t max_len) {
  assert(max_len > 0);

  *circle = (circular){
      .max_len = max_len,
  };
  return circle;
}

void circular_destroy(circular *circle) { free(circle); }

static size_t circular_getpos(circular const *const circle, size_t pos) {
  pos += circle->start;
  pos %= circle->max_len;
  return pos;
}

double *circular_element(circular *const circle, size_t pos) {
  assert(pos < circle->max_len);

  pos = circular_getpos(circle, pos);
  return &circle->tab[pos];
}

circular *circular_resize(circular *circle, size_t new_max_len) {
  assert(new_max_len > 0 && circle->max_len < new_max_len);
  assert(new_max_len != circle->max_len);

  size_t ostart = circular_getpos(circle, 0UL);
  size_t nstart = ostart;
  // double *otab = circle->tab;
  size_t len = circle->len;
  circular *resized_circle =
      realloc(circle, sizeof(circular) + sizeof(double[new_max_len]));
  if (!resized_circle) {
    err(EXIT_FAILURE, "%s", "couldn't resize the existing circle");
  }
  // TODO(verify-two-separate-chucks): verify whether you need the 2 separate
  // segments fix
  //  add content to the flex array and view the behaviour of realloc on the
  //  already existing array
  *resized_circle = (circular){
      .max_len = new_max_len, .start = nstart, .len = len,
      // .tab = {resized_circle->tab},
  };
  circle = resized_circle;
  return circle;
}

circular *circular_new(size_t len) {
  circular *new_circle = calloc(1UL, sizeof(circular) + sizeof(double[len]));
  if (!new_circle) {
    err(EXIT_FAILURE, "%s", "couldn't allocate a new circle");
  }
  return circular_init(new_circle, len);
}

int main() {
  size_t const length = {10};
  double *array = nullptr;
  printf("size of malloc is %zu\n", sizeof(typeof(array)[length]));
}
