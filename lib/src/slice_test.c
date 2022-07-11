#include "slice_test.h"

typedef struct test_slice_struct {
    int  test_id;
    char test_name[50];
} test_slice_struct;

int first_compare_test_slice_struct(const void *first, const void *second) {
    return ((test_slice_struct *) first)->test_id - ((test_slice_struct *) second)->test_id;
}

int second_compare_test_slice_struct(const void *first, const void *second) {
    char     *cast1 = ((test_slice_struct *) first)->test_name;
    char     *cast2 = ((test_slice_struct *) second)->test_name;
    for (int i      = 0; i < strlen(cast1); i++) (void) tolower(cast1[i]);
    for (int i      = 0; i < strlen(cast2); i++) (void) tolower(cast2[i]);
    int      res    = strncmp(cast1, cast2, 50);
    return res;
}

string *first_test_slice_struct_to_string(const void *el) {
    const test_slice_struct *cast = el;
    string                  *s    = str("");
    append(s, (void *) &cast->test_id, str_int);
    return s;
}

string *second_test_slice_struct_to_string(const void *el) {
    const test_slice_struct *cast = el;
    string                  *s    = str("");
    append(s, (void *) &cast->test_name, str_str);
    return s;
}

static test_slice_struct *test_tss(int id) {
    test_slice_struct *tss = malloc(sizeof(test_slice_struct));
    tss->test_id = id;
    strncpy(tss->test_name, "test name here", sizeof(tss->test_name));
    return tss;
}

/*
** this function tests initialization and deallocation of slice objects
*/
void test_slice_init() {
    slice *s = malloc(sizeof(slice));
    slice_default(s);
    assert(s != NULL);
    assert(s->capacity == 10);
    assert(s->length == 0);
    assert(s->keys != NULL);
    int first = slice_free(s);
    assert(s->keys == NULL);
    assert(first == 0);
}

void test_slice_resize() {
    slice *s = malloc(sizeof(slice));
    slice_default(s);
    int first = slice_resize(s, 5);
    assert(first == EINVAL);
    assert(s->capacity == 10);
    int second = slice_resize(s, 15);
    assert(second == 0);
    assert(s->capacity == 15);
    slice_free(s);
}

void test_slice_insert() {
    slice *s = malloc(sizeof(slice));
    slice_default(s);
    int num   = 6132020;
    int first = slice_insert(s, &num);
    assert(first == 0);
    assert(s->length == 1);
    assert(*(int *) s->keys[0] == 6132020);
    int second = slice_free(s);
    assert(second == 0);
    int third = slice_insert(NULL, &num);
    assert(third == EINVAL);
}

void test_slice_append() {
    slice *s = malloc(sizeof(slice));
    slice_default(s);
    int num   = 6132020;
    int first = slice_append(s, &num);
    assert(first == 0);
    assert(s->length == 1);
    assert(*(int *) s->keys[0] == 6132020);
    int second = slice_append(NULL, &num);
    assert(second == EINVAL);
    slice_free(s);
}

void test_slice_delete_front() {
    slice *s = malloc(sizeof(slice));
    slice_default(s);
    int num = 6132020;
    slice_append(s, &num);
    assert(*(int *) s->keys[0] == 6132020);
    assert(s->length == 1);
    void *first = slice_delete_front(s);
    assert(first != NULL);
    assert(*(int *) first == 6132020);
    assert(s->length == 0);
    void *second = slice_delete_front(s);
    assert(second == NULL);
    assert(s->length == 0);
    slice_free(s);
}

void test_slice_delete_back() {
    slice *s = malloc(sizeof(slice));
    slice_default(s);
    int num = 6132020;
    slice_insert(s, &num);
    assert(*(int *) s->keys[0] == 6132020);
    assert(s->length == 1);
    void *first = slice_delete_back(s);
    assert(first != NULL);
    assert(*(int *) first == 6132020);
    assert(s->length == 0);
    void *second = slice_delete_back(s);
    assert(second == NULL);
    assert(s->length == 0);
    slice_free(s);
}

