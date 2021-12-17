#include <stdio.h>
#include <stdlib.h>
#include "machine.h"
#include "mystr.h"

void main(int argc, char *argv[]){    
    machine *M = parserMT(argv[1], argv[2]);
    int status = 1;
    printMT(M);
    while(status){
	status = runMT(M);
	printMT(M);
    }
    exit(0);
}
