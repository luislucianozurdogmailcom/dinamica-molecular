#include <stdio.h>
#include <stdlib.h>
#include "arrays_dinamicos.h"

// Inicializar array dinámico
void initArray(DynamicArray* a, size_t initialSize) {
    a->array = (double*)malloc(initialSize * sizeof(double));
    if (a->array == NULL) {
        fprintf(stderr, "Error de asignación de memoria\n");
        exit(1);
    }
    a->used = 0;
    a->size = initialSize;
}


// Agregar elemento al final de array
void insertArray(DynamicArray* a, double element){
    // Si el array está lleno, sumamos uno
    if (a->used == a->size){
        a->size += 1;
        a->array = (double*)realloc(a->array, a->size * sizeof(double));
        if (a->array == NULL){
            fprintf(stderr, "Error al redimensionar memoria\n");
            exit(1); // Mata el programa y devuelve un error.
        }
    }
    a->array[a->used++] = element;
}

// Reducir el tamaño del array para liberar memoria no utilizada
void trimToSize(DynamicArray* a){
    a->array = (double*)realloc(a->array, a->used * sizeof(double));
    a->size  = a->used;
    if (a->array == NULL && a->size > 0){
        fprintf(stderr, "Error al reducir memoria\n");
        exit(1);
    }
}

// Liberar memoria del array
void freeArray(DynamicArray* a){
    free(a->array);
    a->array = NULL;
    a->used  = a->size = 0;
}