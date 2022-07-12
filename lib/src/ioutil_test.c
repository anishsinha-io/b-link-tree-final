#include "ioutil.h"
#include "ioutil_test.h"

#define IOUTIL_TEST_PATH "/Users/anishsinha/Home/cprojects/project-final/__ioutil_tests__/test_file.bin"

typedef struct test_header {
    int  id;
    char name[50];
} test_header;

void test_write_buffer() {
    test_header header;
    header.id = 1;
    strncpy(header.name, "text here", sizeof(header.name));
    int first = write_buffer(IOUTIL_TEST_PATH, &header, sizeof(header), 0);
    assert(first == 0);
}

void test_read_buffer() {
    test_header header;
    int         first = read_buffer(IOUTIL_TEST_PATH, &header, sizeof(header), 0);
    assert(first == 0);
    assert(header.id == 1);
    assert(strncmp(header.name, "text here", sizeof(header.name)) == 0);
}

void test_clear_file() {
    int first = clear_file(IOUTIL_TEST_PATH);
    assert(first == 0);
}

void test_ioutil() {
    test_write_buffer();
    test_read_buffer();
    test_clear_file();
}