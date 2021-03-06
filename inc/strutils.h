#ifndef STRUTILS_H
#define STRUTILS_H

typedef struct {

	char** strings;
	int number;

} strTokens;

strTokens* strSplit (char* source, char* delimiters);
void strFreeTokens (strTokens* T);

int strFindFirstOf (char* source, char* targets, int start);
int strFindLastOf (char* source, char* targets, int start);
int strFindFirstNotOf (char* source, char* targets, int start);
int strFindLastNotOf (char* source, char* targets, int start);
int strCountChar (char* source, char ch);
char* strSubstr (char* source, int start, int end);

void strToUpper (char* source);
char* strDup (char* source);

#endif
