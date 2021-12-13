#include <stdio.h>
#include <stdlib.h>
#include "machine.h"
#include "mystr.h"

void main(int argc, char *argv[]){    
    machine *M = parserMT(argv[1], argv[2]);
    int status = 1;
    while(status){
	status = runMT(M);
    }
    exit(0);
}
