#include "procesamientoArchivo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

registro* LeerArchivo(char nombre_archivo[], int numero_lineas_archivo){

    FILE * archivo;
    char linea[max_tamano_cadena];
    char linea_aux[max_tamano_cadena];
    char delim[] = " ";
    int contador = 0;
    int numero_linea = 0;
    int i = 0;
    char *item;
    char *ptr;
    registro* array_registros = (registro*)malloc(numero_lineas_archivo*sizeof(registro));

    if (array_registros == NULL) {
        printf("Memoria no alocada para las filas del log.\n");
        exit(1);
    }

    archivo = fopen( nombre_archivo ,"r");

    if ( archivo != NULL ){
      /* Leer cada línea del archivo y almacenarla en array de registro */

      while (fgets(linea,sizeof(linea),archivo)) {
          strcpy(array_registros[numero_linea].cadena, linea);
          /*printf("%s",array_registros[numero_linea].cadena);*/
          numero_linea = numero_linea + 1;
      }
      fclose(archivo);/* Close file */

    }else{
       free(array_registros);
       perror ( nombre_archivo ); /* why didn't the file open? */
       exit(1);
    }


    for (i = 0; i < numero_lineas_archivo; i++){

        strcpy(linea_aux, array_registros[i].cadena);

        if( (ptr = strchr(linea_aux, '\n')) != NULL)
          *ptr = '\0';

        item = strtok(linea_aux, delim);
        contador = 1;
        while(item != NULL){
            /*printf("Contador: %d\n",contador);
            printf("%s\n",item);*/
            if (contador == 4){
                array_registros[i].tiempo_ejecucion = atoi(item);
                /*printf("%d\n",array_registros[i].tiempo_ejecucion);*/
            }
            if (contador == 5){
                strcpy(array_registros[i].fecha_ejecucion,item);
                /*printf("%s\n",array_registros[i].fecha_ejecucion);*/
            }
            if (contador == 6){
                strcpy(array_registros[i].hora_ejecucion,item);
                /*printf("%s\n",array_registros[i].hora_ejecucion);*/
                item = NULL;
            }

            contador = contador+1;
            item = strtok(NULL, delim);
        }

    }

    return array_registros;

}

int ContarLineasArchivo(char nombre_archivo[]){
    FILE *fp;
    int count = 0;  /* Line counter (result) */
    /*char nombre_archivo[MAX_FILE_NAME];*/
    char c;  /* To store a character read from file*/

    /* Open the file */
    fp = fopen(nombre_archivo, "r");

    /* Check if file exists */
    if (fp == NULL){
        perror ( nombre_archivo ); /* why didn't the file open? */
        /*exit(1);*/
        return 0;
    }

    /* Extract characters from file and store in character c */
    for (c = getc(fp); c != EOF; c = getc(fp))
        if (c == '\n') /* Increment count if this character is newline */
            count = count + 1;

    /* Close the file */
    fclose(fp);
    /*printf("The file %s has %d lines\n ", nombre_archivo, count);*/

    return count;
}

