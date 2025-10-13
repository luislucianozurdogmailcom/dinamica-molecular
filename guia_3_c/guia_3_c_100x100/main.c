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

// Función encargada de escribir números (agregando no sobre escribiendo)
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

// Función encargada de escribir matrices
void EscrituraMatriz(char *texto, int** matriz, int tamano){
    FILE *archivo;

    // Abrimos el archivo en modo escritura ("w") para sobrescribir el contenido
    archivo = fopen(texto, "w");

    // Chequeo de errores
    if (archivo == NULL) {
        printf("Error al abrir el archivo %s \n", texto);
        return;
    }

    // Escribimos la matriz en el archivo
    for (int i = 0; i < tamano; i++) {
        for (int j = 0; j < tamano; j++) {
            fprintf(archivo, "%d ", matriz[i][j]);
        }
        fprintf(archivo, "\n"); // Nueva línea al final de cada fila
    }
    fprintf(archivo, "\n\n\n");

    fclose(archivo);
}

// Función encargada de leer matrices para DEBUGGING
int** LecturaMatriz(int** dominio, int tamano){
    FILE *archivoMatriz;

    // Abrimos el archivo en modo lectura ("r") para leer el contenido
    archivoMatriz = fopen("input/ultimoDominio", "r");

    // Chequeo de errores
    if (archivoMatriz == NULL) {
        printf("Error al abrir el archivo %s \n", "input/ultimoDominio");
        return dominio;
    }

    // Reservamos memoria para la matriz
    int** matriz = (int**)malloc(tamano * sizeof(int*)); 
    for(int i = 0; i < tamano; i++) {
        matriz[i] = (int*)malloc(tamano * sizeof(int));
    }

    // Leemos la matriz del archivo
    for (int i = 0; i < tamano; i++) {
        for (int j = 0; j < tamano; j++) {
            if (fscanf(archivoMatriz, "%d", &matriz[i][j]) != 1) {
                
                printf("Error al leer el elemento [%d][%d] del archivo %s\n", i, j, "input/ultimoDominio");
                
                // Liberamos memoria en caso de error
                for(int k = 0; k <= i; k++) {
                    free(matriz[k]);
                }
                
                free(matriz);
                fclose(archivoMatriz);
                
                return dominio;
            }
        }
    }

    fclose(archivoMatriz);
    return matriz;
}

// Inicializar el dominio
int** CrearDominio(int N){
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
            if (uni() < 0.5){
                matriz[i][j] = -1;
            }
            else{
                matriz[i][j] = 1;
            }
        }    
    }

    return matriz;
}

