#ifndef ARRAYS_DINAMICOS_H
#define ARRAYS_DINAMICOS_H

typedef struct{
    double* array; // Puntero al bloque de memoria 
    size_t used;   // Cantidad de elementos usados
    size_t size;   // Tamaño total disponible
} DynamicArray;

// Inicializar array dinámico
void initArray(DynamicArray* a, size_t initialSize);

// Agregar elemento al final de array
void insertArray(DynamicArray* a, double element);

// Reducir el tamaño del array para liberar memoria no utilizada
void trimToSize(DynamicArray* a);

// Liberar memoria del array
void freeArray(DynamicArray* a);

#endif // DYNAMIC_ARRAY_H