void OrdenarRegistroPorBurbuja(registro* array_registros, int numero_elementos, bool bandera_orden_reverso){

    int i = 0, j = 0;
    int resultado_comparacion_strings;
    char fecha_hora_1[tamano_fila_fecha_hora+tamano_fila_fecha_hora+1];
    char fecha_hora_2[tamano_fila_fecha_hora+tamano_fila_fecha_hora+1];
    registro registro_auxiliar;

    /*for (i = 0; i < numero_elementos; i++){
      printf("Contador:%d\n",i);
      printf("%s",array_registros[i].cadena);
      printf("%d ",array_registros[i].tiempo_ejecucion);
      printf("%s ",array_registros[i].fecha_ejecucion);
      printf("%s\n",array_registros[i].hora_ejecucion);
    }*/

    for (i = 0; i < numero_elementos; i++){

      for (j = 0; j < numero_elementos-i-1; j++){

        if (bandera_orden_reverso == false){ /*Se va a ordenar de menor a mayor*/

            if(array_registros[j].tiempo_ejecucion > array_registros[j+1].tiempo_ejecucion){

                /*Hay que intercabiar los elementos para que quede primero el menor*/

                /*Salvar los datos del que tiene menor tiempo de ejecución*/
                strcpy(registro_auxiliar.cadena, array_registros[j+1].cadena);
                registro_auxiliar.tiempo_ejecucion = array_registros[j+1].tiempo_ejecucion;
                strcpy(registro_auxiliar.fecha_ejecucion, array_registros[j+1].fecha_ejecucion);
                strcpy(registro_auxiliar.hora_ejecucion, array_registros[j+1].hora_ejecucion);

                /*Cambiar elemento con menor tiempo por el otro*/
                strcpy(array_registros[j+1].cadena, array_registros[j].cadena);
                array_registros[j+1].tiempo_ejecucion = array_registros[j].tiempo_ejecucion;
                strcpy(array_registros[j+1].fecha_ejecucion, array_registros[j].fecha_ejecucion);
                strcpy(array_registros[j+1].hora_ejecucion, array_registros[j].hora_ejecucion);

                /*Guardar los datos temporales para terminar intercambio*/
                strcpy(array_registros[j].cadena, registro_auxiliar.cadena);
                array_registros[j].tiempo_ejecucion = registro_auxiliar.tiempo_ejecucion;
                strcpy(array_registros[j].fecha_ejecucion, registro_auxiliar.fecha_ejecucion);
                strcpy(array_registros[j].hora_ejecucion, registro_auxiliar.hora_ejecucion);


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

                      /*Salvar los datos del que tiene menor tiempo de ejecución*/
                      strcpy(registro_auxiliar.cadena, array_registros[j+1].cadena);
                      registro_auxiliar.tiempo_ejecucion = array_registros[j+1].tiempo_ejecucion;
                      strcpy(registro_auxiliar.fecha_ejecucion, array_registros[j+1].fecha_ejecucion);
                      strcpy(registro_auxiliar.hora_ejecucion, array_registros[j+1].hora_ejecucion);

                      /*Cambiar elemento con menor tiempo por el otro*/
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

                }
            }

        }else{ /*Se va a ordenar de mayor a menor*/

          if(array_registros[j].tiempo_ejecucion < array_registros[j+1].tiempo_ejecucion){

              /*Hay que intercabiar los elementos para que quede primero el mayor*/

              /*Salvar los datos del que tiene mayor tiempo de ejecución*/
              strcpy(registro_auxiliar.cadena, array_registros[j+1].cadena);
              registro_auxiliar.tiempo_ejecucion = array_registros[j+1].tiempo_ejecucion;
              strcpy(registro_auxiliar.fecha_ejecucion, array_registros[j+1].fecha_ejecucion);
              strcpy(registro_auxiliar.hora_ejecucion, array_registros[j+1].hora_ejecucion);

              /*Cambiar elemento con mayor tiempo por el otro*/
              strcpy(array_registros[j+1].cadena, array_registros[j].cadena);
              array_registros[j+1].tiempo_ejecucion = array_registros[j].tiempo_ejecucion;
              strcpy(array_registros[j+1].fecha_ejecucion, array_registros[j].fecha_ejecucion);
              strcpy(array_registros[j+1].hora_ejecucion, array_registros[j].hora_ejecucion);

              /*Guardar los datos temporales para terminar intercambio*/
              strcpy(array_registros[j].cadena, registro_auxiliar.cadena);
              array_registros[j].tiempo_ejecucion = registro_auxiliar.tiempo_ejecucion;
              strcpy(array_registros[j].fecha_ejecucion, registro_auxiliar.fecha_ejecucion);
              strcpy(array_registros[j].hora_ejecucion, registro_auxiliar.hora_ejecucion);


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

                    /*Salvar los datos del que tiene menor tiempo de ejecución*/
                    strcpy(registro_auxiliar.cadena, array_registros[j+1].cadena);
                    registro_auxiliar.tiempo_ejecucion = array_registros[j+1].tiempo_ejecucion;
                    strcpy(registro_auxiliar.fecha_ejecucion, array_registros[j+1].fecha_ejecucion);
                    strcpy(registro_auxiliar.hora_ejecucion, array_registros[j+1].hora_ejecucion);

                    /*Cambiar elemento con menor tiempo por el otro*/
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

              }
          }

        }


      }
    }

}

void OrdenarRegistroPorMergeSort(registro* array_registros, int numero_elementos, bool bandera_orden_reverso){

    mergeSort(array_registros, 0, numero_elementos - 1, bandera_orden_reverso);

}

/* l is for left index and r is right index of the
   sub-array of arr to be sorted */
void mergeSort(registro* array_registros, int l, int r, bool bandera_orden_reverso){
    if (l < r){
        /* Same as (l+r)/2, but avoids overflow for large l and h */
        int m = l+(r-l)/2;

        /* Sort first and second halves*/
        mergeSort(array_registros, l, m, bandera_orden_reverso);
        mergeSort(array_registros, m+1, r, bandera_orden_reverso);

        merge(array_registros, l, m, r, bandera_orden_reverso);
    }
}

