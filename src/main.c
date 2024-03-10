#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define INCHE_PER_FOOT 12

extern int oldMain(int argc, char const * [argc + 1]);

static enum Weekends {
  Friday,
  Saturday,
  Sunday,
} bestday = Friday,
  secondbest = Saturday;

static void circle(double diameter) {
  const double PI = 3.14'159'265;
  const double radius = diameter / 2.0;
  [[maybe_unused]] const double b = 0x101'010;
  const double circumference = 2.0 * PI * radius;
  const double area = PI * radius * radius;
  printf("\nThe circumference is %-3.2f\n", circumference);
  printf("\nArea of table is %+3.4f\n", area);
}

[[deprecated]] [[maybe_unused]] double static myrand() {
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
      [3] = 0.00007,
  };

  for (size_t index = 0; index < sizeof(arr) / sizeof(arr[0]); ++index) {
    printf("elemets %zu is %g,\nits square is %g\n", index, arr[index],
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
  arr _ [[maybe_unused]] = saysomething(input);
  puts(input);
  return EXIT_SUCCESS;
}
