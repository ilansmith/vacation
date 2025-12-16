#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "vacation.h"

/* Test framework macros */
static int tests_run = 0;
static int tests_passed = 0;
static int tests_failed = 0;

#define EPSILON 0.0001

#define TEST_START(name) \
	do { \
		printf("  Running: %s... ", name); \
		tests_run++; \
	} while (0)

#define TEST_PASS() \
	do { \
		printf("PASS\n"); \
		tests_passed++; \
	} while (0)

#define TEST_FAIL(msg) \
	do { \
		printf("FAIL: %s\n", msg); \
		tests_failed++; \
	} while (0)

#define ASSERT_INT_EQ(expected, actual) \
	do { \
		if ((expected) != (actual)) { \
			char buf[256]; \
			snprintf(buf, sizeof(buf), \
				 "Expected %d, got %d", (expected), (actual)); \
			TEST_FAIL(buf); \
			return; \
		} \
	} while (0)

#define ASSERT_DOUBLE_EQ(expected, actual) \
	do { \
		if (fabs((expected) - (actual)) > EPSILON) { \
			char buf[256]; \
			snprintf(buf, sizeof(buf), \
				 "Expected %f, got %f", (expected), (actual)); \
			TEST_FAIL(buf); \
			return; \
		} \
	} while (0)

#define ASSERT_TRUE(condition) \
	do { \
		if (!(condition)) { \
			TEST_FAIL("Condition is false"); \
			return; \
		} \
	} while (0)

#define ASSERT_FALSE(condition) \
	do { \
		if (condition) { \
			TEST_FAIL("Condition is true"); \
			return; \
		} \
	} while (0)

/*
 * Test: parse_integer
 */

static void test_parse_integer_positive(void)
{
	int value;

	TEST_START("parse_integer with positive number");
	ASSERT_INT_EQ(0, parse_integer("42", &value));
	ASSERT_INT_EQ(42, value);
	TEST_PASS();
}

static void test_parse_integer_negative(void)
{
	int value;

	TEST_START("parse_integer with negative number");
	ASSERT_INT_EQ(0, parse_integer("-15", &value));
	ASSERT_INT_EQ(-15, value);
	TEST_PASS();
}

static void test_parse_integer_zero(void)
{
	int value;

	TEST_START("parse_integer with zero");
	ASSERT_INT_EQ(0, parse_integer("0", &value));
	ASSERT_INT_EQ(0, value);
	TEST_PASS();
}

static void test_parse_integer_with_whitespace(void)
{
	int value;

	TEST_START("parse_integer with leading/trailing whitespace");
	ASSERT_INT_EQ(0, parse_integer("  123  ", &value));
	ASSERT_INT_EQ(123, value);
	TEST_PASS();
}

static void test_parse_integer_float_rejected(void)
{
	int value;

	TEST_START("parse_integer rejects floating point");
	ASSERT_INT_EQ(1, parse_integer("3.14", &value));
	TEST_PASS();
}

static void test_parse_integer_text_rejected(void)
{
	int value;

	TEST_START("parse_integer rejects text");
	ASSERT_INT_EQ(1, parse_integer("abc", &value));
	TEST_PASS();
}

static void test_parse_integer_mixed_rejected(void)
{
	int value;

	TEST_START("parse_integer rejects mixed input");
	ASSERT_INT_EQ(1, parse_integer("42abc", &value));
	TEST_PASS();
}

static void test_parse_integer_empty_rejected(void)
{
	int value;

	TEST_START("parse_integer rejects empty string");
	ASSERT_INT_EQ(1, parse_integer("", &value));
	TEST_PASS();
}

static void test_parse_integer_whitespace_only_rejected(void)
{
	int value;

	TEST_START("parse_integer rejects whitespace only");
	ASSERT_INT_EQ(1, parse_integer("   ", &value));
	TEST_PASS();
}

/*
 * Test: parse_number
 */

static void test_parse_number_integer(void)
{
	double value;

	TEST_START("parse_number with integer");
	ASSERT_INT_EQ(0, parse_number("42", &value));
	ASSERT_DOUBLE_EQ(42.0, value);
	TEST_PASS();
}

static void test_parse_number_float(void)
{
	double value;

	TEST_START("parse_number with float");
	ASSERT_INT_EQ(0, parse_number("3.14", &value));
	ASSERT_DOUBLE_EQ(3.14, value);
	TEST_PASS();
}

static void test_parse_number_negative_float(void)
{
	double value;

	TEST_START("parse_number with negative float");
	ASSERT_INT_EQ(0, parse_number("-2.5", &value));
	ASSERT_DOUBLE_EQ(-2.5, value);
	TEST_PASS();
}

static void test_parse_number_with_whitespace(void)
{
	double value;

	TEST_START("parse_number with whitespace");
	ASSERT_INT_EQ(0, parse_number("  150.5  ", &value));
	ASSERT_DOUBLE_EQ(150.5, value);
	TEST_PASS();
}

static void test_parse_number_text_rejected(void)
{
	double value;

	TEST_START("parse_number rejects text");
	ASSERT_INT_EQ(1, parse_number("abc", &value));
	TEST_PASS();
}

static void test_parse_number_empty_rejected(void)
{
	double value;

	TEST_START("parse_number rejects empty string");
	ASSERT_INT_EQ(1, parse_number("", &value));
	TEST_PASS();
}

/*
 * Test: is_half_day_resolution
 */

static void test_half_day_zero(void)
{
	TEST_START("is_half_day_resolution with zero");
	ASSERT_TRUE(is_half_day_resolution(0.0));
	TEST_PASS();
}

static void test_half_day_integer(void)
{
	TEST_START("is_half_day_resolution with integer");
	ASSERT_TRUE(is_half_day_resolution(5.0));
	TEST_PASS();
}

static void test_half_day_half(void)
{
	TEST_START("is_half_day_resolution with 0.5");
	ASSERT_TRUE(is_half_day_resolution(0.5));
	TEST_PASS();
}

