#!/usr/bin/python

import sys

#Usage: <script-name> <filename-with-all-identified-ticker-events> <directory-of-raw-csvfiles> <number-of-prior-days>
#Example: generate_feature_data.py ../events/7pct_ticker_events  ../raw_data 15
if len(sys.argv) < 4:
    print "Usage: <script-name> <filename-with-all-identified-ticker-events> <directory-of-raw-csvfiles> <number-of-prior-days>"
    sys.exit(1)

def print_one_feature(ticker, date, nr_prev_days, lines_reversed):
    '''
    ticker: a string, ticker symbol
    date: a string, the date on which the event happened
    nr_prev_days: how many previous days you want reported
    lines_reversed: all the events in a list of strings (lines in the csv file)
    '''
    #quickly scan and go up to the event day
    line_idx = -1
    nr_lines = 0
    for line in lines_reversed:
        dt = line.split()[0] 
        if dt[:2] == '20' and dt == date:
            line_idx = nr_lines
            break
        nr_lines += 1

    assert(line_idx != -1)
    lidx = 0
    pct_moves = [0.0 for x in range(nr_prev_days)]
    while lidx < nr_prev_days and (line_idx+lidx+1) < len(lines_reversed):
        closing_price_today = float(lines_reversed[line_idx+lidx].split()[4])
        closing_price_yeday = float(lines_reversed[line_idx+lidx+1].split()[4])
        pct_moves[lidx] = 100.0*(closing_price_today-closing_price_yeday)/closing_price_yeday
        lidx += 1

    pct_moves.reverse()
    #emit one event now
    print ticker, date,
    for m in pct_moves:
        print "{0:.2f}".format(m),
    print
    return
    
def print_features(ticker, dates, N, csv_file):
    '''
    ticker: a string, ticker symbol
    dates: a list of dates (a date is a string)
    N: how many previous days you want reported
    csv_file: Full csv pathname for the raw data
    This heavy-duty routine consults the csv file passed in which has the raw market data.
    For each of the date specified in the list, this routine prints the preceding 'N' days
    movement of this ticker.
    '''
    f = open(csv_file,'r')
    lines = f.readlines()
    f.close()
    lines = lines[2:] #get rid of the first two lines in each csv file
    lines.reverse()
    for d in dates:
        print_one_feature(ticker, d, lines_reversed=lines, nr_prev_days=N)
    return

ticker_event_dates = [] #list of tuples ("ticker-symbol", [list-of-dates])
nr_lines = 0
nr_tickers = 0
with open(sys.argv[1]) as f:
    for line in f:
        nr_lines += 1
        if 'Ticker:' in line:
            curr_ticker = line.split()[1]
            #TODO:gg put it into dictionary ticker_event_dates
            ticker_event_dates.append( (curr_ticker, []) )
            nr_tickers += 1
            continue
        #All other lines should start with date, hence...
        if line[:2] == '20':
            event_date = line.strip().split()[0]
            ticker_event_dates[nr_tickers-1][1].append(event_date)

nr_lines = 0
curr_ticker = "None"
for t in ticker_event_dates:
    print_features(t[0], t[1], int(sys.argv[3]), sys.argv[2]+"/"+t[0]+".csv")

sys.exit(0)

