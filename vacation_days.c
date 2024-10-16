#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <getopt.h>
#include <unistd.h>

#define VACATION_DAYS_PER_YEAR 24 /* my number of annual vacation days */
#define MAX_VACATION_ACCUMULATION_ALLOWED 36 /* max total number of
						accumulated vacation days
						permitted by the company */
#define MONTHS_IN_YEAR 12
#define WORK_DAY_IN_HOURS 8.416666666

#define MAX(a, b) (a < b ? b : a)

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

/* check if a year is a leap year */
static int is_leap_year(int year)
{
	return (!(year % 4) && (year % 100)) || !(year % 400) ? 1 : 0;
}

/* return the number of days in a month for a given year */
static int days_in_month(int month, int year)
{
	int days_in_months[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30,
		31 };

	if (month == 2 && is_leap_year(year)) {
		return 29; /* February in a leap year */
	}

	return days_in_months[month - 1];
}

static int get_year_month_day(int *year, int *month, int *day)
{
	int internal_day, *pday;
	int internal_month, *pmonth;
	int internal_year, *pyear;

	/* Get the current date */
	time_t now = time(NULL);
	struct tm *current = localtime(&now);

	if (!current)
		return -1;

	pday = day ? day : &internal_day;
	pmonth = month ? month : &internal_month;
	pyear = year ? year : &internal_year;

	*pday = current->tm_mday;
	*pmonth = current->tm_mon + 1; /* tm_mon is 0-based
					  (Jan = 0, Dec = 11) */
	*pyear = current->tm_year + 1900; /* tm_year is years since 1900 */

	return 0;
}

static int vacation_days_this_month(int vacation_days_per_year)
{
	int day;
	int month;
	int year;
	int days_this_month;
	int vacation_days_per_month = vacation_days_per_year / MONTHS_IN_YEAR;

	if (get_year_month_day(&year, &month, &day))
		return -1;

	days_this_month = days_in_month(month, year);

	return (int)(((double)day / days_this_month) * vacation_days_per_month);
}

static int vacation_days_since_start_of_year(int vacation_days_per_year)
{
	int day;
	int month;
	int year;
	int vacation_days_per_month = vacation_days_per_year / MONTHS_IN_YEAR;

	if (get_year_month_day(&year, &month, &day))
		return -1;

	return (int)((month - 1 +
		((double)day / days_in_month(month, year))) *
		vacation_days_per_month);
}

static int calculate_vacation(int vacation_days_per_year,
		int max_vacation_accumulation_allowed,
		double payslip_vacation_hours)
{
	int days_due_total;
	int days_due_this_year;
	int days_remaining_this_year;
	int days_due_accumulated;

	days_due_total = (int)(payslip_vacation_hours / WORK_DAY_IN_HOURS) +
		vacation_days_this_month(vacation_days_per_year);

	days_due_this_year =
		vacation_days_since_start_of_year(vacation_days_per_year);

	if (days_due_this_year == -1)
		return -1;

	days_remaining_this_year = vacation_days_per_year -
		days_due_this_year;
	days_due_accumulated = MAX(0, days_due_total - days_due_this_year);

	printf("Current vacation days due: %d", days_due_total);
	if (days_due_accumulated) {
		printf(" (accumulated %d, current year %d)",
			days_due_accumulated, days_due_this_year);
	}
	printf("\n");
	printf("Expected additional days to end of year: %d\n",
		days_remaining_this_year);
	printf("Expected total due at end of year: %d\n",
		days_due_total + days_remaining_this_year);

	printf("Expected excess days at end of year: %d\n",
		MAX(0, days_due_total + days_remaining_this_year -
			max_vacation_accumulation_allowed));

	return 0;
}

int main(int argc, char **argv)
{
	int vacation_days_per_year;
	int max_vacation_accumulation_allowed;
	double payslip_vacation_hours;
	char *optstring = "ha:m:";
	struct option longopts[] = {
		{
			.name = "anual",
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
	int ret = -1;

	vacation_days_per_year = VACATION_DAYS_PER_YEAR;
	max_vacation_accumulation_allowed = MAX_VACATION_ACCUMULATION_ALLOWED;

	while ((arg = getopt_long(argc, argv, optstring, longopts, NULL)) !=
			-1) {
		switch (arg) {
		case 'h':
			ret = 0;
			goto show_usage;
			break;
		case 'a':
			vacation_days_per_year = strtol(optarg, &err, 10);
			if (*err) {
				printf("Invalid agrument for --anual/-a: %s\n",
					argv[optind]);
				usage(argv[0]);
				goto show_usage;
			}
			argc_expected += 2;
			break;
		case 'm':
			max_vacation_accumulation_allowed = strtol(optarg,
				&err, 10);
			if (*err) {
				printf("Invalid agrument for --max/-m: %s\n",
					argv[optind]);
				usage(argv[0]);
				goto show_usage;
			}
			argc_expected += 2;
			break;
		}
	}

	if (argc != argc_expected || max_vacation_accumulation_allowed <
			vacation_days_per_year) {
		goto show_usage;
	}

	payslip_vacation_hours = strtod(argv[argc_expected - 1], &err);
	if (*err) {
		printf("Invalid agrument: %s\n", argv[argc_expected - 1]);
		goto show_usage;
	}

	printf("aunual = %d, max = %d, hours = %.2lf\n",vacation_days_per_year,
		max_vacation_accumulation_allowed, payslip_vacation_hours);

	if (calculate_vacation(vacation_days_per_year,
			max_vacation_accumulation_allowed,
			payslip_vacation_hours)) {
		printf("Error in vacation calculation\n");
		return -1;
	}

	return 0;

show_usage:
	usage(argv[0]);
	return ret;
}

