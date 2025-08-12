#include <assert.h>
#include <complex.h>
#include <inttypes.h>
#include <limits.h>
#include <stdbit.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

// From Section 2 - 5

typedef enum Weekends {
  Friday,
  Saturday,
  Sunday,
} Weekends;

enum corvid : uint8_t {
  chough,
  magpie,
  raven,
  jay,
  corvid_num,
};

// BUG: Takeaway 5.6.5
// NOTE: constexpr in macros with compound literals don't work as of
// clang 20.1.2
static constexpr char CORVID_NAME[corvid_num][8] = {
    [chough] = "chough",
    [raven] = "raven",
    [magpie] = "magpie",
    [jay] = "jay",
};

static void literals();
static void use_choose();
static void switching(Weekends const day);
extern void cMain();

void cMain() {
  for (size_t index = 0; index < corvid_num; ++index) {
    printf("Corvid %zu is the %s\n", index, CORVID_NAME[index]);
  }
  literals();
  use_choose();
  switching(Sunday);
}

typedef struct birds {
  char const *jay;
  char const *magpie;
  char const *raven;
  char const *chough;
} Birds;

static Birds const bird [[maybe_unused]] = {
    .jay = "jay",
    .magpie = "magpie",
    .raven = "raven",
    .chough = "chough",
};

static char const *const birdie [[maybe_unused]][corvid_num] = {
    [chough] = "chough",
    [raven] = "raven",
    [magpie] = "magpie",
    [jay] = "jay",
};

enum vehicle { bus, airplane, train, car, horse, feet };

static enum vehicle choose(char arg) {
  switch (arg) {
  case 'B':
    return bus;
  case 'A':
    return airplane;
  case 'T':
    return train;
  case 'C':
    return car;
  case 'H':
    return horse;
  default:
    return feet;
  }
}

static void use_choose() {
  assert(choose('H') == horse && choose('F') == feet);
}

static void switching(Weekends const day) {

  Weekends bestday = Friday;
  Weekends secondbest [[maybe_unused]] = Saturday;

  Weekends churchday = Sunday;

  bestday = churchday;
  secondbest = bestday;

  switch (day) {
  case Friday:
    printf("\nIt is friday\n");
    if (churchday == Friday) {
      secondbest = Friday;
    }
    break;
  case Saturday:
    printf("\nIts Saturday\n");
    return;
  case Sunday:
    printf("\nIts sunday,go to church\n");
    return;
  }
}

static void literals() {
  // clang-format off
  constexpr double π [[maybe_unused]] = 3.141'592'653'589'793'238'46;
  // clang-format on

  constexpr bool this_is [[maybe_unused]] = true;
  constexpr char char_a [[maybe_unused]] = 'a';

  auto const alongfloat [[maybe_unused]] = 10.0L;
  // cimagl for printing I
  auto const complex_literal [[maybe_unused]] = 0.5L + 0.5IL;
  double const complex num [[maybe_unused]] = 3.0;
  double const Carray [[maybe_unused]][] = {
      [0] = 6,
      [3] = 1,
  };

  int64_t const big = (-UINT64_C(1)) >> 1UL; // Same value as INT64_MAX
  uint32_t const n32 = 78;

#if __STDC_VERSION__ >= 202311L
#if defined(__clang__) && __clang_major__ >= 21
  printf("n is %w32u, and big is %w64d \n", n32, big);
#else
  printf("n is %" PRIu32 " , and big is %" PRIi64 "\n", n32, big);
#endif
#endif

#if __STDC_VERSION_STDBIT_H__
  unsigned _BitInt(3) ubitwidth3 = 7wbU; // values 0, ... , 3, ... ,7
  signed _BitInt(3) sbitwidth3 [[maybe_unused]] =
      3wb;                                      // values -4, ... , 0, ... , 3
  auto const abitwidth3 [[maybe_unused]] = 3WB; // same
  auto const uautobitwidth3 [[maybe_unused]] = ubitwidth3 + 1wbU;  // value 0
  constexpr unsigned _BitInt(3) max3u = (unsigned _BitInt(3)) - 1; // 0b 111
  constexpr unsigned _BitInt(4) max4u = (unsigned _BitInt(4)) - 1; // 0b 1111
  constexpr unsigned _BitInt(4) high4u [[maybe_unused]] =
      max4u - max3u;                                           // 0b 1000
  constexpr signed _BitInt(4) max4s = max3u;                   // 0b 0111
  constexpr signed _BitInt(4) min4s [[maybe_unused]] = ~max4s; // 0b 1000
#if defined(__clang__) && __clang_major__ >= 21
  printf("max3u is %w3d, and max4u is %w4d \n", max3u, max4u);
#endif

#endif
}
