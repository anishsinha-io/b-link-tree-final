#include "b_link_tree_test.h"

void test_create_node() {
    node *n = create_node();
    assert(n != NULL);
    assert(n->num_keys == 0);
    assert(n->leaf == true);
    assert(n->high_key == INT32_MAX);
    assert(n->next_loc == -1);
    int first = destroy_node(n);
    assert(first == 0);
}

void test_format() {
    int         fd    = open(INDEX_PATH, O_RDONLY);
    struct stat buf;
    int         first = format(TEST_MIN_ORDER, sizeof(node));
    fstat(fd, &buf);
    assert(first == 0);
    assert(buf.st_size == 80);
    int second = destroy_index();
    fstat(fd, &buf);
    assert(second == 0);
    assert(buf.st_size == 0);
}

void test_b_link_tree() {
    test_create_node();
    test_format();
}