#!/usr/bin/python

import subprocess
import sys
import os.path
import struct

import pandas as pd
#import matplotlib.pyplot as plt
import pylab
import random

'''
This program reads a binary file and looks at the 4-byte values (atoms) in the file.
It uses the python 'struct' to do the reading and parsing.
A sample of all such atoms are collected (SAMPLE_SIZE controls the size).
A frequency distribution of all the atoms are charted and presented using pandas and pylab
'''

SAMPLE_SIZE = 5.0 #percent

if len(sys.argv) < 2:
    print "Usage:",sys.argv[0], "filename"
    sys.exit(1)

FILENAME = sys.argv[1]

values = {} # dictionary of <value: frequency>
f = open(FILENAME, "rb")
fsize = os.stat(FILENAME).st_size
nr_atoms = fsize/4 # 4 bytes per atom
nr_samples = int(nr_atoms * SAMPLE_SIZE / 100.0)
b = f.read(4)
nr_values = 0
cnt = 0
random.seed()
while b and cnt < nr_samples:
    nr_values += 1
    val = struct.unpack('>I',b)[0]
    if values.has_key(val):
        values[val] += 1
    else:
        values[val] = 1
    next_atom_idx = random.randint(1,nr_atoms-1)
    f.seek(4*next_atom_idx)
    b = f.read(4)
    cnt += 1

'''
#print values
print "nr_values", nr_values
cnt=0
values_arr = [] # will be a sorted array of tuples
for t in values.items():
    values_arr.append(t)

values_arr.sort(key=lambda x: x[1], reverse=True)

for t in values_arr:
    print t
    cnt += 1
    if cnt > 10:
        break
'''
# assign the dictionary
myDict = values #{65: 3, 90: 2, 75: 1, 45: 1, 77: 1, 21: 1, 23: 1, 88: 1}
# convert dictionary to a dataframe
myDataframe=pd.DataFrame(myDict.items(), columns=['Atoms', 'Frequency'])
mydf = myDataframe.sort('Frequency', ascending=False)
mydf['Index']=range(len(myDataframe))
print mydf.head(n=50)
print mydf.shape

# plot the dataframe by descending order of frequency
#my_plot = myDataframe.sort('Frequency',ascending=False).plot(x='Atoms',y='Frequency',kind='scatter',legend=None,title="Frequency Distribution of Atoms")
my_plot = mydf.head(n=50).plot(x='Index', y='Frequency',kind='bar',legend=None,title="Frequency Distribution of Atoms")
# to view the plot
pylab.show()

sys.exit(0)


