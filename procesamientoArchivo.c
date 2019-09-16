/*
Nombre del archivo: procesamientoArchivo.c
Autores: Adrián García y Luis Rosales
Objetivo: proveer la implementación para procesar cada archivo de entrada en
los comandos csortp, csorth y csortpexec.
Funciones: LeerArchivo, ContarLineasArchivo, IntercambiarElementosArreglo,
OrdenarRegistroPorBurbuja, OrdenarRegistroPorMergeSort, ImprimirArchivo,
MergeSort, CopiarElementoArreglo, Merge, AbrirArchivo.
Fecha de última modificación: 14/09/19
*/

#include "procesamientoArchivo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*
Función: AbrirArchivo
Autores de la función: Adrián García.
Parámetros de entrada: nombre del archivo a abrir.
Retorno: un valor booleano que es true si el archivo se puede abrir
o false si el archivo no se puede abrir.
Descripción: verifica que un archivo de entrada se puede abrir.
*/

bool AbrirArchivo(char nombre_archivo[]){
    FILE *archivo;
    archivo = fopen( nombre_archivo ,"r");
    if ( archivo != NULL ){
        fclose(archivo);
        return true;
    }else{
        perror ( nombre_archivo );
        return false;
    }/*end if*/
}

/*
Función: LeerArchivo
Autores de la función: Adrián García.
Parámetros de entrada: nombre del archivo a leer y número de líneas que tiene.
Retorno: apuntador a arreglo de tipo registro, que contiene todos los datos del
archivo.
Descripción: lee un archivo línea por línea, almacenando los elementos de cada
una en una estructura registro (definida en procesamientoArchivo.h) y devuelve
un apuntador a arreglo de tipo registro que contiene todos los datos del
archivo.
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
    registro* arreglo_registros = (registro*)malloc(numero_lineas_archivo*sizeof(registro));
    char* auxiliar_para_strtok_r;

    if (arreglo_registros == NULL) {
        perror("Memoria no alocada para las filas del log");
        exit(1);
    }/*end if*/

    archivo = fopen( nombre_archivo ,"r");

    if ( archivo != NULL ){

      while (fgets(linea,sizeof(linea),archivo)) {
          strcpy(arreglo_registros[numero_linea].cadena, linea);
          numero_linea = numero_linea + 1;
      }/*end while*/
      fclose(archivo);

    }else{
       free(arreglo_registros);
       perror ( nombre_archivo );
       exit(1);
    }/*end if*/


    for (i = 0; i < numero_lineas_archivo; i++){

        strcpy(linea_aux, arreglo_registros[i].cadena);

        if( (caracter_aux = strchr(linea_aux, '\n')) != NULL){
            *caracter_aux = '\0';
        }/*end if*/

        auxiliar_para_strtok_r = linea_aux;

        item = strtok_r(linea_aux, delimitador, &auxiliar_para_strtok_r);
        contador = 1;
        while(item != NULL){
            if (contador == 4){
                arreglo_registros[i].tiempo_ejecucion = atoi(item);
            }/*end if*/
            if (contador == 5){
                strcpy(arreglo_registros[i].fecha_ejecucion,item);
            }/*end if*/
            if (contador == 6){
                strcpy(arreglo_registros[i].hora_ejecucion,item);
                item = NULL;
            }/*end if*/
            contador = contador+1;
            item = strtok_r(NULL, delimitador, &auxiliar_para_strtok_r);
        }/*end while*/

    }/*end for*/

    return arreglo_registros;

}

/*
Función:ContarLineasArchivo
Autores de la función: Adrián García y Luis Rosales.
Parámetros de entrada: nombre del archivo al que se le van a contar las líneas.
Retorno: el número de líneas que tiene el archivo.
Descripción: cuenta el número de líneas que tiene un archivo y retorna dicho valor.
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

    /* extraer caracteres del archivo y almacenarlos en variable caracter*/
    for (caracter = getc(archivo); caracter != EOF; caracter = getc(archivo)){
        if (caracter == '\n'){
            contador_lineas = contador_lineas + 1;
        }/*end if*/
    }/*end for*/

    fclose(archivo);

    return contador_lineas;
}

