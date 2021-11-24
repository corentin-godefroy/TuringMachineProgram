TuringMachineProgram: main.o machine.o
	gcc -o TuringMachineProgram main.o machine.o
	
main.o: main.c machine.h
	gcc -c main.c
	
machine.o: machine.c
	gcc -c machine.c