static void test_half_day_one_and_half(void)
{
	TEST_START("is_half_day_resolution with 1.5");
	ASSERT_TRUE(is_half_day_resolution(1.5));
	TEST_PASS();
}

static void test_half_day_large_half(void)
{
	TEST_START("is_half_day_resolution with 10.5");
	ASSERT_TRUE(is_half_day_resolution(10.5));
	TEST_PASS();
}

static void test_half_day_quarter_rejected(void)
{
	TEST_START("is_half_day_resolution rejects 0.25");
	ASSERT_FALSE(is_half_day_resolution(0.25));
	TEST_PASS();
}

static void test_half_day_third_rejected(void)
{
	TEST_START("is_half_day_resolution rejects 0.33");
	ASSERT_FALSE(is_half_day_resolution(0.33));
	TEST_PASS();
}

static void test_half_day_three_quarters_rejected(void)
{
	TEST_START("is_half_day_resolution rejects 0.75");
	ASSERT_FALSE(is_half_day_resolution(0.75));
	TEST_PASS();
}

static void test_half_day_negative_rejected(void)
{
	TEST_START("is_half_day_resolution rejects negative");
	ASSERT_FALSE(is_half_day_resolution(-1.0));
	TEST_PASS();
}

static void test_half_day_arbitrary_rejected(void)
{
	TEST_START("is_half_day_resolution rejects 2.3");
	ASSERT_FALSE(is_half_day_resolution(2.3));
	TEST_PASS();
}

/*
 * Test: is_leap_year
 */

static void test_leap_year_divisible_by_4(void)
{
	TEST_START("is_leap_year divisible by 4");
	ASSERT_TRUE(is_leap_year(2024));
	TEST_PASS();
}

static void test_leap_year_divisible_by_100_not_400(void)
{
	TEST_START("is_leap_year divisible by 100 but not 400");
	ASSERT_FALSE(is_leap_year(1900));
	TEST_PASS();
}

static void test_leap_year_divisible_by_400(void)
{
	TEST_START("is_leap_year divisible by 400");
	ASSERT_TRUE(is_leap_year(2000));
	TEST_PASS();
}

static void test_leap_year_not_divisible_by_4(void)
{
	TEST_START("is_leap_year not divisible by 4");
	ASSERT_FALSE(is_leap_year(2023));
	TEST_PASS();
}

/*
 * Test: days_in_month
 */

static void test_days_in_month_january(void)
{
	TEST_START("days_in_month January");
	ASSERT_INT_EQ(31, days_in_month(1, 2025));
	TEST_PASS();
}

static void test_days_in_month_february_non_leap(void)
{
	TEST_START("days_in_month February non-leap year");
	ASSERT_INT_EQ(28, days_in_month(2, 2025));
	TEST_PASS();
}

static void test_days_in_month_february_leap(void)
{
	TEST_START("days_in_month February leap year");
	ASSERT_INT_EQ(29, days_in_month(2, 2024));
	TEST_PASS();
}

static void test_days_in_month_april(void)
{
	TEST_START("days_in_month April");
	ASSERT_INT_EQ(30, days_in_month(4, 2025));
	TEST_PASS();
}

static void test_days_in_month_december(void)
{
	TEST_START("days_in_month December");
	ASSERT_INT_EQ(31, days_in_month(12, 2025));
	TEST_PASS();
}

static void test_days_in_month_invalid_zero(void)
{
	TEST_START("days_in_month invalid month 0");
	ASSERT_INT_EQ(0, days_in_month(0, 2025));
	TEST_PASS();
}

static void test_days_in_month_invalid_thirteen(void)
{
	TEST_START("days_in_month invalid month 13");
	ASSERT_INT_EQ(0, days_in_month(13, 2025));
	TEST_PASS();
}

/*
 * Test: day_of_week
 */

static void test_day_of_week_known_sunday(void)
{
	TEST_START("day_of_week known Sunday (Dec 15, 2024)");
	/* December 15, 2024 is a Sunday */
	ASSERT_INT_EQ(0, day_of_week(15, 12, 2024));
	TEST_PASS();
}

static void test_day_of_week_known_monday(void)
{
	TEST_START("day_of_week known Monday (Dec 16, 2024)");
	/* December 16, 2024 is a Monday */
	ASSERT_INT_EQ(1, day_of_week(16, 12, 2024));
	TEST_PASS();
}

static void test_day_of_week_known_saturday(void)
{
	TEST_START("day_of_week known Saturday (Dec 14, 2024)");
	/* December 14, 2024 is a Saturday */
	ASSERT_INT_EQ(6, day_of_week(14, 12, 2024));
	TEST_PASS();
}

static void test_day_of_week_january_first_2025(void)
{
	TEST_START("day_of_week January 1, 2025 (Wednesday)");
	/* January 1, 2025 is a Wednesday */
	ASSERT_INT_EQ(3, day_of_week(1, 1, 2025));
	TEST_PASS();
}

static void test_day_of_week_leap_year_feb_29(void)
{
	TEST_START("day_of_week February 29, 2024 (Thursday)");
	/* February 29, 2024 is a Thursday */
	ASSERT_INT_EQ(4, day_of_week(29, 2, 2024));
	TEST_PASS();
}

/*
 * Test: calculate_working_days
 */

static void test_working_days_december_2024_sunday_start(void)
{
	TEST_START("calculate_working_days December 2024 (Sunday start)");
	/* December 2024: 31 days, starts on Sunday */
	/* Sun-Thu weeks, so working days are Sun, Mon, Tue, Wed, Thu */
	/* Week 1: Dec 1 (Sun) - Dec 5 (Thu) = 5 days */
	/* Week 2: Dec 8 (Sun) - Dec 12 (Thu) = 5 days */
	/* Week 3: Dec 15 (Sun) - Dec 19 (Thu) = 5 days */
	/* Week 4: Dec 22 (Sun) - Dec 26 (Thu) = 5 days */
	/* Week 5: Dec 29 (Sun) - Dec 31 (Tue) = 3 days */
	/* Total: 23 working days */
	ASSERT_INT_EQ(23, calculate_working_days(12, 2024, WEEK_START_SUNDAY));
	TEST_PASS();
}

