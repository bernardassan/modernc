#include <complex.h>
#include <stdbit.h>
#include <stdint.h>
#include <stdio.h>

#ifndef __STDC_VERSION_STDBIT_H__
#error "stdbit.h is required for this module"
#endif

[[maybe_unused]] static void printing() {
  constexpr double π [[maybe_unused]] = 3.141'592'653'589'793'238'46;
  constexpr bool this_is [[maybe_unused]] = true;
  constexpr char char_a [[maybe_unused]] = 'a';

  uint32_t const n32 = 78;
  int64_t const big = (-UINT64_C(1)) >> 1UL; // Same value as INT64_MAX
  auto const alongfloat [[maybe_unused]] = 10.0L;
  // cimagl for printing I
  auto const complex_long [[maybe_unused]] = creall(3.0) + I;
  double const complex num [[maybe_unused]] = 3.0;
  double const Carray [[maybe_unused]][] = {
      [0] = 6,
      [3] = 1,
  };

#if __STDC_VERSION__ >= 202311L
  printf("n is %w32u , and big is %w64d \n", n32, big);
#else
  printf("n is %u , and big is %li\n", n32, big);
#endif
  // auto si = (CORVID_NAME(raven));

#if __STDC_VERSION_STDBIT_H__
  // unsigned _BitInt(3) u3 = 7wbu; // values 0, ... , 3, ... ,7
  // signed _BitInt(3) u3 = 3wb;                  // values -4, ... , 0, ... , 3
  // _BitInt(3) n3 = 3WB;                         // same
  auto const u0 [[maybe_unused]] = +1wbU; // value 0
  // constexpr unsigned _BitInt(3) max3u = -1wbu; // 0b 111
  // constexpr unsigned _BitInt(4) max4u = -1wbu; // 0b 1111
  // constexpr unsigned _BitInt(4) high4u = max4u - max3u; // 0b 1000
  // constexpr signed _BitInt(4) max4s = max3u;            // 0b 0111
  // constexpr signed _BitInt(4) min4s = ~max4s;           // 0b 1000
  // printf("max3u is %" PRIu8 ", max4u is %b", max3u, max4u);
#endif
}

static void circle(double diameter) {
  double const PI = 3.14'159'265;
  double const radius = diameter / 2.0;
  [[maybe_unused]] double const hex = 0x101'010;
  double const circumference = 2.0 * PI * radius;
  double const area = PI * radius * radius;
  printf("\nThe circumference is %-3.2f\n", circumference);
  printf("\nArea of table is %+3.4f\n", area);
}

enum { INCHE_PER_FOOT = 12 };

[[maybe_unused]] static void fmt_sal() {
  int const salary = 10'000ULL;
  size_t usize = 0;
  double const f = 0.1;
  printf(
      "hello grandpa c is %d inches and %d salary and usize %-20.1f and float "
      "%zu\n",
      INCHE_PER_FOOT, salary, f, usize);
  printf("Input the diameter of the table : ");
  double diameter = 0.0;
  scanf("%lf", &diameter);
  circle(diameter);
}
