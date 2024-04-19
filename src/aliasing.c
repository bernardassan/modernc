#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

static int square(int const num[restrict const static 1],
                  int const num2[restrict const static 1]) {
  return num[0] * num2[0];
}
// not recommended
static uint32_t swap_words1(uint32_t arg) {
  uint16_t *const sp = (uint16_t *)&arg;
  uint16_t hi = sp[0];
  uint16_t lo = sp[1];

  sp[1] = hi;
  sp[0] = lo;

  return (arg);
}

// https://cellperformance.beyond3d.com/articles/2006/06/understanding-strict-aliasing.html
// https://web.archive.org/web/20080107035604/http://www.cellperformance.com/mike_acton/2006/05/demystifying_the_restrict_keyw.html
// recommended
typedef union {
  uint32_t u32;
  uint16_t u16[2];
} U32;

static uint32_t swap_words_init(uint32_t arg) {
  U32 in;
  uint16_t lo;
  uint16_t hi;

  in.u32 = arg;
  hi = in.u16[0];
  lo = in.u16[1];
  in.u16[0] = lo;
  in.u16[1] = hi;

  return (in.u32);
}

// simplified swap_words (Recommended Type punning which allows aliasing)
static uint32_t swap_words(uint32_t arg) {
  U32 in = {.u32 = arg};
  U32 out = {.u16[0] = in.u16[1], .u16[1] = in.u16[0]};

  return (out.u32);
}

typedef union {
  uint16_t *sp;
  uint32_t *wp;
} U32P;

static uint32_t swap_words_p(uint32_t arg) {
  U32P in = {.wp = &arg};
  const uint16_t hi = in.sp[0];
  const uint16_t lo = in.sp[1];

  in.sp[0] = lo;
  in.sp[1] = hi;

  return (arg);
}

static size_t len(int array[static const 4]) {
  return sizeof(array) / sizeof array[0];
}

int main() {
  int val[1] = {1};
  square(val, val);
  int array[] = {1, 2, 3, 4, 5};
  printf("size in block scope %ld while parameter scope %ld\n",
         sizeof(array) / sizeof(int), len(array)); // pirnts 4 <-> 2

  char const *const p = "hello";
  char const q[] = "hello";
  assert(((void)"Size of q != char*", sizeof(q) != sizeof(char *)));
  assert(sizeof(p) == 8);
  assert(sizeof(q) == 6);

  char sz [[maybe_unused]][12] = {};

  // This is fine, p points to sz's first element's address
  char *p1 [[maybe_unused]] = sz;

  // Compiling error, Can't convert a pointer to 12 elements to a pointer to a
  // char char *p2 = &sz;

  // This is the correct way to create a pointer to an array
  char(*x [[maybe_unused]])[12] = &sz;

  // Compiling error, can't convert a pointer to 12 elements to a pointer to 10
  // elements char (*y)[10] = &sz; printf("%w32u",swap_words((uint32_t){30}));
  printf("%" PRIu32 "\n", swap_words((uint32_t){30}));
  printf("%" PRIu32 "\n", swap_words1((uint32_t){30}));
  printf("%" PRIu32 "\n", swap_words_p((uint32_t){30}));
}
