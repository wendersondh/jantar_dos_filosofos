#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h> /* Para a função sleep */

#define N 5 /* número de filósofos */
#define ESQUERDA (i + N - 1) % N /* vizinho à esquerda */
#define DIREITA (i + 1) % N /* vizinho à direita */
#define PENSANDO 0 /* filósofo está pensando */
#define FAMINTO 1 /* filósofo está tentando pegar os garfos */
#define COMENDO 2 /* filósofo está comendo */

/* Estado dos filósofos e semáforos */
int estado[N];
sem_t mutex;
sem_t semaforos[N];

/* Função que simula o pensamento e a alimentação */
void esperar(int tempo) {
    sleep(tempo); /* Aguarda 'tempo' segundos */
}

/* Função que um filósofo usa para pegar os garfos */
void pegar_garfos(int i) {
    sem_wait(&mutex); /* entra na região crítica */
    estado[i] = FAMINTO; /* registra que o filósofo i está com fome */
    if (estado[ESQUERDA] != COMENDO && estado[DIREITA] != COMENDO) {
        estado[i] = COMENDO; 
        sem_post(&semaforos[i]); /* libera o semáforo do filósofo i */
    }
    sem_post(&mutex); /* sai da região crítica */
    sem_wait(&semaforos[i]); /* bloqueia se os garfos não foram adquiridos */
}

/* Função que um filósofo usa para colocar os garfos de volta na mesa */
void colocar_garfos(int i) {
    sem_wait(&mutex); /* entra na região crítica */
    estado[i] = PENSANDO; /* filósofo terminou de comer */
    if (estado[ESQUERDA] == FAMINTO && estado[ESQUERDA] != COMENDO) {
        estado[ESQUERDA] = COMENDO; 
        sem_post(&semaforos[ESQUERDA]); /* libera o semáforo do vizinho da esquerda */
    }
    if (estado[DIREITA] == FAMINTO && estado[DIREITA] != COMENDO) {
        estado[DIREITA] = COMENDO; 
        sem_post(&semaforos[DIREITA]); /* libera o semáforo do vizinho da direita */
    }
    sem_post(&mutex); /* sai da região crítica */
}

/* Função principal do filósofo */
void* filosofo(void* num) {
    int i = *(int*)num;
    while (1) {
        printf("Filosofo %d esta pensando...\n", i+1);
        esperar(3); /* Simula o pensamento */
        printf("Filosofo %d esta faminto...\n", i+1);
        pegar_garfos(i);
        printf("Filosofo %d esta comendo...\n", i+1);
        esperar(3); /* Simula a alimentação */
        printf("Filosofo %d terminou de comer...\n", i+1);
        colocar_garfos(i);
    }
    return NULL;
}

/* Função principal do programa */
void main() {
    pthread_t threads[N];
    int i;

    sem_init(&mutex, 0, 1); /* inicializa o semáforo mutex */
    for (i = 0; i < N; i++) {
        sem_init(&semaforos[i], 0, 0); /* inicializa semáforos individuais */
        estado[i] = PENSANDO; /* todos os filósofos começam pensando */
    }

    for (i = 0; i < N; i++) {
        int* num = malloc(sizeof(int));
        *num = i;
        pthread_create(&threads[i], NULL, filosofo, num);
    }

    for (i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    /* Limpeza */
    sem_destroy(&mutex);
    for (i = 0; i < N; i++) {
        sem_destroy(&semaforos[i]);
    }


}
