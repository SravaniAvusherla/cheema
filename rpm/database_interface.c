#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include <hiredis.h>
#include <async.h>
#include <adapters/libuv.h>

#include "database_interface.h"

#define base_ptr(ptr, type, member)  (type*)((char*)ptr - offsetof(type, member))

struct db_module {
   redisAsyncContext* context;
   uv_loop_t* ev_loop;
   bool stopped;
};

typedef struct db_tx {
   db_get_str_cb_fn cb_fn;
   void* cb_arg;
   char* key;
} db_tx_t;

void db_redisDisconnectCallback(const redisAsyncContext *c, int status) {
    if (status != REDIS_OK) {
        printf("Error: %s\n", c->errstr);
	fflush(stdout);
        return;
    }
    printf("Databse is disconnected...\n");
    fflush(stdout);
}

void db_redisGetCallback(redisAsyncContext *c, void *r, void *privdata) {
    redisReply *reply = r;
    if (reply == NULL) return;
    db_tx_t* txn = (db_tx_t*)privdata;
    printf("get key %s : value %s\n", txn->key, reply->str);
    fflush(stdout);
    
    if (txn->cb_fn != NULL) {
      txn->cb_fn(true, txn->cb_arg, reply->str);
    }
    free(txn->key);
    free(txn);
}

void
db_set_string_value(db_module_t* db_module, const char* str, const char* val)
{
   /*size_t size = strlen(str) + 1;
   char* key = malloc(size);
   strncpy(key, str, strlen(str));
   key[size] = '\0';
   
   size = strlen(val) + 1;
   char* key_val = malloc(size);
   strncpy(key_val, val, strlen(val));
   key_val[size] = '\0';*/
   
   printf("set key %s : value %s\n", str, val);
   fflush(stdout);
   redisAsyncCommand(db_module->context, NULL, NULL, "SET %s %s", str, val);
}

void
db_get_string_value(db_module_t* db_module, const char* str, db_get_str_cb_fn cb, void* cb_arg)
{
   db_tx_t* txn = calloc(1, sizeof(*txn));
   txn->cb_fn = cb;
   txn->cb_arg = cb_arg;
   
   size_t size = strlen(str) + 1;
   txn->key = malloc(size);
   strncpy(txn->key, str, strlen(str));
   txn->key[size] = '\0';

   redisAsyncCommand(db_module->context, db_redisGetCallback, (char*)txn, "GET %s", txn->key, strlen(txn->key));
}

void db_redisConnectCallback(const redisAsyncContext *c, int status) {
    (void)c;
    if (status != REDIS_OK) {
        printf("Error: %s\n", c->errstr);
	fflush(stdout);
        return;
    }
    printf("Database is connected...\n");
    fflush(stdout);
}

bool
db_is_connected(db_module_t* db_module)
{
   return true;
}

db_module_t*
db_module_create(uv_loop_t* loop)
{
   db_module_t* db_module = calloc(1, sizeof(*db_module));
   db_module->stopped = true;
   db_module->ev_loop = loop;
   
   redisAsyncContext* c = redisAsyncConnect("10.96.4.155", 6379);
   if (c->err) {
        /* Let *c leak for now... */
        printf("Error: %s\n", c->errstr);
	fflush(stdout);
        return NULL;
   }
   db_module->context = c;
   redisLibuvAttach(db_module->context, loop);
   redisAsyncSetConnectCallback(db_module->context, db_redisConnectCallback);
   redisAsyncSetDisconnectCallback(db_module->context, db_redisDisconnectCallback);
   printf("db module is created\n");
   fflush(stdout);
   return db_module;
}

void
db_module_destroy(db_module_t* db_module)
{
   redisAsyncDisconnect(db_module->context);
   // TODO: free later to disconnect cb
   free(db_module);
}
