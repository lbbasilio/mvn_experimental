#include <stdio.h>
#include <string.h>

#include "cli.h"
#include "mvn.h"
#include "strutils.h"

void cliInit () {

	usrCommands = dictCreate();
	dictInsert (usrCommands, "HELP", 0);
	dictInsert (usrCommands, "TR", 1);
	dictInsert (usrCommands, "MM", 2);
	dictInsert (usrCommands, "MD", 3);
	dictInsert (usrCommands, "QUIT", 4);
	dictInsert (usrCommands, "LOAD", 5);
	dictInsert (usrCommands, "ASM", 6);
	dictInsert (usrCommands, "BUILD", 7);
	dictInsert (usrCommands, "RESET", 8);
	dictInsert (usrCommands, "SET", 9);
	dictInsert (usrCommands, "PRINT", 10);
	dictInsert (usrCommands, "STEP", 11);
	dictInsert (usrCommands, "CAT", 12);
	dictInsert (usrCommands, "BP", 13);
	dictInsert (usrCommands, "RUN", 14);

}

void cliLoop () {
	
	int shouldQuit = 0;
	int shouldTrace = 0;
	char line[255];

	while(!shouldQuit) {
	
	
		printf(">> ");
		fgets(line, 255, stdin);

		int len = strlen(line);
		line[len - 1] = 0;
		strToUpper(line);

		dictEl* elem = dictLookup (usrCommands, line);
		if (elem != NULL) {
			
			int com = elem->value;
			switch (com) {
				
				case 4: shouldQuit = 1; break;
				case 10: cliPrintReg(); break;
			
			}
		} else {
			printf("Unrecognized command\n");
		}
	}
}

void cliPrintReg () {

	printf("\n\tPC = 0x%04x\tACC = 0x%02x\n\n", mvn.pc, mvn.acc);

}

int assertArgValidity (char* arg, int* value, int hexSize) {
	
	*value = 0;
	int valid = 0;
	int i;

	int argLen = strlen(arg);
	if (argLen) {
	
		if (arg[0] == '$' && argLen > 1 && argLen <= hexSize + 1) {
			
			valid = 1;
			strToUpper(arg);
			for (i = 1; i < argLen; ++i) {
				*value = *value << 4;
				char ch = arg[i];
				if (ch >= 48 && ch <= 57) *value += ch - 48;
				else if (ch >= 65 && ch <= 70) *value += ch - 55;
				else {
					valid = 0;
					*value = 0;
				}
			}
			
		} else { // Assume its decimal

			valid = 1;
			for (i = 0; i < argLen; ++i) {
				*value *= 10;
				char ch = arg[i];
				if (ch >= 48 && ch <= 57) *value += ch - 48;
				else {
					valid = 0;
					*value = 0;
				}
			}

			unsigned int val = 1;
			for (i = 0; i < hexSize; ++i) val = val << 4;
			if (*value >= val) {
				valid = 0;
				*value = 0;
			}
		}
	}

	return valid;
}
