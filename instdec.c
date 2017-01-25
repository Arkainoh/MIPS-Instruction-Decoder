#include <stdio.h>
#include <stdlib.h>

#define SIGNEXT  0b10000000000
#define SHIFTL16 0b01000000000
#define REGWRITE 0b00100000000
#define REGDST   0b00010000000
#define ALUSRC   0b00001000000
#define BRANCH   0b00000100000
#define MEMWRITE 0b00000010000
#define MEMTOREG 0b00000001000
#define JUMP     0b00000000100
#define ALUOP    0b00000000011

void printBin(unsigned int num, int bit);

int main(int argc, char* argv[]) {

	if(argc != 2) {
		printf("<USAGE> instdec 12345678");
		return 1;
	}
	int i;
	unsigned int opcode, rs, rt, rd, imm, funct, controls;
	char* hexstring = argv[1];
	char* instname;
	unsigned int inst = (unsigned int) strtol(hexstring, NULL, 16);
	printf("%d\n", (0b0010 & 2) >> 1);
	printBin(inst, 32);

	opcode = inst >> 26;
	
	// controls = {signext, shiftl16, regwrite, regdst, alusrc, branch, memwrite, memtoreg, jump, aluop} (11 bits)

	switch(opcode) {
		case 0b000000: controls = 0b00110000011; // R type
			instname = "R type";
			break;
		case 0b100011: controls = 0b10101001000; // LW
			instname = "LW";
			break;
		case 0b101011: controls = 0b10001010000; // SW
			instname = "SW";
			break;
		case 0b000101: // BNE
		case 0b000100: controls = 0b10000100001; // BEQ
			instname = "BEQ / BNE";
			break;
		case 0b001000: 
		case 0b001001: controls = 0b10101000000; // ADDI, ADDIU: only difference is exception
			instname = "ADDI / ADDIU";
			break;
		case 0b001101: controls = 0b00101000010; // ORI
			instname = "ORI";
			break;
		case 0b001111: controls = 0b01101000000; // LUI
			instname = "LUI";
			break;
		case 0b000010: controls = 0b00000000100; // J
			instname = "JUMP";
			break;
		case 0b000011: controls = 0b00100000100; // JAL
			instname = "JAL";
			break;
		default: 
			printf("<ERROR> Unknown Instruction\n");
			return 1;
	}

	printf("%s\n", instname);
	printf("opcode: %u\n", opcode);

	funct = inst << 26 >> 26;
	printf("funct: %u\n", funct);

	rs = inst << 6 >> 27;
	printf("rs: %u\n", rs);

	rt = inst << 11 >> 27;
	printf("rt: %u\n", rt);

	rd = inst << 16 >> 27;
	printf("rd: %u\n", rd);

	imm = (int) inst << 16 >> 16; // sign extension
	// imm = inst << 16 >> 16; // unsigned extension
	printf("imm: %u\n", imm);
	printBin(imm, 16);
	return 0;
}

void printBin(unsigned int num, int bit) {
	int i, n;
	for (i = bit - 1; i >= 0; i--) { // print binary
		n = num >> i;
		printf("%d", n&1);
  	}
  	printf("\n");
}