static void test_working_days_december_2024_monday_start(void)
{
	TEST_START("calculate_working_days December 2024 (Monday start)");
	/* December 2024: 31 days, starts on Sunday */
	/* Mon-Fri weeks */
	/* Dec 2-6 (Mon-Fri) = 5 days */
	/* Dec 9-13 (Mon-Fri) = 5 days */
	/* Dec 16-20 (Mon-Fri) = 5 days */
	/* Dec 23-27 (Mon-Fri) = 5 days */
	/* Dec 30-31 (Mon-Tue) = 2 days */
	/* Total: 22 working days */
	ASSERT_INT_EQ(22, calculate_working_days(12, 2024, WEEK_START_MONDAY));
	TEST_PASS();
}

static void test_working_days_february_2024_leap_sunday(void)
{
	TEST_START("calculate_working_days February 2024 leap year (Sunday start)");
	/* February 2024: 29 days, starts on Thursday (Feb 1, 2024) */
	/* Working days calculation with Sunday start */
	int working = calculate_working_days(2, 2024, WEEK_START_SUNDAY);
	ASSERT_TRUE(working > 0 && working <= 29);
	TEST_PASS();
}

static void test_working_days_february_2025_sunday(void)
{
	TEST_START("calculate_working_days February 2025 (Sunday start)");
	/* February 2025: 28 days, starts on Saturday */
	int working = calculate_working_days(2, 2025, WEEK_START_SUNDAY);
	/* Should be around 20 working days */
	ASSERT_TRUE(working >= 18 && working <= 22);
	TEST_PASS();
}

static void test_working_days_january_2025_sunday(void)
{
	TEST_START("calculate_working_days January 2025 (Sunday start)");
	/* January 2025: 31 days, starts on Wednesday */
	int working = calculate_working_days(1, 2025, WEEK_START_SUNDAY);
	ASSERT_TRUE(working >= 20 && working <= 25);
	TEST_PASS();
}

static void test_working_days_january_2025_monday(void)
{
	TEST_START("calculate_working_days January 2025 (Monday start)");
	int working = calculate_working_days(1, 2025, WEEK_START_MONDAY);
	ASSERT_TRUE(working >= 20 && working <= 25);
	TEST_PASS();
}

static void test_working_days_invalid_month(void)
{
	TEST_START("calculate_working_days invalid month");
	ASSERT_INT_EQ(0, calculate_working_days(0, 2025, WEEK_START_SUNDAY));
	TEST_PASS();
}

/*
 * Test: calculate_remaining_working_days
 */

static void test_remaining_working_days_january(void)
{
	TEST_START("calculate_remaining_working_days from January");
	int remaining = calculate_remaining_working_days(1, 2025, WEEK_START_SUNDAY);
	/* Should be sum of all 12 months, roughly 250-265 working days */
	ASSERT_TRUE(remaining >= 250 && remaining <= 270);
	TEST_PASS();
}

static void test_remaining_working_days_december(void)
{
	TEST_START("calculate_remaining_working_days from December");
	int remaining = calculate_remaining_working_days(12, 2024, WEEK_START_SUNDAY);
	/* Should be just December's working days */
	ASSERT_INT_EQ(23, remaining);
	TEST_PASS();
}

static void test_remaining_working_days_mid_year(void)
{
	TEST_START("calculate_remaining_working_days from July");
	int remaining = calculate_remaining_working_days(7, 2025, WEEK_START_SUNDAY);
	/* Should be roughly half year, 125-135 days */
	ASSERT_TRUE(remaining >= 120 && remaining <= 140);
	TEST_PASS();
}

static void test_remaining_working_days_monday_start(void)
{
	TEST_START("calculate_remaining_working_days with Monday start");
	int remaining_sunday = calculate_remaining_working_days(12, 2024, WEEK_START_SUNDAY);
	int remaining_monday = calculate_remaining_working_days(12, 2024, WEEK_START_MONDAY);
	/* Should be different based on week start */
	ASSERT_INT_EQ(23, remaining_sunday);
	ASSERT_INT_EQ(22, remaining_monday);
	TEST_PASS();
}

/*
 * Test: calculate_working_days_from_prev_month
 */

static void test_working_days_from_prev_month_january(void)
{
	TEST_START("calculate_working_days_from_prev_month from January");
	/* Should include December of previous year plus all of current year */
	int days = calculate_working_days_from_prev_month(1, 2025, WEEK_START_SUNDAY);
	/* December 2024 has 23 working days (Sun-Thu), plus full year 2025 */
	int dec_2024 = calculate_working_days(12, 2024, WEEK_START_SUNDAY);
	int full_2025 = calculate_remaining_working_days(1, 2025, WEEK_START_SUNDAY);
	ASSERT_INT_EQ(dec_2024 + full_2025, days);
	TEST_PASS();
}

static void test_working_days_from_prev_month_february(void)
{
	TEST_START("calculate_working_days_from_prev_month from February");
	/* Should include January plus Feb to Dec */
	int days = calculate_working_days_from_prev_month(2, 2025, WEEK_START_SUNDAY);
	int expected = calculate_remaining_working_days(1, 2025, WEEK_START_SUNDAY);
	ASSERT_INT_EQ(expected, days);
	TEST_PASS();
}

static void test_working_days_from_prev_month_december(void)
{
	TEST_START("calculate_working_days_from_prev_month from December");
	/* Should include November plus December */
	int days = calculate_working_days_from_prev_month(12, 2024, WEEK_START_SUNDAY);
	int nov = calculate_working_days(11, 2024, WEEK_START_SUNDAY);
	int dec = calculate_working_days(12, 2024, WEEK_START_SUNDAY);
	ASSERT_INT_EQ(nov + dec, days);
	TEST_PASS();
}

