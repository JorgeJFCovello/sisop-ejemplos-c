#include <stdlib.h>
#include <stdio.h>

#define TAMANIO_COCHERA 20

void* cochera[TAMANIO_COCHERA] = {NULL};
int tamanio_cochera = TAMANIO_COCHERA;

void agregar_a_cochera(void* elemento) {
    for (int i = 0; i < TAMANIO_COCHERA; i++) { // Buscar el primer espacio vacio del cochera (NULL)
        if (cochera[i] == NULL) {
            cochera[i] = elemento; // Agrega el elemento al cochera
            return; // Salir de la función
        }
    }
    // Si llega hasta acá es porque no hay espacio en el cochera
    printf("No se pudo agregar el elemento al cochera \n");
}

void* tomar_de_cochera(void) {
    void* elemento;
    for (int i = 0; i < TAMANIO_COCHERA; i++) { // Buscar el primer elemento del cochera que no sea NULL
        if (cochera[i] != NULL) {
            elemento = cochera[i]; // Tomar el elemento del cochera
            cochera[i] = NULL; // Liberar la posición del cochera para almacenar otro elemento en el futuro
            return elemento;
        }
    }
    // Si llega hasta acá es porque no hay elementos en el cochera
    printf("No se pudo tomar el elemento del cochera \n");
}