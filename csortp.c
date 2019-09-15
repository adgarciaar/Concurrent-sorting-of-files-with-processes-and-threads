/*
Nombre del archivo: csortp.c
Autores: Adrián García y Luis Rosales
Objetivo: proveer la implementación para ejecutar el comando csortp
Funciones:
Fecha de última modificación: 12/09/19
*/

#include "csortp.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

/*
Función: RepartirArchivosProcesos
Autores de la función: Adrián García y Luis Rosales.
Parámetros de entrada: un arreglo con los nombres de los archivos de entrada,
un entero con el número de archivos de entrada y un booleano indicando si el
ordenamiento se debe realizar o no en orden reverso.
Retorno: no tiene.
Descripción: para cada archivo de entrada crea un proceso hijo (excepto si sólo
hay un archivo de entrada, caso en el que el padre se encarga) que se ocupa de
leer su respectivo archivo, ordenar los elementos de éste (usando Bubble Sort) e
 imprimir un archivo temporal con dichos elementos ordenados. Para ello, cada
 proceso realiza llamadas a las funciones encargadas de cada tarea. El proceso
 padre espera que todos los procesos hijos terminen.
*/
void RepartirArchivosProcesos(char array_archivos_input[][MAXIMO_NOMBRE_ARCHIVO],
  int numero_archivos_input, bool bandera_orden_reverso){

    int status, i,nprocesos=numero_archivos_input;
    pid_t childpid;
    registro* array_registros = NULL;
    int lineas_por_archivo[MAXIMO_NUMERO_ARCHIVOS];

    if(nprocesos == 1){

        lineas_por_archivo[0] = ContarLineasArchivo( array_archivos_input[ 0 ] );
        array_registros = LeerArchivo( array_archivos_input[0], lineas_por_archivo[0]);
        OrdenarRegistroPorBurbuja(array_registros, lineas_por_archivo[0], bandera_orden_reverso);
        ImprimirArchivo(array_registros, lineas_por_archivo[0], array_archivos_input[0], true);
        free(array_registros);
        array_registros = NULL;

    }else{

        for (i = 0; i < nprocesos; ++i) {
                if ((childpid = fork()) < 0) {
                    perror("fork:");
                    exit(1);
                }/*end if*/

                if (childpid == 0) {
                    printf("Inicia proceso hijo con pid %d\n", getpid());

                    lineas_por_archivo[i] = ContarLineasArchivo( array_archivos_input[ i ] );
                    array_registros = LeerArchivo( array_archivos_input[i], lineas_por_archivo[i]);
                    OrdenarRegistroPorBurbuja(array_registros, lineas_por_archivo[i], bandera_orden_reverso);
                    ImprimirArchivo(array_registros, lineas_por_archivo[i], array_archivos_input[i], true);
                    free(array_registros);
                    array_registros = NULL;

                    printf("Termina proceso hijo con pid %d\n", getpid());
                    exit(1);
                }/*end if*/
        }/*end for*/

        for (i = 0; i < nprocesos; ++i){
            wait(&status);
        }/*end for*/

        printf("Todos los procesos hijos han finalizado\n");
    }/*end if*/

}

/*
Función: ContarTotalLineasTemporales
Autores de la función: Adrián García.
Parámetros de entrada: un entero con el número de archivos de entrada,
un arreglo con los nombres de los archivos de entrada.
Retorno: un entero con el número de líneas totales de todos los archivos
temporales que fueron creados.
Descripción: cuenta las líneas de cada archivo temporales y suma éstas para
retornar la cantidad total de líneas de todos los archivos temporales.
*/
int ContarTotalLineasTemporales(int numero_archivos_input,
  char array_archivos_input[][MAXIMO_NOMBRE_ARCHIVO]){

    int total_lineas = 0;
    int numero_lineas_archivo;
    int i;
    char archivo_nombre[MAXIMO_NOMBRE_ARCHIVO];

    for (i = 0; i < numero_archivos_input; i++) {
        strcpy(archivo_nombre, array_archivos_input[i]);
        strcat(archivo_nombre,"_temporal");
        numero_lineas_archivo = ContarLineasArchivo( archivo_nombre );
        if(numero_lineas_archivo==0){
            BorrarTemporales(numero_archivos_input, array_archivos_input);
            exit(1);
        }/*end if*/
        total_lineas = total_lineas + numero_lineas_archivo;
    }/*end for*/
    return total_lineas;
}

