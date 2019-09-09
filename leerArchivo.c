#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*#include <limits.h>*/
#include "leerArchivo.h"
/*#define maxNombreArchivo 100
#define maxTamanoCadena 255
#define maxNumeroRegistros 20000*/

int ContarLineasArchivo(char filename[maxNombreArchivo]);
void OrdenarRegistro(registro record[], int numeroElementos);

int main (int argc, char **argv) {

    FILE * archivo;

    char linea[maxTamanoCadena];
    char lineaAux[maxTamanoCadena];

    char delim[] = " ";
    char delim1[] = "\n";
    int contador = 0;
    int numeroLinea = 0;
    int i = 0;
    char *item;
    char *ptr;

    int numeroLineasArchivo = ContarLineasArchivo(argv[1]);

    /*typedef struct {
      char cadena[255];
      int tiempoEjecucion;
      char fechaEjecucion[9];
      char horaEjecucion[9];
    } registro ;*/

    registro record[numeroLineasArchivo];

    if(argc!=2){
        printf("Error con numero de argumentos\n");
        exit(1);
    }

    archivo = fopen(argv[1],"r");

    if ( archivo != NULL ){
      /* Read file line by line */

      while (fgets(linea,sizeof(linea),archivo)) {
          strcpy(record[numeroLinea].cadena,linea);
          /*printf("%s",record[numeroLinea].cadena);*/
          numeroLinea = numeroLinea + 1;
      }
      fclose(archivo);/* Close file */

    }else{
       perror ( argv[1] ); /* why didn't the file open? */
    }


    for (i = 0; i < numeroLineasArchivo; i++){

        strcpy(lineaAux, record[i].cadena);

        if( (ptr = strchr(lineaAux, '\n')) != NULL)
          *ptr = '\0';

        item = strtok(lineaAux, delim);
        contador = 1;
        while(item != NULL){
            /*printf("Contador: %d\n",contador);
            printf("%s\n",item);*/
            if (contador == 4){
                record[i].tiempoEjecucion = atoi(item);
                /*printf("%d\n",record[i].tiempoEjecucion);*/
            }
            if (contador == 5){
                strcpy(record[i].fechaEjecucion,item);
                /*printf("%s\n",record[i].fechaEjecucion);*/
            }
            if (contador == 6){
                strcpy(record[i].horaEjecucion,item);
                /*printf("%s\n",record[i].horaEjecucion);*/
                item = NULL;
            }

            contador = contador+1;
            item = strtok(NULL, delim);
        }

    }

    i = 0;
    /*for (i = 0; i < numeroLineasArchivo; i++){
      /*printf("Contador:%d\n",i);
      printf("%s",record[i].cadena);
      printf("%d ",record[i].tiempoEjecucion);
      printf("%s ",record[i].fechaEjecucion);
      printf("%s\n",record[i].horaEjecucion);
    }*/

    OrdenarRegistro(record, numeroLineasArchivo);

    return 0;

}

int ContarLineasArchivo(char filename[maxNombreArchivo]){
    FILE *fp;
    int count = 0;  /* Line counter (result) */
    /*char filename[MAX_FILE_NAME];*/
    char c;  /* To store a character read from file*/

    /* Open the file */
    fp = fopen(filename, "r");

    /* Check if file exists */
    if (fp == NULL){
        perror ( filename ); /* why didn't the file open? */
    }

    /* Extract characters from file and store in character c */
    for (c = getc(fp); c != EOF; c = getc(fp))
        if (c == '\n') /* Increment count if this character is newline */
            count = count + 1;

    /* Close the file */
    fclose(fp);
    /*printf("The file %s has %d lines\n ", filename, count);*/

    return count;
}

