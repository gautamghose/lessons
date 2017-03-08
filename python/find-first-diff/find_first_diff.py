#!/usr/bin/python

import sys

CHUNKSIZE = (1024*1024)

def fopen(fname, mode):
  f = None
  try:
    f = open(fname, mode)
  except IOError as e:
    print "Could not open {0}. Error ({1}) {2}.".format(fname, e.errno, e.strerror)
  finally:
    return f

if len(sys.argv) < 3:
  print "Usage:", sys.argv[0], "<file-1> <file-2>"
  sys.exit(1)

f1 = fopen(sys.argv[1], "rb")
f2 = fopen(sys.argv[2], "rb")
if f1 == None or f2 == None:
  sys.exit(1)

location = 0
diff_location = 0
eof1 = False
eof2 = False

try:
  while True:
    b1 = f1.read(CHUNKSIZE)
    if b1 == "":
      #print "EOF on", sys.argv[1]
      eof1 = True

    b2 = f2.read(CHUNKSIZE)
    if b2 == "":
      #print "EOF on", sys.argv[2]
      eof2 = True

    if eof1 and eof2:
      #print "No diff"
      break

    if eof1 or eof2:
      #print "First Diff at", location+1
      diff_location = location+1
      break

    if b1 != b2:
      upto = len(b1)
      if len(b1) != len(b2):
        if len(b1) < len(b2):
          upto = len(b1)
        else:
          upto = len(b2)

      i = 0
      while i < upto:
        if b1[i] != b2[i]:
          break
        i += 1
      #print "First diff at", location+i+1
      diff_location = location+i+1
      break
    location += CHUNKSIZE

except:
  print "Encountered a read failure."
  sys.exit(2)

finally:
  if diff_location != 0:
    print "First diff at", diff_location
    if eof1:
      print "EOF reached on", sys.argv[1]
    if eof2:
      print "EOF reached on", sys.argv[2]
    sys.exit(1)
  else:
    print "No diff"
    sys.exit(0)

