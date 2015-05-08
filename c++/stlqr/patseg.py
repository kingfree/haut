#!/usr/bin/env python
#
# Extract lines between pattern(s)
# Author:  Yotam Medini  yotam.medini@gmail.com -- Created: 2006/May/22
#

import sys
import string

def usage(a0, rc, errmsg=None):
    if errmsg != None:
        sys.stderr.write(errmsg + '\n')
    sys.stderr.write("Usage: %s <beginPattern> [endPattern]\n" % a0)
    sys.exit(rc)

rc = 0
if len(sys.argv) < 2:
    usage(sys.argv[0], 1)
begPat = sys.argv[1]
endPat = len(sys.argv) > 2 and sys.argv[2] or begPat
sys.stderr.write("begPat=%s, endPat=%s\n" % (begPat, endPat))

skip = True
while skip:
    line = sys.stdin.readline()
    skip = (string.find(line, begPat) == -1)

line = sys.stdin.readline()
while line != '' and (string.find(line, endPat) == -1):
    sys.stdout.write(line)
    line = sys.stdin.readline()
if line == '':
    sys.stderr.write("No end pattern\n")
    rc = 1
sys.exit(rc)
