#ifndef ZIGGURAT
#define ZIGGURAT  // Añado esta protección de inclusión

// Valores de fábrica que necesitamos
// Doubles-----------------------------------
extern double m1;
extern double m2;
extern double half;
extern double dn;
extern double tn;
extern double vn;
extern double q;
extern double de;
extern double te;
extern double ve;
extern double wn[128], fn[128], we[256], fe[256];

// Integers ----------------------------------
extern int iz, jz;
extern int jsr;
extern int kn[128], ke[256], hz;

// Bool --------------------------------------
extern int initialized; // No tenemos booleanos estandar en C, solo ints

// Lectura de semilla
int seed_read();

// Escritura de semilla
void seed_write();

// Generación random de un número de 32 bits
int shr3();

// Distribución uniforme
double uni();

// Distribución normal
double rnor();

// inicialización de semilla
void initialize_random();

// Finalización de semilla
void finalize_random();

// Distribución normal
//double rexp();

#endif // ZIGGURAT_H
