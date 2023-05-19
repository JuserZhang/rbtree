#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/rbt_api.h"

void destroy_fn(void *private_data) {
    free(private_data);
}

void dump_fn(void *private_data) {
    printf("value: %s\n", (char *)private_data);
}

int main() {
    rbt_root_t *rbt = rbt_init();

    rbt_add(rbt, 10, strdup("ten"), destroy_fn, dump_fn);
    rbt_add(rbt, 20, strdup("twenty"), destroy_fn, dump_fn);
    rbt_add(rbt, 30, strdup("thirty"), destroy_fn, dump_fn);

    //printf("size: %d\n", rbt_size(rbt));
    rbt_dump(rbt);
    char *value = (char *)rbt_find(rbt, 20);
    if (value != NULL) {
        printf("value: %s\n", value);
    }

    rbt_delete(rbt, 20);

    //printf("size: %d\n", rbt_size(rbt));

    rbt_dump(rbt);

    rbt_destroy(rbt);

    return 0;
}