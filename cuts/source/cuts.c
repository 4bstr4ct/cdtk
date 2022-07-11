#include "../include/cuts.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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
