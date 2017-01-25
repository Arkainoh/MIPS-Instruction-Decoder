#include <stdio.h>
#include <stdlib.h>

void printBin(unsigned int num, int bit);

int main(int argc, char* argv[]) {

	if(argc != 2) {
		printf("<USAGE> instdec 12345678");
		return 1;
	}
	int i;
	unsigned int opcode, rs, rt, rd, imm, funct, n;
	char* hexstring = argv[1];
	unsigned int inst = (unsigned int) strtol(hexstring, NULL, 16);

	printBin(inst, 32);

	opcode = inst >> 26;
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