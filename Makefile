CPP = gcc
CFLAGS = -Wall -ansi -g -c

all: relaxation

relaxation: main.o relaxation.o 
	$(CPP)  main.o relaxation.o  -o relaxation

main.o: main.c
	$(CPP) $(CFLAGS) main.c -o main.o

relaxation.o: relaxation.c
	$(CPP) $(CFLAGS) relaxation.c -o relaxation.o

check-syntax:
	$(CPP) -o nul -S ${CHK_SOURCES}
run:
	./relaxation

clean:

	rm -rf *.o *.txt relaxation