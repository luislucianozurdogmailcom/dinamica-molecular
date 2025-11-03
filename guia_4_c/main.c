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
double** CrearMatriz(int N, int dim, double L, double valorPorDefecto){
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
            if (valorPorDefecto != 0){
                matriz[i][j] = valorPorDefecto;
            }else{
                matriz[i][j] = uni()*L;   
            }
        }    
    }

    return matriz;
}

// Crear e inicializar tensor de NxN
double** TensorNN(int N){

    // Crea la matriz de distancias entre partículas al cuadrado
    double** matriz = (double**)malloc(N * sizeof(double*)); 
    for(int i = 0; i < N; i++) {
        matriz[i] = (double*)malloc(N * sizeof(double));
    }

    for (int j = 0; j < N; j++)
    {
        for (int i = 0; i < N; i++)
        {
            matriz[j][i] = 0;
        }    
    }

    return matriz;
}

// Crear e inicializar Tensor de NxNxDim
double*** TensorNND(int N, int dim){
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
        for (int i = 0; i < N; i++)
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

// Calcula las fuerzas entre partículas y llena los tensores correspondientes
void FuerzasEntreParticulas(int particula1, int particula2, double*** tensorFuerzas, double*** tensorPosiciones, double** tensorDistancias, double epsilon, double sigma, int dim){
    // Fuerzas que siente la partícula en cada eje por par de partículas

    for(int k = 0; k < dim; k++){
        // Almacenamos la fuerza en el tensor
        
        // Encima de la diagonal de la matriz con el signo positivo
        tensorFuerzas[particula1][particula2][k] = - ( tensorPosiciones[particula1][particula2][k]  / tensorDistancias[particula1][particula2] ) * DerivadaPotencialLJ(epsilon, sigma, tensorDistancias[particula1][particula2]); 
        
        // Debajo de la diagonal de la matriz signo cambiado debido a que es la reacción de la otra partícula
        tensorFuerzas[particula2][particula1][k] = - tensorFuerzas[particula1][particula2][k];
    }
}

// Calcula las distancias entre partículas y llena los tensores correspondientes
void DistanciasEntreParticulas(int particula1, int particula2, double*** tensorPosiciones, double** tensorDistancias, double** vectorPosiciones, int dim){
    // Calculo de la distancia para la dimensiones de cada par de partículas
    
    double distancia = 0;

    for(int k = 0; k < dim; k++){
        
        distancia += pow( vectorPosiciones[k][particula2] - vectorPosiciones[k][particula1] ,2);
        
        // Llenamos el tensor de posiciones
        tensorPosiciones[particula1][particula2][k] = vectorPosiciones[k][particula2] - vectorPosiciones[k][particula1];
        tensorPosiciones[particula2][particula1][k] = - tensorPosiciones[particula1][particula2][k];
    }
    
    // Almaceno la distancia en el tensor distancias
    distancia = sqrt(distancia);
    tensorDistancias[particula1][particula2] = distancia;
    tensorDistancias[particula2][particula1] = distancia;
}

// Algoritmo de verlet para posiciones
void Verlet(double deltaT, int dim, int N, int particula, double** vectorPosicion, double** vectorFuerzas, double** vectorPosicionAnterior, double*** tensorFuerzas, double** vectorMasas, double** vectorVelocidad){
    // Calculamos las posiciones con el algoritmo de verlet
    // En este mismo algoritmo se realiza la sumatoria total de fuerzas
    // sobre una misma partícula para posteriormente realizar verlett
    
    // Variable auxiliar para fuerzas
    double fuerzaTotal = 0; 
    double aceleracion = 0;
    
    // paso anterior de posiciones auxiliar
    double vectorPosicionAuxiliar  = 0;
    double vectorVelocidadAuxiliar = 0;

    for (int k = 0; k < dim; k++){
        
        // Inicializamos con cero
        fuerzaTotal = 0;

        for (int columna = 0; columna < N; columna++){
            
            // Acumulamos fuerzas para una dimensión
            fuerzaTotal += tensorFuerzas[particula][columna][k];
        }

        
        // Guardamos las fuerzas totales en el vectorFuerzas
        vectorFuerzas[k][particula] = fuerzaTotal;
        aceleracion                 = vectorFuerzas[k][particula]/vectorMasas[0][particula];
        
        // Guardamos la posicion y la velocidad anterior
        vectorVelocidadAuxiliar = vectorVelocidad[k][particula]; 
        vectorPosicionAuxiliar  = vectorPosicion[k][particula];

        // Calculamos la nueva posición
        vectorPosicion[k][particula] = 2 * vectorPosicion[k][particula] \
                                      - vectorPosicionAnterior[k][particula] \ 
                                + ( ( vectorFuerzas[k][particula] * pow(deltaT,2) ) / vectorMasas[0][particula] );

        // Igualamos la pos anterior a el vector correspondiente
        vectorPosicionAnterior[k][particula] = vectorPosicionAuxiliar;
    }
}

// Igualar matrices
void IgualarMatriz(double** matriz1, double** matriz2, int N, int dim){
    
    for (int i = 0; i < dim; i++){
        for (int j = 0; j < N; j++){
            matriz1[i][j] = matriz2[i][j];
        }
    }
}

// Impresión por pantalla de matrices para debbug
void ImpresionMatrices3D(int N, int dim, double*** matriz, char* nombreMatriz){
    
    printf("\n---------- Impresión de matriz 3D %s ------------\n", nombreMatriz);

    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            
            printf("[");
            for (int k = 0; k < dim; k++){
                printf( "%.2f ", matriz[i][j][k]);
            }
            printf("]");
            printf("\t");
        }
        printf("\n");
    }
}
void ImpresionMatrices2D(int N, int dim, double** matriz, char* nombreMatriz){
    
    printf("\n---------- Impresión de matriz 2D %s ------------\n", nombreMatriz);

    for (int i = 0; i < dim; i++){
        for (int j = 0; j < N; j++){
            printf( "%.6f ", matriz[i][j]);
            printf("\t");
        }
        printf("\n");
    }
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
    int pasosT     = LecturaInputInt("input/pasosT");     // Cantidad de pasos temporales
    double L       = LecturaInputDouble("input/L");       // Lado de caja cúbica
    double Reff    = LecturaInputDouble("input/Reff");    // Radio efectivo de partícula
    double epsilon = LecturaInputDouble("input/epsilon"); // Epsilon
    double sigma   = LecturaInputDouble("input/sigma");   // Sigma
    double deltaT  = LecturaInputDouble("input/deltaT");  // Paso temporal
    
    // Inicializamos los vectores r, v y f
    double** R         = CrearMatriz(N, dim, L, 0); // Vector de posiciones actuales
    double** Ranterior = CrearMatriz(N, dim, 0, 0); // Vector de posiciones paso temporal anterior
    double** V         = CrearMatriz(N, dim, 0, 0); // Vector de velocidades
    double** F         = CrearMatriz(N, dim, 0, 0); // Vector de Fuerzas
    double** M         = CrearMatriz(N, 1, 1, 1);   // Vector de masas al azar entre 0 y 1
    
    // Igualamos la matriz Ranterior a la actual R creada al azar
    IgualarMatriz(Ranterior, R, N, dim);

    // Inicializamos los tensores
    double**  Td = TensorNN(N);       // Tensor que almacena distancias calculadas
    double*** Tr = TensorNND(N, dim); // Tensor que almacena posiciones entre particulas
    double*** Tf = TensorNND(N, dim); // Tensor que almacena fuerzas entre partículas
    
    // Variables necesarias durante las iteraciones
    double potencial = 0;
    
    // Inicializamos el loop
    for (int iter = 0; iter < pasosT; iter++){

        // Iteramos por cada par de partículas
        for (int particula1 = 0; particula1 < N; particula1++){
            
            // Comparacion entre particula 1 y las demas
            for (int particula2 = 0; particula2 < particula1; particula2 ++){

                // Reseteamos la distancia para cada par de partículas
                potencial = 0;
                
                DistanciasEntreParticulas(particula1, particula2, Tr, Td, R, dim);
                
                // Cálculo del potencial de lennard-Jones ----------------------------------------
                // Si la distancia es mayor a la efectiva, no computamos la energía de interacción ni la fuerza
                if (Reff < Td[particula1][particula2]){
                    continue;    
                }
                
                // Potencial de Lennard-Jones
                // potencial = PotencialLJ(epsilon, sigma, distancia);
                
                // Calculamos las fuerzas entre partículas y se guarda en Tf
                FuerzasEntreParticulas(particula1, particula2, Tf, Tr, Td, epsilon, sigma, dim);
            }

        }
        
        // Debug
        ImpresionMatrices2D(N, N, Td, "Tensor de Distancias");
        //ImpresionMatrices2D(N, dim, R, "Vector de posiciones");
        //ImpresionMatrices2D(N, 1, M, "Vector de Masas");
        //ImpresionMatrices3D(N, dim, Tf, "Tensor de Fuerzas");
        //ImpresionMatrices2D(N, dim, F, "Vector de Fuerzas");
        //ImpresionMatrices3D(N, dim, Tr, "Tensor de Posiciones");
        
        // Con tensores calculados ahora hay que hacer calculos de posiciones
        for (int particula = 0; particula < N; particula++){

            Verlet(deltaT, dim, N, particula, R, F, Ranterior, Tf, M, V);
        }

        //ImpresionMatrices2D(N, dim, R, "Vector de posiciones");
        //ImpresionMatrices2D(N, 1, M, "Vector de Masas");
        //ImpresionMatrices2D(N, dim, Ranterior, "Vector de posiciones anterior");
        //ImpresionMatrices2D(N, dim, F, "Vector de Fuerzas");
    }
        
        finalize_random();
        return 0;
    }