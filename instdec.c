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
int opcodeDecoder(unsigned int opcode, unsigned int *controls, char **instname);
int aluDecoder(unsigned int funct, unsigned int aluop, unsigned int *alucontrol, unsigned int *jr, char **aluopname);
void printControls(unsigned int controls, unsigned int alucontrol, unsigned int jr);

int main(int argc, char* argv[]) {

	if(argc != 2) {
		printf("<USAGE> instdec 12345678");
		return 1;
	}
	int i;
	unsigned int opcode, rs, rt, rd, imm, funct, controls, aluop, alucontrol, jr;
	char *hexstring = argv[1];
	char *instname, *aluopname;
	int aluDecoderErr;
	unsigned int inst = (unsigned int) strtoll(hexstring, NULL, 16);

	printBin(inst, 32);

	opcode = inst >> 26;
	funct = inst << 26 >> 26;

	if (opcodeDecoder(opcode, &controls, &instname) == -1) return 1;
	
	aluop = controls & ALUOP;

	if (aluDecoder(funct, aluop, &alucontrol, &jr, &aluopname) == -1) return 1;
	jr = jr && (controls & REGDST);

	if (jr) printf("\nJR\n");
	else printf("\n%s (ALU op: %s)\n", instname, aluopname);

	printf("opcode: ");
	printBin(opcode, 6);
	printf(" (%u)\n", opcode);

	printf("funct: ");
	printBin(funct, 6);
	printf(" (%u)\n", funct);

	rs = inst << 6 >> 27;
	printf("rs: ");
	printBin(rs, 5);
	printf(" (%u)\n", rs);

	rt = inst << 11 >> 27;
	printf("rt: ");
	printBin(rt, 5);
	printf(" (%u)\n", rt);

	rd = inst << 16 >> 27;
	printf("rd: ");
	printBin(rd, 5);
	printf(" (%u)\n", rd);

	if (controls & SIGNEXT)
		imm = (int) inst << 16 >> 16; // sign extension
	else 
		imm = inst << 16 >> 16; // zero extension

	printf("imm: %08x\n", imm);

	printControls(controls, alucontrol, jr);

	return 0;
}

void printBin(unsigned int num, int bit) {
	int i, n;
	for (i = bit - 1; i >= 0; i--) { // print binary
		n = num >> i;
		printf("%d", n & 1);
  	}

}

int opcodeDecoder(unsigned int opcode, unsigned int *controls, char **instname) {
	// controls = {signext, shiftl16, regwrite, regdst, alusrc, branch, memwrite, memtoreg, jump, aluop} (11 bits)

	switch(opcode) {
		case 0b000000: *controls = 0b00110000011; // R type
			*instname = "R type";
			break;
		case 0b100011: *controls = 0b10101001000; // LW
			*instname = "LW";
			break;
		case 0b101011: *controls = 0b10001010000; // SW
			*instname = "SW";
			break;
		case 0b000101: *controls = 0b10000100001; // BNE
			*instname = "BNE";
			break;
		case 0b000100: *controls = 0b10000100001; // BEQ
			*instname = "BEQ";
			break;
		case 0b001000: *controls = 0b10101000000; // ADDI
			*instname = "ADDI";
			break;
		case 0b001001: *controls = 0b10101000000; // ADDIU: only difference is exception
			*instname = "ADDIU";
			break;
		case 0b001101: *controls = 0b00101000010; // ORI
			*instname = "ORI";
			break;
		case 0b001111: *controls = 0b01101000000; // LUI
			*instname = "LUI";
			break;
		case 0b000010: *controls = 0b00000000100; // J
			*instname = "JUMP";
			break;
		case 0b000011: *controls = 0b00100000100; // JAL
			*instname = "JAL";
			break;
		default: 
			printf("<ERROR> Unknown Instruction\n");
			return -1;
	}

	return 0;
}

int aluDecoder(unsigned int funct, unsigned int aluop, unsigned int *alucontrol, unsigned int *jr, char **aluopname) {

	*jr = 0;

    switch(aluop) {
		case 0b00: *alucontrol = 0b010;  // add
			*aluopname = "ADD";
			break;
		case 0b01: *alucontrol = 0b110;  // sub
			*aluopname = "SUB";
			break;
		case 0b10: *alucontrol = 0b001;  // or
			*aluopname = "OR";
			break;
		default:
	      	switch(funct) { // RTYPE
	        	case 0b100000: *alucontrol = 0b010; // ADD
	        		*aluopname = "ADD";
					break;
		        case 0b100001: *alucontrol = 0b010; // ADDU: only difference is exception
		        	*aluopname = "ADDU";
					break;
				case 0b100010: *alucontrol = 0b110; // SUB
					*aluopname = "SUB";
					break;
				case 0b100011: *alucontrol = 0b110; // SUBU: only difference is exception
					*aluopname = "SUBU";
					break;
				case 0b100100: *alucontrol = 0b000; // AND
					*aluopname = "AND";
					break;
				case 0b100101: *alucontrol = 0b001; // OR
					*aluopname = "OR";
					break;
				case 0b101010: *alucontrol = 0b111; // SLT
					*aluopname = "SLT";
					break;
				case 0b101011: *alucontrol = 0b111; // SLTU
					*aluopname = "SLTU";
					break;
				case 0b001000: // Maybe JR
					*jr = 1;
					break;
				default:
					printf("<ERROR> Unknown Instruction\n");
					return -1;
	        }
    }
}

void printControls(unsigned int controls, unsigned int alucontrol, unsigned int jr) {
	int signext, shiftl16, regwrite, regdst, alusrc, branch, memwrite, memtoreg, jump, aluop;

	signext = (SIGNEXT & controls) ? 1 : 0;
	shiftl16 = (SHIFTL16 & controls) ? 1 : 0;
	regwrite = (REGWRITE & controls) ? 1 : 0;
	regdst = (REGDST & controls) ? 1 : 0;
	alusrc = (ALUSRC & controls) ? 1 : 0;
	branch = (BRANCH & controls) ? 1 : 0;
	memwrite = (MEMWRITE & controls) ? 1 : 0;
	memtoreg = (MEMTOREG & controls) ? 1 : 0;
	jump = (JUMP & controls) ? 1 : 0;
	aluop = ALUOP & controls;
	printf("signext: %d\n", signext);
	printf("shiftl16: %d\n", shiftl16);
	printf("regwrite: %d\n", regwrite);
	printf("regdst: %d\n", regdst);
	printf("alusrc: %d\n", alusrc);
	printf("branch: %d\n", branch);
	printf("memwrite: %d\n", memwrite);
	printf("memtoreg: %d\n", memtoreg);
	printf("jump: %d\n", jump);
	printf("aluop: ");
	printBin(aluop, 2);

	printf("\nalucontrol: ");
	if (jr) printf("xxx");
	else printBin(alucontrol, 3);
	printf("\njr: %d\n", jr);

}