#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Structure containing an array of integers
struct Structure {
  int numbers[3];
};

// Function with the specified declaration
static int const (*return_ptr_to_array(void const *const ptr))[3] {
  // Cast the pointer to the correct type (assuming p points to a MyData struct)
  auto data = (struct Structure const *const)ptr;
  // Return a pointer to the array within the data structure
  return &data->numbers;
}

static int (*return_ptr_to_array_static(const void *ptr))[3] {
  static int arr[3] = {}; // Static array to be returned
  // Casting the input pointer to a constant integer pointer
  const int *ptr_ = ptr;

  // Assuming the input pointer points to an array of at least 3 integers
  for (int index = 0; index < 3; index++) {
    // Copy the values from the input array to the static array
    arr[index] = ptr_[index];
  }

  return &arr; // Return a pointer to the static array
}

static int (*return_ptr_to_array_dynamic
            [[maybe_unused]] (const void * /*ptr*/))[3] {
  // return type is pointer to array of 3 int
  return malloc(sizeof(int[3]));
}

static auto constexpr CHAR_ARRY_SIZE = 8;
typedef struct {
  char arr[CHAR_ARRY_SIZE];
} char_array;

// TRYOUT: attributes [[nonnull_if_nonzero, null_terminated_string_arg]]
[[gnu::const, gnu::nonnull(1)]] static char_array
saysomething(char const str[]) {
  char_array val = {.arr = {}};
  size_t index = 1;
  size_t value = 1;
  while (str[value]) {
    if (str[value] != str[value - 1]) {
      val.arr[index] = str[value];
      index++;
    }
    value++;
  }
  val.arr[index] = '\0';
  return val;
}

[[maybe_unused]] static void array() {
  const char sayings[][32] = {
      "Manners maketh man.",
      "Many hands make light work.",
      "Too many cooks spoil the broth.",
  };
  for (unsigned int index = 0; index < sizeof(sayings) / sizeof(sayings[0]);
       ++index) {
    printf("%s\n", sayings[index]);
  }

  union Union {
    int a;
    double c;
  };
  union Union set_a [[maybe_unused]] = {.a = 32};

  const double arr[5] = {
    [0] = 9.0,
    [1] = 7.0,
    [4] = 3.0E+23,
    [3] = 0.000'07, };

    for (size_t index = 0; index < sizeof(arr) / sizeof(arr[0]);
         ++index){printf("elemets %zu is %g,\tits square is %g\n", index,
                         arr[index], arr[index] * arr[index]);
}
}

static int use_array [[maybe_unused]] () {
  struct Structure my_data = {{1, 2, 3}};

  // Call the function and access the array
  int const(*const array_ptr)[3] = return_ptr_to_array(&my_data);
  printf("First element: %d\n",
         (*array_ptr)[0]); // Access elements using double dereference

  // return_ptr_to_array_static
  int arr[3] = {1, 2, 3};

  // Call the function with an array of 3 integers
  int (*result)[3] = return_ptr_to_array_static(arr);
  for (int i = 0; i < 3; i++) {
    printf("%d ", (*result)[i]); // Access the elements of the returned array
  }
  char input[] = "pebbbabbbles";
  array();
  puts(input);
  (void)saysomething(input);
  return 0;
}
