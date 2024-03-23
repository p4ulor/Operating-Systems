#include "cyclic.h"
#include "waitblock.h"

typedef struct cyclicbarrier_waitblock {
	waitblock_t header;
	int units;
} cyclicbarrier_waitblock_t;


//
// Initializes the specified c wait block.
//
static void init_cyclicwaitblock(cyclicbarrier_waitblock_t * c_wblock, int units) {
	init_waitblock(&c_wblock->header);
	c_wblock->units = units;
}


//
// Initializes a c instance. units is the starting number of available units and 
// limit is the maximum number of units allowed for the specified c instance.
//
void cyclic_init(cyclicbarrier_t * c, int units, int limit) {
	init_list_head(&c->waiters);
	c->idx = units;
	c->parties = limit;
}

//
// Gets the specified number of units from the c. If there aren't enough units available,  
//
void cyclic_wait(cyclicbarrier_t * c, int units) {
	cyclicbarrier_waitblock_t waitblock;
	//
	// If there are enough units available, get them and keep running.
	//
	if (is_list_empty(&c->waiters) && c->idx >= units) {
		c->idx -= units;
		return;
	}
	//
	// There are no units available. Insert the running thread in the wait list.
	//
	init_cyclic_waitblock(&waitblock, units);   
	insert_list_last(&c->waiters, &waitblock.header.entry);
	//
	// Remove the current thread from the ready list.
	//
	ut_deactivate();
}