/* Merges two subarrays of arr[].
// First subarray is arr[l..m]
// Second subarray is arr[m+1..r] */
void merge(registro* array_registros, int l, int m, int r, bool bandera_orden_reverso){

    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;

    int resultado_comparacion_strings;
    char fecha_hora_1[tamano_fila_fecha_hora+tamano_fila_fecha_hora+1];
    char fecha_hora_2[tamano_fila_fecha_hora+tamano_fila_fecha_hora+1];

    /* create temp arrays */
    /*int L[n1], R[n2];*/
    registro* array_L = (registro*)malloc(n1*sizeof(registro));
    if (array_L == NULL) {
        printf("Memory not allocated.\n");
        exit(1);
    }
    registro* array_R = (registro*)malloc(n2*sizeof(registro));
    if (array_R == NULL) {
        printf("Memory not allocated.\n");
        exit(1);
    }

    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++){
        /* L[i] = arr[l + i]; */
        strcpy(array_L[i].cadena, array_registros[l + i].cadena);
        array_L[i].tiempo_ejecucion = array_registros[l + i].tiempo_ejecucion;
        strcpy(array_L[i].fecha_ejecucion, array_registros[l + i].fecha_ejecucion);
        strcpy(array_L[i].hora_ejecucion, array_registros[l + i].hora_ejecucion);
    }

    for (j = 0; j < n2; j++){
        /* R[j] = arr[m + 1+ j]; */
        strcpy(array_R[j].cadena, array_registros[m + 1 + j].cadena);
        array_R[j].tiempo_ejecucion = array_registros[m + 1 + j].tiempo_ejecucion;
        strcpy(array_R[j].fecha_ejecucion, array_registros[m + 1 + j].fecha_ejecucion);
        strcpy(array_R[j].hora_ejecucion, array_registros[m + 1 + j].hora_ejecucion);
    }

    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; /* Initial index of first subarray */
    j = 0; /* Initial index of second subarray */
    k = l; /* Initial index of merged subarray */
    while (i < n1 && j < n2)
    {

        if (bandera_orden_reverso == false){ /*Se va a ordenar de menor a mayor*/

            /*if (L[i] <= R[j]) /*de menor a mayor*/
            if (array_L[i].tiempo_ejecucion < array_R[j].tiempo_ejecucion){

                strcpy(array_registros[k].cadena, array_L[i].cadena);
                array_registros[k].tiempo_ejecucion = array_L[i].tiempo_ejecucion;
                strcpy(array_registros[k].fecha_ejecucion, array_L[i].fecha_ejecucion);
                strcpy(array_registros[k].hora_ejecucion, array_L[i].hora_ejecucion);

                /* arr[k] = L[i]; */
                i++;
            }else{

                if (array_L[i].tiempo_ejecucion > array_R[j].tiempo_ejecucion){

                    strcpy(array_registros[k].cadena, array_R[j].cadena);
                    array_registros[k].tiempo_ejecucion = array_R[j].tiempo_ejecucion;
                    strcpy(array_registros[k].fecha_ejecucion, array_R[j].fecha_ejecucion);
                    strcpy(array_registros[k].hora_ejecucion, array_R[j].hora_ejecucion);

                    /* arr[k] = R[j]; */
                    j++;
                    /*Hasta acá MergeSort con base en un sóla variable*/

                }else{
                    /*Si los tiempos de ejecución son iguales, entonces desempatar por lo siguiente*/

                    strcpy(fecha_hora_1, array_L[i].fecha_ejecucion);
                    strcat(fecha_hora_1, " ");
                    strcat(fecha_hora_1, array_L[i].hora_ejecucion);
                    /*printf("%s\n",fecha_hora_1);*/

                    strcpy(fecha_hora_2, array_R[j].fecha_ejecucion);
                    strcat(fecha_hora_2, " ");
                    strcat(fecha_hora_2, array_R[j].hora_ejecucion);

                    resultado_comparacion_strings = strcmp(fecha_hora_1, fecha_hora_2);

                    if( resultado_comparacion_strings < 0 ){
                      /*Si el elemento siguiente es mayor en fecha u hora*/

                        strcpy(array_registros[k].cadena, array_L[i].cadena);
                        array_registros[k].tiempo_ejecucion = array_L[i].tiempo_ejecucion;
                        strcpy(array_registros[k].fecha_ejecucion, array_L[i].fecha_ejecucion);
                        strcpy(array_registros[k].hora_ejecucion, array_L[i].hora_ejecucion);

                        /* arr[k] = L[i]; */
                        i++;

                    }else{

                        strcpy(array_registros[k].cadena, array_R[j].cadena);
                        array_registros[k].tiempo_ejecucion = array_R[j].tiempo_ejecucion;
                        strcpy(array_registros[k].fecha_ejecucion, array_R[j].fecha_ejecucion);
                        strcpy(array_registros[k].hora_ejecucion, array_R[j].hora_ejecucion);

                        /* arr[k] = R[j]; */
                        j++;

                    }
                }
            }

        }else{ /*Se va a ordenar de mayor a menor*/

          if (array_L[i].tiempo_ejecucion > array_R[j].tiempo_ejecucion){

              strcpy(array_registros[k].cadena, array_L[i].cadena);
              array_registros[k].tiempo_ejecucion = array_L[i].tiempo_ejecucion;
              strcpy(array_registros[k].fecha_ejecucion, array_L[i].fecha_ejecucion);
              strcpy(array_registros[k].hora_ejecucion, array_L[i].hora_ejecucion);

              /* arr[k] = L[i]; */
              i++;
          }else{

              if (array_L[i].tiempo_ejecucion < array_R[j].tiempo_ejecucion){

                  strcpy(array_registros[k].cadena, array_R[j].cadena);
                  array_registros[k].tiempo_ejecucion = array_R[j].tiempo_ejecucion;
                  strcpy(array_registros[k].fecha_ejecucion, array_R[j].fecha_ejecucion);
                  strcpy(array_registros[k].hora_ejecucion, array_R[j].hora_ejecucion);

                  /* arr[k] = R[j]; */
                  j++;
                  /*Hasta acá MergeSort con base en un sóla variable*/

              }else{
                  /*Si los tiempos de ejecución son iguales, entonces desempatar por lo siguiente*/

                  strcpy(fecha_hora_1, array_L[i].fecha_ejecucion);
                  strcat(fecha_hora_1, " ");
                  strcat(fecha_hora_1, array_L[i].hora_ejecucion);
                  /*printf("%s\n",fecha_hora_1);*/

                  strcpy(fecha_hora_2, array_R[j].fecha_ejecucion);
                  strcat(fecha_hora_2, " ");
                  strcat(fecha_hora_2, array_R[j].hora_ejecucion);

                  resultado_comparacion_strings = strcmp(fecha_hora_1, fecha_hora_2);

                  if( resultado_comparacion_strings > 0 ){
                    /*Si el elemento siguiente es mayor en fecha u hora*/

                      strcpy(array_registros[k].cadena, array_L[i].cadena);
                      array_registros[k].tiempo_ejecucion = array_L[i].tiempo_ejecucion;
                      strcpy(array_registros[k].fecha_ejecucion, array_L[i].fecha_ejecucion);
                      strcpy(array_registros[k].hora_ejecucion, array_L[i].hora_ejecucion);

                      /* arr[k] = L[i]; */
                      i++;

                  }else{

                      strcpy(array_registros[k].cadena, array_R[j].cadena);
                      array_registros[k].tiempo_ejecucion = array_R[j].tiempo_ejecucion;
                      strcpy(array_registros[k].fecha_ejecucion, array_R[j].fecha_ejecucion);
                      strcpy(array_registros[k].hora_ejecucion, array_R[j].hora_ejecucion);

                      /* arr[k] = R[j]; */
                      j++;

                  }
              }
          }

        }


        k++;
    }

    /* Copy the remaining elements of L[], if there
       are any */
    while (i < n1)
    {

        strcpy(array_registros[k].cadena, array_L[i].cadena);
        array_registros[k].tiempo_ejecucion = array_L[i].tiempo_ejecucion;
        strcpy(array_registros[k].fecha_ejecucion, array_L[i].fecha_ejecucion);
        strcpy(array_registros[k].hora_ejecucion, array_L[i].hora_ejecucion);

        /* arr[k] = L[i]; */
        i++;
        k++;
    }

    /* Copy the remaining elements of R[], if there
       are any */
    while (j < n2)
    {

        strcpy(array_registros[k].cadena, array_R[j].cadena);
        array_registros[k].tiempo_ejecucion = array_R[j].tiempo_ejecucion;
        strcpy(array_registros[k].fecha_ejecucion, array_R[j].fecha_ejecucion);
        strcpy(array_registros[k].hora_ejecucion, array_R[j].hora_ejecucion);

        /* arr[k] = R[j]; */
        j++;
        k++;
    }
    free(array_L);
    free(array_R);
}

void ImprimirArchivo(registro* array_registros, int numero_elementos, char nombre_archivo[], bool temporal){

     int i = 0;
     FILE *fptr;

     char archivo_nombre[maximo_nombre_archivo];
     strcpy(archivo_nombre, nombre_archivo);

     if (temporal == true){
          strcat(archivo_nombre,"_temporal");
     }

     fptr = fopen(archivo_nombre, "w");

     if(fptr == NULL){
        perror( archivo_nombre );
        free(array_registros);
        exit(1);
     }

     /*if( bandera_orden_reverso == false ){  /*De menor a mayor*/

         for (i = 0; i < numero_elementos; i++){
              fprintf(fptr,"%s", array_registros[i].cadena);
         }

     /*}else{ /*De mayor a menor

         for (i = numero_elementos; i > -1; i--){
              fprintf(fptr,"%s", array_registros[i].cadena);
         }
     /*}*/

     fclose(fptr);

}
