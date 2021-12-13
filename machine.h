typedef struct MT machine;
typedef struct QstateList Qlist;
typedef struct Qstate Qstate;
typedef struct linkList linkList;
linkList *getLastLink(linkList *links);
void addLink(Qstate *state, linkList *stateLink, linkList *link);
linkList *searchLink(linkList *list, char c);
Qlist *addQlist(Qlist *list, Qstate *state);
Qstate *searchQlist(Qlist *list, char *name);
machine *initMachine(char *input);
void printMachine(machine *M);
machine *parserMT(char *path, char *input);
int runMT(machine *M);
