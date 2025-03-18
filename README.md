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

## "nostdlib" Motivation & Purpose

nostdlib is a lightweight, minimalistic approach to C development that removes dependencies on the standard library. The motivation behind this project is to provide developers with greater control over their code by eliminating unnecessary overhead, reducing binary size, and enabling deployment in resource-constrained environments.

Many modern development environments rely heavily on the standard library, which, while convenient, introduces unnecessary bloat, security risks, and unpredictable dependencies. nostdlib aims to give developers fine-grained control over memory management, execution flow, and system calls by working directly with the underlying platform.

### Benefits

#### Minimal overhead
By removing the standard library, nostdlib significantly reduces runtime overhead, allowing for faster execution and smaller binary sizes.

#### Increased security
Standard libraries often include unnecessary functions that increase the attack surface of an application. nostdlib mitigates security risks by removing unused and potentially vulnerable components.

#### Reduced binary size
Without linking to the standard library, binaries are smaller, making them ideal for embedded systems, bootloaders, and operating systems where storage is limited.

#### Enhanced performance
Direct control over system calls and memory management leads to performance gains by eliminating abstraction layers imposed by standard libraries.

#### Better portability
By relying only on fundamental system interfaces, nostdlib allows for easier porting across different platforms without worrying about standard library availability.

## Run Example: nostdlib, freestsanding

In this repo you will find the "examples/dyn_array_win32_nostdlib.c" with the corresponding "build.bat" file which
creates an executable only linked to "kernel32" and is not using the C standard library and executes the program afterwards.
