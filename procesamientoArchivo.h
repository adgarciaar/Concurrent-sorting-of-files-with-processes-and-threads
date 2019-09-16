/*
Nombre del archivo: procesamientoArchivo.h
Autores: Adrián García y Luis Rosales
Objetivo: proveer los prototipos de las funciones que procesan cada archivo de
entrada, definir constantes que se usan para la declaración de varios arreglos
y definir la estructura "registro" la cual permite almacenar cada fila de un
archivo de entrada (log).
Funciones: LeerArchivo, ContarLineasArchivo, IntercambiarElementosArreglo,
OrdenarRegistroPorBurbuja, OrdenarRegistroPorMergeSort, ImprimirArchivo,
MergeSort, CopiarElementoArreglo, Merge, AbrirArchivo.
Fecha de última modificación: 12/09/19
*/

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
void IntercambiarElementosArreglo(registro* arreglo_registros, int j);
void OrdenarRegistroPorBurbuja(registro* arreglo_registros, int numero_elementos, bool bandera_orden_reverso);
void OrdenarRegistroPorMergeSort(registro* arreglo_registros, int numero_elementos, bool bandera_orden_reverso);
void ImprimirArchivo(registro* arreglo_registros, int numero_elementos, char nombre_archivo[], bool temporal);
void MergeSort(registro* arreglo_registros, int l, int r, bool bandera_orden_reverso);
void CopiarElementoArreglo(registro* arreglo1, registro* arreglo2, int posicion1, int posicion2);
void Merge(registro* arreglo_registros, int l, int m, int r, bool bandera_orden_reverso);
bool AbrirArchivo(char nombre_archivo[]);
