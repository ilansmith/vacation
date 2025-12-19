# Vacation Days Calculator

A cross-platform C program that calculates the expected vacation days credit an employee will have at the end of the year.

The calculation is based on your **Annual Allowance** and **Current Accumulation** as they appear in your latest pay slip.

It also depends on your maximum allowed accumulation and should take into account any extra vacation consumed or planned which is not reflected in your latest pay slip.

Such vacation consists of:
- Any vacation days taken during the previous month
- Any vacation days taken during the current month up to and including today
- Any planned vacation days expected to be taken from now on until the end of the year

For accurate calculation, sum up these vacation days that do not appear in your pay slip and provide the sum with the `-v` / `--vacation-extra` command line option.

In some cases, you may be entitled to accumulate more than the standard 36 days by the end of the year. Check [here](https://www.kolzchut.org.il/he/%D7%A6%D7%91%D7%99%D7%A8%D7%AA_%D7%99%D7%9E%D7%99_%D7%97%D7%95%D7%A4%D7%A9%D7%94_%D7%9C%D7%A2%D7%95%D7%91%D7%93%D7%99%D7%9D_%D7%A9%D7%9C%D7%90_%D7%99%D7%9B%D7%9C%D7%95_%D7%9C%D7%A0%D7%A6%D7%9C_%D7%90%D7%AA_%D7%94%D7%99%D7%9E%D7%99%D7%9D_%D7%91%D7%92%D7%9C%D7%9C_%D7%9E%D7%9C%D7%97%D7%9E%D7%AA_%D7%97%D7%A8%D7%91%D7%95%D7%AA_%D7%91%D7%A8%D7%96%D7%9C_%D7%90%D7%95_%D7%94%D7%9E%D7%9C%D7%97%D7%9E%D7%94_%D7%9E%D7%95%D7%9C_%D7%90%D7%99%D7%A8%D7%90%D7%9F_(%D7%9E%D7%9C%D7%97%D7%9E%D7%AA_%D7%94%D7%AA%D7%A7%D7%95%D7%9E%D7%94)) to see if you meet the criteria for extra accumulation days. If you meet the criteria, indicate this with the `-s` / `--special-accum` command line option.

## Web Interface

Open `vacation_calculator.html` in any web browser for a graphical interface that includes:
- Interactive form for all input parameters
- Real-time calculation without server requirements
- Usage documentation and examples

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
| `-d` | `--annual-days <days>` | Annual vacation days (integer) |
| `-A` | `--annual-hours <hours>` | Annual vacation hours (integer, alternative to -d) |
| `-s` | `--special-accum` | Use special max accumulated days calculation |
| `-c` | `--current-hours <hours>` | Current accumulated vacation hours |
| `-v` | `--vacation-extra <days>` | Extra vacation days consumption |
| `-M` | `--monday-start` | Use Monday as first working day of week (default: Sunday) |
| `-h` | `--help` | Display help message |

**Valid annual vacation options:**
| Days | Hours |
|------|-------|
| 15 | 126 |
| 16 | 135 |
| 17 | 143 |
| 18 | 152 |
| 19 | 159 |
| 20 | 168 |
| 21 | 177 |
| 22 | 185 |
| 23 | 194 |
| 24 | 202 |

**Notes:**
- Either `-d`/`--annual-days` or `-A`/`--annual-hours` must be specified.
- Extra vacation days cannot exceed working days from the start of the previous month.

### Examples

Basic usage with annual days and current hours provided:
```bash
./vacation_calculator -d 21 -c 150.5
```

Specifying annual days with special case for max accumulation:
```bash
./vacation_calculator -d 20 -s -c 200
```

Using annual hours instead of days:
```bash
./vacation_calculator -A 202 -c 150
```

With extra vacation days consumption:
```bash
./vacation_calculator -d 24 -c 200 -v 5
```

With fractional vacation and special accumulation criteria met:
```bash
./vacation_calculator -d 24 -c 200 -v 2.5 -s
```

Interactive mode (will prompt for current hours):
```bash
./vacation_calculator -d 24
```

### Sample Output

```
=== Vacation Days Calculator ===

Annual vacation: 177 hours (21 days)
Maximum accumulation: 302.4 hours (36 days)
Current accumulated hours: 150.5

Current month: December (12)
Remaining working days this year: 23

Additional vacation expected by end of year: 1.75 days (14.7 hours)
Total accumulated vacation expected: 165.2 hours (19.67 days)
Vacation to be deducted (exceeding max): 0 hours (0 days)
```

## Constants

- **WORK_DAY_HOURS**: 8.4 hours per working day
- **MAX_ACCUM_DAYS** (normal): 36 days
- **MAX_ACCUM_DAYS** (special accumulation, 18-24 days): annual-days × 2

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
- **Leap year calculation**: divisibility rules for leap years
- **Days in month**: all months, leap year February, invalid months
- **Day of week**: Zeller's congruence for various known dates
- **Working days calculation**: Sunday and Monday week starts, various months
- **Vacation days validation**: max working days limits, fractional values
- **Conversion functions**: hours_to_days, days_to_hours, roundtrip conversions
- **Remaining months calculation**: valid months, boundary cases, invalid inputs
- **Monthly hours calculation**: default values, zero, custom values
- **Additional hours calculation**: full year, partial year, edge cases
- **Total hours calculation**: various input combinations
- **Rounding total days**: exact, round down/up, half rounds up
- **Excess days calculation**: under/at/over limit
- **Integration tests**: complete vacation calculations with vacation days consumed
- **Argument handling**: initialization, validation, mutual exclusivity


