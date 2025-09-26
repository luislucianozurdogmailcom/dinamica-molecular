#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "ziggurat.h"

// Definiciones de las variables (añadir al inicio del archivo)
double m1   = 2147483648.0;
double m2   = 2147483648.0;
double half = 0.5;
double dn   = 3.442619855899;
double tn   = 3.442619855899;
double vn   = 0.00991256303526217;
double q;
double de   = 7.697117470131487;
double te   = 7.697117470131487;
double ve   = 0.003949659822581572;
double wn[128], fn[128], we[256], fe[256];
int iz, jz;
int jsr = 123456789;
int kn[128], ke[256], hz;
int initialized = 0;

// Leemos la semilla
int seed_read(){
    FILE *archivo;
    char *texto = "ziggurat/seed.dat";

    // "rb" es para leer archivos binarios y "r" normales
    archivo = fopen(texto,"r"); 

    // Chequeo de errores
    if (archivo == NULL){
        
        // Intentamos por segunda vez
        texto = "seed.dat";
        archivo = fopen(texto,"r"); 
        
        if (archivo == NULL){

            printf("Error al abrir el archivo %s \n", texto);
            return 1;
        }
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

// Guardamos la semilla
void seed_write(){
    FILE *archivo;
    char *texto = "ziggurat/seed.dat";

    // "rb" es para leer archivos binarios y "r" normales
    archivo = fopen(texto,"w"); 

    // Obtener la fecha y hora actual como un time_t (normalmente es un int de 32 bits)
    time_t tiempo_actual = time(NULL);
    int32_t seed_value   = (int32_t)tiempo_actual; // Casteamos a int 32 bits

    // Chequeo de errores
    if (archivo == NULL){
        
        // Intentamos por segunda vez
        texto   = "seed.dat";
        archivo = fopen(texto,"w"); 
        
        if (archivo == NULL){

            printf("Error al abrir el archivo %s \n", texto);
            return;
        }
    }

    // Escribimos el archivo con el nuevo número aleatorio
    fprintf(archivo, "%d", seed_value); 

    fclose(archivo);
}

// Genera numeros random de 32-bit enteros
int shr3(){
    // printf("Semilla cargada en RAM: %d", jsr);
    int valor; // Numero aleatorio que devolveremos

    // Acá hacemos una operación en bits de corrimientos
    // ^ es equivalente a hacer XOR
    // << o >> es un corrimiento de bits
    jz    = jsr;
    jsr   = jsr ^ (jsr << 13 ); 
    jsr   = jsr ^ (jsr >> 17 );
    jsr   = jsr ^ (jsr << 5 );
    valor = jz + jsr;

    return valor;
}

// Genera numeros aleatorios entre 0 y 1
double uni(){

    double valor = half + 0.2328306e-9 * shr3();
    return valor;
}

// Genera números aleatorios normales centrados en 0 con Desviacion estandar de 1
double rnor(){
    
    // Valores varios
    double valor;         // A devolver
    double r = 3.442620;  // parametros 
    double x,y;           // parametros

    hz = shr3();   // Seteamos nuevo número pseudo aleatorio
    iz = hz & 127; // Operación and bit a bit entre hz y el número 127
    

    if (abs(hz) < kn[iz])
    {
        valor = hz * wn[iz];
        return valor;
    }
    else
    {
        while(1) // Primer bucle infinito
        {
            if(iz == 0)
            {
                while(1) // Segundo bucle infinito
                {
                    x = -0.2904764 * log(uni());
                    y = -log(uni());

                    if(y+y >= x*x) // Salimos del bucle si se cumple la condición
                    {
                        break;
                    }

                }

                valor = r+x;
                
                if(hz <= 0)
                {
                    valor = -valor;
                } 

                return valor;
            }

            x = hz * wn[iz];
            if(fn[iz] + uni() * (fn[iz-1] - fn[iz]) < exp(-half*x*x))
            {
                valor = x;
                return valor;
            }

            hz = shr3();
            iz = hz & 127; // Operación "and" bit a bit

            if(abs(hz) < kn[iz])
            {
                valor = hz * wn[iz];
                return valor;
            }
        } 
    }

    return valor;
}

// Genera numeros aleatorios exponenciales
double rexp(){
    
    double valor;  
    double x;

    jz      = shr3();
    iz      = jz & 255;
    
    if( abs(jz) < ke[iz] ){

        valor = (double)(abs(jz) * we[iz]);
        return valor;
    }

    for(;;){

        if( iz == 0 ){

            valor = 7.69711 - log(uni( ));
            return valor;
        }
        
        x = (double)(abs(jz) * we[iz]);
        
        if( fe[iz] + uni()*(fe[iz-1] - fe[iz]) < exp(-x)){

            valor = x;
            return valor;
        }

        jz      = shr3();
        iz      = jz & 255;

        if(abs(jz) < ke[iz]){

            valor = (double)(abs(jz) * we[iz]);
            return valor;
        }
    }
    return valor;
}

// Función de inicialización de semilla
void initialize_random(){
    jsr = seed_read();

    // Tablas para RNOR
    q       = vn*exp(half*dn*dn);
    kn[0]   = (dn/q)*m1;
    kn[1]   = 0;
    wn[0]   = q/m1;
    wn[127] = dn/m1;
    fn[0]   = 1.0;
    fn[127] = exp( -half*dn*dn );
    
    for (int i=126; i>=1; i--){
        dn      = sqrt(-2.0 * log((vn/dn) + exp(-half * dn * dn)));
        kn[i+1] = (dn/tn)*m1;
        tn      = dn;
        fn[i]   = exp(-half*dn*dn);
        wn[i]   = dn/m1;
    }

    // Tablas para REXP
    q       = ve*exp(de);
    ke[0]   = (de/q)*m2;
    ke[1]   = 0.0;
    we[0]   = q/m2;
    we[255] = de/m2;
    fe[0]   = 1.0;
    fe[255] = exp(-de);
    for (int i = 254; i>=1; i--){

        de      = -log( (ve/de) + exp(-de));
        ke[i+1] = m2 * (de/te);
        te      = de;
        fe[i]   = exp(-de);
        we[i]   = de/m2;
    }
};

// Función de finalización de uso de random
void finalize_random(){
    seed_write();
};

// Función de numeros aleatorios uniformes 0<x<1
/*
double uni(){
    double fn_val;
    
    fn_val = half + 0.2328306e-9 * shr3();
}
*/