#include "countlatch.h"
#define LIMIT 20
void cl_init(count_latch_t *latch, int par_level){
	pthread_mutex_init(&latch->lock, NULL);
	pthread_cond_init(&latch->waiters, NULL);
	latch->count = par_level;
	latch->limit = LIMIT;
}
void countlatch_cleanup(count_latch_t * cdl) {
	pthread_mutex_destroy(&cdl->lock);
	pthread_cond_destroy(&cdl->waiters);
}
void cl_up(count_latch_t *latch){
	pthread_mutex_lock(&latch->lock);
	{
        int max=latch->limit;
		if (latch->count > 0&&latch->count < max) {
            latch->count += 1;
            if (latch->count == max) {
				pthread_cond_broadcast(&latch->waiters);
			}
        }
	}
	pthread_mutex_unlock(&latch->lock);
}
void cl_down(count_latch_t *latch){
	pthread_mutex_lock(&latch->lock);
		if (latch->count > 0) {
			latch->count -= 1;
			if (latch->count == 0) {
				pthread_cond_broadcast(&latch->waiters);
			}
		}
		pthread_mutex_unlock(&latch->lock);
}

void cl_wait_all(count_latch_t *latch){
	pthread_mutex_lock(&latch->lock);
	{
		while (latch->count > 0&& latch->count<latch->limit) {
			pthread_cond_wait(&latch->waiters, &latch->lock);
		}
	}
	pthread_mutex_unlock(&latch->lock);
}