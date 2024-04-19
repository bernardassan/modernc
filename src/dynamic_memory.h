#pragma once
#include <stddef.h>
// circular: a type for a circular buffer for double values
// This data structure allows to add double values in rear and to take them out
// in front. Each such structure has a maximal amount of elements that can be
// stored in it
/** @brief the implementation of the circular buffer type */
typedef struct circular {
  size_t start;   /**< Position of element 0 */
  size_t len;     /**< Number of elements stored */
  size_t max_len; /**< Maximum capacity */
  double tab[];   /**< Array holding the data */
} circular;

// circular_append: Append a new element with value value to the buffer c.
// Returns: c if the new element could be appended, 0 otherwise.
[[gnu::nonnull]] circular *circular_append(circular *const c, double value);

// circular_pop: Remove the oldest element from c and return its value.
// Returns: the removed element if it exists, 0.0 otherwise.
[[gnu::nonnull]] double circular_pop(circular *const c);

// circular_element: Return a pointer to position pos in buffer c.
// Returns: a pointer to the pos’ element of the buffer, 0 otherwise.
[[gnu::nonnull]] double *circular_element(size_t pos, circular const *const c);

// circular_init: Initialize a circular buffer c with maximally max_len
// elements. Only use this function on an uninitialized buffer. Each buffer that
// is initialized with this function must be destroyed with a call to
// `circular_destroy`.
circular *circular_init(circular *c, size_t max_len);

// circular_destroy: Destroy circular buffer c.
// c must have been initialized with a call to `circular_init`
void circular_destroy(circular *c);
// circular_new: Allocate and initialize a circular buffer with maximally len
// elements. Each buffer that is allocated with this function must be deleted
// with a call to circular_delete.
circular *circular_new(size_t len);

// circular_delete: Delete circular buffer c.
// c must have been allocated with a call to circular_new
void circular_delete(circular *c);

// circular_resize: Resize to capacity max_len.
circular *circular_resize(circular *c, size_t max_len);

// circular_getlength: Return the number of elements stored.
[[gnu::nonnull]] size_t circular_getlength(circular const *const c);
