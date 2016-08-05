#!/usr/bin/python

import subprocess
import sys
import os.path


EMAIL_SUBJECT = "\"No specific subject specified\""
EMAIL_BODY = "\"No specific email-body specified\""
EMAIL_RECIPIENT = "\"gautam.ascava@gmail.com\""
USER_NAME = os.getlogin()
TMPFILE = "/tmp/gg.mailer"

if len(sys.argv) < 4:
    print "Usage:",sys.argv[0], "<subject> <body> <recipient>"
    print "Example:",sys.argv[0], EMAIL_SUBJECT, EMAIL_BODY, EMAIL_RECIPIENT
    sys.exit(1)

EMAIL_SUBJECT = sys.argv[1]
EMAIL_BODY = sys.argv[2]
EMAIL_RECIPIENT = sys.argv[3]

subprocess.call("rm -rf " + TMPFILE, shell=True)
subprocess.call("echo " + EMAIL_BODY + " > " + TMPFILE, shell=True)
cmd = "cat " + TMPFILE + " | mail -s " + "\"" + EMAIL_SUBJECT + "\" " + EMAIL_RECIPIENT
subprocess.call(cmd, shell=True)
#subprocess.call("rm -rf " + TMPFILE, shell=True)

sys.exit(0)


