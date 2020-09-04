#ifndef MVN_H
#define MVN_H

#include <stdio.h>

#define MEM_SIZE 4096

static struct {

	unsigned char ram[MEM_SIZE];
	unsigned char acc;
	unsigned short pc;

	int halt;
	
	FILE* inStream;
	FILE* outStream;

} mvn;

void mvnReset ();
void mvnExecOp ();

#endif
