#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>

pthread_mutex_t mutex_autos_esperando, mutex_subida, mutex_bajada;

sem_t sem_puente, llego_auto_subida, llego_auto_bajada;

int cantidad_autos_esperando = 0, contador_subida = 0, contador_bajada = 0;

void autos(void* args);
void bajada(void* args);
void subida(void* args);
void decrementar_autos_esperando(void);

int main(int argc, char** argv) {
    // Inicializar semáforos mutex en 1
    pthread_mutex_init(&mutex_autos_esperando, NULL);
    pthread_mutex_init(&mutex_subida, NULL);
    pthread_mutex_init(&mutex_bajada, NULL);

    // Semáforo binario para controlar el puente
    sem_init(&sem_puente, 0, 1);

    // Semáforos para controlar la llegada de autos para subir y bajar
    sem_init(&llego_auto_subida, 0, 0);
    sem_init(&llego_auto_bajada, 0, 0);

    // Crear los hilos
    pthread_t thread_autos, thread_autos_subida, thread_autos_bajada;
    pthread_create(&thread_autos, NULL, (void*) autos, NULL);
    pthread_join(thread_autos, NULL);

    return 0;
}

void autos(void* args) {
    int i = 1;

    while (1) {
        pthread_mutex_lock(&mutex_autos_esperando);
        cantidad_autos_esperando++;
        printf("Autos esperando: %d \n", cantidad_autos_esperando);
        pthread_mutex_unlock(&mutex_autos_esperando);

        int r = rand() % 2; // Número aleatorio entre 0 y 1
        pthread_t thread_auto;

        int* id = malloc(sizeof(int));
        *id = i;
        i++;

        if (r == 0) {
            printf("Auto ID:%d llegó por la subida \n", *id);
            pthread_create(&thread_auto, NULL, (void*) subida, id);
            pthread_detach(thread_auto);
        } 
        else {
            printf("Auto ID:%d llegó por la bajada \n", *id);
            pthread_create(&thread_auto, NULL, (void*) bajada, id);
            pthread_detach(thread_auto);
        }

        sleep(2);
    }
}

void subida(void* args) {
    int id = *((int*) args);
    free(args);

    pthread_mutex_lock(&mutex_bajada);
    contador_bajada++;
    if (contador_bajada == 1) {
        sem_wait(&sem_puente);
    }
    pthread_mutex_unlock(&mutex_bajada);

    // Se sube el puente
    printf("Auto ID:%d accede a la subida del puente \n", id);
    sleep(3);
    printf("Auto ID:%d termina de subir al puente \n", id);

    pthread_mutex_lock(&mutex_bajada);
    contador_bajada--;
    if (contador_bajada == 0) {
        sem_post(&sem_puente);
    }
    pthread_mutex_unlock(&mutex_bajada);

    decrementar_autos_esperando();
}

void bajada(void* args) {
    int id = *((int*) args);
    free(args);

    pthread_mutex_lock(&mutex_subida);
    contador_subida++;
    if (contador_subida == 1) {
        sem_wait(&sem_puente);
    }
    pthread_mutex_unlock(&mutex_subida);

    // Se baja el puente
    printf("Auto ID:%d accede a la bajada \n", id);
    sleep(3);
    printf("Auto ID:%d termina de bajar por el puente \n", id);
    

    pthread_mutex_lock(&mutex_subida);
    contador_subida--;
    if (contador_subida == 0) {
        sem_post(&sem_puente);
    }
    pthread_mutex_unlock(&mutex_subida);

    decrementar_autos_esperando();
}

void decrementar_autos_esperando(void) {
    pthread_mutex_lock(&mutex_autos_esperando);
    cantidad_autos_esperando--;
    printf("Autos esperando: %d \n", cantidad_autos_esperando);
    pthread_mutex_unlock(&mutex_autos_esperando);
}