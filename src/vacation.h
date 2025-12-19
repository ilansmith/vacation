#ifndef VACATION_H
#define VACATION_H

/* Constants */
#define WORK_DAY_HOURS		8.4
#define MONTHS_PER_YEAR		12
#define DEFAULT_ANNUAL_DAYS	24
#define DEFAULT_MAX_ACCUM_DAYS	36
#define MIN_MAX_ACCUM_DAYS	36
#define MAX_MAX_ACCUM_DAYS	48
#define WORKING_DAYS_PER_WEEK	5
#define MIN_ANNUAL_DAYS		15
#define MAX_ANNUAL_DAYS		24
#define NUM_VALID_ANNUAL_OPTIONS 10

/* Valid annual vacation options (days and corresponding hours) */
extern const int VALID_ANNUAL_DAYS[NUM_VALID_ANNUAL_OPTIONS];
extern const int VALID_ANNUAL_HOURS[NUM_VALID_ANNUAL_OPTIONS];

/* Week start options */
#define WEEK_START_SUNDAY	0
#define WEEK_START_MONDAY	1

/* Structure to hold calculation inputs */
struct vacation_input {
	int annual_days;	/* Must be integer (15-24) */
	int max_accum_days;	/* Must be integer (36-48) */
	double current_hours;	/* May be floating point */
	int current_month;	/* 1-12 */
	int current_year;	/* e.g., 2025 */
	double vacation_extra;	/* Extra vacation days consumption (X or X.5) */
	int week_start;		/* WEEK_START_SUNDAY or WEEK_START_MONDAY */
};

/* Structure to hold calculation results */
struct vacation_result {
	int remaining_months;
	double monthly_hours;
	double additional_hours;
	double additional_days;
	double total_hours;
	double total_days;	/* Total accumulated days at year end */
	double excess_days;	/* Days to be deducted (total_days - max_accum_days) */
	int working_days_remaining;	/* Remaining working days this year */
};

/* Structure to hold parsed arguments */
struct vacation_args {
	int annual_days;	/* Must be integer (15-24) */
	double current_hours;	/* May be floating point */
	int annual_days_set;
	int annual_hours_set;
	int current_hours_set;
	double vacation_extra;	/* Extra vacation days consumption */
	int vacation_extra_set;
	int week_start;		/* WEEK_START_SUNDAY or WEEK_START_MONDAY */
	int special_accum;	/* If set, use special max accum calculation */
};

/*
 * Conversion functions
 */
double hours_to_days(double hours);
double days_to_hours(double days);

/*
 * Check if a string represents a valid integer.
 * str: string to check
 * value: pointer to store the integer value
 * Returns: 0 on success, non-zero on error
 */
int parse_integer(const char *str, int *value);

/*
 * Check if a string represents a valid number (integer or float).
 * str: string to check
 * value: pointer to store the value
 * Returns: 0 on success, non-zero on error
 */
int parse_number(const char *str, double *value);

/*
 * Get current month from system time.
 * Returns month number (1-12).
 */
int get_current_month(void);

/*
 * Get current year from system time.
 * Returns year (e.g., 2025).
 */
int get_current_year(void);

/*
 * Check if a year is a leap year.
 * year: the year to check
 * Returns: 1 if leap year, 0 otherwise
 */
int is_leap_year(int year);

/*
 * Get the number of days in a month.
 * month: month number (1-12)
 * year: year (for leap year calculation)
 * Returns: number of days in the month (28-31), or 0 for invalid month
 */
int days_in_month(int month, int year);

/*
 * Get the day of week for a given date (Zeller's congruence).
 * day: day of month (1-31)
 * month: month (1-12)
 * year: year (e.g., 2025)
 * Returns: day of week (0=Sunday, 1=Monday, ..., 6=Saturday)
 */
int day_of_week(int day, int month, int year);

/*
 * Calculate the number of working days in a month.
 * month: month number (1-12)
 * year: year (e.g., 2025)
 * week_start: WEEK_START_SUNDAY or WEEK_START_MONDAY
 * Returns: number of working days
 */
int calculate_working_days(int month, int year, int week_start);

