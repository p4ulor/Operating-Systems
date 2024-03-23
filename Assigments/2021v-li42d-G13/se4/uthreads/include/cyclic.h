/////////////////////////////////////////////////////////////////
//
// CCISEL 
// 2007-2021
//
// UThread library:
//   User threads supporting cooperative multithreading.
//
// Authors:
//   Carlos Martins, Jo�o Trindade, Duarte Nunes, Jorge Martins
// 

#pragma once

#include "uthread.h"
#include "list.h"

//
// A single-shot broadcast event. Threads are kept blocked in wait
// until set is called.
//
 
typedef struct event {
	bool signaled;
	list_entry_t waiters;
} event_t;
 
void event_init(event_t * event);// Garantir que a flag está false
 
void event_wait(event_t * event);// colocar flag true
 
void event_set(event_t * event);// verificar se a flag está a true

INLINE bool event_value(event_t * event) {
	return event->signaled; 
}


typedef struct cyclicbarrier {
	list_entry_t waiters;
	int parties;
	int idx;
} cyclicbarrier_t;

void cyclic_init(cyclicbarrier_t * c,int idx, int p);

void cyclic_wait(cyclicbarrier_t * c,int idx);
