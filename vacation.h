#ifndef _VACATION_H_
#define _VACATION_H_

#include <stdint.h>

#define WORK_DAY_IN_HOURS 8.41666666666666

#if defined(TESTS)
extern uint16_t test_day;
extern uint16_t test_month;
#endif

struct vacation_params {
	uint16_t vacation_days_per_year;
	uint16_t max_vacation_accumulation_allowed;
	double payslip_vacation_hours;
};

struct vacation_stats {
	uint16_t days_due_total;
	uint16_t days_due_this_year;
	uint16_t days_remaining_this_year;
	uint16_t days_due_accumulated;
	uint16_t total_due_at_end_of_year;
	uint16_t expected_excess_at_end_of_year;
};

int vacation_calculate(struct vacation_params *params,
	struct vacation_stats *stats);

#endif