static void test_working_days_from_prev_month_mid_year(void)
{
	TEST_START("calculate_working_days_from_prev_month from July");
	/* Should include June plus July to Dec */
	int days = calculate_working_days_from_prev_month(7, 2025, WEEK_START_SUNDAY);
	int expected = calculate_remaining_working_days(6, 2025, WEEK_START_SUNDAY);
	ASSERT_INT_EQ(expected, days);
	TEST_PASS();
}

static void test_working_days_from_prev_month_monday_start(void)
{
	TEST_START("calculate_working_days_from_prev_month with Monday start");
	int days_sunday = calculate_working_days_from_prev_month(12, 2024, WEEK_START_SUNDAY);
	int days_monday = calculate_working_days_from_prev_month(12, 2024, WEEK_START_MONDAY);
	/* November 2024 + December 2024 */
	int nov_sun = calculate_working_days(11, 2024, WEEK_START_SUNDAY);
	int dec_sun = calculate_working_days(12, 2024, WEEK_START_SUNDAY);
	int nov_mon = calculate_working_days(11, 2024, WEEK_START_MONDAY);
	int dec_mon = calculate_working_days(12, 2024, WEEK_START_MONDAY);
	ASSERT_INT_EQ(nov_sun + dec_sun, days_sunday);
	ASSERT_INT_EQ(nov_mon + dec_mon, days_monday);
	TEST_PASS();
}

/*
 * Test: validate_vacation_days
 */

static void test_validate_vacation_days_zero(void)
{
	TEST_START("validate_vacation_days zero");
	ASSERT_INT_EQ(0, validate_vacation_days(0.0, 250));
	TEST_PASS();
}

static void test_validate_vacation_days_valid_integer(void)
{
	TEST_START("validate_vacation_days valid integer");
	ASSERT_INT_EQ(0, validate_vacation_days(5.0, 250));
	TEST_PASS();
}

static void test_validate_vacation_days_valid_half(void)
{
	TEST_START("validate_vacation_days valid half day");
	ASSERT_INT_EQ(0, validate_vacation_days(2.5, 250));
	TEST_PASS();
}

static void test_validate_vacation_days_max_remaining(void)
{
	TEST_START("validate_vacation_days at max remaining days");
	ASSERT_INT_EQ(0, validate_vacation_days(250.0, 250));
	TEST_PASS();
}

static void test_validate_vacation_days_exceeds_remaining(void)
{
	TEST_START("validate_vacation_days exceeds remaining days");
	ASSERT_INT_EQ(1, validate_vacation_days(251.0, 250));
	TEST_PASS();
}

static void test_validate_vacation_days_negative(void)
{
	TEST_START("validate_vacation_days negative");
	ASSERT_INT_EQ(1, validate_vacation_days(-1.0, 250));
	TEST_PASS();
}

static void test_validate_vacation_days_invalid_resolution(void)
{
	TEST_START("validate_vacation_days invalid resolution (0.25)");
	ASSERT_INT_EQ(1, validate_vacation_days(0.25, 250));
	TEST_PASS();
}

static void test_validate_vacation_days_invalid_resolution_quarter(void)
{
	TEST_START("validate_vacation_days invalid resolution (2.75)");
	ASSERT_INT_EQ(1, validate_vacation_days(2.75, 250));
	TEST_PASS();
}

/*
 * Test: hours_to_days
 */

static void test_hours_to_days_zero(void)
{
	TEST_START("hours_to_days with zero");
	ASSERT_DOUBLE_EQ(0.0, hours_to_days(0.0));
	TEST_PASS();
}

static void test_hours_to_days_one_day(void)
{
	TEST_START("hours_to_days with one work day");
	ASSERT_DOUBLE_EQ(1.0, hours_to_days(WORK_DAY_HOURS));
	TEST_PASS();
}

static void test_hours_to_days_multiple_days(void)
{
	TEST_START("hours_to_days with multiple days");
	ASSERT_DOUBLE_EQ(5.0, hours_to_days(5.0 * WORK_DAY_HOURS));
	TEST_PASS();
}

static void test_hours_to_days_fractional(void)
{
	TEST_START("hours_to_days with fractional result");
	double hours = WORK_DAY_HOURS * 2.5;

	ASSERT_DOUBLE_EQ(2.5, hours_to_days(hours));
	TEST_PASS();
}

static void test_hours_to_days_negative(void)
{
	TEST_START("hours_to_days with negative hours");
	ASSERT_DOUBLE_EQ(-1.0, hours_to_days(-WORK_DAY_HOURS));
	TEST_PASS();
}

/*
 * Test: days_to_hours
 */

static void test_days_to_hours_zero(void)
{
	TEST_START("days_to_hours with zero");
	ASSERT_DOUBLE_EQ(0.0, days_to_hours(0.0));
	TEST_PASS();
}

static void test_days_to_hours_one_day(void)
{
	TEST_START("days_to_hours with one day");
	ASSERT_DOUBLE_EQ(WORK_DAY_HOURS, days_to_hours(1.0));
	TEST_PASS();
}

static void test_days_to_hours_multiple_days(void)
{
	TEST_START("days_to_hours with multiple days");
	ASSERT_DOUBLE_EQ(WORK_DAY_HOURS * 10.0, days_to_hours(10.0));
	TEST_PASS();
}

static void test_days_to_hours_fractional(void)
{
	TEST_START("days_to_hours with fractional days");
	ASSERT_DOUBLE_EQ(WORK_DAY_HOURS * 0.5, days_to_hours(0.5));
	TEST_PASS();
}

static void test_days_to_hours_negative(void)
{
	TEST_START("days_to_hours with negative days");
	ASSERT_DOUBLE_EQ(-WORK_DAY_HOURS, days_to_hours(-1.0));
	TEST_PASS();
}

/*
 * Test: hours_to_days and days_to_hours roundtrip
 */

