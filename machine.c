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
    return links;
}

void addLink(Qstate *state, linkList *stateLink, linkList *link){
    if(stateLink == NULL){ state->links = link; }
    else if(stateLink->next == NULL){
	stateLink->next = link;
    }
    else{ addLink(state, stateLink->next, link); }
}

linkList *searchLink(linkList *list, char c){
    if(list ==NULL){ return NULL; }
    char In = list->letterIn;
    linkList *next = list->next; 
    if(In == c){
	return list;
    }
    else if(next != NULL){
	return searchLink(next, c);
    }
    else{
	return NULL;
    }
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

void printQlist(Qlist *list){
    if(list == NULL){ printf("null\n"); }
    else{
	if(list->state != NULL){
	    Qstate *state = list->state;
	    printf("state name : %s\n", state->name);
	    linkList *slist = state->links;
	    if(slist == NULL){ printf("Il n'y a aucun lien pour cet état\n"); }
	    else{
		printf("LINKS :\n");
		do{
		    printf("letterIn : %c, letterOut : %c, moove : %c, new current state : %s\n", slist->letterIn, slist->letterOut, slist->moove, slist->state->name);
		    slist = slist->next;
		}while(slist != NULL);
		}
	    printQlist(list->next);
	}
    }
}

machine *parserMT(char *path, char *input){
    //reconnaitre et initialiser état
    //faire liaison correspondantes => créer états si non existant
    machine *M = initMachine(input);
    const char *delimiters = "; :,\n\0";

    Qlist *statesList = malloc(sizeof(Qlist));
    statesList->state = NULL;
    statesList->next = NULL;
    
    FILE *descMachine = fopen(path, "r");
    if(descMachine == NULL){
	fprintf(stdout, "\x1B[31m(1) Descripting file of turing machine not found.\x1B[0m\n");
	exit(-1);
    }
    
    char *line = malloc(128*sizeof(char));
    token *tok = NULL;
    token *tokBuf = NULL;
    char *str = NULL;
    int lineNumber = 0;
    
    while(lineNumber >= 0){
	do{
	    lineNumber++;
	    line = fgets(line, 128, descMachine);
	    if(line == NULL){
		tok = NULL;
		str = NULL;
		break;
	    }
	    if(line[0] != 10){
		tok = strToTok(line, delimiters);
		str = getTokStr(tok);
	    }
	}while((line[0] == 10) || ((str[0] == 47) && (str[1] == 47))); //ascii 10 code "\n", 47 code "/"
	if(str == NULL){ break; }
	tokBuf = tok;
	
        if(strcmp(str, "name") == 0){
	    M->name = getTokStr(getNextTok(tokBuf));
	}
	else if(strcmp(str, "init") == 0){
	    tokBuf = getNextTok(tokBuf);
	    str = getTokStr(tokBuf);
	    Qstate *init = newQstate(str, 0);
	    M->initState = init;
	    M->currentState = init;
	    M->position = 0;
	    addQlist(statesList, init);
	    tokBuf = getNextTok(tokBuf);
	    if(tokBuf != NULL){
		str = getTokStr(tokBuf);
		if(((str[0] != 47) || (str[1] != 47))){
		    fprintf(stderr, "\x1B[31m(2) 2 init states or more have been given on line %d\n%sword : %s\n\x1B[0m", lineNumber, line, getTokStr(tokBuf));
		    exit(0);
		}
	    }
	}
	
	else if(strcmp(str, "accept") == 0){
	    tokBuf = getNextTok(tokBuf);
	    if(tokBuf == NULL){
		fprintf(stderr, "\x1B[31m(4) No accept state given\n\x1B[0m");
                exit(1);
	    }
	    str = getTokStr(tokBuf);
	    if((str[0] == 47) && (str[1] == 47)){
		fprintf(stderr, "\x1B[31m(3) Accept state are commentary\n\x1B[0m");
                exit(2); 
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

	else if(str != NULL){
	    Qstate *state = searchQlist(statesList, str);
	    if(state == NULL){
	        Qlist *list = addQlist(statesList, newQstate(str, 0));
		state = list->state;
	    }
	    linkList *newLink = malloc(sizeof(linkList));
	    addLink(state, state->links, newLink);
	    newLink->next = NULL;
	    tokBuf = getNextTok(tokBuf);
	    
	    if(tokBuf == NULL){
		fprintf(stderr, "\x1B[31mMissing token at line %d. Readed letter espected.\x1B[0m\n", lineNumber);
		exit(3);
	    }
	    str = getTokStr(tokBuf);
	    if(strlen(str) != 1){
		fprintf(stderr, "\x1B[31mMultiple char given for readed letter at line %d.\x1B[0m\n", lineNumber);
		exit(4);
	    }
	    newLink->letterIn = str[0];
	    
	    tokBuf = getNextTok(tokBuf);
	    if(tokBuf != NULL){
		str = getTokStr(tokBuf);
		if((str[0] != 47) || (str[1] != 47)){
		    fprintf(stderr, "\x1B[31mMultiple token given at line %d. You maybe want to write a commentary ?\x1B[0m\n", lineNumber);
		    exit(5);
		}
	    }
	    //récup prochaine ligne
	    do{
		lineNumber++;
		line = fgets(line, 128, descMachine);
		if(line == NULL){
		    fprintf(stderr, "\x1B[31mMissing line for transition at line %d.\x1B[0m\n", lineNumber);
		    exit(6);
		}
		if(line[0] != 10){
		    tok = strToTok(line, delimiters);
		    str = getTokStr(tok);
		}
	    }while((line[0] == 10) || ((str[0] == 47) && (str[1] == 47)));
	    tokBuf = tok;
	    
	    state = searchQlist(statesList, str);
	    if(state == NULL){
		Qlist *list = addQlist(statesList, newQstate(str, 0));
		state = list->state;
	    }
	    newLink->state = state;
	    tokBuf = getNextTok(tokBuf);
	    if(tokBuf == NULL){
		fprintf(stderr, "\x1B[31mMissing \"write\" symbol or \"moove\" token at line %d.\x1B[0m\n", lineNumber);
		exit(7);
	    }
	    str = getTokStr(tokBuf);
	    
	    if(strlen(str) != 1){
		fprintf(stderr, "\x1B[31mMultiple char given for \"write symbol\" token at line %d.\x1B[0m\n", lineNumber);
		exit(8);
	    }
	    newLink->letterOut = str[0];
	    
	    tokBuf = getNextTok(tokBuf);
	    if(tokBuf == NULL){
		fprintf(stderr, "\x1B[31mMissing \"write\" symbol or \"moove\" token at line %d.\x1B[0m\n", lineNumber);
		exit(9);
	    }
	    str = getTokStr(tokBuf);
	    if(strlen(str) != 1){
		fprintf(stderr, "\x1B[31mMultiple char given for \"moove\" token at line %d.\x1B[0m\n", lineNumber);
		exit(10);
	    }
	    newLink->moove = str[0];

	    tokBuf = getNextTok(tokBuf);
	    if(tokBuf != NULL){
		str = getTokStr(tokBuf);
		if((str[0] != 47) || (str[1] != 47)){
		    fprintf(stderr, "\x1B[31mMultiple token given at line %d. You maybe want to write a commentarxy ?\x1B[0m\n", lineNumber);
		    exit(11);
		}
	    }
	}
    }

    fclose(descMachine);
    return M;
}

void printMT(machine *M){
    int pos  = M->position;
    char *posIndic = malloc(pos*sizeof(char));
    for(int i = 0; i < pos; i ++){
	posIndic[i] = 32;
    }
    posIndic[pos] = 94;
    posIndic[pos + 1] = 0;

    printf("Configuration :\n\
Etat courrant : %s\n\
Etat de la bande      : %s\n\
Position sur la bande : %s\n\n", M->currentState->name, M->input, posIndic);

}


int runMT(machine *M){
    Qstate *current = M->currentState;
    char *input = M->input;
    int pos = M->position;
    
    if(input[pos] == 32){ input[pos] = 95; }
    char in = input[pos];
    linkList *link = searchLink(current->links, in);
    if(link == NULL){
	if(current->final == 0){ printf("REJECTED : Le mot n'est pas reconnu par le langage.\n"); }
	else { printf("ACCEPTED : Le mot est reconnu par le langage.\n"); }
	return 0;
    }
    
    M->currentState = link->state;
    input[pos] = link->letterOut;
    M->input = input;
    char moove = link->moove;
    if(moove == 60){
	pos = pos - 1;
	if(pos < 0){
	    int i = 0;
	    char *newInput = malloc((strlen(input) + 1) * sizeof(char));
	    newInput[0] = 95;
	    i++;
	    while(i <= strlen(input)+1){
		newInput[i] = input[i - 1];
		i++;
	    }
	    M->input = newInput;
	    pos = 0;
	}
	M->position = pos;
    }
    else if(moove == 62){
	pos++;
	if(pos >= strlen(input)){
	    int i = 0;
	    char *newInput = malloc((strlen(input) + 1) * sizeof(char));
	    while(i < strlen(input)){
		newInput[i] = input[i];
		i++;
	    }
	    newInput[i] = 95;
	    newInput[i + 1] = 0;
	    M->input = newInput;
	}
	M->position = pos;
    }
    else if(moove == 45){ M->position = pos; }
    current = M->currentState;
    int final = current->final; 
    if(final == 1){
	printf("ACCEPTED : le mot est reconnus par le langage\n");
	return 0;
    }
    
    return 1;
}
