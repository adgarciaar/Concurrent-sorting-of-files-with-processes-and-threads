/*
Nombre del archivo: procesamientoArchivo.c
Autores: Adrián García y Luis Rosales
Objetivo: proveer la implementación para procesar cada archivo de entrada en
los comandos csortp, csorth y csortpexec.
Funciones:
Fecha de última modificación: 12/09/19
*/

#include "procesamientoArchivo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*
Función:
Autores de la función:
Parámetros de entrada:
Retorno:
Descripción:
*/
registro* LeerArchivo(char nombre_archivo[], int numero_lineas_archivo){

    FILE *archivo;
    char linea[MAX_TAMANO_CADENA];
    char linea_aux[MAX_TAMANO_CADENA];
    char delimitador[] = " ";
    int contador = 0;
    int numero_linea = 0;
    int i = 0;
    char *item;
    char *caracter_aux;
    registro* array_registros = (registro*)malloc(numero_lineas_archivo*sizeof(registro));

    if (array_registros == NULL) {
        printf("Memoria no alocada para las filas del log.\n");
        exit(1);
    }/*end if*/

    archivo = fopen( nombre_archivo ,"r");

    if ( archivo != NULL ){

      while (fgets(linea,sizeof(linea),archivo)) {
          strcpy(array_registros[numero_linea].cadena, linea);
          numero_linea = numero_linea + 1;
      }/*end while*/
      fclose(archivo);

    }else{
       free(array_registros);
       perror ( nombre_archivo );
       exit(1);
    }/*end if*/


    for (i = 0; i < numero_lineas_archivo; i++){

        strcpy(linea_aux, array_registros[i].cadena);

        if( (caracter_aux = strchr(linea_aux, '\n')) != NULL){
            *caracter_aux = '\0';
        }/*end if*/

        item = strtok(linea_aux, delimitador);
        contador = 1;
        while(item != NULL){
            if (contador == 4){
                array_registros[i].tiempo_ejecucion = atoi(item);
            }/*end if*/
            if (contador == 5){
                strcpy(array_registros[i].fecha_ejecucion,item);
            }/*end if*/
            if (contador == 6){
                strcpy(array_registros[i].hora_ejecucion,item);
                item = NULL;
            }/*end if*/
            contador = contador+1;
            item = strtok(NULL, delimitador);
        }/*end while*/

    }/*end for*/

    return array_registros;

}

/*
Función:
Autores de la función:
Parámetros de entrada:
Retorno:
Descripción:
*/
int ContarLineasArchivo(char nombre_archivo[]){
    FILE *archivo;
    int contador_lineas = 0;
    char caracter;

    archivo = fopen(nombre_archivo, "r");

    if (archivo == NULL){
        perror ( nombre_archivo );
        return 0;
    }/*end if*/

    /* Extract characters from file and store in character c */
    for (caracter = getc(archivo); caracter != EOF; caracter = getc(archivo)){
        if (caracter == '\n'){
            contador_lineas = contador_lineas + 1;
        }/*end if*/
    }/*end for*/

    fclose(archivo);

    return contador_lineas;
}

/*
Función:
Autores de la función:
Parámetros de entrada:
Retorno:
Descripción:
*/
void IntercambiarElementosArreglo(registro* array_registros, int j){

    registro registro_auxiliar;

    /*Salvar los datos del primero*/
    strcpy(registro_auxiliar.cadena, array_registros[j+1].cadena);
    registro_auxiliar.tiempo_ejecucion = array_registros[j+1].tiempo_ejecucion;
    strcpy(registro_auxiliar.fecha_ejecucion, array_registros[j+1].fecha_ejecucion);
    strcpy(registro_auxiliar.hora_ejecucion, array_registros[j+1].hora_ejecucion);

    /*Intercambiar un elemento*/
    strcpy(array_registros[j+1].cadena, array_registros[j].cadena);
    array_registros[j+1].tiempo_ejecucion = array_registros[j].tiempo_ejecucion;
    strcpy(array_registros[j+1].fecha_ejecucion, array_registros[j].fecha_ejecucion);
    strcpy(array_registros[j+1].hora_ejecucion, array_registros[j].hora_ejecucion);

    /*Guardar los datos temporales para terminar intercambio*/
    strcpy(array_registros[j].cadena, registro_auxiliar.cadena);
    array_registros[j].tiempo_ejecucion = registro_auxiliar.tiempo_ejecucion;
    strcpy(array_registros[j].fecha_ejecucion, registro_auxiliar.fecha_ejecucion);
    strcpy(array_registros[j].hora_ejecucion, registro_auxiliar.hora_ejecucion);

}