/*
Función: IntercambiarElementosArreglo
Autores de la función: Adrián García.
Parámetros de entrada: un apuntador a arreglo de tipo registro que se está
ordenando, y un entero con la posición del primer elemento a intercambiar.
Retorno: no tiene.
Descripción: intercambia dos elementos en un arreglo de tipo registro,
específicamente intercambia los elementos j y j+1. Constituye una función
auxiliar para la función OrdenarRegistroPorBurbuja.
*/
void IntercambiarElementosArreglo(registro* arreglo_registros, int j){

    registro registro_auxiliar;

    /*Salvar los datos del primero*/
    strcpy(registro_auxiliar.cadena, arreglo_registros[j+1].cadena);
    registro_auxiliar.tiempo_ejecucion = arreglo_registros[j+1].tiempo_ejecucion;
    strcpy(registro_auxiliar.fecha_ejecucion, arreglo_registros[j+1].fecha_ejecucion);
    strcpy(registro_auxiliar.hora_ejecucion, arreglo_registros[j+1].hora_ejecucion);

    /*Intercambiar un elemento*/
    strcpy(arreglo_registros[j+1].cadena, arreglo_registros[j].cadena);
    arreglo_registros[j+1].tiempo_ejecucion = arreglo_registros[j].tiempo_ejecucion;
    strcpy(arreglo_registros[j+1].fecha_ejecucion, arreglo_registros[j].fecha_ejecucion);
    strcpy(arreglo_registros[j+1].hora_ejecucion, arreglo_registros[j].hora_ejecucion);

    /*Guardar los datos temporales para terminar intercambio*/
    strcpy(arreglo_registros[j].cadena, registro_auxiliar.cadena);
    arreglo_registros[j].tiempo_ejecucion = registro_auxiliar.tiempo_ejecucion;
    strcpy(arreglo_registros[j].fecha_ejecucion, registro_auxiliar.fecha_ejecucion);
    strcpy(arreglo_registros[j].hora_ejecucion, registro_auxiliar.hora_ejecucion);

}

/*
Función: OrdenarRegistroPorBurbuja
Autores de la función: Adrián García.
Parámetros de entrada: un apuntador a arreglo de tipo registro que se va a
ordenar, un entero con el número de elementos de dicho arreglo y un booleano
indicando si el ordenamiento se va a hacer o no en orden reverso.
Retorno: no tiene.
Descripción: realiza el ordenamiento de un arreglo de tipo registro usando el
algoritmo de ordenamiento Bubble Sort.
*/
void OrdenarRegistroPorBurbuja(registro* arreglo_registros, int numero_elementos, bool bandera_orden_reverso){

    int i = 0, j = 0;
    int resultado_comparacion_strings;
    char fecha_hora_1[TAMANO_FILA_FECHA_HORA+TAMANO_FILA_FECHA_HORA+1];
    char fecha_hora_2[TAMANO_FILA_FECHA_HORA+TAMANO_FILA_FECHA_HORA+1];

    for (i = 0; i < numero_elementos; i++){

      for (j = 0; j < numero_elementos-i-1; j++){

        if (bandera_orden_reverso == false){ /*Se va a ordenar de menor a mayor*/

            if(arreglo_registros[j].tiempo_ejecucion > arreglo_registros[j+1].tiempo_ejecucion){
                /*Hay que intercabiar los elementos para que quede primero el menor*/
                IntercambiarElementosArreglo(arreglo_registros, j);

            }else{

                if(arreglo_registros[j].tiempo_ejecucion == arreglo_registros[j+1].tiempo_ejecucion){
                    /*Si los tiempos de ejecución son iguales, entonces desempatar por lo siguiente*/

                    strcpy(fecha_hora_1, arreglo_registros[j].fecha_ejecucion);
                    strcat(fecha_hora_1, " ");
                    strcat(fecha_hora_1, arreglo_registros[j].hora_ejecucion);

                    strcpy(fecha_hora_2, arreglo_registros[j+1].fecha_ejecucion);
                    strcat(fecha_hora_2, " ");
                    strcat(fecha_hora_2, arreglo_registros[j+1].hora_ejecucion);

                    resultado_comparacion_strings = strcmp(fecha_hora_1, fecha_hora_2);

                    if( resultado_comparacion_strings > 0 ){
                      /*Si el elemento siguiente es mayor en fecha u hora*/
                      /*Hay que intercabiar los elementos para que quede primero el menor*/
                      IntercambiarElementosArreglo(arreglo_registros, j);
                    }/*end if*/

                }/*end if*/
            }/*end if*/

        }else{ /*Se va a ordenar de mayor a menor*/

          if(arreglo_registros[j].tiempo_ejecucion < arreglo_registros[j+1].tiempo_ejecucion){

              /*Hay que intercabiar los elementos para que quede primero el mayor*/
              IntercambiarElementosArreglo(arreglo_registros, j);

          }else{

              if(arreglo_registros[j].tiempo_ejecucion == arreglo_registros[j+1].tiempo_ejecucion){
                  /*Si los tiempos de ejecución son iguales, entonces desempatar por lo siguiente*/

                  strcpy(fecha_hora_1, arreglo_registros[j].fecha_ejecucion);
                  strcat(fecha_hora_1, " ");
                  strcat(fecha_hora_1, arreglo_registros[j].hora_ejecucion);

                  strcpy(fecha_hora_2, arreglo_registros[j+1].fecha_ejecucion);
                  strcat(fecha_hora_2, " ");
                  strcat(fecha_hora_2, arreglo_registros[j+1].hora_ejecucion);

                  resultado_comparacion_strings = strcmp(fecha_hora_1, fecha_hora_2);

                  if( resultado_comparacion_strings < 0 ){
                    /*Si el elemento siguiente es menor en fecha u hora*/
                    /*Hay que intercabiar los elementos para que quede primero el menor*/
                    IntercambiarElementosArreglo(arreglo_registros, j);
                  }/*end if*/
              }/*end if*/
          }/*end if*/
        }/*end if*/
      }/*end for*/
    }/*end for*/
}

