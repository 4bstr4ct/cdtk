#ifndef _CFLAG_H_
#define _CFLAG_H_

/**
 * A typedef for boolean.
 */
typedef enum { false, true } bool;

/**
 * A typedef for signed long long.
 */
typedef signed long long int64;

/**
 * Function _boolFlag() defines a boolean flag with specified prefix, name,
 * default value, and description string. It returns an address of a @ref bool
 * variable that stores the value of the flag.
 */
bool* _boolFlag(const char* prefix, const char* name, const bool defaultValue, const char* description);

/**
 * Function _intFlag() defines a 64-bit int flag with specified prefix, name,
 * default value, and description string. It returns an address of an int64
 * variable that stores the value of the flag.
 */
int64* _intFlag(const char* prefix, const char* name, const int64 defaultValue, const char* description);

/**
 * Function _doubleFlag() defines a double flag with specified prefix, name,
 * default value, and description string. It returns an address of a double
 * variable that stores the value of the flag.
 */
double* _doubleFlag(const char* prefix, const char* name, const double defaultValue, const char* description);

/**
 * Function _stringFlag() defines a string flag with specified prefix, name,
 * default value, and description string. It returns an address of a cstring
 * variable (pointer to char pointer) that stores the value of the flag.
 */
const char** _stringFlag(const char* prefix, const char* name, const char* const defaultValue, const char* description);

/**
 * Function _parseFlags() emits first command-line arguments from provided argv
 * array and the rest of the arguments to flags that are stored in flags cache.
 * It must be called after all flags are defined and before flags are accessed
 * by the program. 
 */
void _parseFlags(int argc, char** argv);

#endif
