typedef struct machine machine;
machine* initMachine(char *input);
void printMachine(machine *M);
void parserMT(char *path, machine *M);
