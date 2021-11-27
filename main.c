#include <stdio.h>
#include <stdlib.h>
#include "machine.h"
#include "mystr.h"
void main(int argc, char *argv[]){
    machine *M = initMachine("test");
    char *str = "ceci est un test de séparation, on vas bien voir ce que ca donne.";
    char *del = " ,:";

    char *strModif = strToTok(str, del);
    //if (argc == 3){
    
    //}
    exit(0);
}
