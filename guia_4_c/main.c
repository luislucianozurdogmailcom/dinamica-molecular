#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "ziggurat/ziggurat.h"

// Biblios para crear carpetas
#include <sys/stat.h>
#include <sys/types.h>


// Función encargada de leer archivos de entrada con datos INT
int LecturaInputInt(char *texto){
    FILE *archivo;

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

// Función encargada de leer archivos de entrada con datos DOUBLE
double LecturaInputDouble(char *texto){
    FILE *archivo;

    // "rb" es para leer archivos binarios y "r" normales
    archivo = fopen(texto,"r"); 

    // Chequeo de errores
    if (archivo == NULL){
        printf("Error al abrir el archivo %s \n", texto);
        return 1;
    }

    // Variables para almacenar los datos
    double valor = 0.0;

    // Leemos el numero int del archivo
    if (fscanf(archivo, "%lf", &valor) != 1) {
        printf("Error al leer el valor del archivo %s\n", texto);
        fclose(archivo);
        return 1;  // Return default seed value
    }

    fclose(archivo);

    return valor;
}

// Función encargada de escribir números a archivo(agregando no sobre escribiendo)
void EscrituraData(char *texto, double numero){
    FILE *archivo;

    // "rb" es para leer archivos binarios y "r" normales
    archivo = fopen(texto,"a"); 

    // Chequeo de errores
    if (archivo == NULL){
        printf("Error al abrir el archivo %s \n", texto);
        return;
    }

    fprintf(archivo, "%f ", numero); 
    
    fprintf(archivo, " \n ");

    fclose(archivo);
}

// Función encargada de escribir a archivo números de un array flexible
void EscrituraDataArrays(char *texto ,double *array, int tamano){
    FILE *archivo;

    // "rb" es para leer archivos binarios y "r" normales
    archivo = fopen(texto,"a"); 

    // Chequeo de errores
    if (archivo == NULL){
        printf("Error al abrir el archivo %s \n", texto);
        return;
    }

    for (int i=0; i<tamano; i++){
        fprintf(archivo, "%f ", array[i]); 
    }
    
    fprintf(archivo, "\n");

    fclose(archivo);
}

// Inicializar matriz de vectores
double** CrearMatriz(int N, int dim, double L){
    // Reserva memoria dinámica para una matriz de punteros a double, 
    // donde cada puntero representará una fila de la matriz de tamaño N.
    // Creará una matriz de dimxN con valores al azar entre 0 y L en cada 
    // posición.
    
    double** matriz = (double**)malloc(dim * sizeof(double*)); 
    for(int i = 0; i < N; i++) {
        matriz[i] = (double*)malloc(N * sizeof(double));
    }

    // Inicializamos el dominio aleatoriamente
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < N; j++)
        {
            matriz[i][j] = uni()*L;   
        }    
    }

    return matriz;
}

// Crear e inicializar matriz de distancias
double** CrearMatrizDistanciasCuadrado(int N, int dim, double** R){
    // Crea la matriz de distancias entre partículas al cuadrado
    
    double** matriz = (double**)malloc(N * sizeof(double*)); 
    for(int i = 0; i < N; i++) {
        matriz[i] = (double*)malloc(N * sizeof(double));
    }

    // Variable auxiliar
    double distanciaCuadrada = 0;

    // Lógica de calcular la distancia al cuadrado solo para las partículas que estan 
    // por encima de la diagonal de la matriz de distancias cuadradas.
    for (int j = 0; j < N; j++)
    {
        for (int i = 0; i < j; i++)
        {
            // Reseteamos la distancia para cada par de partículas
            distanciaCuadrada = 0;

            // Calculo de las tres dimensiones para cada partícula
            for(int k = 0; k < dim; k++){

                distanciaCuadrada += pow( R[k][i] - R[k][j] ,2);
            }

            // Guardamos la distancia cuadrada en la matriz.
            matriz[j][i] = distanciaCuadrada;
        }    
    }

    return matriz;
}

// Potencial de Lennard-Jones
double EnergiaPotencialLJ(double epsilon, double sigma, double** Rcuadrado, int N, int dim, double Reff){
    // Epsilon es la medida del pozo de potencial y da cuenta de la 
    // intensidad de la interacción.
    // Sigma es una medida del diámetro efectivo de las particulas
    // R Es la matriz de posiciones de las partículas.
    // N y dim son las constantes de las partículas y las dimensiones espaciales.
    double energia = 0;

    for (int particula1 = 0; particula1 < N; particula1++){
        
        for (int particula2 = 0; particula2 < particula1; particula2){

            // Si la distancia es mayor a la efectiva, no computamos la energía de interacción
            if (Reff < Rcuadrado[particula1][particula2]){continue;}

            // Potencial de Lennard-Jones
            energia += 4 * epsilon * ( - ( pow(sigma,6) ) / ( pow(Rcuadrado[particula1][particula2] ,3) ) + ( pow(sigma, 12) / pow(Rcuadrado[particula1][particula2], 6) ) );
        }
    }

    return energia;
}


char* NombreArchivo(char *texto, double T, int iteracion){
    /* Función encargada de fabricar el nombre del archivo con 
    los subindices necesarios para identificar al archivo y en que 
    carpeta irá alojado*/

    // Creamos el nombre del archivo
    char *nombreArchivo = (char*)malloc(256 * sizeof(char));
    sprintf(nombreArchivo, "output/T_%.4f/%d_%s", T, iteracion, texto);

    // Retornamos
    return nombreArchivo;
}

// Rutina principal
int main() {

    // Inicializamos ziggurat
    initialize_random();

    // Leemos los archivos de entrada
    int N          = LecturaInputInt("input/N");          // Cantidad de partículas 
    int dim        = 3;                                   // Dimensiones espaciales
    double L       = LecturaInputDouble("input/L");       // Lado de caja cúbica
    double Reff    = LecturaInputDouble("input/Reff");    // Radio efectivo de partícula
    double epsilon = LecturaInputDouble("input/epsilon"); // Epsilon
    double sigma   = LecturaInputDouble("input/sigma");   // Sigma

    
    // Inicializamos los vectores r, v y f
    double** R = CrearMatriz(N, dim, L); // Vector de posiciones
    double** V = CrearMatriz(N, dim, L); // Vector de velocidades
    double** F = CrearMatriz(N, dim, L); // Vector de Fuerzas


    // Variables necesarias durante las iteraciones
    double distanciaCuadrada = 0;
    double energia           = 0;

    // Iteramos por cada par de partículas
    for (int particula1 = 0; particula1 < N; particula1++){

        for (int particula2 = 0; particula2 < particula1; particula2 ++){

            // Reseteamos la distancia para cada par de partículas
            distanciaCuadrada = 0;

            // Calculo de las tres dimensiones para cada partícula
            for(int k = 0; k < dim; k++){

                distanciaCuadrada += pow( R[k][particula1] - R[k][particula2] ,2);
            }

            // Cálculo del potencial de lennard-Jones ----------------------------------------
            // Si la distancia es mayor a la efectiva, no computamos la energía de interacción
            if (Reff > distanciaCuadrada){
                
                // Potencial de Lennard-Jones
                energia += 4 * epsilon * ( - ( pow(sigma,6) ) / ( pow(distanciaCuadrada ,3) ) + ( pow(sigma, 12) / pow(distanciaCuadrada, 6) ) );
            }

            // Cálculo de la fuerza total sobre cada partícula
            

        }
    }


    
    finalize_random();
    return 0;
}