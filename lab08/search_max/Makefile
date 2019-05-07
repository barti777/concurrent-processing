CC = gcc
LD = gcc


OPT = -O3 -fopenmp
LIB = -lm

test: test.o search_max.o 
	$(LD) $(OPT) test.o search_max.o -o ../bin/test.o $(LIB)


test.o: test.c search_max.h
	$(CC) -c $(OPT) test.c

search_max.o: search_max.c search_max.h
	$(CC) -c $(OPT) search_max.c

clean:
	rm -f *.o
