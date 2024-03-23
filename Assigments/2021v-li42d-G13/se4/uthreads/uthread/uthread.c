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

#include <stdint.h>
#include <stdlib.h>
#include <string.h>  // memset
#include <assert.h>
#include <stdio.h>

#include "uthread.h"
#include "list.h"

//////////////////////////////////////
//
// UThread types and constants.
//

//
// The data structure representing the layout of a thread's execution
// context when saved in the stack.
//
typedef struct uthread_context {
	uint64_t r15;
	uint64_t r14;
	uint64_t r13;
	uint64_t r12;
	uint64_t rbx;
	uint64_t rbp;
	void (*ret_addr)();
} uthread_context_t;

//
// The descriptor of a user thread, containing an intrusive link
// (through which the thread is linked in the ready queue), the
// thread's starting function and argument, the memory block used as
// the thread's stack and a pointer to the saved execution context.
//
enum uthread_state {
	RUNNING, //0
	READY, //1 espera do CPU
	BLOCKED //2 espera
};
typedef enum uthread_priority {
	IDLE, //0
	LOW, //1
	NORMAL, //2
	HIGH //3
} uthread_priority_t;

struct uthread {
	uthread_context_t * context;
	list_entry_t        entry;
	void             (* function)(void *);   
	void *              argument; 
	void *              stack;
	enum uthread_state state;
	enum uthread_priority priority;
};
static uthread_t* get_priority_thread(list_entry_t * list);

//
// The fixed stack size of a user thread.
//
#define STACK_SIZE (8 * 4096)

//////////////////////////////////////
//
// UThread internal state variables.
//

//
// The number of existing user threads.
//
static unsigned int number_of_threads;

//
// The sentinel of the circular list linking the user threads that are
// currently schedulable. The next thread to run is retrieved from the
// head of this list.
//
static list_entry_t ready_queue;
/*uthread_table ut_table; //NAO ESTA IMPLEMENTADA tabela auxiliar para guardar as threads em um estrutura onde contem um array de threads. 
						//Desta forma conseguiriamos comparar as threads com maior prioridade
void print_tinfo() {//NAO ESTA IMPLEMENTADA
    int i;
    uthread_t *cur;

    for (i = 0; i < MAX_UTHREADS; i++) {
        cur = ut_table.threads[i];
        if (cur != 0) {
            printf("priority=%d",
                        cur->priority);
        }
    }
	printf("\n");
}*/


//
// The currently executing thread.
//
__attribute__ ((visibility ("hidden")))
uthread_t * running_thread;

//
// The user thread proxy of the underlying operating system thread.
// This thread is switched back in when there are no more runnable
// user threads, causing the scheduler to exit.
//
static uthread_t * main_thread;


////////////////////////////////////////////////
//
// Forward declaration of internal operations.
//

//
// The trampoline function that a user thread begins by executing,
// through which the associated function is called.
//
static void internal_start();

//
// Performs a context switch from currThread to nextThread.
// In x64 calling convention currThread is in RDI and nextThread in RSI.
//
__attribute__((visibility ("hidden")))
void context_switch(uthread_t * currThread, uthread_t * nextThread);

//
// Frees the resources associated with currThread and switches to nextThread.
// In x64 calling convention currThread is in RDI and nextThread in RSI.
//
__attribute__ ((visibility ("hidden")))
void internal_exit(uthread_t * currThread, uthread_t * nextThread);

//
// Frees the resources associated with the terminated thread.
//
__attribute__ ((visibility ("hidden")))
void internal_cleanup(uthread_t * thread);


////////////////////////////////////////
//
// UThread inline internal operations.
//

//
// Returns and removes the first user thread in the ready queue. If the ready
// queue is empty, the main thread is returned.
// 
static inline uthread_t * extract_next_thread() {
	if (is_list_empty(&ready_queue))
	{
		main_thread->priority=IDLE;
		printf("main %d",main_thread->priority);
		return main_thread;
	}
	uthread_t * extract = get_priority_thread(&ready_queue);/*container_of(remove_list_first(&ready_queue),uthread_t,entry);*/
	printf("maior prioridade %d\n",extract->priority);
	remove_list_entry(&(extract->entry));
	return extract;
}

