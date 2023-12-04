#include "lenn_kv_store.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#define LENN_KV_MAX_TOKEN   128

const char* commands = {
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

    switch(i)
    {
        case LENN_KV_CMD_SET:
            printf("cmd: set\r\n");
            break;
        case LENN_KV_CMD_GET:
            printf("cmd: get\r\n");
            break;
        case LENN_KV_CMD_DEL:
            printf("cmd: del\r\n");
            break;
        case LENN_KV_CMD_MOD:
            printf("cmd: mod\r\n");
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

}

int main()
{
    reactor_entry();

    return 0;
}