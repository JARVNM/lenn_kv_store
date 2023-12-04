#include "lenn_kv_store.h"
#include <stdio.h>
#include <string.h>

typedef struct lenn_kv_array_s lenn_kv_array_t;

lenn_kv_array_t array_table[ARRAY_MAX_LENGTH] = {0};
int array_cnt = 0;

int lenn_kv_array_set(char* key, char* val)
{
    if(key == NULL || val == NULL || array_cnt == NULL) return -1;

    char* kcopy = lenn_kv_malloc(strlen(key) + 1);
    if(kcopy == NULL)
    {
        kcopy = NULL;
        return -1;
    }
    strncpy(kcopy, key, strlen(key) + 1);

    char* vcopy = lenn_kv_malloc(strlen(val) + 1);
    if(vcopy == NULL)
    {
        lenn_kv_free(kcopy);
        kcopy = NULL;
        vcopy = NULL;
    }
    strncpy(vcopy, key, strlen(val) + 1);

    array_table[array_cnt].key = kcopy;
    array_table[array_cnt].val = vcopy;
    array_cnt++;
}
char* lenn_kv_array_get(char* key)
{

}
int lenn_kv_array_del(char* key);
int lenn_kv_array_mod(char* key, char* oval, char* nval);