static uthread_t* get_priority_thread(list_entry_t * list)
{   
	/*printf("number in priority %d",list->e);
	uthread_t *extract = container_of(find_list(2,list),uthread_t,entry);*/
    
	// Find the node with the highest priority
	if (is_list_empty(list))
	{
		printf("list empty\n");
		return main_thread;
	}
	// Estamos a tentar fazer um clone para nao afetarmos diretamente a ready_queue
	// ver quantos elementos tem a lista list
	//list_entry_t * n1 = (list_entry_t *) malloc(sizeof (list_entry_t));
	list_entry_t * n1 = list;

	//duplicate a list
	uthread_t *priority_node = NULL;
	int highest_priority = 0;
	uthread_t *extract=NULL;
	while (!is_list_empty(n1))
	{
		extract =  container_of(remove_list_first(n1),uthread_t,entry);
		if (extract!=NULL)
		{
			if (extract->priority > highest_priority)
        	{
				priority_node = extract;
            	highest_priority = extract->priority;
				//printf("high %d\n",highest_priority);
			}
		}
	}
    return priority_node;
	//return extract;
}

//
// Schedule a new thread to run.
// Eleger uma thread forcar comutacao
static inline void schedule () {
	uthread_t * next_thread;
    next_thread = extract_next_thread();
	next_thread->state  = RUNNING;
	//printf("schedule %d \n", running_thread->state);
	context_switch(running_thread, next_thread);
	//ATENCAO
}

///////////////////////////////
//
// UThread public operations.
//

//
// Initialize the scheduler.
// This function must be the first to be called. 
//
void ut_init() {
	/*ut_table.cur = 0;
    ut_table.run_tid = -1;
    memset(ut_table.threads, 0, sizeof(ut_table.threads));*///NAO ESTA IMPLEMENTADA
	init_list_head(&ready_queue);
}

//
// Cleanup all UThread internal resources.
//
void ut_end() {
	/* (this function body was intentionally left empty) */
}

//
// Run the user threads. The operating system thread that calls this
// function performs a context switch to a user thread and resumes
// execution only when all user threads have exited.
//
void ut_run() {
	//print_tinfo();
	uthread_t thread; // Represents the underlying operating system thread.

	//
	// There can be only one scheduler instance running.
	//
	assert(running_thread == NULL);
	
	//
	// At least one user thread must have been created before calling run.
	//
	if (is_list_empty(&ready_queue)) {
		return;
	}

	//
	// Switch to a user thread.
	//
	main_thread = &thread;
	running_thread = main_thread;
	running_thread->state = READY;
	//printf("ut_run %d \n", running_thread->state);
	schedule();

	//
	// When we get here, there are no more runnable user threads.
	//
	assert(is_list_empty(&ready_queue));
	assert(number_of_threads == 0);

	//
	// Allow another call to ut_run().
	//
	running_thread = NULL;
	main_thread = NULL;
}

//
// Terminates the execution of the currently running thread. All
// associated resources are released after the context switch to
// the next ready thread.
//
void ut_exit() {
	number_of_threads -= 1;	
	ready_queue.e=number_of_threads;
	internal_exit(running_thread, extract_next_thread());
	
	assert(!"Supposed to be here!");
}

//
// Relinquishes the processor to the first user thread in the ready queue.
// If there are no ready threads, the function returns immediately.
//
void ut_yield() {
	if (!is_list_empty(&ready_queue)) {
		insert_list_last(&ready_queue, &running_thread->entry);
		running_thread->state  = READY;
		//printf("ut_yield %d \n", running_thread->state);
		schedule();
	}
}

//
// Returns a handle to the executing user thread.
//
uthread_t* ut_self() {
	return running_thread;
}

//
// Halts the execution of the current user thread.
//
void ut_deactivate() {// significa thread fez deactivate e ready_queue vazia 
					//cenário em que uma thread desativou a espera que outra venha acordar mas não existe outra porque n esta ready
	running_thread->state  = BLOCKED;
	//printf("ut_deactivate %d \n", running_thread->state);
	schedule();
}

