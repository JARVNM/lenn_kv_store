#include "lenn_kv_store.h"
#include <stdio.h>
#include <string.h>

typedef struct lenn_kv_array_s lenn_kv_array_t;

lenn_kv_array_t array_table[ARRAY_MAX_LENGTH] = {0};
int array_cnt = 0;
int array_idx = 0;

int lenn_kv_array_set(char* key, char* val)
{

    if(key == NULL || val == NULL) return -1;
    char* kcopy = lenn_kv_malloc(strlen(key) + 1);
    if(kcopy == NULL)
    {
        return -1;
    }
    strncpy(kcopy, key, strlen(key) + 1);
    char* vcopy = lenn_kv_malloc(strlen(val) + 1);
    if(vcopy == NULL)
    {
        lenn_kv_free(kcopy);
        return -1;
    }
    strncpy(vcopy, val, strlen(val) + 1);

    int i = 0;
#if 0
    while(i < ARRAY_MAX_LENGTH)
    {
        if(array_table[i].key == NULL && array_table[i].val == NULL)
        {
            array_table[i].key = kcopy;
            array_table[i].val = vcopy;
            array_cnt++;
            break;
        }
    }
#endif
    for(i = 0; i < array_idx; i++)
    {
        if(array_table[i].key == NULL)
        {
            array_table[i].key = kcopy;
            array_table[i].val = vcopy;
            array_cnt++;
            return 0;
        }
    }

    if(i < ARRAY_MAX_LENGTH && i == array_idx)
    {
        array_table[i].key = kcopy;
        array_table[i].val = vcopy;
        array_cnt++;
        array_idx++;
    }

    return 0;

}
char* lenn_kv_array_get(char* key)
{
    int i = 0;
    for(i = 0; i < array_cnt; i++)
    {
        if(strcmp(array_table[i].key, key) == 0)
        {
            return array_table[i].val;
        }
    }

    return NULL;
}

int lenn_kv_array_del(char* key) 
{
    char* cval = lenn_kv_array_get(key);
    if(cval == NULL)
        return -1;
    int i = 0;
    for(i = 0; i < array_cnt; i++)
    {
        if(strcmp(key, array_table[i].key) == 0)
            break;
    }
    lenn_kv_free(array_table[i].key);
    lenn_kv_free(array_table[i].val);
    array_table[i].key = NULL;
    array_table[i].val = NULL;
#if 0
    int j = i;
    for(j = i; j < j - 1; j++)
    {
        array_table[j].key = array_table[j + 1].key;
        array_table[j].val = array_table[j + 1].val;
    }

    array_table[array_cnt-1].key = NULL;
    array_table[array_cnt-1].val = NULL;
#endif
    array_cnt--;

    return 0;
}

int lenn_kv_array_mod(char* key, char* nval)
{
    int i = 0;
    for(i = 0; i < array_cnt; i++)
    {
        if(strcmp(array_table[i].key, key) == 0)
        {
            lenn_kv_free(array_table[i].val);
            char* vcopy = lenn_kv_malloc(strlen(nval) + 1);
            strncpy(vcopy, nval, strlen(nval) + 1);
            array_table[i].val = vcopy;
            return 0;
        }
    }

    return -1;

}

int lenn_kv_array_count()
{
    return array_cnt;
}