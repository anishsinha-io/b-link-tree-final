#include "strings.h"

string *str(char *seq) {
    string *s = malloc(sizeof(str));
    s->len      = seq ? (int) strlen(seq) : 0;
    s->capacity = seq ? (int) strlen(seq) * 2 : 10;
    s->buf      = malloc(sizeof(char) * s->capacity);
    if (seq) memcpy(s->buf, seq, sizeof(char) * s->capacity);
    return s;
}

int string_resize(string *str, int new_size) {
    if (!str) return EINVAL;
    str->buf      = realloc(str->buf, new_size);
    str->capacity = new_size;
    return 0;
}

int str_free(string *str) {
    if (!str) return EINVAL;
    str->capacity = str->len = 0;
    free(str->buf);
    str = NULL;
    free(str);
    return 0;
}


static string *append_str(string *s, char *str) {
    if (s->capacity < strlen(s->buf) + strlen(str))
        string_resize(s, (int) ((s->capacity + (double) strlen(str) * 10)));
    memcpy((void *) (s->buf + strlen(s->buf)), str, strlen(str));
    s->len = (int) strlen(s->buf);
    return s;
}

static string *append_long_double(string *s, const long double *num) {
    if (s->capacity < strlen(s->buf) + sizeof(long double) * 10)
        string_resize(s, (int) ((s->capacity + sizeof(*num) * 10)));
    sprintf((void *) (s->buf + strlen(s->buf)), "%Lf", *num);
    s->len = (int) strlen(s->buf);
    return s;
}

static string *append_unsigned_long_long(string *s, const unsigned long long *num) {
    if (s->capacity < strlen(s->buf) + sizeof(unsigned long long) * 10)
        string_resize(s, (int) ((s->capacity + sizeof(*num) * 10)));
    sprintf((void *) (s->buf + strlen(s->buf)), "%llu", *num);
    s->len = (int) strlen(s->buf);
    return s;
}

static string *append_long_long(string *s, const long long *num) {
    if (s->capacity < strlen(s->buf) + sizeof(long long) * 10)
        string_resize(s, (int) ((s->capacity + sizeof(*num) * 10)));
    sprintf((void *) (s->buf + strlen(s->buf)), "%lld", *num);
    s->len = (int) strlen(s->buf);
    return s;
}

static string *append_unsigned_long(string *s, const unsigned long *num) {
    if (s->capacity < strlen(s->buf) + sizeof(unsigned long long) * 10)
        string_resize(s, (int) ((s->capacity + sizeof(*num) * 10)));
    sprintf((void *) (s->buf + strlen(s->buf)), "%lu", *num);
    s->len = (int) strlen(s->buf);
    return s;
}

static string *append_long(string *s, const long *num) {
    if (s->capacity < strlen(s->buf) + sizeof(long) * 10)
        string_resize(s, (int) ((s->capacity + sizeof(*num) * 10)));
    sprintf((void *) (s->buf + strlen(s->buf)), "%ld", *num);
    s->len = (int) strlen(s->buf);
    return s;
}

static string *append_unsigned_int(string *s, const unsigned int *num) {
    if (s->capacity < strlen(s->buf) + sizeof(unsigned int) * 10)
        string_resize(s, (int) ((s->capacity + sizeof(*num) * 10)));
    sprintf((void *) (s->buf + strlen(s->buf)), "%i", *num);
    s->len = (int) strlen(s->buf);
    return s;
}

static string *append_int(string *s, const int *num) {
    if (s->capacity < strlen(s->buf) + sizeof(int) * 10)
        string_resize(s, (int) ((s->capacity + sizeof(*num) * 10)));
    sprintf((void *) (s->buf + strlen(s->buf)), "%d", *num);
    s->len = (int) strlen(s->buf);
    return s;
}

static string *append_double(string *s, const double *num) {
    if (s->capacity < strlen(s->buf) + sizeof(double) * 10)
        string_resize(s, (int) ((s->capacity + sizeof(*num) * 10)));
    sprintf((void *) (s->buf + strlen(s->buf)), "%f", *num);
    s->len = (int) strlen(s->buf);
    return s;
}

static string *append_string(string *s, const string *str) {
    if (s->capacity < strlen(s->buf) + str->len)
        string_resize(s, (int) ((s->capacity + str->len * 2)));
    memcpy((void *) (s->buf + strlen(s->buf)), str->buf, str->len);
    s->len = (int) strlen(s->buf);
    return s;
}

string *append(string *s, void *str, string_type type) {
    if (!s || !str) return s;
    switch (type) {
        case str_double:
            return append_double(s, (const double *) str);
        case str_int:
        case str_signed_int:
            return append_int(s, (const int *) str);
        case str_unsigned_int:
            return append_unsigned_int(s, (const unsigned int *) str);
        case str_long:
        case str_signed_long:
            return append_long(s, (const long *) str);
        case str_unsigned_long:
            return append_unsigned_long(s, (const unsigned long *) str);
        case str_signed_long_long:
        case str_long_long:
            return append_long_long(s, (const long long *) str);
        case str_unsigned_long_long:
            return append_unsigned_long_long(s, (const unsigned long long *) str);
        case str_long_double:
            return append_long_double(s, (const long double *) str);
        case str_str:
            return append_str(s, (char *) str);
        case str_string:
            return append_string(s, (string *) str);
        default:
            return str;
    }
}

void println(string *str) {
    if (!str) return;
    printf("%s\n", str->buf);
}