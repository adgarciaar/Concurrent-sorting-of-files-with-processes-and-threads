#define maxNombreArchivo 100
#define maxNumeroRegistros 20000
#define maxTamanoCadena 255
#define tamanoFilaFechaHora 9

typedef struct {
  char cadena[maxTamanoCadena];
  int tiempoEjecucion;
  char fechaEjecucion[tamanoFilaFechaHora];
  char horaEjecucion[tamanoFilaFechaHora];
} registro ;

int contarLineasArchivo(char filename[]);
void ordenarRegistro(registro* record, int numeroElementos);
void ImprimirArchivoTemporal(registro* record, int numeroElementos, char filename[]);
