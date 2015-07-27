/**
 * @file
 * Simple union find demo program
 */  

#include <assert.h>

#include "union_find.h"

int main(int argc, char **argv)
{
    int i1=1, i2=2, i3=3;

    forest_node_t *s1=make_set(&i1), *s2=make_set(&i2), *s3=make_set(&i3);

    assert(find_set(s1) == s1);
    union_sets(s1, s2);

    assert(find_set(s1) == find_set(s2));
    assert(find_set(s1) != find_set(s3));
    union_sets(s2, s3);

    assert(find_set(s1) == find_set(s2) &&
           find_set(s1) == find_set(s3));

    return 0;
}