/*
Función: OrdenarRegistroPorMergeSort
Autores de la función: la fuente original es M. Kjanna and C. Nayal. (2019) Merge Sort. [Online]. Available: https://www.
geeksforgeeks.org/Merge-sort/. Adaptado por Adrián García.
Parámetros de entrada: un apuntador a arreglo de tipo registro que se va a
ordenar, un entero con el número de elementos de dicho arreglo y un booleano
indicando si el ordenamiento se va a hacer o no en orden reverso.
Retorno: no tiene.
Descripción: invoca a la función MergeSort, enviando a ésta como parámetros
el apuntador a arreglo de tipo registro, la primera posición de dicho arreglo,
la última posición de dicho arreglo, y el booleano indicando si el ordenamiento
se va a realizar en orden reverso.
*/
void OrdenarRegistroPorMergeSort(registro* arreglo_registros, int numero_elementos, bool bandera_orden_reverso){
    MergeSort(arreglo_registros, 0, numero_elementos - 1, bandera_orden_reverso);
}

/*
Función: OrdenarRegistroPorMergeSort
Autores de la función: la fuente original es M. Kjanna and C. Nayal. (2019) Merge Sort. [Online]. Available: https://www.
geeksforgeeks.org/merge-sort/. Adaptado por Adrián García.
Parámetros de entrada: un apuntador a arreglo de tipo registro que se va a
ordenar, un entero con una posición izquierdo (left) del arreglo, un entero con
una posición derecha (right) del arreglo y un booleano indicando si el
ordenamiento se va a hacer o no en orden reverso.
Retorno: no tiene.
Descripción: es la función principal para realizar el ordenamiento del arreglo
mediante el algoritmo Merge Sort. Si los límites izquierdo (left) y derecho (right)
del sub-arreglo a ser ordenado son correctos, se toma como pívot el elemento en
la mitad entre ambos límitesy luego se invoca a la misma función dos veces:
en la primera enviando como parámetros la parte del arreglo desde el límite
izquierdo hasta el pívot, y en la segunda la parte del arreglo desde el pívot
hasta el límite derecho.
Posteriormente, se llama a la función Merge, la cual mezcla de forma ordenada
las partes izquierda y derecha previamente ordenadas.
*/
void MergeSort(registro* arreglo_registros, int l, int r, bool bandera_orden_reverso){
    if (l < r){
        int m; /*posición del elemento que se toma como pívot*/
        m = l+(r-l)/2;/* Igual que (l+r)/2, pero evita desbordamiento para l y h grandes*/

        /* Ordenar primera y segunda mitad del arreglo*/
        MergeSort(arreglo_registros, l, m, bandera_orden_reverso);
        MergeSort(arreglo_registros, m+1, r, bandera_orden_reverso);

        Merge(arreglo_registros, l, m, r, bandera_orden_reverso);
    }/*end if*/
}

