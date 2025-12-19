# Vacation Calculator - Design Document

## Overview

The Vacation Calculator is a tool that helps employees calculate their expected vacation days credit at the end of the year. It is available as both a command-line application (C) and a web interface (HTML/JavaScript).

## Architecture

```
┌───────────────────────────────────────────────────────────────────────────┐
│                            Vacation Calculator                            │
├───────────────────────────────────────────────────────────────────────────┤
│                                                                           │
│   ┌────────────────────────┐         ┌────────────────────────────────┐   │
│   │   Command Line Tool    │         │      Web Interface             │   │
│   │      (C Program)       │         │    (HTML/JavaScript)           │   │
│   ├────────────────────────┤         ├────────────────────────────────┤   │
│   │                        │         │                                │   │
│   │  ┌─────────────────┐   │         │  ┌─────────────────────────┐   │   │
│   │  │  Argument       │   │         │  │    Form Input           │   │   │
│   │  │  Parser         │   │         │  │    Handler              │   │   │
│   │  └────────┬────────┘   │         │  └───────────┬─────────────┘   │   │
│   │           │            │         │              │                 │   │
│   │  ┌────────▼────────┐   │         │  ┌───────────▼─────────────┐   │   │
│   │  │  Input          │   │         │  │    Input                │   │   │
│   │  │  Validation     │   │         │  │    Validation           │   │   │
│   │  └────────┬────────┘   │         │  └───────────┬─────────────┘   │   │
│   │           │            │         │              │                 │   │
│   │  ┌────────▼────────┐   │         │  ┌───────────▼─────────────┐   │   │
│   │  │  Calculation    │   │         │  │    Calculation          │   │   │
│   │  │  Engine         │   │         │  │    Engine               │   │   │
│   │  └────────┬────────┘   │         │  └───────────┬─────────────┘   │   │
│   │           │            │         │              │                 │   │
│   │  ┌────────▼────────┐   │         │  ┌───────────▼─────────────┐   │   │
│   │  │  Console        │   │         │  │    Results              │   │   │
│   │  │  Output         │   │         │  │    Display              │   │   │
│   │  └─────────────────┘   │         │  └─────────────────────────┘   │   │
│   │                        │         │                                │   │
│   └────────────────────────┘         └────────────────────────────────┘   │
│                                                                           │
└───────────────────────────────────────────────────────────────────────────┘
```

## Component Diagram

```
┌────────────────────────────────────────────────────────────────────────────┐
│                              Source Files                                  │
├────────────────────────────────────────────────────────────────────────────┤
│                                                                            │
│  ┌──────────────────┐    ┌──────────────────┐    ┌──────────────────────┐  │
│  │   vacation.h     │    │    main.c        │    │  test_vacation.c     │  │
│  │                  │    │                  │    │                      │  │
│  │  - Constants     │◄───┤  - main()        │    │  - Unit tests        │  │
│  │  - Structures    │    │  - Parsing       │    │  - 127 test cases    │  │
│  │  - Prototypes    │    │  - Validation    │    │  - All functions     │  │
│  │                  │    │  - Calculation   │    │    tested            │  │
│  └──────────────────┘    │  - Output        │    └──────────────────────┘  │
│                          └──────────────────┘                              │
│                                                                            │
│  ┌──────────────────────────────────────────────────────────────────────┐  │
│  │                    vacation_calculator.html                          │  │
│  │                                                                      │  │
│  │  - Self-contained HTML/CSS/JavaScript                                │  │
│  │  - No server required                                                │  │
│  │  - Mirrors C calculation logic                                       │  │
│  │  - Interactive form with validation                                  │  │
│  └──────────────────────────────────────────────────────────────────────┘  │
│                                                                            │
└────────────────────────────────────────────────────────────────────────────┘
```

## Data Flow Diagram

```
┌────────────────────────────────────────────────────────────────────────────┐
│   Inputs    │────►│  Validation  │────►│   Calculation   │────►│  Output   │
└────────────────────────────────────────────────────────────────────────────┘
      │                    │                      │                    │
      ▼                    ▼                      ▼                    ▼
┌────────────────────────────────────────────────────────────────────────────┐
│ Annual      │     │ Valid annual │     │ Monthly hours   │     │ Annual    │
│ allowance   │     │ days (15-24) │     │ calculation     │     │ vacation  │
├────────────────────────────────────────────────────────────────────────────┤
│ Current     │     │ Valid hours  │     │ Additional days │     │ Max       │
│ accumulation│     │ mapping      │     │ by year end     │     │ accum     │
├────────────────────────────────────────────────────────────────────────────┤
│ Extra       │     │ Non-negative │     │ Total expected  │     │ Total     │
│ vacation    │     │ values       │     │ accumulation    │     │ expected  │
├────────────────────────────────────────────────────────────────────────────┤
│ Special     │     │ Max working  │     │ Excess days     │     │ Excess    │
│ accumulation│     │ days limit   │     │ calculation     │     │ to deduct │
└────────────────────────────────────────────────────────────────────────────┘
```

## Key Data Structures

### vacation_input
```c
struct vacation_input {
    int annual_days;       // Annual vacation entitlement (15-24)
    int max_accum_days;    // Maximum allowed accumulation (36-48)
    double current_hours;  // Current accumulated hours
    int current_month;     // Current month (1-12)
    int current_year;      // Current year
    double vacation_extra; // Extra vacation days not in pay slip
    int week_start;        // Sunday (0) or Monday (1)
};
```

