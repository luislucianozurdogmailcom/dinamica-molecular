#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "ziggurat/ziggurat.h"


// Función encargada de leer el archivo input.c
int lectura_input(char *texto){
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

// Inicializar el dominio
int** crearDominio(int N){
    // Reserva memoria dinámica para una matriz de punteros a double, 
    // donde cada puntero representará una fila de la matriz de tamaño N.
    // Creará una matriz de NxN con valores al azar entre 0 y 1 en cada 
    // posición.
    
    int** matriz = (int**)malloc(N * sizeof(int*)); 
    for(int i = 0; i < N; i++) {
        matriz[i] = (int*)malloc(N * sizeof(int));
    }

    // Inicializamos el dominio aleatoriamente
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            matriz[i][j] = (int)round(uni()); // Numero al azar redondeado a 0 o 1
        }    
    }

    return matriz;
}

// Medir magnetización
int magnetizacion(int** matriz, int tamano){
    
    //Función encargada de medir la magnetización del sistema en su totalidad
    //como magnitud física definida para un número finito de particulas en nuestra
    //matriz 2D

    int valor = 0;

    for (int i = 0; i< tamano; i++){
        for (int j = 0; j < tamano; j++){
            valor = valor + matriz[i][j];
        }
    }

    return valor;
}

// Medir magnetización media
double magnetizacionMedia(int** matriz, int tamano){
    
    //Función encargada de medir la magnetización del sistema en su totalidad
    //como magnitud física definida para un número finito de particulas en nuestra
    //matriz 2D

    double valor = 0;

    for (int i = 0; i< tamano; i++){
        for (int j = 0; j < tamano; j++){
            valor = valor + (double)matriz[i][j];
        }
    }
    
    valor = valor/pow((double)tamano,2);
    
    return valor;
}

// Función para no salirnos de límites de matrices
int bordeInfinito(int indice, int tamano){
    
    // Esta función está encargada de hacer que nuestros índices no se
    // salgan de los bordes de nuestras matrices. Esto nos ayuda a implementar
    // condiciones de borde periódicas, haciendo que un indice N+1 estando 
    // en el borde sea un 0 para una matriz de largo N. 

    // índice que se pasa del tamaño de la matriz.
    if (indice + 1 > tamano){
        return 0;
    }
    
    // índice que se pasa negativamente del tamaño de la matriz
    if (indice < 0){
        return tamano - 1; 
    }

    // índice que está correcto y no genera problemas:
    return indice;
}

// Medir energia
double energia(int** matriz, int tamano, int J){
    
    // Función encargada de medir la energia total del
    // sistema de particulas 2D del modelo de izing

    int valor, iPlus1, iMinus1, jPlus1, jMinus1;

    for ( int i = 0; i < tamano; i++){
        for( int j = 0; j < tamano; j++){
            
            iPlus1  = bordeInfinito(i+1, tamano);
            iMinus1 = bordeInfinito(i-1, tamano);
            jPlus1  = bordeInfinito(i+1, tamano);
            jMinus1 = bordeInfinito(i-1, tamano);

            valor = valor\ 
                  + matriz[i][j] * matriz[iPlus1][j] \
                  + matriz[i][j] * matriz[iMinus1][j] \
                  + matriz[i][j] * matriz[i][jPlus1] \
                  + matriz[i][j] * matriz[i][jMinus1];

        }
    }

    double valor_return = (double)(valor * (-J)) / 2;

    return valor_return;
}

// Cambio de estado spin_single_flip
int** spinSingleFlip(int** matriz, int tamano){
    // Función encargada de voltear al azar un solo spin de la matriz 
    // de spines que tiene como entrada.
    
    // Creamos la matriz dinámica nueva
    int** matrizNueva = (int**)malloc(tamano * sizeof(int*));

    for(int i = 0; i < tamano; i++) {
        matrizNueva[i] = (int*)malloc(tamano * sizeof(int));
    }

    // Copiamos los datos de la matriz original
    for (int i = 0; i < tamano; i++) {
        for (int j = 0; j < tamano; j++) {
            matrizNueva[i][j] = matriz[i][j];
        }
    }

    // Calculamos al azar la posición del Spin que cambiará
    int fila = (int)round(uni()*tamano);
    int colu = (int)round(uni()*tamano);

    // Cambiamos el spin multiplicandoló por -1
    matrizNueva[fila][colu] *= -1;

    return matrizNueva;
}

// Aceptar o no el paso al siguiente estado
int aceptabilidad(int** matriz, int** matrizNueva, double T){
    // Función encargada de calcular la probabilidad de aprobar el cambio
    // de estado de un estado al siguiente.
    
    // Constantes del calculo
    double K          = 1.380649e-23;
    double beta       = 1/(K*T);

    // Energias
    double energia       = energia(matriz);
    double energiaNueva  = energia(matrizNueva);

    // Dependiendo que energia tenga el nuevo estado, la probabilidad de cambio será diferente
    if (energia > energiaNueva)
    {
        return 1; // se aprueba el estado de una ya que la energia es menor
    }
    else // Caso en el que la energia nueva es mayor que la energia anterior 
    {
        // Calculamos un número al azar entre 0 a 1.
        probabilidad = uni();

        // e^{beta*|Energia_nueva - Energia|}
        if (probabilidad <= exp(beta*(abs(energia_nueva - energia)))) 
        {
            return 1; // Se aprueba el estado al lanzar la moneda
        }
        else 
        {
            return 0; // Se niega el cambio de estado por probabilidad
        }
    }
}

int main() {

    // Inicializamos ziggurat
    initialize_random();

    // Leemos el tamaño de la grilla y la temperatura
    int N    = lectura_input("N");
    double T = lectura_input("T");

    // Creamos el dominio y lo llenamos con datos random
    int** dominio = crearDominio(N);

    // Calculamos su magnetización
    printf("Su magnetización inicial es: %d \n", magnetizacion(dominio, N));
    printf("Su magnetización media incial es: %f \n", magnetizacionMedia(dominio, N));
    printf("Su energia incial es: %f \n", energia(dominio, N, 1));

    // Bucle infinito
    int** dominioFuturo;
    for (;;){
        dominioFuturo = spinSingleFlip(dominio);

        if (aceptabilidad(dominio, dominioFuturo, T))
        {
            
        }
    }

    finalize_random();
    return 0;
}