#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <ctype.h>

#include "vacation.h"

/* Month names for output */
static const char *month_names[] = {
	"January", "February", "March", "April", "May", "June",
	"July", "August", "September", "October", "November", "December"
};

/* Valid annual vacation days and corresponding hours */
const int VALID_ANNUAL_DAYS[NUM_VALID_ANNUAL_OPTIONS] = {
	15, 16, 17, 18, 19, 20, 21, 22, 23, 24
};

const int VALID_ANNUAL_HOURS[NUM_VALID_ANNUAL_OPTIONS] = {
	126, 135, 143, 152, 159, 168, 177, 185, 194, 202
};

/*
 * Conversion functions
 */

double hours_to_days(double hours)
{
	return hours / WORK_DAY_HOURS;
}

double days_to_hours(double days)
{
	return days * WORK_DAY_HOURS;
}

/*
 * Parsing helper functions
 */

int parse_integer(const char *str, int *value)
{
	char *endptr;
	long result;

	/* Skip leading whitespace */
	while (isspace((unsigned char)*str))
		str++;

	if (*str == '\0')
		return 1;

	result = strtol(str, &endptr, 10);

	/* Check for trailing non-whitespace characters */
	while (isspace((unsigned char)*endptr))
		endptr++;

	if (*endptr != '\0')
		return 1;

	/* Check for overflow */
	if (result < -2147483647 || result > 2147483647)
		return 1;

	*value = (int)result;
	return 0;
}

int parse_number(const char *str, double *value)
{
	char *endptr;

	/* Skip leading whitespace */
	while (isspace((unsigned char)*str))
		str++;

	if (*str == '\0')
		return 1;

	*value = strtod(str, &endptr);

	/* Check for trailing non-whitespace characters */
	while (isspace((unsigned char)*endptr))
		endptr++;

	if (*endptr != '\0')
		return 1;

	return 0;
}

/*
 * Time functions
 */

int get_current_month(void)
{
	time_t now = time(NULL);
	struct tm *local_time = localtime(&now);

	return local_time->tm_mon + 1;	/* tm_mon is 0-11, we want 1-12 */
}

int get_current_year(void)
{
	time_t now = time(NULL);
	struct tm *local_time = localtime(&now);

	return local_time->tm_year + 1900;	/* tm_year is years since 1900 */
}

