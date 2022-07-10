#ifndef __SLICE__
#define __SLICE__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#include "common.h"
#include "strings.h"

typedef struct slice {
    int       length, capacity;
    void      **keys;
    cmpfunc   compare;
    to_string elem_to_string;
    validator validator;
} slice;

int slice_init(slice *s, cmpfunc compare, to_string elem_to_string, validator validator);
int slice_default(slice *s);
int slice_free(slice *s);
int slice_resize(struct slice *s, int capacity);
int slice_insert(slice *s, void *key);
int slice_append(slice *s, void *key);
int slice_insert_index(struct slice *s, void *key, int index);
int slice_set_index(slice *s, void *key, int index);
int slice_from_ptr_array(slice *s, void *keys, int num_keys);
int slice_from_primitive_array(slice *s, void *keys, int num_keys, size_t key_size);
int slice_join(slice *s1, slice *s2);
int slice_find_index(slice *s, const void *key, cmpfunc override);

void *slice_delete_front(slice *s);
void *slice_delete_back(slice *s);
void *slice_delete_index(slice *s, int index);
void *slice_get_index(slice *s, int index);

struct slice *subslice(struct slice *s, int start, int end);

#endif