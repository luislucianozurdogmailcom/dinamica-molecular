#include "fisica.h"

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
double PotencialLJ(double epsilon, double sigma, double distancia, double rcut){
    // Epsilon es la medida del pozo de potencial y da cuenta de la 
    // intensidad de la interacción.
    // Sigma es una medida del diámetro efectivo de las particulas
    
    // Potencial del cutoff para evitar discontinuidades
    double potencialCut = - 4 * epsilon * ( - ( pow( sigma / rcut, 6) ) + ( pow( sigma / rcut, 12) ) );

    return 4 * epsilon * ( - ( pow( sigma / distancia, 6) ) + ( pow( sigma / distancia, 12) ) ) + potencialCut;
}

// Derivada en r del Potencial de Lennard-Jones
double DerivadaPotencialLJ(double epsilon, double sigma, double distancia){
    // Epsilon es la medida del pozo de potencial y da cuenta de la 
    // intensidad de la interacción.
    // Sigma es una medida del diámetro efectivo de las particulas
    

    
    return 4 * epsilon * ( 6 * ( pow( sigma, 6 ) / pow( distancia, 7 ) ) - 12 * ( pow( sigma, 12) / pow(distancia, 13) ) );
}

// Calcula las fuerzas entre partículas y llena los tensores correspondientes
void FuerzasEntreParticulas(int particula1, 
                            int particula2, 
                            double*** tensorFuerzas, 
                            double*** tensorPosiciones, 
                            double** tensorDistancias, 
                            double epsilon, 
                            double sigma, 
                            int dim, 
                            double reff){
    
    if(reff > tensorDistancias[particula1][particula2]) // Dentro del radio cutoff
    { 
        // Fuerzas que siente la partícula en cada eje por par de partículas
        for(int k = 0; k < dim; k++){
            // Almacenamos la fuerza en el tensor
            
            // Encima de la diagonal de la matriz con el signo positivo
            tensorFuerzas[particula1][particula2][k] = - ( tensorPosiciones[particula1][particula2][k]  / tensorDistancias[particula1][particula2] ) * DerivadaPotencialLJ(epsilon, sigma, tensorDistancias[particula1][particula2]); 
            
            // Debajo de la diagonal de la matriz signo cambiado debido a que es la reacción de la otra partícula
            tensorFuerzas[particula2][particula1][k] = - tensorFuerzas[particula1][particula2][k];
        }
    }
    else // Fuera del radio cutoff
    {
        // Fuerzas que siente la partícula en cada eje por par de partículas
        for(int k = 0; k < dim; k++){
            // Almacenamos la fuerza en el tensor
            tensorFuerzas[particula1][particula2][k] = 0;
            tensorFuerzas[particula2][particula1][k] = 0;
        }
    }
}

