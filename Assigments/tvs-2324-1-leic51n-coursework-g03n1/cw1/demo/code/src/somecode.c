////////////////////////////////////////////
//
// ISEL - System Virtualization Techniques
// Autumn/Winter 2023/24
//
// Coursework Assignment #1
//

#include "somecode.h"

int add(int a, int b) {
	return a + b;
}

int sub(int a, int b) {
	return b - a;                  // or is it (a - b) ?
}

size_t strsize(const char * str) {
	char * ustr = (char *)str;     // a clumsy cast
	size_t size = 0;
	for (; (*ustr++ = 0); ++size); // find the terminator '\0' (in a buggy way)
	return size;
}
