////////////////////////////////////////////
//
// ISEL - System Virtualization Techniques
// Autumn/Winter 2023/24
//
// Coursework Assignment #1
//

#include "chuta.h"
#include "ftests.h"

///////////////////////////////
// 
// DECLARE ALL FUNCTION TESTS
//
test_function all_function_tests[] = {
	test_add,
	test_sub,
	test_strsize,
};

//////////////////////////////
// 
// DECLARE ALL PROGRAM TESTS
//
prog_test_spec all_program_tests[] = {
	{"test-cat", {"cat"}, 
        "test_files/testFile.txt", 
        "test_files/1_catExpected.txt"},

    {"test_grep", {"grep", "zzz"}, 
        "test_files/testFile.txt", 
        "test_files/2_grepExpected.txt"},

    {"test_sort", {"sort"}, 
        "test_files/testFile.txt", 
        "test_files/3_sortExpected.txt"},

    {"test_wc", {"wc"}, 
        "test_files/testFile.txt", 
        "test_files/4_wcExpected.txt"}
};

////////////////////////
// 
// RUN ALL TESTS
//
// (do not modify, except to add the call to run_stdio_program_tests)
//

#define ARRLEN(arr) (sizeof(arr)/sizeof(arr[0]))

const size_t num_function_tests = ARRLEN(all_function_tests);
const size_t num_program_tests = ARRLEN(all_program_tests);

int main() {
	printf("---Running run_function_tests() ---\n");
	//run_function_tests(all_function_tests, num_function_tests, false);
    
    printf("---Running run_stdio_program_tests() ---\n");
	run_stdio_program_tests(all_program_tests, num_program_tests, false);
	
	return 0;
}
