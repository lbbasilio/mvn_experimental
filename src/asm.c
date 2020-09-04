#include <stdio.h>
#include <stdlib.h>

#include "asm.h"
#include "strutils.h"

void asmInit() {

	asmMnemonics = dictCreate ();
	dictInsert (asmMnemonics, "JP", 0x0);
	dictInsert (asmMnemonics, "JZ", 0x1);
	dictInsert (asmMnemonics, "JN", 0x2);
	dictInsert (asmMnemonics, "LV", 0x3);
	dictInsert (asmMnemonics, "ADD", 0x4);
	dictInsert (asmMnemonics, "SUB", 0x5);
	dictInsert (asmMnemonics, "MUL", 0x6);
	dictInsert (asmMnemonics, "DIV", 0x7);
	dictInsert (asmMnemonics, "LD", 0x8);
	dictInsert (asmMnemonics, "MM", 0x9);
	dictInsert (asmMnemonics, "SC", 0xa);
	dictInsert (asmMnemonics, "RS", 0xb);
	dictInsert (asmMnemonics, "HM", 0xc);
	dictInsert (asmMnemonics, "GD", 0xd);
	dictInsert (asmMnemonics, "PD", 0xe);
	dictInsert (asmMnemonics, "OS", 0xf);
	dictInsert (asmMnemonics, "ORG", 0x10);
	dictInsert (asmMnemonics, "END", 0x11);
	dictInsert (asmMnemonics, "CONST", 0x12);

}

void asmAssemble(strTokens* files) {
	
	//fopen(files->strings[0], "rb");
	//output = fopen("output.obj", "wb");
	//list = fopen("list.txt", "wb");

	// Read input file
	char* content;
	FILE* input = fopen("src/main.c", "rb");
	if (input) {
	
		// Get file size
		fseek(input, 0, SEEK_END);
		unsigned size = ftell(input);
		content = (char*)malloc(size + 2);
		
		// Read file into buffer
		rewind(input);
		fread((void*) content, sizeof(char), size, input);

		// Add NUL termination
		content[size] = '\n';
		content[size + 1] = '\0';
	}
	fclose(input);

	strTokens* lines = strSplit (content, "\n\r");
	dict* symbolTable = dictCreate ();

	unsigned argOffset = 0;
	unsigned byteCount = 0;

	int i;
	int count = lines->number;
	for (i = 0; i < count; ++i) {

		int res;
		asmLineType* blobs = asmParseLine(lines->strings[i], &res);

		if (res == LINE_SKIP) continue;
		if (res == LINE_ERROR) 
			printf ("ASM: assembly error at line %d\n", i + 1);	

		if (blobs->label) {
			if (dictLookup (symbolTable, blobs->label)) {
				printf ("ASM: multiple definitions of label %s\n", blobs->label);
				printf ("ASM: assembly error at line %d\n", i + 1);
			} else dictInsert (symbolTable, blobs->label, argOffset + byteCount);	
		}

		dictEl* mnemonicElement = dictLookup (asmMnemonics, blobs->mnemonic);
		if (mnemonicElement) {
			
		// CONTINUAR AQUI

		} else {
		
		}
		
		dictEl* 

		byteCount += 2;
		free(blobs->label);
		free(blobs->mnemonic);
		free(blobs->argument);
		free(blobs);
	}

	// Cleanup
	dictFree (symbolTable);
	strFreeTokens (lines);

}

asmLineType* asmParseLine (char* line, int* result) {

	*result = LINE_OK;
	char* copy = strDup (line);

	char* label = NULL;
	char* mnemonic = NULL;
	char* argument = NULL;


	// Check for Empty Line
	int charPos = strFindFirstNotOf (copy, " \t\n", 0);
	if (charPos == -1) {
		*result = LINE_SKIP;
		free(copy);
		return NULL;
	}

	// Check for comment line
	if (copy[charPos] == ';') {
		*result = LINE_SKIP;
		free(copy);
		return NULL;
	}

	// Remove comments
	charPos = strFindFirstOf (copy, ";", 0);
	if (charPos != -1) {
		free(copy);
		copy = strSubstr (line, 0, charPos);
	}

	// Parse Line
	strToUpper (copy);
	strTokens* tok = strSplit (copy, " \t\n");
	if (tok->number < 2 || tok->number > 3) {
		*result = LINE_ERROR;
		free(copy);
		return NULL;
	}

	// Extract label/mnemonic/argument
	if (tok->number == 2) {
		if (strFindFirstNotOf(copy, " \t", 0) == 0) {
			printf ("ASM: missing argument");
			*result = LINE_ERROR;
			free(copy);
			return NULL;
		}

		label = NULL;
		mnemonic = tok->strings[0];
		argument = tok->strings[1];
	}

	label = tok->strings[0];
	mnemonic = tok->strings[1];
	argument = tok->strings[2];

	// Allocate return object
	asmLineType* retLine = (asmLineType*)malloc(sizeof(asmLineType));	
	retLine->label = label;
	retLine->mnemonic = mnemonic;
	retLine->argument = argument;

	return retLine;
}
