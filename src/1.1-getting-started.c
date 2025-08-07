#include <stdio.h>
#include <stdlib.h>

int main(int argc, [[maybe_unused]] char *argv[argc + 1]) {
  // Listing 1.1
  const double arr[5] = {
    [0] = 9.0,
    [1] = 7.0,
    [4] = 3.0E+23,
    [3] = 0.000'07 };

    for (size_t index = 0; index < sizeof(arr) / sizeof(arr[0]); ++index){

        printf("elemets %zu is %g,\tits square is %g\n", index, arr[index],
               arr[index] * arr[index]);
}

return EXIT_SUCCESS;
}
