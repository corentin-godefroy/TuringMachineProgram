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

Qstate *newQstate(char *name, int final){
    Qstate *new = malloc(sizeof(Qstate));
    new->name = name;
    if((final == 0) || (final == 1)){
	new->final = final;
    }
    else{
	fprintf(stdout, "\x1B[33mWARNING Final value are not valid on state \"%s\". It must be 0 if the state is not final 1 else.\n\x1B[0m", name);
	exit(0);
    }
    new->links = NULL;
    return new;
    
}

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

Qlist *addQlist(Qlist *list, Qstate *State){
    if(list->next == NULL){
	list->state = State;
	Qlist *newChain = malloc(sizeof(Qlist));
	newChain->state = NULL;
	newChain->next = NULL;
	list->next = newChain;
	return list;
    }
    addQlist(list->next, State);
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
    //reconnaitre et initialiser état
    //faire liaison correspondantes => créer états si non existant
    machine *M = initMachine(input);
    char *delimiters = "; :,\n\0";

    Qlist *statesList = malloc(sizeof(Qlist));
    statesList->state = NULL;
    statesList->next = NULL;
    
    FILE *descMachine = fopen(path, "r");
    if(descMachine == NULL){
	fprintf(stdout, "\x1B[31m(1) Descripting file of turing machine not found.\x1B[0m\n");
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
	    tokBuf = getNextTok(tokBuf);
	    str = getTokStr(tokBuf);
	    Qstate *init = newQstate(str, 0);
	    M->initState = init;
	    addQlist(statesList, init);
	    tokBuf = getNextTok(tokBuf);
	    if(tokBuf != NULL){
		str = getTokStr(tokBuf);
		if(((str[0] != 47) || (str[1] != 47))){
		    fprintf(stderr, "\x1B[31m(2) 2 init states have been given on line %d\n%sword : %s\n\x1B[0m", lineNumber, line, getTokStr(tokBuf));
		    exit(2);
		}
	    }
	}
	
	else if(strcmp(str, "accept") == 0){
	    tokBuf = getNextTok(tokBuf);
	    if(tokBuf != NULL){
		Qstate *newState = malloc(sizeof(Qstate));
		str = getTokStr(tokBuf);

		if((str[0] != 47) && (str[1] != 47)){
		    addQlist(statesList, newQstate(str, 1));
		    tokBuf = getNextTok(tokBuf);		    
		    while(tokBuf != NULL){
			str = getTokStr(tokBuf);
			if((str[0] == 47) || (str[1] == 47)){ break; }
			addQlist(statesList, newQstate(str, 1));
			tokBuf = getNextTok(tokBuf);
		    }
		}
		else{
		    fprintf(stdout, "\x1B[31m(3) Accept state are commentary\n\x1B[0m");
		    exit(3);
		}
	    }
	    else{
		fprintf(stdout, "\x1B[31m(4) No accept state given\n\x1B[0m");
		exit(4);
	    }
	}
	//else if((str[0] != 47) && (str[1] != 47)){
	
	//}
	
    }
    fclose(descMachine);
}
