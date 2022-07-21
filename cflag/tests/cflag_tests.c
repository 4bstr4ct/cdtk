#include "../include/cflag.h"

#include <string.h>
#include <stdio.h>

int main(int argc, char** argv)
{
	bool* boolFlag = _boolFlag(
		"--boolFlag", "--boolFlag <value>",
		true, "Enable tests"
	);

	int64* int64Flag = _int64Flag(
		"--int64Flag", "--int64Flag <value>",
		0, "Special value for some kind of thing"
	);

	double* doubleFlag = _doubleFlag(
		"--doubleFlag", "--doubleFlag <value>",
		1, "Doubly thing"
	);

	const char* textFlag = _stringFlag(
		"--textFlag", "--textFlag <path>",
		"none", "Some text for somtehing"
	);

	--argc; ++argv;
	_parseFlags(argc, argv, PARSE_STRICT, "Usage: cflag_test.exe [OPTIONS]");

	fprintf(stdout, "-------------------------\nboolFlag = %d\n", *boolFlag);
	fprintf(stdout, "int64Flag = %lld\n", *int64Flag);
	fprintf(stdout, "doubleFlag = %f\n", *doubleFlag);
	fprintf(stdout, "textFlag = %s\n", textFlag);
	return 0;
}
