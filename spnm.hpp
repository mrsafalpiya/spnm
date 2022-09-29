/*
 ===============================================================================
 |                         License - GPL-3.0-or-later                          |
 ===============================================================================
 *
 * spnm - C++ Library for Numerical Methods analysis
 * Copyright (C) 2022  Safal Piya
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/*
 ===============================================================================
 |                                   spnm.hpp                                  |
 |                     https://github.com/mrsafalpiya/spnm                     |
 |                                                                             |
 |                Library for solving numerical method problems                |
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
 */

/*
 ===============================================================================
 |                                Prerequisites                                |
 ===============================================================================
 *
 * - libgiac - https://www-fourier.ujf-grenoble.fr/~parisse/giac_us.html
 * - spl_maths.h - https://github.com/mrsafalpiya/spl
 */

/*
 ===============================================================================
 |                                    Usage                                    |
 ===============================================================================
 *
 * Do this:
 *
 *         #define SPNM_IMPL
 *
 * before you include this file in *one* C or C++ file to create the
 * implementation.
 *
 * - Link with libgiac: '-lgiac -lgmp'
 */

#ifndef SPNM_HPP
#define SPNM_HPP

#include <cstddef>
#include <string>

/* giac - https://www-fourier.ujf-grenoble.fr/~parisse/giac_us.html */
#include <giac/config.h>
#include <giac/giac.h>

/*
 ===============================================================================
 |                              HEADER-FILE MODE                               |
 ===============================================================================
 */

/*
 ===============================================================================
 |                                   Options                                   |
 ===============================================================================
 */

/* = SPNM = */

namespace spnm
{

/*
 ===============================================================================
 |                                  Constants                                  |
 ===============================================================================
 */

const size_t STEPS_MAX = 100;
const size_t STR_MAX   = 100;

/* = ERRORS = */

const int err_fx_parse          = 1;
const int err_x_input           = 2;
const int err_condition_not_met = 3;

/*
 ===============================================================================
 |                                    Enums                                    |
 ===============================================================================
 */

enum process_t {
	DECIMAL_PLACES,     /* correct upto n decimal places */
	SIGNIFICANT_DIGITS, /* correct upto n significant digits */
	NO_OF_STEPS,        /* perform n number of steps */
};

/*
 ===============================================================================
 |                                   Structs                                   |
 ===============================================================================
 */

/* = 1 - SOLUTION OF NONLINEAR EQUATIONS = */

/* == 1. bisection method == */

/* === bisection step === */
struct bisection_s {
	size_t n;         /* step number */
	char  *a_n;       /* string representation of a_n */
	char   fa_n_sign; /* '-': -ve, '+': +ve */
	char  *b_n;       /* string representation of b_n */
	char   fb_n_sign; /* '-': -ve, '+': +ve */
	char  *c_n;       /* string representation of c_n */
	char   fc_n_sign; /* '-': -ve, '+': +ve */
};

struct bisection_t {
	/* === input === */
	char     *fx;             /* input function */
	char     *interval_lower; /* lower interval */
	char     *interval_upper; /* upper interval */
	process_t process;        /* how to perform the calculation */
	size_t    process_n;      /* degree of n for the given process */

	/* === output === */
	size_t      steps_c; /* number of steps required for the process */
	bisection_s steps[STEPS_MAX]; /* array of all steps required */
	std::stringstream log;        /* output log */
};

/* == 2. secant method == */

/* === secant step === */
struct secant_s {
	size_t n;       /* step number */
	char  *x_prev;  /* string representation of x_(n-1) */
	char  *fx_prev; /* string representation of f(x_(n-1)) */
	char  *x_n;     /* string representation of x_n */
	char  *fx_n;    /* string representation of f(x_n) */
	char  *x_next;  /* string representation of x_(n+1) */
	char  *fx_next; /* string representation of f(x_(n+1)) */
};

struct secant_t {
	/* === input === */
	char     *fx;             /* input function */
	char     *initial_point1; /* first initial point */
	char     *initial_point2; /* second initial point */
	process_t process;        /* how to perform the calculation */
	size_t    process_n;      /* degree of n for the given process */

