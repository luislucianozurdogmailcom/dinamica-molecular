#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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


    return 0;
}
