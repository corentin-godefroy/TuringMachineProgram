#include <stdio.h>
#include <stdlib.h>
#include "machine.h"

struct Qstate{
	char name;
	bool final;
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

void parserMT(char *path){
	//TODO parsing du fichier par lecture ligne à ligne en ignorant celle qui commencent par // ou qui sont entre "/* ... */".
	//Reconnaitre état initial
	//reconnaitre état
	//initialiser état
	//faire liaison correspondantes => créer états si non existants
}
