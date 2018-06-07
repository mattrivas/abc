#include <stdio.h>
#include <stdlib.h>

void almacenar(int iteraccion, double mejor_solucion, double mejor_fuente){
    FILE *fp;
    char nombre[50], linea[100];
    short i;
    int j;
    fp = fopen ("./home/silvana/Codigo_Fuente/resultados.txt", "a+");

    fprintf(fp, "%d\t", iteraccion);
    fprintf(fp, "%f\t", mejor_solucion);
    fprintf(fp, "%f\t", mejor_fuente);

    printf("%d\t%f\t%f\t\n", iteraccion, mejor_solucion, mejor_fuente);

    fclose(fp);
    printf("Datos almacenados correctamente\n");
}
