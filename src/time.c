#define C23_FALLBACK_SILENT
#include "c23-fallback.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

extern void use_time();

static bool leapyear(uint32_t year) {
  /* All years that are divisible by 4 are leap years,
  unless they start a new century, provided they
  are not divisible by 400. */
  return !(year % 4) && ((year % 100) || !(year % 400));
}

#define DAYS_BEFORE                                                            \
  (int const[12]) {                                                            \
    [0] = 0, [1] = 31, [2] = 59, [3] = 90, [4] = 120, [5] = 151, [6] = 181,    \
    [7] = 212, [8] = 243, [9] = 273, [10] = 304, [11] = 334,                   \
  }

static struct tm time_set_yday(struct tm t) {
  // tm_mdays starts at 1.
  t.tm_yday += DAYS_BEFORE[t.tm_mon] + t.tm_mday - 1;
  // Takes care of leap years
  if ((t.tm_mon > 1) && leapyear((uint32_t)t.tm_year + 1900))
    ++t.tm_yday;
  return t;
}
constexpr auto UNIX_TIME = 1900;

void use_time() {
  struct tm today = {
      .tm_year = 2024 - UNIX_TIME,
      .tm_mon = 4 - 1,
      .tm_mday = 14,
      .tm_hour = 0,
      .tm_min = 40,
      .tm_sec = 39,
  };
  printf("this year is % d, next year will be % d\n", today.tm_year + 1900,
         today.tm_year + UNIX_TIME + 1);
  today = time_set_yday(today);
  printf("day of the year is % d\n", today.tm_yday);
}
