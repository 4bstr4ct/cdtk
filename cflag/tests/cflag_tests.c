#include "../include/cflag.h"

#include <stdio.h>

int main(int argc, char** argv)
{
	bool* boolFlag = _boolFlag(
		"--",
		"tests",
		true,
		"Enable tests"
	);

	int64* int64Flag = _intFlag(
		"--",
		"special-value",
		0,
		"Special value for some kind of thing"
	);

	double* doubleFlag = _doubleFlag(
		"-",
		"doubly",
		1,
		"Doubly thing"
	);

	const char* const * textFlag = _stringFlag(
		"+",
		"text",
		"",
		"Some text for somtehing"
	);

	--argc; ++argv;
	_parseFlags(argc, argv);

	fprintf(stdout, "bool flag = %d\n", *boolFlag);
	fprintf(stdout, "int64 flag = %d\n", *int64Flag);
	fprintf(stdout, "double flag = %lf\n", *doubleFlag);
	fprintf(stdout, "text flag = %s\n", textFlag);

	return 0;
}