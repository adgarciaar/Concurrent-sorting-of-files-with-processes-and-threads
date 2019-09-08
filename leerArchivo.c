#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char **argv) {

    if(argc!=2){
        printf("Error con numero de argumentos\n");
        exit(1);
    }

    typedef struct {
      char cadena[255];
      int tiempoEjecucion;
      char fechaEjecucion[9];
      char horaEjecucion[9];
    } registro ;

    registro record[20000];

    FILE * archivo;
    /*nombreArchivo = argv[1];*/

    char linea[255];
    char lineaAux[255];

    char delim[] = " ";
  	char delim1[] = "\n";
  	int contador = 0;
    int numeroLinea = 0;

    char *item;

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

    int i = 0;
    for (i = 0; i < 20000; i++){

        strcpy(lineaAux, record[i].cadena);

        char *ptr;
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
    for (i = 0; i < 20000; i++){
      printf("Contador:%d\n",i);
      printf("%s",record[i].cadena);
      printf("%d ",record[i].tiempoEjecucion);
      printf("%s ",record[i].fechaEjecucion);
      printf("%s\n",record[i].horaEjecucion);
    }

    return 0;

}
