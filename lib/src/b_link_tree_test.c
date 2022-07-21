#include "b_link_tree_test.h"

void test_create_node() {
    node *n = create_node(-1);
    assert(n != NULL);
    assert(n->num_keys == 0);
    assert(n->leaf == true);
    assert(n->high_key == INT32_MAX);
    assert(n->link_ptr == -1);
    int first = destroy_node(n);
    assert(first == 0);
}

void test_format() {
    int         fd    = open(INDEX_PATH, O_RDONLY);
    struct stat buf;
    int         first = format(TEST_MIN_ORDER, sizeof(node));
    fstat(fd, &buf);
    assert(first == 0);
    int second = destroy_index();
    fstat(fd, &buf);
    assert(second == 0);
    assert(buf.st_size == 0);
}

void test_read_header() {
    destroy_index();
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
    destroy_index();
    format(TEST_MIN_ORDER, sizeof(node));
    node n;
    int  first = read_node(&n, 0);
    assert(first == 0);
    assert(n.num_keys == 0);
    assert(n.leaf == true);
    assert(n.high_key == INT32_MAX);
    assert(n.link_ptr == -1);
    assert(n.loc == 0);
    destroy_index();
}

void test_write_node() {
    destroy_index();
    format(TEST_MIN_ORDER, sizeof(node));
    header h;
    read_header(&h);
    node *n = create_node(h.node_count);
    n->num_keys = 1;
    write_node(1, n);
    h.node_count++;
    write_header(&h);
    read_header(&h);
    assert(h.node_count == 2);
    node test;
    read_node(&test, 1);
    assert(test.loc == 1);
    assert(test.num_keys == 1);
    destroy_index();
}

void test_scannode() {
    destroy_index();
    format(TEST_MIN_ORDER, sizeof(node));
    node n;
    read_node(&n, 0);
    n.num_keys = 3;
    n.keys[0] = 1;
    n.keys[1] = 2;
    n.keys[2] = 3;
    write_node(0, &n);
    read_node(&n, 0);
    int key2   = 2;
    int index2 = scannode(key2, &n);
    assert(index2 != n.link_ptr);
    // assert(index2 == 1);
    destroy_index();
}

void test_split_node() {
    destroy_index();
    format(TEST_MIN_ORDER, sizeof(node));
    node root;
    read_node(&root, 0);
    root.num_keys = 4;
    root.keys[0]     = 1;
    root.keys[1]     = 2;
    root.keys[2]     = 3;
    root.keys[3]     = 4;
    root.children[0] = 10;
    root.children[1] = 20;
    root.children[2] = 30;
    root.children[3] = 40;
    write_node(0, &root);
    read_node(&root, 0);
    split *test_split = split_node(5, 50, &root);
    assert(test_split->new_root != NULL);
    // println(node_to_string(test_split->A));
    // println(node_to_string(test_split->B));
    // println(node_to_string(test_split->new_root));
    destroy_index();
}

void test_insert() {
    destroy_index();
    format(TEST_MIN_ORDER, sizeof(node));
    insert(1, 10);
    insert(2, 20);
    insert(3, 30);
    insert(4, 40);
    insert(5, 50);
    insert(6, 60);
    insert(7, 70);
    insert(8, 80);
    insert(9, 90);
    insert(10, 100);
    insert(11, 110);
    insert(12, 120);
    insert(13, 130);
    insert(14, 140);
    insert(15, 150);
    insert(16, 160);
    insert(17, 170);
    node   root;
    header h;
    read_header(&h);
    println(header_to_string(&h));
    read_node(&root, h.root_loc);
    println(node_to_string(&root));
    node d1;
    read_node(&d1, root.children[0]);
    println(node_to_string(&d1));
    node d2;
    read_node(&d2, root.children[1]);
    println(node_to_string(&d2));
    node d3;
    read_node(&d3, d1.children[0]);
    println(node_to_string(&d3));
    node d4;
    read_node(&d4, d3.link_ptr);
    println(node_to_string(&d4));
    node d5;
    read_node(&d5, d4.link_ptr);
    println(node_to_string(&d5));
    node d6;
    read_node(&d6, d5.link_ptr);
    println(node_to_string(&d6));
    node d7;
    read_node(&d7, d6.link_ptr);
    println(node_to_string(&d7));
    node d8;
    read_node(&d8, d7.link_ptr);
    println(node_to_string(&d8));
    destroy_index();
}

void test_b_link_tree() {
    test_create_node();
    test_format();
    test_read_header();
    test_read_node();
    test_write_node();
    test_scannode();
    test_split_node();
    test_insert();
}