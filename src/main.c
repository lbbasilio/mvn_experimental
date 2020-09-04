#include "mvn.h"
#include "dict.h"
#include "cli.h"
#include "strutils.h"
#include "asm.h"

int main () {
	/*dict dicio;
	dictInit(&dicio);
	dictInsert(&dicio, "oimundo", 47);
	int x = (dictLookup(&dicio, "oimundo"))->value;
	printf("Hello dict! %d", x);
	char string[] = "Olha la o bilha, bruno, tiago tchau \t tchau  ";
	strTokens* tok = strSplit (string, " \t");
	int i;
	for (i = 0; i < tok->number; ++i) printf("%s\n", tok->strings[i]); 
	cliInit();
	cliLoop();*/
	int result = 0;
	asmLineType* teste1 = asmParseLine("ola\n", &result);
	asmLineType* teste2 = asmParseLine("eu nao aguento;ah prompou\n", &result);
	asmLineType* teste3 = asmParseLine(" ;ola\n", &result);
	asmLineType* teste4 = asmParseLine("\n", &result);

	asmAssemble(NULL);
	return 0;
}
