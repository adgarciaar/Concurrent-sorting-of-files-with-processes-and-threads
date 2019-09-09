#include "leerArchivo.h"
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
    int contador = 0;
    int numero_linea = 0;
    int i = 0;
    char *item;
    char *ptr;

    int numero_lineasArchivo = ContarLineasArchivo(argv[1]);

    /*registro array_registros[numero_lineasArchivo];*/
    registro* array_registros = (registro*)malloc(numero_lineasArchivo*sizeof(registro));

    if (array_registros == NULL) {
        printf("Memory not allocated.\n");
        exit(1);
    }

    if(argc!=2){
        printf("Error con numero de argumentos\n");
        free(array_registros);
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
       free(array_registros);
       perror ( argv[1] ); /* why didn't the file open? */
       exit(1);
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
        exit(1);
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

    /*for (i = 0; i < numero_elementos; i++){
      printf("Contador:%d\n",i);
      printf("%s",array_registros[i].cadena);
      printf("%d ",array_registros[i].tiempo_ejecucion);
      printf("%s ",array_registros[i].fecha_ejecucion);
      printf("%s\n",array_registros[i].hora_ejecucion);
    }*/

    for (i = 0; i < numero_elementos; i++){

      for (j = 0; j < numero_elementos-i-1; j++){

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
            /*if(array_registros[i].tiempo_ejecucion < array_registros[i+1].tiempo_ejecucion){*/

              /*Hay que intercambiar los elementos para que quede primero */

            /*}else{*/

            if(array_registros[j].tiempo_ejecucion == array_registros[j+1].tiempo_ejecucion){
                /*Si los tiempos de ejecución son iguales, entonces desempatar por lo siguiente*/

                strcpy(fecha_hora_1, array_registros[j].fecha_ejecucion);
                strcat(fecha_hora_1, " ");
                strcat(fecha_hora_1, array_registros[j].hora_ejecucion);
                /*printf("%s\n",fecha_hora_1);*/

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

                /*}else{
                    if( resultado_comparacion_strings < 0 ){
                */
                }

                /*}*/

            }
        }
      }
    }

    /*for (i = 0; i < numero_elementos; i++){
      printf("Contador:%d\n",i);
      printf("%s",array_registros[i].cadena);
      printf("%d ",array_registros[i].tiempo_ejecucion);
      printf("%s ",array_registros[i].fecha_ejecucion);
      printf("%s\n",array_registros[i].hora_ejecucion);
    }*/

}

void ImprimirArchivoTemporal(registro* array_registros, int numero_elementos, char nombre_archivo[]){

     int i = 0;
     FILE *fptr;
     fptr = fopen(strcat(nombre_archivo,"Temp"), "w");
     if(fptr == NULL){
        perror( strcat(nombre_archivo,"Temp") );
        free(array_registros);
        exit(1);
     }

     for (i = 0; i < numero_elementos; i++){
          fprintf(fptr,"%s", array_registros[i].cadena);
     }

     fclose(fptr);

}
