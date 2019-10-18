hh
==

**hh** is a bash script which calculates the Hanke-Henry Permanent Calendar
(HHPC) date from the current (or supplied) date.

The HHPC is one of many examples of leap week calendars, calendars which
maintain synchronization with the solar year by intercalating entire weeks
rather than single days. By reducing common years to 364 days (52 weeks),
and adding an extra week every five or six years, it eliminates weekday
drift and synchronizes the calendar year with the seasonal change as the
Earth circles the Sun. As part of the calendar proposal, time zones would be
eliminated and replaced with UTC.

## Installation
- Copy script to local `bin` directory path.
- Copy man file to your local `share/man` or manpath.
- Copy **getusage** to local `bin` directory or path.

## History
As an aficionado of horology, I went looking for a tool which would easily
calculate the HHPC date. While several examples of code could be found to
enable such calculations through websites (e.g. Javascript), I needed
something which could display the HHPC date on the commandline.

Finding none, I created my own.

## Author
Paul J Henry <cpjhenry@gmail.com>  
12 October 2019
