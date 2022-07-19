/*
** July 10, 2022
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
***********************************************************************************************************************
**
** This file is part of the `slice` module. This part of the module contains implementations of functions used for
** creating, manipulating, and utilizing dynamic slice objects. Slice objects are meant to be a more convenient way to
** work with C arrays, which are, in general, limited in their use. Slices implemented here are fully featured,
** java.lang.ArrayList or std::vector type structures, fully capable of dynamically resizing themselves, among other
** important operations. They may be searched through, sorted efficiently, and inserted into at any point with simple
** function calls.
**
*/

#include "slice.h"

/*
** initialize slice object. the compare and elem_to_string parameters are optional and may be initialized with NULL.
*/
int slice_init(slice *s, cmpfunc compare, to_string elem_to_string, validator validator) {
    if (!s) return EINVAL;
    s->length         = 0;
    s->capacity       = 10;
    s->compare        = compare;
    s->elem_to_string = elem_to_string;
    s->validator      = validator;
    s->keys           = malloc(sizeof(void *) * s->capacity);
    s->keys           = malloc(sizeof(void *) * s->capacity);
    return 0;
}

/*
** create a default slice object with uninitialized function pointers for comparison, validation, and to_string
** functions
*/
int slice_default(slice *s) {
    if (!s) return EINVAL;
    return slice_init(s, NULL, NULL, NULL);
}

/*
** destroy slice object.
*/
int slice_free(slice *s) {
    if (!s) return -1;
    for (int i = 0; i < s->length; i++) {
        s->keys[i] = NULL;
        free(s->keys[i]);
    }
    s->keys = NULL;
    free(s->keys);
    s->compare        = NULL;
    s->elem_to_string = NULL;
    s->capacity       = s->length = 0;
    s = NULL;
    free(s);
    return 0;
}

/*
** resize slice object.
*/
int slice_resize(slice *s, int capacity) {
    // if (!s || capacity == s->length || capacity < 10) return EINVAL;
    if (!s) return EINVAL;
    s->keys     = realloc(s->keys, sizeof(void *) * capacity);
    s->capacity = capacity;
    return 0;
}

/*
** autoresize slice object. no null check.
*/
static void slice_autoresize(slice *s) {
    if (s->length < floor((double) s->capacity / 4) && s->length > 20) slice_resize(s, s->capacity / 2);
    if (s->capacity < 10 && s->length == s->capacity) slice_resize(s, s->capacity * 2 + 1);
    else slice_resize(s, ceil((double) s->capacity * 1.5));
}

/*
** insert a key at the front of a slice
*/
int slice_insert(slice *s, void *key) {
    if (!s || !key || (s->validator && !s->validator(key))) return EINVAL;
    slice_autoresize(s);
    memcpy(&(s->keys[1]), s->keys, sizeof(void *) * s->length);
    s->length++;
    s->keys[0] = key;
    return 0;
}

/*
** append a key at the end of a slice
*/
int slice_append(slice *s, void *key) {
    if (!s || !key || (s->validator && !s->validator(key))) return EINVAL;
    slice_autoresize(s);
    if (*(int *) key == 37) printf("here: %d\n", s->capacity);

    s->keys[s->length++] = key;
    return 0;
}

/*
** delete a key from the front of a slice
*/
void *slice_delete_front(slice *s) {
    if (!s || s->length == 0) return NULL;
    void *key = s->keys[0];
    s->keys[0] = NULL;
    free(s->keys[0]);
    memcpy(s->keys, &(s->keys[1]), sizeof(void *) * (s->length - 1));
    s->length--;
    slice_autoresize(s);
    return key;
}

/*
** delete a key from the back of a slice
*/
void *slice_delete_back(slice *s) {
    if (!s || s->length == 0) return NULL;
    void *key = s->keys[s->length - 1];
    s->keys[s->length - 1] = NULL;
    free(s->keys[s->length - 1]);
    s->length--;
    slice_autoresize(s);
    return key;
}

