typedef struct MT machine;
typedef struct QstateList Qlist;
Qlist *addQlist(Qlist *list, char *name);
machine *initMachine(char *input);
void printMachine(machine *M);
void parserMT(char *path, char *input);