static void test_conversion_roundtrip(void)
{
	TEST_START("hours_to_days and days_to_hours roundtrip");
	double original_hours = 123.456;
	double days = hours_to_days(original_hours);
	double result_hours = days_to_hours(days);

	ASSERT_DOUBLE_EQ(original_hours, result_hours);
	TEST_PASS();
}

/*
 * Test: calculate_remaining_months
 */

static void test_remaining_months_january(void)
{
	TEST_START("calculate_remaining_months for January");
	ASSERT_INT_EQ(12, calculate_remaining_months(1));
	TEST_PASS();
}

static void test_remaining_months_june(void)
{
	TEST_START("calculate_remaining_months for June");
	ASSERT_INT_EQ(7, calculate_remaining_months(6));
	TEST_PASS();
}

static void test_remaining_months_december(void)
{
	TEST_START("calculate_remaining_months for December");
	ASSERT_INT_EQ(1, calculate_remaining_months(12));
	TEST_PASS();
}

static void test_remaining_months_invalid_zero(void)
{
	TEST_START("calculate_remaining_months with invalid month 0");
	ASSERT_INT_EQ(0, calculate_remaining_months(0));
	TEST_PASS();
}

static void test_remaining_months_invalid_negative(void)
{
	TEST_START("calculate_remaining_months with negative month");
	ASSERT_INT_EQ(0, calculate_remaining_months(-1));
	TEST_PASS();
}

static void test_remaining_months_invalid_thirteen(void)
{
	TEST_START("calculate_remaining_months with month 13");
	ASSERT_INT_EQ(0, calculate_remaining_months(13));
	TEST_PASS();
}

static void test_remaining_months_invalid_large(void)
{
	TEST_START("calculate_remaining_months with large invalid month");
	ASSERT_INT_EQ(0, calculate_remaining_months(100));
	TEST_PASS();
}

/*
 * Test: calculate_monthly_hours
 */

static void test_monthly_hours_default(void)
{
	TEST_START("calculate_monthly_hours with default annual days");
	double expected = days_to_hours((double)DEFAULT_ANNUAL_DAYS) / MONTHS_PER_YEAR;

	ASSERT_DOUBLE_EQ(expected, calculate_monthly_hours(DEFAULT_ANNUAL_DAYS));
	TEST_PASS();
}

static void test_monthly_hours_zero(void)
{
	TEST_START("calculate_monthly_hours with zero");
	ASSERT_DOUBLE_EQ(0.0, calculate_monthly_hours(0));
	TEST_PASS();
}

static void test_monthly_hours_custom(void)
{
	TEST_START("calculate_monthly_hours with 12 days (1 day per month)");
	double expected = days_to_hours(12.0) / 12.0;

	ASSERT_DOUBLE_EQ(expected, calculate_monthly_hours(12));
	TEST_PASS();
}

static void test_monthly_hours_large(void)
{
	TEST_START("calculate_monthly_hours with large value");
	double expected = days_to_hours(120.0) / 12.0;

	ASSERT_DOUBLE_EQ(expected, calculate_monthly_hours(120));
	TEST_PASS();
}

/*
 * Test: calculate_additional_hours
 */

static void test_additional_hours_full_year(void)
{
	TEST_START("calculate_additional_hours for full year");
	ASSERT_DOUBLE_EQ(120.0, calculate_additional_hours(12, 10.0));
	TEST_PASS();
}

static void test_additional_hours_one_month(void)
{
	TEST_START("calculate_additional_hours for one month");
	ASSERT_DOUBLE_EQ(10.0, calculate_additional_hours(1, 10.0));
	TEST_PASS();
}

static void test_additional_hours_zero_months(void)
{
	TEST_START("calculate_additional_hours for zero months");
	ASSERT_DOUBLE_EQ(0.0, calculate_additional_hours(0, 10.0));
	TEST_PASS();
}

static void test_additional_hours_zero_monthly(void)
{
	TEST_START("calculate_additional_hours with zero monthly hours");
	ASSERT_DOUBLE_EQ(0.0, calculate_additional_hours(6, 0.0));
	TEST_PASS();
}

static void test_additional_hours_negative_months(void)
{
	TEST_START("calculate_additional_hours with negative months");
	ASSERT_DOUBLE_EQ(0.0, calculate_additional_hours(-5, 10.0));
	TEST_PASS();
}

static void test_additional_hours_half_year(void)
{
	TEST_START("calculate_additional_hours for half year");
	ASSERT_DOUBLE_EQ(60.0, calculate_additional_hours(6, 10.0));
	TEST_PASS();
}

/*
 * Test: calculate_total_hours
 */

static void test_total_hours_basic(void)
{
	TEST_START("calculate_total_hours basic");
	ASSERT_DOUBLE_EQ(150.0, calculate_total_hours(100.0, 50.0));
	TEST_PASS();
}

static void test_total_hours_zero_current(void)
{
	TEST_START("calculate_total_hours with zero current");
	ASSERT_DOUBLE_EQ(50.0, calculate_total_hours(0.0, 50.0));
	TEST_PASS();
}

static void test_total_hours_zero_additional(void)
{
	TEST_START("calculate_total_hours with zero additional");
	ASSERT_DOUBLE_EQ(100.0, calculate_total_hours(100.0, 0.0));
	TEST_PASS();
}

static void test_total_hours_both_zero(void)
{
	TEST_START("calculate_total_hours with both zero");
	ASSERT_DOUBLE_EQ(0.0, calculate_total_hours(0.0, 0.0));
	TEST_PASS();
}

static void test_total_hours_large_values(void)
{
	TEST_START("calculate_total_hours with large values");
	ASSERT_DOUBLE_EQ(10000.0, calculate_total_hours(5000.0, 5000.0));
	TEST_PASS();
}

/*
 * Test: round_total_days
 */

static void test_round_total_days_exact(void)
{
	TEST_START("round_total_days with exact integer");
	ASSERT_INT_EQ(30, round_total_days(30.0));
	TEST_PASS();
}

static void test_round_total_days_round_down(void)
{
	TEST_START("round_total_days rounds down");
	ASSERT_INT_EQ(30, round_total_days(30.4));
	TEST_PASS();
}

