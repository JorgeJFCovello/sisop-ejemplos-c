#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>

pthread_mutex_t mutex_puerta;

extern void agregar_a_cochera(void* elemento);
extern void* tomar_de_cochera(void);

sem_t espacios_disponibles, espacios_llenos, finalizacion;

int cantidad_espacios_disponibles = 20, cantidad_espacios_llenos = 0;

void sacar_auto(void);
void entrar_auto(void);
void* crearAuto();

int main(int argc, char** argv) {
    // Inicializar semáforos mutex en 1
    pthread_mutex_init(&mutex_puerta, NULL);

    // Semáforos para controlar la llegada de autos para subir y bajar
    sem_init(&espacios_disponibles, 0, cantidad_espacios_disponibles);
    sem_init(&espacios_llenos, 0, cantidad_espacios_llenos);
    sem_init(&finalizacion, 0, 0);

    // Crear los hilos
    for(int i=0; i<100; i++){
        pthread_t thread_autos_salida, thread_autos_entrada;
        pthread_create(&thread_autos_salida, NULL, (void*) sacar_auto, NULL);
        pthread_create(&thread_autos_entrada, NULL, (void*) entrar_auto, NULL);
        pthread_detach(thread_autos_entrada);
        pthread_detach(thread_autos_salida);
    }

    for(int i=0; i<200;i++){
        sem_wait(&finalizacion); //x200
    }

    return 0;
}


void sacar_auto(){
    //sleep(4);
    sem_wait(&espacios_llenos);
    pthread_mutex_lock(&mutex_puerta);
    char* patente = tomar_de_cochera();
    printf("Sacando auto: %s \n", patente);
    pthread_mutex_unlock(&mutex_puerta);
    sem_post(&espacios_disponibles);
    free(patente);
    sem_post(&finalizacion);
}

void* crearAuto(){
    char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";        
    char *patente = NULL;
    patente = malloc(sizeof(char) * (7));
    if (patente) {            
        for (int n = 0;n < 6;n++) {            
            int key = rand() % (int)(sizeof(charset) -1);
            patente[n] = charset[key];
        }

        patente[6] = '\0';
    }
    return patente;
}
void entrar_auto(){
    //sleep(3);
    char* nuevoAuto = crearAuto();
    sem_wait(&espacios_disponibles);
    pthread_mutex_lock(&mutex_puerta);
    agregar_a_cochera(nuevoAuto);
    printf("Entrando auto: %s \n", nuevoAuto);
    pthread_mutex_unlock(&mutex_puerta);
    sem_post(&espacios_llenos);
    sem_post(&finalizacion);
}
