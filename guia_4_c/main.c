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
double** TensorDistancias(int N, int dim){

    // Crea la matriz de distancias entre partículas al cuadrado
    double** matriz = (double**)malloc(N * sizeof(double*)); 
    for(int i = 0; i < N; i++) {
        matriz[i] = (double*)malloc(N * sizeof(double));
    }

    for (int j = 0; j < N; j++)
    {
        for (int i = 0; i < j; i++)
        {
            matriz[j][i] = 0;
        }    
    }

    return matriz;
}

// Crear e inicializar matriz de fuerzas
double*** TensorFuerzas(int N, int dim){
    // Crea la matriz de fuerzas entre partículas y almacena cada componente
    // x, y z de la fuerza

    double*** matriz3D = (double***)malloc(N * sizeof(double**)); 
    
    for(int i = 0; i < N; i++) {
        
        matriz3D[i] = (double**)malloc(N * sizeof(double*));

        for (int j = 0; j < N; j++){
            matriz3D[i][j] = (double*)malloc(dim*sizeof(double));
        }
    }

    // Reseteamos matriz
    for (int j = 0; j < N; j++)
    {
        for (int i = 0; i < j; i++)
        {
            for (int k = 0; k < dim; k++)
            matriz3D[j][i][k] = 0;
        }    
    }

    return matriz3D;
}

// Potencial de Lennard-Jones
double PotencialLJ(double epsilon, double sigma, double distancia){
    // Epsilon es la medida del pozo de potencial y da cuenta de la 
    // intensidad de la interacción.
    // Sigma es una medida del diámetro efectivo de las particulas
    
    return 4 * epsilon * ( - ( pow( sigma / distancia, 6) ) + ( pow( sigma / distancia, 12) ) );
}

// Derivada en r del Potencial de Lennard-Jones
double DerivadaPotencialLJ(double epsilon, double sigma, double distancia){
    // Epsilon es la medida del pozo de potencial y da cuenta de la 
    // intensidad de la interacción.
    // Sigma es una medida del diámetro efectivo de las particulas
    
    return 4 * epsilon * ( 6 * ( pow( sigma, 6 ) / pow( distancia, 7 ) ) - 12 * ( pow( sigma, 12) / pow(distancia, 13) ) );
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
    int dim        = LecturaInputInt("input/dim");        // Dimensiones espaciales
    double L       = LecturaInputDouble("input/L");       // Lado de caja cúbica
    double Reff    = LecturaInputDouble("input/Reff");    // Radio efectivo de partícula
    double epsilon = LecturaInputDouble("input/epsilon"); // Epsilon
    double sigma   = LecturaInputDouble("input/sigma");   // Sigma

    
    // Inicializamos los vectores r, v y f
    double** R = CrearMatriz(N, dim, L); // Vector de posiciones
    double** V = CrearMatriz(N, dim, L); // Vector de velocidades
    double** F = CrearMatriz(N, dim, L); // Vector de Fuerzas

    // Inicializamos los tensores
    double**  Tr = TensorDistancias(N, dim); // Tensor que almacena las distancias entre particulas
    double*** Tf = TensorFuerzas(N, dim);    // Tensor que almacena las fuerzas entre partículas

    // Variables necesarias durante las iteraciones
    double distancia = 0;
    double potencial = 0;

    // Iteramos por cada par de partículas
    for (int particula1 = 0; particula1 < N; particula1++){

        // Comparacion entre particula 1 y las demas
        for (int particula2 = 0; particula2 < particula1; particula2 ++){

            // Reseteamos la distancia para cada par de partículas
            distancia = 0;
            potencial = 0;

            // Calculo de la distancia para la dimensiones de cada par de partículas
            for(int k = 0; k < dim; k++){

                distancia += pow( R[k][particula1] - R[k][particula2] ,2);
            }
            distancia = pow(distancia, 0.5);

            // Almaceno la distancia en el tensor:
            Tr[particula1][particula2] = distancia;

            // Cálculo del potencial de lennard-Jones ----------------------------------------
            // Si la distancia es mayor a la efectiva, no computamos la energía de interacción ni la fuerza
            if (Reff < distancia){
                continue;    
            }
            
            // Potencial de Lennard-Jones
            potencial = PotencialLJ(epsilon, sigma, distancia);

            // Fuerzas que siente la partícula en cada eje 
            for(int k = 0; k < dim; k++){

                // Almacenamos la fuerza en el tensor
                Tf[particula1][particula2][k] += - ( ( R[k][particula2] - R[k][particula1] )  / distancia ) * DerivadaPotencialLJ(epsilon, sigma, distancia);
            }
        }
    }

    // Con tensores calculados ahora hay que hacer:

    
    finalize_random();
    return 0;
}