### vacation_result
```c
struct vacation_result {
    int remaining_months;       // Months remaining in year
    double monthly_hours;       // Hours earned per month
    double additional_hours;    // Hours to be earned by year end
    double additional_days;     // Days to be earned by year end
    double total_hours;         // Total expected hours at year end
    double total_days;          // Total expected days at year end
    double excess_days;         // Days exceeding max (to deduct)
    int working_days_remaining; // Working days left in year
};
```

## Calculation Algorithm

```
┌────────────────────────────────────────────────────────────────────────────┐
│                         Calculation Flow                                   │
├────────────────────────────────────────────────────────────────────────────┤
│                                                                            │
│  1. Calculate Monthly Hours                                                │
│     ┌─────────────────────────────────────────────────────────────────┐    │
│     │  monthly_hours = (annual_days × WORK_DAY_HOURS) / 12            │    │
│     └─────────────────────────────────────────────────────────────────┘    │
│                                      │                                     │
│                                      ▼                                     │
│  2. Calculate Remaining Months                                             │
│     ┌─────────────────────────────────────────────────────────────────┐    │
│     │  remaining_months = 12 - current_month + 1                      │    │
│     └─────────────────────────────────────────────────────────────────┘    │
│                                      │                                     │
│                                      ▼                                     │
│  3. Calculate Additional Vacation                                          │
│     ┌─────────────────────────────────────────────────────────────────┐    │
│     │  additional_hours = remaining_months × monthly_hours            │    │
│     │  additional_days = additional_hours / WORK_DAY_HOURS            │    │
│     └─────────────────────────────────────────────────────────────────┘    │
│                                      │                                     │
│                                      ▼                                     │
│  4. Calculate Total Accumulation                                           │
│     ┌─────────────────────────────────────────────────────────────────┐    │
│     │  total_hours = current_hours + additional_hours                 │    │
│     │              - (vacation_extra × WORK_DAY_HOURS)                │    │
│     │  total_days = total_hours / WORK_DAY_HOURS                      │    │
│     └─────────────────────────────────────────────────────────────────┘    │
│                                      │                                     │
│                                      ▼                                     │
│  5. Calculate Excess Days                                                  │
│     ┌─────────────────────────────────────────────────────────────────┐    │
│     │  if (total_days > max_accum_days)                               │    │
│     │      excess_days = total_days - max_accum_days                  │    │
│     │  else                                                           │    │
│     │      excess_days = 0                                            │    │
│     └─────────────────────────────────────────────────────────────────┘    │
│                                                                            │
└────────────────────────────────────────────────────────────────────────────┘
```

## Maximum Accumulation Logic

```
┌────────────────────────────────────────────────────────────────────────────┐
│                    Maximum Accumulation Calculation                        │
├────────────────────────────────────────────────────────────────────────────┤
│                                                                            │
│                        ┌───────────────────┐                               │
│                        │  special_accum?   │                               │
│                        └─────────┬─────────┘                               │
│                                  │                                         │
│                    ┌─────────────┴─────────────┐                           │
│                    │                           │                           │
│                    ▼                           ▼                           │
│            ┌───────────────┐           ┌───────────────┐                   │
│            │    No         │           │    Yes        │                   │
│            │               │           │               │                   │
│            │  max = 36     │           │ annual_days   │                   │
│            │               │           │   < 18?       │                   │
│            └───────────────┘           └───────┬───────┘                   │
│                                                │                           │
│                                    ┌───────────┴───────────┐               │
│                                    │                       │               │
│                                    ▼                       ▼               │
│                            ┌───────────────┐       ┌───────────────┐       │
│                            │    Yes        │       │    No         │       │
│                            │               │       │               │       │
│                            │  max = 36     │       │ max = annual  │       │
│                            │               │       │      × 2      │       │
│                            └───────────────┘       └───────────────┘       │
│                                                                            │
└────────────────────────────────────────────────────────────────────────────┘
```

## Valid Annual Vacation Options

| Days | Hours |
|------|-------|
| 15   | 126   |
| 16   | 135   |
| 17   | 143   |
| 18   | 152   |
| 19   | 159   |
| 20   | 168   |
| 21   | 177   |
| 22   | 185   |
| 23   | 194   |
| 24   | 202   |

## Build System

```
┌────────────────────────────────────────────────────────────────────────────┐
│                              CMake Build                                   │
├────────────────────────────────────────────────────────────────────────────┤
│                                                                            │
│  CMakeLists.txt                                                            │
│       │                                                                    │
│       ├──────────────────┬──────────────────────────────────────────┐      │
│       │                  │                                          │      │
│       ▼                  ▼                                          ▼      │
│  ┌──────────┐      ┌──────────────┐                          ┌──────────┐  │
│  │ vacation │      │ test_vacation│                          │  build/  │  │
│  │_calculator│     │              │                          │          │  │
│  │  (exe)   │      │    (exe)     │                          │ Output   │  │
│  └──────────┘      └──────────────┘                          └──────────┘  │
│       │                  │                                                 │
│       │                  │                                                 │
│       ▼                  ▼                                                 │
│  src/main.c         src/main.c                                             │
│                     src/test_vacation.c                                    │
│                                                                            │
└────────────────────────────────────────────────────────────────────────────┘
```

## Test Coverage

The test suite includes **127 test cases** covering:

- Integer and number parsing
- Leap year calculation
- Days in month calculation
- Day of week calculation
- Working days calculation
- Vacation days validation
- Annual days validation
- Maximum accumulation calculation
- Conversion functions (hours ↔ days)
- Remaining months calculation
- Monthly hours calculation
- Additional hours/days calculation
- Total hours/days calculation
- Excess days calculation
- Full vacation calculation scenarios
- Argument handling
- Time functions


