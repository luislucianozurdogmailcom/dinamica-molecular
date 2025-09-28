#include <stdio.h>
#include <math.h>
#include "ziggurat/ziggurat.h"


// Función encargada de leer el archivo input.c
int lectura_input(){
    FILE *archivo;
    char *texto = "input.dat";

    // "rb" es para leer archivos binarios y "r" normales
    archivo = fopen(texto,"r"); 

    // Chequeo de errores
    if (archivo == NULL){
        printf("Error al abrir el archivo %s \n", texto);
        return 1;
    }

    // Variables para almacenar los datos
    int valor = 1;

    // Leemos el numero int del archivo
    if (fscanf(archivo, "%d", &valor) != 1) {
        printf("Error al leer el valor del archivo %s\n", texto);
        fclose(archivo);
        return 1;  // Return default seed value
    }

    fclose(archivo);

    return valor;
}

// Función encargada de escribir números (agregando no sobre escribiendo)
void escritura_data(char *texto, double *numeros, int tamano){
    FILE *archivo;

    // "rb" es para leer archivos binarios y "r" normales
    archivo = fopen(texto,"a"); 

    // Chequeo de errores
    if (archivo == NULL){
        printf("Error al abrir el archivo %s \n", texto);
        return;
    }

    // Escribimos el archivo con el nuevo número aleatorio
    for (int i=0; i < tamano; i++){

        fprintf(archivo, "%f ", numeros[i]); 
    }
    
    fprintf(archivo, " \n ");

    fclose(archivo);
}

// Función del círculo
double f(double x){
    return sqrt(1-pow(x,2));
}

int main() {

    // Inicializamos ziggurat
    initialize_random();

    // Borramos archivos que vamos a crear nuevamente
    remove("cociente_puntos.dat");

    // Inicialización de variables
    double punto[2], cociente[0];
    int puntos_totales;
    int contador_aceptados = 0;


    for (puntos_totales = 100; puntos_totales < 10000000; puntos_totales = puntos_totales + 1000){
        
        contador_aceptados = 0;

        // iteramos para evaluar puntos con distribución normal.
        for (int i = 0; i<puntos_totales; i++){
            punto[0] = uni();
            punto[1] = uni();
            
            if (f(punto[0]) > punto[1]){ // Evaluamos en la función SQRT(1-x²)
                contador_aceptados++;
            }
        }

        cociente[0] = (double) contador_aceptados/(double) puntos_totales;
        escritura_data("cociente_puntos.dat", cociente, 1);
    }

    finalize_random();
    return 0;
}