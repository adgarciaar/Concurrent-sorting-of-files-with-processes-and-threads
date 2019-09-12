all: csortp csorth csortpexec

csortp: csortp.o procesamientoArchivo.o
	gcc -ansi -o csortp csortp.o procesamientoArchivo.o
csortp.o: csortp.c csortp.h procesamientoArchivo.c procesamientoArchivo.h
	gcc -c csortp.c

csorth: csorth.o procesamientoArchivo.o
	gcc -lpthread -pthread -ansi -o csorth csorth.o procesamientoArchivo.o
csorth.o: csorth.c csorth.h procesamientoArchivo.c procesamientoArchivo.h
	gcc -c csorth.c -lpthread -pthread

csortpexec: csortpexec.o procesamientoArchivo.o
	gcc -ansi -o csortpexec csortpexec.o procesamientoArchivo.o
csortpexec.o: csortpexec.c csortpexec.h procesamientoArchivo.c procesamientoArchivo.h
	gcc -c csortpexec.c

procesamientoArchivo.o: procesamientoArchivo.c
	gcc -c procesamientoArchivo.c

clean:
	rm *.o csortp csorth csortpexec
