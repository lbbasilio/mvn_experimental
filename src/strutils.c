#include <string.h>
#include <stdlib.h>

#include "strutils.h"

strTokens* strSplit (char* source, char* delimiters) {

	int delimLength = strlen(delimiters);
	int count = 0;
	int i;
	for (i = 0; i < delimLength; ++i) {
		count += strCountChar(source, delimiters[i]);
		if (source[strlen(source)] == delimiters[i]) count--;
	} 

	char** strArray = (char**)malloc(sizeof(char*) * (count + 1));

	int pos = 0;
	int lastPos;
	for (i = 0; i <= count; ++i) {
		
		lastPos = pos;
		pos = strFindFirstOf (source, delimiters, pos);
		if (pos == -1) pos = strlen(source);

		strArray[i] = strSubstr (source, lastPos, pos);
		if (i < count) pos++;

	}

	strTokens* tokens = (strTokens*)malloc(sizeof(strTokens));;
	tokens->strings = strArray;
	tokens->number = count + 1;
	return tokens;
}

int strFindFirstOf (char* source, char* targets, int start) {

	int position;
	int length = strlen(source);

	if (start > length - 1) return -1;

	for (position = start; position < length; ++position) {
		
		char ch = source[position];
		int num = strlen(targets);
		int it;
		for (it = 0; it < num; ++it) {
			if (ch == targets[it]) return position;	
		}
	}
	
	return -1;
}

int strFindLastOf (char* source, char* targets, int start) {
	
	int position;
	int length = strlen(source);

	if (start > length - 1) return -1;

	for (position = length - 1; position > 0; --position) {
	
		char ch = source[position];
		int num = strlen(targets);
		int it;
		for (it = 0; it < num; ++it) {
			if (ch == targets[it]) return position;
		}
	}

	return -1;
}

int strCountChar (char* source, char ch) {

	int i, length = strlen(source);
	int count = 0;
	for (i = 0; i < length; ++i) {
		if (source[i] == ch) count++;
	}

	return count;
}

char* strSubstr (char* source, int start, int end) {

	int length = strlen(source);
	
	if (start > length - 1 || start >= end || start < 0 || end < 0) return NULL;
	else {

		if (end > length - 1) end = length - 1;
		
		int charCount = end - start + 1; 		
		char* newStr = (char*)malloc(sizeof(char) * (charCount + 1));
		memcpy((void*) newStr, (void*) (source + start), charCount);
		newStr[charCount] = '\0';

		return newStr;
	
	}

	return NULL;
}

void strToUpper (char* source) {
	
	int i, length = strlen(source);
	for (i = 0; i < length; ++i) {
		char ch = source[i];
		if (ch > 96 && ch < 123) source[i] = ch - 32;
	}
}

char* strDup (char* source) {

	char* copy = (char*)malloc(strlen(source) + 1);
	strcpy (copy, source);
	return copy;
}
