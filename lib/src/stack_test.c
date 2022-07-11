#include "stack.h"
#include "stack_test.h"

void test_stack_init() {
    stack *s    = malloc(sizeof(stack));
    int   first = stack_init(s);
    assert(first == 0);
    int second = stack_init(NULL);
    assert(second == EINVAL);
    assert(stack_size(s) == 0);
    assert(s->top == NULL);
    int third = stack_free(s);
    assert(third == 0);
}

void test_stack_push() {
    stack *s = malloc(sizeof(stack));
    stack_init(s);
    int k1 = 1;
    int k2 = 2;
    int k3 = 3;
    assert(s->frame_ct == 0);
    int first = stack_push(s, &k1);
    assert(first == 0);
    assert(stack_size(s) == 1);
    int second = stack_push(s, &k2);
    assert(second == 0);
    assert(stack_size(s) == 2);
    int third = stack_push(s, &k3);
    assert(third == 0);
    assert(stack_size(s) == 3);
    stack_free(s);
}

void test_stack_pop() {
    stack *s = malloc(sizeof(stack));
    stack_init(s);
    int k1 = 1;
    int k2 = 2;
    int k3 = 3;
    assert(s->frame_ct == 0);
    stack_push(s, &k1);
    stack_push(s, &k2);
    stack_push(s, &k3);
    assert(stack_size(s) == 3);
    void *first = stack_pop(s);
    assert(first != NULL);
    assert(*(int *) first == 3);
    assert(stack_size(s) == 2);
    void *second = stack_pop(s);
    assert(second != NULL);
    assert(*(int *) second == 2);
    assert(stack_size(s) == 1);
    void *third = stack_pop(s);
    assert(third != NULL);
    assert(*(int *) third == 1);
    assert(stack_size(s) == 0);
    void *fourth = stack_pop(s);
    assert(fourth == NULL);
    assert(stack_size(s) == 0);
    stack_free(s);
}

void test_stack() {
    test_stack_init();
    test_stack_push();
    test_stack_pop();
}