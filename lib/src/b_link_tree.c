#include "b_link_tree.h"

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
    h.page_count = 1;
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

int read_header(header *h) {
    return read_buffer(INDEX_PATH, h, sizeof(*h), 0);
}

int read_node(node *n, int loc) {
    off_t offset = get_offset(loc);
    return read_buffer(INDEX_PATH, n, sizeof(*n), offset);
}
