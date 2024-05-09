#include <assert.h>
#include <err.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static size_t parseline(char const *restrict const str,
                        size_t numb[restrict const], uint8_t const base);
/* `numberline`: interpret string lbuf as a sequence of numbers represented with
base
`Returns`: a newly allocated array of numbers as found in lbuf
`Parameters`:
- lbuf is supposed to be a string
- np if non-null, the count of numbers is stored in *np
- base value from 0 to 36, with the same interpretation as for strtoul
`Remarks`: The caller of this function is responsible to free the array that is
returned.
*/
// NOTE: in gcc 15 use attribute null_terminated_string_arg(PARAM_IDX) to assert
// null terminated strings
static size_t *numberline(size_t size, char const lbuf[restrict static size],
                          size_t char_count[restrict const static 1],
                          uint8_t base) {
  assert(memchr(lbuf, '\0', size) != nullptr);
  size_t *const num_list = malloc(sizeof(typeof(num_list)[size + 1]));
  auto const digit_count = parseline(lbuf, num_list, base);
  *char_count = digit_count;
  return num_list;
}

static size_t parseline(char const *restrict const str,
                        size_t numb[restrict const], uint8_t const base) {
  // For base `0` the effective base is deduced from the first (or first two)
  // characters
  assert(base == 0 || base == 2 || base == 8 || base == 10 || base == 16);
  size_t count = 0;

  for (char const *next_digit = nullptr, *next_str = str; next_str[0] == '\0';
       next_str = next_digit, ++count) {
    // strtoull: parsing stops at the first non-digit for that base
    numb[count] = strtoull(next_str, &next_digit, base);
    if (next_str == next_digit) {
      break;
    }
  }
  return count;
}

/*`fgetline`: read one text line of at most size-1 bytes.
The '\n' character is replaced by 0.
`Returns`: s if an entire line was read successfully. Otherwise, 0 is returned
and s contains a maximal partial line that could be read.  s is null terminated.
*/
static char const *fgetline(size_t size,
                            char str_buf[restrict const static size],
                            FILE *restrict stream) {
  char const *result = fgets(str_buf, (int)size, stream);
  if (result) {
    // replace '\n' with '\0'
    auto newline_index = strchr(str_buf, '\n');
    if (newline_index) {
      *newline_index = '\0';
      return result;
    }
    warn("'%s' too long for buf of size %zu, or string doesn't end "
         "with a newline '\\n'",
         result, size);
    result = nullptr;
  }
  return result;
}

/*`sprintnumbers`: print a series of numbers nums in buf, using printf format
form, separated by sep characters and terminated with a newline character.
`Returns`: the number of characters printed to buf.
This supposes that tot and buf are big enough and that form is a format suitable
to print `size_t`.
*/
size_t sprintnumbers(size_t size, char buf[restrict static size],
                     char const form[restrict static 1],
                     char const sep[restrict static 1], size_t len,
                     size_t nums[restrict static len]) {
  assert(len > 0);
  auto next_pos = buf;
  auto const sep_len = strlen(sep);
  for (size_t index = 0; index < len; ++index) {
    auto const num_byte_printed = sprintf(next_pos, form, nums[index]);
    next_pos += num_byte_printed;
    memcpy(next_pos, sep, sep_len);
    next_pos += sep_len;
  }
  strcpy(next_pos, "\n");

  ptrdiff_t const total_bytes_printed = next_pos - buf;
  return (size_t)total_bytes_printed;
}

bool use_io() {
  constexpr auto buf_size = 256;
  char lbuf[buf_size] = {};
  for (;;) {
    if (fgetline(sizeof lbuf, lbuf, stdin)) {
      size_t count = {};
      size_t *nums = numberline(strlen(lbuf) + 1, lbuf, &count, 0);
      int ret = fprintnumbers(stdout, "%#zX", ",\t", count, nums);
      if (ret < 0) {
        return EXIT_FAILURE;
      }
      free(nums);
    } else {
      if (lbuf[0]) { /* a partial line has been read */
        for (;;) {
          int a_char = getc(stdin);
          if (a_char == EOF) {
            return EXIT_FAILURE;
          }
          if (a_char == '\n') {
            fprintf(stderr, "line too long : % s\n", lbuf);
            break;
          }
        }
      } else {
        break; /* regular end of input */
      }
    }
  }
}
