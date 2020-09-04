#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	dictInsert (asmMnemonics, "OVRL", 0x13);

}

void asmAssemble(strTokens* files) {

	// Read input file
	char* content;
	FILE* input = fopen("src/loader.asm", "rb");
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
	unsigned currentOverlay = 0;
	unsigned assemblyFail = 0;

	typedef struct {

		int skip;
		int absolute;

		unsigned line;
		unsigned overlay;

		unsigned size;
		unsigned opcode;
		unsigned address;
		
		char* argument;
		int argumentDefined;

	} memObj;

	int count = lines->number;
	memObj** MO = (memObj**)malloc(count * sizeof(memObj*));

	int i;
	for (i = 0; i < count && !assemblyFail; ++i) {

		int res;
		asmLineType* blobs = asmParseLine(lines->strings[i], &res);
		MO[i] = (memObj*)malloc(sizeof(memObj));
		MO[i]->skip = 0;
		MO[i]->absolute = 0;
		MO[i]->line = i + 1;
		MO[i]->opcode = 0;
		MO[i]->address = argOffset + byteCount;
		MO[i]->argumentDefined = 0;

		if (res == LINE_SKIP) {
			MO[i]->skip = 1;
			continue;
		}

		if (res == LINE_ERROR) {
			assemblyFail = 1;
			break;
		}

		char* label = blobs->label;
		char* argument = blobs->argument;
		char* mnemonic = blobs->mnemonic;

		MO[i]->argument = argument;

		dictEl* labelElement = NULL;
		dictEl* argumentElement = NULL; 
		dictEl* mnemonicElement = dictLookup (asmMnemonics, mnemonic);


		// Interpret Label definitions
		if (label) {
			if (strFindFirstOf (label, "0123456789$", 0) == 0) {
				printf ("ASM: label should not be numeric constant\n");
				assemblyFail = 1;
			}

			labelElement = dictLookup (symbolTable, label);
			if (labelElement) {

				printf ("ASM: multiple definitions of label %s\n", label);
				assemblyFail = 1;

			} else if (mnemonicElement->value == 0x10 || mnemonicElement->value == 0x11 || mnemonicElement ->value == 0x13) {
				
				printf ("ASM: directives other than CONST should have no label", label);
				assemblyFail = 1;
			
			} else 
				dictInsert (symbolTable, label, argOffset + byteCount);	
		}



		// Interpret argument
		int argValue = 0;
		if (argument) {

			argumentElement = dictLookup (symbolTable, argument);
			if (argumentElement) {
				
				MO[i]->argumentDefined = 1;
				argValue = argumentElement->value;
			
			} else if (strFindFirstOf (argument, "0123456789$", 0) == 0) {
			
				if(!asmNumArgumentValidity(argument, &argValue, 3)) {
					
					printf ("ASM: invalid argument %s\n", argument);
					assemblyFail = 1;
				
				} else { 

					MO[i]->argumentDefined = 1; 
					MO[i]->opcode = argValue;
					MO[i]->absolute = 1;

				}
			}
		} else if (mnemonicElement->value < 0xC || mnemonicElement->value > 0xE) {
		
			printf ("ASM: missing argument for mnemonic %s\n", mnemonicElement->key);
			assemblyFail = 1;	
		
		}


		// Assert mnemonic validity
		if (mnemonicElement) {
			
			char* key = mnemonicElement->key;
			int value = mnemonicElement->value;

			// Valid mnemonic
			if (value < 0x10) {

				MO[i]->opcode |= value << 12; 
				MO[i]->size = 2;
				byteCount += 2;
			
			// Valid directive
			} else {

				if (!MO[i]->argumentDefined && value != 0x11) {
					
					printf("ASM: assembly directive arguments should be numerical constant\n");
					assemblyFail = 1;

				}

				switch (value) {
				
					case 0x10: // ORG
						MO[i]->size = 0;
						argOffset = argValue;
						break;

					case 0x12: // CONST
						byteCount++;
						MO[i]->size = 1;
						MO[i]->opcode = argValue;
						break;

					case 0x13: // OVRL
						argOffset = 0;
						byteCount = 0;
						currentOverlay = argValue;
						break;
				
				}
			}

		} else {

			printf ("ASM: unrecognized mnemonic %s\n", mnemonic);
			assemblyFail = 1;
		
		}

		MO[i]->overlay = currentOverlay;
		//printf ("0x%04x\t%d\n", MO[i]->opcode, i + 1);

		free(blobs->label);
		free(blobs->mnemonic);
		free(blobs);
	}

	if (assemblyFail) {
	   	printf ("ASM: assembly error at line %d\n", i);
		dictFree (symbolTable);
		strFreeTokens (lines);
		return;
	}

	// Solve Dependencies
	assemblyFail = 0;
	for (i = 0; i < count && !assemblyFail; ++i) {

		if (MO[i]->argument && !MO[i]->argumentDefined && !MO[i]->skip) {

			dictEl* el = dictLookup (symbolTable, MO[i]->argument);
			if (el)	MO[i]->opcode |= el->value;
			else assemblyFail = 1;
		}
	}

	if (assemblyFail) {
		printf ("ASM: undefined label %s\n", MO[i - 1]->argument);
		printf ("ASM: assembly error at line %d\n", i);
		dictFree (symbolTable);
		strFreeTokens (lines);
		return;
	}

	FILE** overlays = (FILE**)malloc(16 * sizeof(FILE*));
	
	overlays[0x0] = fopen("overlay0.hex", "wb");
	overlays[0x1] = fopen("overlay1.hex", "wb");
	overlays[0x2] = fopen("overlay2.hex", "wb");
	overlays[0x3] = fopen("overlay3.hex", "wb");
	overlays[0x4] = fopen("overlay4.hex", "wb");
	overlays[0x5] = fopen("overlay5.hex", "wb");
	overlays[0x6] = fopen("overlay6.hex", "wb");
	overlays[0x7] = fopen("overlay7.hex", "wb");
	overlays[0x8] = fopen("overlay8.hex", "wb");
	overlays[0x9] = fopen("overlay9.hex", "wb");
	overlays[0xA] = fopen("overlayA.hex", "wb");
	overlays[0xB] = fopen("overlayB.hex", "wb");
	overlays[0xC] = fopen("overlayC.hex", "wb");
	overlays[0xD] = fopen("overlayD.hex", "wb");
	overlays[0xE] = fopen("overlayE.hex", "wb");
	overlays[0xF] = fopen("overlayF.hex", "wb");
	
	for (i = 0; i < count; ++i) {

		int k = MO[i]->overlay;
		if (!MO[i]->skip && MO[i]->size) {
			if (k > 0xF) break;

			unsigned address = MO[i]->address;
			unsigned opcode = MO[i]->opcode;
			if (MO[i]->size == 2) {
			
				fputc(1, overlays[k]);
				fputc((char) (address >> 0x8), overlays[k]);
				fputc((char) (address & 0xFF), overlays[k]);
				fputc((char) ((opcode >> 0x8) & 0xFF), overlays[k]);
				
				fputc((char) MO[i]->absolute, overlays[k]);
				fputc((char) ((address + 1) >> 0x8), overlays[k]);
				fputc((char) ((address + 1) & 0xFF), overlays[k]);
				fputc((char) (opcode & 0xFF), overlays[k]);
			
			} else {
			
				fputc(1, overlays[k]);
				fputc((char) (address >> 0x8), overlays[k]);
				fputc((char) ((opcode >> 0x8) & 0xFF), overlays[k]);
				fputc((char) (opcode & 0xFF), overlays[k]);
			
			}
		}
	}

	if (i != count) {
		printf("ASM: overlay index %d is greater than maximum (15)\n", MO[i]->overlay);
		printf("ASM: assembly error at line %d\n", i + 1);
	}

	for (i = 0; i < 0xF; ++i) {
		fputc(0x00, overlays[i]);
		fputc(0x00, overlays[i]);
		fputc(0x00, overlays[i]);
		fputc(0x00, overlays[i]);
	}

	// Cleanup
	dictFree (symbolTable);
	strFreeTokens (lines);

	fclose(overlays[0x0]);
	fclose(overlays[0x0]);
	fclose(overlays[0x1]);
	fclose(overlays[0x2]);
	fclose(overlays[0x3]);
	fclose(overlays[0x4]);
	fclose(overlays[0x5]);
	fclose(overlays[0x6]);
	fclose(overlays[0x7]);
	fclose(overlays[0x8]);
	fclose(overlays[0x9]);
	fclose(overlays[0xA]);
	fclose(overlays[0xB]);
	fclose(overlays[0xC]);
	fclose(overlays[0xD]);
	fclose(overlays[0xE]);
	fclose(overlays[0xF]);
	free(overlays);
}

