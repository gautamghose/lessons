#!/usr/bin/python
import re
import sys

'''
Coursera-python-web-access: Week 2 Assignment
Find all the numbers sprinkled out across a file using regex.
Then, find the sum of all such numbers.
'''
FNAME = ""
if len(sys.argv) < 2:
    print "Please provide filename to look into."
    sys.exit(1)
FNAME = sys.argv[1]
f = open(FNAME,'r')
f_str = f.read()
all_numbers_str = re.findall('([0-9]+)', f_str)
all_numbers_int = [int(x) for x in all_numbers_str]
print "Len", len(all_numbers_int), "Sum", sum(all_numbers_int)
