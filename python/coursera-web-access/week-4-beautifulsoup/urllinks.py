#!/usr/bin/python
# Note - this code must run in Python 2.x and you must download
# http://www.pythonlearn.com/code/BeautifulSoup.py
# Into the same folder as this program

import urllib
from BeautifulSoup import *

url = raw_input('Enter - ')
html = urllib.urlopen(url).read()
soup = BeautifulSoup(html)

follow_link_at = 18
repeat_cnt = 7

# Retrieve all of the anchor tags
repeat = 0
while repeat < repeat_cnt:
  tags = soup('a')
  cnt = 1
  for tag in tags:
    if cnt == follow_link_at:
      url = tag.get('href', None)
      print "Fetching", url
      html = urllib.urlopen(url).read()
      soup = BeautifulSoup(html)
      break
    cnt += 1
  repeat += 1
