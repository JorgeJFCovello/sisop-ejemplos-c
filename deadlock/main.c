#include <stdio.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <commons/collections/list.h>
#include <commons/collections/queue.h>
#include <commons/string.h>

int contador = 0;
void usarRecursosA(void);
void usarRecursosB(void);
void crear_hilo(pthread_t hilo, void* actividad);
sem_t recurso_a;
sem_t recurso_b;
sem_t finalizar;
pthread_mutex_t mutex;
int main(int argc, char** argv) {
    //// Recursos
    // pthread_mutex_init(&mutex, NULL); //Solucion punto B de la guia
    sem_init(&recurso_a,0,1);
    sem_init(&recurso_b,0,1);
    sem_init(&finalizar,0,0);

    for(int i; i<10; i++){
        pthread_t hilo;
        if(i%2 == 0){
            crear_hilo(hilo, usarRecursosA);
        } else {
            crear_hilo(hilo, usarRecursosB);
        }
    }
    for(int i; i<10; i++){
        sem_wait(&finalizar);
        printf("Finalizando nro %d\n", i);
    }
    return 0;
}

void crear_hilo(pthread_t hilo, void* actividad){
    pthread_create(&hilo, NULL, (void*) actividad, NULL);
    pthread_detach(hilo);
}

void usarRecursosA(){
    pthread_mutex_lock(&mutex);
    printf("Recursos solicitados A\n");
    sem_wait(&recurso_a);
    sleep(1);
    sem_wait(&recurso_b);
    printf("Recursos usados A\n");
    sleep(1);
    sem_post(&recurso_a);
    sem_post(&recurso_b);
    sem_post(&finalizar);
    pthread_mutex_unlock(&mutex);
}
// [ABC]
// [BCA]
void usarRecursosB(){
    // pthread_mutex_lock(&mutex);
    printf("Recursos solicitados B\n");
    sem_wait(&recurso_b);
    sem_wait(&recurso_a);
    sleep(2);
    printf("Recursos usados B\n");
    sem_post(&recurso_b);
    sem_post(&recurso_a);
    sem_post(&finalizar);
    // pthread_mutex_unlock(&mutex);
}
