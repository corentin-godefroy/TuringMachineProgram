#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mystr.h"

struct token{
    char *str;
    struct token *next;
};

typedef struct token token;

//Renvois le pointeur du dernier token
token *endOfTok(token *tok){
    if(tok->next == NULL){ return tok; }
    else{ endOfTok(tok->next); }
}

void addStrTok(token *tok, char *c){
    tok->str = c;
}

token *addNewTok(token *tok, char *c){
    token *tokBuf = endOfTok(tok);
    token *next = malloc(sizeof(token));
    next->str = c;
    next->next = NULL;
    tokBuf->next = next;
    return next;
}

char *getTokStr(token *tok){
    char *c = tok->str;
    return c;
}

void printTok(token *tok){
    if(tok != NULL){
	char *str = tok->str;
	printf("\"%s\", ", str);
	printTok(tok->next);
    }
    else printf("\n");
}

token *getNextTok(token *tok){
    if(tok == NULL){ return NULL; }
    if(tok->next == NULL){ return NULL; }
    return tok->next;
}

//str : chaine d'entrée
//del : carractères de délimitation servant à découper la chaine en token
//renvoie une liste chainée de token 
token *strToTok(char *str, char *del){
    int posStr = 0;
    int posDel = 0;
    int startTokPos = 0;
    token *tok;
    tok = NULL;

    for(posStr = 0; posStr <= strlen(str); posStr++){
	for(posDel = 0; posDel < strlen(del); posDel++){
	    char charStrBuf = str[posStr];
	    char charDelBuf = del[posDel];

	    if((charDelBuf == charStrBuf) && (startTokPos != posStr)){
		char *newToken = malloc((posStr - startTokPos + 1)*sizeof(char));
		int tokPos = 0;
		for(int i = startTokPos; i < posStr; i++){
		    newToken[tokPos] = str[i];
		    tokPos++;
		}
		newToken[posStr-startTokPos] = 0;
		if(tok != NULL){
		    token *next = malloc(sizeof(tok));
		    next->str = newToken;
		    next->next = NULL;
		    token *lastTok = endOfTok(tok);
		    lastTok->next = next;
		    startTokPos = posStr + 1;
		}
		else {
		    tok = malloc(sizeof(token));
		    tok->str = newToken;
		    tok->next = NULL;
		    startTokPos = posStr + 1;
		}
	    }
	    else if((charDelBuf == charStrBuf) && (startTokPos == posStr)){
		startTokPos = posStr + 1;
	    }
	}
    }
    return tok;
}
