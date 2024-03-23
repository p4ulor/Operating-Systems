#include <stdio.h>
#include <stdlib.h> //for atoi

int main(int argc, char * argv[]) {
    int base = atoi(argv[1]); // converte string com dígitos para inteiro
    for (int i = 0; i < 4; ++i) 
        printf("%d\n", base + i);
    return 0;
}

// a. [2] Indique, justificando, o que é afixado no standard output e qual o conteúdo dos ficheiros output*.txt, com a execução de cada uma das seguintes linhas de comandos pelo shell:
// ./ex1.out 18 | grep 1 > output1.txt
// ./ex2.out 77 | tee output2.txt | grep 8

/**
 * Nothing will be printed to stdout
 * 1st command - executes ex1.out, which writes to file descriptor 1(stdout) "18, 19, 20, 21" (each in a line), BUT this output is redirected via pipe '|' to the input of the 'grep' command w/ the param '1', which returns a filtered list of the input provided containing only lines that contain the character '1', and this result output is redirected to the file 'outpu1.txt'. Thus the contents of the 'output1.txt' will be: 18, 19, 21 (each in a line)
 * 2nd command - executes ex2.out, which writes to stdout "77, 78, 79, 80" (each in a line). But this output is redirected via pipe '|' to the input of the 'tee' command, which writes to the 'output2.txt' "77, 78, 79, 80" (each in a line) and outputs to stdout these values, but another pipe redirects this output to the input of 'grep' with the param '8', which returns only lines with '8' in it, thus printing "78, 80" (each in a line)
*/

/** What is the 'tee' command?
 * The tee command, used with a pipe, reads standard input, then writes the output of a program to standard output and simultaneously copies it into the specified file or files. Use the tee command to view your output immediately and at the same time, store it for future use.
*/