void test_slice_insert_index() {
    slice *s = malloc(sizeof(slice));
    slice_default(s);
    int first_num = 6132020;
    int first     = slice_insert_index(s, &first_num, 0);
    assert(first == 0);
    assert(*(int *) s->keys[0] == 6132020);
    int second_num = 8042020;
    int second     = slice_insert_index(s, &second_num, s->length);
    assert(second == 0);
    assert(*(int *) s->keys[0] == 6132020);
    assert(*(int *) s->keys[1] == 8042020);
    int third_num = 10212020;
    int third     = slice_insert_index(s, &third_num, 1);
    assert(third == 0);
    assert(*(int *) s->keys[0] == 6132020);
    assert(*(int *) s->keys[1] == 10212020);
    assert(*(int *) s->keys[2] == 8042020);
    int fourth_num = 0;
    int fourth     = slice_insert_index(s, &fourth_num, 8);
    assert(fourth == EINVAL);
    slice_free(s);
}

void test_slice_delete_index() {
    slice *s = malloc(sizeof(slice));
    slice_default(s);
    int first_num  = 6132020;
    int second_num = 8042020;
    int third_num  = 10212020;
    slice_insert_index(s, &first_num, 0);
    slice_insert_index(s, &second_num, 0);
    slice_insert_index(s, &third_num, 0);
    assert(s->length == 3);
    void *first = slice_delete_index(s, 1);
    assert(first != NULL);
    assert(*(int *) first == 8042020);
    assert(s->length == 2);
    void *second = slice_delete_index(s, 1);
    assert(second != NULL);
    assert(*(int *) second == 6132020);
    assert(s->length == 1);
    void *third = slice_delete_index(s, 0);
    assert(third != NULL);
    assert(*(int *) third == 10212020);
    void *fourth = slice_delete_index(s, 1);
    assert(fourth == NULL);
    assert(s->length == 0);
    slice_free(s);
}

void test_slice_set_index() {
    slice *s = malloc(sizeof(slice));
    slice_default(s);
    test_slice_struct *tss1 = test_tss(1);
    test_slice_struct *tss2 = test_tss(2);
    test_slice_struct *tss3 = test_tss(3);
    slice_insert_index(s, tss1, 0);
    slice_insert_index(s, tss2, 0);
    slice_insert_index(s, tss3, 0);
    assert((*(test_slice_struct *) s->keys[0]).test_id == 3);
    assert((*(test_slice_struct *) s->keys[1]).test_id == 2);
    assert((*(test_slice_struct *) s->keys[2]).test_id == 1);
    test_slice_struct *new_tss = test_tss(5);
    int               first    = slice_set_index(s, new_tss, 1);
    assert(first == 0);
    assert((*(test_slice_struct *) s->keys[1]).test_id == 5);
    int second = slice_set_index(s, new_tss, 6132020);
    assert(second == EINVAL);
    slice_free(s);
}

void test_slice_get_index() {
    slice *s = malloc(sizeof(slice));
    slice_default(s);
    test_slice_struct *tss1 = test_tss(1);
    test_slice_struct *tss2 = test_tss(2);
    test_slice_struct *tss3 = test_tss(3);
    slice_insert_index(s, tss1, 0);
    slice_insert_index(s, tss2, 0);
    slice_insert_index(s, tss3, 0);
    assert((*(test_slice_struct *) slice_get_index(s, 1)).test_id == 2);
    test_slice_struct *new_tss = test_tss(5);
    int               first    = slice_set_index(s, new_tss, 1);
    assert(first == 0);
    assert((*(test_slice_struct *) slice_get_index(s, 1)).test_id == 5);
    slice_free(s);
}

