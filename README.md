# Vacation Days Calculator

A cross-platform C program that calculates the expected vacation days credit an employee will have at the end of the year.

## Features

- Calculates additional vacation days expected by year end
- Calculates total accumulated vacation days
- Determines excess days to be deducted (exceeding maximum allowed)
- Supports both command-line arguments and interactive input
- Cross-platform: builds and runs on Linux and Windows

## Building

### Prerequisites

- CMake 3.10 or higher
- C compiler (GCC, Clang, or MSVC)

### Linux

```bash
mkdir build
cd build
cmake ..
make
```

### Windows (with Visual Studio)

```cmd
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

### Windows (with MinGW)

```cmd
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
mingw32-make
```

## Usage

```
vacation_calculator [OPTIONS]
```

### Options

| Short | Long | Description |
|-------|------|-------------|
| `-d` | `--annual-days <days>` | Annual vacation days (integer, default: 24) |
| `-A` | `--annual-hours <hours>` | Annual vacation hours (integer, alternative to -d) |
| `-m` | `--max-accum <days>` | Maximum accumulated days (integer, default: 36) |
| `-c` | `--current-hours <hours>` | Current accumulated vacation hours |
| `-v` | `--vacation-days <days>` | Vacation days to consume this month (X or X.5) |
| `-M` | `--monday-start` | Use Monday as first working day of week (default: Sunday) |
| `-h` | `--help` | Display help message |

**Notes:**
- `-d`/`--annual-days` and `-A`/`--annual-hours` are mutually exclusive.
- Vacation days (`-v`) must be in half-day increments (0.5, 1, 1.5, 2, etc.)
- Vacation days cannot exceed the number of working days in the current month.

### Examples

Basic usage with current hours provided:
```bash
./vacation_calculator -c 150.5
```

Specifying annual days and max accumulation:
```bash
./vacation_calculator -d 26 -m 40 -c 200
```

Using annual hours instead of days:
```bash
./vacation_calculator -A 202 -c 150
```

With vacation days to consume this month:
```bash
./vacation_calculator -c 200 -v 5
```

With half-day vacation and Monday as week start:
```bash
./vacation_calculator -c 200 -v 2.5 -M
```

Interactive mode (will prompt for current hours):
```bash
./vacation_calculator
```

### Sample Output

```
=== Vacation Days Calculator ===
Annual vacation days: 24
Maximum accumulated days: 36
Current accumulated hours: 150.50

Current month: December (12)
Working days in current month: 23
Vacation days to consume this month: 5
Remaining months (including current): 1

Additional vacation expected by year end: 2 days
Total accumulated vacation expected: 15 days
Vacation to be deducted (exceeding max): 0 days
```

## Constants

- **WORK_DAY_HOURS**: 8.416666666 hours per working day
- **Default ANNUAL_DAYS**: 24 days
- **Default MAX_ACCUM_DAYS**: 36 days

## Calculation Logic

1. Remaining months = 12 - current_month + 1 (includes current month)
2. Monthly hours credit = ANNUAL_HOURS / 12
3. Additional hours = remaining_months × monthly_hours
4. Total hours = current_hours + additional_hours
5. Excess days = max(0, total_days - MAX_ACCUM_DAYS)

## Running Tests

After building, run the unit tests:

```bash
cd build
./test_vacation
```

Or use CTest:

```bash
cd build
ctest
```

### Test Coverage

The test suite includes 116 tests covering:

- **Integer parsing**: positive, negative, zero, whitespace, rejection of floats/text
- **Number parsing**: integers, floats, whitespace, rejection of invalid input
- **Half-day resolution**: valid values (0, 0.5, 1, 1.5, etc.), rejection of invalid fractions
- **Leap year calculation**: divisibility rules for leap years
- **Days in month**: all months, leap year February, invalid months
- **Day of week**: Zeller's congruence for various known dates
- **Working days calculation**: Sunday and Monday week starts, various months
- **Vacation days validation**: half-day resolution, max working days limits
- **Conversion functions**: hours_to_days, days_to_hours, roundtrip conversions
- **Remaining months calculation**: valid months, boundary cases, invalid inputs
- **Monthly hours calculation**: default values, zero, custom values
- **Additional hours calculation**: full year, partial year, edge cases
- **Total hours calculation**: various input combinations
- **Rounding total days**: exact, round down/up, half rounds up
- **Excess days calculation**: under/at/over limit
- **Integration tests**: complete vacation calculations with vacation days consumed
- **Argument handling**: initialization, validation, mutual exclusivity


