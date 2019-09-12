all: csortp csorth csortpexec

csortp: csortp.o procesamientoArchivo.o
	gcc -lpthread -pthread -ansi -o csortp csortp.o procesamientoArchivo.o
csortp.o: csortp.c procesamientoArchivo.c procesamientoArchivo.h
	gcc -c csortp.c -lpthread -pthread

csorth: csorth.o procesamientoArchivo.o
	gcc -lpthread -pthread -ansi -o csorth csorth.o procesamientoArchivo.o
csorth.o: csorth.c procesamientoArchivo.c procesamientoArchivo.h
	gcc -c csorth.c -lpthread -pthread

csortpexec: csortpexec.o procesamientoArchivo.o
	gcc -lpthread -pthread -ansi -o csortpexec csortpexec.o procesamientoArchivo.o
csortpexec.o: csortpexec.c procesamientoArchivo.c procesamientoArchivo.h
	gcc -c csortpexec.c -lpthread -pthread

procesamientoArchivo.o: procesamientoArchivo.c
	gcc -c procesamientoArchivo.c

clean:
	rm *.o csortp csorth csortpexec
