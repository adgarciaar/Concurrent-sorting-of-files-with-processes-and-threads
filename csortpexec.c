#include "procesamientoArchivo.h"
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

void RepartirArchivosProcesos(char array_archivos_input[][maximo_nombre_archivo], int numero_archivos_input, bool bandera_orden_reverso);
registro* LeerArchivosTemporales(int numero_archivos_input, char array_archivos_input[][maximo_nombre_archivo], int total_lineas);
int ContarTotalLineasTemporales(int numero_archivos_input, char array_archivos_input[][maximo_nombre_archivo]);
void ImprimirResultado( registro* array_temporales, char archivo_output[maximo_nombre_archivo], int total_lineas );
void BorrarTemporales(int numero_archivos_input, char array_archivos_input[][maximo_nombre_archivo]);

int main (int argc, char **argv) {

    int numero_archivos_input = 0; /*j para manejar la posición en el array de archivos input*/
    int i;
    int resultado_comparacion_strings;
    bool bandera_orden_reverso = false;
    char array_archivos_input[maximo_numero_archivos][maximo_nombre_archivo];
    char archivo_output[maximo_nombre_archivo];
    registro* array_temporales = NULL;

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

    int total_lineas = ContarTotalLineasTemporales( numero_archivos_input, array_archivos_input );
    array_temporales =  LeerArchivosTemporales( numero_archivos_input, array_archivos_input, total_lineas );
    BorrarTemporales( numero_archivos_input, array_archivos_input );
    OrdenarRegistroPorMergeSort(array_temporales, total_lineas, bandera_orden_reverso);
    ImprimirResultado( array_temporales, archivo_output, total_lineas );

    free(array_temporales);
    array_temporales = NULL;

    return(0);
}

void RepartirArchivosProcesos(char array_archivos_input[][maximo_nombre_archivo], int numero_archivos_input, bool bandera_orden_reverso){

    int status, i,nprocesos=numero_archivos_input;
    pid_t childpid;
    int confirmacion;
    int numero_lineas_archivo;

    char *argumentos[4] = {
        "/bin/bash",
        "-c",
        "",
        NULL
    };

    char comando[300] = "sort";
    char nombre_temporal[200];

    if(bandera_orden_reverso == true){
        strcat(comando, " -r");
    }

    strcat(comando, " -b -V -k4 -k5.1,5.8 -k6.1,6.8 ");

    /*int numero_lineas_archivo;*/
    /*registro* array_registros = NULL;*/
    /*int lineas_por_archivo[maximo_numero_archivos];*/

    /*Validar que los archivos existan*/
    for(i=0; i<numero_archivos_input; i++){
        numero_lineas_archivo = ContarLineasArchivo( array_archivos_input[ i ] );
        if (numero_lineas_archivo == 0){
            exit(1);
        }
    }

    for (i = 0; i < nprocesos; ++i) {
          if ((childpid = fork()) < 0) {
                perror("fork:");
                exit(1);
          }
                /* Codigo que ejecutaran los hijos */
          if (childpid == 0) {
              printf("Inicia proceso hijo con pid %d\n", getpid());

              /*array_registros = LeerArchivo( array_archivos_input[i], lineas_por_archivo[i]);
              OrdenarRegistroPorBurbuja(array_registros, lineas_por_archivo[i]);
              ImprimirArchivo(array_registros, lineas_por_archivo[i], array_archivos_input[i], false, true);

              free(array_registros);
              array_registros = NULL;*/

              strcat(comando, array_archivos_input[i]);
              strcat(comando, " > ");

              strcpy(nombre_temporal, array_archivos_input[i]);
              strcat(nombre_temporal, "_temporal");

              strcat(comando, nombre_temporal);

              argumentos[2] = comando;

              confirmacion = execvp(argumentos[0], argumentos);
              /* If the child process reaches this point, then */
              /* execvp must have failed. */
              /*fprintf(stderr, "Child process could not do execvp.\n");*/
              if (confirmacion == -1){
                  perror("No ejecutado");
                  exit(1);
              }

              printf("Termina proceso hijo con pid %d\n", getpid());
              exit(1);
          }
    }
        /* El padre espera por los hijos */
    for (i = 0; i < nprocesos; ++i){
        wait(&status);
    }

    printf("Todos los procesos hijos han finalizado\n");

}

int ContarTotalLineasTemporales(int numero_archivos_input, char array_archivos_input[][maximo_nombre_archivo]){

    int total_lineas = 0;
    int numero_lineas_archivo;
    int i;
    char archivo_nombre[maximo_nombre_archivo];

    for (i = 0; i < numero_archivos_input; i++) {
        strcpy(archivo_nombre, array_archivos_input[i]);
        strcat(archivo_nombre,"_temporal");
        /*printf("EN contar lineas: %s\n", archivo_nombre);*/
        numero_lineas_archivo = ContarLineasArchivo( archivo_nombre );
        if(numero_lineas_archivo==0){
            BorrarTemporales(numero_archivos_input, array_archivos_input);
            exit(1);
        }
        total_lineas = total_lineas + numero_lineas_archivo;
    }
    /*printf("Total lineas: %d\n", total_lineas);*/
    return total_lineas;
}

registro* LeerArchivosTemporales(int numero_archivos_input, char array_archivos_input[][maximo_nombre_archivo], int total_lineas){

    int i, j, k = 0;
    int numero_lineas_archivo;
    registro* array_registros = NULL;
    registro* array_temporales = (registro*)malloc(total_lineas*sizeof(registro));
    char archivo_nombre[maximo_nombre_archivo];

    if (array_temporales == NULL) {
        printf("Memory not allocated.\n");
        exit(1);
    }

    for (i = 0; i < numero_archivos_input; i++) {
        strcpy(archivo_nombre, array_archivos_input[i]);
        strcat(archivo_nombre,"_temporal");
        /*printf("EN leer temporales: %s\n", archivo_nombre);*/
        numero_lineas_archivo = ContarLineasArchivo( archivo_nombre );

        if(numero_lineas_archivo == 0){
            /*printf("%s\n", "Problema leyendo archivos temporales.");*/
            BorrarTemporales(numero_archivos_input, array_archivos_input);
            exit(1);
        }

        array_registros = LeerArchivo( archivo_nombre, numero_lineas_archivo);

        for (j = 0; j < numero_lineas_archivo; j++){
            strcpy(array_temporales[k].cadena, array_registros[j].cadena);
            array_temporales[k].tiempo_ejecucion = array_registros[j].tiempo_ejecucion;
            strcpy(array_temporales[k].fecha_ejecucion, array_registros[j].fecha_ejecucion);
            strcpy(array_temporales[k].hora_ejecucion, array_registros[j].hora_ejecucion);
            k = k + 1;
        }
        free(array_registros);
        array_registros = NULL;

    }

    return array_temporales;
}

void BorrarTemporales(int numero_archivos_input, char array_archivos_input[][maximo_nombre_archivo]){
    int status;
    int i;
    char archivo_nombre[maximo_nombre_archivo];

    for (i=0; i<numero_archivos_input; i++){
        strcpy(archivo_nombre, array_archivos_input[i]);
        strcat(archivo_nombre,"_temporal");
        status = remove( archivo_nombre );
        if( status != 0 ){
            perror( archivo_nombre );
        }
    }

}

void ImprimirResultado(registro* array_temporales, char archivo_output[maximo_nombre_archivo], int total_lineas){

    ImprimirArchivo(array_temporales, total_lineas, archivo_output, false);

}
