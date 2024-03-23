////////////////////////////////////////////
//
// ISEL - System Virtualization Techniques
// Autumn/Winter 2023/24
//
// Coursework Assignment #1
//

#include <stdio.h> //standart I/O. Used for perror(), puts() & sprintf()
#include <unistd.h> //for pipe(), close(), contains miscellaneous/general symbolic constants, types & functions. Used for write()
#include <sys/types.h> //cuz of pid_t
#include <sys/wait.h> //cuz of waitpid(), wait() https://man7.org/linux/man-pages/man2/wait.2.html
#include <bits/sigaction.h> //cuz of struct sigaction

#include "chuta.h"

void run_function_tests(test_function tests[], size_t num_tests, bool stop_at_first_failure) {
	unsigned int tests_ran = 0;
	unsigned int tests_successful = 0;
	unsigned int tests_failed = 0;

    for(int i = 0; i < num_tests; i++) {
        int pipefd[2];
        pipe(pipefd); //pipe[0]->read input, pipe[1]->write output

        pid_t pid = fork(); //pid==0 for the Child, pid==Child_PID for the Parent
	    if (pid == 0) {
            printf("Running child w/ PID=%d\n", getpid());

            close(pipefd[0]);
            dup2(pipefd[1], STDERR_FILENO); //copy child error output reference, to the pipe's output

            tests[i]();
			exit(EXIT_SUCCESS); //"If the test function returns, the test is considered successful and the exit status of the dedicated test process will be EXIT_SUCCESS"
            //child dies here
        }
        else if (pid == -1) puts("Out of memory O.O ?");
        else {
            int childReturnedBundledValues = 0;
			waitpid(pid, &childReturnedBundledValues, 0); //Wait for a child matching PID to die
            int childReturnedValue = WEXITSTATUS(childReturnedBundledValues); //extract returned value

            if(WIFEXITED(childReturnedBundledValues)) { //"For each test function, run_function_tests prints a line to standard output with the number of the test ..."
				if(childReturnedValue == EXIT_SUCCESS) {
					printf("Test %d -> EXIT_SUCCESS", i);
					tests_successful++;
				} else if(childReturnedValue == EXIT_FAILURE) {
					printf("Test %d -> EXIT_FAILURE. ", i);
					tests_failed++;

                    close(pipefd[1]);
                    char msg[READ_PIPE_MAX_LENGHT];
                    read(pipefd[0], msg, READ_PIPE_MAX_LENGHT); //blocking
                    printf("Child sent error msg %s", msg);
				} else {
					printf("Test %d -> Unknown status", i);
					tests_failed++;
				}

                printf(" (childReturnedValue = %d)\n", childReturnedValue);

            } else if(WIFSIGNALED(childReturnedBundledValues)!=0) { //true if the child process terminated because it received a signal that was not handled (ended abnormally)
				printf("Test %d: Terminated signal %d\n", i, WTERMSIG(childReturnedBundledValues)); //true if signal == terminated process signal
				tests_failed++;
			} else {
				printf("Test %d: Unknown status2\n", i);
				tests_failed++;
			}
            tests_ran++;
        }

        if(stop_at_first_failure && tests_failed==1)
            break;
    }

    printf(
    "Tests ran -> %d\n"
	"Tests Successful -> %d\n"
	"Tests Failed -> %d\n",
	tests_ran, tests_successful, tests_failed);
}

