#!/usr/bin/python

import urllib2
import pytz
import pandas as pd

from bs4 import BeautifulSoup
from datetime import datetime

import sys

SITE = "http://en.wikipedia.org/wiki/List_of_S%26P_500_companies"
START = datetime(1900, 1, 1, 0, 0, 0, 0, pytz.utc)
END = datetime.today().utcnow()


def scrape_list(site):
    hdr = {'User-Agent': 'Mozilla/5.0'}
    req = urllib2.Request(site, headers=hdr)
    page = urllib2.urlopen(req)
    soup = BeautifulSoup(page)

    table = soup.find('table', {'class': 'wikitable sortable'})
    sector_tickers = dict()
    for row in table.findAll('tr'):
        col = row.findAll('td')
        if len(col) > 0:
            sector = str(col[3].string.strip()).lower().replace(' ', '_')
            ticker = str(col[0].string.strip())
            if sector not in sector_tickers:
                sector_tickers[sector] = list()
            sector_tickers[sector].append(ticker)
    return sector_tickers

st = scrape_list(SITE)
cnt = 0
for sector, tickers in st.iteritems():
    print "Sector:",sector
    for t in tickers:
        print t
        cnt += 1

print 'Number of sectors:', len(st.keys()), 
print 'Number of tickers:', cnt

sys.exit(0)

