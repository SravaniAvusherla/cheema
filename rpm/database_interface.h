#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <adapters/libuv.h>

typedef struct db_module db_module_t;

typedef void (*db_get_str_cb_fn)(bool result, void *cb_arg, char *value);

db_module_t*
db_module_create(uv_loop_t* loop);