// Calcula las distancias entre partículas y llena los tensores correspondientes
void DistanciasEntreParticulas(int particula1, 
                               int particula2, 
                               double*** tensorPosiciones, 
                               double** tensorDistancias, 
                               double** vectorPosiciones, 
                               int dim){
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
            double** vectorMasas){
    // Calculamos las posiciones con el algoritmo de verlet
    // En este mismo algoritmo se realiza la sumatoria total de fuerzas
    // sobre una misma partícula para posteriormente realizar verlett
    
    // Variable auxiliar para fuerzas
    double fuerzaTotal = 0; 

    for (int k = 0; k < dim; k++){
        
        // Inicializamos con cero
        fuerzaTotal = 0;

        for (int columna = 0; columna < N; columna++){
            
            // Acumulamos fuerzas para una dimensión
            fuerzaTotal += tensorFuerzas[particula][columna][k];
        }

        // Guardamos la posicion, la velocidad y la fuerza anterior
        vectorFuerzasAnterior[k][particula]   = vectorFuerzas[k][particula];
        vectorVelocidadAnterior[k][particula] = vectorVelocidad[k][particula]; 
        vectorPosicionAnterior[k][particula]  = vectorPosicion[k][particula];
        
        // ----------- ACTUALIZAMOS FUERZA ---------------//
        // Guardamos las fuerzas nuevas totales en el vectorFuerzas f(t)
        vectorFuerzas[k][particula] = fuerzaTotal;

        // ----------- ACTUALIZAMOS VELOCIDAD ------------//
        // Calculamos la velocidad como V(t) = v(t-dT) + 0.5 [a(t-dT)+a(t)] * deltaT
        vectorVelocidad[k][particula] = vectorVelocidadAnterior[k][particula] + \
                                        0.5 * ( (vectorFuerzasAnterior[k][particula] / vectorMasas[0][particula]) \
                                        + (vectorFuerzas[k][particula]/vectorMasas[0][particula]) ) * deltaT; 

        // ----------- ACTUALIZAMOS POSICIÓN -------------//
        // Calculamos la nueva posición como r(t+dT) = r(t) + v(t)*deltaT + 0.5*a(t)*deltaT^2
        vectorPosicion[k][particula] =  vectorPosicion[k][particula] \
                                      + vectorVelocidad[k][particula] * deltaT \
                                      + ( ( vectorFuerzas[k][particula] * pow( deltaT, 2 ) ) / ( vectorMasas[0][particula] * 2 ) );

        // Condición de contorno periódica
        vectorPosicion[k][particula] = condicionContornoPeriodica(vectorPosicion[k][particula], L);

    }
}

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
                             double** vectorMasas){
    // Calculamos las posiciones con el algoritmo de verlet
    // En este mismo algoritmo se realiza la sumatoria total de fuerzas
    // sobre una misma partícula para posteriormente realizar verlett
    
    // Variable auxiliar para fuerzas y posiciones
    double fuerzaTotal   = 0; 

    for (int k = 0; k < dim; k++){
        
        // Inicializamos con cero
        fuerzaTotal   = 0;

        for (int columna = 0; columna < N; columna++){
            
            // Acumulamos fuerzas para una dimensión
            fuerzaTotal += tensorFuerzas[particula][columna][k];
        }

        // Guardamos la posicion, la velocidad y la fuerza anterior
        vectorFuerzasAnterior[k][particula]   = vectorFuerzas[k][particula];
        vectorPosicionAnterior[k][particula]  = vectorPosicion[k][particula];
        
        // ----------- ACTUALIZAMOS FUERZA ---------------//
        // Guardamos las fuerzas nuevas a f(t)
        vectorFuerzas[k][particula] = fuerzaTotal;

        // ----------- ACTUALIZAMOS POSICIÓN -------------//
        // Calculamos la nueva posición como r(t+dT) = r(t) + 0.5 * ( f(t) / m ) * deltaT^2
        vectorPosicion[k][particula] = vectorPosicion[k][particula] \
                                     + ( ( vectorFuerzas[k][particula] * pow( deltaT, 2 ) ) / ( vectorMasas[0][particula] * 2 ) );

        // Condición de contorno periódica
        vectorPosicion[k][particula] = condicionContornoPeriodica(vectorPosicion[k][particula], L);
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
void ImpresionMatrices3D(int N, int dim, double*** matriz, char* nombreMatriz, char* texto){
    
    if(strcmp(texto, "") != 0)
    {
        FILE *archivo;

        // "rb" es para leer archivos binarios y "r" normales
        archivo = fopen(texto,"a"); 

        // Chequeo de errores
        if (archivo == NULL){
            printf("Error al abrir el archivo %s \n", texto);
            return;
        }
        
        for (int i = 0; i < N; i++){
            for (int j = 0; j < N; j++){

                fprintf(archivo,"[");
                for (int k = 0; k < dim; k++){
                    fprintf( archivo, "%.2f ", matriz[i][j][k]);
                }
                fprintf(archivo, "]");
                fprintf(archivo, "\t");
            }
            fprintf(archivo, "\n");
        }

        fprintf(archivo, "\n");

        fclose(archivo);
    }
    else // Imprimimos por pantalla pero no guardamos
    {
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
}
void ImpresionMatrices2D(int N, int dim, double** matriz, char* nombreMatriz, char* texto){
    
    if(strcmp(texto, "") != 0)
    {
        FILE *archivo;

        // "rb" es para leer archivos binarios y "r" normales
        archivo = fopen(texto,"a"); 

        // Chequeo de errores
        if (archivo == NULL){
            printf("Error al abrir el archivo %s \n", texto);
            return;
        }
        
        for (int i = 0; i < dim; i++){
            for (int j = 0; j < N; j++){
                fprintf(archivo, "%.6f ", matriz[i][j]);
                fprintf(archivo, "\t");
            }
            fprintf(archivo, "\n");
        }

        fprintf(archivo, "\n");

        fclose(archivo);
    }
    else // Imprimimos por pantalla pero no guardamos
    {
        
        printf("\n---------- Impresión de matriz 2D %s ------------\n", nombreMatriz);

        for (int i = 0; i < dim; i++){
            for (int j = 0; j < N; j++){
                printf( "%.6f ", matriz[i][j]);
                printf("\t");
            }
            printf("\n");
        }
    }

    
}
void salidaOvito(int N, int dim, double t, double** vector, char* nombreArchivo){
    // Función que guarda las partículas en el formato XYZ    

    FILE *archivo;
    
    // "rb" es para leer archivos binarios y "r" normales
    archivo = fopen(nombreArchivo,"a"); 
    
    // Chequeo de errores
    if (archivo == NULL){
        printf("Error al abrir el archivo %s \n", nombreArchivo);
        return;
    }
    
    fprintf(archivo, "%d\n", N);
    fprintf(archivo, "t = %f\n", t);

    for (int particula = 0; particula < N; particula++){
        
        fprintf(archivo, "A ");
        for (int dimension = 0; dimension < dim; dimension++){
            fprintf(archivo, "%.6f ", vector[dimension][particula]);
        }
        if (dim < 3){
            fprintf(archivo, "0");
        }
        fprintf(archivo, "\n");
    }
    fclose(archivo);
}

// Condición de contorno de no salirse de la caja
double condicionContornoPeriodica(double posicion, double L){
    
    double varAux = 0;

    if (posicion < 0){
        varAux = L + fmod(posicion, L);
        return varAux;
    }
    else if (posicion > L)
    {
        varAux = fmod(posicion, L);
        return varAux;
    }
    
    return posicion;
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
