#ifndef LENN_KV_STORE_H_
#define LENN_KV_STORE_H_
#include <unistd.h>
#include <stdio.h>

#define BUFFER_LENGTH		512

#define NETWORK_EPOLL	0
#define NETWORK_COROUTINE	1
#define NETWORK_IOURING		2

#define NETWORK_ENABLE_MODULE	NETWORK_COROUTINE

#define ENABLE_ARRAY_KVENGINE 	1
#define ENABLE_RBTREE_KVENGINE	1

typedef int (*RCALLBACK)(int fd);

// conn, fd, buffer, callback
struct conn_item {
	int fd;
	
	char rbuffer[BUFFER_LENGTH];
	int rlen;
	char wbuffer[BUFFER_LENGTH];
	int wlen;

	union {
		RCALLBACK accept_callback;
		RCALLBACK recv_callback;
	} recv_t;
	RCALLBACK send_callback;
};

typedef struct conn_item conn_item_t;

int lenn_epoll_entry();
int lenn_coroutine_entry();

void* lenn_kv_malloc(size_t size);
void lenn_kv_free(void* ptr);
int lenn_kv_request(conn_item_t* item);

#if ENABLE_ARRAY_KVENGINE
struct lenn_kv_array_s {
    char* key;
    char* val;
};
#define ARRAY_MAX_LENGTH    1024

int lenn_kv_array_set(char* key, char* val);
char* lenn_kv_array_get(char* key);
int lenn_kv_array_del(char* key);
int lenn_kv_array_mod(char* key, char* nval);
int lenn_kv_array_count();

#endif

#if ENABLE_ARRAY_KVENGINE

typedef struct _rbtree_head lenn_kv_rbhead_t;

extern lenn_kv_rbhead_t Tree;

int rbtree_create(lenn_kv_rbhead_t* tree);
int kvs_rbtree_set(lenn_kv_rbhead_t* tree, char* key, char* val);
char* kvs_rbtree_get(lenn_kv_rbhead_t* tree, char* key);
int kvs_rbtree_mod(lenn_kv_rbhead_t* tree, char* key, char* val);
int kvs_rbtree_del(lenn_kv_rbhead_t* tree, char* key);
int kvs_rbtree_count(lenn_kv_rbhead_t* tree);

// int lenn_kv_rbtree_set(char* key, char* val);
// char* lenn_kv_rbtree_get(char* key);
// int lenn_kv_rbtree_mod(char* key, char* val);
// int lenn_kv_rbtree_del(char* key);
// int lenn_kv_rbtree_count();

#endif

#endif