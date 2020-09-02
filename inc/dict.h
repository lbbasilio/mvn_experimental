#ifndef DICT_H
#define DICT_H

#define HASH_SIZE 77
#define HASH_SRC 31

typedef struct dictEl {
	struct dictEl* next;
	char* key;
	int value;
} dictEl; 

typedef struct {
	dictEl* table[HASH_SIZE];
} dict;

dict* dictCreate ();
unsigned dictHash (const char* str);
dictEl* dictLookup (dict* D, const char* key);
dictEl* dictInsert (dict* D, const char* key, int value);

#endif
