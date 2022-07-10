#include "strings.h"
#include "strings_test.h"

void test_string_create() {
    string *s = str(NULL);
    assert(s != NULL);
    assert(s->len == 0);
    assert(s->capacity == 10);
    int first = str_free(s);
    assert(first == 0);
    string *s2 = str("test string here");
    assert(s2 != NULL);
    assert(s2->len == 16);
    assert(strncmp(s2->buf, "test string here", strlen(s2->buf)) == 0);
    str_free(s);
    str_free(s2);
}

void test_string_append_string() {
    string *s = str("test ");
    assert(s->capacity == 10);
    assert(s->len == 5);
    append(s, "string", str_string);
    assert(strcmp("test string", s->buf) == 0);
    assert(strlen(s->buf) == 11);
    assert(s->capacity == (int) (10 + strlen("string") * 10));
    str_free(s);
}

void test_string_append_long_double() {
    string *s               = str("test ");
    int    initial_capacity = s->capacity;
    assert(s->capacity == 10);
    assert(s->len == 5);
    long double x = -1000000000000000;
    append(s, &x, str_long_double);
    assert(s->len == 29);
    assert(s->capacity == initial_capacity + sizeof(long double) * 10);
    str_free(s);
}

void test_string_append_unsigned_long_long() {
    string *s               = str("test ");
    int    initial_capacity = s->capacity;
    assert(s->capacity == 10);
    assert(s->len == 5);
    unsigned long long x = 1000000000000000;
    append(s, &x, str_unsigned_long_long);
    assert(s->len == 21);
    assert(s->capacity == initial_capacity + sizeof(unsigned long long) * 10);
    str_free(s);
}

void test_string_append_long_long() {
    string *s               = str("test ");
    int    initial_capacity = s->capacity;
    assert(s->capacity == 10);
    assert(s->len == 5);
    long long x = -1000000000000000;
    append(s, &x, str_long_long);
    assert(s->len == 22);
    assert(s->capacity == initial_capacity + sizeof(unsigned long long) * 10);
    str_free(s);
}

void test_string_append_unsigned_long() {
    string *s               = str("test ");
    int    initial_capacity = s->capacity;
    assert(s->capacity == 10);
    assert(s->len == 5);
    unsigned long x = 1000000000000000;
    append(s, &x, str_unsigned_long);
    assert(s->len == 21);
    assert(s->capacity == initial_capacity + sizeof(unsigned long long) * 10);
    str_free(s);
}

void test_string_append_long() {
    string *s               = str("test ");
    int    initial_capacity = s->capacity;
    assert(s->capacity == 10);
    assert(s->len == 5);
    long x = 3000000000;
    append(s, &x, str_long);
    assert(s->len == 15);
    assert(s->capacity == initial_capacity + sizeof(long) * 10);
    str_free(s);
}

void test_string_append_unsigned_int() {
    string *s               = str("test ");
    int    initial_capacity = s->capacity;
    assert(s->capacity == 10);
    assert(s->len == 5);
    unsigned int x = 2000000000;
    append(s, &x, str_unsigned_int);
    assert(s->len == 15);
    assert(s->capacity == initial_capacity + sizeof(unsigned int) * 10);
    str_free(s);
}

void test_string_append_int() {
    string *s               = str("test ");
    int    initial_capacity = s->capacity;
    assert(s->capacity == 10);
    assert(s->len == 5);
    unsigned int x = -2000000000;
    append(s, &x, str_int);
    assert(s->len == 16);
    assert(s->capacity == initial_capacity + sizeof(unsigned int) * 10);
    str_free(s);
}

void test_string_append_double() {
    string *s               = str("test ");
    int    initial_capacity = s->capacity;
    assert(s->capacity == 10);
    assert(s->len == 5);
    double x = -1000000000;
    append(s, &x, str_double);
    assert(s->len == 23);
    assert(s->capacity == initial_capacity + sizeof(double) * 10);
    str_free(s);
}

void test_strings() {
    test_string_create();
    test_string_append_string();
    test_string_append_long_double();
    test_string_append_unsigned_long_long();
    test_string_append_long_long();
    test_string_append_unsigned_long();
    test_string_append_long();
    test_string_append_unsigned_int();
    test_string_append_int();
    test_string_append_double();
}