void test_slice_from_ptr_array() {
    test_slice_struct *t1      = test_tss(1);
    test_slice_struct *t2      = test_tss(2);
    test_slice_struct *t3      = test_tss(3);
    test_slice_struct *t4      = test_tss(4);
    test_slice_struct *t5      = test_tss(5);
    test_slice_struct *ptrs[5] = {t1, t2, t3, t4, t5};
    slice             *s       = malloc(sizeof(slice));
    slice_default(s);
    int first = slice_from_ptr_array(s, ptrs, 5);
    assert(first == 0);
    assert(s->length == 5);
    assert(((test_slice_struct *) s->keys[0])->test_id == 1);
    assert(((test_slice_struct *) s->keys[1])->test_id == 2);
    assert(((test_slice_struct *) s->keys[2])->test_id == 3);
    assert(((test_slice_struct *) s->keys[3])->test_id == 4);
    assert(((test_slice_struct *) s->keys[4])->test_id == 5);
    assert(s->keys[5] == NULL);
    slice_free(s);
}

void test_slice_from_primitive_array() {
    test_slice_struct t1       = {.test_id=1, .test_name="test name here"};
    test_slice_struct t2       = {.test_id=2, .test_name="test name here"};
    test_slice_struct t3       = {.test_id=3, .test_name="test name here"};
    test_slice_struct t4       = {.test_id=4, .test_name="test name here"};
    test_slice_struct t5       = {.test_id=5, .test_name="test name here"};
    test_slice_struct array[5] = {t1, t2, t3, t4, t5};
    slice             *s       = malloc(sizeof(slice));
    int               first    = slice_from_primitive_array(s, array, 5, sizeof(test_slice_struct));
    assert(first == 0);
    assert(s->length == 5);
    assert(((test_slice_struct *) s->keys[0])->test_id == 1);
    assert(((test_slice_struct *) s->keys[1])->test_id == 2);
    assert(((test_slice_struct *) s->keys[2])->test_id == 3);
    assert(((test_slice_struct *) s->keys[3])->test_id == 4);
    assert(((test_slice_struct *) s->keys[4])->test_id == 5);
    assert(s->keys[5] == NULL);
    slice_free(s);
}

void test_subslice() {
    test_slice_struct t1       = {.test_id=1, .test_name="test name here"};
    test_slice_struct t2       = {.test_id=2, .test_name="test name here"};
    test_slice_struct t3       = {.test_id=3, .test_name="test name here"};
    test_slice_struct t4       = {.test_id=4, .test_name="test name here"};
    test_slice_struct t5       = {.test_id=5, .test_name="test name here"};
    test_slice_struct array[5] = {t1, t2, t3, t4, t5};
    slice             *s       = malloc(sizeof(slice));
    slice_from_primitive_array(s, array, 5, sizeof(test_slice_struct));
    slice *first_half  = subslice(s, 0, 3);
    slice *second_half = subslice(s, 3, 5);
    assert(first_half != NULL);
    assert(second_half != NULL);
    assert(first_half->length == 3);
    assert(second_half->length == 2);
    assert(((test_slice_struct *) first_half->keys[0])->test_id == 1);
    assert(((test_slice_struct *) first_half->keys[1])->test_id == 2);
    assert(((test_slice_struct *) first_half->keys[2])->test_id == 3);
    assert(((test_slice_struct *) second_half->keys[0])->test_id == 4);
    assert(((test_slice_struct *) second_half->keys[1])->test_id == 5);
    slice *error_one = subslice(s, -1, 6);
    slice *error_two = subslice(s, 4, 3);
    assert(error_one == NULL);
    assert(error_two == NULL);
    slice_free(s);
}

