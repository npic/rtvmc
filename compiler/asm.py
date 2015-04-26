#!/usr/bin/env python

import sys

def mk16bit(number):
    hi = (number / 256) % 256
    lo = number % 256
    return [hi, lo]

filename = sys.argv[1]
fin = open(filename, "r")
program = fin.readlines()
program = map(lambda x: x.strip().split(), program)
fin.close()

proc_lengths = []

offsets = {}
current_offset = 0

variables = {}
current_var_offset = 0

for line in program:
    if line == []:
        continue
    elif line[0].endswith(':'):
        offsets[line[0][:-1]] = current_offset
    elif line[0].startswith("proc"):
        proc_lengths.append(current_offset)
        current_offset = 0
        current_var_offset = 0
        continue
    elif line[0].startswith("var"):
        variables[line[1]] = current_var_offset
        current_var_offset += 1
    elif line[0] == "stop":
        current_offset += 1
    elif line[0] == "nop":
        current_offset += 1
    elif line[0] == "push":
        current_offset += 2
    elif line[0] == "dup":
        current_offset += 1
    elif line[0] == "drop":
        current_offset += 1
    elif line[0] == "add":
        current_offset += 1
    elif line[0] == "sub":
        current_offset += 1
    elif line[0] == "mul":
        current_offset += 1
    elif line[0] == "div":
        current_offset += 1
    elif line[0] == "mod":
        current_offset += 1
    elif line[0] == "not":
        current_offset += 1
    elif line[0] == "and":
        current_offset += 1
    elif line[0] == "or":
        current_offset += 1
    elif line[0] == "bnot":
        current_offset += 1
    elif line[0] == "band":
        current_offset += 1
    elif line[0] == "bor":
        current_offset += 1
    elif line[0] == "uartin":
        current_offset += 1
    elif line[0] == "uartout":
        current_offset += 1
    elif line[0] == "store":
        current_offset += 1
    elif line[0] == "get":
        current_offset += 1
    elif line[0] == "jt":
        current_offset += 1
    elif line[0] == "eq":
        current_offset += 1
    elif line[0] == "neq":
        current_offset += 1
    elif line[0] == "geq":
        current_offset += 1
    elif line[0] == "leq":
        current_offset += 1
    elif line[0] == "gt":
        current_offset += 1
    elif line[0] == "lt":
        current_offset += 1
    elif line[0] == "syncuart":
        current_offset += 1
    else:
        print line[0]
        print "Unknown opcode"
        sys.exit(1)
proc_lengths.append(current_offset)
proc_lengths = proc_lengths[1:]

N = 0
code = []
current_offset = 0
for line in program:
    if line == []:
        continue
    elif line[0].endswith(':'):
        continue
    elif line[0].startswith("var"):
        continue
    elif line[0].startswith("proc"):
        period = 0
        length = 0
        for i in range(1, 4, 2):
            if line[i] == "period":
                period = int(line[i+1])
            elif line[i] == "length":
                length = int(line[i+1])
            else:
                print line[i]
                print "Unknown directive of proc"
                sys.exit(1)
        code += mk16bit(period)
        code += mk16bit(length)
        code += mk16bit(proc_lengths[N])
        N += 1
        current_offset = 0
        continue
    elif line[0] == "stop":
        code.append(0x00)
        current_offset += 1
    elif line[0] == "nop":
        code.append(0x01)
        current_offset += 1
    elif line[0] == "push":
        code.append(0x02)
        current_offset += 2
        value = line[1]
        if value.startswith("@"):
            code.append(offsets[value[1:]] - current_offset - 1)
        elif value.startswith("$"):
            code.append(variables[value[1:]])
        else:
            code.append(int(value))
    elif line[0] == "dup":
        code.append(0x03)
        current_offset += 1
    elif line[0] == "drop":
        code.append(0x04)
        current_offset += 1
    elif line[0] == "add":
        code.append(0x05)
        current_offset += 1
    elif line[0] == "sub":
        code.append(0x06)
        current_offset += 1
    elif line[0] == "mul":
        code.append(0x07)
        current_offset += 1
    elif line[0] == "div":
        code.append(0x08)
        current_offset += 1
    elif line[0] == "mod":
        code.append(0x09)
        current_offset += 1
    elif line[0] == "not":
        code.append(0x0A)
        current_offset += 1
    elif line[0] == "and":
        code.append(0x0B)
        current_offset += 1
    elif line[0] == "or":
        code.append(0x0C)
        current_offset += 1
    elif line[0] == "bnot":
        code.append(0x0D)
        current_offset += 1
    elif line[0] == "band":
        code.append(0x0E)
        current_offset += 1
    elif line[0] == "bor":
        code.append(0x0F)
        current_offset += 1
    elif line[0] == "uartin":
        code.append(0x10)
        current_offset += 1
    elif line[0] == "uartout":
        code.append(0x11)
        current_offset += 1
    elif line[0] == "store":
        code.append(0x12)
        current_offset += 1
    elif line[0] == "get":
        code.append(0x13)
        current_offset += 1
    elif line[0] == "jt":
        code.append(0x14)
        current_offset += 1
    elif line[0] == "eq":
        code.append(0x15)
        current_offset += 1
    elif line[0] == "neq":
        code.append(0x16)
        current_offset += 1
    elif line[0] == "geq":
        code.append(0x17)
        current_offset += 1
    elif line[0] == "leq":
        code.append(0x18)
        current_offset += 1
    elif line[0] == "gt":
        code.append(0x19)
        current_offset += 1
    elif line[0] == "lt":
        code.append(0x1A)
        current_offset += 1
    elif line[0] == "syncuart":
        code.append(0x1B)
        current_offset += 1
    else:
        print line[0]
        print "Uknown opcode"
        sys.exit(1)

filename = filename.replace(".vmasm", ".bin")
fout = open(filename, "w")
fout.write(bytearray([N]))
fout.write(bytearray(code))
fout.close();

sys.exit(0)

