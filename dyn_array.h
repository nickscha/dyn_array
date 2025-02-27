/* dyn_array.h - v0.1 - public domain data structures - nickscha 2025

A C89 standard compliant, single header, nostdlib (no C Standard Library) dynamic generic array implementation.

COMPILE-TIME OPTIONS

  #define DYN_ARRAY_GROW_FACTOR_FUNCTION(c)

    This global flag defines by which factor the dyn_array should grow if it exceeds the previous
    allocated size.

    Default: ((c) * 1.5 - (c))

    The default adds 1.5 times the amount to the existing capacity.
    Because the value of "DYN_ARRAY_GROW_FACTOR_FUNCTION" is added to the current capacity
    the macro substracts the capacity at the end "- (c)".

    Example of how this MACRO is used in the dyn_array implementation:
    capacity += add_length + (DYN_ARRAY_GROW_FACTOR_FUNCTION(capacity));

    Example fixed growth add memory for 16 more elements:
    #define DYN_ARRAY_GROW_FACTOR_FUNCTION(c) (16)
    #include "dyn_array.h"

  #define DYN_ARRAY_FUNCTION_REALLOC
  #define DYN_ARRAY_FUNCTION_FREE

    These TWO flags have to be specified TOGETHER if the default realloc/free from the C standard library
    should not be used!

    Example:
    #define DYN_ARRAY_FUNCTION_REALLOC(p, s) (a_better_realloc_implementation(p, s))
    #define DYN_ARRAY_FUNCTION_FREE(p)       (a_better_free_implementation(p))
    #include "dyn_array.h"

  #define DYN_ARRAY_COLLECT_STATISTICS

    This global flag needs to be set if some statistics should be gathered
    during invocations of this library which can be accessed by the user
    for some additional information.

    If defined the following statistic counts are available:

    dyn_array_stats_init
    dyn_array_stats_realloc
    dyn_array_stats_grow_with_factor
    dyn_array_stats_free

    Example:
    #define DYN_ARRAY_COLLECT_STATISTICS
    #include "dyn_array.h"


USAGE

   double *myNumbers = NULL;     // IMPORTANT: Always initialize!
   dyn_array_init(myNumbers, 8); // OPTIONAL: Initialize with initial capacity of 8 elements

   dyn_array_add(myNumbers, 42.0);
   dyn_array_add(myNumbers, 1337.0);

    for (i = 0; i < dyn_array_length(myNumbers); ++i)
    {
        printf("%g\n", myNumbers[i]);
    }

    printf("   cap: %i\n", dyn_array_capacity(myNumbers));
    printf("length: %i\n", dyn_array_length(myNumbers));

    dyn_array_del(myNumbers);    // Delete last element
    dyn_array_free(myNumbers);   // Free array. Invokes the "DYN_ARRAY_FUNCTION_FREE"


LICENSE

  Placed in the public domain and also MIT licensed.
  See end of file for detailed license information.

*/
#ifndef DYN_ARRAY_H
#define DYN_ARRAY_H

/* #############################################################################
 * # COMPILER SETTINGS
 * #############################################################################
 */
/* Check if using C99 or later (inline is supported) */
#if __STDC_VERSION__ >= 199901L
#define DYN_ARRAY_INLINE inline
#elif defined(__GNUC__) || defined(__clang__)
#define DYN_ARRAY_INLINE __inline__
#elif defined(_MSC_VER)
#define DYN_ARRAY_INLINE __inline
#else
#define DYN_ARRAY_INLINE
#endif

#ifndef DYN_ARRAY_GROW_FACTOR_FUNCTION
#define DYN_ARRAY_GROW_FACTOR_FUNCTION(c) ((c) * 1.5 - (c))
#endif

#if !defined(DYN_ARRAY_FUNCTION_REALLOC) && !defined(DYN_ARRAY_FUNCTION_FREE)
#include <stdlib.h>
#define DYN_ARRAY_FUNCTION_REALLOC(p, s) (realloc(p, s))
#define DYN_ARRAY_FUNCTION_FREE(p) (free(p))
#endif

