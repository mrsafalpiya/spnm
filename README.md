# spnm - C++ Library for Numerical Methods analysis

## Inspiration

The numerical methods subject in my univ course is a bit tedious such that
mistakes in calculation is quite often to say the least. Using this library one
can get step by step answer with proper rounding off. So comparison of answer
is easy and mistakes are easy to follow.

## Contents in this repo

1. The main library (single header file): `spnm.hpp` along with its dependencies inside `dep/`.
2. A cli demo: `cli/`.
3. A simple testing tool: `test/`.

## Compiling

In each of the directories, simply run `make release` or `make` to compile with
debug symbols.

## Demo

### CLI

- Q. Find a solution of $e^x - x - 2 = 0$ correct upto 4 significant digits in the interval `[1, 2]` using bisection method.

Ran using `./bin/spnm-cli 1 1 "e^x -x - 2" 1 2 2 4`:

```
1       1       n       2       p       1.5     p
2       1       n       1.5     p       1.25    p
3       1       n       1.25    p       1.125   n
4       1.125   n       1.25    p       1.1875  p
5       1.125   n       1.1875  p       1.1562  p
6       1.125   n       1.1562  p       1.1406  n
7       1.1406  n       1.1562  p       1.1484  p
8       1.1406  n       1.1484  p       1.1445  n
9       1.1445  n       1.1484  p       1.1464  p
10      1.1445  n       1.1464  p       1.1454  n
11      1.1454  n       1.1464  p       1.1459  n
```

# License

GPL-3.0-or-later
