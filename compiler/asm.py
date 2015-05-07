#!/usr/bin/env python

import sys

def mk16bit(number):
    hi = (number / 256) % 256
    lo = number % 256
    return [hi, lo]

class Process:
    def __init__(self, ci, ti):
        self.length = ci
        self.period = ti
        self.labels = {}
        self.variables = {}
        self.code_size = 0
        self.var_offset = 0
        return

filename = sys.argv[1]
fin = open(filename, "r")
program = fin.readlines()
program = map(lambda x: x.strip().split(), program)
fin.close()

procs = []
current_proc = None

for line in program:
    if line == []:
        continue
    elif line[0].endswith(':'):
        current_proc.labels[line[0][:-1]] = current_proc.code_size
    elif line[0].startswith("proc"):
        if current_proc:
            procs.append(current_proc)
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
        current_proc = Process(length, period)
        continue
    elif line[0].startswith("var"):
        if line[1] not in current_proc.variables.keys():
            current_proc.variables[line[1]] = current_proc.var_offset
            current_proc.var_offset += 1
    elif line[0] == "stop":
        current_proc.code_size += 1
    elif line[0] == "nop":
        current_proc.code_size += 1
    elif line[0] == "push":
        current_proc.code_size += 2
    elif line[0] == "bxor":
        current_proc.code_size += 1
    elif line[0] == "drop":
        current_proc.code_size += 1
    elif line[0] == "add":
        current_proc.code_size += 1
    elif line[0] == "sub":
        current_proc.code_size += 1
    elif line[0] == "mul":
        current_proc.code_size += 1
    elif line[0] == "div":
        current_proc.code_size += 1
    elif line[0] == "mod":
        current_proc.code_size += 1
    elif line[0] == "not":
        current_proc.code_size += 1
    elif line[0] == "and":
        current_proc.code_size += 1
    elif line[0] == "or":
        current_proc.code_size += 1
    elif line[0] == "bnot":
        current_proc.code_size += 1
    elif line[0] == "band":
        current_proc.code_size += 1
    elif line[0] == "bor":
        current_proc.code_size += 1
    elif line[0] == "uartin":
        current_proc.code_size += 1
    elif line[0] == "uartout":
        current_proc.code_size += 1
    elif line[0] == "store":
        current_proc.code_size += 1
    elif line[0] == "get":
        current_proc.code_size += 1
    elif line[0] == "jt":
        current_proc.code_size += 1
    elif line[0] == "eq":
        current_proc.code_size += 1
    elif line[0] == "neq":
        current_proc.code_size += 1
    elif line[0] == "geq":
        current_proc.code_size += 1
    elif line[0] == "leq":
        current_proc.code_size += 1
    elif line[0] == "gt":
        current_proc.code_size += 1
    elif line[0] == "lt":
        current_proc.code_size += 1
    else:
        print line[0]
        print "Unknown opcode"
        sys.exit(1)
procs.append(current_proc)

N = len(procs)
code = [N]
i = None
current_offset = None

for line in program:
    if line == []:
        continue
    elif line[0].endswith(':'):
        continue
    elif line[0].startswith("var"):
        continue
    elif line[0].startswith("proc"):
        if i is None:
            i = 0
        else:
            i += 1
        current_offset = 0
        code += mk16bit(procs[i].period)
        code += mk16bit(procs[i].length)
        code += mk16bit(procs[i].code_size)
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
            code.append(procs[i].labels[value[1:]] - current_offset - 1)
        elif value.startswith("$"):
            code.append(procs[i].variables[value[1:]])
        else:
            code.append(int(value))
    elif line[0] == "bxor":
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
    else:
        print line[0]
        print "Uknown opcode"
        sys.exit(1)

U = sum( [float(procs[i].length) / float(procs[i].period) for i in range(1, N)] )
threshold = (N-1.0) * (2.0 ** (1.0 / (N-1.0)) - 1.0)
if U > threshold:
    print U, ">", threshold
    print "Task set is not surely schedulable - aborting"
    sys.exit(2)

code = map(lambda x: 256 + x if x < 0 else x, code)
filename = filename.replace(".vmasm", ".bin")
fout = open(filename, "w")
fout.write(bytearray(code))
fout.close();

sys.exit(0)