/*
Función:
Autores de la función:
Parámetros de entrada:
Retorno:
Descripción:
*/
void OrdenarRegistroPorBurbuja(registro* array_registros, int numero_elementos, bool bandera_orden_reverso){

    int i = 0, j = 0;
    int resultado_comparacion_strings;
    char fecha_hora_1[TAMANO_FILA_FECHA_HORA+TAMANO_FILA_FECHA_HORA+1];
    char fecha_hora_2[TAMANO_FILA_FECHA_HORA+TAMANO_FILA_FECHA_HORA+1];

    for (i = 0; i < numero_elementos; i++){

      for (j = 0; j < numero_elementos-i-1; j++){

        if (bandera_orden_reverso == false){ /*Se va a ordenar de menor a mayor*/

            if(array_registros[j].tiempo_ejecucion > array_registros[j+1].tiempo_ejecucion){
                /*Hay que intercabiar los elementos para que quede primero el menor*/
                IntercambiarElementosArreglo(array_registros, j);

            }else{

                if(array_registros[j].tiempo_ejecucion == array_registros[j+1].tiempo_ejecucion){
                    /*Si los tiempos de ejecución son iguales, entonces desempatar por lo siguiente*/

                    strcpy(fecha_hora_1, array_registros[j].fecha_ejecucion);
                    strcat(fecha_hora_1, " ");
                    strcat(fecha_hora_1, array_registros[j].hora_ejecucion);

                    strcpy(fecha_hora_2, array_registros[j+1].fecha_ejecucion);
                    strcat(fecha_hora_2, " ");
                    strcat(fecha_hora_2, array_registros[j+1].hora_ejecucion);

                    resultado_comparacion_strings = strcmp(fecha_hora_1, fecha_hora_2);

                    if( resultado_comparacion_strings > 0 ){
                      /*Si el elemento siguiente es mayor en fecha u hora*/
                      /*Hay que intercabiar los elementos para que quede primero el menor*/
                      IntercambiarElementosArreglo(array_registros, j);
                    }/*end if*/

                }/*end if*/
            }/*end if*/

        }else{ /*Se va a ordenar de mayor a menor*/

          if(array_registros[j].tiempo_ejecucion < array_registros[j+1].tiempo_ejecucion){

              /*Hay que intercabiar los elementos para que quede primero el mayor*/
              IntercambiarElementosArreglo(array_registros, j);

          }else{

              if(array_registros[j].tiempo_ejecucion == array_registros[j+1].tiempo_ejecucion){
                  /*Si los tiempos de ejecución son iguales, entonces desempatar por lo siguiente*/

                  strcpy(fecha_hora_1, array_registros[j].fecha_ejecucion);
                  strcat(fecha_hora_1, " ");
                  strcat(fecha_hora_1, array_registros[j].hora_ejecucion);

                  strcpy(fecha_hora_2, array_registros[j+1].fecha_ejecucion);
                  strcat(fecha_hora_2, " ");
                  strcat(fecha_hora_2, array_registros[j+1].hora_ejecucion);

                  resultado_comparacion_strings = strcmp(fecha_hora_1, fecha_hora_2);

                  if( resultado_comparacion_strings < 0 ){
                    /*Si el elemento siguiente es menor en fecha u hora*/
                    /*Hay que intercabiar los elementos para que quede primero el menor*/
                    IntercambiarElementosArreglo(array_registros, j);
                  }/*end if*/
              }/*end if*/
          }/*end if*/
        }/*end if*/
      }/*end for*/
    }/*end for*/
}

