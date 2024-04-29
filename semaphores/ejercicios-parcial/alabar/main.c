#include <stdio.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <commons/collections/list.h>

sem_t sem_a;
sem_t sem_b;
sem_t sem_l;
sem_t sem_r;
sem_t sem_alabar;


void a(void);
void l(void);
void b(void);
void r(void);

int main(int argc, char** argv) {

    //SEMAFOROS
    //// MUTEX
    ///// no hay mutex en este ejercicio
    //BINARIOS
    sem_init(&sem_a,0,1);
    sem_init(&sem_b,0,0); // poner 1 en vez de 0 y ver que pasa
    sem_init(&sem_l,0,1);
    sem_init(&sem_r,0,0);
    sem_init(&sem_alabar,0,0);



    pthread_t hiloA, hiloB, hiloL, hiloR;
    pthread_create(&hiloA, NULL, (void*) a, NULL);
    pthread_detach(hiloA);
    pthread_create(&hiloB, NULL, (void*) b, NULL);
    pthread_detach(hiloB);
    pthread_create(&hiloL, NULL, (void*) l, NULL);
    pthread_detach(hiloL);
    pthread_create(&hiloR, NULL, (void*) r, NULL);
    pthread_join(hiloR, NULL);

    return 0;
}



void r(){
    while(1){
        sem_wait(&sem_r);
        sem_wait(&sem_alabar);
        printf("R\n");
        sleep(1);
        sem_post(&sem_a);
        sem_post(&sem_l);
    }
}
void a(){
    while(1){
        sem_wait(&sem_a);
        printf("A");
        sem_post(&sem_alabar);
    }
}
void l(){
    while(1){
        sem_wait(&sem_l);
        sem_wait(&sem_alabar);
        printf("L");
        sem_post(&sem_a);
        sem_post(&sem_b);
    }
}
void b(){
    while(1){
        sem_wait(&sem_b);
        sem_wait(&sem_alabar);
        printf("B");
        sem_post(&sem_a);
        sem_post(&sem_r);
    }
}