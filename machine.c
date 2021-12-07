#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "machine.h"
#include "mystr.h"

struct Qstate{
	char *name;
	int final;
	struct link *links;
};

typedef struct Qstate Qstate;

struct link{
	char letter;
	Qstate *qstate;	
};

typedef struct link link;

struct MT{
    char *name;
    Qstate *initState;
    Qstate *currentState;
    char *input;
    int position;
};

typedef struct MT machine;

machine *initMachine(char *input){
	machine *M = malloc(sizeof(machine));
	M->initState = NULL;
	M->currentState = NULL;
	M->input = input;
	M->position = 0;
	return M;
}

void parserMT(char *path, char *input){
    //TODO parsing du fichier par lecture ligne à ligne en ignorant celle qui commencent par //
    //Reconnaitre état initial
    //reconnaitre état
    //initialiser état
    //faire liaison correspondantes => créer états si non existant
    machine *M = initMachine(input);
    char *delimiters = "; :,\n\0";
    
    FILE *descMachine = fopen(path, "r");
    if(descMachine != NULL){
	char *line = malloc(128*sizeof(char));
	line = fgets(line, 128, descMachine);	
	token *tok = NULL;
	int compteur = 0;
	while(line != NULL){
	    
	    compteur++;
	    tok = strToTok(line, delimiters);

	    printTok(tok);

	    token *tokBuf = tok;
	    char *str = getTokStr(tokBuf);
	    if(strcmp(str, "name") == 0){
		M->name = getTokStr(getNextTok(tokBuf));
	    }		
	    else if(strcmp(str, "init") == 0){
		Qstate *init = malloc(sizeof(Qstate));
		tokBuf = getNextTok(tokBuf);
		init->name = getTokStr(tokBuf);
		init->final = 0;
		init->links = NULL;
		M->initState = init;
		
		tokBuf = getNextTok(tokBuf);
		if(tokBuf != NULL){
		    fprintf(stderr, "(2) 2 init states have been given : line %d\n%sword : %s\n", compteur, line, getTokStr(tokBuf));
		    exit(2);
		}
	    }
	    //else if(strcmp(str, "accept") == 0){
	    //    tokBuf = getNextTok(tokBuf);
	    //if(M->initState->name == tokBuf)
	    //}
	    else if((str[0] != 47) && (str[1] != 47)){
		
	    }
	    do{ line = fgets(line, 128, descMachine);
		if(line == NULL) break;
	    }while((line[0] == 10) && (line != NULL));
	}
    }
    else{
	fprintf(stderr, "(1) Descripting file of turing machine not found.");
	exit(1);
    }
    fclose(descMachine);
}
