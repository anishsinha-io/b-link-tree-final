#include "b_link_tree.h"

/*
** This function is a utility function that compares two integer keys. This is used as the `compare` function pointer
** in all slices present in this file when working with node data.
*/
int compare_key(const void *key1, const void *key2) {
    return *(int *) key1 - *(int *) key2;
}

/*
** This function is a utility function that converts a node key or value to string (as they are the same type). This
** is used as the `elem_to_string` function pointer in all slices present in this file when working with node data.
*/
string *key_to_string(const void *key) {
    string *s = str(NULL);
    append(s, (int *) key, str_int);
    return s;
}

/*
** This function converts a node's keyset into a manipulable slice object and is used extensively in all tree operations
*/
slice *node_keys_to_slice(node *n) {
    slice *s = malloc(sizeof(slice));
    slice_from_primitive_array(s, n->keys, n->num_keys, sizeof(int));
    s->compare        = compare_key;
    s->elem_to_string = key_to_string;
    return s;
}

/*
** This function converts a node's children set into a manipulable slice object and is used extensively in all tree
** operations
*/
slice *node_children_to_slice(node *n) {
    slice *s = malloc(sizeof(slice));
    slice_from_primitive_array(s, n->children, n->leaf ? n->num_keys : n->num_keys + 1, sizeof(int));
    s->compare        = compare_key;
    s->elem_to_string = key_to_string;
    return s;
}

string *node_to_string(const void *n) {
    string *out = str("node [");
    append(out, "\n\tleaf=", str_str);
    append(out, ((node *) n)->leaf ? "true" : "false", str_str);
    append(out, "\n\torder=", str_str);
    append(out, &((node *) n)->order, str_int);
    append(out, "\n\tnum_keys=", str_str);
    append(out, &((node *) n)->num_keys, str_int);
    append(out, "\n\thigh_key=", str_str);
    append(out, &((node *) n)->high_key, str_int);
    append(out, "\n\tlink_ptr=", str_str);
    append(out, &((node *) n)->link_ptr, str_int);
    append(out, "\n\tloc=", str_str);
    append(out, &((node *) n)->loc, str_int);
    append(out, "\n\tkeys=", str_str);
    append(out, slice_to_string(node_keys_to_slice((node *) n), NULL), str_string);
    append(out, "\tchildren=", str_str);
    append(out, slice_to_string(node_children_to_slice((node *) n), NULL), str_string);
    append(out, "]\n", str_str);
    return out;
}

string *header_to_string(const void *h) {
    string *out = str("header [");
    append(out, "\n\tnode_count=", str_str);
    append(out, &((header *) h)->node_count, str_int);
    append(out, "\n\troot_loc=", str_str);
    append(out, &((header *) h)->root_loc, str_int);
    append(out, "\n\torder=", str_str);
    append(out, &((header *) h)->order, str_int);
    append(out, "\n\tnode_size=", str_str);
    append(out, &((header *) h)->node_size, str_unsigned_long);
    append(out, "\n]\n", str_str);
    return out;
}

/*
** This function is used to create a node. A node is initialized with a `loc` (location) which is a node's logical
** position on the disk file where it resides. Every node is automatically initialized as a leaf, with zero keys, a high
** key of INT32_MAX (2147483647) which is large enough for the purposes of this project.
*/
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

/*
** This function is used to free a node after it has been written to the disk or is no longer necessary to hold in
** memory. This function returns 0 on success or EINVAL (22) on any error.
*/
int destroy_node(node *n) {
    if (!n) return EINVAL;
    free(n);
    return 0;
}

/*
** This function formats the disk. Formatting is defined as (1) writing a new header at the start of the disk. The
** header's node size and order are initialized to the parameters passed in, the initial root location is 0, and the
** initial node count is 1. This is because, immediately after writing the header, a new, empty root is written to the
** disk at 0 bytes of offset from the end of the header. This function returns 0 on success or -1 on any error.
*/
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

/*
** This function destroys an index by clearing the file entirely. It returns 0 on success or an error code if anything
** else happens.
*/
int destroy_index() {
    return clear_file(INDEX_PATH);
}

/*
** This function returns the offset (in bytes) of the node in location `loc` (the input).
*/
off_t get_offset(int loc) {
    return (off_t) sizeof(header) + (off_t) sizeof(node) * loc;
}

/*
** This function reads the header of the index into memory and returns 0 on success or an error code in the case of an
** error. This is just a convenient wrapper around `read_buffer` from the `ioutil` module.
*/
int read_header(header *h) {
    return read_buffer(INDEX_PATH, h, sizeof(*h), 0);
}

