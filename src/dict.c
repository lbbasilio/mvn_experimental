#include "dict.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

dict* dictCreate () {

	dict* D = (dict*)malloc(sizeof(dict));

	int i;
	for (i = 0; i < HASH_SIZE; ++i) D->table[i] = NULL;

	return D;
}

unsigned dictHash (const char* str) {
	unsigned hashVal;
	for (hashVal = 0; *str != 0; ++str)
		hashVal = *str + HASH_SRC * hashVal;

	return hashVal % HASH_SIZE;
}

dictEl* dictLookup (dict* D, const char* key) {
	
	dictEl* it;
	unsigned hashval = dictHash(key);
	for (it = D->table[hashval]; it != NULL; it = it->next) {
		if (strcmp(key, it->key) == 0) 
			return it;
	}

	return NULL;
}

dictEl* dictInsert (dict* D, const char* key, int value) {
	dictEl* it = dictLookup(D, key);

	if (it == NULL) {

		it = (dictEl*) malloc(sizeof(dictEl));
		it->value = value;
		it->key = (char*) malloc(strlen(key) + 1);
		if (it == NULL || strcpy(it->key, key) == NULL)
			return NULL;

		unsigned hashval;
		hashval = dictHash(key);
		it->next = D->table[hashval];
		D->table[hashval] = it;

	} else return NULL;

	return it;
}
