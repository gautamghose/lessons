#!/usr/bin/python

import subprocess
import sys
import os.path


if len(sys.argv) < 2:
    print "Usage:",sys.argv[0], "filename-that-has-all-integers"
    sys.exit(1)

FILENAME = sys.argv[1]

values = [] # list of all the integers
f = open(FILENAME)
for line in iter(f):
    values.append(int(line.strip()))
f.close()

i = 0
j = 0
count = 0
while i < len(values):
    pivot = values[i]
    j = i+1
    while j < len(values):
        if values[j] < pivot:
            count += 1
        j += 1
    i += 1
    
print "Number of integers", len(values)
print "Number of inversions", count
sys.exit(0)


