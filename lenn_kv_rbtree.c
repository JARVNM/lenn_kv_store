
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lenn_kv_store.h"

#define RED     1
#define BLACK   2

#define ENABLE_KEY_CHAR 1

#define MAX_KEY_LEN     256
#define MAX_VALUE_LEN   1024

#if ENABLE_KEY_CHAR
typedef char* KEY_TYPE;
#else
typedef int* KEY_TYPE;
#endif

typedef struct _rbtree_node
{
    unsigned char color;
    struct _rbtree_node* parent;
    struct _rbtree_node* left;
    struct _rbtree_node* right;

    KEY_TYPE key;
    void* val;
} lenn_kv_rbnode_t;

typedef struct _rbtree_head
{
    int count;
    lenn_kv_rbnode_t* root;
    lenn_kv_rbnode_t* nil;
} lenn_kv_rbhead_t;

lenn_kv_rbnode_t* rbtree_mini(lenn_kv_rbhead_t* T, lenn_kv_rbnode_t* x)
{
    while(x->left != T->nil)
        x = x->left;

    return x;
}

lenn_kv_rbnode_t* rbtree_maxi(lenn_kv_rbhead_t* T, lenn_kv_rbnode_t* x)
{
    while(x->right != T->nil)
        x = x->right;

    return x;
}


lenn_kv_rbnode_t* rbtree_successor(lenn_kv_rbhead_t* T, lenn_kv_rbnode_t* x)
{
    if(x->right != T->nil)
    {
        return rbtree_mini(T, x->right);
    }

    lenn_kv_rbnode_t* y = x->parent;
    while((x == y->right) && (y != T->nil))
    {
        x = y;
        y = y->parent;
    }

    return y;
}

void rbtree_left_rotate(lenn_kv_rbhead_t* T, lenn_kv_rbnode_t* x)
{
    lenn_kv_rbnode_t* y = x->right;
    
    x->right = y->left;
    if(y->left != T->nil)
        y->left->parent = x;

    y->parent = x->parent;
    if(x->parent == T->nil)
        T->root = y;
    else if(x == x->parent->right)
        x->parent->right = y;
    else
        x->parent->left = y;

    y->left = x;
    x->parent = y;
}

void rbtree_right_rotate(lenn_kv_rbhead_t* T, lenn_kv_rbnode_t* y)
{
    lenn_kv_rbnode_t* x = y->left;

    y->left = x->right;
    if(x->right != T->nil)
        x->right->parent = y;
        
    x->parent = y->parent;
    if(y->parent == T->nil)
        T->root = x;
    else if(y == y->parent->left)
        y->parent->left = x;
    else
        y->parent->right = x;

    x->right = y;
    y->parent = x;
}

