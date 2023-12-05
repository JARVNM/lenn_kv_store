#include "lenn_kv_store.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#define LENN_KV_MAX_TOKEN   128

const char* commands[] = {
    "SET", "GET", "DEL", "MOD"
};

enum {
    LENN_KV_CMD_START = 0,
    LENN_KV_CMD_SET = 0,
    LENN_KV_CMD_GET,
    LENN_KV_CMD_DEL,
    LENN_KV_CMD_MOD,
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
            printf("cmd: set\r\n");
            ret = lenn_kv_array_set(tokens[1], tokens[2]);
            if(ret < 0)
                snprintf(item->wbuffer, BUFFER_LENGTH, "FAILED");
            else
                snprintf(item->wbuffer, BUFFER_LENGTH, "SUCCESS");
            break;
        case LENN_KV_CMD_GET:
            printf("cmd: get\r\n");
            char* val = lenn_kv_array_get(tokens[1]);
            if(val)
                snprintf(item->wbuffer, BUFFER_LENGTH, "value is %s", val);
            else
                snprintf(item->wbuffer, BUFFER_LENGTH, "FAiled");
            break;
        case LENN_KV_CMD_DEL:
            printf("cmd: del\r\n");
            ret = lenn_kv_array_del(tokens[1]);
            if(ret < 0)
                snprintf(item->wbuffer, BUFFER_LENGTH, "FAILED");
            else
                snprintf(item->wbuffer, BUFFER_LENGTH, "SUCCESS");
            break;
        case LENN_KV_CMD_MOD:
            printf("cmd: mod\r\n");
            ret = lenn_kv_array_mod(tokens[1], tokens[2], tokens[3]);
            if(ret < 0)
                snprintf(item->wbuffer, BUFFER_LENGTH, "FAILED");
            else
                snprintf(item->wbuffer, BUFFER_LENGTH, "SUCCESS");
            break;
        default:
            assert(0);
    }
}

int lenn_kv_request(conn_item_t* item)
{
    char* msg = item->rbuffer;
    char* token[LENN_KV_MAX_TOKEN] = {0};
    int nb_token = lenn_kv_split_tokens(msg, token);
    int idx = 0;
    for(idx; idx < nb_token; idx++)
    {
        printf("idx: %d,  token: %s\r\n", idx, token[idx]);
    }

    lenn_kv_analyze_protocol(item, token, nb_token);

    return 0;
}

int main()
{
    reactor_entry();

    return 0;
}