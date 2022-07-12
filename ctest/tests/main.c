#include "../include/ctest.h"

CTEST_CREATE_TEST(MySuite, First,
{
	CTEST_ASSERT(0);
})

int main(void)
{
	CTEST_ADD_TEST(MySuite, First);
	return CTEST_RUN_ALL_TESTS();
}
