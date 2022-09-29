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
#include <cstdlib>
#include <cstring>

/* spnm - https://github.com/mrsafalpiya/spnm */
#define SPNM_IMPL
#include "../spnm.hpp"

/* spl - https://github.com/mrsafalpiya/spl */
#include "dep/spl_flags.h"
#define SPLU_IMPL
#include "dep/spl_utils.h"

/*
 ===============================================================================
 |                                   Macros                                    |
 ===============================================================================
 */

/* = CORE = */

#define PARSE_PROCESS(PROCESS_T)                                             \
	if (!strcmp(process, "decimal_places") || !strcmp(process, "1"))     \
		PROCESS_T = spnm::DECIMAL_PLACES;                            \
	else if (!strcmp(process, "significant_digits") ||                   \
	         !strcmp(process, "2"))                                      \
		PROCESS_T = spnm::SIGNIFICANT_DIGITS;                        \
	else if (!strcmp(process, "no_of_steps") || !strcmp(process, "3")) { \
		PROCESS_T = spnm::NO_OF_STEPS;                               \
	} else {                                                             \
		fprintf(stderr, "[ERROR] Invalid process given.\n");         \
		print_usage(stderr);                                         \
	}

/* = CLI = */

#define USAGE_STR                                                                                                        \
	"Usage: %s topic process inputs...\n"                                                                            \
	"\n"                                                                                                             \
	"where 'topic' can be\n"                                                                                         \
	"- 'solution_of_nonlinear_equations'/'1'\n"                                                                      \
	"\n"                                                                                                             \
	"'process' can be\n"                                                                                             \
	"- Solution of Nonlinear Equations: 'bisection'/'1', 'secant'/'2'\n"                                             \
	"\n"                                                                                                             \
	"'inputs' can be\n"                                                                                              \
	"- For \"Solution of Nonlinear Equations\", \"Bisection\": fx interval_lower interval_upper process process_n\n" \
	"- For \"Solution of Nonlinear Equations\", \"Secant\": fx initial_point_1 initial_point_2 process process_n\n"  \
	"\n"                                                                                                             \
	"'process' can be\n"                                                                                             \
	"- 'decimal_places'/'1'\n"                                                                                       \
	"- 'significant_digits'/'2'\n"                                                                                   \
	"- 'no_of_steps'/'3'\n"                                                                                          \
	"\n"                                                                                                             \
	"Available flags are:\n",                                                                                        \
		prog

/*
 ===============================================================================
 |                              Global Variables                               |
 ===============================================================================
 */

char *prog;

/*
 ===============================================================================
 |                            Function Declarations                            |
 ===============================================================================
 */

/* = SPNM = */

/* == 1 - solution of nonlinear equations == */

void
perform_bisection(const char *fx, const char *interval_lower,
                  const char *interval_upper, const char *process,
                  const char *process_n);

void
perform_secant(const char *fx, const char *initial_point1,
               const char *initial_point2, const char *process,
               const char *process_n);

/* = CORE = */

void
print_usage(FILE *stream);

/*
 ===============================================================================
 |                          Function Implementations                           |
 ===============================================================================
 */

/* = SPNM = */

/* == 1 - solution of nonlinear equations == */

void
perform_bisection(const char *fx, const char *interval_lower,
                  const char *interval_upper, const char *process,
                  const char *process_n)
{
	/* parse process */
	spnm::process_t process_input;
	PARSE_PROCESS(process_input);

	/* init */
	spnm::bisection_t bisection_instance =
		spnm::bisection_init(fx, interval_lower, interval_upper,
	                             process_input, atoi(process_n));

	/* perform the process */
	int ret;
	if ((ret = spnm::bisection_perform(&bisection_instance)) != 0) {
		switch (ret) {
		case spnm::err_fx_parse: {
			std::cout << "[ERROR] Invalid function input."
				  << std::endl
				  << std::endl;
			std::cout << bisection_instance.log.str() << std::endl;
		} break;
		case spnm::err_x_input: {
			std::cout << "[ERROR] Invalid inputs on interval."
				  << std::endl
				  << std::endl;
			std::cout << bisection_instance.log.str() << std::endl;
		} break;
		case spnm::err_condition_not_met: {
			std::cout
				<< "[ERROR] The two intervals don't met the conditions for bisection of f(a).f(b) < 0."
				<< std::endl;
		} break;
		default:
			std::cout << "[ERROR] Unknown error." << std::endl
				  << std::endl;
			std::cout << bisection_instance.log.str() << std::endl;
		}
		exit(EXIT_FAILURE);
	}

	/* output */
	for (size_t i = 0; i < bisection_instance.steps_c; i++) {
		printf("%lu\t%s\t%c\t%s\t%c\t%s\t%c\n",
		       bisection_instance.steps[i].n,
		       bisection_instance.steps[i].a_n,
		       bisection_instance.steps[i].fa_n_sign,
		       bisection_instance.steps[i].b_n,
		       bisection_instance.steps[i].fb_n_sign,
		       bisection_instance.steps[i].c_n,
		       bisection_instance.steps[i].fc_n_sign);
	}

	spnm::bisection_free(&bisection_instance);
}

