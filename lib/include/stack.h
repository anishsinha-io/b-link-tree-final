#ifndef __STACK__
#define __STACK__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#include "common.h"
#include "strings.h"

typedef struct frame {
    void         *data;
    struct frame *next;
} frame;

typedef struct stack {
    frame *top;
    int   frame_ct;
} stack;

int stack_init(stack *s);
int stack_push(stack *s, void *data);
int stack_size(stack *s);
int stack_free(stack *s);

void *stack_pop(stack *s);

#endif