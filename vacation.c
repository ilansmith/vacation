#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

#include "vacation.h"

#define MONTHS_IN_YEAR 12
#define WORK_DAY_IN_HOURS 8.416666666

#define MAX(a, b) (a < b ? b : a)

/* check if a year is a leap year */
static int is_leap_year(uint16_t year)
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

static int get_year_month_day(uint16_t *year, uint16_t *month, uint16_t *day)
{
	uint16_t internal_day, *pday;
	uint16_t internal_month, *pmonth;
	uint16_t internal_year, *pyear;

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

static uint16_t vacation_days_this_month(uint16_t vacation_days_per_year)
{
	uint16_t day;
	uint16_t month;
	uint16_t year;
	uint16_t days_this_month;
	uint16_t vacation_days_per_month = vacation_days_per_year /
		MONTHS_IN_YEAR;

	if (get_year_month_day(&year, &month, &day))
		return -1;

	days_this_month = days_in_month(month, year);

	return (uint16_t)(((double)day / days_this_month) *
		vacation_days_per_month);
}

static uint16_t vacation_days_since_start_of_year(int vacation_days_per_year)
{
	uint16_t day;
	uint16_t month;
	uint16_t year;
	uint16_t vacation_days_per_month = vacation_days_per_year /
		MONTHS_IN_YEAR;

	if (get_year_month_day(&year, &month, &day))
		return -1;

	return (uint16_t)((month - 1 +
		((double)day / days_in_month(month, year))) *
		vacation_days_per_month);
}

int vacation_calculate(struct vacation_params *params,
		struct vacation_stats *stats)
{
	stats->days_due_total = (int)(params->payslip_vacation_hours /
		WORK_DAY_IN_HOURS) +
		vacation_days_this_month(params->vacation_days_per_year);

	stats->days_due_this_year = vacation_days_since_start_of_year(
		params->vacation_days_per_year);

	if (stats->days_due_this_year == -1)
		return -1;

	stats->days_remaining_this_year = params->vacation_days_per_year -
		stats->days_due_this_year;
	stats->days_due_accumulated = MAX(0, stats->days_due_total -
		stats->days_due_this_year);
	stats->total_due_at_end_of_year = MAX(0, stats->days_due_total +
		stats->days_remaining_this_year);
	stats->expected_excess_at_end_of_year = MAX(0,
		stats->total_due_at_end_of_year -
		params->max_vacation_accumulation_allowed);

	return 0;
}