	/* === output === */
	size_t   steps_c; /* number of steps required for the process */
	secant_s steps[STEPS_MAX]; /* array of all steps required */
	std::stringstream log;     /* output log */
};

/*
 ===============================================================================
 |                            Function Declarations                            |
 ===============================================================================
 */

/* = CORE = */

/*
 * Initializes any prerequisites to use the library.
 *
 * Should be called before calling any other functions.
 *
 * Can be called multiple times.
 *
 * Returns 0 on error.
 */
int
init(void);

/* = 1 - SOLUTION OF NONLINEAR EQUATIONS = */

/* == 1. bisection method == */

/*
 * Initializes a 'bisection_t' with the required values for the given options in
 * argument.
 *
 * Returns the initialized 'bisection_t'.
 */
bisection_t
bisection_init(const char *fx, const char *interval_lower,
               const char *interval_upper, process_t process, size_t process_n);

/*
 * Performs the bisection calculation for the given inputs in `t`.
 *
 * Returns 0 on success and an error code on failure. The error codes can be:
 * - err_fx_parse: Invalid input in f(x).
 * - err_x_input: Invalid inputs in interval.
 * - err_condition_not_met: f(a).f(b) > 0
 */
int
bisection_perform(bisection_t *t);

/* Free's dynamically allocated resources from the struct. */
void
bisection_free(bisection_t *t);

/* == 2. secant method == */

/*
 * Initializes a 'secant_t' with the required values for the given options in
 * argument.
 *
 * Returns the initialized 'secant_t'.
 */
secant_t
secant_init(const char *fx, const char *initial_point1,
            const char *initial_point2, process_t process, size_t process_n);

/*
 * Performs the secant calculation for the given inputs in `t`.
 *
 * Returns 0 on success and an error code on failure. The error codes can be:
 * - err_fx_parse: Invalid input in f(x).
 * - err_x_input: Invalid inputs in initial point.
 */
int
secant_perform(secant_t *t);

/* Free's dynamically allocated resources from the struct. */
void
secant_free(secant_t *t);

} /* namespace spnm */

/*
 ===============================================================================
 |                                    Utils                                    |
 ===============================================================================
 */

namespace spnm_utils
{

/* = STRING = */

char *
strdup(const char *str);

/* = GIAC = */

/* Returns 0 on error. */
int
giac_fx_parse(const char *fx, giac::context *ct);

/* Returns an empty string "" on error. */
std::string
giac_fx_val_string(const char *x, giac::context *ct);

/* Returns NaN on error. */
double
giac_fx_val_double(const char *x, giac::context *ct);

/* Returns '*' on error. */
char
giac_fx_val_sign(const char *x, giac::context *ct);

/* = 1 - SOLUTION OF NONLINEAR EQUATIONS = */

/* == 1. bisection == */

/* Returns an empty string "" on error. */
std::string
bisection_get_c(const char *a, const char *b);

/* == 2. secant == */

/* Returns an empty string "" on error. */
std::string
secant_get_x_next(const char *x_prev, const char *fx_prev, const char *x_n,
                  const char *fx_n);

} // namespace spnm_utils

#endif /* SPNM_H */

/*
 ===============================================================================
 |                             IMPLEMENTATION MODE                             |
 ===============================================================================
 */

#ifdef SPNM_IMPL

#include <cstdlib>
#include <cstring>
#include <cmath>

/* spl - https://github.com/mrsafalpiya/spl */
#define SPL_MATHS_IMPL
#include "dep/spl_maths.h"

/*
 ===============================================================================
 |                                   Macros                                    |
 ===============================================================================
 */

/* Performs roundoff on `VAR` with respect to decimal places or significant
 * digits according to the given `PROCESS` which is a 'spnm_process' and
 * `PROCESS_N` which is the degree of n for the given process. */
