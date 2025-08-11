#include <stdio.h>

static void circle(double diameter) {
  // clang-format off
  double const PI = 3.14'159'265;
  // clang-format on
  double const radius = diameter / 2.0;
  [[maybe_unused]] double const hex =
      0x101'010; double const circumference = 2.0 * PI * radius; double const area = PI * radius * radius; printf(
          "\nThe circumference is %-3.2f\n", circumference);
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