int is_leap_year(int year)
{
	return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int days_in_month(int month, int year)
{
	static const int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	if (month < 1 || month > 12)
		return 0;

	if (month == 2 && is_leap_year(year))
		return 29;

	return days[month - 1];
}

int day_of_week(int day, int month, int year)
{
	int y, m, q, k, j, h;

	/* Zeller's congruence for Gregorian calendar */
	/* Adjust for January and February */
	if (month < 3) {
		month += 12;
		year--;
	}

	q = day;
	m = month;
	y = year;
	k = y % 100;
	j = y / 100;

	h = (q + (13 * (m + 1)) / 5 + k + k / 4 + j / 4 - 2 * j) % 7;

	/* Convert from Zeller (0=Sat, 1=Sun, ..., 6=Fri) to standard (0=Sun, ..., 6=Sat) */
	return ((h + 6) % 7);
}

int calculate_working_days(int month, int year, int week_start)
{
	int total_days;
	int working_days = 0;
	int day;
	int dow;
	int is_working;

	total_days = days_in_month(month, year);
	if (total_days == 0)
		return 0;

	for (day = 1; day <= total_days; day++) {
		dow = day_of_week(day, month, year);

		if (week_start == WEEK_START_SUNDAY) {
			/* Working days: Sun(0) - Thu(4) */
			is_working = (dow >= 0 && dow <= 4);
		} else {
			/* Working days: Mon(1) - Fri(5) */
			is_working = (dow >= 1 && dow <= 5);
		}

		if (is_working)
			working_days++;
	}

	return working_days;
}

int calculate_remaining_working_days(int current_month, int year, int week_start)
{
	int total_working_days = 0;
	int month;

	for (month = current_month; month <= 12; month++)
		total_working_days += calculate_working_days(month, year, week_start);

	return total_working_days;
}

int calculate_working_days_from_prev_month(int current_month, int year, int week_start)
{
	int total_working_days = 0;
	int start_month;
	int month;

	/* Previous month (handle January wrapping to December of previous year) */
	if (current_month == 1) {
		/* Add December of previous year */
		total_working_days += calculate_working_days(12, year - 1, week_start);
		start_month = 1;
	} else {
		start_month = current_month - 1;
	}

	/* Add working days from start_month to end of year */
	for (month = start_month; month <= 12; month++)
		total_working_days += calculate_working_days(month, year, week_start);

	return total_working_days;
}

/*
 * Calculation functions
 */

int calculate_remaining_months(int month)
{
	if (month < 1 || month > 12)
		return 0;
	return MONTHS_PER_YEAR - month + 1;
}

double calculate_monthly_hours(int annual_days)
{
	return days_to_hours((double)annual_days) / MONTHS_PER_YEAR;
}

double calculate_additional_hours(int remaining_months, double monthly_hours)
{
	if (remaining_months < 0)
		return 0.0;
	return remaining_months * monthly_hours;
}

double calculate_total_hours(double current_hours, double additional_hours)
{
	return current_hours + additional_hours;
}

double calculate_excess_days(double total_days, int max_accum_days)
{
	double excess = total_days - (double)max_accum_days;

	return (excess > 0) ? excess : 0.0;
}

void calculate_vacation(const struct vacation_input *input,
			struct vacation_result *result)
{
	double vacation_hours;

	result->remaining_months = calculate_remaining_months(input->current_month);
	result->monthly_hours = calculate_monthly_hours(input->annual_days);
	result->additional_hours = calculate_additional_hours(
		result->remaining_months, result->monthly_hours);
	result->additional_days = hours_to_days(result->additional_hours);
	result->total_hours = calculate_total_hours(input->current_hours,
						    result->additional_hours);

	/* Deduct extra vacation days consumption */
	vacation_hours = days_to_hours(input->vacation_extra);
	result->total_hours -= vacation_hours;

	result->total_days = hours_to_days(result->total_hours);
	result->excess_days = calculate_excess_days(result->total_days,
						    input->max_accum_days);

	/* Calculate remaining working days this year */
	result->working_days_remaining = calculate_remaining_working_days(
		input->current_month, input->current_year, input->week_start);
}

/*
 * Argument handling functions
 */

void init_vacation_args(struct vacation_args *args)
{
	args->annual_days = 0;	/* Must be set by user */
	args->current_hours = 0.0;
	args->annual_days_set = 0;
	args->annual_hours_set = 0;
	args->current_hours_set = 0;
	args->vacation_extra = 0.0;
	args->vacation_extra_set = 0;
	args->week_start = WEEK_START_SUNDAY;
	args->special_accum = 0;
}

int validate_arguments(const struct vacation_args *args)
{
	if (!args->annual_days_set && !args->annual_hours_set) {
		fprintf(stderr,
			"Error: Annual vacation days (-d) or hours (-A) must be specified.\n");
		return 1;
	}

	if (args->annual_days_set && args->annual_hours_set) {
		fprintf(stderr,
			"Error: --annual-days (-d) and --annual-hours (-A) are mutually exclusive.\n");
		fprintf(stderr, "Please specify only one of these options.\n");
		return 1;
	}
	return 0;
}

void print_valid_annual_options(void)
{
	int i;

	fprintf(stderr, "\nValid annual vacation options:\n");
	for (i = 0; i < NUM_VALID_ANNUAL_OPTIONS; i++) {
		fprintf(stderr, "  %d days or %d hours\n",
			VALID_ANNUAL_DAYS[i], VALID_ANNUAL_HOURS[i]);
	}
	fprintf(stderr, "\nPlease consult your pay slip for your annual vacation allowance.\n");
}

int is_valid_annual_hours(int hours, int *days_out)
{
	int i;

	for (i = 0; i < NUM_VALID_ANNUAL_OPTIONS; i++) {
		if (VALID_ANNUAL_HOURS[i] == hours) {
			if (days_out)
				*days_out = VALID_ANNUAL_DAYS[i];
			return 1;
		}
	}
	return 0;
}

int get_annual_hours_for_days(int days)
{
	int i;

	for (i = 0; i < NUM_VALID_ANNUAL_OPTIONS; i++) {
		if (VALID_ANNUAL_DAYS[i] == days)
			return VALID_ANNUAL_HOURS[i];
	}
	/* Fallback: calculate from WORK_DAY_HOURS */
	return (int)(days * WORK_DAY_HOURS);
}

int validate_annual_days(int annual_days)
{
	int i;
	int valid = 0;

	for (i = 0; i < NUM_VALID_ANNUAL_OPTIONS; i++) {
		if (VALID_ANNUAL_DAYS[i] == annual_days) {
			valid = 1;
			break;
		}
	}

	if (!valid) {
		fprintf(stderr,
			"Error: Invalid annual vacation days (%d).\n",
			annual_days);
		print_valid_annual_options();
		return 1;
	}

	return 0;
}

int calculate_max_accum(int annual_days, int special_accum)
{
	/* Normal case: always 36 */
	if (!special_accum)
		return MIN_MAX_ACCUM_DAYS;

	/* Special accum: 12-17 days -> 36, 18-24 days -> annual_days * 2 */
	if (annual_days < 18)
		return MIN_MAX_ACCUM_DAYS;

	return annual_days * 2;
}

int validate_vacation_days(double vacation_days, int max_working_days)
{
	if (vacation_days < 0) {
		fprintf(stderr,
			"Error: Extra vacation days consumption cannot be negative.\n");
		return 1;
	}

	if (vacation_days > max_working_days) {
		fprintf(stderr,
			"Error: Extra vacation days consumption (%.1f) cannot exceed working days from start of previous month (%d).\n",
			vacation_days, max_working_days);
		return 1;
	}

	return 0;
}

void print_usage(const char *program_name)
{
	int i;

	printf("Usage: %s [OPTIONS]\n", program_name);
	printf("\n");
	printf("Calculate expected vacation days credit at year end.\n");
	printf("\n");
	printf("Options:\n");
	printf("  -d, --annual-days <days>    Annual vacation days (integer)\n");
	printf("                              Mutually exclusive with -A/--annual-hours\n");
	printf("  -A, --annual-hours <hours>  Annual vacation hours (integer)\n");
	printf("                              Mutually exclusive with -d/--annual-days\n");
	printf("  -s, --special-accum         Use special max accumulated days calculation\n");
	printf("                              (15-17 days: 36, 18-24 days: annual-days * 2)\n");
	printf("                              Default (without -s): always 36\n");
	printf("  -c, --current-hours <hours> Current accumulated vacation hours\n");
	printf("  -v, --vacation-extra <days> Extra vacation days consumption\n");
	printf("  -M, --monday-start          Use Monday as first working day of week\n");
	printf("                              (default: Sunday)\n");
	printf("  -h, --help                  Display this help message\n");
	printf("\n");
	printf("Valid annual vacation options:\n");
	for (i = 0; i < NUM_VALID_ANNUAL_OPTIONS; i++) {
		printf("  %d days or %d hours\n",
		       VALID_ANNUAL_DAYS[i], VALID_ANNUAL_HOURS[i]);
	}
	printf("\n");
	printf("Either -d/--annual-days or -A/--annual-hours must be specified.\n");
	printf("If --current-hours is not provided, the program will prompt for input.\n");
}

int parse_arguments(int argc, char *argv[], struct vacation_args *args)
{
	int i;
	int int_value;
	double double_value;

	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-h") == 0 ||
		    strcmp(argv[i], "--help") == 0) {
			print_usage(argv[0]);
			exit(0);
		} else if (strcmp(argv[i], "-d") == 0 ||
			   strcmp(argv[i], "--annual-days") == 0) {
			if (i + 1 >= argc) {
				fprintf(stderr, "Error: %s requires a value.\n",
					argv[i]);
				return 1;
			}
			i++;
			if (parse_integer(argv[i], &int_value) != 0) {
				fprintf(stderr,
					"Error: Annual days must be an integer: %s\n",
					argv[i]);
				return 1;
			}
			args->annual_days = int_value;
			args->annual_days_set = 1;
		} else if (strcmp(argv[i], "-A") == 0 ||
			   strcmp(argv[i], "--annual-hours") == 0) {
			if (i + 1 >= argc) {
				fprintf(stderr, "Error: %s requires a value.\n",
					argv[i]);
				return 1;
			}
			i++;
			if (parse_integer(argv[i], &int_value) != 0) {
				fprintf(stderr,
					"Error: Annual hours must be an integer: %s\n",
					argv[i]);
				return 1;
			}
			/* Validate against specific valid hour values */
			if (!is_valid_annual_hours(int_value, &args->annual_days)) {
				fprintf(stderr,
					"Error: Invalid annual vacation hours (%d).\n",
					int_value);
				print_valid_annual_options();
				return 1;
			}
			args->annual_hours_set = 1;
		} else if (strcmp(argv[i], "-s") == 0 ||
			   strcmp(argv[i], "--special-accum") == 0) {
			args->special_accum = 1;
		} else if (strcmp(argv[i], "-c") == 0 ||
			   strcmp(argv[i], "--current-hours") == 0) {
			if (i + 1 >= argc) {
				fprintf(stderr, "Error: %s requires a value.\n",
					argv[i]);
				return 1;
			}
			i++;
			if (parse_number(argv[i], &double_value) != 0) {
				fprintf(stderr,
					"Error: Invalid number format for current hours: %s\n",
					argv[i]);
				return 1;
			}
			args->current_hours = double_value;
			args->current_hours_set = 1;
		} else if (strcmp(argv[i], "-v") == 0 ||
			   strcmp(argv[i], "--vacation-extra") == 0) {
			if (i + 1 >= argc) {
				fprintf(stderr, "Error: %s requires a value.\n",
					argv[i]);
				return 1;
			}
			i++;
			if (parse_number(argv[i], &double_value) != 0) {
				fprintf(stderr,
					"Error: Invalid number format for extra vacation days: %s\n",
					argv[i]);
				return 1;
			}
			args->vacation_extra = double_value;
			args->vacation_extra_set = 1;
		} else if (strcmp(argv[i], "-M") == 0 ||
			   strcmp(argv[i], "--monday-start") == 0) {
			args->week_start = WEEK_START_MONDAY;
		} else {
			fprintf(stderr, "Error: Unknown option: %s\n", argv[i]);
			fprintf(stderr, "Use -h or --help for usage information.\n");
			return 1;
		}
	}
	return 0;
}