#define SPNM_MANIP(PROCESS, PROCESS_N, VAR)                       \
	switch (PROCESS) {                                        \
	case DECIMAL_PLACES: {                                    \
		spl_maths_roundoff_deci_str(VAR, PROCESS_N + 1);  \
	} break;                                                  \
	case SIGNIFICANT_DIGITS: {                                \
		spl_maths_roundoff_signi_str(VAR, PROCESS_N + 1); \
	} break;                                                  \
	case NO_OF_STEPS: {                                       \
		spl_maths_roundoff_deci_str(VAR, 6);              \
	} break;                                                  \
	}

#define SPNM_CMP(PROCESS, PROCESS_N, VAR1, VAR2)                           \
	switch (PROCESS) {                                                 \
	case DECIMAL_PLACES: {                                             \
		if (spl_maths_is_equal_deci_str(VAR1, VAR2, PROCESS_N)) {  \
			done = 1;                                          \
			continue;                                          \
		}                                                          \
	} break;                                                           \
	case SIGNIFICANT_DIGITS: {                                         \
		if (spl_maths_is_equal_signi_str(VAR1, VAR2, PROCESS_N)) { \
			done = 1;                                          \
			continue;                                          \
		}                                                          \
	} break;                                                           \
	case NO_OF_STEPS: {                                                \
		if (steps_c == PROCESS_N) {                                \
			done = 1;                                          \
			continue;                                          \
		}                                                          \
	} break;                                                           \
	}

