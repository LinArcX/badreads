#ifndef BADREADS_UTIL_H
#define BADREADS_UTIL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

extern char *source;

void
usage(char *program);

void
read_file_into_buffer(char *file_name);

#endif
