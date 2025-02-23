/* dyn_array.h - v0.1 - public domain data structures - nickscha 2025

A C89 standard compliant, single header, nostdlib (no C Standard Library) dynamic generic array implementation.

This Test class defines cases to verify that we don't break the excepted behaviours in the future upon changes.

LICENSE

  Placed in the public domain and also MIT licensed.
  See end of file for detailed license information.

*/
#define DYN_ARRAY_COLLECT_STATISTICS
#include "../dyn_array.h"

#include "test.h" /* Simple Testing framework */

void dyn_array_test_simple_type(void)
{
    unsigned int i;

    double *myNumbers = NULL;

    dyn_array_stats_reset();

    dyn_array_init(myNumbers, 2);

    assert(dyn_array_capacity(myNumbers) == 2);
    assert(dyn_array_length(myNumbers) == 0);

    dyn_array_add(myNumbers, 1.0);

    assert(dyn_array_capacity(myNumbers) == 2);
    assert(dyn_array_length(myNumbers) == 1);

    dyn_array_add(myNumbers, 2.0);

    assert(dyn_array_capacity(myNumbers) == 2);
    assert(dyn_array_length(myNumbers) == 2);

    dyn_array_add(myNumbers, 3.0);

    assert(dyn_array_capacity(myNumbers) == 4);
    assert(dyn_array_length(myNumbers) == 3);
    assert(dyn_array_last(myNumbers) == 3.0);

    dyn_array_del(myNumbers);

    assert(dyn_array_capacity(myNumbers) == 4);
    assert(dyn_array_length(myNumbers) == 2);
    assert(dyn_array_last(myNumbers) == 2.0);

    for (i = 0; i < dyn_array_length(myNumbers); ++i)
    {
        assert(myNumbers[i] == 1.0 + i);
    }

    dyn_array_del(myNumbers);
    dyn_array_del(myNumbers);

    assert(dyn_array_capacity(myNumbers) == 4);
    assert(dyn_array_length(myNumbers) == 0);

    /* length is already 0. check if calling delete will not decrease the length further */
    dyn_array_del(myNumbers);

    assert(dyn_array_length(myNumbers) == 0);

    dyn_array_free(myNumbers);

    assert(!myNumbers);
    assert(dyn_array_stats_init == 1);
    assert(dyn_array_stats_realloc == 2);
    assert(dyn_array_stats_grow_with_factor == 1);
    assert(dyn_array_stats_free == 1);
    assert(dyn_array_stats_init - dyn_array_stats_free == 0);
}

typedef struct point
{
    int x;
    int y;
} point;

void dyn_array_test_complex_type(void)
{
    unsigned int i;

    point *myPoints = NULL;
    point p1 = {3, 6};
    point p2 = {2, 4};

    dyn_array_stats_reset();

    assert(!myPoints);

    dyn_array_add(myPoints, p1);

    assert(dyn_array_capacity(myPoints) == 1);
    assert(dyn_array_length(myPoints) == 1);
    assert(dyn_array_last(myPoints).x == 3);
    assert(dyn_array_last(myPoints).y == 6);

    dyn_array_add(myPoints, p2);

    assert(dyn_array_capacity(myPoints) == 2);
    assert(dyn_array_length(myPoints) == 2);
    assert(dyn_array_last(myPoints).x == 2);
    assert(dyn_array_last(myPoints).y == 4);

    for (i = 0; i < dyn_array_length(myPoints); ++i)
    {
        point current = myPoints[i];
        assert(current.y == current.x * 2);
    }

    dyn_array_free(myPoints);

    assert(!myPoints);
    assert(dyn_array_stats_init == 1);
    assert(dyn_array_stats_realloc == 2);
    assert(dyn_array_stats_grow_with_factor == 2);
    assert(dyn_array_stats_free == 1);
    assert(dyn_array_stats_init - dyn_array_stats_free == 0);
}

void dyn_array_test_big_capacity(void)
{
    unsigned int i;
    unsigned int capacity = 1024;

    int *numbers = NULL;

    dyn_array_stats_reset();

    assert(!numbers);

    dyn_array_init(numbers, capacity);

    assert(dyn_array_capacity(numbers) == capacity);
    assert(dyn_array_length(numbers) == 0);

    for (i = 0; i < capacity; ++i)
    {
        dyn_array_add(numbers, (int)i);
    }

    assert(dyn_array_capacity(numbers) == capacity);
    assert(dyn_array_length(numbers) == capacity);

    dyn_array_free(numbers);

    assert(!numbers);
    assert(dyn_array_stats_init == 1);
    assert(dyn_array_stats_realloc == 1);
    assert(dyn_array_stats_grow_with_factor == 0);
    assert(dyn_array_stats_free == 1);
    assert(dyn_array_stats_init - dyn_array_stats_free == 0);
}

int main(void)
{

    /* dyn_array.h - v0.1 */
    dyn_array_test_simple_type();
    dyn_array_test_complex_type();
    dyn_array_test_big_capacity();

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
