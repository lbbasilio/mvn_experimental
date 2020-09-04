#include <string.h>
#include <stdlib.h>

#include "strutils.h"

strTokens* strSplit (char* source, char* delimiters) {

	strTokens* tokens = (strTokens*)malloc(sizeof(strTokens));;
	tokens->strings = NULL;
	tokens->number = 0;

	int sourceLength = strlen(source);
	if (!sourceLength) return tokens;

	int delimLength = strlen(delimiters);
	int count = 0;
	
	// Count delimiter occurrences	
	int i;
	for (i = 0; i < delimLength; ++i) {
		count += strCountChar(source, delimiters[i]);
		if (source[sourceLength - 1] == delimiters[i]) count--;
	} 

	if (count == 0) {
		tokens->strings = (char**)malloc(sizeof(char*));
		tokens->strings[0] = strDup (source);
		tokens->number = 1;
		return tokens;
	}

	// Ignore double delimiter occurrence
	char lastCh = source[0];
	for (i = 1; i < sourceLength; ++i) {
		
		char ch = source[i];
		
		int flag = 0;
		int j;
		for (j = 0; j < delimLength; ++j) {
			if (lastCh == delimiters[j]) flag++;
			if (ch == delimiters[j]) flag++;
		}

		if (flag == 2) count--;
		lastCh = ch;

	}

	// Allocate correct number of substrings
	char** strArray = (char**)malloc(sizeof(char*) * (count + 1));

	// Find and create substrings
	int pos = 0;
	int lastPos;
	for (i = 0; i <= count; ++i) {
		
		lastPos = strFindFirstNotOf (source, delimiters, pos);
		pos = strFindFirstOf (source, delimiters, lastPos);
		if (pos == -1) pos = sourceLength;

		strArray[i] = strSubstr (source, lastPos, pos);
		pos++;
	}

	tokens->strings = strArray;
	tokens->number = count + 1;
	return tokens;
}

void strFreeTokens (strTokens* T) {
	
	if (T) {
		int i;
		int count = T->number;
		for (i = 0; i < count; ++i) free(T->strings[i]);
		free(T->strings);
		free(T);
	}	
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

	for (position = start; position > 0; --position) {
	
		char ch = source[position];
		int num = strlen(targets);
		int it;
		for (it = 0; it < num; ++it) {
			if (ch == targets[it]) return position;
		}
	}

	return -1;
}

int strFindFirstNotOf (char* source, char* targets, int start) {
	
	int position;
	int length = strlen(source);

	if (start > length - 1) return -1;

	for (position = start; position < length; ++position) {
	
		char ch = source[position];
		int num = strlen(targets);
		int flag = 0;
		int it;
		for (it = 0; it < num; ++it) {
			if (ch != targets[it]) flag++;
		}

		if (flag == num) return position;
	}

	return -1;
}

int strFindLastNotOf (char* source, char* targets, int start) {
	
	int position;
	int length = strlen(source);

	if (start > length - 1) return -1;

	for (position = start; position > 0; --position) {
	
		char ch = source[position];
		int num = strlen(targets);
		int flag = 0;
		int it;
		for (it = 0; it < num; ++it) {
			if (ch != targets[it]) flag++;
		}

		if (flag == num) return position;
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

		if (end > length) end = length;
		
		int charCount = end - start;
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
