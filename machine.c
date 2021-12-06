#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "machine.h"
#include "mystr.h"

struct Qstate{
	char name;
	int final;
	struct link *links[];
};

typedef struct Qstate Qstate;

struct link{
	char letter;
	Qstate *qstate;	
};

typedef struct link link;

struct machine{
    char *name;
    Qstate *initState;
    Qstate *currentState;
    char *input;
    int position;
};

typedef struct machine machine;

machine* initMachine(char *input){
	machine *M = malloc(sizeof(machine));
	M->initState = NULL;
	M->currentState = NULL;
	M->input = input;
	M->position = 0;
	return M;
}

void parserMT(char *path, machine *M){
    //TODO parsing du fichier par lecture ligne à ligne en ignorant celle qui commencent par //
    //Reconnaitre état initial
    //reconnaitre état
    //initialiser état
    //faire liaison correspondantes => créer états si non existant
    char *delimiters = "; :,\n\0";
    
    FILE *descMachine = fopen(path, "r");
    if(descMachine != NULL){
	char *line = malloc(128*sizeof(char));
	line = fgets(line, 128, descMachine);	
	token *tok = NULL;
	while(line != NULL){
	    tok = strToTok(line, delimiters);


	    printTok(tok);

	    
	    token *tokBuf = tok;
	    while(tokBuf != NULL){
		char *str = getTokStr(tokBuf);
		if(strcmp(str, "name") == 0){
		    M->name = getTokStr(getNextTok(tokBuf));
		}		
		tokBuf = getNextTok(tokBuf);
	    }
	    //faire le parsing a partir des token
	    
	    line = fgets(line, 128, descMachine);
	}
    }
    else{
	fprintf(stderr, "(1) Descripting file of turing machine not found.");
	exit(1);
    }
    fclose(descMachine);
}
