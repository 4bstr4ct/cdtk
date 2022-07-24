#include "../include/cflag.h"

#include <string.h>
#include <stdio.h>

int main(int argc, char** argv)
{
	struct _ParserContext* context = _createParserContext(4);

	bool* boolFlag = _boolFlag(
		context,
		"--boolFlag", "--boolFlag <value>",
		"Enable tests"
	);

	int64* int64Flag = _int64Flag(
		context,
		"--int64Flag", "--int64Flag <value>",
		"Special value for some kind of thing"
	);

	double* doubleFlag = _doubleFlag(
		context,
		"--doubleFlag", "--doubleFlag <value>",
		"Doubly thing"
	);

	const char* textFlag = _stringFlag(
		context,
		"--textFlag", "--textFlag <path>",
		"Some text for somtehing"
	);

	--argc; ++argv;
	_parseFlags(context, argc, argv, "Usage: cflag_test.exe [OPTIONS]");

	fprintf(stdout, "-------------------------\nboolFlag = %d\n", *boolFlag);
	fprintf(stdout, "int64Flag = %lld\n", *int64Flag);
	fprintf(stdout, "doubleFlag = %f\n", *doubleFlag);
	fprintf(stdout, "textFlag = %s\n", textFlag);
	return 0;
}
