#include <string.h>
#include <stdio.h>

char random[256 *1024] = {1};

int fun(){
    
    printf("a\n");
    printf("%p\n", random);
    getchar();
    //random[0] = 6; //Segmentation fault (core dumped)
    //printf("%d\n", random[0]);
   
    return 1;
}