/*
Función:
Autores de la función:
Parámetros de entrada:
Retorno:
Descripción:
*/
void OrdenarRegistroPorMergeSort(registro* array_registros, int numero_elementos, bool bandera_orden_reverso){
    mergeSort(array_registros, 0, numero_elementos - 1, bandera_orden_reverso);
}

/*
Función:
Autores de la función:
Parámetros de entrada:
Retorno:
Descripción:
*/
/* l is for left index and r is right index of the
   sub-array of arr to be sorted */
void mergeSort(registro* array_registros, int l, int r, bool bandera_orden_reverso){
    if (l < r){
        int m;
        m = l+(r-l)/2;/* Same as (l+r)/2, but avoids overflow for large l and h */

        /* Sort first and second halves*/
        mergeSort(array_registros, l, m, bandera_orden_reverso);
        mergeSort(array_registros, m+1, r, bandera_orden_reverso);

        merge(array_registros, l, m, r, bandera_orden_reverso);
    }/*end if*/
}

/*
Función:
Autores de la función:
Parámetros de entrada:
Retorno:
Descripción:
*/
void CopiarElementoArreglo(registro* arreglo1, registro* arreglo2, int posicion1, int posicion2){
      strcpy(arreglo1[posicion1].cadena, arreglo2[posicion2].cadena);
      arreglo1[posicion1].tiempo_ejecucion = arreglo2[posicion2].tiempo_ejecucion;
      strcpy(arreglo1[posicion1].fecha_ejecucion, arreglo2[posicion2].fecha_ejecucion);
      strcpy(arreglo1[posicion1].hora_ejecucion, arreglo2[posicion2].hora_ejecucion);
}