void test_slice_join() {
    test_slice_struct t1       = {.test_id=1, .test_name="test name here"};
    test_slice_struct t2       = {.test_id=2, .test_name="test name here"};
    test_slice_struct t3       = {.test_id=3, .test_name="test name here"};
    test_slice_struct t4       = {.test_id=4, .test_name="test name here"};
    test_slice_struct t5       = {.test_id=5, .test_name="test name here"};
    test_slice_struct array[5] = {t1, t2, t3, t4, t5};
    slice             *s       = malloc(sizeof(slice));
    slice_from_primitive_array(s, array, 5, sizeof(test_slice_struct));
    slice *first_half  = subslice(s, 0, 3);
    slice *second_half = subslice(s, 3, 5);
    slice_free(s);
    assert(first_half != NULL);
    assert(second_half != NULL);
    assert(first_half->length == 3);
    assert(second_half->length == 2);
    assert(((test_slice_struct *) first_half->keys[0])->test_id == 1);
    assert(((test_slice_struct *) first_half->keys[1])->test_id == 2);
    assert(((test_slice_struct *) first_half->keys[2])->test_id == 3);
    assert(((test_slice_struct *) second_half->keys[0])->test_id == 4);
    assert(((test_slice_struct *) second_half->keys[1])->test_id == 5);
    int first = slice_join(first_half, second_half);
    assert(((test_slice_struct *) first_half->keys[0])->test_id == 1);
    assert(((test_slice_struct *) first_half->keys[1])->test_id == 2);
    assert(((test_slice_struct *) first_half->keys[2])->test_id == 3);
    assert(((test_slice_struct *) first_half->keys[3])->test_id == 4);
    assert(((test_slice_struct *) first_half->keys[4])->test_id == 5);
    assert(first_half->length == 5);
    assert(first == 0);
}

void test_slice_find_index() {
    test_slice_struct *t1 = test_tss(1);
    strncpy(t1->test_name, "b", sizeof(t1->test_name));
    test_slice_struct *t2 = test_tss(3);
    strncpy(t2->test_name, "c", sizeof(t1->test_name));
    test_slice_struct *t3 = test_tss(5);
    strncpy(t3->test_name, "e", sizeof(t1->test_name));
    test_slice_struct *t4 = test_tss(7);
    strncpy(t4->test_name, "g", sizeof(t1->test_name));
    test_slice_struct *t5 = test_tss(9);
    strncpy(t5->test_name, "i", sizeof(t1->test_name));
    test_slice_struct *ptrs[5] = {t1, t2, t3, t4, t5};
    slice             *s       = malloc(sizeof(slice));
    slice_default(s);
    slice_from_ptr_array(s, ptrs, 5);
    test_slice_struct *t6 = test_tss(6);
    strncpy(t6->test_name, "A", sizeof(t6->test_name));
    test_slice_struct *t7 = test_tss(10);
    strncpy(t7->test_name, "d", sizeof(t7->test_name));
    test_slice_struct *t8 = test_tss(0);
    strncpy(t8->test_name, "j", sizeof(t8->test_name));
    int first = slice_find_index(s, t6, NULL);
    assert(first == -1);
    s->compare = first_compare_test_slice_struct;
    int second = slice_find_index(s, t6, NULL);
    assert(second == 3);
    int third  = slice_find_index(s, t7, NULL);
    int fourth = slice_find_index(s, t8, NULL);
    assert(third == 5);
    assert(fourth == 0);
    int fifth = slice_find_index(s, t6, second_compare_test_slice_struct);
    assert(fifth == 0);
    int sixth = slice_find_index(s, t7, second_compare_test_slice_struct);
    assert(sixth == 2);
    int seventh = slice_find_index(s, t8, second_compare_test_slice_struct);
    assert(seventh == 5);
}

void test_slice_to_string() {
    test_slice_struct *t1      = test_tss(1);
    test_slice_struct *t2      = test_tss(2);
    test_slice_struct *t3      = test_tss(3);
    test_slice_struct *t4      = test_tss(4);
    test_slice_struct *t5      = test_tss(5);
    test_slice_struct *ptrs[5] = {t1, t2, t3, t4, t5};
    slice             *s       = malloc(sizeof(slice));
    slice_default(s);
    slice_from_ptr_array(s, ptrs, 5);
    string *slice_string = slice_to_string(s, NULL);
    assert(slice_string == NULL);
    s->elem_to_string = first_test_slice_struct_to_string;
    slice_string = slice_to_string(s, NULL);
    assert(slice_string != NULL);
    // println(slice_string);
    slice_string = slice_to_string(s, second_test_slice_struct_to_string);
    // println(slice_string);
}

