# dyn_array
A C89 standard compliant, single header, nostdlib (no C Standard Library) typesafe, dynamic generic array implementation

For more information please look at the "dyn_array.h" file or take a look at the "examples" or "tests" folder.

## Quick Start

Download or clone dyn_array.h and include it in your project.

```C
#include "dyn_array.h"

int main() {
   double *myNumbers = NULL;     /* IMPORTANT: Always initialize! */
   dyn_array_init(myNumbers, 8); /* OPTIONAL: Initialize with initial capacity of 8 elements */

   dyn_array_add(myNumbers, 42.0);
   dyn_array_add(myNumbers, 1337.0);

    for (i = 0; i < dyn_array_length(myNumbers); ++i)
    {
        printf("%g\n", myNumbers[i]);
    }

    printf("   cap: %i\n", dyn_array_capacity(myNumbers));
    printf("length: %i\n", dyn_array_length(myNumbers));

    dyn_array_del(myNumbers);    /* Delete last element */
    dyn_array_free(myNumbers);   /* Free array. Invokes the "DYN_ARRAY_FUNCTION_FREE" */

    return 0;
}
```

## Run Example: nostdlib, freestsanding

In this repo you will find the "examples/dyn_array_win32_nostdlib.c" with the corresponding "build.bat" file which
creates an executable only linked to "kernel32" and is not using the C standard library and executes the program afterwards.
