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

struct QstateList{
    Qstate *state;
    struct QstateList *next;
};
typedef struct QstateList Qlist;

machine *initMachine(char *input){
	machine *M = malloc(sizeof(machine));
	M->initState = NULL;
	M->currentState = NULL;
	M->input = input;
	M->position = 0;
	return M;
}

Qlist *addQlist(Qlist *list, char *name){
    if(list->next == NULL){
	Qstate *newState = malloc(sizeof(Qstate));
	newState->name = name;
	newState->final = 0;
	newState->links = NULL;

	Qlist *newChain = malloc(sizeof(Qlist));
	newChain->state = newState;
	newChain->next = NULL;
	list->next = newChain;
	return newChain;
    }
    addQlist(list->next, name);
}

//Return the Qstate of given name searched on Qlist if exists, NULL returned else 
Qstate *searchQlist(Qlist *list, char *name){
    //TODO
}

//Modify given values 
void modifyQlist(Qlist *list, char *name, char *type){
    //TODO
}

void parserMT(char *path, char *input){
    //reconnaitre état
    //initialiser état
    //faire liaison correspondantes => créer états si non existant
    machine *M = initMachine(input);
    char *delimiters = "; :,\n\0";
    
    FILE *descMachine = fopen(path, "r");
    if(descMachine == NULL){
	fprintf(stderr, "Descripting file of turing machine not found");
	exit(1);
    }
    
    char *line = malloc(128*sizeof(char));
    token *tok = NULL;
    token *tokBuf = NULL;
    char *str = NULL;
    int lineNumber = 0;
    while(lineNumber >= 0){
	lineNumber++;
	do{
	    line = fgets(line, 128, descMachine);
	    if(line == NULL) return;
	    if(line[0] != 10){
		tok = strToTok(line, delimiters);
		str = getTokStr(tok);
	    }
	}while((line[0] == 10) || ((str[0] == 47) && (str[1] == 47))); //ascii 10 code "\n", 47 code "/"
	tokBuf = tok;
	printTok(tokBuf);
	
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
		str = getTokStr(tokBuf);
		if((str[0] != 47) || (str[1] != 47)){
		    fprintf(stderr, "\x1B[31m(2) 2 init states have been given on line %d\n%sword : %s\n\x1B[0m", lineNumber, line, getTokStr(tokBuf));
		    exit(2);
		}
	    }
	}
	
	//else if(strcmp(str, "accept") == 0){
	//tokBuf = getNextTok(tokBuf);
	//}
	//else if((str[0] != 47) && (str[1] != 47)){
	
	//}
	
    }
    fclose(descMachine);
}
