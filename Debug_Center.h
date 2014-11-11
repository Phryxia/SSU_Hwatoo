#ifndef DEBUG_CENTER_H
#define DEBUG_CENTER_H
#include <stdio.h>

FILE *log_file;

void logReset(void);
void printError(char const *where, char const *type, char const *function, char const *message);

#endif