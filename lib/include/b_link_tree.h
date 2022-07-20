#ifndef __B_LINK_TREE__
#define __B_LINK_TREE__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/stat.h>

#include "common.h"
#include "ioutil.h"
#include "slice.h"
#include "stack.h"

#define TEST_MIN_ORDER 2
#define PROD_MIN_ORDER 300
#define _STDC_LIMIT_MACROS

#define INDEX_PATH "/Users/anishsinha/Home/cprojects/project-final/index/index.bin"

typedef struct node {
    bool leaf;
    int  order, num_keys, high_key, link_ptr, loc;
    int  keys[TEST_MIN_ORDER * 2], children[TEST_MIN_ORDER * 2 + 1];
} node;

typedef struct header {
    int    node_count, root_loc, order;
    size_t node_size;
} header;

typedef struct split {
    node *A, *B, *new_root;
} split;

int destroy_node(node *n);
int format(int order, size_t node_size);
int destroy_index();
int write_header(header *h);
int read_header(header *h);
int read_node(node *n, int loc);
int write_node(int loc, node *n);
int scannode(int key, node *n);
int insert(int v, int w);
slice *node_keys_to_slice(node *n);
slice *node_children_to_slice(node *n);
off_t get_offset(int loc);
node *create_node(int loc);
split *split_node(int v, int w, node *A);
string *node_to_string(const void *n);
string *header_to_string(const void *h);

#endif

