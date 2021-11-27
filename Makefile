run: main.o machine.o
	./TuringMachineProgram testMachine.txt

TuringMachineProgram: main.o machine.o mystr.o
	gcc -o TuringMachineProgram main.o machine.o mystr.o

main.o: main.c machine.o
	gcc -c -o main.o main.c

machine.o: machine.c
	gcc -c -o machine.o machine.c

mystr.o: mystr.c
	gcc -c -o mystr.o mystr.c