static void test_round_total_days_round_up(void)
{
	TEST_START("round_total_days rounds up");
	ASSERT_INT_EQ(31, round_total_days(30.6));
	TEST_PASS();
}

static void test_round_total_days_half_rounds_up(void)
{
	TEST_START("round_total_days half rounds up");
	ASSERT_INT_EQ(31, round_total_days(30.5));
	TEST_PASS();
}

static void test_round_total_days_zero(void)
{
	TEST_START("round_total_days with zero");
	ASSERT_INT_EQ(0, round_total_days(0.0));
	TEST_PASS();
}

static void test_round_total_days_small_fraction(void)
{
	TEST_START("round_total_days with small fraction");
	ASSERT_INT_EQ(20, round_total_days(19.88));
	TEST_PASS();
}

/*
 * Test: calculate_excess_days
 */

static void test_excess_days_under_limit(void)
{
	TEST_START("calculate_excess_days under limit");
	ASSERT_INT_EQ(0, calculate_excess_days(30, 36));
	TEST_PASS();
}

static void test_excess_days_at_limit(void)
{
	TEST_START("calculate_excess_days at limit");
	ASSERT_INT_EQ(0, calculate_excess_days(36, 36));
	TEST_PASS();
}

static void test_excess_days_over_limit(void)
{
	TEST_START("calculate_excess_days over limit");
	ASSERT_INT_EQ(4, calculate_excess_days(40, 36));
	TEST_PASS();
}

static void test_excess_days_way_over_limit(void)
{
	TEST_START("calculate_excess_days way over limit");
	ASSERT_INT_EQ(64, calculate_excess_days(100, 36));
	TEST_PASS();
}

static void test_excess_days_zero_total(void)
{
	TEST_START("calculate_excess_days with zero total");
	ASSERT_INT_EQ(0, calculate_excess_days(0, 36));
	TEST_PASS();
}

static void test_excess_days_zero_limit(void)
{
	TEST_START("calculate_excess_days with zero limit");
	ASSERT_INT_EQ(10, calculate_excess_days(10, 0));
	TEST_PASS();
}

static void test_excess_days_one_over(void)
{
	TEST_START("calculate_excess_days one over limit");
	ASSERT_INT_EQ(1, calculate_excess_days(37, 36));
	TEST_PASS();
}

/*
 * Test: calculate_vacation (integration)
 */

static void test_calculate_vacation_full_year_january(void)
{
	struct vacation_input input;
	struct vacation_result result;

	TEST_START("calculate_vacation for January with zero current hours");
	input.annual_days = 24;
	input.max_accum_days = 36;
	input.current_hours = 0.0;
	input.current_month = 1;
	input.current_year = 2025;
	input.vacation_extra = 0.0;
	input.week_start = WEEK_START_SUNDAY;

	calculate_vacation(&input, &result);

	ASSERT_INT_EQ(12, result.remaining_months);
	ASSERT_DOUBLE_EQ(24.0, result.additional_days);
	ASSERT_INT_EQ(24, result.total_days);
	ASSERT_INT_EQ(0, result.excess_days);
	TEST_PASS();
}

static void test_calculate_vacation_december_excess(void)
{
	struct vacation_input input;
	struct vacation_result result;

	TEST_START("calculate_vacation for December with excess");
	input.annual_days = 24;
	input.max_accum_days = 36;
	input.current_hours = days_to_hours(40.0);
	input.current_month = 12;
	input.current_year = 2024;
	input.vacation_extra = 0.0;
	input.week_start = WEEK_START_SUNDAY;

	calculate_vacation(&input, &result);

	ASSERT_INT_EQ(1, result.remaining_months);
	ASSERT_DOUBLE_EQ(2.0, result.additional_days);
	ASSERT_INT_EQ(42, result.total_days);
	ASSERT_INT_EQ(6, result.excess_days);
	TEST_PASS();
}

static void test_calculate_vacation_with_vacation_extra(void)
{
	struct vacation_input input;
	struct vacation_result result;

	TEST_START("calculate_vacation with vacation extra");
	input.annual_days = 24;
	input.max_accum_days = 36;
	input.current_hours = days_to_hours(20.0);
	input.current_month = 12;
	input.current_year = 2024;
	input.vacation_extra = 5.0;	/* Extra 5 days */
	input.week_start = WEEK_START_SUNDAY;

	calculate_vacation(&input, &result);

	/* 20 + 2 - 5 = 17 days */
	ASSERT_INT_EQ(17, result.total_days);
	ASSERT_INT_EQ(0, result.excess_days);
	TEST_PASS();
}

static void test_calculate_vacation_with_half_day_vacation(void)
{
	struct vacation_input input;
	struct vacation_result result;

	TEST_START("calculate_vacation with half day vacation");
	input.annual_days = 24;
	input.max_accum_days = 36;
	input.current_hours = days_to_hours(20.0);
	input.current_month = 12;
	input.current_year = 2024;
	input.vacation_extra = 2.5;	/* Extra 2.5 days */
	input.week_start = WEEK_START_SUNDAY;

	calculate_vacation(&input, &result);

	/* 20 + 2 - 2.5 = 19.5 rounds to 19 (standard rounding) */
	ASSERT_INT_EQ(19, result.total_days);
	TEST_PASS();
}

static void test_calculate_vacation_vacation_prevents_excess(void)
{
	struct vacation_input input;
	struct vacation_result result;

	TEST_START("calculate_vacation - vacation prevents excess");
	input.annual_days = 24;
	input.max_accum_days = 36;
	input.current_hours = days_to_hours(40.0);
	input.current_month = 12;
	input.current_year = 2024;
	input.vacation_extra = 6.0;	/* Extra enough to avoid excess */
	input.week_start = WEEK_START_SUNDAY;

	calculate_vacation(&input, &result);

	/* 40 + 2 - 6 = 36 days (at limit) */
	ASSERT_INT_EQ(36, result.total_days);
	ASSERT_INT_EQ(0, result.excess_days);
	TEST_PASS();
}

