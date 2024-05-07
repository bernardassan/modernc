#define C23_FALLBACK_SILENT
#include "c23-fallback.h"
#include <stdio.h>
// Structure containing an array of integers
struct MyData {
  int numbers[3];
};

// Function with the specified declaration
static int const (*return_ptr_to_array(void const *const p))[3] {
  // Cast the pointer to the correct type (assuming p points to a MyData struct)
  auto data = (struct MyData const *const)p;
  // Return a pointer to the array within the data structure
  return &data->numbers;
}

static int (*return_ptr_to_array_static(const void *p))[3] {
  static int arr[3] = {}; // Static array to be returned
  const int *ptr = p; // Casting the input pointer to a constant integer pointer

  // Assuming the input pointer points to an array of at least 3 integers
  for (int i = 0; i < 3; i++) {
    arr[i] = ptr[i]; // Copy the values from the input array to the static array
  }

  return &arr; // Return a pointer to the static array
}

static int (*return_ptr_to_array_dynamic(const void *p))[3] {
  // return type is pointer to array of 3 int
  return malloc(sizeof(int[3]));
}

typedef struct {
  char arr[3];
} char_array;

[[gnu::const, gnu::nonnull(1)]] static char_array saysomething(char str[]);

static char_array saysomething(char str[]) {
  size_t index = 1;
  size_t value = 1;
  while (str[value]) {
    if (str[value] != str[value - 1]) {
      str[index] = str[value];
      index++;
    }
    value++;
  }
  str[index] = '\0';
  arr val = {.arr = {}};
  return val;
}

[[maybe_unused]] static void array() {
  const char sayings[][32] = {
      "Manners maketh man.",
      "Many hands make light work.",
      "Too many cooks spoil the broth.",
  };
  for (unsigned int i = 0; i < sizeof(sayings) / sizeof(sayings[0]); ++i) {
    printf("%s\n", sayings[i]);
  }

  union A {
    int a;
    double c;
  };

  union A b [[maybe_unused]] = {.a = 32};
  const double arr[5] = {
      [0] = 9.0,
      [1] = 7.0,
      [4] = 3.0E+23,
      [3] = 0.000'07,
  };

  for (size_t index = 0; index < sizeof(arr) / sizeof(arr[0]); ++index) {
    printf("elemets %zu is %g,\tits square is %g\n", index, arr[index],
           arr[index] * arr[index]);
  }
}

static int use_array() {
  struct MyData my_data = {{1, 2, 3}};

  // Call the function and access the array
  int const(*const array_ptr)[3] = return_ptr_to_array(&my_data);
  printf("First element: %d\n",
         (*array_ptr)[0]); // Access elements using double dereference

  // return_ptr_to_array_static
  int arr[3] = {1, 2, 3};

  int(*result)[3] = return_ptr_to_array_static(
      arr); // Call the function with an array of 3 integers
  for (int i = 0; i < 3; i++) {
    printf("%d ", (*result)[i]); // Access the elements of the returned array
  }
  char input[] = "pebbbabbbles";
  array();
  puts(input);
  auto _ [[maybe_unused]] = saysomething(input);
  return 0;
}