/*
Función: CopiarElementoArreglo
Autores de la función: Adrián García.
Parámetros de entrada: dos apuntadores a sub-arreglos de tipo registro (del arreglo
original que se está ordenando), y dos enteros indicando posiciones en los dos
subarreglos respectivamente.
Retorno: no tiene.
Descripción: copia el elemento del sub-arreglo 2 en la posición 2, al sub-arreglo
1 en la posición 1. Se trata de una función auxiliar para la función Merge.
*/
void CopiarElementoArreglo(registro* arreglo1, registro* arreglo2, int posicion1, int posicion2){
      strcpy(arreglo1[posicion1].cadena, arreglo2[posicion2].cadena);
      arreglo1[posicion1].tiempo_ejecucion = arreglo2[posicion2].tiempo_ejecucion;
      strcpy(arreglo1[posicion1].fecha_ejecucion, arreglo2[posicion2].fecha_ejecucion);
      strcpy(arreglo1[posicion1].hora_ejecucion, arreglo2[posicion2].hora_ejecucion);
}

/*
Función: OrdenarRegistroPorMergeSort
Autores de la función: la fuente original es M. Kjanna and C. Nayal. (2019) Merge Sort. [Online]. Available: https://www.
geeksforgeeks.org/Merge-sort/. Adaptado por Adrián García.
Parámetros de entrada: un apuntador a arreglo de tipo registro que se va a
ordenar, enteros con las posiciones izquierda (left), derecha (right) y la del
pívot (hallada en la función MergeSort), y booleano indicando si el
ordenamiento se va a realizar o no en orden reverso.
Retorno: no tiene.
Descripción: mezcla dos sub-arreglos del arreglo original. El primer subarreglo
va desde la posición izquierda hasta la posición del pívot, y el segundo subarreglo
va desde la posición del pívot hasta la posición derecha. Cada sub-arreglo es
mantenido con sub-arreglos auxiliares llamados arreglo_L y arreglo_R, y de acuerdo
a las condiciones del algoritmo elementos del arreglo original son
intercambiados usando los sub-arreglos auxiliares.
*/
void Merge(registro* arreglo_registros, int l, int m, int r, bool bandera_orden_reverso){

    int i, j, k;
    int n1 = m - l + 1; /*número de elementos a la izquierda del pívot más uno*/
    int n2 =  r - m; /*número de elementos a la derecha del pívot*/
    int resultado_comparacion_strings;
    char fecha_hora_1[TAMANO_FILA_FECHA_HORA+TAMANO_FILA_FECHA_HORA+1];
    char fecha_hora_2[TAMANO_FILA_FECHA_HORA+TAMANO_FILA_FECHA_HORA+1];
    registro* arreglo_L;
    registro* arreglo_R;

    /*Creación de arreglos temporales para manejar izquierda (Left) y derecha (Right)*/
    arreglo_L = (registro*)malloc(n1*sizeof(registro));
    arreglo_R = (registro*)malloc(n2*sizeof(registro));

    if (arreglo_L == NULL) {
        perror("Memoria no alocada");
        exit(1);
    }/*end if*/

    if (arreglo_R == NULL) {
        perror("Memoria no alocada");
        exit(1);
    }/*end if*/

    /* copiar datos a sub-arreglos temporales Left[] and Right[] */
    for (i = 0; i < n1; i++){
        /* Left[i] = original[l + i]; */
        CopiarElementoArreglo(arreglo_L, arreglo_registros, i, l + i);
    }/*end for*/

    for (j = 0; j < n2; j++){
        /* Right[j] = original[m + 1+ j]; */
        CopiarElementoArreglo(arreglo_R, arreglo_registros, j, m + 1 + j);
    }/*end for*/

    /* Unir los arreglos temporales de nuevo en el original[l..r]*/
    i = 0; /* Primera posición del primer sub-arreglo */
    j = 0; /* Primera posición del segundo subarreglo */
    k = l; /* Posición inicial del subarreglo unido */
    while (i < n1 && j < n2){

        if (bandera_orden_reverso == false){ /*Se va a ordenar de menor a mayor*/

            /*if (Left[i] <= Right[j]) /*de menor a mayor*/
            if (arreglo_L[i].tiempo_ejecucion < arreglo_R[j].tiempo_ejecucion){
                /* original[k] = Left[i]; */
                CopiarElementoArreglo(arreglo_registros, arreglo_L, k, i);
                i++;
            }else{

                if (arreglo_L[i].tiempo_ejecucion > arreglo_R[j].tiempo_ejecucion){
                    /* original[k] = Right[j]; */
                    CopiarElementoArreglo(arreglo_registros, arreglo_R, k, j);
                    j++;
                }else{
                    /*Si los tiempos de ejecución son iguales, entonces desempatar por lo siguiente*/
                    strcpy(fecha_hora_1, arreglo_L[i].fecha_ejecucion);
                    strcat(fecha_hora_1, " ");
                    strcat(fecha_hora_1, arreglo_L[i].hora_ejecucion);

                    strcpy(fecha_hora_2, arreglo_R[j].fecha_ejecucion);
                    strcat(fecha_hora_2, " ");
                    strcat(fecha_hora_2, arreglo_R[j].hora_ejecucion);

                    resultado_comparacion_strings = strcmp(fecha_hora_1, fecha_hora_2);

                    if( resultado_comparacion_strings < 0 ){
                      /*Si el elemento siguiente es menor en fecha u hora*/
                        /* original[k] = Left[i]; */
                        CopiarElementoArreglo(arreglo_registros, arreglo_L, k, i);
                        i++;
                    }else{
                        /* original[k] = Right[j]; */
                        CopiarElementoArreglo(arreglo_registros, arreglo_R, k, j);
                        j++;
                    }
                }
            }

        }else{ /*Se va a ordenar de mayor a menor*/

          if (arreglo_L[i].tiempo_ejecucion > arreglo_R[j].tiempo_ejecucion){
              /* original[k] = Left[i]; */
              CopiarElementoArreglo(arreglo_registros, arreglo_L, k, i);
              i++;
          }else{
              if (arreglo_L[i].tiempo_ejecucion < arreglo_R[j].tiempo_ejecucion){
                  /* original[k] = Right[j]; */
                  CopiarElementoArreglo(arreglo_registros, arreglo_R, k, j);
                  j++;
              }else{
                  /*Si los tiempos de ejecución son iguales, entonces desempatar por lo siguiente*/
                  strcpy(fecha_hora_1, arreglo_L[i].fecha_ejecucion);
                  strcat(fecha_hora_1, " ");
                  strcat(fecha_hora_1, arreglo_L[i].hora_ejecucion);

                  strcpy(fecha_hora_2, arreglo_R[j].fecha_ejecucion);
                  strcat(fecha_hora_2, " ");
                  strcat(fecha_hora_2, arreglo_R[j].hora_ejecucion);

                  resultado_comparacion_strings = strcmp(fecha_hora_1, fecha_hora_2);

                  if( resultado_comparacion_strings > 0 ){
                    /*Si el elemento siguiente es mayor en fecha u hora*/
                      /* original[k] = Left[i]; */
                      CopiarElementoArreglo(arreglo_registros, arreglo_L, k, i);
                      i++;
                  }else{
                      /* original[k] = Right[j]; */
                      CopiarElementoArreglo(arreglo_registros, arreglo_R, k, j);
                      j++;
                  }/*end if*/
              }/*end if*/
          }/*end if*/
        }/*end if*/
        k++;
    }/*end while*/
    /* Copiar los elementos restantes de Left[], si hay */
    while (i < n1){
        /* arr[k] = Left[i]; */
        CopiarElementoArreglo(arreglo_registros, arreglo_L, k, i);
        i++;
        k++;
    }/*end while*/
    /* Copiar los elementos restantes de Right[], si hay */
    while (j < n2){
        /* original[k] = Right[j]; */
        CopiarElementoArreglo(arreglo_registros, arreglo_R, k, j);
        j++;
        k++;
    }/*end while*/
    free(arreglo_L);
    free(arreglo_R);
}

/*
Función: ImprimirArchivo
Autores de la función: Luis Rosales.
Parámetros de entrada: un apuntador a arreglo de tipo registro que ya fue
ordenado, un entero indicando el número de elementos de dicho arreglo, el
nombre del archivo donde se van a imprimir los elementos del arreglo y un
booleano indicando si el archivo que se va a escribir es temporal (para el caso
de los comandos que crean procesos hijos).
Retorno: no tiene.
Descripción: imprime los elementos de un arreglo de tipo registro en el archivo
de salida indicado.
*/
void ImprimirArchivo(registro* arreglo_registros, int numero_elementos, char nombre_archivo[], bool temporal){

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
        free(arreglo_registros);
        exit(1);
     }/*end if*/

     for (i = 0; i < numero_elementos; i++){
        fprintf(archivo,"%s", arreglo_registros[i].cadena);
     }/*end for*/

     fclose(archivo);

}
