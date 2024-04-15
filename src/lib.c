#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static_assert(sizeof(double) == sizeof(long),
              "Extra precision needed for convergence.");

static void puts_safe(char const s[static 1]) {
  static bool failed = false;
  if (!failed && puts(s) == EOF) {
    perror("can't output to terminal:");
    failed = true;
    errno = 0;
  }
  exit(EXIT_SUCCESS);
}

static int open_file() {
  FILE *logfile = fopen("mylog.txt", "a");
  if (!logfile) {
    perror("fopen failed");
    return EXIT_FAILURE;
  }
  fputs("feeling fine today\n", logfile);
  fclose(logfile);
  return EXIT_SUCCESS;
}

static int dump_multiple_files(int8_t file_count,
                               char const *const files[file_count]) {
  enum { buf_max = 32 };
  int ret = EXIT_FAILURE;
  char buffer[buf_max] = {0};
  for (int i = 1; i < file_count; ++i) {   // Processes args
    FILE *instream = fopen(files[i], "r"); // as filenames
    if (instream) {
      while (fgets(buffer, buf_max, instream)) {
        fputs(buffer, stdout);
      }
      fclose(instream);
      ret = EXIT_SUCCESS;
    } else {
      /* Provides some error diagnostic. */
      fprintf(stderr, "Could not open %s : ", files[i]);
      perror(0);
      errno = 0; // Resets the error code
    }
  }
  return ret;
}
