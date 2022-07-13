#include "b_link_tree.h"

int compare_key(const void *key1, const void *key2) {
    return *(int *) key1 - *(int *) key2;
}

string *key_to_string(const void *key) {
    string *s = str(NULL);
    append(s, (int *) key, str_int);
    return s;
}

slice *node_keys_to_slice(node *n) {
    slice *s = malloc(sizeof(slice));
    slice_from_primitive_array(s, n->keys, n->num_keys, sizeof(int));
    s->compare        = compare_key;
    s->elem_to_string = key_to_string;
    return s;
}

slice *node_children_to_slice(node *n) {
    slice *s = malloc(sizeof(slice));
    slice_from_primitive_array(s, n->keys, n->leaf ? n->num_keys : n->num_keys + 1, sizeof(int));
    s->compare        = compare_key;
    s->elem_to_string = key_to_string;
    return s;
}

node *create_node() {
    node *n = malloc(sizeof(node));
    n->leaf     = true;
    n->num_keys = 0;
    n->high_key = INT32_MAX;
    n->next_loc = -1;
    n->order    = TEST_MIN_ORDER;
    return n;
}

int destroy_node(node *n) {
    if (!n) return EINVAL;
    free(n);
    return 0;
}

int format(int order, size_t node_size) {
    int    status;
    header h;
    h.node_size  = node_size;
    h.root_loc   = 0;
    h.order      = order;
    h.node_count = 1;
    node *root = create_node();
    status = write_buffer(INDEX_PATH, &h, sizeof(h), 0);
    if (status != 0) return -1;
    status = write_buffer(INDEX_PATH, root, sizeof(*root), sizeof(header));
    if (status != 0) return -1;
    return 0;
}

int destroy_index() {
    return clear_file(INDEX_PATH);
}

off_t get_offset(int loc) {
    return (off_t) sizeof(header) + (off_t) sizeof(node) * loc;
}

off_t get_end_offset() {
    header h;
    read_header(&h);
    return (off_t) h.node_count * (off_t) h.node_size + (off_t) sizeof(h);
}

int read_header(header *h) {
    return read_buffer(INDEX_PATH, h, sizeof(*h), 0);
}

int read_node(node *n, int loc) {
    off_t offset = get_offset(loc);
    return read_buffer(INDEX_PATH, n, sizeof(*n), offset);
}

int write_node(int loc, node *n) {
    header h;
    read_header(&h);
    write_buffer(INDEX_PATH, n, sizeof(*n), get_offset(loc));
    h.node_count++;
    return write_buffer(INDEX_PATH, &h, sizeof(h), 0);
}

int scannode(node *n) {
    slice *keys = node_keys_to_slice(n);
    println(slice_to_string(keys, NULL));
    
}
