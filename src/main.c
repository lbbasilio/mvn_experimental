#include "mvn.h"
#include "dict.h"
#include "cli.h"
#include "strutils.h"

int main () {
	/*dict dicio;
	dictInit(&dicio);
	dictInsert(&dicio, "oimundo", 47);
	int x = (dictLookup(&dicio, "oimundo"))->value;
	printf("Hello dict! %d", x);*/
	char string[] = "Olha la o bilha, bruno, tiago tchau \t tchau  ";
	strTokens* tok = strSplit (string, " \t");
	int i;
	for (i = 0; i < tok->number; ++i) printf("%s\n", tok->strings[i]); 
	cliInit();
	cliLoop();
	return 0;
}
