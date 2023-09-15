#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex_buffer;
sem_t contador_vacios; // Representa cantidad de espacios vacíos en el buffer
sem_t contador_llenos; // Representa cantidad de espacios llenos en el buffer (cantidad de recursos disponibles)

extern void agregar_a_buffer(void* elemento);
extern void* tomar_de_buffer(void);

void productor(void* args);
void consumidor(void* args);
char* producir_elemento(void);

int main(int argc, char** argv) {
    int cantidad_productores = 1, cantidad_consumidores = 3;

    if (argc == 3) {
        // Se reciben los parametros por consola para definir la cantidad de productores y consumidores
        cantidad_productores = atoi(argv[1]);
        cantidad_consumidores = atoi(argv[2]);
    }

    // Inicializar semáforos
    extern int tamanio_buffer;
    pthread_mutex_init(&mutex_buffer, NULL);
    sem_init(&contador_vacios, 0, tamanio_buffer);
    sem_init(&contador_llenos, 0, 0);

    // Crear hilos de los productores
    pthread_t productores[cantidad_productores];
    for (int i = 0; i < cantidad_productores; i++) {
        int* id_productor = malloc(sizeof(int));
        *id_productor = i;
        pthread_create(&productores[i], NULL, (void*) productor, id_productor);
    }

    // Crear hilos de los consumidores
    pthread_t consumidores[cantidad_consumidores];
    for (int i = 0; i < cantidad_consumidores; i++) {
        int* id_consumidor = malloc(sizeof(int));
        *id_consumidor = i;
        pthread_create(&consumidores[i], NULL, (void*) consumidor, id_consumidor);
    }

    // Esperar a que terminen los hilos de los productores
    for (int i = 0; i < cantidad_productores; i++) {
        pthread_join(productores[i], NULL);
    }

    // Esperar a que terminen los hilos de los consumidores
    for (int i = 0; i < cantidad_consumidores; i++) {
        pthread_join(consumidores[i], NULL);
    }
}

void productor(void* args) {
    int id_productor = *((int*) args) + 1;
    free(args);

    while(1) {
        char* elemento = producir_elemento();
        
        sem_wait(&contador_vacios); // Avanzar cuando haya espacio en el buffer
        
        /* Escribir el buffer, asegurando mutua exclusión */
        pthread_mutex_lock(&mutex_buffer);
        printf("Productor ID:%d agrega elemento <<%c>> al buffer \n", id_productor, *elemento);
        agregar_a_buffer(elemento);
        pthread_mutex_unlock(&mutex_buffer);
        
        sem_post(&contador_llenos); // Avisa que hay un recurso más en el buffer
    }
}

char* producir_elemento(void) {
    /* Simula producción de un elemento. Retorna un caracter aleatorio */
    sleep(3); // 😴 (Simula el tiempo que se tarda en producir)

    char* elemento = malloc(sizeof(char));
    
    // Genera un caracter aleatorio
    *elemento = "abcdefghijklmnopqrstuvwxyz"[rand() % 26];
    
    return elemento;
}

void consumidor(void* args) {
    int id_consumidor = *((int*) args) + 1;
    free(args);

    while(1) {
        sem_wait(&contador_llenos); // Espera a que haya un recurso producido (buffer lleno)

        /* Consumir un elemento del buffer, asegurando mutua exclusión */
        pthread_mutex_lock(&mutex_buffer);
        char* elemento = (char*) tomar_de_buffer(); // Tomar un elemento del buffer
        printf("Consumidor ID:%d consume <<%c>> \n", id_consumidor, *elemento);
        pthread_mutex_unlock(&mutex_buffer);

        sem_post(&contador_vacios); // Avisa que hay un espacio vacio (buffer vacio)
        
        sleep(2); // 😴 (Simula el tiempo que tarda en consumir)
        free(elemento);
    }
}