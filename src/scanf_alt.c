#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

// SOURCE: https://sekrit.de/webdocs/c/beginners-guide-away-from-scanf.html
// ALT:
// https://stackoverflow.com/questions/58403537/what-can-i-use-for-input-conversion-instead-of-scanf/58403955#58403955

[[maybe_unused]] static bool using_fgets_and_strtol() {
  constexpr auto buf_size = 1024;
  constexpr auto base_10 = 10;
  char buf[buf_size] = {}; // use 1KiB just to be sure

  bool success = false; // flag for successful conversion
  long number = {};

  do {
    printf("enter a number: ");
    if (!fgets(buf, buf_size, stdin)) {
      // reading input failed:
      return EXIT_FAILURE;
    }

    // have some input, convert it to integer:
    char *endptr = nullptr;

    errno = 0; // reset error number
    number = strtol(buf, &endptr, base_10);
    if (errno == ERANGE) {
      printf("Sorry, this number is too small or too large.\n");
      success = false;
    } else if (endptr == buf || (*endptr && *endptr != '\n')) {
      // no character was read OR
      // *endptr is neither end of string nor newline,
      // so we didn't convert the *whole* input
      success = false;
    } else {
      success = true;
    }
  } while (!success); // repeat until we got a valid number

  printf("You entered %ld.\n", number);
  return EXIT_SUCCESS;
}