void run_stdio_program_tests(prog_test_spec tests[], size_t num_tests, bool stop_at_first_failure){
    unsigned int tests_ran = 0;
	unsigned int tests_successful = 0;
	unsigned int tests_failed = 0;

    for(int i = 0; i < num_tests; i++) {

        int pipefd[2];
        pipe(pipefd); //pipe[0]->read input, pipe[1]->write output

        pid_t pid = fork(); //pid==0 for the Child, pid==Child_PID for the Parent
        if (pid == 0) {
            printf("Running child w/ PID=%d\n", getpid());

            close(pipefd[0]);
            dup2(pipefd[1], STDERR_FILENO); //copy child error output reference, to the pipe's output
            dup2(pipefd[1], STDOUT_FILENO); //copy child output reference, to the pipe's output

            prog_test_spec t = tests[i];
            if (t.cmdAndArgs[1] != NULL) {
                execlp(t.cmdAndArgs[0], t.testName, t.cmdAndArgs[1], t.inputFile, NULL);
            } else {
                execlp(t.cmdAndArgs[0], t.testName, t.inputFile, NULL);
            }
        }
        else if (pid == -1) puts("Out of memory O.O ?");
        else { //Parent
            int childReturnedBundledValues = 0;
            waitpid(pid, &childReturnedBundledValues, 0); //Wait for a child matching PID to die
            int childReturnedValue = WEXITSTATUS(childReturnedBundledValues); //extract returned value

            if(WIFEXITED(childReturnedBundledValues)) { //"For each test function, run_function_tests prints a line to standard output with the number of the test ..."
                if(childReturnedValue == EXIT_SUCCESS || childReturnedValue == EXIT_FAILURE) {
                    printf("Test %d (%s) -> EXIT_SUCCESS | EXIT_FAILURE, lets compare the output & the expected file\n", i, tests[i].testName);
                    
                    close(pipefd[1]);
                    char pipeMsg[READ_PIPE_MAX_LENGHT];
                    read(pipefd[0], pipeMsg, READ_PIPE_MAX_LENGHT); //blocking
                    pipeMsg[READ_PIPE_MAX_LENGHT-1] = '\0';
                    
                    printf("Parent read pipe: %s\n", pipeMsg);
                    
                    //read expected file
                    FILE *file = fopen(tests[i].expectedOutputFile, "r");
                    char fileContent[READ_FILE_MAX_LENGHT];
                    fileContent[READ_FILE_MAX_LENGHT-1] = '\0';
                    //size_t bytesRead = fgets(fileContent, READ_FILE_MAX_LENGHT, file); //alt
                    size_t bytesRead = fread(fileContent, 1, READ_FILE_MAX_LENGHT, file);
                    if (bytesRead == 0) {
                        if (feof(file)) {
                            break;
                        } else {
                            perror("Error reading file");
                            fclose(file);
                            tests_failed++;
                        }
                    } else {
                        char errorMsg[READ_FILE_MAX_LENGHT];
                        char* p_pipeMsg = pipeMsg;
                        char* p_fileContent = fileContent;
                        bool failed = false;
                        int fileCharIndex = 0;
                        
                        while(*p_pipeMsg!='\0' && *p_fileContent!='\0'){
                            if(*p_pipeMsg != *p_fileContent){
                                tests_failed++;
                                failed = true;
                                sprintf(errorMsg, "Test %d -> Error: results diff -> pipe=%c, file=%c at index %d\n", i, *p_pipeMsg, *p_fileContent, fileCharIndex);
                                printf("%s", errorMsg);
                                break;
                            }
                            p_pipeMsg++;
                            p_fileContent++;
                            fileCharIndex++;
                        }

                        if(!failed){
                            tests_successful++;
                            printf("Test %d -> SUCCESS\n", i);
                        }
                        
                        fclose(file);
                    }

                } else {
                    printf("Test %d -> Unknown status", i);
                    tests_failed++;
                }

                printf(" (childReturnedValue = %d)\n-----------------------------\n", childReturnedValue);

            } else if(WIFSIGNALED(childReturnedBundledValues)!=0) { //true if if the child process terminated because it received a signal that was not handled (ended abnormally)
                printf("Test %d: Terminated signal %d\n", i, WTERMSIG(childReturnedBundledValues)); //true if signal == terminated process signal
                tests_failed++;
            } else {
                printf("Test %d: Unknown status2\n", i);
                tests_failed++;
            }
            tests_ran++;
        }
    }

    printf(
    "Tests ran -> %d\n"
	"Tests Successful -> %d\n"
	"Tests Failed -> %d\n",
	tests_ran, tests_successful, tests_failed); 
}
