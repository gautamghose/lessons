#!/usr/bin/python

import sys

#Usage: <script-name> <filename-with-eventdates-foreach-ticker> <filename-with-market-eventdates>
#Example: identify_ticker_events.py 7pct_events 7pct_market_events
#TODO:gg
market_event_dates = {} #dictionary of <key=date, value=list-of-tickers>
nr_lines = 0
with open(sys.argv[2]) as f:
    for line in f:
        nr_lines += 1
        if 'Ticker:' in line:
            curr_ticker = line.split()[1]
            continue
        #All other lines should start with date, hence...
        if line[:2] == '20':
            event_date = line.strip().split()[0]
            if market_event_dates.get(event_date) is None:
                market_event_dates[event_date] = 0
            market_event_dates[event_date] += 1            

nr_lines = 0
curr_ticker = "None"
ticker_event_dates = [] #list of (ticker, [list of ticker event dates])
with open(sys.argv[1]) as f:
    for line in f:
        nr_lines += 1
        if 'Ticker:' in line:
            curr_ticker = line.split()[1]
            curr_tuple = (curr_ticker, [])
            ticker_event_dates.append(curr_tuple)
            continue
        #All other lines should start with date, hence...
        assert(line[:2] == '20')

        event_date = line.strip().split()[0]
        if market_event_dates.get(event_date) is None:
            curr_tuple[1].append(event_date)


nr_ticker_events = 0
nr_tickers = 0
for t in ticker_event_dates:
    print 'Ticker:', t[0], len(t[1])
    if len(t[1]) > 0:
        nr_tickers += 1
    for d in t[1]:
        print d
        nr_ticker_events += 1

print "Total number of tickers:", nr_tickers
print "Total number of ticker event dates:", nr_ticker_events
print "Total number of market event dates:", len(market_event_dates.keys())
sys.exit(0)

