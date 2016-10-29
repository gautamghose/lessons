#!/usr/bin/python
# Note - this code must run in Python 2.x and you must download
# http://www.pythonlearn.com/code/BeautifulSoup.py
# Into the same folder as this program

import urllib
from BeautifulSoup import *

url = raw_input('Enter - ')
html = urllib.urlopen(url).read()

soup = BeautifulSoup(html)

# Retrieve all of the span tags
spans = soup('span')
total = 0
for span in spans:
    # Look at the parts of a tag
    #print 'TAG:',span
    #print 'Contents:',span.contents[0]
    total += int(span.contents[0])
    #print 'Attrs:',span.attrs

print "Sum", total
