/*
Nombre del archivo: csorth.h
Autores: Adrián García y Luis Rosales
Objetivo: proveer los prototipos de las funciones que usa el comando csorth
Funciones:
Fecha de última modificación: 12/09/19
*/

#include "procesamientoArchivo.h"
#include <stdbool.h>

registro* arreglo_temporales[MAXIMO_NUMERO_ARCHIVOS];
char arreglo_archivos_input[MAXIMO_NUMERO_ARCHIVOS][MAXIMO_NOMBRE_ARCHIVO];
int total_lineas;
int lineas_por_archivo[MAXIMO_NUMERO_ARCHIVOS];
bool bandera_orden_reverso;

void *ProcesarArchivo(void *thread_id);
void RepartirArchivosHilos(int numero_archivos_input);
void ContarTotalLineas(int numero_archivos_input);
registro* UnirRegistros(int numero_archivos_input);
void ImprimirResultado(registro* arreglo_general, char archivo_output[MAXIMO_NOMBRE_ARCHIVO]);
