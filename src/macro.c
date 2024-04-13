// https://pmihaylov.com/macros-in-c/
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
//__attribute__((section("DUART_A")))
// nonnull(1, 2)
// pure,const,cleanup,format,aligned,packed
// vector_size(4 * sizeof(int))
// matrix_type(4, 4)
// deprecated("message") || [[deprecated("reason")]]
// noreturn
#define MAX 100

#define ADD(p, q) ((p) + (q))

#define INCREMENT(n) ++(n)

#define min(a, b) (((a) < (b)) ? (a) : (b))

#define log_info(M, ...)                                                       \
  fprintf(stderr, "[INFO] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)

#define MACRO(n, limit)                                                        \
  while (n < limit) {                                                          \
    printf("Educba ");                                                         \
    n++;                                                                       \
  }

#define MODIFY(arr, index)                                                     \
  do {                                                                         \
    arr[index] *= 5;                                                           \
    index++;                                                                   \
  } while (false)

#define DIV(a, b) ((a) / (b))

#define concate_(p, q) p##q
#define concate(p, q) concate_(p, q)

#define get_string(n) #n

#define VERBOSE 3

[[gnu::always_inline]] void f(char *a);
[[gnu::always_inline]] void f(char *a [[maybe_unused]]) {}

[[maybe_unused]] static int macro_main() {
  char const *p = "Educba";
  int a = 20;
  printf(" This use char datatype %s \n", INCREMENT(p));
  printf("\n This uses int datatype %d\n", INCREMENT(a));

  int n = 0;
  MACRO(n, 5);

  printf("\n%d\n", DIV(5 + 3, 3 + 2));
  printf("%d \n", concate(56, 78));
  printf("%s\n", get_string(Educba));

  printf("%i + %i is %i\n", 1, 3, ADD(1, 3));

#if VERBOSE >= 2
  printf("Message\n");
#endif
  const char *concate(hello, __COUNTER__) = "hello";
  printf("%s\n", hello0);

  return EXIT_SUCCESS;
}
