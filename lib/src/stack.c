#include "stack.h"

int stack_init(stack *s) {
    if (!s) return EINVAL;
    s->top      = NULL;
    s->frame_ct = 0;
    return 0;
}

static frame *frame_create(void *data) {
    frame *f = malloc(sizeof(frame));
    f->data = data;
    f->next = NULL;
    return f;
}

static int frame_free(frame *f) {
    f->data = f->next = NULL;
    f = NULL;
    free(f);
    return 0;
}

int stack_push(stack *s, void *data) {
    if (!s || !data) return EINVAL;
    frame *new_frame = frame_create(data);
    new_frame->next = s->top;
    s->top          = new_frame;
    s->frame_ct++;
    return 0;
}

void *stack_pop(stack *s) {
    if (!s || s->frame_ct == 0) return NULL;
    frame *popped = s->top;
    s->top = s->top->next;
    void *data = popped->data;
    frame_free(popped);
    s->frame_ct--;
    return data;
}

int stack_size(stack *s) {
    if (!s) return -1;
    return s->frame_ct;
}

int stack_free(stack *s) {
    if (!s) return EINVAL;
    frame *itr = s->top;
    while (itr) {
        frame *tmp = itr;
        itr = itr->next;
        frame_free(tmp);
        s->frame_ct--;
    }
    s->top = NULL;
    s = NULL;
    free(s);
    return 0;
}

string *stack_to_string(stack *s, to_string elem_to_string) {
    if (!s || !elem_to_string) return str("");
    frame  *itr = s->top;
    string *out = str("stack [ ");
    while (itr) {
        append(out, elem_to_string(itr->data), str_string);
        if (itr->next != NULL) append(out, ", ", str_str);
        itr = itr->next;
    }
    append(out, " ]\n", str_str);
    return out;
}