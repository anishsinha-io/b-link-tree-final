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

void test_read_header() {
    format(TEST_MIN_ORDER, sizeof(node));
    header h;
    int    first = read_header(&h);
    assert(first == 0);
    assert(h.node_size == sizeof(node));
    assert(h.node_count == 1);
    assert(h.root_loc == 0);
    assert(h.order == TEST_MIN_ORDER);
    destroy_index();
}

void test_read_node() {
    format(TEST_MIN_ORDER, sizeof(node));
    node n;
    int  first = read_node(&n, 0);
    assert(first == 0);
    assert(n.num_keys == 0);
    assert(n.leaf == true);
    assert(n.high_key == INT32_MAX);
    assert(n.next_loc == -1);
    destroy_index();
}

void test_write_node() {
    format(TEST_MIN_ORDER, sizeof(node));
    node *n = create_node();
    n->num_keys = 1;
    write_node(1, n);
    header h;
    read_header(&h);
    assert(h.node_count == 2);
    node test;
    read_node(&test, 1);
    assert(test.num_keys == 1);
    destroy_index();
}

void test_scannode() {
    format(TEST_MIN_ORDER, sizeof(node));
    node n;
    read_node(&n, 0);
    n.num_keys = 3;
    n.keys[0] = 1;
    n.keys[1] = 2;
    n.keys[2] = 3;
    write_node(0, &n);
    read_node(&n, 0);
    scannode(&n);
    destroy_index();
}

void test_b_link_tree() {
    test_create_node();
    test_format();
    test_read_header();
    test_read_node();
    test_write_node();
    test_scannode();
}