#include <stdio.h>
#include <stdlib.h>
#include <time.h>

[[maybe_unused]] static void sudoRandom() {
  // randomise seed
  srand((unsigned)time(nullptr));
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
      quick_exit(EXIT_SUCCESS);
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