/*
** insert a key into a slice at a specific index
*/
int slice_insert_index(slice *s, void *key, int index) {
    if (!s || !key || index > s->length || (s->validator && !s->validator(key))) return EINVAL;
    if (index == s->length) return slice_append(s, key);
    if (index == 0) return slice_insert(s, key);
    slice_autoresize(s);
    memcpy(&(s->keys[index + 1]), &(s->keys[index]), sizeof(void *) * (s->length - index));
    s->keys[index] = key;
    s->length++;
    return 0;
}

/*
** delete a key from a specific index within the slice
*/
void *slice_delete_index(slice *s, int index) {
    if (!s || index >= s->length || s->length == 0) return NULL;
    void *key = s->keys[index];
    s->keys[index] = NULL;
    free(s->keys[index]);
    memcpy(&(s->keys[index]), &(s->keys[index + 1]), sizeof(void *) * (s->length - index - 1));
    s->length--;
    return key;
}

/*
** set the value of a key at a certain index within the slice
*/
int slice_set_index(slice *s, void *key, int index) {
    if (!s || !key || (s->validator && !s->validator(key)) || index >= s->length) return EINVAL;
    slice_autoresize(s);
    s->keys[index] = key;
    return 0;
}

/*
** retrieve a key from a certain index within the slice
*/
void *slice_get_index(slice *s, int index) {
    if (index >= s->length) return NULL;
    return s->keys[index];
}

/*
** generate a slice object from an array of pointers
*/
int slice_from_ptr_array(slice *s, void *keys, int num_keys) {
    if (!s || !keys || num_keys < 1) return EINVAL;
    slice_default(s);
    slice_resize(s, (int) ((double) num_keys * 1.5));
    for (int i = 0; i < num_keys; i++) memcpy(&(s->keys[i]), keys + i * sizeof(void *), sizeof(void *));
    s->length = num_keys;
    return 0;
}

/*
** generate a slice object from an array of primitives
*/
int slice_from_primitive_array(slice *s, void *keys, int num_keys, size_t key_size) {
    if (!s || !keys || num_keys < 1) return EINVAL;
    slice_default(s);
    if (num_keys == 0) {
        printf("here\n");
        slice_resize(s, 10);
        return 0;
    }
    slice_resize(s, (int) ((double) num_keys * 1.5));
    for (int i = 0; i < num_keys; i++) {
        void *key = malloc(key_size);
        memcpy(key, keys + i * key_size, key_size);
        s->keys[i] = key;
    }
    s->length = num_keys;
    return 0;
}

/*
** generate a subslice on a half-open interval from within another slice
*/
slice *subslice(slice *s, int start, int end) {
    if (!s || start > s->length || end < 0 || start < 0 || end <= start) return NULL;
    slice *ss = malloc(sizeof(slice));
    slice_default(ss);
    slice_resize(ss, end - start);
    memcpy(ss->keys, &(s->keys[start]), sizeof(void *) * (end - start));
    ss->length         = end - start;
    ss->compare        = s->compare;
    ss->elem_to_string = s->elem_to_string;
    ss->validator      = s->validator;
    return ss;
}

/*
** join together two slice objects. the second is appended to the first
*/
int slice_join(slice *s1, slice *s2) {
    if (!s1 || !s2) return EINVAL;
    if (s1->length + s2->length >= s1->capacity) slice_resize(s1, s1->length + s2->length);
    memcpy(&(s1->keys[s1->length]), s2->keys, sizeof(void *) * s2->length);
    s1->length += s2->length;
    return 0;
}

/*
** find the index at which the key passed in should be inserted. note: the slice must be sorted for this function
** to have well-defined behavior
*/
int slice_find_index(slice *s, const void *key, cmpfunc override) {
    if (!s || !key) return -1;
    cmpfunc compare = override ? override : s->compare;
    if (!compare) return -1;
    int start = 0, end = s->length - 1;
    while (start <= end) {
        int mid = (start + end) / 2;
        if (compare(s->keys[mid], key) == 0) return mid;
        if (compare(s->keys[mid], key) < 0) start = mid + 1;
        else end = mid - 1;
    }
    return end + 1;
}

