#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void imprimir_mensaje (int nro_msg){
    sleep(3);
    printf("Mensaje %d\n", nro_msg);
}
void imprimir_mensaje_con_mas_tiempo (int nro_msg){
    sleep(13);
    printf("Mensaje %d\n", nro_msg);
}
void crear_hilos () {
    pthread_t * primer_hilo = malloc(sizeof(pthread_t));
    pthread_create(primer_hilo, NULL, imprimir_mensaje_con_mas_tiempo, 1);
    pthread_t * segundo_hilo = malloc(sizeof(pthread_t));
    pthread_create(segundo_hilo, NULL, imprimir_mensaje, 2);
    pthread_t * tercer_hilo = malloc(sizeof(pthread_t));
    pthread_create(tercer_hilo, NULL, imprimir_mensaje, 3);
    pthread_t * cuarto_hilo = malloc(sizeof(pthread_t));
    pthread_create(cuarto_hilo, NULL, imprimir_mensaje, 4);
    pthread_join(*primer_hilo,NULL);
    printf("Use un join :O!\n");
    pthread_detach(*segundo_hilo);
    pthread_detach(*tercer_hilo);
    pthread_detach(*cuarto_hilo);
}
int main() {
    crear_hilos();
}