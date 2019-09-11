#include <stdbool.h>

#define max_tamano_cadena 255
#define tamano_fila_fecha_hora 200

typedef struct {
  char cadena[max_tamano_cadena];
  int tiempo_ejecucion;
  char fecha_ejecucion[tamano_fila_fecha_hora];
  char hora_ejecucion[tamano_fila_fecha_hora];
} registro ;

registro* LeerArchivo(char nombre_archivo[], int numero_lineas_archivo);
int ContarLineasArchivo(char nombre_archivo[]);
void OrdenarRegistroPorBurbuja(registro* array_registros, int numero_elementos);
void OrdenarRegistroPorMergeSort(registro* array_registros, int numero_elementos);
void ImprimirArchivo(registro* array_registros, int numero_elementos, char nombre_archivo[], bool bandera_orden_reverso, bool temporal);
void mergeSort(registro* array_registros, int l, int r);
void merge(registro* array_registros, int l, int m, int r);
