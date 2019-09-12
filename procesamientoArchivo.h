#include <stdbool.h>

#define MAX_TAMANO_CADENA 255
#define TAMANO_FILA_FECHA_HORA 9

#define MAXIMO_NOMBRE_ARCHIVO 200
#define MAXIMO_NUMERO_ARCHIVOS 10

typedef struct {
  char cadena[MAX_TAMANO_CADENA];
  int tiempo_ejecucion;
  char fecha_ejecucion[TAMANO_FILA_FECHA_HORA];
  char hora_ejecucion[TAMANO_FILA_FECHA_HORA];
} registro ;

registro* LeerArchivo(char nombre_archivo[], int numero_lineas_archivo);
int ContarLineasArchivo(char nombre_archivo[]);
void OrdenarRegistroPorBurbuja(registro* array_registros, int numero_elementos, bool bandera_orden_reverso);
void OrdenarRegistroPorMergeSort(registro* array_registros, int numero_elementos, bool bandera_orden_reverso);
void ImprimirArchivo(registro* array_registros, int numero_elementos, char nombre_archivo[], bool temporal);
void mergeSort(registro* array_registros, int l, int r, bool bandera_orden_reverso);
void merge(registro* array_registros, int l, int m, int r, bool bandera_orden_reverso);
