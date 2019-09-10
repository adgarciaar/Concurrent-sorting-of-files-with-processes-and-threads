#include "mergeSortProyecto.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int ContarLineasArchivo(char nombre_archivo[]);
void OrdenarRegistro(registro* array_registros, int numero_elementos);
void ImprimirArchivoTemporal(registro* array_registros, int numero_elementos, char nombre_archivo[]);

int main (int argc, char **argv) {

    FILE * archivo;

    char linea[max_tamano_cadena];
    char linea_aux[max_tamano_cadena];

    char delim[] = " ";
    char delim1[] = "\n";
    int contador = 0;
    int numero_linea = 0;
    int i = 0;
    char *item;
    char *ptr;

    int numero_lineasArchivo = ContarLineasArchivo(argv[1]);

    /*registro array_registros[numero_lineasArchivo];*/
    registro* array_registros = (registro*)malloc(numero_lineasArchivo*sizeof(registro));

    if(argc!=2){
        printf("Error con numero de argumentos\n");
        exit(1);
    }

    if (array_registros == NULL) {
        printf("Memory not allocated.\n");
        exit(1);
    }

    archivo = fopen(argv[1],"r");

    if ( archivo != NULL ){
      /* Read file line by line */

      while (fgets(linea,sizeof(linea),archivo)) {
          strcpy(array_registros[numero_linea].cadena,linea);
          /*printf("%s",array_registros[numero_linea].cadena);*/
          numero_linea = numero_linea + 1;
      }
      fclose(archivo);/* Close file */

    }else{
       perror ( argv[1] ); /* why didn't the file open? */
    }


    for (i = 0; i < numero_lineasArchivo; i++){

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

    OrdenarRegistro(array_registros, numero_lineasArchivo);

    /*i = 0;
    for (i = 0; i < numero_lineasArchivo; i++){
      printf("Contador:%d\n",i);
      printf("%s",array_registros[i].cadena);
      printf("%d ",array_registros[i].tiempo_ejecucion);
      printf("%s ",array_registros[i].fecha_ejecucion);
      printf("%s\n",array_registros[i].hora_ejecucion);
    }*/

    ImprimirArchivoTemporal(array_registros, numero_lineasArchivo, argv[1]);

    free(array_registros);

    return 0;

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

void OrdenarRegistro(registro* array_registros, int numero_elementos){

    int i = 0, j = 0;
    int resultado_comparacion_strings;
    char fecha_hora_1[tamano_fila_fecha_hora+tamano_fila_fecha_hora+1];
    char fecha_hora_2[tamano_fila_fecha_hora+tamano_fila_fecha_hora+1];
    registro registro_auxiliar;

    mergeSort(array_registros, 0, numero_elementos - 1);

}

/* l is for left index and r is right index of the
   sub-array of arr to be sorted */
void mergeSort(registro* array_registros, int l, int r){
    if (l < r)
    {
        /* Same as (l+r)/2, but avoids overflow for
        /* large l and h */
        int m = l+(r-l)/2;

        /* Sort first and second halves*/
        mergeSort(array_registros, l, m);
        mergeSort(array_registros, m+1, r);

        merge(array_registros, l, m, r);
    }
}

/* Merges two subarrays of arr[].
// First subarray is arr[l..m]
// Second subarray is arr[m+1..r] */
void merge(registro* array_registros, int l, int m, int r){
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;

    int resultado_comparacion_strings;
    char fecha_hora_1[tamano_fila_fecha_hora+tamano_fila_fecha_hora+1];
    char fecha_hora_2[tamano_fila_fecha_hora+tamano_fila_fecha_hora+1];

    /* create temp arrays */
    /*int L[n1], R[n2];*/
    registro* array_L = (registro*)malloc(n1*sizeof(registro));
    registro* array_R = (registro*)malloc(n2*sizeof(registro));

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
        /*if (L[i] <= R[j]) /*de menor a mayor*/
        if (array_L[i].tiempo_ejecucion < array_R[j].tiempo_ejecucion)
        {

            strcpy(array_registros[k].cadena, array_L[i].cadena);
            array_registros[k].tiempo_ejecucion = array_L[i].tiempo_ejecucion;
            strcpy(array_registros[k].fecha_ejecucion, array_L[i].fecha_ejecucion);
            strcpy(array_registros[k].hora_ejecucion, array_L[i].hora_ejecucion);

            /* arr[k] = L[i]; */
            i++;
        }
        else
        {

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

void ImprimirArchivoTemporal(registro* array_registros, int numero_elementos, char nombre_archivo[]){

     int i = 0;
     FILE *fptr;
     fptr = fopen(strcat(nombre_archivo,"Temp"), "w");
     if(fptr == NULL){
        printf("Error!");
        exit(1);
     }

     for (i = 0; i < numero_elementos; i++){
          fprintf(fptr,"%s", array_registros[i].cadena);
     }

     fclose(fptr);

}
