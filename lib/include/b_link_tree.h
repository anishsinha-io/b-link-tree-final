#ifndef __B_LINK_TREE__
#define __B_LINK_TREE__

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "common.h"
#include "ioutil.h"
#include "slice.h"
#include "stack.h"

#define TEST_MIN_ORDER 2
#define PROD_MIN_ORDER 300

#define INDEX_PATH "/Users/anishsinha/Home/cprojects/project-final/index/index.bin"

typedef struct node {
    bool leaf;
    int  order, num_keys, high_key, next_loc;
    int  keys[TEST_MIN_ORDER * 2], children[TEST_MIN_ORDER * 2 + 1];
} node;

typedef struct header {
    int    node_count, root_loc, order;
    size_t node_size;
} header;

int destroy_node(node *n);
int format(int order, size_t node_size);
int destroy_index();
int read_header(header *h);
int read_node(node *n, int loc);
int write_node(int loc, node *n);
int scannode(node *n);

off_t get_offset(int loc);
node *create_node();

#endif