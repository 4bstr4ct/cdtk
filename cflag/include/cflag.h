#ifndef _CFLAG_H_
#define _CFLAG_H_

#include <stdbool.h>

/**
 * @addtogroup
 * 
 * @{
 */

/**
 * A typedef for signed long long.
 */
typedef signed long long int64;

/**
 * A context for command line arguments parser to hold flags, their count and check bounds.
 */
struct _ParserContext;

struct _ParserContext* _createParserContext(
	const unsigned int maxFlagsCount);

#ifndef CREATE_PARSER_CONTEXT
#	define CREATE_PARSER_CONTEXT(maxFlagsCount) \
	_createParserContext(maxFlagsCount)
#endif

void _destroyParserContext(
	struct _ParserContext* const context);

#ifndef DESTROY_PARSER_CONTEXT
#	define DESTROY_PARSER_CONTEXT(context) \
	_destroyParserContext(context)
#endif

/**
 * Function _boolFlag() defines a boolean flag with specified name, and description string. It
 * returns an address of a @ref bool variable that stores the value of the flag.
 */
bool* _boolFlag(
	struct _ParserContext* const context,
	const char* name,
	const char* usage,
	const char* description);

/**
 * Function _int64Flag() defines a 64-bit signed long long flag with specified name, and
 * description string. It returns an address of an @ref int64 variable that stores the value of the
 * flag.
 */
int64* _int64Flag(
	struct _ParserContext* const context,
	const char* name,
	const char* usage,
	const char* description);

/**
 * Function _doubleFlag() defines a double flag with specified name, and description string. It
 * returns an address of a double variable that stores the value of the flag.
 */
double* _doubleFlag(
	struct _ParserContext* const context,
	const char* name,
	const char* usage,
	const char* description);

/**
 * Function _stringFlag() defines a string flag with specified name, and description string. It
 * returns an address of a cstring's first element (pointer to char array) that stores the value
 * of the flag.
 */
const char* _stringFlag(
	struct _ParserContext* const context,
	const char* name,
	const char* usage,
	const char* description);

/**
 * Function _parseFlags() emits first command-line arguments from provided argv array and the rest
 * of the arguments to flags that are stored in flags cache. It must be called after all flags are
 * defined and before flags are accessed by the program. 
 */
void _parseFlags(
	struct _ParserContext* const context,
	int argc,
	char** argv,
	const char* const usage);

/**
 * @}
 */

#endif
