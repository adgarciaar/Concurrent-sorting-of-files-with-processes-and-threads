csortp: csortp.o procesamientoArchivo.o
	gcc -lpthread -pthread -ansi -o csortp csortp.o procesamientoArchivo.o
csortp.o: csortp.c procesamientoArchivo.h
	gcc -c csortp.c -lpthread -pthread
leerArchivo.o: procesamientoArchivo.c procesamientoArchivo.h
	gcc -c procesamientoArchivo.c

clean:
	rm *.o csortp
