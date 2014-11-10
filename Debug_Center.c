#include <stdio.h>
#include "Debug_Center.h"

void printError(char const *where, char const *type, char const *function, char const *message)
{
	log_file = fopen("log.txt", "a");
	fprintf(log_file, "[%s : %s] %s::%s - %s\n", where, type, where, function, message);
	fclose(log_file);
}