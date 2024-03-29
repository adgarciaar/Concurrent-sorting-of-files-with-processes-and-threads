/*
Nombre del archivo: csorth.c
Autores: Adrián García y Luis Rosales
Objetivo: proveer la implementación para ejecutar el comando csorth
Funciones: ProcesarArchivo, RepartirArchivosHilos, ContarTotalLineas,
UnirRegistros, ImprimirResultado.
Fecha de última modificación: 14/09/19
*/

#include "csorth.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <limits.h>

/*
Función: ProcesarArchivo
Autores de la función: Adrián García y Luis Rosales.
Parámetros de entrada: un identificador que corresponde al número del hilo
(desde 0 hasta 9).
Retorno: no tiene.
Descripción: es la función que se ejecuta en cada hilo creado. Se realiza lectura
del archivo de entrada correspondiente y luego se realiza su ordenamiento usando
Bubble Sort (ordenamiento que queda guardado en la variable global arreglo_temporales).
Variables globales que usa: arreglo_temporales, lineas_por_archivo, bandera_orden_reverso
*/
void *ProcesarArchivo(void *thread_id){

    int *id_ptr;
    int tarea_id;

    id_ptr = (int *) thread_id;
    tarea_id = *id_ptr;
    printf("Inicia hilo %d\n", tarea_id+1);

    lineas_por_archivo[ tarea_id ] = ContarLineasArchivo( arreglo_archivos_input[ tarea_id ] );
    arreglo_temporales[ tarea_id ] = LeerArchivo( arreglo_archivos_input[ tarea_id ], lineas_por_archivo[ tarea_id ] );

    OrdenarRegistroPorBurbuja(arreglo_temporales[ tarea_id ], lineas_por_archivo[tarea_id], bandera_orden_reverso);

    printf("Termina hilo %d\n", tarea_id+1);

    pthread_exit(NULL);
}

/*
Función: RepartirArchivosHilos
Autores de la función: Adrián García y Luis Rosales.
Parámetros de entrada: un entero con el número de archivos de entrada.
Retorno: no tiene.
Descripción: crea tantos hilos como archivos de entrada se reciben, asignándose
a cada uno la ejecución de la función ProcesarArchivo enviando como parámetro
el identificador del hilo. El proceso padre espera a que todos los hilos terminen.
Variables globales que usa: arreglo_temporales, arreglo_archivos_input, lineas_por_archivo
*/
void RepartirArchivosHilos(int numero_archivos_input){

    pthread_t threads[numero_archivos_input];
    int tarea_ids[numero_archivos_input];
    int rc, i;

    for(i=0; i<MAXIMO_NUMERO_ARCHIVOS; i++){
        arreglo_temporales[i] = NULL;
    }/*end for*/

    for(i=0; i<numero_archivos_input; i++){
        tarea_ids[i] = i;
        rc = pthread_create(&threads[i], NULL, ProcesarArchivo, (void *) &tarea_ids[i]);
        if (rc) {
            perror("Error creando un thread");
            exit(-1);
        }/*end if*/
    }/*end for*/

    for(i=0; i<numero_archivos_input; i++){
        pthread_join(threads[i], NULL);
    }/*end if*/
    printf("Todos los hilos han finalizado\n");

}

/*
Función: ContarTotalLineas
Autores de la función: Adrián García
Parámetros de entrada: un entero con el número de archivos de entrada.
Retorno: no tiene.
Descripción: cuenta el número total de líneas de todos los archivos de entrada,
usando el arreglo global lineas_por_archivo que contiene el número de líneas
de cada uno de los archivos.
Variables globales que usa: lineas_por_archivo, total_lineas
*/
void ContarTotalLineas(int numero_archivos_input){
    int i;
    for(i=0; i<numero_archivos_input; i++){
        total_lineas = total_lineas + lineas_por_archivo[i];
    }/*end for*/
}

