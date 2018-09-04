#!/usr/bin/python

import sys
import time
import calendar

def convert_epochseconds_to_MMDDYYYY_str(seconds_since_epoch):
  """utility routine to convert the seconds since_epoch string into a MM/DD/YYYY string
  This routine converts a string representing seconds-since-epoch into the MM/DD/YYYY format.
  Args:
    seconds_since_epoch: string, represents number of seconds since epoch
  Returns:
    a string in "MM/DD/YYYY GMT" format.
  """
  return time.strftime('%m/%d/%Y', time.gmtime(int(seconds_since_epoch)))

def convert_MMDDYYYY_str_to_epochseconds(day_repr):
  """utility routine to convert the MM/DD/YYYY string into seconds since_epoch
  Args:
    day_repr: string, represents the date in MM/DD/YYYY format
  Returns:
    an integer representing the seconds since epoch for the start of the day
  """
  return calendar.timegm(time.strptime(day_repr, '%m/%d/%Y'))

class DailyData(object):
  """All the url hits for a single day are stored and managed in one instance of this class.

  This class is essentially a dictionary, such as, 
  {'www.facebook.com':45, 'www.news.com':12}, where the urls are actually converted to integer id's.
  So, the dictionary will be:
  {2:45, 3:12}, where 2 is the id for www.facebook.com and 3 is the id for www.news.com.
  with number of url hits stored against the urls.
  Attributes:
    data_: the underlying dictionary
    day_: the string representation of the day this DailyData object is holding
    day_start_: the integer value (seconds since epoch) for the start of the day. Will be used to sort the DailyData instances
  """

  def __init__(self, day):
    """Constructor takes the MM/DD/YYYY string

    Args:
      day: string, the MM/DD/YYYY string
    """
    self.data_ = {}
    self.day_ = day
    self.day_start_ = convert_MMDDYYYY_str_to_epochseconds(day)
    return

  def add_hit(self, urlid):
    """Increment the counter for the hit url.
    Args:
      urlid: int, the url identifier for 'www.facebook.com' type of a string
    Returns:
      nothing
    """
    if urlid not in self.data_:
      self.data_[urlid] = 0
    self.data_[urlid] += 1
    return

  def get_start_of_day(self):
    """Returns the epoch seconds for the start of this day."""
    return self.day_start_

  def __str__(self):
    """Standard stringifier operator.
    """
    retval = self.day_ + '\n'
    for url,hits in self.data_.items():
      retval += url + " " + str(hits) + "\n"
    return retval

  def printout_sorted(self, unique_urls):
    """Dump the content in sorted order
    Args:
      unique_urls: list, the vector of the urls to map the url id's to the corresponding url strings 
    """
    print ("{0} GMT".format(self.day_)) #GMT is appended because everything was processed w.r.t UTC
    counts = []
    for urlid,hits in self.data_.items():
      counts.append((urlid,hits))
    #now sort the url hits in descending order, by the hits, i.e. the second value in tuple
    counts.sort(key=lambda x: x[1], reverse=True) #sort by the second elemnt in the tuple

    for urlhits in counts:
      print ("{0} {1}".format(unique_urls[urlhits[0]], urlhits[1]))
    return

def main():
  if len(sys.argv) < 2:
    print ("Usage: {0} filename-with-url-hits".format(sys.argv[0]))
    return 1

  url_dictionary = {} #global database of all unique urls and there assigned id's, e.g. {'www.facebook.com':2, 'www.news.com':0, 'www.google.com':1, ...}
  unique_urls = [] #global database of all unique urls in a list for the reverse lookup ['www.news.com', 'www.google.com', 'www.facebook.com', ...]
  all_data = {} #this will essentially be a dictionary like {'08/28/2018': {1:45, 2:12}, '08/28/2018': {1:14}, ...}. The value is a DailyData instance.

  with open(sys.argv[1]) as f:
    line_cnt = 1
    for line in f:
      tup = line.strip().split('|')
      if (len(tup) < 2):
        print >> sys.stderr, "Unformatted line at line number: {0} [{1}]".format(line_cnt, line.strip())
        continue
      day = convert_epochseconds_to_MMDDYYYY_str(tup[0])
      url = tup[1]
      if day not in all_data:
        all_data[day] = DailyData(day)
       
      if url not in url_dictionary:
        url_id = len(unique_urls)
        unique_urls.append(url)
        url_dictionary[url] = url_id
      else:
        url_id = url_dictionary[url]

      all_data[day].add_hit(url_id) 
      line_cnt += 1

  #File has been read fully, and all_data dictionary is populated fully.
  #At this point, the url_dictionary and unique_urls list should agree on the length.
  assert len(url_dictionary) == len(unique_urls)

  sorted_dates = []
  for key,daily_data in all_data.items():
    sorted_dates.append((key, daily_data.get_start_of_day()))

  sorted_dates.sort(key=lambda x: x[1]) #sort by the second elemnt in the tuple
  for tup in sorted_dates:
    all_data[tup[0]].printout_sorted(unique_urls)
  return 0


if __name__ == "__main__":
  rc = main()
  sys.exit(rc)

