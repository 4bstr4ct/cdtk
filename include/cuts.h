#ifndef _CUTS_H_
#define _CUTS_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * Function `fprintf()` wrapper.
 */
#ifndef CUTS_ECHO
#	define CUTS_ECHO(...) \
	{ \
		fprintf(__VA_ARGS__); \
	}
#endif

/**
 * Special echo macro for printing details of the test.
 */
#ifndef CUTS_ECHO_DETAILS
#	define CUTS_ECHO_DETAILS(...) \
	{ \
		fprintf(stdout, CUTS_GREEN("[                ]")": "); \
		fprintf(stdout, __VA_ARGS__); \
		fprintf(stdout, "\n"); \
	}
#endif

/**
 * Macro to color given string green.
 */
#ifndef CUTS_GREEN
#	define CUTS_GREEN(string) "\033[1;32m"string"\033[0m"
#endif

/**
 * Macro to color given string yellow.
 */
#ifndef CUTS_YELLOW
#	define CUTS_YELLOW(string) "\033[1;33m"string"\033[0m"
#endif

/**
 * Macro to color given string red.
 */
#ifndef CUTS_RED
#	define CUTS_RED(string) "\033[1;31m"string"\033[0m"
#endif

/**
 * @addtogroup DEFINES
 * 
 * @{
 */

#ifndef CUTS_MAX_STRING_LENGTH
#	define CUTS_MAX_STRING_LENGTH 128
#endif

#ifndef CUTS_MAX_TESTS_COUNT
#	define CUTS_MAX_TESTS_COUNT 128
#endif

/**
 * A test wrapper that holds test's suite, name, and
 * pointer to its function.
 */
struct _CUTS_Test
{
	char suite[CUTS_MAX_STRING_LENGTH];
	char name[CUTS_MAX_STRING_LENGTH];
	int(*function)(void);
};

/**
 * A wrapper for tests container. It holds an array of
 * tests, tracks their count, passes and fails.
 */
struct _CUTS_Cache
{
	struct _CUTS_Test tests[CUTS_MAX_TESTS_COUNT];
	unsigned int count;
	unsigned int passesCount;
	unsigned int failsCount;
};

/**
 * @}
 */

/**
 * @addtogroup TESTS_GENERATORS
 * 
 * @{
 */

/**
 * Macro that generates a function for a test with
 * provided suite, name and body.
 */
#ifndef CUTS_CREATE_TEST
#	define CUTS_CREATE_TEST(suite, name, body) \
	int _CUTS_Test_##suite##_##name(void) \
	{ \
		body; \
	}
#endif

/**
 * Adds test to the cache.
 */
void _CUTS_addTest(const char* const suite, const char* const name, int(*function)(void));

/**
 * A macro wrapper for @ref _CUTS_addTest function.
 */
#ifndef CUTS_ADD_TEST
#	define CUTS_ADD_TEST(suite, name) _CUTS_addTest(#suite, #name, _CUTS_Test_##suite##_##name)
#endif

/**
 * Runs all tests and outputs results.
 */
int _CUTS_runAllTests();

/**
 * A macro wrapper for @ref _CUTS_runAllTests function.
 */
