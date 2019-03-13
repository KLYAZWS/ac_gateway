export CC=gcc
all:main.o
	mv main.o ./obj
	make -C src
	make -C obj

main.o:main.c
	$(CC) -c main.c -o main.o -I ./inc/ -lpthread
clean:
	rm obj/*.o
    
    
