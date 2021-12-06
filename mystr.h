typedef struct token token;
token *strToTok(char *str, char *del);
token *newTok(token *tok);
char *getTokStr(token *tok);
void addStrTok(token *tok, char *c);
token *addNewTok(token *tok, char *c);
token *getNextTok(token *tok);
char *getTokStr(token *tok);
void printTok(token *tok);
