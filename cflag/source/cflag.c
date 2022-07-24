#include "../include/cflag.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

enum _FlagType
{
	FLAG_BOOL,
	FLAG_INT64,
	FLAG_DOUBLE,
	FLAG_STRING
};

#ifndef STRING_FLAG_CAPACITY
#	define STRING_FLAG_CAPACITY 256
#endif

union _FlagValue
{
	bool b;
	int64 i64;
	double d;
	char s[STRING_FLAG_CAPACITY + 1];
};

struct _Flag
{
	const char* name;
	const char* usage;
	enum _FlagType type;
	union _FlagValue value;
	const char* description;
};

struct _ParserContext
{
	struct _Flag* flags;
	unsigned int count;
	unsigned int maxCount;
};

struct _ParserContext* _createParserContext(
	const unsigned int maxFlagsCount)
{
	struct _ParserContext* context = (struct _ParserContext*)malloc(sizeof(struct _ParserContext));
	assert(context != NULL);

	context->flags = (struct _Flag*)malloc((maxFlagsCount) * sizeof(struct _Flag));
	assert(context->flags != NULL);

	context->count = 0;
	context->maxCount = maxFlagsCount;

	return context;
}

void _destroyParserContext(
	struct _ParserContext* const context)
{
	assert(context != NULL);
	assert(context->flags != NULL);

	for (unsigned int index = 0; index < context->maxCount; ++index)
	{
		struct _Flag* const flag = &context->flags[index];
		assert(flag != NULL);

		switch (flag->type)
		{
			case FLAG_STRING:
			{
				free(flag->value.s);
			} break;

			default:
			{
			} break;
		}
	}

	free(context);
}

struct _HelpBuffer
{
#ifndef HELP_BUFFER_CAPACITY
#	define HELP_BUFFER_CAPACITY 2048
#endif

	char buffer[HELP_BUFFER_CAPACITY + 1];
	unsigned int length;
};

static struct _HelpBuffer _helpBuffer = { .length = 0 };

static const char* const _stringifiedFlagTypes[] =
{
	[FLAG_BOOL]		= "bool",
	[FLAG_INT64]	= "int64",
	[FLAG_DOUBLE]	= "double",
	[FLAG_STRING]	= "string"
};

static const char* _formatHelp(
	struct _ParserContext* const context,
	const char* const usage)
{
	assert(context != NULL);
	assert(context->flags != NULL);
	_helpBuffer.length = 0;
	_helpBuffer.length += snprintf(
			_helpBuffer.buffer + _helpBuffer.length, HELP_BUFFER_CAPACITY,
			"Usage: %s\nOptions:\n", usage);

	for (unsigned int index = 0; index < context->count; ++index)
	{
		const struct _Flag* const flag = &context->flags[index];
		unsigned int written = snprintf(
			_helpBuffer.buffer + _helpBuffer.length,
			HELP_BUFFER_CAPACITY,
			"    %s\n        Usage: %s\n        [ %s ] - %s\n\n",
			flag->name,
			flag->usage,
			_stringifiedFlagTypes[flag->type],
			flag->description);
		_helpBuffer.length += written;
	}

	_helpBuffer.buffer[_helpBuffer.length] = '\0';
	return _helpBuffer.buffer;
}

static void _printUsage(FILE* const stream)
{
	fprintf(stream, "%s", _helpBuffer.buffer);
}

static struct _Flag* const _metaFlag(
	struct _ParserContext* const context,
	const char* name,
	const char* usage,
	const char* description)
{
	assert(context != NULL);
	assert(context->flags != NULL);
	assert(context->count < context->maxCount);
	struct _Flag* const flag = &context->flags[context->count++];
	assert(flag != NULL);
	flag->name = name;
	flag->usage = usage;
	flag->description = description;
	return flag;
}

bool* _boolFlag(
	struct _ParserContext* const context,
	const char* name,
	const char* usage,
	const char* description)
{
	assert(context != NULL);
	assert(context->flags != NULL);
	struct _Flag* const flag = _metaFlag(context, name, usage, description);
	assert(flag != NULL);
	flag->type = FLAG_BOOL;
	flag->value.b = false;
	return &flag->value.b;
}

int64* _int64Flag(
	struct _ParserContext* const context,
	const char* name,
	const char* usage,
	const char* description)
{
	assert(context != NULL);
	assert(context->flags != NULL);
	struct _Flag* const flag = _metaFlag(context, name, usage, description);
	assert(flag != NULL);
	flag->type = FLAG_INT64;
	flag->value.i64 = 0;
	return &flag->value.i64;
}

