// credit to https://github.com/ayende/libgavran/blob/master/ch02/code/errors.c
// https://github.com/ayende/libgavran/blob/master/ch02/code/memory.c
// end::mem_usage[]
// tag::declarations[]

#include "eh.h"
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ERRORS 64
#define MAX_ERRORS_MSG_BUFFER 2048

thread_local static char messages_buffer[MAX_ERRORS_MSG_BUFFER];
thread_local static const char *errors_messages_buffer[MAX_ERRORS];
thread_local static int error_messages_code[MAX_ERRORS];
thread_local static size_t errors_count;
thread_local static size_t errors_buffer_len;
thread_local static uint32_t out_of_memory;

// end::declarations[]

// tag::try_sprintf[]
[[gnu::format(printf, 4, 0), gnu::nonnull]] bool static try_vsprintf(
    char *buffer[], char const *const buffer_end, size_t *chars,
    const char *format, va_list ap) {
  size_t sz = (size_t)(buffer_end - *buffer);
  int rc = vsnprintf(*buffer, sz, format, ap);
  if (rc < 0 ||         // encoding
      (size_t)rc >= sz) // space
    return false;

  *buffer += rc;
  *chars += (size_t)rc;
  return true;
}

[[gnu::format(printf, 4, 5)]] static bool try_sprintf(char **buffer,
                                                      char *buffer_end,
                                                      size_t *chars,
                                                      const char *format, ...) {
  va_list ap;
  va_start(ap, format);
  bool ret = try_vsprintf(buffer, buffer_end, chars, format, ap);
  va_end(ap);
  return ret;
}
// end::try_sprintf[]

// tag::errors_push_new[]
op_result_t *errors_push_new(const char *file, uint32_t line, const char *func,
                             int32_t code) {
  // <1>
  if (errors_count >= MAX_ERRORS) {
    // we have no space any longer for errors, ignoring
    out_of_memory |= 1;
    return 0;
  }

  // <2>
  size_t index = errors_count++;
  error_messages_code[index] = code;

  char *msg = (messages_buffer + errors_buffer_len);
  char *end = messages_buffer + MAX_ERRORS_MSG_BUFFER;
  char *start = msg;

  char stack_buffer[128];
  int rc = strerror_r(code, stack_buffer, sizeof(stack_buffer));
  if (rc)
    strcpy(stack_buffer, "Unknown code");

  size_t chars_written = 0;
  // <3>
  if (!try_sprintf(&msg, end, &chars_written, "%s()", func) ||
      !try_sprintf(&msg, end, &chars_written, "%-*c - %s:%i",
                   (int)(30 - chars_written), ' ', file, line) ||
      !try_sprintf(&msg, end, &chars_written, "%*c - %3i %-20s |  ",
                   (int)(50 - chars_written), ' ', code, stack_buffer))
    goto oom;

  errors_buffer_len += (size_t)(msg - start);
  errors_messages_buffer[index] = start;
  return 0;
oom:
  out_of_memory |= 2;
  errors_messages_buffer[index] = 0;
  return 0;
}
// end::errors_push_new[]

// tag::errors_append_message[]
op_result_t *errors_append_message(const char *format, ...) {
  if (!errors_count && errors_buffer_len)
    return 0;

  // <1>
  char *msg = (messages_buffer + errors_buffer_len) - 1;
  char *end = messages_buffer + MAX_ERRORS_MSG_BUFFER;
  size_t chars_written = 0;

  va_list ap;
  va_start(ap, format);
  bool ret = try_vsprintf(&msg, end, &chars_written, format, ap);
  va_end(ap);
  if (!ret) {
    // <2>
    *msg = 0; // undo possible overwrite of null terminator
    out_of_memory |= 2;
    return 0;
  }

  errors_buffer_len += chars_written;
  return 0; // simply to allow it to be used in comma operator
}
// end::errors_append_message[]

// tag::rest[]
const char **errors_get_messages(size_t *number_of_errors) {
  *number_of_errors = errors_count;
  return (const char **)errors_messages_buffer;
}

int *errors_get_codes(size_t *number_of_errors) {
  *number_of_errors = errors_count;
  return error_messages_code;
}

void errors_print_all(void) {
  for (size_t i = 0; i < errors_count; i++) {
    printf("%s\n", errors_messages_buffer[i]);
  }

  if (out_of_memory) {
    const char *msg = "Too many errors, "
                      "additional errors were discarded";
    printf("%s (%d)\n", msg, -(int32_t)out_of_memory);
  }
  errors_clear();
}

void errors_clear(void) {
  out_of_memory = 0;
  memset(error_messages_code, 0, sizeof(int32_t *) * errors_count);
  memset(errors_messages_buffer, 0, sizeof(char *) * errors_count);
  memset(messages_buffer, 0, errors_buffer_len);
  errors_buffer_len = 0;
  errors_count = 0;
}

size_t errors_get_count() { return errors_count; }

uint32_t errors_get_oom_flag() { return out_of_memory; }
// end::rest[]

// tag::mem_usage[]
result_t mem_alloc(void **buffer, size_t size) {
  void *tmp_buf = malloc(size);
  if (!tmp_buf) {
    failed(ENOMEM, msg("Unable to allocate buffer"), with(size, "%zu"));
  }
  *buffer = tmp_buf;
  return success();
}

result_t mem_alloc_page_aligned(void **buffer, size_t size) {
  void *tmp_buf;
  int err = posix_memalign(&tmp_buf, 4096, size);
  if (err) {
    failed(err, msg("Unable to allocate page aligned buffer"),
           with(size, "%zu"));
  }
  *buffer = tmp_buf;
  return success();
}

result_t mem_realloc(void **buffer, size_t new_size) {
  void *tmp_buf = realloc(*buffer, new_size);
  if (!tmp_buf) {
    failed(ENOMEM, msg("Unable to re-allocate buffer"), with(new_size, "%zu"));
  }
  *buffer = tmp_buf;
  return success();
}
// end::mem_usage[]

result_t mem_calloc(void **buffer, size_t size) {
  void *tmp_buf = calloc(1, size);
  if (!tmp_buf) {
    failed(ENOMEM, msg("Unable to allocate buffer"), with(size, "%zu"));
  }
  *buffer = tmp_buf;
  return success();
}

result_t mem_duplicate_string(char **dest, const char *src) {
  *dest = strdup(src);
  if (!*dest) {
    failed(ENOMEM, msg("Unable to duplicate string"), with(src, "%s"));
  }
  return success();
}
