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


int CANTIDAD_PRODUCTORES = 5;
int CANTIDAD_RECURSOS = 5;
int CANTIDAD_CONSUMIDORES = 8;

pthread_mutex_t mx_cola;
pthread_mutex_t mx_recursos_disponibles;
pthread_mutex_t mx_por_recurso[5];
pthread_mutex_t mx_solicitud_recurso;

t_queue* cola;
t_list* recursos_disponibles;

sem_t productor_consumidor;

void productor(int* args);
void consumidor(int* args);
void* usar_recurso(int id);
int pedir_recurso(void);

int main(int argc, char** argv) {
    cola = queue_create();
    recursos_disponibles = list_create();
    for(int i = 0; i < CANTIDAD_RECURSOS; i++){
        void* recurso = malloc(sizeof(char)*10);
        strcpy(recurso, "Recurso ");
        strcpy(recurso, string_itoa(i));
        list_add(recursos_disponibles, recurso);
    }

    //SEMAFOROS
    //// MUTEX
    pthread_mutex_init(&mx_cola, NULL);
    pthread_mutex_init(&mx_recursos_disponibles, NULL);
    for(int i = 0; i < CANTIDAD_RECURSOS; i++){
        pthread_mutex_init(&mx_por_recurso[i], NULL);
    }
    pthread_mutex_init(&mx_solicitud_recurso, NULL);

    //// CONTADORES
    sem_init(&productor_consumidor,0,0);



    for(int i = 0; i < CANTIDAD_PRODUCTORES; i++){
        pthread_t hilo_productor;
        int* id = malloc(sizeof(int));
        *id = i+1;
        pthread_create(&hilo_productor, NULL, (void*) productor, id);
        pthread_detach(hilo_productor);
    }

    for(int i = 0; i < CANTIDAD_CONSUMIDORES; i++){
        int* id = malloc(sizeof(int));
        *id = i+1;
        pthread_t * hilo_consumidor;
        hilo_consumidor = malloc(sizeof(pthread_t));
        pthread_create(hilo_consumidor, NULL, (void*) consumidor, id);
    }

    while(1){
        printf("Elementos en la cola: %d\n", queue_size(cola));
        sleep(2);
    }

    return 0;
}

void consumidor(int* id) {
    while(1){
        sem_wait(&productor_consumidor);
        pthread_mutex_lock(&mx_cola);
        void* resultado = queue_pop(cola);
        pthread_mutex_unlock(&mx_cola);
        printf("Consumidor %d: %s\n", *id, resultado);
        sleep(1);
    }
}
void productor(int* id) {
    while(1){
        pthread_mutex_lock(&mx_solicitud_recurso);
        int id_recurso = pedir_recurso();
        pthread_mutex_unlock(&mx_solicitud_recurso);
        pthread_mutex_lock(&mx_por_recurso[id_recurso]);
        void* resultado = usar_recurso(id_recurso);
        pthread_mutex_unlock(&mx_por_recurso[id_recurso]);
        printf("Productor %d: %s\n", *id, resultado);
        pthread_mutex_lock(&mx_cola);
        queue_push(cola, resultado);
        pthread_mutex_unlock(&mx_cola);
        sem_post(&productor_consumidor);
        sleep(1);
     }
}

int pedir_recurso(void) {
    return rand() % CANTIDAD_RECURSOS; 
}

void* usar_recurso(int id) {
    pthread_mutex_lock(&mx_recursos_disponibles);
    void* frase = list_get(recursos_disponibles, id);
    pthread_mutex_unlock(&mx_recursos_disponibles);
    return frase;
}