int prompt_current_hours(double *hours)
{
	char input_buffer[256];
	double value;

	printf("Enter current accumulated vacation hours: ");
	fflush(stdout);
	if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
		fprintf(stderr, "Error: Failed to read input.\n");
		return 1;
	}

	if (parse_number(input_buffer, &value) != 0) {
		fprintf(stderr,
			"Error: Invalid number format for current hours.\n");
		return 1;
	}

	*hours = value;
	return 0;
}

/*
 * Output functions
 */

static void print_double_value(double value)
{
	char buf[32];
	int len;

	/* Format with 2 decimal places */
	snprintf(buf, sizeof(buf), "%.2f", value);
	len = strlen(buf);

	/* Remove trailing zeros after decimal point */
	while (len > 0 && buf[len - 1] == '0')
		buf[--len] = '\0';

	/* Remove trailing decimal point if no decimals remain */
	if (len > 0 && buf[len - 1] == '.')
		buf[--len] = '\0';

	printf("%s", buf);
}

void print_results(const struct vacation_input *input,
		   const struct vacation_result *result)
{
	int annual_hours = get_annual_hours_for_days(input->annual_days);
	double max_accum_hours = days_to_hours(input->max_accum_days);
	double excess_hours = days_to_hours(result->excess_days);

	printf("=== Vacation Days Calculator ===\n\n");
	printf("Annual vacation: %d hours (%d days)\n",
	       annual_hours, input->annual_days);
	printf("Maximum accumulation: ");
	print_double_value(max_accum_hours);
	printf(" hours (%d days)\n", input->max_accum_days);
	printf("Current accumulated hours: ");
	print_double_value(input->current_hours);
	printf("\n");
	printf("\n");
	printf("Current month: %s (%d)\n",
	       month_names[input->current_month - 1], input->current_month);
	printf("Remaining working days in %d: %d\n",
	       input->current_year, result->working_days_remaining);
	if (input->vacation_extra > 0) {
		printf("Extra vacation days consumption: ");
		print_double_value(input->vacation_extra);
		printf("\n");
	}
	printf("\n");
	printf("Additional vacation expected by the end of %d: ", input->current_year);
	print_double_value(result->additional_days);
	printf(" days (");
	print_double_value(result->additional_hours);
	printf(" hours)\n");
	printf("Total accumulated vacation expected by the end of %d: ", input->current_year);
	print_double_value(result->total_hours);
	printf(" hours (");
	print_double_value(result->total_days);
	printf(" days)\n");
	printf("Vacation to be deducted at the end of %d (exceeding max): ", input->current_year);
	print_double_value(excess_hours);
	printf(" hours (");
	print_double_value(result->excess_days);
	printf(" days)\n");
}

