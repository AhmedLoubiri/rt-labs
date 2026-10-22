#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define MAX 15
#define POIDS_CAMION 15
#define POIDS_VOITURE 5
#define NB_CAMIONS 4
#define NB_VOITURES 10

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c_camion = PTHREAD_COND_INITIALIZER;
pthread_cond_t c_voiture = PTHREAD_COND_INITIALIZER;
int charge = 0;
int camions_attente = 0;

void acceder_camion(int id) {
    pthread_mutex_lock(&mutex);
    camions_attente++;
    while (charge > 0)
        pthread_cond_wait(&c_camion, &mutex);
    camions_attente--;
    charge += 15;
    printf("Camion %d entre   (charge = %d)\n", id, charge);
    pthread_mutex_unlock(&mutex);
}

void acceder_voiture(int id) {
    pthread_mutex_lock(&mutex);
    while (charge + 5 > MAX || camions_attente > 0)
        pthread_cond_wait(&c_voiture, &mutex);
    charge += 5;
    printf("Voiture %d entre  (charge = %d)\n", id, charge);
    pthread_mutex_unlock(&mutex);
}

void liberer(char *type, int id, int poids) {
    pthread_mutex_lock(&mutex);
    charge -= poids;
    printf("%s %d sort    (charge = %d)\n", type, id, charge);

    if (camions_attente > 0) {
        if (charge == 0)
            pthread_cond_signal(&c_camion);
    } else {
        pthread_cond_broadcast(&c_voiture);
    }
    pthread_mutex_unlock(&mutex);
}

void *vehicule(void *arg) {
    int id = *(int *)arg;

    usleep(rand() % 500000);
    if (id < NB_CAMIONS) {
        acceder_camion(id);
        usleep(300000);
        liberer("Camion", id, 15);
    } else {
        acceder_voiture(id);
        usleep(300000);
        liberer("Voiture", id, 5);
    }
    return NULL;
}

int main() {
    int n = NB_CAMIONS + NB_VOITURES;
    pthread_t th[n];
    int ids[n];

    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        ids[i] = i;
        pthread_create(&th[i], NULL, vehicule, &ids[i]);
    }
    for (int i = 0; i < n; i++)
        pthread_join(th[i], NULL);

    return 0;
}