#ifndef TEMPLATES_H
#define TEMPLATES_H

#include <stdlib.h>

// Common.h
#define _cat(a, b) a##b
#define _cat2(a, b) _cat(a, b)
#define MEMB(name) _cat2(Self, name)
#ifndef i_tag
#define i_tag i_val
#endif

// Vec.h
#define Self _cat2(Vec_, i_tag)

typedef i_val MEMB(_val);
typedef struct {
  MEMB(_val) * at;
  size_t len, cap;
} Self;

static inline void MEMB(_push)(Self *self, MEMB(_val) v) {
  if (self->len == self->cap)
    self->at = realloc(self->at, (self->cap = self->len * 3 / 2 + 4));
  self->at[self->len++] = v;
}
#undef i_tag
#undef i_val
#undef Self

#endif // TEMPLATES_H
