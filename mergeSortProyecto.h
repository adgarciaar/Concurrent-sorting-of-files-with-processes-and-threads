#define max_tamano_cadena 255
#define tamano_fila_fecha_hora 9

typedef struct {
  char cadena[max_tamano_cadena];
  int tiempo_ejecucion;
  char fecha_ejecucion[tamano_fila_fecha_hora];
  char hora_ejecucion[tamano_fila_fecha_hora];
} registro ;

int contarLineasArchivo(char nombre_archivo[]);
void ordenarRegistro(registro* array_registros, int numero_elementos);
void ImprimirArchivoTemporal(registro* array_registros, int numero_elementos, char nombre_archivo[]);
void mergeSort(registro* array_registros, int l, int r);
void merge(registro* array_registros, int l, int m, int r);
