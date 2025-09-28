#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "arrays_dinamicos.h"

// Función lectura de datos input
int input_reader(char* nombre_file){
    FILE *archivo;

    archivo = fopen(nombre_file,"r"); // "rb" es para leer archivos binarios

    // Chequeo de errores
    if (archivo == NULL){
        printf("Error al abrir el archivo %s \n", nombre_file);
        return 1;
    }

    // Variables para almacenar los datos
    int valor = 1;

    // Leer datos binarios
    // fread(puntero a datos a almacenar; tipo de dato a leer; tamaño de los datos; puntero a la información)
    fscanf(archivo, "%d", &valor);

    fclose(archivo);

    return valor;
}


int main() {
    printf("--------------Comenzamos con el programa------------------\n");

    // Inicializar la semilla del generador de números aleatorios
    srand(time(NULL));

    // Declaración de variables que usaremos
    double numeros_aleatorios[2];
    int int_cantidadNumeros = input_reader("input.dat");

    // Abrimos el archivo de salida
    FILE *file = fopen("output.dat","w"); // con 'a' agregamos al final

    // Chequeo de errores
    if (file == NULL){
        printf("Error al abrir el archivo output.dat para escritura \n");

        return 1;
    }

    // Cabecera del archivo output.dat
    fprintf(file, "X Y\n");

    // Rutina principal
    for (int i=0; i < int_cantidadNumeros; i++){
        
        // Generar dos números aleatorios entre 0 y 1
        numeros_aleatorios[0] = (double)rand() / RAND_MAX;
        numeros_aleatorios[1] = (double)rand() / RAND_MAX;
        
        if (numeros_aleatorios[0] < numeros_aleatorios[1]){
            
            // Imprimir por pantalla los números
            printf("%f, %f \n",numeros_aleatorios[0], numeros_aleatorios[1]);
            
            // Escribir los números en el archivo
            fprintf(file, "%f %f\n", numeros_aleatorios[0], numeros_aleatorios[1]);
        }
    }

    fclose(file);

    // Hacemos el punto D

    // Instanciamos las variables en donde guardaremos la info
    DynamicArray numeros_aleatorios_1;
    DynamicArray numeros_aleatorios_2;

    initArray(&numeros_aleatorios_1, int_cantidadNumeros);
    initArray(&numeros_aleatorios_2, int_cantidadNumeros);

    for (int i=0; i < int_cantidadNumeros; i++){
        
        // Generar dos números aleatorios entre 0 y 1
        numeros_aleatorios[0] = (double)rand() / RAND_MAX;
        numeros_aleatorios[1] = (double)rand() / RAND_MAX;

        if (numeros_aleatorios[0] < numeros_aleatorios[1] && numeros_aleatorios[1] > 0.5){
            insertArray(&numeros_aleatorios_1, numeros_aleatorios[0]);
            insertArray(&numeros_aleatorios_2, numeros_aleatorios[1]);
        }
    }

    // Liberamos memoria ajustando el vector al largo de los números almacenados
    trimToSize(&numeros_aleatorios_1);
    trimToSize(&numeros_aleatorios_2);

    // Abrimos el archivo de salida
    FILE *file_2 = fopen("output_2.dat","w"); // con 'a' agregamos al final

    // Ponemos los nombres de las cabeceras
    fprintf(file_2, "X Y\n");

    // Iteramos sobre el largo de los vectores
    for (int i=0; i<numeros_aleatorios_1.size; i++){
        
        // Guardamos la información en un archivo.
        fprintf(file_2, "%f %f\n", numeros_aleatorios_1.array[i], numeros_aleatorios_2.array[i]);
    }

    freeArray(&numeros_aleatorios_1);
    freeArray(&numeros_aleatorios_2);

    fclose(file_2);

    return 0;
}
