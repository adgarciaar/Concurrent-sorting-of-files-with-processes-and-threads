#include "procesamientoArchivo.h"
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>

#include <limits.h>

#define maximo_nombre_archivo 50
#define maximo_numero_archivos 10

registro* array_temporales[maximo_numero_archivos];
char array_archivos_input[maximo_numero_archivos][maximo_nombre_archivo];
int total_lineas;
int lineas_por_archivo[maximo_numero_archivos];

void *ProcesarArchivo(void *thread_id);
void RepartirArchivosHilos(int numero_archivos_input);
registro* UnirRegistros(int numero_archivos_input);
void ImprimirResultado(registro* array_general, char archivo_output[maximo_nombre_archivo], bool bandera_orden_reverso);
void ContarTotalLineas(int numero_archivos_input);

int main (int argc, char **argv) {

    int numero_archivos_input = 0; /*j para manejar la posición en el array de archivos input*/
    int i;
    int resultado_comparacion_strings;
    bool bandera_orden_reverso = false;

    char archivo_output[maximo_nombre_archivo];
    registro* array_temporales = NULL;
    registro* array_general = NULL;

    /*Si hay 13 argumentos significa que se pasaron: default, flag, 10 archivos de input y 1 de output*/
    /*Debe haber mínimo 3 argumentos: default, input file & output file*/
    if(argc>13 || argc<3){
        /*No pueden haber más de 13 argumentos (máximo)*/
        /*No puede haber menos de 3 argumentos (3 mínimo)*/
        printf("Error con numero de argumentos\n");
        exit(1);
    }

    /*
    for (i = 0; i < argc; i++){
        printf("%s\n", argv[i]);
    }*/
    resultado_comparacion_strings = strcmp("-r", argv[1]);
    if( resultado_comparacion_strings == 0 ){
        bandera_orden_reverso = true;
    }
    if (bandera_orden_reverso == true && argc == 3){
        /*Si se ingresó flag -r pero sólo hay 3 argumentos significa que falta el archivo output*/
        printf("Error con numero de argumentos\n");
        exit(1);
    }
    if (bandera_orden_reverso == false && argc == 13){
        /*Si no se ingresó flag -r y hay 13 argumentos significa que se introdujo un archivo de más*/
        printf("Error con numero de argumentos\n");
        exit(1);
    }

    for (i = 1; i < argc-1; i++){
        resultado_comparacion_strings = strcmp("-r", argv[i]);
        if( resultado_comparacion_strings != 0 ){
            /*si argv[i] es diferente de -r entonces es archivo de entrada*/
            strcpy(array_archivos_input[numero_archivos_input], argv[i]);
            /*printf("%s\n", array_archivos_input[i]);*/
            /*printf("%s\n", array_archivos_input[numero_archivos_input]);*/
            numero_archivos_input = numero_archivos_input+1;
        }
    }

    /*for (i = 0; i < numero_archivos_input; i++){
        printf("%s\n", array_archivos_input[i]);
    }*/

    strcpy(archivo_output, argv[argc-1]);

    total_lineas = 0;

    RepartirArchivosHilos(numero_archivos_input);
    ContarTotalLineas(numero_archivos_input);
    array_general = UnirRegistros(numero_archivos_input);
    OrdenarRegistroPorMergeSort(array_general, total_lineas);
    ImprimirResultado(array_general, archivo_output, bandera_orden_reverso);

    free(array_general);
    array_general = NULL;

    return(0);
}

void *ProcesarArchivo(void *thread_id){

    int *id_ptr;
    int tarea_id;
    /*int numero_lineas_archivo;*/

    id_ptr = (int *) thread_id;
    tarea_id = *id_ptr;
    printf("Thread número %d\n", tarea_id+1);

    lineas_por_archivo[tarea_id] = ContarLineasArchivo( array_archivos_input[ tarea_id ] );
    /*numero_lineas_archivo = ContarLineasArchivo( array_archivos_input[ tarea_id ] );*/

    /*total_lineas = total_lineas + numero_lineas_archivo;*/

    /*array_temporales[ tarea_id ] = LeerArchivo( array_archivos_input[ tarea_id ], numero_lineas_archivo);*/
    array_temporales[ tarea_id ] = LeerArchivo( array_archivos_input[ tarea_id ], lineas_por_archivo[tarea_id] );

    /*OrdenarRegistroPorBurbuja(array_temporales[ tarea_id ], numero_lineas_archivo);*/
    OrdenarRegistroPorBurbuja(array_temporales[ tarea_id ], lineas_por_archivo[tarea_id]);

    /*QUITAR ESTO AHORITA*/
    /*ImprimirArchivo(array_temporales[ tarea_id ], numero_lineas_archivo, array_archivos_input[ tarea_id ], false, true);*/
    ImprimirArchivo(array_temporales[ tarea_id ], lineas_por_archivo[tarea_id], array_archivos_input[ tarea_id ], false, true);

    printf("Termina hilo %d\n", tarea_id+1);
    pthread_exit(NULL);
}

void RepartirArchivosHilos(int numero_archivos_input){

    pthread_t threads[numero_archivos_input];
    int tarea_ids[numero_archivos_input];
    int rc, i;

    for(i=0; i<maximo_numero_archivos; i++){
        array_temporales[i] = NULL;
    }

    for(i=0; i<numero_archivos_input; i++){
        tarea_ids[i] = i;
        rc = pthread_create(&threads[i], NULL, ProcesarArchivo, (void *) &tarea_ids[i]);
        if (rc) {
            perror("Error creando un thread");
            exit(-1);
        }
    }

    for(i=0; i<numero_archivos_input; i++){
        pthread_join(threads[i], NULL);
    }
    printf("Todos los hilos han finalizado\n");
    /*pthread_exit(NULL);*/
}

void ContarTotalLineas(int numero_archivos_input){
    int i;
    for(i=0; i<numero_archivos_input; i++){
        total_lineas = total_lineas + lineas_por_archivo[i];
    }
}

registro* UnirRegistros(int numero_archivos_input){

    int i, j, k=0;
    int numero_lineas_archivo;
    registro* array_general = (registro*)malloc(total_lineas*sizeof(registro));
    if (array_general == NULL) {
        printf("Memory not allocated.\n");
        exit(1);
    }
    registro* array_auxiliar = NULL;

    for(i=0; i<numero_archivos_input; i++){

        numero_lineas_archivo = ContarLineasArchivo( array_archivos_input[i] );

        array_auxiliar = array_temporales[i];

        for (j = 0; j < numero_lineas_archivo; j++){

            strcpy(array_general[k].cadena, array_auxiliar[j].cadena);
            array_general[k].tiempo_ejecucion = array_auxiliar[j].tiempo_ejecucion;
            strcpy(array_general[k].fecha_ejecucion, array_auxiliar[j].fecha_ejecucion);
            strcpy(array_general[k].hora_ejecucion, array_auxiliar[j].hora_ejecucion);
            k = k + 1;

        }

        array_auxiliar = NULL;
        free(array_temporales[i]);
        array_temporales[i] = NULL;
    }

    return array_general;

}

void ImprimirResultado(registro* array_general, char archivo_output[maximo_nombre_archivo], bool bandera_orden_reverso){
    ImprimirArchivo(array_general, total_lineas, archivo_output, bandera_orden_reverso, false);
}
