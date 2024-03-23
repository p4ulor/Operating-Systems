#pragma once

#include <pthread.h>

typedef struct count_latch {
	pthread_mutex_t lock;
	pthread_cond_t waiters;
	int count;
	int limit;
} count_latch_t;


// Inicia  o count  latchcom  o nível  máximo  de  paralelismo especificado  em par_level.  Caso  o  valor  de
// par_level seja 0, considera-se não haver limite ao nível de paralelismo.
void cl_init(count_latch_t *latch, int par_level);
// Espera que a contagem das unidades de trabalho seja 0.
void cl_wait_all(count_latch_t *latch); // bloqueante
//Contabiliza  mais  uma  unidadede  trabalho.  Bloqueia  a  thread  invocante  caso  o  nível  máximo  de
//paralelismo tenha sido atingido.
void cl_up(count_latch_t *latch);
// Contabiliza menos uma unidadede trabalho. Caso se tenha descido abaixo do nívelmáximo de paralelismo 
// liberta umas das threads bloqueadas na operação cl_up. Caso o número de unidades de trabalho chegue a
// 0 desbloqueia todas as threads bloqueadas na operação cl_wait_all.
void cl_down(count_latch_t *latch); // desbloqueante

void countlatch_cleanup(count_latch_t * cdl);