double* _doubleFlag(
	struct _ParserContext* const context,
	const char* name,
	const char* usage,
	const char* description)
{
	assert(context != NULL);
	assert(context->flags != NULL);
	struct _Flag* const flag = _metaFlag(context, name, usage, description);
	assert(flag != NULL);
	flag->type = FLAG_DOUBLE;
	flag->value.d = 0.0;
	return &flag->value.d;
}

const char* _stringFlag(
	struct _ParserContext* const context,
	const char* name,
	const char* usage,
	const char* description)
{
	assert(context != NULL);
	assert(context->flags != NULL);
	struct _Flag* const flag = _metaFlag(context, name, usage, description);
	assert(flag != NULL);
	flag->type = FLAG_STRING;
	memcpy(flag->value.s, "", 0);
	flag->value.s[0] = '\0';
	return (const char*)(flag->value.s);
}

static const char* _shift(
	int* argc,
	char*** argv)
{
	const char* current = NULL;

	if (*argc > 0)
	{
		current = **argv;
		*argc -= 1;
		*argv += 1;
	}

	return current;
}

static int _handleFlag(
	struct _Flag* const flag,
	const char* argValue)
{
	switch (flag->type)
	{
		case FLAG_BOOL:
		{
			char value[6];

			if (sscanf(argValue, "%s", value) == 1)
			{
				if (strcmp(value, "true") == 0)
				{
					flag->value.b = true;
					return 1;
				}
				else if (strcmp(value, "false") == 0)
				{
					flag->value.b = false;
					return 1;
				}
				else
				{
					fprintf(stderr, "ERROR: failed to parse boolean value for a flag %s!\n", flag->name);
					exit(1);
				}
			}
			else
			{
				fprintf(stderr, "ERROR: failed to parse value for a flag %s!\n", flag->name);
				exit(1);
			}
		} break;

		case FLAG_INT64:
		{
			int64 value = 0;

			if (sscanf(argValue, "%lld", &value) == 1)
			{
				flag->value.i64 = value;
				return 1;
			}
			else
			{
				fprintf(stderr, "ERROR: failed to parse value for a flag %s!\n", flag->name);
				exit(1);
			}
		} break;

		case FLAG_DOUBLE:
		{
			double value = 0;

			if (sscanf(argValue, "%lf", &value) == 1)
			{
				flag->value.d = value;
				return 1;
			}
			else
			{
				fprintf(stderr, "ERROR: failed to parse value for a flag %s!\n", flag->name);
				exit(1);
			}
		} break;

		case FLAG_STRING:
		{
			if (argValue != NULL)
			{
				unsigned int length = strlen(argValue);
				length = length >= STRING_FLAG_CAPACITY ? STRING_FLAG_CAPACITY : length;
				memcpy(flag->value.s, argValue, length);
				flag->value.s[length] = '\0';
				return 1;
			}
			else
			{
				fprintf(stderr, "ERROR: failed to parse value for a flag %s!\n", flag->name);
				exit(1);
			}
		} break;

		default:
		{
			fprintf(stderr, "ERROR: %s - unknown flag type!\n", _stringifiedFlagTypes[flag->type]);
			exit(1);
		} break;
	}

	return 0;
}

void _parseFlags(
	struct _ParserContext* const context,
	int argc,
	char** argv,
	const char* const usage)
{
	_formatHelp(context, usage);

	if (argc <= 0)
	{
		fprintf(stderr, "ERROR: no arguments were provided!\n");
		_printUsage(stderr);
		exit(1);
	}

	while (argc > 0)
	{
		const char* arg = _shift(&argc, &argv);

		if (arg == NULL)
		{
			fprintf(stderr, "ERROR: unreachable argument!\n");
			_printUsage(stderr);
			exit(1);
		}

		if (strcmp(arg, "--help") == 0)
		{
			_printUsage(stdout);
			exit(0);
		}

		int invalidArgumentEncountered = 0;

		for (unsigned int flagIndex = 0; flagIndex < context->count; ++flagIndex)
		{
			struct _Flag* const flag = &context->flags[flagIndex];

			if (strcmp(flag->name, arg) == 0)
			{
				const char* argValue = _shift(&argc, &argv);

				if (argValue == NULL)
				{
					fprintf(stderr, "ERROR: value was not found for flag %s!\n", flag->name);
					_printUsage(stderr);
					exit(1);
				}

				if (_handleFlag(flag, argValue))
				{
					invalidArgumentEncountered = 0;
					break;
				}
			}
			else
			{
				++invalidArgumentEncountered;
			}
		}

		if (invalidArgumentEncountered >= context->count)
		{
			fprintf(stderr, "ERROR: unknown argument `%s` encountered for flags cache!\n", arg);
			_printUsage(stderr);
			exit(1);
		}
	}
}
