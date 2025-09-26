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

// Función de probabilidad
double f(double x){
    return (x-1)*(x-1);
}

int main() {

    // Inicializamos ziggurat
    initialize_random();

    // A) Utilizando los comandos open, read y close, logre que el programa 
    // lea de un archivo externo (’input.dat’) la cantidad de puntos aleatorios 
    // N que se van a generar. No se olvide de crear el archivo ’input.dat’ y 
    // escribir un numero entero en el.
    int input = lectura_input();


    // Escriba un programa que genere N = 1000 pares de numeros aleatorios 
    // (X, Y ) a partir de la 
    // función uni() de ziggurat.f90, que cumplan 0 < X < 2 y 0 < Y < 1.
    double numeros[2];

    // Borramos el archivo anterior
    remove("numeros.dat");

    // C)  Escriba una rutina que acepte (escriba a archivo) el número aleatorio 
    // X, si Y ≤ f(x). ¿Puede estimar que fracción de números será aceptado?
    for (int i=0; i<input; i++){
        numeros[0] = uni()*2; // Uni genera numeros entre 0 y 1
        numeros[1] = uni();

        // printf("(%f,%f)",numeros[0], numeros[1]);

        if (numeros[1] <= f(numeros[0])){ // Si y es <= que f(x)
            int tamano = sizeof(numeros) / sizeof(numeros[0]); // Esto da 2 para tu array de 2 elementos
            escritura_data("numeros.dat", numeros, tamano);
        }

    }
    
    finalize_random();
    return 0;
}