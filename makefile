67:67.o
	gcc -o 67 67.o
67.o:67.c
	gcc -c 67.c
clean:
	rm 67 67.o
