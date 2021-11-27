#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "machine.h"

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
	Qstate *initState;
	Qstate *currentState;
	char *input;
	int position;
};

typedef struct machine machine;

machine* initMachine(char *c){
	machine *M = malloc(sizeof(machine));
	M->initState = NULL;
	M->currentState = NULL;
	M->input = c;
	M->position = 0;
	return M;
}

void parserMT(char *params[], machine *M){
    //TODO parsing du fichier par lecture ligne à ligne en ignorant celle qui commencent par //
    //Reconnaitre état initial
    //reconnaitre état
    //initialiser état
    //faire liaison correspondantes => créer états si non existan

    FILE *descMachine = fopen(params[1], "r");
    if(descMachine != NULL){
	char *line = malloc(40*sizeof(char));
	do{
	    line = fgets(line, 40, descMachine);
	    char *tok = NULL;
	    char *saveptr = line;
	    char *delimiters = (", :");

	    do{
		tok = strtok_r(saveptr, delimiters, &saveptr);
		printf("%s", (&tok[0]));
		printf("\n");
	    
		if((&tok[0] == "init")){
		    printf("\n1 état initial\n");
		}
		else if(M->initState != NULL){
		    fprintf(stderr, "(2) Plusieurs états initiaux ont été donnés.");
		}

	    }while(tok != NULL);
	}while(line != NULL);

    }
    else{
	fprintf(stderr, "(1) Descripting file of turing machine not found.");
	exit(1);
    }
    fclose(descMachine);
}