#ifdef DYN_ARRAY_COLLECT_STATISTICS
#define DYN_ARRAY_STATS(x) x

unsigned int dyn_array_stats_init;
unsigned int dyn_array_stats_realloc;
unsigned int dyn_array_stats_grow_with_factor;
unsigned int dyn_array_stats_free;

static DYN_ARRAY_INLINE void dyn_array_stats_reset(void)
{
  dyn_array_stats_init = 0;
  dyn_array_stats_realloc = 0;
  dyn_array_stats_grow_with_factor = 0;
  dyn_array_stats_free = 0;
}

#else
#define DYN_ARRAY_STATS(x)
#endif

typedef struct dyn_array_header
{
  unsigned int capacity;
  unsigned int length;
  void *data;

} dyn_array_header;

#define DYN_ARRAY_NULL ((void *)0)
#define dyn_array_header(t) ((dyn_array_header *)(t) - 1)
#define dyn_array_capacity(t) ((t) ? dyn_array_header(t)->capacity : 0)
#define dyn_array_length(t) ((t) ? dyn_array_header(t)->length : 0)

static DYN_ARRAY_INLINE void *dyn_array_grow_function(void *type, unsigned int type_size, unsigned int capacity, unsigned int add_length)
{
  void *b;

  if (add_length > 0)
  {
    capacity += add_length + ((unsigned int)(DYN_ARRAY_GROW_FACTOR_FUNCTION(capacity)));
    DYN_ARRAY_STATS(++dyn_array_stats_grow_with_factor);
  }

  b = DYN_ARRAY_FUNCTION_REALLOC((type) ? dyn_array_header(type) : 0, type_size * capacity + sizeof(dyn_array_header));

  if (!b)
  {
    return DYN_ARRAY_NULL;
  }

  b = (char *)b + sizeof(dyn_array_header);

  DYN_ARRAY_STATS(++dyn_array_stats_realloc);

  if (type == DYN_ARRAY_NULL)
  {
    dyn_array_header(b)->length = 0;
    dyn_array_header(b)->data = 0;

    DYN_ARRAY_STATS(++dyn_array_stats_init);
  }

  dyn_array_header(b)->capacity = capacity;

  return (b);
}

static DYN_ARRAY_INLINE void dyn_array_free_function(dyn_array_header *type)
{
  DYN_ARRAY_STATS(++dyn_array_stats_free);
  DYN_ARRAY_FUNCTION_FREE(type);
}

#define dyn_array_grow(t, c, n) ((t) = dyn_array_grow_function((t), sizeof *(t), (c), (n)))
#define dyn_array_grow_check(t, n) ((!(t) || dyn_array_header(t)->length + (n) > dyn_array_header(t)->capacity) ? (dyn_array_grow(t, dyn_array_capacity(t), n), 0) : 0)

#define dyn_array_init(t, c) (dyn_array_grow(t, c, 0))
#define dyn_array_add(t, v) (dyn_array_grow_check(t, 1), (t)[dyn_array_header(t)->length++] = (v))
#define dyn_array_add_array(t, a, c)               \
  do                                               \
  {                                                \
    unsigned int i;                                \
    dyn_array_grow_check(t, c);                    \
    for (i = 0; i < c; ++i)                        \
    {                                              \
      (t)[dyn_array_header(t)->length++] = (a)[i]; \
    }                                              \
  } while (0)
#define dyn_array_del(t) (dyn_array_header(t)->length > 0 ? dyn_array_header(t)->length-- : 0)
#define dyn_array_last(t) ((t)[dyn_array_header(t)->length - 1])
#define dyn_array_free(t) ((void)((t) ? dyn_array_free_function(dyn_array_header(t)) : (void)0), (t) = DYN_ARRAY_NULL)

#endif /* DYN_ARRAY_H */

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
