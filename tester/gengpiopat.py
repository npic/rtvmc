#! /usr/bin/env python

import sys

FOUT_PREFIX = 'gpio_'
FOUT_SUFFIX = '.pat'

def usage():
    print 'Usage:', sys.argv[0], '[descriptor file] [test length]'
    return

if len(sys.argv) != 3:
    usage()
    sys.exit(1)

descriptorFile = sys.argv[1]
testLength = int(sys.argv[2])
fin = open(descriptorFile, 'r')
rawDescriptions = fin.readlines()
fin.close()

descriptions = {}

for i in range(len(rawDescriptions)):
    (rawTime, rawPorts, rawValue) = rawDescriptions[i].strip().split(';')
    timestamp = int(rawTime)
    ports = map(lambda x: int(x), rawPorts.split(','))
    values = rawValue
    if len(values) != len(ports):
        print 'Mismatch in ports and values lengths at line', str(i+1)
        sys.exit(1)
    for j in range(len(ports)):
        port = ports[j]
        if not descriptions.has_key(port):
            descriptions[port] = {}
        descriptions[port][timestamp] = values[j]

for port, description in descriptions.items():
    fout = open(FOUT_PREFIX + str(port) + FOUT_SUFFIX, 'w')
    for t in range(testLength):
        fout.write(str(t) + ' ')
        if description.has_key(t):
            fout.write(description[t])
        else:
            fout.write('.*')
        fout.write('\n')
    fout.close()

sys.exit(0)
