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

nr_comparisons = 0

def swap(A,i,j):
    tmp = A[i]
    A[i] = A[j]
    A[j] = tmp
    return

def partition_(A,l,r):
    '''
    A : is the list of integers
    l : left index within the full list to identify the starting point of this sub-array
    r : right index within the full list to identify the ending point of this sub-array
    returns the index where the pivot element moves to.
    '''
    global nr_comparisons
    assert r > l
    nr_comparisons += (r-l)
    p = A[l]
    i = l+1
    j = l+1
    while j <= r:
        if A[j] < p:
            swap(A,i,j)
            i += 1
        j += 1

    swap(A,i-1,l)
    assert l <= (i-1) and r >= (i-1)
    return (i-1)

def maximum(a,b):
    if a > b:
        return a
    else:
        return b
def minimum(a,b):
    if (a < b):
        return a
    else:
        return b

def find_pivot_left(A,l,r,m):
    return l

def find_pivot_right(A,l,r,m):
    return r

def find_pivot_median(A,l,r,m):
    maxm = maximum(maximum(A[l],A[m]),A[r])
    minm = minimum(minimum(A[l],A[m]),A[r])
    if A[l] != maxm and A[l] != minm:
        return l
    if A[r] != maxm and A[r] != minm:
        return r
    if A[m] != maxm and A[m] != minm:
        return m
    assert l == m or r == m
    assert (l-r) <= 2
    return l
    
def qsort(A,l,r):
    if l >= r:
        return
    pivot_idx = find_pivot_median(A,l,r,(l+r)/2)
    swap(A,l,pivot_idx)
    parted_at = partition_(A,l,r)
    qsort(A,l,parted_at-1)
    qsort(A,parted_at+1,r)


qsort(values,0,len(values)-1)
#print values
print values[0:10]
print values[100:110]
print values[1000:1010]
print values[9000:9010]

#print values
print "nr_comparisons", nr_comparisons
sys.exit(0)


