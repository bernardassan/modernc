#ifndef DYNAMIC_MEMORY_H
#define DYNAMIC_MEMORY_H

#include <stddef.h>

// circular: a type for a circular buffer for double values
// This data structure allows to add double values in rear and to take them out
// in front. Each such structure has a maximal amount of elements that can be
// stored in it
/** @brief the implementation of the circular buffer type */
// use counted_by in gcc 15 when it becomes available
typedef struct circular {
  size_t start;   /**< Position of element 0 */
  size_t len;     /**< Number of elements stored */
  size_t max_len; /**< Maximum capacity */
  /**< Array holding the data*/
  double tab[] __attribute__((counted_by(max_len)));
} circular;

// circular_append: Append a new element with value value to the buffer c.
// Returns: c if the new element could be appended, 0 otherwise.
[[nodiscard("Pointer to new data discared"), gnu::nonnull]] circular *
circular_append(circular *const circle, double value);

// circular_pop: Remove the oldest element from c and return its value.
// Returns: the removed element if it exists, 0.0 otherwise.
[[nodiscard, gnu::nonnull]] double circular_pop(circular *const circle);

// circular_element: Return a pointer to position pos in buffer c.
// Returns: a pointer to the pos’ element of the buffer, 0 otherwise.
[[nodiscard, gnu::nonnull]] double *circular_element(circular *const circle,
                                                     size_t pos);

// circular_destroy: Destroy circular buffer c.
// c must have been initialized with a call to `circular_init`
[[gnu::nonnull]] void circular_destroy(circular *circle);

// circular_init: Initialize a circular buffer c with maximally max_len
// elements. Only use this function on an uninitialized buffer. Each buffer that
// is initialized with this function must be destroyed with a call to
// `circular_destroy`.
[[nodiscard, gnu::nonnull]] circular *circular_init(circular *circle,
                                                    size_t max_len);

// circular_delete: Delete circular buffer c.
// c must have been allocated with a call to `circular_new`
#if defined(__clang__)
[[clang::ownership_takes(malloc, 1)]]
#endif
[[gnu::nonnull]] void
circular_delete(circular *circle);
// circular_new: Allocate and initialize a circular buffer with maximally len
// elements. Each buffer that is allocated with this function must be deleted
// with a call to `circular_delete`.
#if defined(__clang__)
// https://github.com/llvm/llvm-project/blob/deafb36f87a3541715854d4a620a4cfd6b1ac672/clang/test/Analysis/malloc-annotations.c
// https://github.com/llvm/llvm-project/issues/76861
//  1 refers to the first parameter of init which is the size of the malloc .ie
//  `len` parma
[[clang::ownership_returns(malloc, 1)]]
#elif defined(__GNUC__)
[[gnu::malloc(circular_delete, 1)]]
#elif defined(_MSC_VER)
#endif
[[nodiscard, gnu::malloc]] circular *
circular_new(size_t len);

// circular_resize: Resize to capacity max_len.
[[nodiscard("returned  pointer  replaces  function  argument"),
  gnu::nonnull]] circular *
circular_resize(circular *circle, size_t new_max_len);

// circular_getlength: Return the number of elements stored.
[[gnu::nonnull]] size_t circular_getlength(circular const *const circle);

#define NEW(T, ...) T##_init(malloc(sizeof(T)) __VA_OPT(, ) __VA_ARGS__)

#endif // DYNAMIC_MEMORY_H
