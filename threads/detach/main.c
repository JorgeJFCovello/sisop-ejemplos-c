#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int contador = 0;
int cantidad_sumadores = 1;
int cantidad_duplicadores = 1;

void sumar (int* id){
    while(1){
        contador++;
        printf("Sumador %d: contador = %d\n", *id, contador);
        sleep(2);
    }
}
void multiplicar (int* id){
    while(1){
        contador *= 2;
        printf("Multiplicador %d: contador = %d\n", *id, contador);
        sleep(2);
    }
}

void dividir (int* id){
    while(1){
        contador /= 2;
        printf("Divisor %d: contador = %d\n", *id, contador);
        sleep(2);
    }
}

void restar (int* id){
    while(1){
        contador--;
        printf("Resta %d: contador = %d\n", *id, contador);
        sleep(4);
    }
}
void crear_hilos () {
    for (int i = 0; i < cantidad_sumadores; i++) {
        pthread_t * sumador = malloc(sizeof(pthread_t));
        int* id = malloc(sizeof(int));
        *id = i+1;
        pthread_create(sumador, NULL, (void*)sumar, id);
        pthread_detach(*sumador);
    }
    for (int i = 0; i < cantidad_duplicadores; i++) {
        pthread_t * multiplicador = malloc(sizeof(pthread_t));
        int* id = malloc(sizeof(int));
        *id = i+1;
        pthread_create(multiplicador, NULL, (void*)multiplicar, id);
        pthread_detach(*multiplicador);
    }
    for (int i = 0; i < cantidad_duplicadores; i++) {
        pthread_t * divisor = malloc(sizeof(pthread_t));
        int* id = malloc(sizeof(int));
        *id = i+1;
        pthread_create(divisor, NULL, (void*)dividir, id);
        pthread_detach(*divisor);
    }
    for (int i = 0; i < cantidad_duplicadores; i++) {
        pthread_t * resta = malloc(sizeof(pthread_t));
        int* id = malloc(sizeof(int));
        *id = i+1;
        pthread_create(resta, NULL, (void*)restar, id);
        pthread_detach(*resta);
    }
}
int main() {
    crear_hilos();
    while(1){
        sleep(1);
    }
}