static void test_calculate_vacation_remaining_days_sunday(void)
{
	struct vacation_input input;
	struct vacation_result result;

	TEST_START("calculate_vacation stores remaining working days (Sunday start)");
	input.annual_days = 24;
	input.max_accum_days = 36;
	input.current_hours = 0.0;
	input.current_month = 12;
	input.current_year = 2024;
	input.vacation_extra = 0.0;
	input.week_start = WEEK_START_SUNDAY;

	calculate_vacation(&input, &result);

	ASSERT_INT_EQ(23, result.working_days_remaining);
	TEST_PASS();
}

static void test_calculate_vacation_remaining_days_monday(void)
{
	struct vacation_input input;
	struct vacation_result result;

	TEST_START("calculate_vacation stores remaining working days (Monday start)");
	input.annual_days = 24;
	input.max_accum_days = 36;
	input.current_hours = 0.0;
	input.current_month = 12;
	input.current_year = 2024;
	input.vacation_extra = 0.0;
	input.week_start = WEEK_START_MONDAY;

	calculate_vacation(&input, &result);

	ASSERT_INT_EQ(22, result.working_days_remaining);
	TEST_PASS();
}

/*
 * Test: init_vacation_args
 */

static void test_init_vacation_args(void)
{
	struct vacation_args args;

	TEST_START("init_vacation_args sets defaults");
	init_vacation_args(&args);

	ASSERT_INT_EQ(DEFAULT_ANNUAL_DAYS, args.annual_days);
	ASSERT_INT_EQ(DEFAULT_MAX_ACCUM_DAYS, args.max_accum_days);
	ASSERT_DOUBLE_EQ(0.0, args.current_hours);
	ASSERT_INT_EQ(0, args.annual_days_set);
	ASSERT_INT_EQ(0, args.annual_hours_set);
	ASSERT_INT_EQ(0, args.current_hours_set);
	ASSERT_DOUBLE_EQ(0.0, args.vacation_extra);
	ASSERT_INT_EQ(0, args.vacation_extra_set);
	ASSERT_INT_EQ(WEEK_START_SUNDAY, args.week_start);
	TEST_PASS();
}

/*
 * Test: validate_arguments
 */

static void test_validate_args_neither_set(void)
{
	struct vacation_args args;

	TEST_START("validate_arguments with neither annual option set");
	init_vacation_args(&args);

	ASSERT_INT_EQ(0, validate_arguments(&args));
	TEST_PASS();
}

static void test_validate_args_days_only(void)
{
	struct vacation_args args;

	TEST_START("validate_arguments with only annual-days set");
	init_vacation_args(&args);
	args.annual_days_set = 1;

	ASSERT_INT_EQ(0, validate_arguments(&args));
	TEST_PASS();
}

static void test_validate_args_hours_only(void)
{
	struct vacation_args args;

	TEST_START("validate_arguments with only annual-hours set");
	init_vacation_args(&args);
	args.annual_hours_set = 1;

	ASSERT_INT_EQ(0, validate_arguments(&args));
	TEST_PASS();
}

static void test_validate_args_both_set(void)
{
	struct vacation_args args;

	TEST_START("validate_arguments with both annual options set");
	init_vacation_args(&args);
	args.annual_days_set = 1;
	args.annual_hours_set = 1;

	ASSERT_INT_EQ(1, validate_arguments(&args));
	TEST_PASS();
}

/*
 * Test: get_current_month and get_current_year
 */

static void test_get_current_month_valid_range(void)
{
	int month;

	TEST_START("get_current_month returns valid month");
	month = get_current_month();

	ASSERT_TRUE(month >= 1 && month <= 12);
	TEST_PASS();
}

static void test_get_current_year_valid_range(void)
{
	int year;

	TEST_START("get_current_year returns valid year");
	year = get_current_year();

	ASSERT_TRUE(year >= 2024 && year <= 2100);
	TEST_PASS();
}

/*
 * Test suites
 */

static void run_parse_integer_tests(void)
{
	printf("\n[Integer Parsing]\n");
	test_parse_integer_positive();
	test_parse_integer_negative();
	test_parse_integer_zero();
	test_parse_integer_with_whitespace();
	test_parse_integer_float_rejected();
	test_parse_integer_text_rejected();
	test_parse_integer_mixed_rejected();
	test_parse_integer_empty_rejected();
	test_parse_integer_whitespace_only_rejected();
}

static void run_parse_number_tests(void)
{
	printf("\n[Number Parsing]\n");
	test_parse_number_integer();
	test_parse_number_float();
	test_parse_number_negative_float();
	test_parse_number_with_whitespace();
	test_parse_number_text_rejected();
	test_parse_number_empty_rejected();
}

static void run_half_day_resolution_tests(void)
{
	printf("\n[Half Day Resolution]\n");
	test_half_day_zero();
	test_half_day_integer();
	test_half_day_half();
	test_half_day_one_and_half();
	test_half_day_large_half();
	test_half_day_quarter_rejected();
	test_half_day_third_rejected();
	test_half_day_three_quarters_rejected();
	test_half_day_negative_rejected();
	test_half_day_arbitrary_rejected();
}

static void run_leap_year_tests(void)
{
	printf("\n[Leap Year]\n");
	test_leap_year_divisible_by_4();
	test_leap_year_divisible_by_100_not_400();
	test_leap_year_divisible_by_400();
	test_leap_year_not_divisible_by_4();
}

static void run_days_in_month_tests(void)
{
	printf("\n[Days in Month]\n");
	test_days_in_month_january();
	test_days_in_month_february_non_leap();
	test_days_in_month_february_leap();
	test_days_in_month_april();
	test_days_in_month_december();
	test_days_in_month_invalid_zero();
	test_days_in_month_invalid_thirteen();
}

