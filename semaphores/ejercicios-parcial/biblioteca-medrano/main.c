#include <stdio.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <commons/collections/list.h>

pthread_mutex_t mx_devoluciones;
pthread_mutex_t mx_retiros;
pthread_mutex_t mx_reservas;
pthread_mutex_t mx_festejo;

sem_t reservasListas;
sem_t devolucionesListas;
sem_t retirosListos;
sem_t copiasListas[5];

struct t_libro {
    int id;
    char* nombre;
};

int contFestejo = 0;
t_list* devoluciones;
t_list*  reservas;
t_list* retiros;
struct t_libro* titulos[5];
void atenderReservas(void* args);
void atenderDevoluciones(void* args);
void cliente(int args);
struct t_libro* elegirLibro(void);
void estudiar();
void festejar();
void irABiblioteca();
void registrarDisponible(struct t_libro* libro);
struct t_libro* tomar(t_list* list);
void agregar(t_list* list, struct t_libro* libro);
void retirar(t_list* list, int id);

int main(int argc, char** argv) {
    devoluciones = list_create();
    reservas = list_create();
    retiros = list_create();

    //SEMAFOROS
    //// MUTEX
    pthread_mutex_init(&mx_devoluciones, NULL);
    pthread_mutex_init(&mx_retiros, NULL);
    pthread_mutex_init(&mx_reservas, NULL);
    pthread_mutex_init(&mx_festejo, NULL);

    //BINARIOS
    sem_init(&reservasListas,0,0);
    sem_init(&devolucionesListas,0,0);
    sem_init(&retirosListos,0,0);

    for(int i = 0; i<5; i++){
        struct t_libro* nuevoTitulo = malloc(sizeof(struct t_libro));
        nuevoTitulo->id=i;
        nuevoTitulo->nombre = malloc(15);
        snprintf(nuevoTitulo->nombre, 15, "Mi Libro nro %d", i);
        titulos[i] = nuevoTitulo;
        sem_init(&copiasListas[i],0,20);
    }

    pthread_t hilo_reservas1, hilo_reservas2;
    pthread_create(&hilo_reservas1, NULL, (void*) atenderReservas, NULL);
    pthread_create(&hilo_reservas2, NULL, (void*) atenderReservas, NULL);
    pthread_detach(hilo_reservas1);
    pthread_detach(hilo_reservas2);

    pthread_t hilo_devoluciones1, hilo_devoluciones2;
    pthread_create(&hilo_devoluciones1, NULL, (void*) atenderDevoluciones, NULL);
    pthread_create(&hilo_devoluciones2, NULL, (void*) atenderDevoluciones, NULL);
    pthread_detach(hilo_devoluciones1);
    pthread_detach(hilo_devoluciones2);

    int id = 1;
    while(1){
        pthread_t hilo_cliente;
        pthread_create(&hilo_cliente, NULL, (void*) cliente, (void*)id);
        pthread_detach(hilo_cliente);
        id++;
    }

    return 0;
}

void cliente(int id) {
    printf("Llega un nuevo cliente \n");
    struct t_libro* libro = elegirLibro();
    printf("El cliente %d eligio el libro %d \n", id, libro->id);
    sem_wait(&copiasListas[libro->id]);
    pthread_mutex_lock(&mx_reservas);
    agregar(reservas, libro);
    pthread_mutex_unlock(&mx_reservas);
    sem_post(&reservasListas);
    printf("El cliente %d fue a la bliblioteca \n", id);
    irABiblioteca();
    sem_wait(&retirosListos);
    pthread_mutex_lock(&mx_retiros);
    retirar(retiros, libro->id);
    pthread_mutex_unlock(&mx_retiros);
    printf("El cliente %d esta estudiando \n", id);
    estudiar();
    printf("El cliente %d fue a devolver a la biblioteca \n", id);
    irABiblioteca();
    pthread_mutex_lock(&mx_devoluciones);
    agregar(devoluciones, libro);
    pthread_mutex_unlock(&mx_devoluciones);
    sem_post(&devolucionesListas);
}

void atenderReservas(void* args){
    while(1){
        sem_wait(&reservasListas);
        pthread_mutex_lock(&mx_reservas);
        struct t_libro* libro = tomar(reservas);
        pthread_mutex_unlock(&mx_reservas);
        pthread_mutex_lock(&mx_retiros);
        agregar(retiros, libro);
        pthread_mutex_unlock(&mx_retiros);
        sem_post(&retirosListos);
    }
}
void atenderDevoluciones(void* args){
    while(1){
        sem_wait(&devolucionesListas);
        pthread_mutex_lock(&mx_devoluciones);
        struct t_libro* libro = tomar(devoluciones);
        pthread_mutex_unlock(&mx_devoluciones);
        registrarDisponible(libro);
        sem_post(&copiasListas[libro->id]);
        pthread_mutex_lock(&mx_festejo);
        if(contFestejo < 100 ) {
            contFestejo++;
        } else {
            contFestejo = 0; 
            festejar();
        }
        pthread_mutex_unlock(&mx_festejo);
    }
}

struct t_libro* tomar(t_list* list){
    return list_get(list, 0);
}
void retirar(t_list* list, int id){
    int find_by_id(struct t_libro* libro) {
        return libro->id==libro;
    } 
    list_remove_by_condition(list, (void*)find_by_id);
}
void agregar(t_list* list, struct t_libro* libro){
    list_add(list, libro);
}

void estudiar(){
    sleep(10);
}
void festejar(){
    sleep(20);
}
void irABiblioteca(){
    sleep(2);
}

struct t_libro* elegirLibro(){
    time_t timeNow = time(NULL);
    int position = timeNow % 5;
    struct t_libro* libroElegido = titulos[position];
    return libroElegido;
}

void registrarDisponible(struct t_libro* libro){
    printf("Registre el libro %d como disponible!", libro->id);
}