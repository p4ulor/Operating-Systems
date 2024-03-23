#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <fcntl.h>


#define DATA_SIZE 32*1024*1024
#define D_SIZE 8*1024*1024

char info[DATA_SIZE];
char data[DATA_SIZE] = {1};//Add one more element increase RSS
char newc[D_SIZE];

int main() {
    printf("PID: %u\n", getpid());
    printf("#1 (press ENTER to continue)"); getchar();
    // a
    /*Private_Dirty are the pages in the mapping that have been written by this process but not referenced by any other process.*/
    char s='s';
    for(int i=0;i<DATA_SIZE;i++){
        info[i]=s;
    }
    
    
    printf("#2 (press ENTER to continue)");  getchar();
    // b
    /*Private_Clean are the pages in the mapping that have been read and not written by this process but not referenced by any other process;*/
    
    for(int i=0;i<DATA_SIZE;i++){
        char c;
        c=info[i];
    }
   
    printf("#3 (press ENTER to continue)"); getchar();
    void *create = mmap(NULL, D_SIZE, PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_PRIVATE, -1, 0); //Cria uma nova regiao de dados no espaço de endereçamento a baixo do heap com 8MB
    // c Aumento do size com a variavel newC
    
    printf("#4 (press ENTER to continue)"); getchar();// nao percebemos como se aumenta o rss
    char* c= (char * )create;
    for (size_t i = 0; i < D_SIZE; i++)
    {
        c[i]='s';
    }
    
    // d
    munmap(create,D_SIZE);
    printf("#5 (press ENTER to continue)"); getchar();
    pid_t child;
    child = fork();
    char *base=NULL;
    if(child == 0){  // fork succeeded DADOS
        int fd = open("cd",O_RDWR);
        base = mmap(NULL, D_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	    if (base == (void*) -1) {
		    close(fd);
		    return -1;
	    }
        close(fd);
        printf("child");
        for (size_t i = 0; i < D_SIZE; i++)
        {
            char x = base[i];
        }
        munmap(base,D_SIZE);
        exit(0); 
    }
    else 
    {    //CODIGO carregamento de biliotecas dllopen
        void *base2 = mmap(NULL, D_SIZE, PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_PRIVATE, -1, 0);
        for(int i=0;i<D_SIZE;i++){
            void * c = base2;
        }
        int returnStatus;    
        waitpid(child, &returnStatus, 0);
        if (returnStatus == 0) {
        printf("The child process terminated success.");    
        }
        if (returnStatus == 1)      {
        printf("The child process terminated with an error!.");    
        }
    }
    //Criar duas novas regioes
    // e
    
    printf("#6 (press ENTER to continue)"); getchar();

    // f
        for (size_t i = 0; i < 1; i++)
        {
            
        }
        munmap(base,D_SIZE);

    printf("#7 (press ENTER to continue)"); getchar();//NAO ESTA IMPLEMENTADA

    // g

    printf("END (press ENTER to continue)"); getchar();

    return 0;
}

