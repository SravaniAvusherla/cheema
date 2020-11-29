#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include <adapters/libuv.h>

#include "database_interface.h"

typedef struct rpm {
  uv_loop_t* loop;
  db_module_t* db_module;
} rpm_t;

static rpm_t*
rpm_create()
{
   rpm_t* rpm = calloc(1, sizeof(*rpm));
   rpm->loop = uv_default_loop();
   rpm->db_module = db_module_create(rpm->loop);

   if (rpm->db_module != NULL) {
     db_set_string_value(rpm->db_module, "rpm:dbtest", "5");
     db_get_string_value(rpm->db_module, "rpm:dbtest", NULL, NULL);
   }
   return rpm;
}

static void
rpm_run(uv_loop_t* loop)
{
  while (1) {
    fflush(stdout);
    uv_run(loop, UV_RUN_DEFAULT);
  }
}

int main (int argc, char **argv) {
#ifndef _WIN32
    signal(SIGPIPE, SIG_IGN);
#endif
    rpm_t* rpm = rpm_create();
    printf("rpm created\n");
    fflush(stdout);
    rpm_run(rpm->loop);
    return 0;
}
