#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// queue.h
// hsearch.h
// insque
// tsearch
constexpr static auto SENTINAL = 1;

extern int oldMain(int argc, char const *argv[argc + SENTINAL]);
extern void use_time();

int oldMain(int argc, char const *argv[argc + SENTINAL]) {
  size_t len = strlen(argv[0]) + SENTINAL;
  char name[len] = {};
  auto const program_name = argv[0];

  auto result = memccpy(name, program_name, '\0', len);
  if (!result) {
    fprintf(stderr,
            "Error result '%s': The character %#X was not found in the string "
            "'%s'",
            (char *)result, (uint32_t)'\0', program_name);
    return EXIT_FAILURE;
  }
  if (!strncmp(name, program_name, len)) {
    printf("\nprogram name '%s' has been successfully copied\n", name);
  } else {
    printf("copying argv[0] '%s' lead to a different string '%s'", argv[0],
           name);
  }

  use_time();
  return EXIT_SUCCESS;
}
