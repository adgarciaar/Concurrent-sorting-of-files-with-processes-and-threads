#include "procesamientoArchivo.h"
#include <stdbool.h>

registro* array_temporales[maximo_numero_archivos];
char array_archivos_input[maximo_numero_archivos][maximo_nombre_archivo];
int total_lineas;
int lineas_por_archivo[maximo_numero_archivos];
bool bandera_orden_reverso;

void *ProcesarArchivo(void *thread_id);
void RepartirArchivosHilos(int numero_archivos_input);
registro* UnirRegistros(int numero_archivos_input);
void ImprimirResultado(registro* array_general, char archivo_output[maximo_nombre_archivo]);
void ContarTotalLineas(int numero_archivos_input);
