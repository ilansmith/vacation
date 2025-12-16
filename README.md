Excess vacation days calculator
===============================

Build Options
-------------
#### To build the _vacation_days_ app, issue:
`make`

#### To build the _vacation_days_ app with debug flags, issue:
`make DEBUG=y`

#### To build the _vacation_test_ unit test app, issue:
`make TEST=y`

Usage
-----
```
vacation_days [h] [-a annual-leave] [-m max-accum] <current-vacation-due>

Where:
  --annual/-a:
      amount of annual leave in days (default: 24)
  --max/-m:
      maximum amount of allowed accumulated leave in days (default: 36)
      this value cannot be greater than the total amount of annual leave
  --help/-h:
      print this message and exit

  <current-vacation-due>
      the amount of vacation currently due in hours as it is stated in your latest pay slip
      note that 1 working day is 8.416 hours
```

Examples
--------

#### Example 1

- Assume the current month is October, and the the amount of vacation due to
  you according to the last payslip (September) is 270.2 hours. This evaluates
  to 32 days.
- Assume your annual leave is the default (i.e. 24 days leave per year).
  In this case, each month 2 days leave are added. Put differently, 1 day of
  leave is added every half month.
- Assume the maximum number of days you may accumulate per year is the default
  (i.e. a total of 36 days).
- Assume you are running the app during the first half of October (i.e.
  sometime between October 1 and 15).

```
./vacation_days 270.2
annual = 24, max = 36, hours = 270.20
Current vacation days due: 32 (accumulated 14, current year 18)
Expected additional days by end of year: 6
Expected total due at end of year: 38
Expected excess days at end of year: 2
```
##### Explantion
- Since the begning of the year, 18 leave days have been awarded to you. 14
  days have accumulated from the previous year. In total, you are currently
  due 32 days leave.
- Until the end of the year, you are expected to accumulated another 6 days (2
  days for each of the months - October, November and December).
- You are thus expected to have accumulated a total of 38 days by the end of
  the year, resulting in an excess of 2 days which will be removed.

**Make sure to take at least 2 days vacation by the end of the year!**

#### Example 2

- Same case as Example 1, except now the app is run during the second half of
  October (i.e. sometime between October 16 and 31).
```
./vacation_days 270.2
annual = 24, max = 36, hours = 270.20
Current vacation days due: 33 (accumulated 14, current year 19)
Expected additional days by end of year: 5
Expected total due at end of year: 38
Expected excess days at end of year: 2
```
##### Explantion
- You are now in the second half of October, hence 1 more day vacation is due
  to you compaired to the previous example. Hence, 19 leave days have been
  awarded to you since the begining of the year and you still have the 14 days
  accumulated from the previous year. In total, you are currently due 33 days
  of leave.
- Until the end of the year, you are expected to accumulated another 5 days (1
  day for the second half of October, and 2 days for each of November and
  December).
- You are thus expected to have accumulated a total of 38 days by the end of
  the year, resulting in an excess of 2 days which will be removed.

**Make sure to take at least 2 days vacation by the end of the year!**