typedef struct bloodborne_character {
    int  id;
    char name[50];
} b_c;

b_c *bc(int id, char *name) {
    b_c *b = malloc(sizeof(b_c));
    b->id = id;
    strncpy(b->name, name, sizeof(b->name));
    return b;
}

string *first_test_print_bc(const void *el) {
    const b_c *cast = el;
    string    *s    = str("");
    append(s, (void *) &cast->id, str_int);
    return s;
}

string *second_test_print_bc(const void *el) {
    const b_c *cast = el;
    string    *s    = str("");
    append(s, (void *) &cast->name, str_str);
    return s;
}

int first_test_compare_bc(const void *first, const void *second) {
    const b_c *c1 = first;
    const b_c *c2 = second;
    return c1->id - c2->id;
}

int second_test_compare_bc(const void *first, const void *second) {
    const b_c *c1 = first;
    const b_c *c2 = second;
    return strncmp(c1->name, c2->name, sizeof(c1->name));
}

void test_slice_sort() {
    slice *s = malloc(sizeof(slice));
    slice_default(s);
    b_c *gascoigne          = bc(1, "father gascoigne");
    b_c *amelia             = bc(2, "vicar amelia");
    b_c *shadows            = bc(3, "shadows of yharnam");
    b_c *cleric             = bc(4, "cleric beast");
    b_c *paarl              = bc(5, "darkbeast paarl");
    b_c *rom                = bc(6, "rom, the vacuous spider");
    b_c *one                = bc(7, "the one reborn");
    b_c *amygdala           = bc(8, "amygdala");
    b_c *queen              = bc(9, "yharnam, pthumerian queen");
    b_c *ebrietas           = bc(10, "ebrietas, daughter of the cosmos");
    b_c *emissary           = bc(11, "celestial emissary");
    b_c *micolash           = bc(12, "micolash, host of the nightmare");
    b_c *nurse              = bc(13, "mergo's wet nurse");
    b_c *watchers           = bc(14, "merciless watchers");
    b_c *giant              = bc(15, "undead giant");
    b_c *descendant         = bc(16, "pthumerian descendant");
    b_c *beast              = bc(17, "abhorrent beast");
    b_c *brainsucker        = bc(18, "brainsucker");
    b_c *loran              = bc(19, "loran darkbeast");
    b_c *bsb                = bc(20, "blood-starved beast");
    b_c *gehrman            = bc(21, "gehrman, the first hunter");
    b_c *witches            = bc(22, "witches of hemwick");
    b_c *logarius           = bc(23, "martyr logarius");
    b_c *bloodletting_beast = bc(24, "bloodletting beast");
    b_c *madman             = bc(25, "forgotten madman");
    b_c *keeper             = bc(26, "keeper of the old lords");
    b_c *maria              = bc(27, "lady maria of the astral clocktower");
    b_c *laurence           = bc(28, "laurence, the first vicar");
    b_c *failures           = bc(29, "living failures");
    b_c *silver             = bc(30, "loran silverbeast");
    b_c *ludwig             = bc(31, "ludwig, the holy blade");
    b_c *boar               = bc(32, "maneater boar");
    b_c *moon               = bc(33, "moon presence");
    b_c *orphan             = bc(34, "orphan of kos");
    b_c *elder              = bc(35, "pthumerian elder");
    b_c *watchdog           = bc(36, "watchdog of the old lords");
    b_c *bloody_crow        = bc(37, "the bloody crow of cainhurst");
    b_c *crow               = bc(38, "eileen the crow");
    b_c *olek               = bc(39, "tomb prospector olek");
    b_c *arianna            = bc(40, "arianna");
    b_c *adella             = bc(41, "adella");
    b_c *patches            = bc(42, "patches");
    b_c *alfred             = bc(43, "alfred");
    b_c *annalise           = bc(44, "annalise, queen of the vilebloods");
    b_c *brador             = bc(45, "brador, church assassin");
    b_c *simon              = bc(46, "simon the harrowed");

    b_c *characters[] = {brador, ludwig, failures, arianna, bsb, loran, amelia, keeper, watchdog, gascoigne,
                         bloody_crow, simon, one, micolash, bloodletting_beast, giant, descendant, nurse, queen,
                         shadows, beast, gehrman, brainsucker, moon, boar, olek, cleric, rom, annalise, paarl, logarius,
                         witches, alfred, amygdala, crow, patches, elder, orphan, watchers, emissary, adella, silver,
                         maria, madman, laurence, ebrietas};
    int first         = slice_from_ptr_array(s, characters, 46);
    assert(first == 0);
    assert(s->length == 46);
    assert(s->capacity > s->length);
    int second = slice_sort(s, NULL);
    assert(second == EINVAL);
    s->elem_to_string = second_test_print_bc;
    s->compare        = first_test_compare_bc;
    int third = slice_sort(s, NULL);
    assert(third == 0);
    // println(slice_to_string(s, NULL));
    // println(slice_to_string(s, first_test_print_bc));
    int fourth = slice_sort(s, second_test_compare_bc);
    assert(fourth == 0);
    // println(slice_to_string(s, NULL));
    // println(slice_to_string(s, first_test_print_bc));
    slice_free(s);
}

