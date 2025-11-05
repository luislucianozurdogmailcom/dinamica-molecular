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

// Función encargada de escribir a archivo números de un array flexible
void EscrituraDataArrays(char *texto ,double *array, int tamano);

// Inicializar matriz de vectores
double** CrearMatriz(int N, int dim, double L, double valorPorDefecto);

// Crear e inicializar tensor de NxN
double** TensorNN(int N);

// Crear e inicializar Tensor de NxNxDim
double*** TensorNND(int N, int dim);

// Potencial de Lennard-Jones
double PotencialLJ(double epsilon, double sigma, double distancia, double rcut);

// Derivada en r del Potencial de Lennard-Jones
double DerivadaPotencialLJ(double epsilon, double sigma, double distancia);

// Calcula las fuerzas entre partículas y llena los tensores correspondientes
void FuerzasEntreParticulas(int particula1,
                            int particula2, 
                            double*** tensorFuerzas, 
                            double*** tensorPosiciones, 
                            double** tensorDistancias, 
                            double epsilon, 
                            double sigma, 
                            int dim, 
                            double reff);

// Calcula las distancias entre partículas y llena los tensores correspondientes
void DistanciasEntreParticulas(int particula1, 
                               int particula2, 
                               double*** tensorPosiciones, 
                               double** tensorDistancias, 
                               double** vectorPosiciones, 
                               int dim);

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
            double** vectorMasas);

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
                             double** vectorMasas);

// Igualar matrices
void IgualarMatriz(double** matriz1, double** matriz2, int N, int dim);

// Impresión por pantalla de matrices para debbug y guardado
void ImpresionMatrices3D(int N, int dim, double*** matriz, char* nombreMatriz, char* texto);
void ImpresionMatrices2D(int N, int dim, double** matriz, char* nombreMatriz, char* texto);
void salidaOvito(int N, int dim, double t, double** vector, char* nombreArchivo);

// Condicion de contorno periódica
double condicionContornoPeriodica(double posicion, double L);

char* NombreArchivo(char *texto, double T, int iteracion);

#endif