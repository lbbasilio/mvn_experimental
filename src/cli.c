#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cli.h"
#include "mvn.h"
#include "strutils.h"

void cliInit () {

	cliCommands = dictCreate();
	dictInsert (cliCommands, "HELP", 0);
	dictInsert (cliCommands, "TR", 1);
	dictInsert (cliCommands, "MM", 2);
	dictInsert (cliCommands, "MD", 3);
	dictInsert (cliCommands, "QUIT", 4);
	dictInsert (cliCommands, "LOAD", 5);
	dictInsert (cliCommands, "ASM", 6);
	dictInsert (cliCommands, "BUILD", 7);
	dictInsert (cliCommands, "RESET", 8);
	dictInsert (cliCommands, "SET", 9);
	dictInsert (cliCommands, "PRINT", 10);
	dictInsert (cliCommands, "STEP", 11);
	dictInsert (cliCommands, "CAT", 12);
	dictInsert (cliCommands, "BP", 13);
	dictInsert (cliCommands, "RUN", 14);

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

		strTokens* tokens = strSplit(line, " \t\n");
		if (tokens->number) {

			dictEl* elem = dictLookup (cliCommands, line);
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

		free(tokens->strings);
		free(tokens);
	}

}

void cliHelp () {

}

void cliTrace () {

}

void cliMemModify () {

}

void cliMemDisplay () {

}


void cliPrintReg () {

	printf("\n\tPC = 0x%04x\tACC = 0x%02x\n\n", mvn.pc, mvn.acc);

}

