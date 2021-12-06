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

token *newTok(token *tok){
    tok = malloc(sizeof(token));
    tok->next = NULL;
    tok->str = "\0";
    return tok;
}

void addStrTok(token *tok, char *c){
    tok->str = c;
}

token *addNewTok(token *tok, char *c){
    token *tokBuf = endOfTok(tok);
    token *next;
    tokBuf->next = newTok(next);
    addStrTok(tokBuf->next, c);
    return tokBuf->next;
}

char *getTokStr(token *tok){
    char *c = tok->str;
    return c;
}

void printTok(token *tok){
    if(tok->next != NULL){
	char *buf = tok->str;
	printf("\"%s\", ", tok->str);
	printTok(tok->next);
    }
    else printf("\n");
}

token *getNextTok(token *tok){
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
    tok = newTok(tok); 

    for(posStr = 0; posStr < strlen(str); posStr++){
	for(posDel = 0; posDel < strlen(del); posDel++){
	    char charStrBuf = str[posStr];
	    char charDelBuf = del[posDel];

	    if((charDelBuf == charStrBuf) && (startTokPos != posStr)){
		char *newTok = malloc((posStr - startTokPos + 1)*sizeof(char));
		int tokPos = 0;
		for(int i = startTokPos; i < posStr; i++){
		    newTok[tokPos] = str[i];
		    tokPos++;
		}
		newTok[posStr-startTokPos] = 0;
		if(tok == NULL) addStrTok(tok, newTok);
		else {
		    token* tokBuf = endOfTok(tok);
		    addNewTok(tokBuf, newTok);
		    startTokPos = posStr + 1;
		}
	    }
	    else if((charDelBuf == charStrBuf) && (startTokPos == posStr)){
		startTokPos++;
	    }
	}
    }
    return tok;
}
