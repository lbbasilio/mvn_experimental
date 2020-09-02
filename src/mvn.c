#include "mvn.h"

void mvnReset () {

	mvn.acc = 0;
	mvn.pc = 0;
	
	int i;
	for (i = 0; i < MEM_SIZE; ++i) mvn.ram[i] = 0;

}

void mvnExecOp () {

	unsigned short opcode = mvn.ram[mvn.pc] << 8;
	opcode |= mvn.ram[mvn.pc + 1];
	mvn.pc += 2;

	unsigned short operand = opcode & 0x0FFF;
	unsigned char instruction = opcode >> 12;

	switch (instruction) {
	
		// Control Flow
		case 0x0: mvn.pc = operand; break;
		case 0x1: if (!mvn.acc) mvn.pc = operand; break;
		case 0x2: if (mvn.acc & 0x80) mvn.pc = operand; break;

		// Arithmetic
		case 0x3: mvn.acc = operand; break;
		case 0x4: mvn.acc += mvn.ram[operand]; break;
		case 0x5: mvn.acc -= mvn.ram[operand]; break;
		case 0x6: mvn.acc *= mvn.ram[operand]; break;
		case 0x7: mvn.acc /= mvn.ram[operand]; break;

		// Memory Access
		case 0x8: mvn.acc = mvn.ram[operand]; break;
		case 0x9: mvn.ram[operand] = mvn.acc; break;

		// Subroutine Call
		case 0xA: mvn.ram[operand] = mvn.pc >> 8;
				  mvn.ram[operand + 1] = mvn.pc & 0xFF;
				  mvn.pc = operand + 2;
				  break;

		case 0xB: break;
		case 0xC: mvn.pc = operand; break;

		// IO
		case 0xD: mvn.acc = fgetc(mvn.stream); break;
		case 0xE: fputc (mvn.acc, mvn.stream); break;

		// OS
		case 0xF: break; 
	}

}
