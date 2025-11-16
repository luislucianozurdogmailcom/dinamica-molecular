#ifndef FISICA_H
#define FISICA_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "../ziggurat/ziggurat.h"

// Función encargada de leer archivos de entrada con datos INT
int LecturaInputInt(char *texto);

// Función encargada de leer archivos de entrada con datos DOUBLE
double LecturaInputDouble(char *texto);

// Función encargada de escribir números a archivo(agregando no sobre escribiendo);
void EscrituraData(char *texto, double numero);

// Funciones para abrir y cerrar archivos facilmente
FILE* AperturaArchivo(char *texto);
void CierreArchivo(FILE *archivo);

// Función encargada de escribir a archivo números de un array flexible
void EscrituraDataArrays(char *texto ,double *array, int tamano);

// Inicializar matriz de vectores
double** CrearMatriz(int N, int dim, double L, double valorPorDefecto);

// Crear e inicializar tensor de NxN
double** TensorNN(int N);

// Crear e inicializar tensor de NxN pero INT
int** TensorNNint(int N);

// Crear e inicializar Tensor de NxNxDim
double*** TensorNND(int N, int dim);

// Potencial de Lennard-Jones
double PotencialLJ(double epsilon, double sigma, double distancia, double potencialCut, double sigmaSeis, double sigmaDoce);

// Derivada en r del Potencial de Lennard-Jones
double DerivadaPotencialLJ(double epsilon, double sigma, double distancia, double sigmaSeis, double sigmaDoce);

// Calcula las fuerzas entre partículas y llena los tensores correspondientes
void FuerzasEntreParticulas(int particula1,
                            int particula2, 
                            double*** tensorFuerzas, 
                            double*** tensorPosiciones, 
                            double** tensorDistancias, 
                            double epsilon, 
                            double sigma, 
                            int dim, 
                            double reff,
                            double sigmaSeis,
                            double sigmaDoce);

// Calcula las distancias entre partículas y llena los tensores correspondientes
void DistanciasEntreParticulas(int particula1, 
                               int particula2, 
                               double*** tensorPosiciones, 
                               double** tensorDistancias, 
                               double** vectorPosiciones, 
                               int dim,
                               double L,
                               double reff);

// Algoritmo de verlet para posiciones, velocidades y aceleraciones
void Verlet(double L, 
            double deltaT, 
            int dim, 
            int N, 
            int particula, 
            double** vectorPosicion, 
            double** vectorPosicionAnterior, 
            double** vectorVelocidad, 
            double** vectorVelocidadAnterior, 
            double** vectorFuerzas, 
            double** vectorFuerzasAnterior , 
            double*** tensorFuerzas, 
            double** vectorMasas,
            double* phiLangevin,
            double gammaLangevin,
            int** tensorVecinos);

// Algoritmo de verlet para minimizar energía
void VerletMinizacionEnergia(double L, 
                             double deltaT, 
                             int dim, 
                             int N, 
                             int particula, 
                             double** vectorPosicion, 
                             double** vectorPosicionAnterior, 
                             double** vectorFuerzas, 
                             double** vectorFuerzasAnterior , 
                             double*** tensorFuerzas, 
                             double** vectorMasas,
                             int** tensorVecinos);

// Igualar matrices
void IgualarMatriz(double** matriz1, double** matriz2, int N, int dim);

// Impresión por pantalla de matrices para debbug y guardado
void ImpresionMatrices3D(int N, int dim, double*** matriz, char* nombreMatriz, char* texto);
void ImpresionMatrices2D(int N, int dim, double** matriz, char* nombreMatriz, char* texto);
void salidaOvito(int N, int dim, double t, double** vector, char* nombreArchivo);

// Condicion de contorno periódica
double condicionContornoPeriodica(double posicion, double L);

// Inicializamos las velocidades con una temperatura dada
void inicializarVelocidades(int N, int dim, double T, double** vector);

char* NombreArchivo(char *texto, double T, int iteracion);

// Calculo del coeficiente del termostato de Langevin
double* coeficienteLangevin(int N, double gamma, double T, double deltaT, double** m);
    
double VirialSinCinetica(int dim, int particula1, int particula2, double*** tensorFuerzas, double** tensorDistancias);

//------------------ OPTIMIZACIÓN -------------------------------
//typedef struct {
//    int nCells;          // ¿En cuántas partes divido cada lado? (ej: 10 celdas por lado)
//    int totalCells;      // Total de celdas (10×10×10 = 1000 celdas en 3D)
//    double cellSize;     // Tamaño de cada celda (L/nCells)
//    int* cellHead;       // "Puerta" de entrada a cada celda
//    int* cellList;       // "Cadena" que conecta partículas
//    int** neighborCells; // Lista de celdas vecinas de cada celda
//    int* numNeighbors;   // Cuántas vecinas tiene cada celda
//} CellList;
//
//// Funciones del cell-list
//CellList* CrearCellList(int N, double L, double reff, int dim);
//
//// Función que libera memoria
//void LiberarCellList(CellList* cellList);
//
//// Función que actuliza la lista de celdas
//void ActualizarCellList(CellList* cellList, double** R, int N, int dim, double L);
//
//// Función que obtiene en que celda está una partícula
//int ObtenerIndiceCelda(double* posicion, CellList* cellList, int dim, double L);
//
//// Construye la lista de vecinos de cada 
//void ConstruirListaVecinos(CellList* cellList, int dim);

#endif