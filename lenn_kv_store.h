#ifndef LENN_KV_STORE_H_
#define LENN_KV_STORE_H_

#define BUFFER_LENGTH		512

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

struct lenn_kv_array_s {
    char* key;
    char* val;
}
#define ARRAY_MAX_LENGTH    1024

typedef struct conn_item conn_item_t;

int reactor_entry();


void* lenn_kv_malloc(size_t size);
void lenn_kv_free(void* ptr);

int lenn_kv_request(conn_item_t* item);

int lenn_kv_array_set(char* key, char* val);
char* lenn_kv_array_get(char* key);
int lenn_kv_array_del(char* key);
int lenn_kv_array_mod(char* key, char* oval, char* nval);
#endif