void test_slice_to_ptr_array() {
    slice *s = malloc(sizeof(slice));
    slice_default(s);
    b_c *gascoigne    = bc(1, "father gascoigne");
    b_c *amelia       = bc(2, "vicar amelia");
    b_c *shadows      = bc(3, "shadows of yharnam");
    b_c *cleric       = bc(4, "cleric beast");
    b_c *paarl        = bc(5, "darkbeast paarl");
    b_c *rom          = bc(6, "rom, the vacuous spider");
    b_c *characters[] = {gascoigne, amelia, shadows, cleric, paarl, rom};
    slice_from_ptr_array(s, characters, 6);
    s->elem_to_string = second_test_print_bc;
    b_c *array[s->length];
    int first         = slice_to_ptr_array(s, array, s->length);
    assert(first == 0);
    // for (int i = 0; i < s->length; i++) println(second_test_print_bc(array[i]));
    int second = slice_to_ptr_array(NULL, NULL, s->length);
    assert(second == EINVAL);
    int third = slice_to_ptr_array(s, array, 0);
    assert(third == EINVAL);
    slice_free(s);
}

void test_slice_to_primitive_array() {
    slice *s = malloc(sizeof(slice));
    slice_default(s);
    b_c characters[] = {{.id=1, .name="father gascoigne"}, {.id=2, .name= "vicar amelia"},
                        {.id=3, .name = "shadows of yharnam"}};
    slice_from_primitive_array(s, characters, 3, sizeof(b_c));
    s->elem_to_string = second_test_print_bc;
    // println(slice_to_string(s, NULL));
    b_c array[s->length];
    int first         = slice_to_primitive_array(s, array, s->length, sizeof(b_c));
    assert(first == 0);
    // for (int i = 0; i < s->length; i++) println(second_test_print_bc(&array[i]));
    int second = slice_to_primitive_array(NULL, NULL, s->length, sizeof(b_c));
    assert(second == EINVAL);
    int third = slice_to_primitive_array(s, array, 0, 0);
    assert(third == EINVAL);
    slice_free(s);
}

void test_slice() {
    test_slice_init();
    test_slice_resize();
    test_slice_insert();
    test_slice_append();
    test_slice_delete_front();
    test_slice_delete_back();
    test_slice_insert_index();
    test_slice_delete_index();
    test_slice_set_index();
    test_slice_get_index();
    test_slice_from_ptr_array();
    test_slice_from_primitive_array();
    test_subslice();
    test_slice_join();
    test_slice_find_index();
    test_slice_to_string();
    test_slice_sort();
    test_slice_to_ptr_array();
    test_slice_to_primitive_array();
}