void
perform_secant(const char *fx, const char *initial_point1,
               const char *initial_point2, const char *process,
               const char *process_n)
{
	/* parse process */
	spnm::process_t process_input;
	PARSE_PROCESS(process_input);

	/* init */
	spnm::secant_t secant_instance =
		spnm::secant_init(fx, initial_point1, initial_point2,
	                          process_input, atoi(process_n));

	/* perform the process */
	int ret;
	if ((ret = spnm::secant_perform(&secant_instance)) != 0) {
		switch (ret) {
		case spnm::err_fx_parse: {
			std::cout << "[ERROR] Invalid function input."
				  << std::endl
				  << std::endl;
			std::cout << secant_instance.log.str() << std::endl;
		} break;
		case spnm::err_x_input: {
			std::cout << "[ERROR] Invalid inputs on initial point."
				  << std::endl
				  << std::endl;
			std::cout << secant_instance.log.str() << std::endl;
		} break;
		default:
			std::cout << "[ERROR] Unknown error." << std::endl
				  << std::endl;
			std::cout << secant_instance.log.str() << std::endl;
		}
		exit(EXIT_FAILURE);
	}

	/* output */
	for (size_t i = 0; i < secant_instance.steps_c; i++) {
		printf("%lu\t%s\t%s\t%s\t%s\t%s\t%s\n",
		       secant_instance.steps[i].n,
		       secant_instance.steps[i].x_prev,
		       secant_instance.steps[i].fx_prev,
		       secant_instance.steps[i].x_n,
		       secant_instance.steps[i].fx_n,
		       secant_instance.steps[i].x_next,
		       secant_instance.steps[i].fx_next);
	}

	spnm::secant_free(&secant_instance);
}

/* = CORE = */

void
print_usage(FILE *stream)
{
	int exit_code = EXIT_SUCCESS;
	if (stream == stderr)
		exit_code = EXIT_FAILURE;

	fprintf(stream, USAGE_STR);
	spl_flag_print_help(stream);
	exit(exit_code);
}

int
main(int argc, char **argv)
{
	prog = argv[0];

	/* = FLAGS = */

	/* initialize values on flags */
	int to_print_help = 0;

	/* set flags */
	spl_flag_toggle(&to_print_help, 'h', "help", "Print the help message");

	spl_flag_info f_info = spl_flag_parse(argc, argv);

	/* printing any gotchas in parsing */
	if (spl_flag_print_gotchas(f_info, stderr))
		exit(EXIT_FAILURE);

	/* check if -h flag was passed */
	if (to_print_help)
		print_usage(stdout);

	/* = INIT = */

	spnm::init();

	/* = ARGUMENT PARSING = */

	if (f_info.non_flag_arguments_c < 3)
		print_usage(stderr);

	char *topic   = f_info.non_flag_arguments[0];
	char *process = f_info.non_flag_arguments[1];
	char *input1  = f_info.non_flag_arguments[2];
	char *input2  = f_info.non_flag_arguments[3];
	char *input3  = f_info.non_flag_arguments[4];
	char *input4  = f_info.non_flag_arguments[5];
	char *input5  = f_info.non_flag_arguments[6];

	/* = TOPIC SELECTION = */

	/* Solution of Nonlinear Equations */
	if (!strcmp(topic, "solution_of_nonlinear_equations") ||
	    !strcmp(topic, "1")) {
		if (!strcmp(process, "bisection") || !strcmp(process, "1")) {
			if (f_info.non_flag_arguments_c < (2 + 5))
				print_usage(stderr);
			spl_flag_warn_ignored_args(f_info, stderr, 2 + 5);

			perform_bisection(input1, input2, input3, input4,
			                  input5);

			exit(EXIT_SUCCESS);
		}

		if (!strcmp(process, "secant") || !strcmp(process, "2")) {
			if (f_info.non_flag_arguments_c < (2 + 5))
				print_usage(stderr);
			spl_flag_warn_ignored_args(f_info, stderr, 2 + 5);

			perform_secant(input1, input2, input3, input4, input5);

			exit(EXIT_SUCCESS);
		}
	}

	/* = EXIT = */

	return EXIT_SUCCESS;
}
