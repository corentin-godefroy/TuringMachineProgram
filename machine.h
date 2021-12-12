typedef struct MT machine;
typedef struct QstateList Qlist;
typedef struct Qstate Qstate;
Qlist *addQlist(Qlist *list, Qstate *state);
Qstate *searchQlist(Qlist *list, char *name);
machine *initMachine(char *input);
void printMachine(machine *M);
void parserMT(char *path, char *input);
