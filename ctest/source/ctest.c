#include "../include/ctest.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifndef STRING_CAPACITY
#	define STRING_CAPACITY 128
#endif

#ifndef TESTS_CACHE_CAPACITY
#	define TESTS_CACHE_CAPACITY 128
#endif

struct _Test
{
	char suite[STRING_CAPACITY];
	char name[STRING_CAPACITY];
	int(*function)(void);
};

struct _TestsCache
{
	struct _Test tests[TESTS_CACHE_CAPACITY];
	unsigned int count;
	unsigned int passesCount;
	unsigned int failsCount;
};

static struct _TestsCache _testsCache =
{
	.count = 0,
	.passesCount = 0,
	.failsCount = 0
};

#ifndef CTEST_ECHO
#	define CTEST_ECHO(...) \
	{ \
		fprintf(__VA_ARGS__); \
	}
#endif

#ifndef CTEST_ECHO_DETAILS
#	define CTEST_ECHO_DETAILS(...) \
	{ \
		fprintf(stdout, CTEST_GREEN("[                ]")": "); \
		fprintf(stdout, __VA_ARGS__); \
		fprintf(stdout, "\n"); \
	}
#endif

#ifndef CTEST_GREEN
#	define CTEST_GREEN(string) "\033[1;32m"string"\033[0m"
#endif

#ifndef CTEST_YELLOW
#	define CTEST_YELLOW(string) "\033[1;33m"string"\033[0m"
#endif

#ifndef CTEST_RED
#	define CTEST_RED(string) "\033[1;31m"string"\033[0m"
#endif

void _addCTest(const char* const suite, const char* const name, int(*function)(void))
{
	if (_testsCache.count >= TESTS_CACHE_CAPACITY)
	{
		CTEST_ECHO(stderr, CTEST_RED("[ INTERNAL ERROR ]: Reached tests limits! Max test count is %d\n"), TESTS_CACHE_CAPACITY);
		exit(1);
	}
	else
	{
		unsigned long long length = strlen(suite);

		if (length >= STRING_CAPACITY)
		{
			CTEST_ECHO(stderr, CTEST_RED("[ INTERNAL ERROR ]: Test suite is too long! Max test suite length is %d\n"), STRING_CAPACITY);
			exit(1);
		}

		memcpy(_testsCache.tests[_testsCache.count].suite, suite, length);
		_testsCache.tests[_testsCache.count].suite[length] = '\0';

		length = strlen(name);

		if (length >= STRING_CAPACITY)
		{
			CTEST_ECHO(stderr, CTEST_RED("[ INTERNAL ERROR ]: Test name is too long! Max test name length is %d\n"), STRING_CAPACITY);
			exit(1);
		}

		memcpy(_testsCache.tests[_testsCache.count].name, name, length);
		_testsCache.tests[_testsCache.count].name[length] = '\0';

		_testsCache.tests[_testsCache.count].function = function;
		++_testsCache.count;
	}
}

int _runAllCTestsInTestsCache()
{
	CTEST_ECHO(stdout, CTEST_GREEN("[================]")"\n");
	CTEST_ECHO(stdout, CTEST_GREEN("[    RUN  ALL    ]")"\n");

	for (unsigned int index = 0; index < _testsCache.count; ++index)
	{
		CTEST_ECHO(stdout, CTEST_GREEN("[----------------]")"\n");
		const char* suite = _testsCache.tests[index].suite;
		const char* name = _testsCache.tests[index].name;
		int result =_testsCache.tests[index].function();
		CTEST_ECHO(stdout, CTEST_GREEN("[ RUN            ]")": %s.%s\n", suite, name);

		if (result)
		{
			CTEST_ECHO(stdout, CTEST_GREEN("[         PASSED ]: %s.%s")"\n", suite, name);
			++_testsCache.passesCount;
		}
		else
		{
			CTEST_ECHO(stdout, CTEST_RED("[         FAILED ]: %s.%s")"\n", suite, name);
			++_testsCache.failsCount;
		}
	}

	if (_testsCache.failsCount <= 0)
	{
		CTEST_ECHO(stdout, CTEST_GREEN("[================]")"\n");
		CTEST_ECHO(stdout, CTEST_GREEN("[ TESTS RESULTS  ]: all %d tests have passed")"\n", _testsCache.passesCount);
		CTEST_ECHO(stdout, CTEST_GREEN("[================]")"\n");
	}
	else
	{
		if (_testsCache.passesCount > 0)
		{
			CTEST_ECHO(stdout, CTEST_YELLOW("[================]")"\n");
			CTEST_ECHO(stdout, CTEST_YELLOW("[ TESTS RESULTS  ]: %d tests passed, %d failed")"\n", _testsCache.passesCount, _testsCache.failsCount);
			CTEST_ECHO(stdout, CTEST_YELLOW("[================]")"\n");
		}
		else
		{
			CTEST_ECHO(stdout, CTEST_RED("[================]")"\n");
			CTEST_ECHO(stdout, CTEST_RED("[ TESTS RESULTS  ]: all %d tests have failed\n"), _testsCache.failsCount);
			CTEST_ECHO(stdout, CTEST_RED("[================]")"\n");
		}
	}

	return _testsCache.passesCount <= 0;
}
