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

#include <iostream>
#include <cstdio>
#include <cstdlib>

/* spnm - https://github.com/mrsafalpiya/spnm */
#define SPNM_IMPL
#include "../spnm.hpp"

#include "test_cases.h"

/* = TEST = */

/* colors */
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define RESET "\x1B[0m"

/* test */
size_t test_failed_c = 0;

/* = 1 - SOLUTION OF NONLINEAR EQUATIONS = */

void
test_bisection(const char *fx, const char *interval_lower,
               const char *interval_upper, spnm::process_t process,
               size_t process_n, const char *correct)
{
	spnm::bisection_t bisection_instance = spnm::bisection_init(
		fx, interval_lower, interval_upper, process, process_n);
	spnm::bisection_perform(&bisection_instance);

	char output_buffer[1024] = "";
	for (size_t i = 0; i < bisection_instance.steps_c; i++) {
		char buffer[1024];
		sprintf(buffer, "%lu\t%s\t%c\t%s\t%c\t%s\t%c\n",
		        bisection_instance.steps[i].n,
		        bisection_instance.steps[i].a_n,
		        bisection_instance.steps[i].fa_n_sign,
		        bisection_instance.steps[i].b_n,
		        bisection_instance.steps[i].fb_n_sign,
		        bisection_instance.steps[i].c_n,
		        bisection_instance.steps[i].fc_n_sign);
		strcat(output_buffer, buffer);
	}

	if (strcmp(output_buffer, correct)) {
		fprintf(stderr, RED "FAILED bisection for '%s':\n" RESET, fx);
		fprintf(stderr, RED "%s" RESET, output_buffer);
		fprintf(stderr, GRN "%s\n" RESET, correct);
		test_failed_c++;
	}
}

void
test_secant(const char *fx, const char *initial_point1,
            const char *initial_point2, spnm::process_t process,
            size_t process_n, const char *correct)
{
	spnm::secant_t secant_instance = spnm::secant_init(
		fx, initial_point1, initial_point2, process, process_n);
	spnm::secant_perform(&secant_instance);

	char output_buffer[1024] = "";
	for (size_t i = 0; i < secant_instance.steps_c; i++) {
		char buffer[1024];
		sprintf(buffer, "%lu\t%s\t%s\t%s\t%s\t%s\t%s\n",
		        secant_instance.steps[i].n,
		        secant_instance.steps[i].x_prev,
		        secant_instance.steps[i].fx_prev,
		        secant_instance.steps[i].x_n,
		        secant_instance.steps[i].fx_n,
		        secant_instance.steps[i].x_next,
		        secant_instance.steps[i].fx_next);
		strcat(output_buffer, buffer);
	}

	if (strcmp(output_buffer, correct)) {
		fprintf(stderr, RED "FAILED secant for '%s':\n" RESET, fx);
		fprintf(stderr, RED "%s" RESET, output_buffer);
		fprintf(stderr, GRN "%s\n" RESET, correct);
		test_failed_c++;
	}
}

int
main(void)
{
	/* = TESTS = */

	/* = 1 - SOLUTION OF NONLINEAR EQUATIONS = */

	/* == 1. bisection method == */

	test_bisection("x^3 - 3x + 1", "0", "1", spnm::DECIMAL_PLACES, 3,
	               correct_bisection_1);
	test_bisection("x^3 - 2 sin(x)", "0.5", "2", spnm::SIGNIFICANT_DIGITS,
	               5, correct_bisection_2);
	test_bisection("x*e^x - 1", "0", "1", spnm::DECIMAL_PLACES, 3,
	               correct_bisection_3);
	test_bisection("e^x - x - 2", "1", "2", spnm::SIGNIFICANT_DIGITS, 4,
	               correct_bisection_4);
	test_bisection("sin(x) - 2x + 1", "0", "1", spnm::DECIMAL_PLACES, 4,
	               correct_bisection_5);
	test_bisection("log(x) - cos(x)", "1", "2", spnm::DECIMAL_PLACES, 3,
	               correct_bisection_6);

	/* == 2. secant method == */

	test_secant("x^3 - 3x + 1", "0", "1", spnm::DECIMAL_PLACES, 3,
	            correct_secant_1);
	test_secant("3x + sin(x) - e^x", "0", "1", spnm::NO_OF_STEPS, 6,
	            correct_secant_2);
	test_secant("x^3 + x^2 - 3x - 3", "1", "2", spnm::SIGNIFICANT_DIGITS, 5,
	            correct_secant_3);
	test_secant("x^5 + x^3 + 3", "1", "-1", spnm::DECIMAL_PLACES, 4,
	            correct_secant_4);
	test_secant("sin(x) - 2x + 1", "0", "1", spnm::DECIMAL_PLACES, 4,
	            correct_secant_5);

	/* = EXIT = */

	if (test_failed_c != 0) {
		fprintf(stderr, RED "Failed %lu test!\n" RESET, test_failed_c);
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