/*
 * Main function
 */

#ifndef TEST_BUILD
int main(int argc, char *argv[])
{
	struct vacation_args args;
	struct vacation_input input;
	struct vacation_result result;
	int remaining_working_days;
	int max_accum_days;

	/* Initialize and parse arguments */
	init_vacation_args(&args);
	if (parse_arguments(argc, argv, &args) != 0)
		return 1;

	/* Validate arguments */
	if (validate_arguments(&args) != 0)
		return 1;

	/* Validate annual days range */
	if (validate_annual_days(args.annual_days) != 0)
		return 1;

	/* Calculate max accumulated days based on special accum flag */
	max_accum_days = calculate_max_accum(args.annual_days, args.special_accum);

	/* Get current hours interactively if not provided */
	if (!args.current_hours_set) {
		if (prompt_current_hours(&args.current_hours) != 0)
			return 1;
	}

	/* Prepare calculation input */
	input.annual_days = args.annual_days;
	input.max_accum_days = max_accum_days;
	input.current_hours = args.current_hours;
	input.current_month = get_current_month();
	input.current_year = get_current_year();
	input.vacation_extra = args.vacation_extra;
	input.week_start = args.week_start;

	/* Validate vacation days against working days from start of previous month */
	if (args.vacation_extra_set) {
		remaining_working_days = calculate_working_days_from_prev_month(
			input.current_month, input.current_year, input.week_start);
		if (validate_vacation_days(args.vacation_extra,
					   remaining_working_days) != 0)
			return 1;
	}

	/* Perform calculations */
	calculate_vacation(&input, &result);

	/* Print results */
	print_results(&input, &result);

	return 0;
}
#endif /* TEST_BUILD */
