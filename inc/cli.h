#include "dict.h"
#include "strutils.h"

static dict* cliCommands;

void cliInit ();
void cliLoop ();

void cliPrintReg ();

void cliHelp ();
void cliTrace ();
void cliMemModify ();
void cliMemDisplay ();

int assertArgumentValidity (char* arg, int* value);
