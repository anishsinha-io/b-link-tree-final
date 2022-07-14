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
    slice_from_primitive_array(s, n->children, n->leaf ? n->num_keys : n->num_keys + 1, sizeof(int));
    s->compare        = compare_key;
    s->elem_to_string = key_to_string;
    return s;
}

node *create_node(int loc) {
    node *n = malloc(sizeof(node));
    n->leaf     = true;
    n->num_keys = 0;
    n->high_key = INT32_MAX;
    n->link_ptr = -1;
    n->order    = TEST_MIN_ORDER;
    n->loc      = loc;
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
    node *root = create_node(0);
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

int write_node(int loc, node *n) {
    header h;
    read_header(&h);
    write_buffer(INDEX_PATH, n, sizeof(*n), get_offset(loc));
    h.node_count++;
    return write_buffer(INDEX_PATH, &h, sizeof(h), 0);
}

int scannode(int key, node *n) {
    slice *keys = node_keys_to_slice(n);
    int   index = slice_find_index(keys, &key, NULL);
    if (index == keys->length) return n->link_ptr;
    return index;
}

static split *create_split(node *A, node *B, node *new_root) {
    split *s = malloc(sizeof(split));
    s->A        = A;
    s->B        = B;
    s->new_root = new_root;
    return s;
}


split *split_node(node *n) {
    header h;
    read_header(&h);
    slice *keys                 = node_keys_to_slice(n);
    slice *children             = node_children_to_slice(n);
    slice *first_half_keys      = subslice(keys, 0, keys->length / 2);
    slice *second_half_keys     = subslice(keys, keys->length / 2, keys->length);
    slice *first_half_children  = subslice(children, 0, children->length / 2);
    slice *second_half_children = subslice(children, children->length / 2, children->length);
    slice_to_primitive_array(first_half_keys, n->keys, first_half_keys->length, sizeof(int));
    slice_to_primitive_array(first_half_children, n->children, first_half_children->length, sizeof(int));
    node *B = create_node(h.node_count);
    slice_to_primitive_array(second_half_keys, B->keys, second_half_keys->length, sizeof(int));
    slice_to_primitive_array(second_half_children, B->children, second_half_children->length, sizeof(int));
    n->num_keys = first_half_keys->length;
    B->num_keys = second_half_keys->length;
    node *new_root = n->leaf ? create_node(h.node_count + 1) : NULL;
    new_root->num_keys = 0;
    return create_split(n, B, new_root);
}

stack *create_ancestor_stack() {
    stack *s = malloc(sizeof(stack));
    stack_init(s);
    return s;
}

static int move_right(int v, node *A) {
    if (!A) return EINVAL;
    int t;
    while ((t = scannode(v, A)) == A->link_ptr) {
        read_node(A, t);
    }
    return 0;
}

static bool check_key_exists(int v, node *n) {
    slice *keys = node_keys_to_slice(n);
}

int insert(int v, int i) {
    stack  *ancestor_stack = create_ancestor_stack();
    header h;
    read_header(&h);
    int  current = h.root_loc;
    node A;
    read_node(&A, current);
    while (!A.leaf) {
        int t = current;
        current = scannode(v, &A);
        if (current != A.link_ptr) stack_push(ancestor_stack, &t);
        read_node(&A, current);
    }
    read_node(&A, current);
    move_right(v, &A);

}