/*
Función: UnirRegistros
Autores de la función: Adrián García.
Parámetros de entrada:
Retorno: un apuntador a arreglo de tipo registro con la totalidad de
elementos de todos los archivos de entrada (ya individualmente ordenados).
Descripción: usa el arreglo global arreglo_temporales para almacenar en un nuevo
arreglo la totalidad de elementos de todos los archivos de entrada (ya
individualmente ordenados), y retorna este último arreglo.
Variables globales que usa: arreglo_archivos_input, arreglo_temporales
*/
registro* UnirRegistros(int numero_archivos_input){

    int i, j, k=0;
    int numero_lineas_archivo;
    registro* arreglo_general = (registro*)malloc(total_lineas*sizeof(registro));
    if (arreglo_general == NULL) {
        perror("Memoria no alocada");
        exit(1);
    }/*end if*/
    registro* arreglo_auxiliar = NULL;

    for(i=0; i<numero_archivos_input; i++){

        numero_lineas_archivo = ContarLineasArchivo( arreglo_archivos_input[i] );

        arreglo_auxiliar = arreglo_temporales[i];

        for (j = 0; j < numero_lineas_archivo; j++){

            strcpy(arreglo_general[k].cadena, arreglo_auxiliar[j].cadena);
            arreglo_general[k].tiempo_ejecucion = arreglo_auxiliar[j].tiempo_ejecucion;
            strcpy(arreglo_general[k].fecha_ejecucion, arreglo_auxiliar[j].fecha_ejecucion);
            strcpy(arreglo_general[k].hora_ejecucion, arreglo_auxiliar[j].hora_ejecucion);
            k = k + 1;

        }/*end for*/

        arreglo_auxiliar = NULL;
        free(arreglo_temporales[i]);
        arreglo_temporales[i] = NULL;
    }/*end for*/

    return arreglo_general;

}

/*
Función: ImprimirResultado
Autores de la función: Luis Rosales.
Parámetros de entrada: un apuntador a arreglo de tipo registro que ya fue
ordenado y el nombre del archivo de salida.
Retorno: no tiene.
Descripción: invoca la función que imprime los elementos del arreglo en el
archivo de salida especificado.
*/
void ImprimirResultado(registro* arreglo_general,
  char archivo_output[MAXIMO_NOMBRE_ARCHIVO]){
    ImprimirArchivo(arreglo_general, total_lineas, archivo_output, false);
}

/*
Función: main
Autores de la función: Adrián García y Luis Rosales.
Parámetros de entrada: un entero con el número de argumentos recibidos por
consola y un arreglo de los argumentos recibidos.
Retorno: un entero indicando si el programa finaliza.
Descripción: es la función que inicia el programa, recibiendo los argumentos
introducidos por consola, validándolos y llamando las otras funciones para
realizar el ordenamiento. Una vez se ha realizado el ordenamiento individual de
cada archivo en los hilos, se ordena un arreglo con la totalidad de los
elementos de todos los archivos de entrada usando Merge Sort (se llama a la
función encargada).
*/
int main (int argc, char **argv) {

    int numero_archivos_input = 0;
    int i;
    int resultado_comparacion_strings;
    bandera_orden_reverso = false;

    char archivo_output[MAXIMO_NOMBRE_ARCHIVO];
    registro* arreglo_temporales = NULL;
    registro* arreglo_general = NULL;

    /*Si hay 13 argumentos significa que se pasaron: default, flag, 10 archivos de input y 1 de output*/
    /*Debe haber mínimo 3 argumentos: default, input file & output file*/
    if(argc>13 || argc<3){
        /*No pueden haber más de 13 argumentos (máximo)*/
        /*No puede haber menos de 3 argumentos (3 mínimo)*/
        printf("Error con numero de argumentos\n");
        exit(1);
    }/*end if*/

    resultado_comparacion_strings = strcmp("-r", argv[1]);
    if( resultado_comparacion_strings == 0 ){
        bandera_orden_reverso = true;
    }/*end if*/
    if (bandera_orden_reverso == true && argc == 3){
        /*Si se ingresó flag -r pero sólo hay 3 argumentos significa que falta el archivo output*/
        printf("Error con numero de argumentos\n");
        exit(1);
    }/*end if*/
    if (bandera_orden_reverso == false && argc == 13){
        /*Si no se ingresó flag -r y hay 13 argumentos significa que se introdujo un archivo de más*/
        printf("Error con numero de argumentos\n");
        exit(1);
    }/*end if*/

    for (i = 1; i < argc-1; i++){
        resultado_comparacion_strings = strcmp("-r", argv[i]);
        if( resultado_comparacion_strings != 0 ){
            /*si argv[i] es diferente de -r entonces es archivo de entrada*/
            strcpy(arreglo_archivos_input[numero_archivos_input], argv[i]);
            numero_archivos_input = numero_archivos_input+1;
        }/*end if*/
    }/*end for*/

    strcpy(archivo_output, argv[argc-1]);

    bool auxiliar;
    for (i = 0; i < numero_archivos_input; i++){
        auxiliar = AbrirArchivo(arreglo_archivos_input[ i ]);
        if( auxiliar == false ){
            exit(1);
        }/*end if*/
    }/*end for*/

    total_lineas = 0;

    RepartirArchivosHilos(numero_archivos_input);
    ContarTotalLineas(numero_archivos_input);
    arreglo_general = UnirRegistros(numero_archivos_input);
    OrdenarRegistroPorMergeSort(arreglo_general, total_lineas, bandera_orden_reverso);
    ImprimirResultado(arreglo_general, archivo_output);

    free(arreglo_general);
    arreglo_general = NULL;

    return(0);
}
