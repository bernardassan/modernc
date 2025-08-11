#include <complex.h>
#include <stdio.h>

// https://stackoverflow.com/questions/4322926/what-exactly-is-an-aligned-pointer

typedef complex double cdbl;

void enable_alignment_bus_check();

void enable_alignment_bus_check() {
// https://orchistro.tistory.com/206
// https://stackoverflow.com/questions/152016/detecting-cpu-architecture-compile-time
#if defined(__x86_64__) || defined(_M_X64)
  __asm__("pushf\n"
          "orl $0x40000, (%rsp)\n"
          "popf");
#elif defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
  __asm__("pushf; "
          "orl $(1<<18), (%esp); "
          "popf;");
#endif
}

int main() {
  /* An overlay of complex values and bytes. */
  // natural alignment for `z`, to load z in a single memory load
  alignas(sizeof(complex double)) complex double z [[maybe_unused]] = {};

  union {
    cdbl val[2];
    unsigned char buf[sizeof(cdbl[2])];
  } toocomplex = {
      .val =
          {
              0.5 + (0.5 * I),
              0.75 + (0.75 * I),
          },
  };
  printf("size/alignment: %zu/%zu\n", sizeof(cdbl), alignof(cdbl));
  /* Run over all offsets, and crash on misalignment. */
  for (size_t offset = sizeof(cdbl); offset; offset /= 2) {
    printf("offset\t%zu:\t", offset);
    fflush(stdout);
    cdbl *bp = (cdbl *)(&toocomplex.buf[offset]); // align!
    printf("%g\t+%gI\t", creal(*bp), cimag(*bp));
    fflush(stdout);
    *bp *= *bp;
    printf("%g\t+%gI", creal(*bp), cimag(*bp));
    fputc('\n', stdout);
  }
}