/*
Función:
Autores de la función:
Parámetros de entrada:
Retorno:
Descripción: Merges two subarrays of arr[]. First subarray is arr[l..m].
Second subarray is arr[m+1..r]
*/
void merge(registro* array_registros, int l, int m, int r, bool bandera_orden_reverso){

    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;
    int resultado_comparacion_strings;
    char fecha_hora_1[TAMANO_FILA_FECHA_HORA+TAMANO_FILA_FECHA_HORA+1];
    char fecha_hora_2[TAMANO_FILA_FECHA_HORA+TAMANO_FILA_FECHA_HORA+1];
    registro* array_L;
    registro* array_R;

    /*Creación de arrays temporales para manejar izquierda y derecha*/
    array_L = (registro*)malloc(n1*sizeof(registro));
    array_R = (registro*)malloc(n2*sizeof(registro));

    if (array_L == NULL) {
        printf("Memoria no alocada.\n");
        exit(1);
    }/*end if*/

    if (array_R == NULL) {
        printf("Memoria no alocada.\n");
        exit(1);
    }/*end if*/

    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++){
        /* L[i] = arr[l + i]; */
        CopiarElementoArreglo(array_L, array_registros, i, l + i);
    }/*end for*/

    for (j = 0; j < n2; j++){
        /* R[j] = arr[m + 1+ j]; */
        CopiarElementoArreglo(array_R, array_registros, j, m + 1 + j);
    }/*end for*/

    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; /* Initial index of first subarray */
    j = 0; /* Initial index of second subarray */
    k = l; /* Initial index of merged subarray */
    while (i < n1 && j < n2){

        if (bandera_orden_reverso == false){ /*Se va a ordenar de menor a mayor*/

            /*if (L[i] <= R[j]) /*de menor a mayor*/
            if (array_L[i].tiempo_ejecucion < array_R[j].tiempo_ejecucion){
                /* arr[k] = L[i]; */
                CopiarElementoArreglo(array_registros, array_L, k, i);
                i++;
            }else{

                if (array_L[i].tiempo_ejecucion > array_R[j].tiempo_ejecucion){
                    /* arr[k] = R[j]; */
                    CopiarElementoArreglo(array_registros, array_R, k, j);
                    j++;
                }else{
                    /*Si los tiempos de ejecución son iguales, entonces desempatar por lo siguiente*/
                    strcpy(fecha_hora_1, array_L[i].fecha_ejecucion);
                    strcat(fecha_hora_1, " ");
                    strcat(fecha_hora_1, array_L[i].hora_ejecucion);

                    strcpy(fecha_hora_2, array_R[j].fecha_ejecucion);
                    strcat(fecha_hora_2, " ");
                    strcat(fecha_hora_2, array_R[j].hora_ejecucion);

                    resultado_comparacion_strings = strcmp(fecha_hora_1, fecha_hora_2);

                    if( resultado_comparacion_strings < 0 ){
                      /*Si el elemento siguiente es menor en fecha u hora*/
                        /* arr[k] = L[i]; */
                        CopiarElementoArreglo(array_registros, array_L, k, i);
                        i++;
                    }else{
                        /* arr[k] = R[j]; */
                        CopiarElementoArreglo(array_registros, array_R, k, j);
                        j++;
                    }
                }
            }

        }else{ /*Se va a ordenar de mayor a menor*/

          if (array_L[i].tiempo_ejecucion > array_R[j].tiempo_ejecucion){
              /* arr[k] = L[i]; */
              CopiarElementoArreglo(array_registros, array_L, k, i);
              i++;
          }else{
              if (array_L[i].tiempo_ejecucion < array_R[j].tiempo_ejecucion){
                  /* arr[k] = R[j]; */
                  CopiarElementoArreglo(array_registros, array_R, k, j);
                  j++;
              }else{
                  /*Si los tiempos de ejecución son iguales, entonces desempatar por lo siguiente*/
                  strcpy(fecha_hora_1, array_L[i].fecha_ejecucion);
                  strcat(fecha_hora_1, " ");
                  strcat(fecha_hora_1, array_L[i].hora_ejecucion);

                  strcpy(fecha_hora_2, array_R[j].fecha_ejecucion);
                  strcat(fecha_hora_2, " ");
                  strcat(fecha_hora_2, array_R[j].hora_ejecucion);

                  resultado_comparacion_strings = strcmp(fecha_hora_1, fecha_hora_2);

                  if( resultado_comparacion_strings > 0 ){
                    /*Si el elemento siguiente es mayor en fecha u hora*/
                      /* arr[k] = L[i]; */
                      CopiarElementoArreglo(array_registros, array_L, k, i);
                      i++;
                  }else{
                      /* arr[k] = R[j]; */
                      CopiarElementoArreglo(array_registros, array_R, k, j);
                      j++;
                  }/*end if*/
              }/*end if*/
          }/*end if*/
        }/*end if*/
        k++;
    }/*end while*/
    /* Copy the remaining elements of L[], if there are any */
    while (i < n1){
        /* arr[k] = L[i]; */
        CopiarElementoArreglo(array_registros, array_L, k, i);
        i++;
        k++;
    }/*end while*/
    /* Copy the remaining elements of R[], if there are any */
    while (j < n2){
        /* arr[k] = R[j]; */
        CopiarElementoArreglo(array_registros, array_R, k, j);
        j++;
        k++;
    }/*end while*/
    free(array_L);
    free(array_R);
}

/*
Función:
Autores de la función:
Parámetros de entrada:
Retorno:
Descripción:
*/
void ImprimirArchivo(registro* array_registros, int numero_elementos, char nombre_archivo[], bool temporal){

     int i = 0;
     FILE *archivo;

     char archivo_nombre[MAXIMO_NOMBRE_ARCHIVO];
     strcpy(archivo_nombre, nombre_archivo);

     if (temporal == true){
        strcat(archivo_nombre,"_temporal");
     }/*end if*/

     archivo = fopen(archivo_nombre, "w");

     if(archivo == NULL){
        perror( archivo_nombre );
        free(array_registros);
        exit(1);
     }/*end if*/

     for (i = 0; i < numero_elementos; i++){
        fprintf(archivo,"%s", array_registros[i].cadena);
     }/*end for*/

     fclose(archivo);

}
