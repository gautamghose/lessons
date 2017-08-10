#!/usr/bin/python

import sys

#Usage: <script-name> <filename-with-eventdates-foreach-ticker>
#Example: identify_market_events.py 7pct_events
event_dates = {} #dictionary of <key=date, value=list-of-tickers>
nr_lines = 0
with open(sys.argv[1]) as f:
    for line in f:
        nr_lines += 1
        if 'Ticker:' in line:
            curr_ticker = line.split()[1]
            continue
        #All other lines should start with date, hence...
        assert(line[:2] == '20')
        event_date = line.strip()
        if event_dates.get(event_date) is None:
            event_dates[event_date] = []
        event_dates[event_date].append(curr_ticker)

event_dates_sorted = []
for date,tickers in event_dates.items():
    event_dates_sorted.append( (date, len(tickers)) )

event_dates_sorted.sort(key=lambda x: x[1], reverse=True)

nr_ticker_specific_events = 0
nr_market_events = 0
for d in event_dates_sorted:
    if d[1] <= 2:
        assert(d[1] > 0)
        nr_ticker_specific_events += 1
    else:
        nr_market_events += 1
        print d[0], d[1]
print "Total number of event dates:", len(event_dates_sorted)
print "Total number of ticker event dates:", nr_ticker_specific_events
print "Total number of market event dates:", nr_market_events
sys.exit(0)

