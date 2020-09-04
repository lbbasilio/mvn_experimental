#ifndef ASM_H
#define ASM_H

#include "dict.h"
#include "strutils.h"

typedef struct {
	
	char* label;
	char* mnemonic;
	char* argument;

} asmLineType;

static dict* asmMnemonics;

void asmInit();
void asmAssemble(strTokens* files);

int asmNumArgumentValidity (char* arg, int* value, int hexSize);

#define LINE_ERROR 0
#define LINE_OK 1
#define LINE_SKIP 2

asmLineType* asmParseLine (char* line, int* result);

#endif
