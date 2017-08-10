#!/usr/bin/python

import sys

def abso_pct(diff, base):
    '''diff: float
    '''
    d = abs(diff)
    return d*100.0/base
    
def count_events(csv_fname, percent_move):
    nr_events = 0
    prev_close = 0.0
    nr_lines = 0
    event_dates = []
    with open(csv_fname, 'r') as f:
        for line in f:
            nr_lines += 1
            if line[:2] == '20':
                try:
                    new_close = float(line.split()[4])
                    if prev_close != 0.0:
                        if abso_pct(new_close - prev_close, prev_close) >= percent_move:
                            nr_events += 1
                            event_dates.append( line.split()[0] )
                except:
                    print "Error encountered at line", nr_lines, csv_fname
                    print "Offending line:", line
                    break
                else:
                    prev_close = new_close
    return (csv_fname.split('.')[0], nr_events, event_dates)


#Usage: <script-name> <percent-move> <list of TICKER.csv filenames>
#Example: count_events.py 7.0 A*.csv 
total_cnt = 0
ticker_events = [] #list of tuples
PCT_MOVE = float(sys.argv[1])
idx = 2
while idx < len(sys.argv):
    t = count_events(sys.argv[idx], PCT_MOVE)
    ticker_events.append(t)
    total_cnt += t[1]
    idx += 1
    

sorted_ticker_events = ticker_events
sorted_ticker_events.sort(key=lambda x: x[1], reverse=True)

print "{0} {1}".format('ticker'.rjust(6), 'nr_events')
for t in sorted_ticker_events:
    print "{0} {1}".format(t[0].rjust(6), t[1])
print 'Total Events:', total_cnt

print "EVENTS=====>"
for t in sorted_ticker_events:
    if t[1] < 1:
        break
    print "Ticker:", t[0]
    for date in t[2]:
        print date
sys.exit(0)

