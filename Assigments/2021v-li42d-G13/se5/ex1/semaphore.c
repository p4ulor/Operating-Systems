#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
/* C semaphores with pthreads
 * compile with: gcc semaphore.c -Wall -lpthread -o semaph
 */
#define NTHREADS 20
#define NITERATIONS 20000000
int counter = 0;

typedef struct {
    pthread_mutex_t mut;
    pthread_cond_t cond;
    volatile unsigned n;
} semaphore_t;

// inicia o semáforo com o número de unidades especificado em initial.
void semaphore_init(semaphore_t *sem, int initial){
    pthread_mutex_init(&sem->mut,NULL);
    pthread_cond_init(&sem->cond,NULL);
    sem->n = initial;
}

// adquire, com timeout infinito, o número de unidades especificado em units.
void semaphore_acquire(semaphore_t *sem, int units){
    pthread_mutex_lock(&sem->mut);
    while(sem->n == 0) {
        pthread_cond_wait(&sem->cond,&sem->mut);
    }
    sem->n-=units;
    pthread_mutex_unlock(&sem->mut);
}

// adquire, com timeout de millismilisegundos, o número de unidades especificado em units.
//bool semaphore_acquire_timed(semaphore_t *sem, int units, long millis);

void semaphore_destroy(semaphore_t * sem) {
    pthread_cond_destroy(&sem->cond);
    pthread_mutex_destroy(&sem->mut);
}

// entrega o número de unidades ao semáforo especificado em units.
void semaphore_release(semaphore_t *sem, int units){
    pthread_mutex_lock(&sem->mut);
    sem->n+=units;
    pthread_mutex_unlock(&sem->mut);
    pthread_cond_signal(&sem->cond);
}

void *thread_func(void *arg) {
    semaphore_t * s = arg;

    semaphore_acquire(s,NTHREADS);
    for(int i = 0; i < NITERATIONS; i++) {
        ++counter;
    }
    semaphore_release(s,NTHREADS);
    return NULL;
}

int main(void) {
    
    semaphore_t semaph;
    
    semaphore_init(&semaph,NTHREADS);

    pthread_t threads[NTHREADS];
    printf(":: START ::\n");
    for(int i = 0; i < NTHREADS; i++) {
        pthread_create(&threads[i],NULL,thread_func,&semaph);
    }
    printf(":: RUNNING ::\n");

    for(int i = 0; i < NTHREADS; i++) {
        pthread_join(threads[i],NULL);
    }
    printf(":: DONE ::\n");

    printf("Expected: %d\n", NTHREADS * NITERATIONS);
	printf("Actual  : %d\n", counter);

	printf(":: END ::\n");
    semaphore_destroy(&semaph);
    return 0;
}