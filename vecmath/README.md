Trying to optimize taking the exponential of _many_ entries in a vector.

Three different ways:

1.  Normal using the compiler to vectorize the loop
2.  Using FMath from https://github.com/herumi/fmath/
3.  Using Vector Class from: www.agner.org/optimize

## Licensing

My code here (in `test.C`) is public domain - feel free to copy.

Most of the rest of it is GPL see license.txt

`fmath.h` uses a modified BSD-3-Clause license.  See `fmath.h` for details
