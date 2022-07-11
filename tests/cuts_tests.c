#include "../include/cuts.h"

CUTS_CREATE_TEST(MySuite, First,
{
	CUTS_ASSERT(0);
})

int main(void)
{
	CUTS_ADD_TEST(MySuite, First);
	return RUN_ALL_TESTS();
}
