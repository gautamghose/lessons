#!/usr/bin/python

import sys

def ten_raised_to(exp):
  i=1
  curr=1
  while i <= exp:
    curr *= 10
    i += 1
  return curr

def F(i):
  '''if i=3, return number of 2's between 000-999
     if i=5, return number of 2's between 00000-99999
  '''
  if i == 0:
    return 0
  nr_of_numbers = i*ten_raised_to(i)
  return nr_of_numbers/10

def count_2s(N):
  Nstr = str(N)
  nr_digits = len(Nstr)
  i = nr_digits-1
  d = int(Nstr[0])

  count = d * F(i)
  tenexp = ten_raised_to(i)
  if d > 2:
    count += tenexp
  elif d == 2:
    count += (N % tenexp) + 1

  if i == 0:
    return count
  print "count", count, "+", "count_2s(", N%tenexp, ")"
  return count + count_2s(N % tenexp)
  
if len(sys.argv) < 2:
  print "Usage:", sys.argv[0], "<any-positive-integer>"
  sys.exit(1)

val = int(sys.argv[1])
if val < 0:
  print val, "is not a positive number."
  sys.exit(1)

print "Number of twos:", count_2s(val)
sys.exit(0)

valstr = str(val)
nr_digits = len(valstr)
i = 0
sum=0
while i < nr_digits:
  d = int(valstr[nr_digits-1-i])
  sum = (d+1)*sum
  if d >= 2:
    sum += ten_raised_to(i)
  i += 1

print "Number of twos:", sum
sys.exit(0)