int asmNumArgumentValidity (char* arg, int* value, int hexSize) {
	
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
					printf ("ASM: invalid hexadecimal numerical constant\n");
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
					printf ("ASM: invalid decimal numerical constant\n");
					valid = 0;
					*value = 0;
				}
			}

			unsigned int val = 1;
			for (i = 0; i < hexSize; ++i) val = val << 4;
			if (*value >= val) {
				printf ("ASM: numeric constant is too big\n");
				valid = 0;
				*value = 0;
			}
		}
	}

	return valid;
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
	if (tok->number < 1 || tok->number > 3) {
		*result = LINE_ERROR;
		free(copy);
		return NULL;
	}

	// Extract label/mnemonic/argument
	if (tok->number == 2) {
		if (strFindFirstNotOf(line, " \t", 0) == 0) {
			
			label = tok->strings[0];
			mnemonic = tok->strings[1];
			argument = NULL;
		
		} else {

			label = NULL;
			mnemonic = tok->strings[0];
			argument = tok->strings[1];
		
		}
	} else if (tok->number == 1) {
		if (strFindFirstNotOf(line, " \t", 0) == 0) {
			printf ("ASM: missing mnemonic\n");
			*result = LINE_ERROR;
			free(copy);
			return NULL;
		}

		label = NULL;
		mnemonic = tok->strings[0];
		argument = NULL;
	
	} else {
		label = tok->strings[0];
		mnemonic = tok->strings[1];
		argument = tok->strings[2];
	}

	// Allocate return object
	asmLineType* retLine = (asmLineType*)malloc(sizeof(asmLineType));	
	retLine->label = label;
	retLine->mnemonic = mnemonic;
	retLine->argument = argument;

	return retLine;
}