void OrdenarRegistro(registro record[], int numeroElementos){

    int i = 0, j = 0;
    int resultadoComparacionStrings;
    char fecha_hora_1[tamanoFilaFechaHora+tamanoFilaFechaHora+1];
    char fecha_hora_2[tamanoFilaFechaHora+tamanoFilaFechaHora+1];
    registro registroAuxiliar;

    /*for (i = 0; i < numeroElementos; i++){
      printf("Contador:%d\n",i);
      printf("%s",record[i].cadena);
      printf("%d ",record[i].tiempoEjecucion);
      printf("%s ",record[i].fechaEjecucion);
      printf("%s\n",record[i].horaEjecucion);
    }*/

    for (i = 0; i < numeroElementos; i++){

      for (j = 0; j < numeroElementos-i-1; j++){

        if(record[j].tiempoEjecucion > record[j+1].tiempoEjecucion){

            /*Hay que intercabiar los elementos para que quede primero el menor*/

            /*Salvar los datos del que tiene menor tiempo de ejecución*/
            strcpy(registroAuxiliar.cadena, record[j+1].cadena);
            registroAuxiliar.tiempoEjecucion = record[j+1].tiempoEjecucion;
            strcpy(registroAuxiliar.fechaEjecucion, record[j+1].fechaEjecucion);
            strcpy(registroAuxiliar.horaEjecucion, record[j+1].horaEjecucion);

            /*Cambiar elemento con menor tiempo por el otro*/
            strcpy(record[j+1].cadena, record[j].cadena);
            record[j+1].tiempoEjecucion = record[j].tiempoEjecucion;
            strcpy(record[j+1].fechaEjecucion, record[j].fechaEjecucion);
            strcpy(record[j+1].horaEjecucion, record[j].horaEjecucion);

            /*Guardar los datos temporales para terminar intercambio*/
            strcpy(record[j].cadena, registroAuxiliar.cadena);
            record[j].tiempoEjecucion = registroAuxiliar.tiempoEjecucion;
            strcpy(record[j].fechaEjecucion, registroAuxiliar.fechaEjecucion);
            strcpy(record[j].horaEjecucion, registroAuxiliar.horaEjecucion);


        }else{
            /*if(record[i].tiempoEjecucion < record[i+1].tiempoEjecucion){*/

              /*Hay que intercambiar los elementos para que quede primero */

            /*}else{*/

            if(record[j].tiempoEjecucion == record[j+1].tiempoEjecucion){
                /*Si los tiempos de ejecución son iguales, entonces desempatar por lo siguiente*/

                strcpy(fecha_hora_1, record[j].fechaEjecucion);
                strcat(fecha_hora_1, " ");
                strcat(fecha_hora_1, record[j].horaEjecucion);
                /*printf("%s\n",fecha_hora_1);*/

                strcpy(fecha_hora_2, record[j+1].fechaEjecucion);
                strcat(fecha_hora_2, " ");
                strcat(fecha_hora_2, record[j+1].horaEjecucion);

                resultadoComparacionStrings = strcmp(fecha_hora_1, fecha_hora_2);

                if( resultadoComparacionStrings > 0 ){
                  /*Si el elemento siguiente es mayor en fecha u hora*/

                  /*Hay que intercabiar los elementos para que quede primero el menor*/

                  /*Salvar los datos del que tiene menor tiempo de ejecución*/
                  strcpy(registroAuxiliar.cadena, record[j+1].cadena);
                  registroAuxiliar.tiempoEjecucion = record[j+1].tiempoEjecucion;
                  strcpy(registroAuxiliar.fechaEjecucion, record[j+1].fechaEjecucion);
                  strcpy(registroAuxiliar.horaEjecucion, record[j+1].horaEjecucion);

                  /*Cambiar elemento con menor tiempo por el otro*/
                  strcpy(record[j+1].cadena, record[j].cadena);
                  record[j+1].tiempoEjecucion = record[j].tiempoEjecucion;
                  strcpy(record[j+1].fechaEjecucion, record[j].fechaEjecucion);
                  strcpy(record[j+1].horaEjecucion, record[j].horaEjecucion);

                  /*Guardar los datos temporales para terminar intercambio*/
                  strcpy(record[j].cadena, registroAuxiliar.cadena);
                  record[j].tiempoEjecucion = registroAuxiliar.tiempoEjecucion;
                  strcpy(record[j].fechaEjecucion, registroAuxiliar.fechaEjecucion);
                  strcpy(record[j].horaEjecucion, registroAuxiliar.horaEjecucion);

                /*}else{
                    if( resultadoComparacionStrings < 0 ){
                */
                }

                /*}*/

            }
        }
      }
    }

    for (i = 0; i < numeroElementos; i++){
      printf("Contador:%d\n",i);
      printf("%s",record[i].cadena);
      printf("%d ",record[i].tiempoEjecucion);
      printf("%s ",record[i].fechaEjecucion);
      printf("%s\n",record[i].horaEjecucion);
    }

}
