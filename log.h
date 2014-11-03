#ifndef _LOG_H
#define _LOG_H

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define MAX_LOG_SIZE 4 * 1024 * 1024
#define MAX_LOG_FILE 5

extern const char* fpm_path;
extern const char* log_prefix;
extern const char* log_postfix;
extern FILE* log_fp;
extern int log_level;


void log_to_file(int print_level, const char* str, ... );

#endif /* _LOG_H */
