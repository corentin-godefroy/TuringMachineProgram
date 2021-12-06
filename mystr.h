typedef struct token token;
token *strToTok(char *str, char *del);
token *newTok(token *tok);
void printTok(token *tok);