//
// Places the specified user thread at the end of the ready queue,
// making it eligible to run.
//
void ut_activate(uthread_t * thread) {
	insert_list_last(&ready_queue, &(thread->entry));
	running_thread->state  = READY;
	ready_queue.e=number_of_threads;
	printf("number of threads %d\n",ready_queue.e);
}


///////////////////////////////////////
//
// Definition of internal operations.
//

//
// The trampoline function that a user thread begins by executing,
// through which the associated function is called.
//
void internal_start() {
	running_thread->function(running_thread->argument);
	ut_exit(); 
}

//
// Frees the resources associated with thread.
//
void internal_cleanup(uthread_t * thread) {
	free(thread->stack);
	free(thread);
}


//
// Creates a user thread to run the specified function. The thread is
// placed at the end of the ready queue.
//
uthread_t* ut_create(void (*start_routine)(void *), void * arg, uthread_priority_t prio) {
	uthread_t * thread;
	//
	// Dynamically allocate an instance of uthread_t and the associated stack.
	//
	thread = (uthread_t *) malloc(sizeof (uthread_t));
	thread->stack = malloc(STACK_SIZE);
	assert(thread != NULL && thread->stack != NULL);
	/*if (ut_table.cur == MAX_UTHREADS) {//NAO ESTA IMPLEMENTADA
        return NULL;
    }
	for (ut_id = 0; ut_id < MAX_UTHREADS; ut_id++) {
        if (ut_table.threads[ut_id] == 0)
            break;
    }
	ut_table.threads[ut_id] = malloc(sizeof(uthread_t));*/
	//
	// Zero the stack for emotional confort.
	//
	memset(thread->stack, 0, STACK_SIZE);

	//
	// Memorize function and argument for use in internal_start.
	//
	thread->function = start_routine;
	thread->argument = arg;
	thread->priority = prio;
		
	
	//
	// Map an uthread_context_t instance on the thread's stack.
	// We'll use it to save the initial context of the thread.
	//
	// +------------+  <- Highest word of a thread's stack space
	// | 0x00000000 |    (needs to be set to 0 for Visual Studio to
	// +------------+      correctly present a thread's call stack).  
	// +============+       
	// |  RetAddr   | \.   
	// +------------+  |
	// |    RBP     |  |
	// +------------+  |
	// |    RBX     |   >   thread->context mapped on the stack.
	// +------------+  |
	// |    RDI     |  |
	// +------------+  |
	// |    RSI     |  |
	// +------------+  |
	// |    R12     |  |
	// +------------+  |
	// |    R13     |  |
	// +------------+  |
	// |    R14     |  |
	// +------------+  |
	// |    R15     | /  <- The stack pointer will be set to this address
	// +============+       at the next context switch to this thread.
	// |            | \.
	// +------------+  |
	// |     :      |  |
	//       :          >   Remaining stack space.
	// |     :      |  |
	// +------------+  |
	// |            | /  <- Lowest word of a thread's stack space
	// +------------+       (thread->stack always points to this location).
	//

	thread->context = (uthread_context_t *) (((uint8_t *)thread->stack) +
		STACK_SIZE - sizeof (uthread_context_t) - sizeof (uint64_t));


	//
	// Set the thread's initial context by initializing the values of 
	// registers that must be saved by the callee (R15, R14, R13, R12,
	// RSI, RDI, RBCX, RBP)
	//
	// Upon the first context switch to this thread, after popping the
	// dummy values of the "saved" registers, a ret instruction will
	// place the address of internal_start on EIP.
	//
	thread->context->r15 = 0x55555555;
	thread->context->r14 = 0x44444444;
	thread->context->r13 = 0x33333333;
	thread->context->r12 = 0x22222222;	
	thread->context->rbx = 0x11111111;
	thread->context->rbp = 0x00000000;		
	thread->context->ret_addr = internal_start;
	//thread->state = RUNNING;//segmentation fault
	//printf("ut_create %d \n", running_thread->state);
	//
	// Ready the thread.
	//
	number_of_threads += 1;
	/*ut_table.cur=number_of_threads;//NAO ESTA IMPLEMENTADA
	ut_table.threads[ut_id] = thread;*/
	//printf("running_thread->state = %d \n", running_thread->state);
	ut_activate(thread);
	
	return thread;
}
