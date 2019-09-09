proyecto1: csortp.o leerArchivo.o
	gcc -lpthread -pthread -ansi -o proyecto1 csortp.o leerArchivo.o
csortp.o: csortp.c leerArchivo.h
	gcc -c csortp.c -lpthread -pthread
leerArchivo.o: leerArchivo.c leerArchivo.h
	gcc -c leerArchivo.c

clean:
	rm *.o proyecto1
