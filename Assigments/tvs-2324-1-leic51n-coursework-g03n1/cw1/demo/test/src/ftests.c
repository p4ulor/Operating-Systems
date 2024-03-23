////////////////////////////////////////////
//
// ISEL - System Virtualization Techniques
// Autumn/Winter 2023/24
//
// Coursework Assignment #1
//

#include "ftests.h"

#include "chuta.h"
#include "somecode.h"

void test_add() {
	int res = add(2, 2);
	CH_ASSERT(res == 4);
}

void test_sub() {
	int res = sub(5, 2);
	CH_ASSERT_MSG(res == 3, "res==3 failed");
}

void test_strsize() {
	size_t size = strsize("ISEL");
	CH_ASSERT(size == 4);
}
