/* dyn_array.h - v0.1 - public domain data structures - nickscha 2025

A C89 standard compliant, single header, nostdlib (no C Standard Library) dynamic generic array implementation.

This example demonstrates a win32 program using dyn_array.h without using and linking to the C standard library.

The resulting executable from the build file has only linking to kernel32 (for VirtualAlloc) and nothing else :)

It also does not uses "windows.h" but defines the windows functions prototypes directly since windows.h is
massivly bloated and reduces the build time significantly.

This example tested with clang and gcc.

Please read build.bat file to see the compiler flags and their description.

LICENSE

  Placed in the public domain and also MIT licensed.
  See end of file for detailed license information.

*/
#ifdef _MSC_VER
#pragma function(memset)
#endif
void *memset(void *dest, int c, unsigned int count)
{
    char *bytes = (char *)dest;
    while (count--)
    {
        *bytes++ = (char)c;
    }
    return dest;
}

#ifdef _MSC_VER
#pragma function(memcpy)
#endif
void *memcpy(void *dest, const void *src, unsigned int count)
{
    char *dest8 = (char *)dest;
    const char *src8 = (const char *)src;
    while (count--)
    {
        *dest8++ = *src8++;
    }
    return dest;
}

/*
Avoid in freestanding build: lld-link: error: undefined symbol: _fltused
*/
int _fltused=0;

#define assert(expression)      \
    if (!(expression))          \
    {                           \
        *(volatile int *)0 = 0; \
    }

/*
    Simple win32 print function without using the "windows.h" include since it slows
    down build time
*/
#include "win32_print.h"

/*
    For simplicity we use a fixed size arena memory for all dyn_array allocations.
    The arena uses win32 VirtualAlloc internally
*/
#include "arena.h"
static arena permanentMemory;

#define DYN_ARRAY_FUNCTION_REALLOC(p, s) (arena_realloc(&permanentMemory, p, s))
#define DYN_ARRAY_FUNCTION_FREE(p) ((void)p) /* We use an fixed size arena so there is no need for freeing dyn_array_memory */
#define DYN_ARRAY_COLLECT_STATISTICS
#include "../dyn_array.h"

#ifdef __clang__
#elif __GNUC__
__attribute((externally_visible))
#endif
#ifdef __i686__
__attribute((force_align_arg_pointer))
#endif
int
mainCRTStartup(void)
{
    unsigned int i;
    float *myNumbers = ARENA_NULL;

    /* Initialize arena for 1MB */
    if (!arena_init(&permanentMemory, 1024 * 1024 * 1))
    {
        win32_printf("[win32][nostdlib] arena allocation failed!\n");
    }

    /* dyn_array usage example */
    dyn_array_init(myNumbers, 16);

    assert(dyn_array_capacity(myNumbers) == 16);
    assert(dyn_array_length(myNumbers) == 0);

    for (i = 0; i < 10; ++i)
    {
        dyn_array_add(myNumbers, 1.0f + (float)i);
    }

    assert(dyn_array_length(myNumbers) == 10);

    for (i = 0; i < dyn_array_length(myNumbers); ++i)
    {
        assert(myNumbers[i] == 1.0f + (float)i);
    }

    for (i = 0; i < 10; ++i)
    {
        dyn_array_add(myNumbers, (float)i);
    }

    assert(dyn_array_length(myNumbers) == 20);

    dyn_array_free(myNumbers);

    assert(!myNumbers);

    win32_printf("[win32][nostdlib] dyn_array example finished\n");

    return 0;
}

/*
   ------------------------------------------------------------------------------
   This software is available under 2 licenses -- choose whichever you prefer.
   ------------------------------------------------------------------------------
   ALTERNATIVE A - MIT License
   Copyright (c) 2025 nickscha
   Permission is hereby granted, free of charge, to any person obtaining a copy of
   this software and associated documentation files (the "Software"), to deal in
   the Software without restriction, including without limitation the rights to
   use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
   of the Software, and to permit persons to whom the Software is furnished to do
   so, subject to the following conditions:
   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.
   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
   ------------------------------------------------------------------------------
   ALTERNATIVE B - Public Domain (www.unlicense.org)
   This is free and unencumbered software released into the public domain.
   Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
   software, either in source code form or as a compiled binary, for any purpose,
   commercial or non-commercial, and by any means.
   In jurisdictions that recognize copyright laws, the author or authors of this
   software dedicate any and all copyright interest in the software to the public
   domain. We make this dedication for the benefit of the public at large and to
   the detriment of our heirs and successors. We intend this dedication to be an
   overt act of relinquishment in perpetuity of all present and future rights to
   this software under copyright law.
   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
   ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
   WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
   ------------------------------------------------------------------------------
*/
