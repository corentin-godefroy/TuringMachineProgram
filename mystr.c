#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mystr.h"

char *strToTok(char *str, char *del){
    printf("%s\n", str);
    char *tokenList = NULL;
    int ptrPosStr = 0;
    int ptrPosDel = 0;

    int limStr = strlen(str);
    int limDel = strlen(del);

    char *strBuf1 = malloc(sizeof(str));
    char *strBuf2 = malloc(sizeof(str));

    while(ptrPosStr < limStr){
	strBuf1[ptrPosStr] = str[ptrPosStr];
	ptrPosStr++;
    }

    printf("limDel : %d\n", limDel);

    if(limDel >= 2){
	while(ptrPosDel < limDel-1){
	    ptrPosStr = 0;
	    while(ptrPosStr < limStr){
		//printf("char str : %c, char del : %c\n",str[ptrPosStr], del[ptrPosDel]);
		if(str[ptrPosStr] == del[ptrPosDel]){
		    str[ptrPosStr] = (char)(del[limDel-1]);
		}
		ptrPosStr++;
	    }
	    ptrPosDel++;
	}
    }

    printf("%s\n", strBuf1);
    
    return strBuf1;
}
