#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/mman.h> //for mmap
#include <string.h> //for memcpy
#include <stdlib.h> //for malloc
#include <dlfcn.h> //for dlopen

/**
less /proc/1234/maps - regioes de mem
less /proc/1234/smaps - regioes de mem e detelhae de cada regiao
*/

#define DATA_SIZE 16*1024*1024 //16777216 Bytes = 16 MB

char info[DATA_SIZE]; //.bss
char data[DATA_SIZE] = {1}; // .data -> all data starts w/ 1

//Aux functions
void writeBytes(char array[], size_t size, int value) {
	for (int i = 0; i < size; i++) {
		array[i] = value;
	}
}

void writeToStartOfPages(char* data, int numberOfWrites, int val, int pageSize) {
    int writes = 0;
    for(size_t i=0; i < DATA_SIZE && writes < numberOfWrites; writes++) {
        data[i] = val;
        i += pageSize;
    }
}

int main() {
	printf("PID: %u\n", getpid());

	const long PAGE_SIZE = sysconf(_SC_PAGE_SIZE); //gets the OS's page size
	printf("PAGE_SIZE: %ld\n", PAGE_SIZE);

	printf("#1 (press ENTER to continue)"); getchar();
	// a) Increase the resident set (Rss, memory occupied by memory that is held in RAM) by about 2MB in the .bss region (contains statically allocated variables that are declared but have not been initialized). 
    size_t _2megaBytes = 2*1024*1024;
    writeBytes(info, _2megaBytes, 1);
    printf("&info= %p\n", &info);
	
	printf("#2 (press ENTER to continue)"); getchar();
	// b) Access 128 bytes of initialized data (.data) with maximum impact in Private Clean pages
    writeToStartOfPages(data, 128, 'a', PAGE_SIZE);
    printf("&data= %p\n", &data);

    int writes = 0;
    char wroteReticencias = false;
    for(int i = 0; i < DATA_SIZE && writes < 128; i = i + PAGE_SIZE, writes++){
        if(writes<4 || writes>125) 
            printf("%d, at %d | ", data[i], i);
        else if(!wroteReticencias) {
            printf("(...) ");
            wroteReticencias = true;
        }
    }
    printf("\n");
	
	printf("#3 (press ENTER to continue)"); getchar();
	// c) Reduce the Pss (Proportional Set Size, contains memory used in other processes) of non-initialized data (.bss) to around 1MB for 30 seconds, while keeping Rss (Resident Set Size)
    /*
    Pss = private set size + (shared set size)/(num of sharing)

    The Rss contains 2MB, to turn Pss to 1MB
    */
    int child1 = fork();
    if(child1==0){
        sleep(30);
        return 0;
    }

	printf("#4 (press ENTER to continue)"); getchar();
	// d) Add about 512KB of data region without increasing the total Rss for data.
    void *mapStart = mmap(
        NULL, //OS will choose the starting address for the mapping
        512 * 1024, //512 KB
        PROT_WRITE, //the mapped memory can be written
        MAP_PRIVATE | MAP_ANONYMOUS, 
        -1, //the mapping is not associated with a file
        0 //offset within the file 
    );
    printf("memory mapped at %p\n", mapStart);
	printf("#5 (press ENTER to continue)"); getchar();
    //munmap(mapStart, 512 * 1024);

	// e) Increase the Rss of the new data region by around 256KB
    int _256kb = 256 * 1024;
    memset(mapStart, 0, _256kb);

	printf("#6 (press ENTER to continue)"); getchar();
	// f) Create two new regions in the address space, one for code (~ 4KB) and one for data (~ 256KB).
    // gcc -shared -fPIC lib.c -o libtvs.so
    void* libex = dlopen("./libtvs.so", RTLD_LOCAL | RTLD_NOW);
	
	printf("#7 (press ENTER to continue)"); getchar();
	// g) Increase Private dirty pages by about 128KB in the region for the data section created in f)
    int (*f)() = dlsym(libex, "fun");
    char* r = dlsym(libex, "random");
    printf("r = %p \n", r);
    printf("f = %p \n", f);
    memset(r, 1, 128*1024);
    f();

    /**
     * smaps for process on .data section libtvs.so:
     * - 7fe32ea55000-7fe32ea96000 rw-p 00003000 103:06 12058735
     * 
     * Program Stdout:
     * r = 0x7fe32ea55060 
     * f = 0x7fe32ea52159
     * 
     * Lib stdout:
     * a
     * 0x7fe32e846230 (out of range of .data section)
    */

	printf("END (press ENTER to continue)"); getchar();

	return 0;
}