/*
** This function writes the header passed into the function to the disk at offset 0. It returns a 0 on success or an
** error code upon an error, and is a convenient wrapper around `write_buffer`.
*/
int write_header(header *h) {
    return write_buffer(INDEX_PATH, h, sizeof(*h), 0);
}

/*
** This function reads a node into the buffer provided (n) at the given location (loc). This is a wrapper around both
** get_offset and read_buffer and returns 0 on success or an error code in the case of an error.
*/
int read_node(node *n, int loc) {
    off_t offset = get_offset(loc);
    return read_buffer(INDEX_PATH, n, sizeof(*n), offset);
}

/*
** This function writes a node from the buffer provided (n) to the location provided (loc). It modifies the header's
** information where necessary and rewrites both the header and the node to the disk
*/
int write_node(int loc, node *n) {
    header h;
    read_header(&h);
    write_buffer(INDEX_PATH, n, sizeof(*n), get_offset(loc));
    return write_header(&h);
}

/*
** This function scans the passed in node for the location at which the passed in key should be inserted at. It reads
** the node's keys into a slice, searches for the index, and returns it.
*/
int scannode(int key, node *n) {
    slice *keys = node_keys_to_slice(n);
    int   index = slice_find_index(keys, &key, NULL);
    if (index == keys->length && n->link_ptr != -1) {
        slice_free(keys);
        return n->link_ptr;
    }
    slice_free(keys);
    return index;
}

/*
** This function initializes a split object with three node pointers and returns it.
*/
static split *create_split(node *A, node *B, node *new_root) {
    split *s = malloc(sizeof(split));
    s->A        = A;
    s->B        = B;
    s->new_root = new_root;
    return s;
}

int increment_node_count(header *h) {
    if (!h) return EINVAL;
    h->node_count++;
    write_header(h);
    return 0;
}

/*
** This function splits a full node into two nodes. The process is as follows:
**
** 1. Read the keys and children into slices to manipulate
** 2. Find the index where value v needs to be inserted into the keys, and insert v into it, and w into the same index
**    in the children slice.
** 3. Cut the keys and children in half (unevenly) so that the left subslice of each has one more node than the right.
** 4. Create a new node called B.
** 5. Write the slices back into their respective buffers. (Left halves of everything go in the original node and
**    right halves go in B).
** 6. Change the link pointers on each node (B's link pointer points to A's old link pointer, and A's link pointer now
**    points to B's location).
** 7. If the node being split was the root, then modify the header and insert the two locations of A and B one after
**    another in the root's children array. If A and B were leaves, copy the value of A's high key to the root's
**    keys field's first slot. If A and B were not leaves, then remove A's high key from A, update A, and insert this
**    into the root.
** 8. Return A, B, and root.
*/
split *split_node(int v, int w, node *A) {
    header h;
    read_header(&h);
    slice *keys     = node_keys_to_slice(A);
    slice *children = node_children_to_slice(A);
    int   index     = slice_find_index(keys, &v, NULL);
    slice_insert_index(keys, &v, index);
    slice_insert_index(children, &w, index);
    slice *first_half_keys      = subslice(keys, 0, keys->length / 2 + 1);
    slice *second_half_keys     = subslice(keys, keys->length / 2 + 1, keys->length);
    slice *first_half_children  = subslice(children, 0, children->length / 2 + 1);
    slice *second_half_children = subslice(children, children->length / 2 + 1, children->length);
    A->num_keys = first_half_keys->length;
    slice_to_primitive_array(first_half_keys, A->keys, first_half_keys->length, sizeof(int));
    slice_to_primitive_array(first_half_children, A->children, first_half_children->length, sizeof(int));
    increment_node_count(&h);
    node *B = create_node(h.node_count - 1);
    B->num_keys = second_half_keys->length;
    slice_to_primitive_array(second_half_keys, B->keys, second_half_keys->length, sizeof(int));
    slice_to_primitive_array(second_half_children, B->children, second_half_children->length, sizeof(int));
    B->link_ptr = A->link_ptr;
    A->link_ptr = B->loc;
    B->leaf = A->leaf;
    A->high_key = *(int *) first_half_keys->keys[keys->length / 2];
    B->high_key = *(int *) second_half_keys->keys[second_half_keys->length - 1];
    node *new_root = NULL;
    if (A->loc == h.root_loc) {
        increment_node_count(&h);
        new_root = create_node(h.node_count - 1);
        if (A->leaf) {
            void *key = slice_get_index(first_half_keys, (first_half_keys->length - 1));
            new_root->keys[0] = *(int *) key;
            new_root->num_keys++;
        } else {
            void *key = slice_delete_index(first_half_keys, (first_half_keys->length - 1));
            new_root->keys[0] = *(int *) key;
            new_root->num_keys++;
        }
        new_root->leaf     = false;
        new_root->high_key = new_root->keys[0];
        slice *new_root_children = malloc(sizeof(slice));
        slice_default(new_root_children);
        slice_append(new_root_children, &A->loc);
        slice_append(new_root_children, &B->loc);
        slice_to_primitive_array(new_root_children, new_root->children, new_root_children->length, sizeof(int));
        slice_free(new_root_children);
        h.root_loc = new_root->loc;
        write_header(&h);
    }
    return create_split(A, B, new_root);
}

