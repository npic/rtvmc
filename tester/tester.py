#! /usr/bin/env python

import sys
import re

def usage():
    print 'Usage:', sys.argv[0], '[pattern file] [file to check]'
    return

if len(sys.argv) != 3:
    usage()
    sys.exit(1)

patternFile = sys.argv[1]
fileToCheck = sys.argv[2]

fin = open(patternFile, "r")
patterns = fin.readlines()
fin.close()

fin = open(fileToCheck, "r")
lines = fin.readlines()
fin.close()

if len(patterns) != len(lines):
    print 'Mismatch in line count, aborting'
    sys.exit(1)

for i in range(len(lines)):
    if not re.search(patterns[i].strip('\n'), lines[i].strip('\n')):
        print 'Mismatch at line', str(i+1)
        print 'Expected:', patterns[i]
        print 'Got:', lines[i]
        sys.exit(1)

sys.exit(0)
