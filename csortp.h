/*
Nombre del archivo: csortp.h
Autores: Adrián García y Luis Rosales
Objetivo: proveer los prototipos de las funciones que usa el comando csortp
Funciones:
Fecha de última modificación: 12/09/19
*/

#include "procesamientoArchivo.h"
#include <stdbool.h>

void RepartirArchivosProcesos(char arreglo_archivos_input[][MAXIMO_NOMBRE_ARCHIVO], int numero_archivos_input, bool bandera_orden_reverso);
registro* LeerArchivosTemporales(int numero_archivos_input, char arreglo_archivos_input[][MAXIMO_NOMBRE_ARCHIVO], int total_lineas);
int ContarTotalLineasTemporales(int numero_archivos_input, char arreglo_archivos_input[][MAXIMO_NOMBRE_ARCHIVO]);
void ImprimirResultado( registro* arreglo_temporales, char archivo_output[MAXIMO_NOMBRE_ARCHIVO], int total_lineas );
void BorrarTemporales(int numero_archivos_input, char arreglo_archivos_input[][MAXIMO_NOMBRE_ARCHIVO]);
int main (int argc, char **argv);
