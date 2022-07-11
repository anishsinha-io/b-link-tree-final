#include <stdio.h>

#include "slice.h"
#include "slice_test.h"
#include "string.h"
#include "strings_test.h"
#include "stack.h"
#include "stack_test.h"

int main() {
    test_slice();
    test_strings();
    test_stack();
    return 0;
}