namespace spnm
{

/*
 ===============================================================================
 |                          Function Implementations                           |
 ===============================================================================
 */

/* = CORE = */

int
init(void)
{
	return 1;
}

/* = 1 - SOLUTION OF NONLINEAR EQUATIONS = */

/* == 1. bisection method == */

bisection_t
bisection_init(const char *fx, const char *interval_lower,
               const char *interval_upper, process_t process, size_t process_n)
{
	bisection_t ret;

	ret.fx             = spnm_utils::strdup(fx);
	ret.interval_lower = spnm_utils::strdup(interval_lower);
	ret.interval_upper = spnm_utils::strdup(interval_upper);
	ret.process        = process;
	ret.process_n      = process_n;

	ret.steps_c = 0;

	return ret;
}

int
bisection_perform(bisection_t *t)
{
	/* = INIT GIAC = */

	giac::context ct;
	giac::logptr(&(t->log), &ct);

	if (!spnm_utils::giac_fx_parse(t->fx, &ct))
		return err_fx_parse;

	/* = CHECK IF CONDITION FOR BISECTION METHOD IS MET = */

	char *a_n = spnm_utils::strdup(t->interval_lower);
	char *b_n = spnm_utils::strdup(t->interval_upper);

	char fa_sign = spnm_utils::giac_fx_val_sign(t->interval_lower, &ct);
	char fb_sign = spnm_utils::giac_fx_val_sign(t->interval_upper, &ct);
	if (fa_sign == '*' || fb_sign == '*')
		return err_x_input;
	if (fa_sign == fb_sign)
		return err_condition_not_met;

	/* = BISECTION PROCESS = */
	size_t steps_c = 0;
	int    done    = 0;
	while (!done) {
		bisection_s *cur_step = t->steps + steps_c;

		/* arithmetic */
		SPNM_MANIP(t->process, t->process_n, a_n);
		SPNM_MANIP(t->process, t->process_n, b_n);

		std::string c_n_string = spnm_utils::bisection_get_c(a_n, b_n);
		if (c_n_string == "")
			return err_x_input;
		char *c_n = spnm_utils::strdup(c_n_string.c_str());
		SPNM_MANIP(t->process, t->process_n, c_n);
		char fc_sign = spnm_utils::giac_fx_val_sign(c_n, &ct);
		if (fc_sign == '*')
			return err_x_input;

		/* filling data */
		cur_step->n         = steps_c + 1;
		cur_step->a_n       = a_n;
		cur_step->b_n       = b_n;
		cur_step->fa_n_sign = fa_sign;
		cur_step->fb_n_sign = fb_sign;
		cur_step->c_n       = c_n;
		cur_step->fc_n_sign = fc_sign;

		steps_c++;

		if (steps_c != 1) /* avoid checking on first step */
			/* following macro modifies 'done' */
			SPNM_CMP(t->process, t->process_n, cur_step->c_n,
			         (t->steps + (steps_c - 2))->c_n)

		/* for next step */
		if (cur_step->fc_n_sign == cur_step->fa_n_sign) {
			a_n = spnm_utils::strdup(c_n);
			b_n = spnm_utils::strdup(b_n);
		} else {
			a_n = spnm_utils::strdup(a_n);
			b_n = spnm_utils::strdup(c_n);
		}
	}
	t->steps_c = steps_c;

	return 0;
}

/* Free's dynamically allocated resources from the struct. */
void
bisection_free(bisection_t *t)
{
	free(t->fx);
	free(t->interval_lower);
	free(t->interval_upper);

	for (size_t i = 0; i < t->steps_c; i++) {
		free(t->steps[i].a_n);
		free(t->steps[i].b_n);
		free(t->steps[i].c_n);
	}
}

/* == 2. secant method == */

secant_t
secant_init(const char *fx, const char *initial_point1,
            const char *initial_point2, process_t process, size_t process_n)
{
	secant_t ret;

	ret.fx             = spnm_utils::strdup(fx);
	ret.initial_point1 = spnm_utils::strdup(initial_point1);
	ret.initial_point2 = spnm_utils::strdup(initial_point2);
	ret.process        = process;
	ret.process_n      = process_n;

	ret.steps_c = 0;

	return ret;
}

int
secant_perform(secant_t *t)
{
	/* = INIT GIAC = */

	giac::context ct;
	giac::logptr(&(t->log), &ct);

	if (!spnm_utils::giac_fx_parse(t->fx, &ct))
		return err_fx_parse;

	/* = FOR FIRST STEP = */

	char *x_prev = spnm_utils::strdup(t->initial_point1);
	char *x_n    = spnm_utils::strdup(t->initial_point2);
	SPNM_MANIP(t->process, t->process_n, x_prev);
	SPNM_MANIP(t->process, t->process_n, x_n);

	std::string fx_prev_string =
		spnm_utils::giac_fx_val_string(x_prev, &ct);
	if (fx_prev_string == "")
		return err_x_input;
	char       *fx_prev     = spnm_utils::strdup(fx_prev_string.c_str());
	std::string fx_n_string = spnm_utils::giac_fx_val_string(x_n, &ct);
	if (fx_n_string == "")
		return err_x_input;
	char *fx_n = spnm_utils::strdup(fx_n_string.c_str());
	SPNM_MANIP(t->process, t->process_n, fx_prev);
	SPNM_MANIP(t->process, t->process_n, fx_n);

	/* = SECANT PROCESS = */
	size_t steps_c = 0;
	int    done    = 0;
	while (!done) {
		secant_s *cur_step = t->steps + steps_c;

		/* arithmetic */
		std::string x_next_string = spnm_utils::secant_get_x_next(
			x_prev, fx_prev, x_n, fx_n);
		if (x_next_string == "")
			return err_x_input;
		char *x_next = spnm_utils::strdup(x_next_string.c_str());
		SPNM_MANIP(t->process, t->process_n, x_next);

		std::string fx_next_string =
			spnm_utils::giac_fx_val_string(x_next, &ct);
		if (fx_next_string == "")
			return err_x_input;
		char *fx_next = spnm_utils::strdup(fx_next_string.c_str());
		SPNM_MANIP(t->process, t->process_n, fx_next);

		/* filling data */
		cur_step->n       = steps_c + 1;
		cur_step->x_prev  = x_prev;
		cur_step->fx_prev = fx_prev;
		cur_step->x_n     = x_n;
		cur_step->fx_n    = fx_n;
		cur_step->x_next  = x_next;
		cur_step->fx_next = fx_next;

		steps_c++;

		if (steps_c != 1) /* avoid checking on first step */
			/* following macro modifies 'done' */
			SPNM_CMP(t->process, t->process_n, cur_step->x_next,
			         (t->steps + (steps_c - 2))->x_next)

		/* for next step */
		x_prev  = x_n;
		fx_prev = fx_n;
		x_n     = x_next;
		fx_n    = fx_next;
	}
	t->steps_c = steps_c;

	return 0;
}

void
secant_free(secant_t *t)
{
	free(t->fx);
	free(t->initial_point1);
	free(t->initial_point2);

	size_t i;
	for (i = 0; i < t->steps_c; i++) {
		free(t->steps[i].x_prev);
		free(t->steps[i].fx_prev);
	}
	free(t->steps[i - 1].x_n);
	free(t->steps[i - 1].fx_n);
	free(t->steps[i - 1].x_next);
	free(t->steps[i - 1].fx_next);
}

} /* namespace spnm */