// Medir magnetización
int Magnetizacion(int** matriz, int tamano){
    
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
double MagnetizacionNormalizada(int** matriz, int tamano){
    
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
int BordeInfinito(int indice, int tamano){
    
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
int Energia(int** matriz, int tamano, double J){
    
    // Función encargada de medir la energia total del
    // sistema de particulas 2D del modelo de izing

    int iPlus1, iMinus1, jPlus1, jMinus1;
    int valor = 0; 
    
    // 
    for ( int i = 0; i < tamano; i++){
        for( int j = 0; j < tamano; j++){
            
            iPlus1  = BordeInfinito(i+1, tamano);
            iMinus1 = BordeInfinito(i-1, tamano);
            jPlus1  = BordeInfinito(j+1, tamano);
            jMinus1 = BordeInfinito(j-1, tamano);

            valor = valor + matriz[i][j] * (
                  + matriz[iPlus1][j] 
                  + matriz[iMinus1][j] 
                  + matriz[i][jPlus1] 
                  + matriz[i][jMinus1]);

        }
    }

    valor = valor * (int)(-J) / 2;

    return valor;
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

    // Leemos el tamaño de la grilla y otros datos
    int N                = LecturaInputInt("input/N");
    double T             = LecturaInputDouble("input/T");
    double J             = LecturaInputInt("input/J");
    double K             = LecturaInputInt("input/K");
    int muestreo         = LecturaInputInt("input/muestreo");
    int iteraciones      = LecturaInputInt("input/iteraciones");
    int simulacionesPorT = LecturaInputInt("input/simulaciones_por_cada_T");

    // Creamos el dominio y lo llenamos con datos random
    int** dominio = CrearDominio(N);

    // Intentamos leer la matriz del T anterior
    dominio = LecturaMatriz(dominio, N);

    // Variables donde almacenaremos datos
    int magnetizacion                  = Magnetizacion(dominio, N);
    double magnetizacionNormalizada    = MagnetizacionNormalizada(dominio, N);
    int energia                        = Energia(dominio, N, J);
    int deltaEnergia                   = 0; // Salto energético
    double energiaSumada               = 0; // Acumulación de energia
    double energiaCuadradoSumada       = 0; // Acumulación de energia al cuadrado
    double magnetizacionSumada         = 0; // Acumulación de magnetizacion
    double magnetizacionCuadradoSumada = 0; // Acumulación de magnetizacion al cuadrado
    double calorEspecífico             = 0; // beta^2*(1/N)*[ <E>^2 - <E^2>]
    double suceptibilidad              = 0; // beta^2*(1/N)*[ <M>^2 - <M^2>]
    double fraccionPasosAceptados      = 0; // pasosAceptados/pasosTotales
    
    // Valores al azar de la posición del Spin que cambiará
    int fila;
    int colu;

    // Valores de cálculo durante el loop
    double beta = 0;
    double probabilidad;
    double muestras;
    int pasosAceptados;
    
    // Valores para estadistica acumulada por lote de simulaciones para 
    // un mismo T
    double energiasPromedioSumadas         = 0;
    double magnetizacionesPromedioSumadas  = 0;
    double varianzasEnergiaSumadas         = 0;
    double varianzasMagnetizacionesSumadas = 0;
    double fraccionPasosAceptadosSumados   = 0;

    // Bucle de iteraciones repetidas
    for (int j=0; j < simulacionesPorT; j++){
        
        // Inicializamos variables para cada iteración
        pasosAceptados = 0;

        // Reseteamos los valores acumulados
        energiaSumada               = 0;
        energiaCuadradoSumada       = 0;
        magnetizacionSumada         = 0;
        magnetizacionCuadradoSumada = 0;
        
        printf("%f\n",T);

        // Bucle principal
        for (int i=0; i < iteraciones; i++){
            
            // Calculos de muestreo para registrar datos
            if( i % muestreo == 0 && j != 0){

                EscrituraData(NombreArchivo("energia.dat",T,j), energia);
                EscrituraData(NombreArchivo("magnetizacionNormalizada.dat",T,j), magnetizacionNormalizada);

                // Calculamos promedios para magnitudes estadísticas
                energiaSumada               += energia; // Para calcular <E>^2
                energiaCuadradoSumada       += pow(energia,2); // Para calcular <E^2>
                magnetizacionSumada         += magnetizacionNormalizada; // Para calcular <M>^2
                magnetizacionCuadradoSumada += pow(magnetizacionNormalizada,2); // Para calcular <M^2>
            
            }
            else if( i % muestreo == 0 && j == 0){ // Registro de datos de termalización

                EscrituraData(NombreArchivo("termalizacion_energia",T,j), energia);
                EscrituraData(NombreArchivo("termalización_magnetizacionNormalizada.dat",T,j), magnetizacionNormalizada);

                // Calculamos promedios para magnitudes estadísticas
                energiaSumada               += energia; // Para calcular <E>^2
                energiaCuadradoSumada       += pow(energia,2); // Para calcular <E^2>
                magnetizacionSumada         += magnetizacionNormalizada; // Para calcular <M>^2
                magnetizacionCuadradoSumada += pow(magnetizacionNormalizada,2); // Para calcular <M^2>
                
            }
            
            // Calculamos al azar el valor del spin a modificar
            fila = (int)floor(uni()*N);
            colu = (int)floor(uni()*N);
            
            // Calculamos el salto de energia al flipear el spin con J positivo
            deltaEnergia = (int)J * dominio[fila][colu] * (
                                dominio[BordeInfinito(fila+1, N)][colu] +
                                dominio[BordeInfinito(fila-1, N)][colu] +
                                dominio[fila][BordeInfinito(colu+1, N)] +
                                dominio[fila][BordeInfinito(colu-1, N)]
                            ) * 2;
                            
            if (deltaEnergia < 0){   
                // Aceptamos el cambio de estado
                dominio[fila][colu]      *= -1; // Flipeamos el spin
                energia                  += deltaEnergia; // Reasignamos la energia
                magnetizacion            += dominio[fila][colu] * 2; // Reasignamos la magnetización
                magnetizacionNormalizada = (double)magnetizacion / (N*N); // Calculamos la magnetización media
                pasosAceptados           += 1;
            }
            else if (deltaEnergia > 0){ // Calculamos la probabilidad de aceptación
                
                beta         = 1/(K*T);
                probabilidad = exp(-beta*deltaEnergia);
                
                if (uni() < probabilidad){
                    
                    dominio[fila][colu]     *= -1; // Aceptamos el cambio de estado
                    energia                 += deltaEnergia; // Reasignamos las energias
                    magnetizacion           += dominio[fila][colu] * 2; // Reasignamos la magnetización
                    magnetizacionNormalizada = (double)magnetizacion / (N*N); // Calculamos la magnetización media
                    pasosAceptados          += 1;
                }
            }
            else { // Caso si la energia en el cambio es igual a la energia anterior
                
                // Aceptamos el cambio de estado
                dominio[fila][colu]     *= -1;
                magnetizacion           += dominio[fila][colu] * 2; // Reasignamos la magnetización
                magnetizacionNormalizada = (double)magnetizacion / (N*N); // Calculamos la magnetización media
                pasosAceptados          += 1;
            }
        
            if (i+1==iteraciones && j != 0){

                fraccionPasosAceptados         = ((double) pasosAceptados)/((double) iteraciones);
                muestras                       = (double)(i/muestreo)+1;
                
                // Calculamos datos estadisticos
                energiasPromedioSumadas         += energiaSumada/muestras;
                magnetizacionesPromedioSumadas  += magnetizacionSumada/muestras;
                varianzasEnergiaSumadas         += ( (energiaCuadradoSumada/muestras) - pow((energiaSumada/muestras),2) );
                varianzasMagnetizacionesSumadas += ( (magnetizacionCuadradoSumada/muestras) - pow((magnetizacionSumada/muestras),2) );                
                fraccionPasosAceptadosSumados   += fraccionPasosAceptados; 
            }
        }
    
    }

    // Escribimos los datos promedios de:
    // Energia promedio General y su varianza
    // Magnetización promedio general y su varianza
    energiasPromedioSumadas         = energiasPromedioSumadas         / ((double)simulacionesPorT - 1);
    magnetizacionesPromedioSumadas  = magnetizacionesPromedioSumadas  / ((double)simulacionesPorT - 1);
    varianzasEnergiaSumadas         = varianzasEnergiaSumadas         / ((double)simulacionesPorT - 1);
    varianzasMagnetizacionesSumadas = varianzasMagnetizacionesSumadas / ((double)simulacionesPorT - 1);
    fraccionPasosAceptadosSumados   = fraccionPasosAceptadosSumados   / ((double)simulacionesPorT - 1);

    // Calculamos el calor específico y la suceptibilidad
    calorEspecífico = pow(beta,2)
    * (1 / (double)(N*N)) 
    * varianzasEnergiaSumadas;
    
    suceptibilidad  = beta * varianzasMagnetizacionesSumadas;
    
    EscrituraDataArrays("output/T_E_M_PAcept_Cv_sus.dat", 
                       (double[]){T, energiasPromedioSumadas, magnetizacionesPromedioSumadas, fraccionPasosAceptadosSumados, calorEspecífico, suceptibilidad},
                       6);
    
    
    // Escribimos la última matriz simulada
    EscrituraMatriz("input/ultimoDominio", dominio, N);

    finalize_random();
    return 0;
}