void rbtree_insert_fixup(lenn_kv_rbhead_t* T, lenn_kv_rbnode_t* z)
{
    while(z->parent->color == RED)
    {
        if(z->parent == z->parent->parent->left)
        {
            lenn_kv_rbnode_t* y = z->parent->parent->right;
            if(y->color == RED)
            {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else
            {
                if(z == z->parent->right)
                {
                    z = z->parent;
                    rbtree_left_rotate(T, z);
                }

                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rbtree_right_rotate(T, z->parent->parent);
            }
        }
        else
        {
            lenn_kv_rbnode_t* y = z->parent->parent->left;
            if(y->color == RED)
            {
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                y->color = BLACK;
                z = z->parent->parent;
            }
            else
            {
                if(z == z->parent->left)
                {
                    z = z->parent;
                    rbtree_right_rotate(T, z);
                }

                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rbtree_left_rotate(T, z->parent->parent);
            }
        }
    }

    T->root->color = BLACK;
}

void rbtree_insert(lenn_kv_rbhead_t* T, lenn_kv_rbnode_t* z)
{
    lenn_kv_rbnode_t* x = T->root;
    lenn_kv_rbnode_t* y = T->nil;

#ifdef ENABLE_KEY_CHAR
    while(x != T->nil)
    {
        y = x;
        if(strcmp(z->key, x->key) < 0)
            x = x->left;
        else if(strcmp(z->key, x->key) > 0)
            x = x->right;
        else
            return;
    }
#else
    while(x != T->nil)
    {
        y = x;
        if(z->key < x->key)
            x = x->left;
        else if(z->key > x->key)
            x = x->right;
        else
            return;
    }
#endif

    z->parent = y;
    if(y == T->nil)
        T->root = z;
#ifdef ENABLE_KEY_CHAR
    else if(strcmp(z->key, y->key) < 0)
        y->left = z;
    else
        y->right = z;
#else
    if(z->key < y->key)
        y->left = z;
    else
        y->right = z;
#endif

    z->color = RED;
    z->left = T->nil;
    z->right = T->nil;
    rbtree_insert_fixup(T, z);
}

void rbtree_delete_fixup(lenn_kv_rbhead_t* T, lenn_kv_rbnode_t* x)
{
    while ((x != T->root) && (x->color == BLACK)) {
		if (x == x->parent->left) {

			lenn_kv_rbnode_t *w= x->parent->right;
			if (w->color == RED) {
				w->color = BLACK;
				x->parent->color = RED;

				rbtree_left_rotate(T, x->parent);
				w = x->parent->right;
			}

			if ((w->left->color == BLACK) && (w->right->color == BLACK)) {
				w->color = RED;
				x = x->parent;
			} else {

				if (w->right->color == BLACK) {
					w->left->color = BLACK;
					w->color = RED;
					rbtree_right_rotate(T, w);
					w = x->parent->right;
				}

				w->color = x->parent->color;
				x->parent->color = BLACK;
				w->right->color = BLACK;
				rbtree_left_rotate(T, x->parent);

				x = T->root;
			}

		} else {

			lenn_kv_rbnode_t *w = x->parent->left;
			if (w->color == RED) {
				w->color = BLACK;
				x->parent->color = RED;
				rbtree_right_rotate(T, x->parent);
				w = x->parent->left;
			}

			if ((w->left->color == BLACK) && (w->right->color == BLACK)) {
				w->color = RED;
				x = x->parent;
			} else {

				if (w->left->color == BLACK) {
					w->right->color = BLACK;
					w->color = RED;
					rbtree_left_rotate(T, w);
					w = x->parent->left;
				}

				w->color = x->parent->color;
				x->parent->color = BLACK;
				w->left->color = BLACK;
				rbtree_right_rotate(T, x->parent);

				x = T->root;
			}

		}
	}

	x->color = BLACK;
}

lenn_kv_rbnode_t* rbtree_delete(lenn_kv_rbhead_t* T, lenn_kv_rbnode_t* z)
{
    lenn_kv_rbnode_t* x = T->nil;
    lenn_kv_rbnode_t* y = T->nil;

    if((z->left == T->nil) || (z->right == T->nil))
        y = z;
    else
        y = rbtree_successor(T, z);

    if(y->left != T->nil)
        x = y->left;
    else if(y->right != T->nil)
        x = y->right;
    
    x->parent = y->parent;
    if(y->parent == T->nil)
        T->root = x;
    else if(y == y->parent->left)
        y->parent->left = x;
    else
        y->parent->right = x;

#ifdef ENABLE_KEY_CHAR
    if(y != x)
    {
        void* tmp = z->key;
        z->key = y->key;
        y->key = tmp;

        tmp = z->val;
        z->val = y->val;
        y->val = tmp;
    }
#else
    z->key = y->key;
    z->val = y->val;
#endif


    if(y->color == BLACK)
        rbtree_delete_fixup(T, x);


    return y;
}

lenn_kv_rbnode_t* rbtree_search(lenn_kv_rbhead_t* T, KEY_TYPE key)
{
    lenn_kv_rbnode_t* tmp = T->root;

    while(tmp != T->nil)
    {
#ifdef ENABLE_KEY_CHAR
        if(strcmp(key, tmp->key) < 0)
            tmp = tmp->left;
        else if(strcmp(key, tmp->key) > 0)
            tmp = tmp->right;
        else
            return tmp;
#else
        if(key < tmp->key)
            tmp = tmp->left;
        else if(key > tmp->key)
            tmp = tmp->right;
        else
            return tmp;
#endif
    }

    return T->nil;
}

void rbtree_traversal(lenn_kv_rbhead_t* T, lenn_kv_rbnode_t* node)
{
    if(node != T->nil)
    {
        rbtree_traversal(T, node->left);
        printf("key:%s. color:%d\r\n", node->key, node->color);
        rbtree_traversal(T, node->right);
    }
}

int rbtree_create(lenn_kv_rbhead_t* tree)
{
    if(!tree) return -1;
    memset(tree, 0, sizeof(tree));

    tree->nil = (lenn_kv_rbnode_t*)malloc(sizeof(lenn_kv_rbnode_t));
    tree->nil->key = malloc(1);
    *(tree->nil->key) = '\0';

    tree->nil->color = BLACK;
    tree->root = tree->nil;

    tree->count = 0;
    return 0;
}


void rbtree_destroy(lenn_kv_rbhead_t* tree)
{
    if(!tree) return;

    if(tree->nil->key)
        lenn_kv_free(tree->nil->key);
    
    lenn_kv_rbnode_t* node = tree->root;
    while(node != tree->nil)
    {
        node = rbtree_mini(tree, tree->root);
        if(node == tree->nil)
            break;

        node = rbtree_delete(tree, node);
        if(node)
        {
            lenn_kv_free(node->key);
            lenn_kv_free(node->val);
            lenn_kv_free(node);
        }
    }

}

int kvs_rbtree_set(lenn_kv_rbhead_t* tree, char* key, char* val)
{
    lenn_kv_rbnode_t* node = (lenn_kv_rbnode_t*)lenn_kv_malloc(sizeof(lenn_kv_rbhead_t));
    if(!node)
        return -1;

    node->key = lenn_kv_malloc(strlen(key) + 1);
    if(!node->key)
    {
        lenn_kv_free(node);
        return -1;
    }
        
    node->val = lenn_kv_malloc(strlen(val) + 1);
    if(!node->val)
    {
        lenn_kv_free(node->key);
        lenn_kv_free(node);
        return -1;
    }
        
    memset(node->key, 0, strlen(key) + 1);
    memset(node->val, 0, strlen(val) + 1);
    strcpy(node->key, key);
    strcpy((char*)node->val, val);
    rbtree_insert(tree, node);
    tree->count++;

    return 0;
}

char* kvs_rbtree_get(lenn_kv_rbhead_t* tree, char* key)
{
    lenn_kv_rbnode_t* node = rbtree_search(tree, key);
    printf("node val is %s\r\n", node->val);
    if(node == tree->nil)
    {
        return NULL;
    }

    return node->val;
}

int kvs_rbtree_mod(lenn_kv_rbhead_t* tree, char* key, char* val)
{
    lenn_kv_rbnode_t* node = rbtree_search(tree, key);
    if(node == tree->nil)   return -1;

    char* old = node->val;
    lenn_kv_free(old);

    node->val = lenn_kv_malloc(strlen(val) + 1);
    if(!node->val) return -1;
    strcpy(node->val, val);
    return 0;
}

int kvs_rbtree_del(lenn_kv_rbhead_t* tree, char* key)
{
    lenn_kv_rbnode_t* node = rbtree_search(tree, key);
    if(node == tree->nil) return -1;

    lenn_kv_rbnode_t* cur = rbtree_delete(tree, node);
    if(!cur)
    {
        lenn_kv_free(node->key);
        lenn_kv_free(node->val);
        lenn_kv_free(node);
    }

    tree->count--;

    return 0;
}

int kvs_rbtree_count(lenn_kv_rbhead_t* tree)
{
    return tree->count;
}

lenn_kv_rbhead_t Tree;