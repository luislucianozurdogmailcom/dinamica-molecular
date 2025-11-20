#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>
#include "ziggurat/ziggurat.h"
#include "utils/fisica.h" // Biblioteca autocreada con funciones

// Biblios para crear carpetas
#include <sys/stat.h>
#include <sys/types.h>

// Rutina principal
int main() {

    // Inicializamos ziggurat
    initialize_random();

    // Leemos los archivos de entrada
    int N                     = LecturaInputInt("input/N");                      // Cantidad de partículas 
    int dim                   = LecturaInputInt("input/dim");                    // Dimensiones espaciales
    int listaVecinos          = LecturaInputInt("input/listaVecinos");           // Pasos temporales para actualizar una lista de vecinos
    int pasosT                = LecturaInputInt("input/pasosT");                 // Cantidad de pasos temporales
    double L                  = LecturaInputDouble("input/L");                   // Lado de caja cúbica
    double reff               = LecturaInputDouble("input/Reff");                // Radio efectivo de partícula
    double epsilon            = LecturaInputDouble("input/epsilon");             // Epsilon
    double sigma              = LecturaInputDouble("input/sigma");               // Sigma
    double deltaT             = LecturaInputDouble("input/deltaT");              // Paso temporal
    double deltaTMinimizacion = LecturaInputDouble("input/deltaTMinimizacion");  // Delta T para la minimizacion (generalmente mas grande)
    int tGuardado             = LecturaInputInt("input/tGuardado");              // Cada cuantos pasos guardamos
    int tMinimizacion         = LecturaInputInt("input/tMinimizacion");          // Pasos temporales hasta cortar minimizacion
    int muestreo              = LecturaInputInt("input/muestreo");               // Muestreo de datos en la simulación post minimizacion
    int muestreoMinimizacion  = LecturaInputInt("input/muestreoMinimizacion");   // Muestreo de datos en la minimizacion
    double gammaLangevin      = LecturaInputDouble("input/gamma");               // Coeficiente gamma de fricción para el termostato
    double kb                 = LecturaInputDouble("input/kb");                  // Constante de Boltzman
    double temp               = LecturaInputDouble("input/Temp") * epsilon / kb; // Temperatura inicial
    int procesadores          = LecturaInputInt("input/procesadores");           // Procesadores en paralelo
    int divisionesFuerzas     = LecturaInputInt("input/divisionesFuerzas");      // Cantidad de datos en la tabla precalculada de fuerzas

    // Inicializamos los vectores r, v y f
    double** R         = CrearMatriz(N, dim, L, 0); // Vector de posiciones actuales creadas al azar
    double** Ranterior = CrearMatriz(N, dim, 0, 0); // Vector de posiciones paso temporal anterior
    double** V         = CrearMatriz(N, dim, 0, 0); // Vector de velocidades
    double** Vanterior = CrearMatriz(N, dim, 0, 0); // Vector de velocidades paso temporal anterior
    double** F         = CrearMatriz(N, dim, 0, 0); // Vector de Fuerzas
    double** Fanterior = CrearMatriz(N, dim, 0, 0); // Vector de Fuerzas paso temporal anterior
    double** M         = CrearMatriz(N, 1, 1, 1);   // Vector de masas como 1
    
    // Seteamos los procesos en paralelo
    omp_set_num_threads(procesadores);  // Ajusta según tu CPU
    printf("Usando %d threads\n", omp_get_max_threads());

    // Igualamos la matrices anteriores a la actual
    IgualarMatriz(Ranterior, R, N, dim);
    IgualarMatriz(Vanterior, V, N, dim);
    IgualarMatriz(Fanterior, F, N, dim);

    // Inicializamos velocidades
    inicializarVelocidades(N, dim, temp, V);

    // Inicializamos los tensores
    double**  Td = TensorNN(N);       // Tensor que almacena distancias calculadas
    double*** Tr = TensorNND(N, dim); // Tensor que almacena posiciones entre particulas
    double*** Tf = TensorNND(N, dim); // Tensor que almacena fuerzas entre partículas
    
    // Lista de vecinos inicializadas todos en 1
    int**     Tv = TensorNNint(N);    // Tensor que almacena una lista de vecinos 

    // Inicializamos los coeficientes de las fuerzas del termostato de langevin
    double* phiLangevin = coeficienteLangevin(N, gammaLangevin, temp, deltaT, M);

    // Variables necesarias durante las iteraciones
    double energiaPotencial        = 0;
    double energiaCinetica         = 0;
    double virialSinCinetica       = 0;
    double presionTotal            = 0;
    double tempInstantanea         = 0;
    double presionMedia            = 0;
    double presionCuadraticaMedia  = 0;
    double virialPromedio          = 0;
    
    // Valores precalculados constantes a lo largo de la simulación
    
    // Potencial del cutoff para evitar discontinuidades
    double potencialCut = - 4 * epsilon * ( - ( pow( sigma / reff, 6) ) + ( pow( sigma / reff, 12) ) );

    // Epsilon y Sigmas elevados a 6 y 12
    double sigmaSeis  = pow(sigma, 6);
    double sigmaDoce  = pow(sigma, 12);

    // Calculamos una lisa de fuerzas ya predefinidas
    double* listaFuerzas = listaFuerzasEntreParticulas(reff, divisionesFuerzas, sigmaSeis, sigmaDoce, epsilon);

    // Abrimos archivos de escritura:
    FILE* energiaArchivo      = AperturaArchivo("output/energia.dat");
    FILE* presionArchivo      = AperturaArchivo("output/presion.dat");
    FILE* temperaturaArchivo  = AperturaArchivo("output/temperatura.dat");
    FILE* presionMediaArchivo = AperturaArchivo("output/fluctP.dat");

    // Medimos tiempo al inicio
    double start_time = omp_get_wtime();

    // Inicializamos el loop
    for (int iter = 0; iter < pasosT; iter++){

        // Reseteamos las energias totales por paso temporal
        energiaPotencial  = 0;
        energiaCinetica   = 0;
        virialSinCinetica = 0;
        presionTotal      = 0;
        tempInstantanea   = 0;

        int particula1, particula2;

        // Iteramos por cada par de partículas
        // Paralelizamos
        #pragma omp parallel for private(particula2) reduction(+:energiaPotencial,virialSinCinetica) schedule(dynamic) 
        for (particula1 = 0; particula1 < N; particula1++){

            // Comparacion entre particula 1 y las demas
            for (particula2 = 0; particula2 < particula1; particula2 ++){
                
                // Si no son vecinos, salteamos. Cada listaVecinos calculamos nuevamente todas las distancias
                if (Tv[particula1][particula2] == 0 && iter % listaVecinos != 0){
                    continue;
                }
                
                DistanciasEntreParticulas(particula1, particula2, Tr, Td, R, dim, L, reff);
                
                // Cálculo del potencial de lennard-Jones ----------------------------------------
                // Si la distancia es mayor a la efectiva, no computamos la energía de interacción ni la fuerza
                if (reff > Td[particula1][particula2])
                {
                    // Potencial de Lennard-Jones
                    energiaPotencial += PotencialLJ(epsilon, sigma, Td[particula1][particula2], potencialCut, sigmaSeis, sigmaDoce);

                }

                // Si estamos a una distancia de 1.5 el radio de cuttoff, cuenta como vecina
                if (iter % listaVecinos == 0)
                {
                    if (reff * 1.5 > Td[particula1][particula2])
                    {
                        Tv[particula1][particula2] = 1;
                        Tv[particula2][particula1] = 1;
                    }
                    else
                    {
                        Tv[particula1][particula2] = 0;
                        Tv[particula2][particula1] = 0;
                    }
                }


                // Calculamos las fuerzas entre partículas y se guarda en Tf
                FuerzasEntreParticulas(particula1, particula2, Tf, Tr, Td, epsilon, sigma, dim, reff, sigmaSeis, sigmaDoce, listaFuerzas, divisionesFuerzas);
                
                // Parte del virial sin la energia cinética:
                virialSinCinetica += VirialSinCinetica(dim, particula1, particula2, Tf, Tr);
            }

        }
        
        // Minimización hasta cierto T
        if (iter < tMinimizacion){

            // Con tensores calculados ahora hay que hacer calculos de posiciones
            // Paralelizamos
            #pragma omp parallel for 
            for (int particula = 0; particula < N; particula++){
                VerletMinizacionEnergia(L ,deltaTMinimizacion, dim, N, particula, R, Ranterior, F, Fanterior, Tf, M, Tv);
            } 
        }
        else
        {
            // Con tensores calculados ahora hay que hacer calculos de posiciones
            // Paralelizamos
            int particula, dimension;
            #pragma omp parallel for private(dimension) reduction(+:energiaCinetica) 
            for (particula = 0; particula < N; particula++){
                Verlet(L ,deltaT, dim, N, particula, R, Ranterior, V, Vanterior, F, Fanterior, Tf, M, phiLangevin, gammaLangevin, Tv);
                
                // Calculamos la energía cinética total
                for (dimension = 0; dimension < dim; dimension++){
                    energiaCinetica += 0.5 * V[dimension][particula] * V[dimension][particula] / M[0][particula] ;
                }
            }

            // Calculamos la presión del sistema:
            tempInstantanea = 2 * energiaCinetica / ( kb * (3*N - 3) );
            presionTotal    = ( ( tempInstantanea * N * kb ) / ( L*L*L ) ) + ( 1 / (3 * L*L*L ) ) * virialSinCinetica ;

        }

        // Guardado de datos en la simulación normal
        if( iter > tMinimizacion && iter % muestreo == 0)
        {   
            // Guardamos las energias totales
            fprintf(energiaArchivo, "%f\n", energiaPotencial + energiaCinetica);
            
            // Guardamos la presión total
            fprintf(presionArchivo, "%f\n", presionTotal);

            // Guardamos la temperatura instantanea
            fprintf(temperaturaArchivo, "%f\n", tempInstantanea);

            virialPromedio += virialSinCinetica;

            // Calculo de valores estadisticos:
            presionMedia           += presionTotal;
            presionCuadraticaMedia += presionTotal * presionTotal;
        }
        else if( iter < tMinimizacion && iter % muestreoMinimizacion == 0 ){
            // Guardamos solo la energía potencial durante la minimización
            fprintf(energiaArchivo, "%f\n", energiaPotencial);
        }

        // Guardado de pasos temporales
        if (iter % tGuardado == 0){

            salidaOvito(N, dim, (double)iter*deltaT, R, "output/posicionT.xyz");
        }
        
        // Imprimimos el progreso cada 1%
        if(pasosT > 0 && iter % (pasosT/100 + 1) == 0){
            printf("Progreso: %d%%\n", (int)((iter * 100) / pasosT));
        }
    }

    // Guardamos la presion media final
    fprintf(presionMediaArchivo, "%f %f\n", presionCuadraticaMedia / (pasosT - tMinimizacion) * muestreo, presionMedia / (pasosT - tMinimizacion) * muestreo);
    printf("Virial: %f\n", virialPromedio/((pasosT-tMinimizacion)/muestreo));
    
    // Cerramos los archivos
    CierreArchivo(energiaArchivo);
    CierreArchivo(presionArchivo);
    CierreArchivo(temperaturaArchivo);
    CierreArchivo(presionMediaArchivo);

    // Cerramos el tiempo en cuanto tardó
    double end_time = omp_get_wtime();
    printf("Tiempo total: %.2f segundos\n", end_time - start_time);
    
    finalize_random();
    return 0;
    }