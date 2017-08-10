#!/usr/bin/python

import pandas_datareader.data as web
import datetime
import sys
import time

g_start = datetime.datetime(2013, 5, 18)
g_end = datetime.datetime(2017, 5, 18)

def download_historical(ticker):
    fileobj = open(ticker + '.csv','w')
    t = web.DataReader(ticker, 'google', g_start, g_end)
    #does not work: t = web.DataReader(ticker, 'yahoo', g_start, g_end)
    fileobj.write(t.to_string())
    fileobj.write('\n')
    fileobj.close() 

cnt = 0
snpfile = open('tickers.lst','r')
for symbol in snpfile.readlines():
    if 'Sector:' in symbol:
        continue
    if symbol.strip() == "":
        continue
    #print 'downloading', symbol.strip()
    download_historical(symbol.strip())
    time.sleep(1)
    cnt += 1
snpfile.close()

print 'Downloaded info for', cnt, 'tickers.'
sys.exit(0)

