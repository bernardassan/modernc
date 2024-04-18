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

int main() {
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
  return 0;
}
