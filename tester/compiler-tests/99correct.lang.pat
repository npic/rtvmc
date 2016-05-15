proc length 0 period 0
stop
proc length 1 period 10
push 1
var a
push \$a
store
push 0
var b
push \$b
store
.*0:
var a
push \$a
get
not
push @.*1
jt
push 0
var a
push \$a
store
push 1
var b
push \$b
store
push 1
push @.*0
jt
.*1:
.*2:
var b
push \$b
get
not
push @.*3
jt
push 0
var b
push \$b
store
push 1
push @.*2
jt
.*3:
.*4:
push 1
not
push @.*5
jt
push 1
push @.*4
jt
.*5:
stop
proc length 1 period 10
push 1
var a
push \$a
store
push 0
var b
push \$b
store
var b
push \$b
get
not
push @.*6
jt
push 50
var a
push \$a
store
push 1
push @.*7
jt
.*6:
var a
push \$a
get
not
push @.*8
jt
push 3
var b
push \$b
store
push 1
push @.*9
jt
.*8:
push 1
var a
push \$a
store
push 1
var b
push \$b
store
.*9:
.*7:
stop
proc length 1 period 5
push 5
gpioin
push \$f
store
var a
push \$a
get
push 1
gpioout
var a
push \$a
get
var a
push \$a
get
add
var a
push \$a
get
push 1
sub
mul
push 8
push 6
push 4
sub
div
var a
push \$a
get
mod
add
var b
push \$b
store
var a
push \$a
get
var a
push \$a
get
var b
push \$b
get
and
or
var c
push \$c
store
var a
push \$a
get
var b
push \$b
get
bxor
var a
push \$a
get
var b
push \$b
get
band
bor
var d
push \$d
store
stop
