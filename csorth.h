#include "procesamientoArchivo.h"
#include <stdbool.h>

registro* array_temporales[MAXIMO_NUMERO_ARCHIVOS];
char array_archivos_input[MAXIMO_NUMERO_ARCHIVOS][MAXIMO_NOMBRE_ARCHIVO];
int total_lineas;
int lineas_por_archivo[MAXIMO_NUMERO_ARCHIVOS];
bool bandera_orden_reverso;

/*Usa las variables globales: array_temporales, lineas_por_archivo, bandera_orden_reverso*/
void *ProcesarArchivo(void *thread_id);
/*Usa las variables globales: array_temporales, array_archivos_input, lineas_por_archivo*/
void RepartirArchivosHilos(int numero_archivos_input);
/*Usa la variable global: lineas_por_archivo*/
void ContarTotalLineas(int numero_archivos_input);
/*Usa la variable global: array_archivos_input, array_temporales*/
registro* UnirRegistros(int numero_archivos_input);
void ImprimirResultado(registro* array_general, char archivo_output[MAXIMO_NOMBRE_ARCHIVO]);