/*
** join together two slice objects. the second is appended to the first
*/
string *slice_to_string(slice *s, to_string override) {
    to_string elem_to_string = override ? override : s->elem_to_string;
    if (!elem_to_string) return NULL;
    string   *slice_string = str("slice [ ");
    for (int i             = 0; i < s->length; i++) {
        string *el = elem_to_string(s->keys[i]);
        append(slice_string, el, str_string);
        if (i != s->length - 1) append(slice_string, ", ", str_str);
    }
    append(slice_string, " ]\n", str_str);
    return slice_string;
}

/*
** simple polymorphic insertion sort implementation. this is used to optimize the implementation of mergesort.
*/
static int insertion_sort(slice *s, cmpfunc compare) {
    int      j;
    for (int i = 0; i < s->length; i++) {
        void *key = s->keys[i];
        j              = i - 1;
        while (j >= 0 && compare(key, s->keys[j]) <= 0) {
            s->keys[j + 1] = s->keys[j];
            j--;
        }
        s->keys[j + 1] = key;
    }
    return 0;
}

/*
** auxiliary operation used in the mergesort (slice_sort) procedure below
*/
static int merge(slice *s, slice *l, slice *r, cmpfunc compare) {
    int i = 0, j = 0, k = 0;
    while (i < l->length && j < r->length) {
        if (compare(r->keys[j], l->keys[i]) >= 0) {
            slice_set_index(s, l->keys[i], k);
            i++;
        } else {
            slice_set_index(s, r->keys[j], k);
            j++;
        }
        k++;
    }
    while (i < l->length) {
        slice_set_index(s, l->keys[i], k);
        i++, k++;
    }
    while (j < r->length) {
        slice_set_index(s, r->keys[j], k);
        j++, k++;
    }
    return 0;
}

/*
** slightly optimized version of mergesort which uses insertion sort when the length of the slice is smaller than
** a set constant threshold (44 in this case). this is because insertion sort is more efficient of smaller sized
** arrays
*/
int slice_sort(slice *s, cmpfunc override) {
    cmpfunc compare = override ? override : s->compare;
    if (!compare) return EINVAL;
    if (s->length < SORT_RECURSION_THRESHOLD) return insertion_sort(s, compare);
    int   mid = s->length / 2;
    slice *l  = subslice(s, 0, mid);
    slice *r  = subslice(s, mid, s->length);
    slice_sort(l, compare);
    slice_sort(r, compare);
    merge(s, l, r, compare);
    return 0;
}

/*
** convert a slice into a basic pointer array
*/
int slice_to_ptr_array(slice *s, void *array, int array_size) {
    if (!s || !array || array_size < 1) return EINVAL;
    for (int i = 0; i < array_size; i++) memcpy(array + i * sizeof(void *), &s->keys[i], sizeof(void *));
    return 0;
}

/*
** convert a slice into an array of primitives
*/
int slice_to_primitive_array(slice *s, void *array, int array_size, size_t key_size) {
    if (!s || !array || array_size < 1 || key_size < 1) return EINVAL;
    for (int i = 0; i < array_size; i++) memcpy(array + i * key_size, s->keys[i], key_size);
    return 0;
}

/*
** search a slice for a desired value. can pass in a search function to override the default
*/
int slice_search(slice *s, void *key, int start, int end, cmpfunc override) {
    if (!s || !key || start > end || start < 0 || end < 0 || s->length == 0) return -1;
    cmpfunc compare = override ? override : s->compare;
    if (!compare) return -1;
    int mid = (start + end) / 2;
    if (start == mid) {
        if (compare(s->keys[mid], key) == 0) return start;
        return -1;
    }
    if (compare(s->keys[mid], key) == 0) return mid;
    else if (compare(s->keys[mid], key) > 0) return slice_search(s, key, 0, mid, override);
    return slice_search(s, key, mid, end, override);
}