#ifndef RUN_ALL_TESTS
#	define RUN_ALL_TESTS() \
	( \
		_CUTS_runAllTests() \
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
#ifndef CUTS_ASSERT
#	define CUTS_ASSERT(expression) return (expression)
#endif

/**
 * Soft, non-terminating assertion for expression to be
 * true.
 */
#ifndef CUTS_ASSERT_TRUE
#	define CUTS_ASSERT_TRUE(expression) return ((expression) != 0)
#endif

/**
 * Soft, non-terminating assertion for expression to be
 * false.
 */
#ifndef CUTS_ASSERT_FALSE
#	define CUTS_ASSERT_FALSE(expression) return ((expression) == 0)
#endif

/**
 * Soft, non-terminating assertion for expression a to be
 * equal to the expression b.
 */
#ifndef CUTS_ASSERT_EQUAL
#	define CUTS_ASSERT_EQUAL(a, b) return ((a) == (b))
#endif

/**
 * Soft, non-terminating assertion for expression a to not
 * be equal to the expression b.
 */
#ifndef CUTS_ASSERT_NEQUAL
#	define CUTS_ASSERT_NEQUAL(a, b) return ((a) == (b))
#endif

/**
 * @}
 */



static struct _CUTS_Cache _cache =
{
	.count = 0,
	.passesCount = 0,
	.failsCount = 0
};

void _CUTS_addTest(const char* const suite, const char* const name, int(*function)(void))
{
	if (_cache.count >= CUTS_MAX_TESTS_COUNT)
	{
		CUTS_ECHO(stderr, CUTS_RED("[ INTERNAL ERROR ]: Reached tests limits! Max test count is %d\n"), CUTS_MAX_TESTS_COUNT);
		exit(1);
	}
	else
	{
		unsigned long long length = strlen(suite);

		if (length >= CUTS_MAX_STRING_LENGTH)
		{
			CUTS_ECHO(stderr, CUTS_RED("[ INTERNAL ERROR ]: Test suite is too long! Max test suite length is %d\n"), CUTS_MAX_STRING_LENGTH);
			exit(1);
		}

		memcpy(_cache.tests[_cache.count].suite, suite, length);
		_cache.tests[_cache.count].suite[length] = '\0';

		length = strlen(name);

		if (length >= CUTS_MAX_STRING_LENGTH)
		{
			CUTS_ECHO(stderr, CUTS_RED("[ INTERNAL ERROR ]: Test name is too long! Max test name length is %d\n"), CUTS_MAX_STRING_LENGTH);
			exit(1);
		}

		memcpy(_cache.tests[_cache.count].name, name, length);
		_cache.tests[_cache.count].name[length] = '\0';

		_cache.tests[_cache.count].function = function;
		++_cache.count;
	}
}

int _CUTS_runAllTests()
{
	CUTS_ECHO(stdout, CUTS_GREEN("[================]")"\n");
	CUTS_ECHO(stdout, CUTS_GREEN("[    RUN  ALL    ]")"\n");

	for (unsigned int index = 0; index < _cache.count; ++index)
	{
		CUTS_ECHO(stdout, CUTS_GREEN("[----------------]")"\n");
		const char* suite = _cache.tests[index].suite;
		const char* name = _cache.tests[index].name;
		int result =_cache.tests[index].function();
		CUTS_ECHO(stdout, CUTS_GREEN("[ RUN            ]")": %s.%s\n", suite, name);

		if (result)
		{
			CUTS_ECHO(stdout, CUTS_GREEN("[         PASSED ]: %s.%s")"\n", suite, name);
			++_cache.passesCount;
		}
		else
		{
			CUTS_ECHO(stdout, CUTS_RED("[         FAILED ]: %s.%s")"\n", suite, name);
			++_cache.failsCount;
		}
	}

	if (_cache.failsCount <= 0)
	{
		CUTS_ECHO(stdout, CUTS_GREEN("[================]")"\n");
		CUTS_ECHO(stdout, CUTS_GREEN("[ TESTS RESULTS  ]: all %d tests have passed")"\n", _cache.passesCount);
		CUTS_ECHO(stdout, CUTS_GREEN("[================]")"\n");
	}
	else
	{
		if (_cache.passesCount > 0)
		{
			CUTS_ECHO(stdout, CUTS_YELLOW("[================]")"\n");
			CUTS_ECHO(stdout, CUTS_YELLOW("[ TESTS RESULTS  ]: %d tests passed, %d failed")"\n", _cache.passesCount, _cache.failsCount);
			CUTS_ECHO(stdout, CUTS_YELLOW("[================]")"\n");
		}
		else
		{
			CUTS_ECHO(stdout, CUTS_RED("[================]")"\n");
			CUTS_ECHO(stdout, CUTS_RED("[ TESTS RESULTS  ]: all %d tests have failed\n"), _cache.failsCount);
			CUTS_ECHO(stdout, CUTS_RED("[================]")"\n");
		}
	}

	return _cache.passesCount <= 0;
}

#endif
