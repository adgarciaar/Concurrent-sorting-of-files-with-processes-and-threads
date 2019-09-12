#include "procesamientoArchivo.h"
#include <stdbool.h>

void RepartirArchivosProcesos(char array_archivos_input[][maximo_nombre_archivo], int numero_archivos_input, bool bandera_orden_reverso);
registro* LeerArchivosTemporales(int numero_archivos_input, char array_archivos_input[][maximo_nombre_archivo], int total_lineas);
int ContarTotalLineasTemporales(int numero_archivos_input, char array_archivos_input[][maximo_nombre_archivo]);
void ImprimirResultado( registro* array_temporales, char archivo_output[maximo_nombre_archivo], int total_lineas );
void BorrarTemporales(int numero_archivos_input, char array_archivos_input[][maximo_nombre_archivo]);
int main (int argc, char **argv);
