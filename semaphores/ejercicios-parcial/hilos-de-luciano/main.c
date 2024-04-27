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
void actividad1(void);
void actividad3(void);
void aumentarContador(void);
void crear_hilo(pthread_t hilo, void* actividad);
sem_t hilo_2_creado;
sem_t hilo_3_creado;
sem_t tope_contador;
sem_t finalizar_programa;
sem_t fin_contador;
pthread_mutex_t mx_contador;

int main(int argc, char** argv) {
    //SEMAFOROS
    //// MUTEX
    pthread_mutex_init(&mx_contador, NULL);

    //// CONTADORES
    sem_init(&hilo_2_creado,0,0);
    sem_init(&hilo_3_creado,0,0);
    sem_init(&tope_contador,0,0);
    sem_init(&finalizar_programa,0,0);
    sem_init(&fin_contador,0,100);

    pthread_t hilo_1;
    pthread_t hilo_3;

    crear_hilo(hilo_1, actividad1);
    sem_wait(&hilo_2_creado);
    crear_hilo(hilo_3, actividad3);
    sem_post(&hilo_3_creado);
    sem_wait(&finalizar_programa);

    return 0;
}

void crear_hilo(pthread_t hilo, void* actividad){
    pthread_create(&hilo, NULL, (void*) actividad, NULL);
    pthread_detach(hilo);
}

void actividad1(){
    pthread_t hilo_2;
    pthread_t hilo_4;
    crear_hilo(hilo_2, aumentarContador);
    sem_post(&hilo_2_creado);
    sem_wait(&hilo_3_creado);
    crear_hilo(hilo_4, aumentarContador);
}
void actividad3(){
    for(int i = 0; i<100; i++){
        sem_wait(&tope_contador); //x100
    }
    printf("Contador llegó a 100!\n");
    sem_post(&finalizar_programa);
}
void aumentarContador(){
    while(1){
        sem_wait(&fin_contador);
        pthread_mutex_lock(&mx_contador);
        contador++;
        pthread_mutex_unlock(&mx_contador);
        sem_post(&tope_contador);
    }
}
