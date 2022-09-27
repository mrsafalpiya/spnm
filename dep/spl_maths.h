/*
 ===============================================================================
 |                                 spl_maths.h                                 |
 |                     https://github.com/mrsafalpiya/spl                      |
 |                                                                             |
 |                       Library for number manipulation                       |
 |                                                                             |
 |                  No warranty implied; Use at your own risk                  |
 |                  See end of file for license information.                   |
 ===============================================================================
 */

/*
 ===============================================================================
 |                               Version History                               |
 ===============================================================================
 *
 - v0.1 (Current)
     - Released with round off and couple of comparison functions.
 */

/*
 ===============================================================================
 |                                    Usage                                    |
 ===============================================================================
 *
 * Do this:
 *
 *         #define SPL_MATHS_IMPL
 *
 * before you include this file in *one* C or C++ file to create the
 * implementation.
 */

/*
 ===============================================================================
 |                              HEADER-FILE MODE                               |
 ===============================================================================
 */

#ifndef SPL_MATHS_H
#define SPL_MATHS_H

/*
 ===============================================================================
 |                                   Options                                   |
 ===============================================================================
 */

/* = SPL_MATHS = */
#ifndef SPL_MATHS_DEF
#define SPL_MATHS_DEF /* You may want `static` or `static inline` here */
#endif

/*
 ===============================================================================
 |                            Function Declarations                            |
 ===============================================================================
 */

/* = ROUND OFF = */

/*
 * Performs round off on string representation of an integer number.
 *
 * NOTE: The operation is performed on the original string itself.
 *
 * Returns the number of characters manipulated for rounding off (which is
 * useful to trim off trailing 0's).
 */
int
spl_maths_roundoff_int_str(char *input, size_t n);

/*
 * Performs round off on the fractional part of the string representation of a
 * decimal number.
 *
 * NOTE: The operation is performed on the original string itself.
 */
void
spl_maths_roundoff_deci_str(char *input, size_t n);

/*
 * Performs round off on the string representation of a decimal number with
 * respect to significant digits.
 *
 * NOTE: The operation is performed on the original string itself.
 */
void
spl_maths_roundoff_signi_str(char *input, size_t n);

/* = COMPARISON = */

/* Returns 1 if the two inputs are equal up to `n` decimal places. */
int
spl_maths_is_equal_deci_str(const char *input1, const char *input2, size_t n);

/* Returns 1 if the two inputs are equal up to `n` significant digits. */
int
spl_maths_is_equal_signi_str(const char *input1, const char *input2, size_t n);

#endif /* SPL_MATHS_H */

/*
 ===============================================================================
 |                             IMPLEMENTATION MODE                             |
 ===============================================================================
 */

#ifdef SPL_MATHS_IMPL

#include <string.h>
#include <math.h>

/*
 ===============================================================================
 |                          Function Implementations                           |
 ===============================================================================
 */

/* = ROUND OFF = */

int
spl_maths_roundoff_int_str(char *input, size_t n)
{
	if (n == 0)
		return -1;

	size_t round_c       = 0; /* number of digits to chop for rounding */
	int    rounded_digit = *(input + (n - 1)) - '0';
	int    next_digit    = *(input + (n - 1) + 1) - '0';

	if (next_digit >= 5) {
		if (next_digit == 5 && rounded_digit % 2 == 0)
			goto done;

		round_c = 1;
		if (rounded_digit == 9) {
			/* find the next non-9 digit from end */
			char cur_digit;
			while ((cur_digit = *(input + (n - 1) - round_c)) ==
			       '9')
				round_c++;
			/* get the required rounded_digit */
			rounded_digit = (cur_digit - '0') + 1;
			rounded_digit *= pow(10, round_c);

			round_c++;
		} else {
			rounded_digit++;
		}
	}

done:
	if (round_c != 0) {
		char rounded_digit_str[n];
		sprintf(rounded_digit_str, "%d", rounded_digit);
		strncpy(input + (n - 1) - round_c + 1, rounded_digit_str,
		        round_c);
	}
	*(input + n) = '\0';

	return round_c;
}

void
spl_maths_roundoff_deci_str(char *input, size_t n)
{
	if (n == 0)
		return;

	/* check if actually a decimal number */
	char *dot = strchr(input, '.');
	if (!dot)
		return;

	/* check if enough decimal digits are available */
	size_t deci_c = strlen(dot + 1);
	if (deci_c <= n)
		return;

	int round_c = spl_maths_roundoff_int_str(dot + 1, n);

	/* trim trailing 0's */
	*(dot + n - round_c + 2) = '\0';
}

void
spl_maths_roundoff_signi_str(char *input, size_t n)
{
	if (n == 0)
		return;

	char  *dot       = strchr(input, '.');
	size_t integer_c = 0;
	if (dot)
		integer_c = dot - input;
	else
		integer_c = strlen(input);
	if (*input == '-')
		integer_c--;

	/* rounding off just the integer digits */
	if (n == integer_c) {
		/* it's just easier to remove the dot and shift all following
		 * digits */
		for (; *dot != '\0'; dot++) {
			*dot = *(dot + 1);
			spl_maths_roundoff_int_str(input, n);
		}
	}

	if (n < integer_c)
		spl_maths_roundoff_int_str(input, n);
	else
		spl_maths_roundoff_deci_str(input, n - integer_c);
}

/* = COMPARISON = */

/* Returns 1 if the two inputs are equal up to `n` decimal places. */
int
spl_maths_is_equal_deci_str(const char *input1, const char *input2, size_t n)
{
	char *dot_input1 = strchr((char *)input1, '.');
	char *dot_input2 = strchr((char *)input2, '.');

	/* check if invalid inputs were given */
	if (!dot_input1 || !dot_input2 || strlen(dot_input1 + 1) < n ||
	    strlen(dot_input2 + 1) < n)
		return 0;

	for (size_t i = 1; i <= n; i++) {
		if (*(dot_input1 + i) != *(dot_input2 + i))
			return 0;
	}

	return 1;
}

/* Returns 1 if the two inputs are equal up to `n` significant digits. */
int
spl_maths_is_equal_signi_str(const char *input1, const char *input2, size_t n)
{
	char *dot_input1 = strchr((char *)input1, '.');
	char *dot_input2 = strchr((char *)input2, '.');

	size_t len_input1 = strlen(input1);
	size_t len_input2 = strlen(input2);
	if (dot_input1)
		len_input1--;
	if (dot_input2)
		len_input2--;

	/* check if invalid inputs were given */
	if (len_input1 < n || len_input2 < n)
		return 0;

	for (size_t i = 1, j = 0; i <= n; i++, j++) {
		if (*(input1 + j) == '.')
			j++;

		if (*(input1 + j) != *(input2 + j))
			return 0;
	}
	
	return 1;
}

#endif /* SPL_MATHS_IMPL */

/*
 ===============================================================================
 |                 License - Public Domain (www.unlicense.org)                 |
 ===============================================================================
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
 */
