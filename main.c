#include <stdio.h>

#include "slice_test.h"
#include "strings_test.h"
#include "stack_test.h"
#include "ioutil_test.h"
#include "b_link_tree_test.h"

#include "b_link_tree.h"

int main() {
    test_slice();
    test_strings();
    test_stack();
    test_ioutil();
    test_b_link_tree();
    return 0;
}
