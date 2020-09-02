#include "dict.h"
#include "strutils.h"

static dict* usrCommands;

void cliInit ();
void cliLoop ();

void cliPrintReg ();

int assertArgumentValidity (char* arg, int* value);
