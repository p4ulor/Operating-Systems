// 	compile 
//			$ gcc prog.c xgetpid.s xopen.s xwrite.s xclose.s
//	run 	
//			$ ./a.out test.txt
//	Then you enter the number of elements in group
//			$ 3
//	Then you enter the number and name
//			$ 44829 - Delcio Augusto
//			$ 44806 - Raul Santos
//			$ 44873 - Paulo Rosa
#include <stdio.h>
#include <fcntl.h>

#define DATA_SIZE 1000

int xgetpid();
int xopen(const char *pathname, int flags, mode_t mode);
int xwrite(int fd, const void *buf, size_t count);
int xclose(int fd);
int main(int argc, char * argv[]){	
	int number;
	char tempbuf[DATA_SIZE];
	int fd = xopen(argv[1], O_RDWR | O_CREAT | O_APPEND| O_RDWR, S_IRUSR | S_IWUSR);
	if (fd != -1) {
		int i = xgetpid();
		int bytes = sprintf(tempbuf, "%d", i);
		if (bytes > 0){
			xwrite(fd, tempbuf, bytes);
		}
	printf("Enter number of students: \n");  
    scanf("%d", &number);
    for(int i =0 ; i<=number;i++){
		fgets(tempbuf, DATA_SIZE, stdin);
		int bytes = sprintf(tempbuf, "%s", tempbuf);
		if (bytes > 0){
			xwrite(fd, tempbuf, bytes);
		}
	}
	xclose(fd);
	}
}
