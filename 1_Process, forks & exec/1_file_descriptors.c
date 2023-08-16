#include <stdio.h> //standart I/O. Used for perror(), puts() & sprintf()
#include <unistd.h> //contains miscellaneous/general symbolic constants, types & functions. Used for write()

/*  How to run
    $ gcc 1_file_descriptors.c (always produces a.out)
    (or)
    $ gcc -o 1_file_descriptors.out 1_file_descriptors.c (produces .out file w/ specified name)
    $ ./a.out
*/

int main() {
    //Part 1 - Execution time file descriptors
	write(1, "Part1\n", 6); //writes to the standart output w/ file descriptor = 1 a string with lenght of 6 bytes
	int res = write(3, "abc\n", 4);
	if (res == -1) perror("Part1 write failed"); //Will get here if you ran like $ ./a.out
	else puts("Part1 successful"); //Will get here if you ran like $ ./a.out 3> abc.txt

    //Part2 - Creating new file descritors (by duplicating them)
    write(1, "Part2\n", 6);
    char string[100];

    int fileDesc = dup(1);  // Duplicate the specified file descriptor and returning the value of the next available integer, pointing to the same file descp provided by the param
    printf("fileDesc: %d\n", fileDesc); //Should be = 3 if you did a plain run OR should be = 4 if you ran $ ./a.out 3> abc.txt
    sprintf(string, "Hi to stdout w/ FD = %d\n", fileDesc);
    int fileDescWrite = write(fileDesc, string, 100);
    if (fileDescWrite == -1) perror("write failed for fileDescWrite");
	else puts("write successful for fileDescWrite");

	int fileDesc2 = dup2(1, 10); //First, it closes fileDesc on param 2, then makes param 2 point to the same fileDesc as param 1
	printf("fileDesc2: %d\n", fileDesc2); //Should be = 10
    sprintf(string, "Hi to stdout w/ FD = %d\n", fileDesc2);
    int fileDescWrite2 = write(fileDesc2, string, 100);
	if (fileDescWrite2 == -1) perror("write failed for fileDescWrite2");
	else puts("write successful for fileDescWrite2");
	
	return 0;
}
