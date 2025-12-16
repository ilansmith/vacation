#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <getopt.h>
#include <unistd.h>

#include "vacation.h"

#define VACATION_DAYS_PER_YEAR 24 /* default number of annual vacation days */
#define MAX_VACATION_ACCUMULATION_ALLOWED 36 /* max number of permitted 
						accumulated vacation days
						permitted by the company */
enum action {
	ACTION_CALCULATE,
	ACTION_HELP,
	ACTION_INPUT_ERROR,
};

static char *app_name_get(char *arg)
{
	char *app_name;

	app_name = strrchr(arg, '/');
	if (app_name)
		app_name += 1;
	else
		app_name = arg;

	return app_name;
}

static void usage(char *app_name)
{
	printf("%s [h] [-a annual-leave] [-m max-accum] "
		"<current-vacation-due>\n", app_name_get(app_name));
	printf("\n");
	printf("\nWhere:\n");
	printf("  --annual/-a:\n");
	printf("      amount of annual leave in days (default: %d)\n",
		VACATION_DAYS_PER_YEAR);
	printf("  --max/-m:\n");
	printf("      maximum amount of allowed accumulated leave in days "
		"(default: %d)\n", MAX_VACATION_ACCUMULATION_ALLOWED);
	printf("      this value cannot be greater than the total amount of "
		"annual leave\n");
	printf("  --help/-h:\n");
	printf("      print this message and exit\n");

	printf("\n  <current-vacation-due>\n");
	printf("      the amount of vacation currently due in hours as it is"
		" stated in your latest pay slip\n");
}

static void print_vacation_stats(struct vacation_stats *stats)
{
	printf("Current vacation days due: %d", stats->days_due_total);
	if (stats->days_due_accumulated) {
		printf(" (accumulated %d, current year %d)",
			stats->days_due_accumulated, stats->days_due_this_year);
	}
	printf("\n");
	printf("Expected additional days by end of year: %d\n",
		stats->days_remaining_this_year);
	printf("Expected total due at end of year: %d\n",
		stats->total_due_at_end_of_year);
	printf("Expected excess days at end of year: %d\n",
		stats->expected_excess_at_end_of_year);
}

static enum action parse_args(int argc, char **argv,
		struct vacation_params *params)
{
	int vacation_days_per_year;
	int max_vacation_accumulation_allowed;
	double payslip_vacation_hours;
	char *optstring = "ha:m:";
	struct option longopts[] = {
		{
			.name = "annual",
			.val = 'a',
			.has_arg = required_argument,
			.flag = NULL,
		},
		{
			.name = "max",
			.val = 'm',
			.has_arg = required_argument,
			.flag = NULL,
		},
		{ 0 }
	};
	char *err;
	int arg;
	int argc_expected = 2;

	vacation_days_per_year = VACATION_DAYS_PER_YEAR;
	max_vacation_accumulation_allowed = MAX_VACATION_ACCUMULATION_ALLOWED;

	while ((arg = getopt_long(argc, argv, optstring, longopts, NULL)) !=
			-1) {
		switch (arg) {
		case 'h':
			return ACTION_HELP;
		case 'a':
			vacation_days_per_year = strtol(optarg, &err, 10);
			if (*err) {
				printf("Invalid agrument for --annual/-a: %s\n",
					argv[optind]);
				return ACTION_INPUT_ERROR;
			}
			argc_expected += 2;
			break;
		case 'm':
			max_vacation_accumulation_allowed = strtol(optarg, &err, 10);
			if (*err) {
				printf("Invalid agrument for --max/-m: %s\n",
					argv[optind]);
				return ACTION_INPUT_ERROR;
			}
			argc_expected += 2;
			break;
		}
	}

	if (argc != argc_expected)
		return ACTION_INPUT_ERROR;

	payslip_vacation_hours = strtod(argv[argc_expected - 1], &err);
	if (*err) {
		printf("Invalid agrument: %s\n", argv[argc_expected - 1]);
		return ACTION_INPUT_ERROR;
	}

	printf("annual = %d, max = %d, hours = %.2lf\n",vacation_days_per_year,
		max_vacation_accumulation_allowed, payslip_vacation_hours);

	params->vacation_days_per_year = vacation_days_per_year;
	params->max_vacation_accumulation_allowed =
		max_vacation_accumulation_allowed;
	params->payslip_vacation_hours = payslip_vacation_hours;

	return ACTION_CALCULATE;
}

int main(int argc, char **argv)
{
	struct vacation_params params;
	struct vacation_stats stats;
	int ret = 0;

	switch (parse_args(argc, argv, &params)) {
	case ACTION_CALCULATE:
		if (vacation_calculate(&params, &stats)) {
			printf("Error in vacation calculation\n");
			ret = -1;
			goto show_usage;
		}
		break;
	case ACTION_HELP:
		goto show_usage;
	case ACTION_INPUT_ERROR:
		ret = -1;
		goto show_usage;
	default:
		break;
	}

	print_vacation_stats(&stats);
	return 0;

show_usage:
	usage(argv[0]);
	return ret;
}

