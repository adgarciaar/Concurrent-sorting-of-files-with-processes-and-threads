#include "procesamientoArchivo.h"
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define maximo_nombre_archivo 40
#define maximo_numero_archivos 10

void RepartirArchivosProcesos(char array_archivos_input[][maximo_nombre_archivo], int numero_archivos_input, bool bandera_orden_reverso);

int main (int argc, char **argv) {

    int numero_archivos_input = 0; /*j para manejar la posición en el array de archivos input*/
    int i;
    int resultado_comparacion_strings;
    bool bandera_orden_reverso = false;
    char array_archivos_input[maximo_numero_archivos][maximo_nombre_archivo];
    char archivo_output[maximo_nombre_archivo];

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
            numero_archivos_input = numero_archivos_input+1;
        }
    }

    /*for (i = 0; i < numero_archivos_input; i++){
        printf("%s\n", array_archivos_input[i]);
    }*/

    strcpy(archivo_output, argv[argc-1]);
    /*printf("%s\n", archivo_output);*/

    RepartirArchivosProcesos(array_archivos_input, numero_archivos_input, bandera_orden_reverso);

    return(0);
}

void RepartirArchivosProcesos(char array_archivos_input[][maximo_nombre_archivo], int numero_archivos_input, bool bandera_orden_reverso){

    int status, i,nprocesos=numero_archivos_input;
    pid_t childpid;
    int numero_lineas_archivo;
    registro* array_registros = NULL;

    if(nprocesos == 1){
        /*ProcesarArchivo(array_archivos_input[0], bandera_orden_reverso);*/

        numero_lineas_archivo = ContarLineasArchivo( array_archivos_input[0] );
        array_registros = LeerArchivo( array_archivos_input[0], numero_lineas_archivo);
        OrdenarRegistro(array_registros, numero_lineas_archivo);
        ImprimirArchivoTemporal(array_registros, numero_lineas_archivo, array_archivos_input[0], bandera_orden_reverso);
        free(array_registros);
        array_registros = NULL;

    }else{

        for (i = 0; i < nprocesos; ++i) {
                if ((childpid = fork()) < 0) {
                    perror("fork:");
                    exit(1);
                }
                /* Codigo que ejecutaran los hijos */
                if (childpid == 0) {
                    printf("Proceso hijo con pid %d\n", getpid());

                    numero_lineas_archivo = ContarLineasArchivo( array_archivos_input[i] );
                    array_registros = LeerArchivo( array_archivos_input[i], numero_lineas_archivo);
                    OrdenarRegistro(array_registros, numero_lineas_archivo);
                    ImprimirArchivoTemporal(array_registros, numero_lineas_archivo, array_archivos_input[i], bandera_orden_reverso);
                    free(array_registros);
                    array_registros = NULL;

                    /*ProcesarArchivo(array_archivos_input[i], bandera_orden_reverso);*/
                    printf("Termina proceso %d\n", getpid());
                    exit(1);
                }
        }
        /* El padre espera por los hijos */
        for (i = 0; i < nprocesos; ++i){
            wait(&status);
        }

        printf("El padre termina\n");
    }

}

registro LeerArchivosTemporales(int numero_archivos_input, char archivo_output[]){
    /*Revisar el # de líneas de cada archivo*/

    /*Hallar el total de líneas de todo*/

    /*registro* array_registros_temporales = (registro*)malloc(total_lineas*sizeof(registro));*/

    /*Abrir cada archivo e ir llenando el array*/

    /*int i = 0;
    for (i = 0; i < numero_archivos_input; ++i) {

    }*/


}
