////////////////////////////////////////////
//
// ISEL - System Virtualization Techniques
// Autumn/Winter 2023/24
//
// Coursework Assignment #1
//

#ifndef CHUTA_H
#define CHUTA_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

//"If some assertion fails during the test, the dedicated process must exit immediately with EXIT_FAILURE"
#define CH_ASSERT(test_expression) \
	do { if (!(test_expression)) { exit(EXIT_FAILURE); } } while (0)

#define CH_ASSERT_MSG(test_expression, error_msg) \
	do { if (!(test_expression)) { fprintf(stderr, "Test -> %s() had error: '%s' in file '%s' at line %d", __func__, error_msg, __FILE__, __LINE__); exit(EXIT_FAILURE); } } while (0)

#define READ_FILE_MAX_LENGHT 1024
#define READ_PIPE_MAX_LENGHT 1024

typedef void (*test_function)();

typedef struct prog_test_spec{
    char* testName;
    char* cmdAndArgs[2]; 
    char* inputFile; 
    char* expectedOutputFile;
} prog_test_spec;

void run_function_tests(test_function tests[], size_t num_tests, bool stop_at_first_failure);

void run_stdio_program_tests(prog_test_spec tests[], size_t num_tests, bool stop_at_first_failure);

#endif // CHUTA_H
