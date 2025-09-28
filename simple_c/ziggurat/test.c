#include <stdio.h>
#include "ziggurat.h"

int main(void) {
    // Bins
    int bins_uni[10]    = {0};// Inicializamos en cero los valores
    int bins_normal[10] = {0};// Inicializamos en cero los valores

    int numero_entero;
    double numero_real;
    double numero_real_normal;
    double numero_real_exp;

    // Inicializamos variables de números aleatorios
    initialize_random();

    for (int i=0; i<100; i++){
        numero_real        = uni();
        numero_real_normal = rnor();
        numero_real_exp    = rexp();

        printf("%f ", numero_real_exp);
        
        // Puertas lógicas para distribución uniforme
        if (0 < numero_real && 0.1 > numero_real){
            bins_uni[0]++;
        }
        else if(0.1 < numero_real && 0.2 > numero_real){
            bins_uni[1]++;
        }
        else if(0.2 < numero_real && 0.3 > numero_real){
            bins_uni[2]++;
        }
        else if(0.3 < numero_real && 0.4 > numero_real){
            bins_uni[3]++;
        }
        else if(0.4 < numero_real && 0.5 > numero_real){
            bins_uni[4]++;
        }
        else if(0.5 < numero_real && 0.6 > numero_real){
            bins_uni[5]++;
        }
        else if(0.6 < numero_real && 0.7 > numero_real){
            bins_uni[6]++;
        }
        else if(0.7 < numero_real && 0.8 > numero_real){
            bins_uni[7]++;
        }
        else if(0.8 < numero_real && 0.9 > numero_real){
            bins_uni[8]++;
        }
        else if(0.9 < numero_real && 1 > numero_real){
            bins_uni[9]++;
        }

        // Puertas lógicas para distribución uniforme
        if (-5 < numero_real_normal && -4 > numero_real_normal){
            bins_normal[0]++;
        }
        else if(-4 < numero_real_normal && -3 > numero_real_normal){
            bins_normal[1]++;
        }
        else if(-3 < numero_real_normal && -2 > numero_real_normal){
            bins_normal[2]++;
        }
        else if(-2 < numero_real_normal && -1 > numero_real_normal){
            bins_normal[3]++;
        }
        else if(-1 < numero_real_normal && 0 > numero_real_normal){
            bins_normal[4]++;
        }
        else if(0 < numero_real_normal && 1 > numero_real_normal){
            bins_normal[5]++;
        }
        else if(1 < numero_real_normal && 2 > numero_real_normal){
            bins_normal[6]++;
        }
        else if(2 < numero_real_normal && 3 > numero_real_normal){
            bins_normal[7]++;
        }
        else if(3 < numero_real_normal && 4 > numero_real_normal){
            bins_normal[8]++;
        }
        else if(4 < numero_real_normal && 5 > numero_real_normal){
            bins_normal[9]++;
        }

    }

    // Numeros reales
    /*
    printf("Para numeros con distribución uniforme \n");
    for (int i=0; i<10; i++){
        printf("%d ",bins_uni[i]);
        }
        // Numeros reales normales
        printf("Para numeros con distribución uniforme \n");
        for (int i=0; i<10; i++){
            printf("%d ",bins_normal[i]);
        }
    */

    finalize_random();

    return 0;
}