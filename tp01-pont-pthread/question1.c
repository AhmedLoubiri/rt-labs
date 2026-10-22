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
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int charge = 0;


/*fonction d'acquisition du pont*/
void acceder(char *type, int id, int poids) {
    pthread_mutex_lock(&mutex);
    while (charge + poids > MAX)
        pthread_cond_wait(&cond, &mutex);
    charge += poids;
    printf("%s %d entre   (charge = %d)\n", type, id, charge);
    pthread_mutex_unlock(&mutex);
}

/*fonction de liberation du pont*/
void liberer(char *type, int id, int poids) {
    pthread_mutex_lock(&mutex);
    charge -= poids;
    printf("%s %d sort    (charge = %d)\n", type, id, charge);
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
}
 
void *vehicule(void *arg) {
    int id = *(int *)arg;
    int poids = (id < NB_CAMIONS) ? 15 : 5;
    char *type = (id < NB_CAMIONS) ? "Camion" : "Voiture";
 
    usleep(rand() % 500000);
    acceder(type, id, poids);
    usleep(300000);
    liberer(type, id, poids);
    return NULL;
}
 
int main()
{
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