#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "countlatch.h"
#define NTHREADS 10
/* C count_latch
 * compile with: gcc -Wall -pedantic -o simple simple_test_count.c countlatch.c -lpthread
 * simple test for count_latch
 */
void * fun(void *cl) {
    count_latch_t * s = cl;
    cl_wait_all(s);
    printf("Wait is over \n");
    return NULL;
}
int main() {
    count_latch_t cl;
    cl_init(&cl,NTHREADS);
    pthread_t threads[NTHREADS];
    printf(":: START ::\n");
    for(int i = 0; i < NTHREADS; i++) {
        pthread_create(&threads[i],NULL,fun,&cl);
    }
    printf(":: RUNNING ::\n");
    int i = 0;
    while (i++ < NTHREADS) {
        sleep(1);
        cl_up(&cl);
    }
    for(int i = 0; i < NTHREADS; i++) {
        pthread_join(threads[i],NULL);
    }
    printf(":: DONE ::\n");

    printf("NTHREADS: %d\n", NTHREADS );
    printf("Count  : %d\n", cl.count);
    countlatch_cleanup(&cl);
    return 0;
}