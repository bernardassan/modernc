// In a header
#include <stdio.h>

typedef int logger_function(char const *, ...);
extern logger_function *logger;
enum logs { log_pri, log_ign, log_ver, log_num };

// In a .c file ( TU )
extern int logger_verbose(char const *, ...);
static int logger_ignore(char const *, ...) { return 0; }
logger_function *logger = logger_ignore;
static logger_function *loggers[] = {
    [log_pri] = printf,
    [log_ign] = logger_ignore,
    [log_ver] = logger_verbose,
};

void log() {
  constexpr auto LOGGER = log_ver;
  if (LOGGER < log_num)
    logger = loggers[LOGGER];

  logger(" Do ␣ we ␣ ever ␣ see ␣ line ␣ %lu ␣ of ␣ file ␣ %s?", __LINE__ + 0UL,
         __FILE__);
}