/*
** This function initializes and returns the ancestor stack that will be used to keep track of node ancestors. It is a
** wrapper around `stack_init`
*/
stack *create_ancestor_stack() {
    stack *s = malloc(sizeof(stack));
    stack_init(s);
    return s;
}

/*
** This function is used to move right horizontally by following node link pointers until the value v does not return
** A's link pointer when scannode is called
*/
static int move_right(int v, node *A) {
    if (!A) return EINVAL;
    int t = scannode(v, A);
    while (t == A->link_ptr && A->link_ptr != -1) {
        read_node(A, t);
        t = scannode(v, A);
    }
    return 0;
}

/*
** This function checks whether the passed in value is present in the node or not
*/
static int check_key_exists(int v, node *n) {
    slice *keys = node_keys_to_slice(n);
    int   index = slice_search(keys, &v, 0, keys->length, NULL);
    return index;
}

/*
** This function determines whether the passed in node is safe by checking the node's number of keys and its order
*/
static bool is_safe(node *n) {
    return n->num_keys < n->order * 2;
}

/*
** This function inserts a value into a node that is known to be safe (has space)
*/
static int insert_safe(int v, int w, node *n) {
    if (!n) return EINVAL;
    slice *keys      = node_keys_to_slice(n);
    slice *children  = node_children_to_slice(n);
    int   key_index  = slice_find_index(keys, &v, NULL);
    int   data_index = slice_find_index(children, &w, NULL);
    slice_insert_index(keys, &v, key_index);
    slice_insert_index(children, &w, data_index);
    n->high_key = *(int *) keys->keys[keys->length - 1];
    n->num_keys++;
    slice_to_primitive_array(keys, n->keys, keys->length, sizeof(int));
    slice_to_primitive_array(children, n->children, children->length, sizeof(int));
    return 0;
}

/*
** This is the recursive procedure that inserts the value and information into the tree, updating nodes on its way up.
*/
static int doinsertion(int current, int v, int w, node *A, stack *ancestor_stack) {
    if (is_safe(A)) {
        insert_safe(v, w, A);
        write_node(A->loc, A);
        return 0;
    } else {
        split *pages = split_node(v, w, A);
        int   y      = A->high_key;
        node  test;
        read_node(&test, 0);
        write_node(pages->B->loc, pages->B);
        write_node(current, A);
        if (pages->new_root) {
            write_node(pages->new_root->loc, pages->new_root);
            return 0;
        }
        int old_node = current;
        v       = y;
        w       = pages->B->loc;
        current = *(int *) stack_pop(ancestor_stack);
        // lock current
        read_node(A, current);
        move_right(v, A);
        return doinsertion(current, v, w, A, ancestor_stack);
        // unlock old_node
    }
}

/*
** This is the insert procedure wrapper. Given a value and information, insert it into the tree. This function makes
** use of the recursive doinsertion procedure as well as nearly every procedure above it.
*/
int insert(int v, int w) {
    stack  *ancestor_stack = create_ancestor_stack();
    header h;
    read_header(&h);
    int  current = h.root_loc;
    node A;
    read_node(&A, current);
    while (!A.leaf && scannode(v, &A) != -1) {
        int t = current;
        current = scannode(v, &A);
        if (current != A.link_ptr) stack_push(ancestor_stack, &t);
        if (!A.leaf) current = A.children[current];
        read_node(&A, A.children[current]);
    }
    read_node(&A, current);
    move_right(v, &A);
    int exists = check_key_exists(v, &A);
    if (exists != -1) {
        println(str("unable to insert: key already exists in tree!"));
        return EINVAL;
    }
    return doinsertion(current, v, w, &A, ancestor_stack);
}