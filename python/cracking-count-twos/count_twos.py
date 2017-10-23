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
  return count + count_2s(N % tenexp)
  
def count_2s_bruteforce(N):
  count = 0
  i = 1
  while i <= N:
    istr = str(i)
    idx = 0
    while idx < len(istr):
      if istr[idx] == '2':
        count += 1
      idx += 1
    i += 1
  return count

if len(sys.argv) < 2:
  print "Usage:", sys.argv[0], "<any-positive-integer>"
  sys.exit(1)

val = int(sys.argv[1])
if val < 0:
  print val, "is not a positive number."
  sys.exit(1)

print "Answer:", count_2s(val)
sys.exit(0)

i = 1
while i <= val:
  v1 = count_2s(i)
  v2 = count_2s_bruteforce(i)
  assert(v1 == v2), "v1: {0} v2: {1} did not match for i= {2}".format(v1,v2,i)
  i += 1
  if i % 5000 == 0:
    print "Processed", i, "numbers till now."

print "Number of twos:", count_2s(val)
print "Number of twos(brute force):", count_2s_bruteforce(val)
sys.exit(0)

