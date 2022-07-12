#ifndef _CTEST_H_
#define _CTEST_H_

/**
 * @addtogroup TESTS_GENERATORS
 * 
 * @{
 */

/**
 * Macro that generates a function for a test with provided suite, name and
 * body.
 */
#ifndef CTEST_CREATE_TEST
#	define CTEST_CREATE_TEST(suite, name, body) \
	int _CTEST_Test_##suite##_##name(void) \
	{ \
		body; \
	}
#endif

/**
 * Adds test to the cache.
 */
void _addCTest(const char* const suite, const char* const name, int(*function)(void));

/**
 * A macro wrapper for @ref _addCTest function.
 */
#ifndef CTEST_ADD_TEST
#	define CTEST_ADD_TEST(suite, name) _addCTest(#suite, #name, _CTEST_Test_##suite##_##name)
#endif

/**
 * Runs all tests and outputs the result.
 */
int _runAllCTestsInTestsCache();

/**
 * A macro wrapper for @ref _runAllCTestsInTestsCache function.
 */
#ifndef CTEST_RUN_ALL_TESTS
#	define CTEST_RUN_ALL_TESTS() \
	( \
		_runAllCTestsInTestsCache() \
	)
#endif

/**
 * @}
 */

/**
 * @addtogroup TESTING_UTILITIES
 * 
 * @{
 */

/**
 * Soft, non-terminating assertion.
 */
#ifndef CTEST_ASSERT
#	define CTEST_ASSERT(expression) return (expression)
#endif

/**
 * Soft, non-terminating assertion for expression to be true.
 */
#ifndef CTEST_ASSERT_TRUE
#	define CTEST_ASSERT_TRUE(expression) return ((expression) != 0)
#endif

/**
 * Soft, non-terminating assertion for expression to be false.
 */
#ifndef CTEST_ASSERT_FALSE
#	define CTEST_ASSERT_FALSE(expression) return ((expression) == 0)
#endif

/**
 * Soft, non-terminating assertion for expression a to be equal to the
 * expression b.
 */
#ifndef CTEST_ASSERT_EQUAL
#	define CTEST_ASSERT_EQUAL(a, b) return ((a) == (b))
#endif

/**
 * Soft, non-terminating assertion for expression a to not be equal to the
 * expression b.
 */
#ifndef CTEST_ASSERT_NEQUAL
#	define CTEST_ASSERT_NEQUAL(a, b) return ((a) == (b))
#endif

/**
 * @}
 */

#endif