/*
 ===============================================================================
 |                                    Utils                                    |
 ===============================================================================
 */

namespace spnm_utils
{

/* = STRING = */

char *
strdup(const char *str)
{
	size_t str_len = strlen(str) + 1;

	char *out = (char *)malloc(str_len);
	memcpy(out, str, str_len);

	return out;
}

/* = GIAC = */

int
giac_fx_parse(const char *fx, giac::context *ct)
{
	/* construct f(x) */
	std::string expr = "f(x) := ";
	expr += fx;

	giac::eval(giac::gen(expr, ct), 1, ct);
	if (giac::first_error_line(ct))
		return 0;
	return 1;
}

std::string
giac_fx_val_string(const char *x, giac::context *ct)
{
	/* creating string off the double makes it easier to deal with very
	 * small numbers -- avoid 6e-09 case for example. */
	double val = giac_fx_val_double(x, ct);
	if (std::isnan(val))
		return "";

	/* get for example "4.000000" as "4" */
	if (std::floor(val) == val)
		return std::to_string((int)val);
	return std::to_string(val);
}

double
giac_fx_val_double(const char *x, giac::context *ct)
{
	std::string input = "approx(f(";
	input += x;
	input += "))";

	giac::gen f = giac::eval(giac::gen(input, ct), 1, ct);

	return f.to_double(ct);
}

char
giac_fx_val_sign(const char *x, giac::context *ct)
{
	double val = giac_fx_val_double(x, ct);
	if (std::isnan(val))
		return '*';

	if (val < 0)
		return '-';
	return '+';
}

/* = 1 - SOLUTION OF NONLINEAR EQUATIONS = */

/* == 1. bisection == */

std::string
bisection_get_c(const char *a, const char *b)
{
	giac::context ct;

	/* construct formula */
	std::string formula = "approx(((";
	formula += a;
	formula += ") + (";
	formula += b;
	formula += ")) / 2)";

	giac::gen c = giac::eval(giac::gen(formula, &ct), 1, &ct);
	if (giac::first_error_line(&ct))
		return "";

	return c.print();
}

/* == 2. secant == */

std::string
secant_get_x_next(const char *x_prev, const char *fx_prev, const char *x_n,
                  const char *fx_n)
{
	giac::context ct;

	/* construct formula */
	std::string formula = "approx((((";
	formula += x_prev;
	formula += ") * (";
	formula += fx_n;
	formula += ")) - ((";
	formula += x_n;
	formula += ") * (";
	formula += fx_prev;
	formula += "))) / ((";
	formula += fx_n;
	formula += ") - (";
	formula += fx_prev;
	formula += ")))";

	giac::gen c = giac::eval(giac::gen(formula, &ct), 1, &ct);
	if (giac::first_error_line(&ct))
		return "";

	return c.print();
}

} /* namespace spnm_utils */

#endif /* SPNM_IMPL */
