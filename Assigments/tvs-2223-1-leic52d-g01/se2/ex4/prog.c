#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>

#define DATA_SIZE 3*1024*1024

char info[DATA_SIZE];
char data[DATA_SIZE] = {1};

int sum(char array[], size_t size) {
	int res = 0;
	for (int i = 0; i < size; i++) {
		res += array[i];
	}
	return res;
}

void fill(char array[], size_t size, int value) {
	for (int i = 0; i < size; i++) {
		array[i] = value;
	}
}

void pssReduction(int num){
    sleep(30);
    printf("ended child nª %d\n",num);
}

void fill_data(char* data, int size, int val, int page) {
    for(int i=0; i < size; i += page) {
        data[i] = val;
    }
}

int main() {
	printf("PID: %u\n", getpid());

	const long PAGE_SIZE = sysconf(_SC_PAGE_SIZE);
	printf("PAGE_SIZE: %ld\n", PAGE_SIZE);

	printf("#1 (press ENTER to continue)"); getchar();

	// a
	fill(info, DATA_SIZE, 10);
    printf("&info= %p\n", &info);
    
	printf("#2 (press ENTER to continue)"); getchar();

	// b
	printf("&data= %p\n", &data);
    fill_data(data,256,'0',PAGE_SIZE);

	printf("#3 (press ENTER to continue)"); getchar();

    // c
    pid_t child1 = fork();

    if(child1 == 0){
        pid_t child2 = fork();
        if(child2 == 0){
            pssReduction(2);
            return 1;
        }
        else {
            pssReduction(1);
            return 1;
        }
    }

	printf("#4 (press ENTER to continue)"); getchar();
	// d
    void *mapbase = mmap(NULL, 96 * 1024, PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    printf("memory mapped at %p\n", mapbase);


	printf("#5 (press ENTER to continue)"); getchar();

    fill_data(mapbase,96*1024,1,PAGE_SIZE);
	// e

	printf("#6 (press ENTER to continue)"); getchar();
    munmap(mapbase,96*1024); //clean memory used
	// f

    void *mapCode = mmap(NULL, 96 * 1024, PROT_EXEC | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    void *mapData = mmap(NULL, 96 * 1024, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    printf("mem CODE mapped at %p\n", mapCode);
    printf("mem DATA mapped at %p\n", mapData);

    printf("#7 (press ENTER to continue)"); getchar();
    // g
    
    fill_data(mapData,96*1024,1,PAGE_SIZE);
    printf("mem DATA mapped at %p\n", mapData);


	/* OPCIONAL
	printf("#8 (press ENTER to continue)"); getchar();

	// h

	*/
	printf("END (press ENTER to continue)"); getchar();

	return 0;
}
