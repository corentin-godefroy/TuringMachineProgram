#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "machine.h"
#include "mystr.h"

struct Qstate{
    char *name;
    int final;
    struct linkList *links;
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

struct linkList{
    char letterIn;
    char letterOut;
    char moove;
    struct Qstate *state;
    struct linkList *next;
};
typedef struct linkList linkList;

linkList *getLastLink(linkList *links){
    if(links == NULL){ return NULL; }
    else if(links->next != NULL){ return getLastLink(links->next); }
    else { return links; }
}

void addLink(Qstate *state, linkList *link){
    linkList *list = state->links;
    list = getLastLink(link);
    if(list == NULL){ state->links = link; }
    else{ list->next = link; }
}

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
    if(list->state == NULL){
	list->state = State;
	Qlist *newChain = malloc(sizeof(Qlist));
	newChain->state = NULL;
	newChain->next = NULL;
	list->next = newChain;
	return list;
    }
    return addQlist(list->next, State);
}

//Return the Qstate of given name searched on Qlist if exists, NULL returned else 
Qstate *searchQlist(Qlist *list, char *name){
    if(list->state != NULL){
	Qstate *state = list->state;
	char *str2 = state->name;
	if(strcmp(str2, name) == 0){
	    return state;
	}
	if(list->next != NULL){
	    return searchQlist(list->next, name);
	}
	return NULL;
    }
    return NULL;
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
    int creatingState = 0;
    while(lineNumber >= 0){
	do{
	    lineNumber++;
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
		    fprintf(stderr, "\x1B[31m(2) 2 init states or more have been given on line %d\n%sword : %s\n\x1B[0m", lineNumber, line, getTokStr(tokBuf));
		    exit(2);
		}
	    }
	}
	
	else if(strcmp(str, "accept") == 0){
	    tokBuf = getNextTok(tokBuf);
	    if(tokBuf == NULL){
		fprintf(stdout, "\x1B[31m(4) No accept state given\n\x1B[0m");
                exit(4);
	    }
	    str = getTokStr(tokBuf);
	    if((str[0] == 47) && (str[1] == 47)){
		fprintf(stdout, "\x1B[31m(3) Accept state are commentary\n\x1B[0m");
                exit(3); 
	    }
	    Qstate *state = searchQlist(statesList, str);
	    if(state == NULL){
		addQlist(statesList, newQstate(str, 1));
	    }
	    else{
		state->final = 1;
	    }
	    tokBuf = getNextTok(tokBuf);		    
	    while(tokBuf != NULL){
		str = getTokStr(tokBuf);
		if((str[0] == 47) && (str[1] == 47)){ break; }
		addQlist(statesList, newQstate(str, 1));
		tokBuf = getNextTok(tokBuf);
	    }
	}

	else{
	    str = getTokStr(tokBuf);
	    Qstate *state =  searchQlist(statesList, str);
	    if(state == NULL){
		state = newQstate(str, 0);
		addQlist(statesList, state);
	    }
	    tokBuf = getNextTok(tokBuf);
	    str = getTokStr(tokBuf);

	    linkList *links= state->links;
	    links = getLastLink(links);
	    linkList *new = malloc(sizeof(linkList));
	    if(links == NULL){ links = new; }
	    else{ links->next = new; }
	    new->next = NULL;
	    
	    if(strlen(str) == 1){ links->letterIn = str[0]; }
	    else{
		fprintf(stdout, "\x1B[31mError, transistion at line %d, \"%s\" are not a simple carracter\x1B[0m\n", lineNumber, str);
		exit(6);
	    }
	    
	    tokBuf = getNextTok(tokBuf);
	    
	    if(tokBuf != NULL){
		str = getTokStr(tokBuf);
		if((str[0] != 47) || (str[1] != 47)){
		    fprintf(stdout, "\x1B[31m3 or more parameters at line %d \"%s\" are given. Commentary expected.\x1B[0m\n", lineNumber, str);
		    exit(5);
		}
	    }
	    
	    addLink(state, links);
	    
	    do{
		lineNumber++;
		line = fgets(line, 128, descMachine);
		if(line == NULL) return;
		if(line[0] != 10){
		    tok = strToTok(line, delimiters);
		    str = getTokStr(tok);
		}
	    }while((line[0] == 10) || ((str[0] == 47) && (str[1] == 47)));
	    tokBuf = tok;

	    //New state
	    state =  searchQlist(statesList, str);
            if(state == NULL){
                state = newQstate(str, 0);
                addQlist(statesList, state);
	    }
	    links->state = state;

	    //writed char
	    tokBuf = getNextTok(tokBuf);
	    if(tokBuf == NULL){
		fprintf(stdout, "\x1B[31mError, \"writed char\" token  expected at line %d. If blank, pleaze write \"_\"\x1B[0m\n", lineNumber);
		exit(6);
	    }
	    str = getTokStr(tokBuf);
	    if(strlen(str) != 1){
		fprintf(stdout, "\x1B[31mError, \"writed char\" token at line %d, \"%s\" is not a simple char\x1B[0m\n", lineNumber, str);
                exit(6);      
	    }
	    links->letterOut = str[0];

	    //moove
	    tokBuf = getNextTok(tokBuf);
	    if(tokBuf == NULL){
		fprintf(stdout, "\x1B[31mError, \"moove\" token expected at line %d\x1B[0m\n", lineNumber);
		exit(6);
	    }
	    str = getTokStr(tokBuf);
	    if(strlen(str) != 1){
		fprintf(stdout, "\x1B[31mError, \"moove\" token at line %d is not a simple char\x1B[0m\n", lineNumber);
		exit(6);
	    }
	    if((str[0] == 45) || (str[0] == 60) || (str[0] == 62)){
		links->moove = str[0];
	    }
	    else{
		fprintf(stdout, "\x1B[31mThe moove on line %d \"%s\" doesn't exist\x1B[0m\n", lineNumber, str);
		exit(7);
	    }

	    tokBuf = getNextTok(tokBuf);
	    if(tokBuf != NULL){
		str = getTokStr(tokBuf);
		if((str[0] != 47) || (str[1] != 47)){
		    fprintf(stdout, "\x1B[31m3 or more parameters at line %d \"%s\" are given. Commentary expected.\x1B[0m\n", lineNumber, str);
		    exit(5);
		}
	    }


	    
	}	
    }
    fclose(descMachine);
}
