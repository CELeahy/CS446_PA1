simpleshell: simpleshell.o
	gcc -o simpleshell simpleshell.o -Wall

clean:
	rm *.o simpleshell