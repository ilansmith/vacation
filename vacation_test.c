#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "vacation.h"

#undef C_RED
#define C_RED "\033[01;31m"
#undef C_GREEN
#define C_GREEN "\033[01;32m"
#undef C_NORMAL
#define C_NORMAL "\033[00;00;00m"

#define ERR_FMT_HDR "%-33s%-13s%s\n"
#define ERR_FMT "%-33s%-13d%d\n"

#define WORK_DAY_TO_HOURS(_days_) ((_days_) * WORK_DAY_IN_HOURS)

typedef struct {
	char *description;
	uint16_t current_month;
	uint16_t day_of_month;
	struct vacation_params params;
	struct vacation_stats expected;
} test_t;

static int vio_colour(int (*vfunc)(const char *format, va_list ap),
		char *colour, char *fmt, va_list va)
{
	int ret;

	if (!colour)
		colour = C_NORMAL;

	ret = printf("%s", colour);
	ret += vfunc(fmt, va);
	ret += printf("%s", C_NORMAL);
	fflush(stdout);

	return ret;
}

static int p_colour(char *colour, char *fmt, ...)
{
	int ret;
	va_list va;

	va_start(va, fmt);
	ret = vio_colour(vprintf, colour, fmt, va);
	va_end(va);

	return ret;
}

static test_t tests[] = {
	{
		.description = "Two days excess - October 15",
		.current_month = 10,
		.day_of_month = 15,
		.params = {
			.vacation_days_per_year = 24,
			.max_vacation_accumulation_allowed = 36,
			.payslip_vacation_hours = WORK_DAY_TO_HOURS(32),
		},
		.expected = {
			.days_due_total = 32,
			.days_due_this_year = 18,
			.days_remaining_this_year = 6,
			.days_due_accumulated = 14,
			.total_due_at_end_of_year = 38,
			.expected_excess_at_end_of_year = 2,
		},
	},
	{
		.description = "Two days excess - October 16",
		.current_month = 10,
		.day_of_month = 16,
		.params = {
			.vacation_days_per_year = 24,
			.max_vacation_accumulation_allowed = 36,
			.payslip_vacation_hours = WORK_DAY_TO_HOURS(32),
		},
		.expected = {
			.days_due_total = 33,
			.days_due_this_year = 19,
			.days_remaining_this_year = 5,
			.days_due_accumulated = 14,
			.total_due_at_end_of_year = 38,
			.expected_excess_at_end_of_year = 2,
		},
	},
	{
		.description = "Two days removed - January 15",
		.current_month = 1,
		.day_of_month = 15,
		.params = {
			.vacation_days_per_year = 24,
			.max_vacation_accumulation_allowed = 36,
			.payslip_vacation_hours = WORK_DAY_TO_HOURS(38),
		},
		.expected = {
			.days_due_total = 36,
			.days_due_this_year = 0,
			.days_remaining_this_year = 24,
			.days_due_accumulated = 36,
			.total_due_at_end_of_year = 60,
			.expected_excess_at_end_of_year = 24,
		},
	},
	{ NULL }
};

int main(int argc, char *argv[])
{
	test_t *t;

	for (t = tests; t->description; t++) {
		struct vacation_stats stats;

		test_month = t->current_month;
		test_day = t->day_of_month;

		if (vacation_calculate(&t->params, &stats)) {
			printf("Calculation error, continuing...\n");
			continue;
		}

		printf("%s\n", t->description);
		if (memcmp(&stats, &t->expected,
					sizeof(struct vacation_stats))) {
			printf(ERR_FMT_HDR, "", "Calculated", "Expected");
			printf(ERR_FMT_HDR, "", "----------", "--------");

			printf(ERR_FMT, "days due total",
				stats.days_due_total,
				t->expected.days_due_total);
			printf(ERR_FMT, "days due this year",
				stats.days_due_this_year,
				t->expected.days_due_this_year);
			printf(ERR_FMT, "days remaining this year",
				stats.days_remaining_this_year,
				t->expected.days_remaining_this_year);
			printf(ERR_FMT, "days due accumulated",
				stats.days_due_accumulated,
				t->expected.days_due_accumulated);
			printf(ERR_FMT, "total due at end of year",
				stats.total_due_at_end_of_year,
				t->expected.total_due_at_end_of_year);
			printf(ERR_FMT, "expected excess at end of year",
				stats.expected_excess_at_end_of_year,
				t->expected.expected_excess_at_end_of_year);
			p_colour(C_RED, "Failed");
		} else {
			p_colour(C_GREEN, "OK");
		}
		printf("\n");
	}

	return 0;
}