/*
 * Calculate the remaining working days from current month to end of year.
 * current_month: current month (1-12)
 * year: year (e.g., 2025)
 * week_start: WEEK_START_SUNDAY or WEEK_START_MONDAY
 * Returns: total remaining working days
 */
int calculate_remaining_working_days(int current_month, int year, int week_start);

/*
 * Calculate working days from the start of the previous month to end of year.
 * current_month: current month (1-12)
 * year: year (e.g., 2025)
 * week_start: WEEK_START_SUNDAY or WEEK_START_MONDAY
 * Returns: total working days from previous month start to year end
 */
int calculate_working_days_from_prev_month(int current_month, int year, int week_start);

/*
 * Calculate remaining months in the year (including current month).
 * month: current month (1-12)
 * Returns: number of remaining months (1-12)
 */
int calculate_remaining_months(int month);

/*
 * Calculate monthly vacation hours credit.
 * annual_days: total annual vacation days
 * Returns: monthly hours credit
 */
double calculate_monthly_hours(int annual_days);

/*
 * Calculate additional vacation hours expected by year end.
 * remaining_months: number of months remaining (including current)
 * monthly_hours: monthly vacation hours credit
 * Returns: additional hours to be credited
 */
double calculate_additional_hours(int remaining_months, double monthly_hours);

/*
 * Calculate total accumulated vacation hours.
 * current_hours: current accumulated hours
 * additional_hours: hours to be added by year end
 * Returns: total hours at year end
 */
double calculate_total_hours(double current_hours, double additional_hours);

/*
 * Calculate excess days that will be deducted.
 * total_days: total accumulated days at year end
 * max_accum_days: maximum allowed accumulated days
 * Returns: days to be deducted (0 if under limit)
 */
double calculate_excess_days(double total_days, int max_accum_days);

/*
 * Perform all vacation calculations.
 * input: calculation inputs
 * result: structure to store results
 */
void calculate_vacation(const struct vacation_input *input,
			struct vacation_result *result);

/*
 * Initialize vacation_args with default values.
 * args: structure to initialize
 */
void init_vacation_args(struct vacation_args *args);

/*
 * Validate parsed arguments.
 * args: parsed arguments to validate
 * Returns: 0 on success, non-zero on error
 */
int validate_arguments(const struct vacation_args *args);

/*
 * Validate annual days are within allowed range.
 * annual_days: annual vacation days to validate
 * Returns: 0 on success, non-zero on error
 */
int validate_annual_days(int annual_days);

/*
 * Calculate maximum accumulated days based on annual days and special accum flag.
 * Normal case: always returns 36
 * Special accum:
 *   For 15-17 annual days: returns 36
 *   For 18-24 annual days: returns annual_days * 2
 * annual_days: annual vacation days
 * special_accum: 1 if special accumulation, 0 otherwise
 * Returns: calculated maximum accumulated days
 */
int calculate_max_accum(int annual_days, int special_accum);

/*
 * Print valid annual vacation options.
 * Shows all valid days and hours combinations.
 */
void print_valid_annual_options(void);

/*
 * Check if annual hours value is valid.
 * hours: annual hours to check
 * days_out: pointer to store the corresponding days if valid
 * Returns: 1 if valid, 0 otherwise
 */
int is_valid_annual_hours(int hours, int *days_out);

/*
 * Validate planned vacation days.
 * vacation_days: vacation days planned till end of year
 * remaining_working_days: number of remaining working days this year
 * Returns: 0 on success, non-zero on error
 */
int validate_vacation_days(double vacation_days, int remaining_working_days);

/*
 * Print calculation results.
 * input: calculation inputs
 * result: calculation results
 */
void print_results(const struct vacation_input *input,
		   const struct vacation_result *result);

/*
 * Print usage information.
 * program_name: name of the program (argv[0])
 */
void print_usage(const char *program_name);

/*
 * Parse command-line arguments.
 * argc: argument count
 * argv: argument values
 * args: structure to store parsed arguments
 * Returns: 0 on success, non-zero on error
 */
int parse_arguments(int argc, char *argv[], struct vacation_args *args);

/*
 * Prompt user for current accumulated hours.
 * hours: pointer to store the result
 * Returns: 0 on success, non-zero on error
 */
int prompt_current_hours(double *hours);

#endif /* VACATION_H */