/*
Función: LeerArchivosTemporales
Autores de la función: Adrián García.
Parámetros de entrada: un entero con el número de archivos de entrada,
un arreglo con los nombres de los archivos de entrada y
un entero con el número total de líneas de todos los archivos de entrada
Retorno: un apuntador a arreglo de tipo registro con la totalidad de
elementos contenidos en los archivos temporales (que guardan el ordenamiento
de cada archivo de entrada).
Descripción: lee todos los archivos temporales creados por los procesos hijos y
almacena todos los datos de éstos, en un arreglo de tipo registro.
*/
registro* LeerArchivosTemporales(int numero_archivos_input,
  char array_archivos_input[][MAXIMO_NOMBRE_ARCHIVO], int total_lineas){

    int i, j, k = 0;
    int numero_lineas_archivo;
    registro* array_registros = NULL;
    registro* array_temporales = (registro*)malloc(total_lineas*sizeof(registro));
    char archivo_nombre[MAXIMO_NOMBRE_ARCHIVO];

    if (array_temporales == NULL) {
        printf("Memory not allocated.\n");
        exit(1);
    }/*end if*/

    for (i = 0; i < numero_archivos_input; i++) {
        strcpy(archivo_nombre, array_archivos_input[i]);
        strcat(archivo_nombre,"_temporal");
        numero_lineas_archivo = ContarLineasArchivo( archivo_nombre );

        if(numero_lineas_archivo == 0){
            BorrarTemporales(numero_archivos_input, array_archivos_input);
            exit(1);
        }/*end if*/

        array_registros = LeerArchivo( archivo_nombre, numero_lineas_archivo);

        for (j = 0; j < numero_lineas_archivo; j++){
            strcpy(array_temporales[k].cadena, array_registros[j].cadena);
            array_temporales[k].tiempo_ejecucion = array_registros[j].tiempo_ejecucion;
            strcpy(array_temporales[k].fecha_ejecucion, array_registros[j].fecha_ejecucion);
            strcpy(array_temporales[k].hora_ejecucion, array_registros[j].hora_ejecucion);
            k = k + 1;
        }/*end for*/
        free(array_registros);
        array_registros = NULL;

    }/*end for*/

    return array_temporales;
}

/*
Función: BorrarTemporales
Autores de la función: Adrián García y Luis Rosales.
Parámetros de entrada: un entero con el número de archivos de entrada,
un arreglo con los nombres de los archivos de entrada.
Retorno: no tiene.
Descripción: borra los archivos temporales creados para almacenar
el ordenamiento de cada uno de los archivos de entrada.
*/
void BorrarTemporales(int numero_archivos_input,
  char array_archivos_input[][MAXIMO_NOMBRE_ARCHIVO]){
    int status;
    int i;
    char archivo_nombre[MAXIMO_NOMBRE_ARCHIVO];

    for (i=0; i<numero_archivos_input; i++){
        strcpy(archivo_nombre, array_archivos_input[i]);
        strcat(archivo_nombre,"_temporal");
        status = remove( archivo_nombre );
        if( status != 0 ){
            perror( archivo_nombre );
        }/*end if*/
    }/*end for*/

}

/*
Función: ImprimirResultado
Autores de la función: Luis Rosales.
Parámetros de entrada: un apuntador a arreglo de tipo registro que ya fue
ordenado, el nombre del archivo de salida y el total de líneas que éste va a
tener.
Retorno: no tiene.
Descripción: invoca la función que imprime los elementos del arreglo en el
archivo de salida especificado.
*/
void ImprimirResultado(registro* array_temporales,
  char archivo_output[MAXIMO_NOMBRE_ARCHIVO], int total_lineas){
    ImprimirArchivo(array_temporales, total_lineas, archivo_output, false);
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
cada archivo en los procesos hijos, se ordena un arreglo con la totalidad de los
elementos de todos los archivos de entrada usando Merge Sort (se llama a la
función encargada).
*/
int main (int argc, char **argv) {

    int numero_archivos_input = 0;
    int i;
    int resultado_comparacion_strings;
    bool bandera_orden_reverso = false;
    char array_archivos_input[MAXIMO_NUMERO_ARCHIVOS][MAXIMO_NOMBRE_ARCHIVO];
    char archivo_output[MAXIMO_NOMBRE_ARCHIVO];
    registro* array_temporales = NULL;

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
        printf("Error con numero de argumentos\n");
        exit(1);
    }/*end if*/
    if (bandera_orden_reverso == false && argc == 13){
        printf("Error con numero de argumentos\n");
        exit(1);
    }/*end if*/

    for (i = 1; i < argc-1; i++){
        resultado_comparacion_strings = strcmp("-r", argv[i]);
        if( resultado_comparacion_strings != 0 ){
            /*si argv[i] es diferente de -r entonces es archivo de entrada*/
            strcpy(array_archivos_input[numero_archivos_input], argv[i]);
            numero_archivos_input = numero_archivos_input+1;
        }/*end if*/
    }/*end for*/

    strcpy(archivo_output, argv[argc-1]);

    bool auxiliar;
    for (i = 0; i < numero_archivos_input; i++){
        auxiliar = AbrirArchivo(array_archivos_input[ i ]);
        if( auxiliar == false ){
            exit(1);
        }
    }

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
