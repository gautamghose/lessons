#!/usr/bin/python

import subprocess
import sys
import os.path
import random


def run_system_command(cmd):
    rc = subprocess.call(cmd, shell=True)
    return rc

cmd_to_run = ""
time_period_in_sec = 0
nr_iterations = 1234567890 #just a very big number

if len(sys.argv) < 3:
    print "Usage:",sys.argv[0], "<cmd-to-run-periodically> <time-period-in-seconds> [<number-of-iterations>]" 
    sys.exit(1)

cmd_to_run = sys.argv[1]
time_period_in_sec = int(sys.argv[2])
if len(sys.argv) > 3:
    nr_iterations = int(sys.argv[3])

while nr_iterations > 0:
    run_system_command (cmd_to_run)
    run_system_command ("sleep " + str(time_period_in_sec))
    nr_iterations -= 1


sys.exit(0)


