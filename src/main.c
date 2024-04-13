#include <assert.h>
#define C23_FALLBACK_SILENT
#include "c23-fallback.h"
#include <complex.h>
#include <stdbit.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifndef __STDC_VERSION_STDBIT_H__
#error "stdbit.h is required for this module"
#endif

enum corvid {
  chough,
  magpie,
  raven,
  jay,
  corvid_num,
};

constexpr char CORVID_NAME[corvid_num][8] = {
    [chough] = "chough",
    [raven] = "raven",
    [magpie] = "magpie",
    [jay] = "jay",
};
// queue.h
// hsearch.h
// insque
// tsearch
#define INCHE_PER_FOOT 12

extern int oldMain(int argc, char const * [argc + 1]);

static enum Weekends {
  Friday,
  Saturday,
  Sunday,
} bestday = Friday,
  secondbest = Saturday;

static void enumerations() {

#define FLOCK_MAGPIE (1U << magpie)
#define FLOCK_RAVEN (1U << raven)
#define FLOCK_JAY (1U << jay)
#define FLOCK_CHOUGH (1U << chough)
#define FLOCK_EMPTY 0U
#define FLOCK_FULL ((1U << corvid_num) - 1)

  constexpr double π [[maybe_unused]] = 3.141'592'653'589'793'238'46;
  constexpr bool this_is [[maybe_unused]] = true;
  constexpr char a [[maybe_unused]] = 'a';

  uint32_t n = 78;
  int64_t big = (-UINT64_C(1)) >> 1; // Same value as INT64_MAX
  auto alongfloat [[maybe_unused]] = 10.0l;
  auto complex_long [[maybe_unused]] = 3.0L + I;
  double complex num [[maybe_unused]] = 3.0;
  double C [[maybe_unused]][] = {
      [0] = 6,
      [3] = 1,
  };

#if __STDC_VERSION__ >= 202311L
  printf("n␣ is ␣%w32u ,␣ and ␣ big ␣ is ␣%w64d \n", n, big);
#else
  printf("n is %u , and big is %li\n", n, big);
#endif
  // auto si = (CORVID_NAME(raven));

#if __STDC_VERSION_STDBIT_H__
  // unsigned _BitInt(3) u3 = 7wbu; // values 0, ... , 3, ... ,7
  // signed _BitInt(3) u3 = 3wb;                  // values -4, ... , 0, ... , 3
  // _BitInt(3) n3 = 3WB;                         // same
  auto u0 [[maybe_unused]] = +1wbu; // value 0
  // constexpr unsigned _BitInt(3) max3u = -1wbu; // 0b 111
  // constexpr unsigned _BitInt(4) max4u = -1wbu; // 0b 1111
  // constexpr unsigned _BitInt(4) high4u = max4u - max3u; // 0b 1000
  // constexpr signed _BitInt(4) max4s = max3u;            // 0b 0111
  // constexpr signed _BitInt(4) min4s = ~max4s;           // 0b 1000
  // printf("max3u is %" PRIu8 ", max4u is %b", max3u, max4u);
#endif

  for (size_t index = 0; index < corvid_num; ++index) {
    printf("Corvid %zu is the %s\n", index, CORVID_NAME[index]);
  }
}

static void circle(double diameter) {
  const double PI = 3.14'159'265;
  const double radius = diameter / 2.0;
  [[maybe_unused]] const double b = 0x101'010;
  const double circumference = 2.0 * PI * radius;
  const double area = PI * radius * radius;
  printf("\nThe circumference is %-3.2f\n", circumference);
  printf("\nArea of table is %+3.4f\n", area);
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

[[maybe_unused]] static void switching(enum Weekends day) {
  enum Weekends churchday = Sunday;
  bestday = churchday;
  secondbest = bestday;
  bool boolean = true;
  switch (day) {
  case Friday:
    printf("\nIt is friday\n");
    if (boolean && churchday == Friday) {
      boolean = false;
      churchday = Friday;
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

[[maybe_unused]] static void sudoRandom() {
  // randomise seed
  srand((unsigned)time(NULL));
  const int limit = 20;
  const long choosen_random = rand() % limit; // 0 to limit-1 inclusive

  printf("This is the guessing game\n"
         "I have choosen a number between 0 and  10 guess which number it is");

  for (int count = 3; count > 0; --count) {
    printf("\nYou have %d tr%s left", count, count == 1 ? "y" : "ies");
    printf("\nWhat number do you guess : ");
    int users_guess = 0;
    scanf("%d", &users_guess);

    if (choosen_random == users_guess) {
      printf("\nCongratulations you guessed right\n");
      return;
    } else if (users_guess < 0 || users_guess > 20) {
      printf("\nSorry bruh, 😢 the number is between 0 and 20");
    } else {
      printf(
          "\nYou tried 😆 better luck next time\nHint:The choosen number is %s "
          "than %d",
          choosen_random > users_guess ? "greater" : "less", users_guess);
    }
  }
  printf("\nHomey 😭 you had your 3 tries and failed.\nThe choosen number is "
         "%ld\n",
         choosen_random);
}

[[maybe_unused]] static void sal() {
  int salary = 10000ULL;
  size_t usize = 0;
  double f = 0.1;
  printf(
      "hello grandpa c is %d inches and %d salary and usize %-20.1f and float "
      "%zu\n",
      INCHE_PER_FOOT, salary, f, usize);
  printf("Input the diameter of the table : ");
  double diameter = 0.0;
  scanf("%lf", &diameter);
  circle(diameter);
}

[[maybe_unused]] static void array() {
  const char sayings[][32] = {
      "Manners maketh man.",
      "Many hands make light work.",
      "Too many cooks spoil the broth.",
  };
  for (unsigned int i = 0; i < sizeof(sayings) / sizeof(sayings[0]); ++i)
    printf("%s\n", sayings[i]);

  union A {
    int a;
    double c;
  };

  [[maybe_unused]] union A b = {.a = 32};
  const double arr[5] = {
      [0] = 9.0,
      [1] = 7.0,
      [4] = 3.0E+23,
      [3] = 0.000'07,
  };

  for (size_t index = 0; index < sizeof(arr) / sizeof(arr[0]); ++index) {
    printf("elemets %zu is %g,\tits square is %g\n", index, arr[index],
           arr[index] * arr[index]);
  }
}

typedef struct {
  char arr[3];
} arr;

[[gnu::const, gnu::nonnull(1)]] static arr saysomething(char str[]);

static arr saysomething(char str[]) {
  size_t index = 1;
  size_t value = 1;
  while (str[value]) {
    if (str[value] != str[value - 1]) {
      str[index] = str[value];
      index++;
    }
    value++;
  }
  str[index] = '\0';
  arr val = {.arr = {}};
  return val;
}

int oldMain(int argc [[maybe_unused]],
            char const *argv [[maybe_unused]][argc + 1]) {
  char input[] = "pebbbabbbles";
  array();
  enumerations();
  arr _ [[maybe_unused]] = saysomething(input);
  puts(input);
  return EXIT_SUCCESS;
}
