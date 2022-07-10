#ifndef __STRINGS__
#define __STRINGS__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef enum string_type {
    str_unsigned_int, str_int, str_signed_int,
    str_unsigned_long, str_long, str_signed_long,
    str_unsigned_long_long, str_long_long, str_signed_long_long,
    str_double, str_long_double,
    str_string
} string_type;

typedef struct string {
    int  len, capacity;
    char *buf;
} string;

string *str(char *seq);
string *append(string *self, void *str, string_type type);
string *substring(string *str);

int str_free(string *str);

void println(string *str);

#endif