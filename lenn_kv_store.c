#include "lenn_kv_store.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

#define LENN_KV_MAX_TOKEN   128

const char* commands[] = {
    "SET", "GET", "DEL", "MOD", "COUNT",
    "RSET", "RGET", "RDEL", "RMOD", "RCOUNT",
};

enum {
    LENN_KV_CMD_START = 0,
    LENN_KV_CMD_SET = 0,
    LENN_KV_CMD_GET,
    LENN_KV_CMD_DEL,
    LENN_KV_CMD_MOD,
    LENN_KV_CMD_COUNT,

    LENN_KV_CMD_RSET,
    LENN_KV_CMD_RGET,
    LENN_KV_CMD_RDEL,
    LENN_KV_CMD_RMOD,
    LENN_KV_CMD_RCOUNT,
    LENN_KV_CMD_END,
};


void* lenn_kv_malloc(size_t size)
{
    return malloc(size);
}


void lenn_kv_free(void* ptr)
{
    free(ptr);
}

#if ENABLE_RBTREE_KVENGINE

int lenn_kv_rbtree_set(char* key, char* val)
{
    return kvs_rbtree_set(&Tree, key, val);
}

char* lenn_kv_rbtree_get(char* key)
{
    return kvs_rbtree_get(&Tree, key);
}

int lenn_kv_rbtree_mod(char* key, char* val)
{
    return kvs_rbtree_mod(&Tree, key, val);
}

int lenn_kv_rbtree_del(char* key)
{
    return kvs_rbtree_del(&Tree, key);
}

int lenn_kv_rbtree_count() 
{
	return kvs_rbtree_count(&Tree);
}


#endif

int lenn_kv_split_tokens(char* msg, char** tokens)
{
    if(msg == NULL || tokens == NULL) return -1;

    int cnt = 0;
    char* token = strtok(msg, " ");
    while(token != NULL)
    {
        tokens[cnt++] = token;
        token = strtok(NULL, " ");
    }

    return cnt;
}

int lenn_kv_analyze_protocol(conn_item_t* item, char** tokens, int count)
{
    if(item == NULL || tokens == NULL || count == 0) return -1;

    int i = 0;
    for(i = LENN_KV_CMD_START; i < LENN_KV_CMD_END; i++)
    {
        if(strcmp(commands[i], tokens[0]) == 0)
            break;
    }
    int ret = 0;
    switch(i)
    {
        case LENN_KV_CMD_SET:
            ret = lenn_kv_array_set(tokens[1], tokens[2]);
            if(ret < 0)
                snprintf(item->wbuffer, BUFFER_LENGTH, "FAILED");
            else
                snprintf(item->wbuffer, BUFFER_LENGTH, "SUCCESS");
            break;
        case LENN_KV_CMD_GET:
            char* val = lenn_kv_array_get(tokens[1]);
            if(val)
                snprintf(item->wbuffer, BUFFER_LENGTH, "%s", val);
            else
                snprintf(item->wbuffer, BUFFER_LENGTH, "FAILED");
            break;
        case LENN_KV_CMD_DEL:
            ret = lenn_kv_array_del(tokens[1]);
            if(ret < 0)
                snprintf(item->wbuffer, BUFFER_LENGTH, "FAILED");
            else
                snprintf(item->wbuffer, BUFFER_LENGTH, "SUCCESS");
            break;
        case LENN_KV_CMD_MOD:
            ret = lenn_kv_array_mod(tokens[1], tokens[2]);
            if(ret < 0)
                snprintf(item->wbuffer, BUFFER_LENGTH, "FAILED");
            else
                snprintf(item->wbuffer, BUFFER_LENGTH, "SUCCESS");
            break;
        case LENN_KV_CMD_COUNT:
            ret = lenn_kv_array_count();
            snprintf(item->wbuffer, BUFFER_LENGTH, "%d", ret);
            break;
        case LENN_KV_CMD_RSET:
            ret = lenn_kv_rbtree_set(tokens[1], tokens[2]);
            if(ret < 0)
                snprintf(item->wbuffer, BUFFER_LENGTH, "FAILED");
            else
                snprintf(item->wbuffer, BUFFER_LENGTH, "SUCCESS");
            break;
        case LENN_KV_CMD_RGET:
            char* rbval = lenn_kv_rbtree_get(tokens[1]);
            printf("rbval is %s\r\n", rbval);
            if(rbval)
                snprintf(item->wbuffer, BUFFER_LENGTH, "%s", rbval);
            else
                snprintf(item->wbuffer, BUFFER_LENGTH, "FAILED");
            break;
        case LENN_KV_CMD_RDEL:
            ret = lenn_kv_rbtree_del(tokens[1]);
            if(ret < 0)
                snprintf(item->wbuffer, BUFFER_LENGTH, "FAILED");
            else
                snprintf(item->wbuffer, BUFFER_LENGTH, "SUCCESS");
            break;
        case LENN_KV_CMD_RMOD:
            ret = lenn_kv_rbtree_mod(tokens[1], tokens[2]);
            if(ret < 0)
                snprintf(item->wbuffer, BUFFER_LENGTH, "FAILED");
            else
                snprintf(item->wbuffer, BUFFER_LENGTH, "SUCCESS");
            break;
        case LENN_KV_CMD_RCOUNT:
            ret = lenn_kv_rbtree_count();
            snprintf(item->wbuffer, BUFFER_LENGTH, "%d", ret);
            break;
        default:
            snprintf(item->wbuffer, BUFFER_LENGTH, "ERROR COMMAND");
            break;
    }
}

int lenn_kv_request(conn_item_t* item)
{
    char* msg = item->rbuffer;
    char* token[LENN_KV_MAX_TOKEN] = {0};
    int nb_token = lenn_kv_split_tokens(msg, token);
    int idx = 0;
    // for(idx; idx < nb_token; idx++)
    // {
    //     printf("idx: %d,  token: %s\r\n", idx, token[idx]);
    // }

    lenn_kv_analyze_protocol(item, token, nb_token);

    return 0;
}

int init_kvengine()
{
#if ENABLE_ARRAY_KVENGINE

#endif

#if ENABLE_RBTREE_KVENGINE
    rbtree_create(&Tree);
#endif
}

int main()
{

    init_kvengine();

    switch(NETWORK_ENABLE_MODULE)
    {
        case NETWORK_EPOLL:
            lenn_epoll_entry();
            break;
        case NETWORK_COROUTINE:
            lenn_coroutine_entry();
            break;
    }
        

    return 0;
}