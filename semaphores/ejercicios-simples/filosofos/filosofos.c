#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

#define CANTIDAD_FILOSOFOS  5

typedef enum {
    PENSANDO,
    HAMBRIENTO,
    COMIENDO
} t_estado;

t_estado filosofos[CANTIDAD_FILOSOFOS];
sem_t sem_filosofos[CANTIDAD_FILOSOFOS];
pthread_mutex_t mutex_tenedores;

void filosofo(void* args);
void tomar_tenedores(int id_filosofo);
void dejar_tenedores(int id_filosofo);
void comprobar(int id_filosofo);
bool podria_comer(int id_filosofo);
int id_sentado_a_la_derecha(int id_filosofo);
int id_sentado_a_la_izquierda(int id_filosofo);

int main(int argc, char** argv) {
    // Inicializar los semáforos de todos los filósofos en 0
    for (int i = 0; i < CANTIDAD_FILOSOFOS; i++) {
        filosofos[i] = PENSANDO;
        sem_init(&sem_filosofos[i], 0, 0);
    }

    // Inicializar semáforo mutex de tenedores (Al ser un mutex, se inicializa en 1)
    pthread_mutex_init(&mutex_tenedores, NULL);

    // Crear hilos de los filósofos para que se sienten en la mesa
    pthread_t filosofos[CANTIDAD_FILOSOFOS];
    for (int i = 0; i < CANTIDAD_FILOSOFOS; i++) {
        int* id_filosofo = malloc(sizeof(int));
        *id_filosofo = i;
        pthread_create(&filosofos[i], NULL, (void*) filosofo, id_filosofo);
    }

    // Detener la ejecución del hilo main hasta que todos los filósofos terminen (spoiler: nunca)
    for (int i = 0; i < CANTIDAD_FILOSOFOS; i++) {
        pthread_join(filosofos[i], NULL);
    }

    return 0;
}

void filosofo(void* args) {
    int id_filosofo = *((int*) args);
    free(args);
    printf("Filosofo %d se sienta a la mesa \n", id_filosofo + 1);

    while (1) {
        // Pensar
        printf("Filosofo %d esta pensando... \n", id_filosofo + 1);
        sleep(2);
        printf("Filosofo %d tiene hambre \n", id_filosofo + 1);

        tomar_tenedores(id_filosofo);

        // Comer
        printf("Filosofo %d esta comiendo... \n", id_filosofo + 1);
        sleep(2);
        printf("Filosofo %d termina de comer \n", id_filosofo + 1);

        dejar_tenedores(id_filosofo);
    }
}

void tomar_tenedores(int id_filosofo) {
    pthread_mutex_lock(&mutex_tenedores);

    filosofos[id_filosofo] = HAMBRIENTO;
    comprobar(id_filosofo);
    
    pthread_mutex_unlock(&mutex_tenedores);

    // Esperar a que le indiquen que puede tomar los tenedores
    sem_wait(&sem_filosofos[id_filosofo]);
}

void comprobar(int id_filosofo) {
    /**
     * Comprobar si un filósofo puede comer (tiene tenedores disponibles), en caso de que pueda, 
     * se le indica que puede tomar los tenedores
     */ 
    if (podria_comer(id_filosofo)) {
        filosofos[id_filosofo] = COMIENDO;
        sem_post(&sem_filosofos[id_filosofo]);
    }
}

bool podria_comer(int id_filosofo) {
    /**
     * Indica si el filósofo indicado podría proceder a comer, esto pasa si:
     * - El filósofo indicado está hambriendo 
     * - Los filósofos de su izquierda y derecha no están comiendo (no están comiendo -> no están usando tenedores)
     */
    return filosofos[id_filosofo] == HAMBRIENTO 
           && filosofos[id_sentado_a_la_izquierda(id_filosofo)] != COMIENDO 
           && filosofos[id_sentado_a_la_derecha(id_filosofo)] != COMIENDO;
}

int id_sentado_a_la_izquierda(int id_filosofo) {
    /** 
     *Devuelve el número del filósofo sentado a la derecha del filósofo indicado
     */
    if (id_filosofo == 0) {
        return CANTIDAD_FILOSOFOS - 1;
    } 
    return id_filosofo - 1;
}

int id_sentado_a_la_derecha(int id_filosofo) {
    /** 
     *Devuelve el número del filósofo sentado a la derecha del filósofo indicado
     */
    if (id_filosofo == CANTIDAD_FILOSOFOS - 1) {
        return 0;
    } 
    return id_filosofo + 1;
}

void dejar_tenedores(int id_filosofo) {
    pthread_mutex_lock(&mutex_tenedores);

    filosofos[id_filosofo] = PENSANDO;

    // Comprobar si los filósofos de los lados pueden comer
    comprobar((id_filosofo - 1) % CANTIDAD_FILOSOFOS);
    comprobar((id_filosofo + 1) % CANTIDAD_FILOSOFOS);

    pthread_mutex_unlock(&mutex_tenedores);
}