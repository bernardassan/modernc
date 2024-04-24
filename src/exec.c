#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static void exec(char *input) {
  char const *const args[3] = {"any_exe", input, nullptr};
  char **env;
  extern char const **const environ;

  /* ... Sanitize arguments ... */

  pid_t pid = fork();
  if (pid == -1) {
    /* Handle error */
  } else if (pid != 0) {
    int status = {};
    pid_t ret = 0;
    while ((ret = waitpid(pid, &status, 0)) == -1) {
      if (errno != EINTR) {
        /* Handle error */
        break;
      }
    }
    if ((ret == 0) || !(WIFEXITED(status) && !WEXITSTATUS(status))) {
      /* Report unexpected child status */
    }
  } else {
    /* ... Initialize env as a sanitized copy of environ ... */
    if (execve("/usr/bin/any_cmd", (char *const *)args, env) == -1) {
      /* Handle error */
      _Exit(127);
    }
  }
}
