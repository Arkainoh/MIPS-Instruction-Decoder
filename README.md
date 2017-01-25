# MIPS-Instruction-Decoder

#### Abstraction:
This program decodes an instruction and prints out several details.

----
#### Usage:
Give a hexadecimal number as an argument to the program. <br>For example, 
```
instdec 3c1d0000
```
Then, the result will be as follows.
```
00111100000111010000000000000000
LUI (ALU op: ADD)
opcode: 001111 (15)
funct: 000000 (0)
rs: 00000 (0)
rt: 11101 (29)
rd: 00000 (0)
imm: 00000000
signext: 0
shiftl16: 1
regwrite: 1
regdst: 0
alusrc: 1
branch: 0
memwrite: 0
memtoreg: 0
jump: 0
aluop: 00
alucontrol: 010
jr: 0
```
----
#### Compiler:
gcc version 5.4.0 (GCC)