static void run_day_of_week_tests(void)
{
	printf("\n[Day of Week]\n");
	test_day_of_week_known_sunday();
	test_day_of_week_known_monday();
	test_day_of_week_known_saturday();
	test_day_of_week_january_first_2025();
	test_day_of_week_leap_year_feb_29();
}

static void run_working_days_tests(void)
{
	printf("\n[Working Days Calculation]\n");
	test_working_days_december_2024_sunday_start();
	test_working_days_december_2024_monday_start();
	test_working_days_february_2024_leap_sunday();
	test_working_days_february_2025_sunday();
	test_working_days_january_2025_sunday();
	test_working_days_january_2025_monday();
	test_working_days_invalid_month();
}

static void run_remaining_working_days_tests(void)
{
	printf("\n[Remaining Working Days Calculation]\n");
	test_remaining_working_days_january();
	test_remaining_working_days_december();
	test_remaining_working_days_mid_year();
	test_remaining_working_days_monday_start();
}

static void run_working_days_from_prev_month_tests(void)
{
	printf("\n[Working Days From Previous Month]\n");
	test_working_days_from_prev_month_january();
	test_working_days_from_prev_month_february();
	test_working_days_from_prev_month_december();
	test_working_days_from_prev_month_mid_year();
	test_working_days_from_prev_month_monday_start();
}

static void run_validate_vacation_days_tests(void)
{
	printf("\n[Validate Vacation Days]\n");
	test_validate_vacation_days_zero();
	test_validate_vacation_days_valid_integer();
	test_validate_vacation_days_valid_half();
	test_validate_vacation_days_max_remaining();
	test_validate_vacation_days_exceeds_remaining();
	test_validate_vacation_days_negative();
	test_validate_vacation_days_invalid_resolution();
	test_validate_vacation_days_invalid_resolution_quarter();
}

static void run_conversion_tests(void)
{
	printf("\n[Conversion Functions]\n");
	test_hours_to_days_zero();
	test_hours_to_days_one_day();
	test_hours_to_days_multiple_days();
	test_hours_to_days_fractional();
	test_hours_to_days_negative();
	test_days_to_hours_zero();
	test_days_to_hours_one_day();
	test_days_to_hours_multiple_days();
	test_days_to_hours_fractional();
	test_days_to_hours_negative();
	test_conversion_roundtrip();
}

static void run_remaining_months_tests(void)
{
	printf("\n[Remaining Months Calculation]\n");
	test_remaining_months_january();
	test_remaining_months_june();
	test_remaining_months_december();
	test_remaining_months_invalid_zero();
	test_remaining_months_invalid_negative();
	test_remaining_months_invalid_thirteen();
	test_remaining_months_invalid_large();
}

static void run_monthly_hours_tests(void)
{
	printf("\n[Monthly Hours Calculation]\n");
	test_monthly_hours_default();
	test_monthly_hours_zero();
	test_monthly_hours_custom();
	test_monthly_hours_large();
}

static void run_additional_hours_tests(void)
{
	printf("\n[Additional Hours Calculation]\n");
	test_additional_hours_full_year();
	test_additional_hours_one_month();
	test_additional_hours_zero_months();
	test_additional_hours_zero_monthly();
	test_additional_hours_negative_months();
	test_additional_hours_half_year();
}

static void run_total_hours_tests(void)
{
	printf("\n[Total Hours Calculation]\n");
	test_total_hours_basic();
	test_total_hours_zero_current();
	test_total_hours_zero_additional();
	test_total_hours_both_zero();
	test_total_hours_large_values();
}

static void run_round_total_days_tests(void)
{
	printf("\n[Rounding Total Days]\n");
	test_round_total_days_exact();
	test_round_total_days_round_down();
	test_round_total_days_round_up();
	test_round_total_days_half_rounds_up();
	test_round_total_days_zero();
	test_round_total_days_small_fraction();
}

static void run_excess_days_tests(void)
{
	printf("\n[Excess Days Calculation]\n");
	test_excess_days_under_limit();
	test_excess_days_at_limit();
	test_excess_days_over_limit();
	test_excess_days_way_over_limit();
	test_excess_days_zero_total();
	test_excess_days_zero_limit();
	test_excess_days_one_over();
}

static void run_integration_tests(void)
{
	printf("\n[Integration Tests - calculate_vacation]\n");
	test_calculate_vacation_full_year_january();
	test_calculate_vacation_december_excess();
	test_calculate_vacation_with_vacation_extra();
	test_calculate_vacation_with_half_day_vacation();
	test_calculate_vacation_vacation_prevents_excess();
	test_calculate_vacation_remaining_days_sunday();
	test_calculate_vacation_remaining_days_monday();
}

static void run_args_tests(void)
{
	printf("\n[Argument Handling]\n");
	test_init_vacation_args();
	test_validate_args_neither_set();
	test_validate_args_days_only();
	test_validate_args_hours_only();
	test_validate_args_both_set();
}

static void run_time_tests(void)
{
	printf("\n[Time Functions]\n");
	test_get_current_month_valid_range();
	test_get_current_year_valid_range();
}

int main(void)
{
	printf("=== Vacation Calculator Unit Tests ===\n");

	run_parse_integer_tests();
	run_parse_number_tests();
	run_half_day_resolution_tests();
	run_leap_year_tests();
	run_days_in_month_tests();
	run_day_of_week_tests();
	run_working_days_tests();
	run_remaining_working_days_tests();
	run_working_days_from_prev_month_tests();
	run_validate_vacation_days_tests();
	run_conversion_tests();
	run_remaining_months_tests();
	run_monthly_hours_tests();
	run_additional_hours_tests();
	run_total_hours_tests();
	run_round_total_days_tests();
	run_excess_days_tests();
	run_integration_tests();
	run_args_tests();
	run_time_tests();

	printf("\n========================================\n");
	printf("Tests run:    %d\n", tests_run);
	printf("Tests passed: %d\n", tests_passed);
	printf("Tests failed: %d\n", tests_failed);
	printf("========================================\n");

	return (tests_failed > 0) ? 1 : 0;
}
