#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define INCHE_PER_FOOT 12

extern int oldMain(int, char **);

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

static void sudoRandom() {
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

int oldMain(int, char **) {
  sudoRandom();
  return EXIT_SUCCESS;
}
