#include <stdio.h>
#include <assert.h>
#include <malloc.h>

#if !defined(uint16_t)
typedef unsigned short uint16_t;
#endif

typedef struct snode_s {
  int key_;
  struct snode_s* next_;
} snode_t;

typedef struct dnode_s {
  int key_;
  struct dnode_s* next_;
  struct dnode_s* prev_;
} dnode_t;

#define snode_NIL (-1)
#define dnode_NIL (-1)

int make_slist(snode_t* sentinel)
{
    if (!sentinel) return -1;
    sentinel->key_ = snode_NIL;
    sentinel->next_ = sentinel;
    return 0;
}

int make_dlist(dnode_t* sentinel)
{
    if (!sentinel) return -1;
    sentinel->key_ = dnode_NIL;
    sentinel->next_ = sentinel;
    sentinel->prev_ = sentinel;
    return 0;
}

int slist_insert(snode_t* sentinel, snode_t* x)
{
    if (!sentinel || !x) return -1;
    x->next_ = sentinel->next_;
    sentinel->next_ = x;
    return 0;
}

int dlist_insert(dnode_t* sentinel, dnode_t* x)
{
    if (!sentinel || !x) return -1;
    x->next_ = sentinel->next_;
    x->prev_ = sentinel;
    sentinel->next_->prev_ = x;
    sentinel->next_ = x;
    return 0;
}

int dlist_insert_tail(dnode_t* sentinel, dnode_t* x)
{
    if (!sentinel || !x) return -1;
    x->next_ = sentinel;
    x->prev_ = sentinel->prev_;
    sentinel->prev_->next_ = x;
    sentinel->prev_ = x;
    return 0;
}

int is_slist_empty(const snode_t* sentinel)
{
    return (sentinel->next_ == sentinel);
}

int is_dlist_empty(const dnode_t* sentinel)
{
    return (sentinel->next_ == sentinel);
}

void slist_print(const snode_t* sentinel)
{
    snode_t* n = sentinel->next_;
    printf("sentinel");
    while (n != sentinel) {
        printf(" --> %d", n->key_);
        n = n->next_;
    }
    printf("\n");
}

void dlist_print(const dnode_t* sentinel)
{
    dnode_t* n = sentinel->next_;
    printf("sentinel");
    while (n != sentinel) {
        printf(" --> %d", n->key_);
        n = n->next_;
    }
    printf("\n");
}

snode_t* slist_pop(snode_t* sentinel)
{
    snode_t* first = NULL;
    if (!is_slist_empty(sentinel)) {
        first = sentinel->next_;
        sentinel->next_ = first->next_;
    }
    return first;
}

dnode_t* dlist_pop(dnode_t* sentinel)
{
    dnode_t* first = NULL;
    if (!is_dlist_empty(sentinel)) {
        first = sentinel->next_;
        first->next_->prev_ = sentinel;
        sentinel->next_ = first->next_;
    }
    return first;
}

void slist_reverse(snode_t* sentinel)
{
    snode_t* n = NULL;
    snode_t* last = NULL;
    snode_t local_list;
    make_slist(&local_list);
    last = slist_pop(sentinel);
    if (!last) return; //list was empty to start with
    slist_insert(&local_list, last);
    while ((n = slist_pop(sentinel)) != NULL) {
        slist_insert(&local_list, n);
    } 
    //now patch the pointer in sentinel and the last one
    sentinel->next_ = local_list.next_;
    last->next_ = sentinel;
}

int slist_insert_key(snode_t* sentinel, int key)
{
    snode_t* x = (snode_t*)malloc(sizeof(snode_t));
    x->key_ = key;
    return slist_insert(sentinel, x);
}

int dlist_insert_key(dnode_t* sentinel, int key)
{
    dnode_t* x = (dnode_t*)malloc(sizeof(dnode_t));
    x->key_ = key;
    return dlist_insert_tail(sentinel, x);
}

int main()
{
    int i = 0;
    {
    snode_t l1;
    assert(make_slist(&l1) == 0);
    for (; i<10; i++) {
        assert(slist_insert_key(&l1, i*2) == 0);
    }
    slist_print(&l1);
    slist_reverse(&l1);
    slist_print(&l1);
    }
    {
    dnode_t l1;
    assert(make_dlist(&l1) == 0);
    for (i=0; i<10; i++) {
        assert(dlist_insert_key(&l1, i*2) == 0);
    }
    dlist_print(&l1);
    dnode_t *first = NULL;
    while ((first = dlist_pop(&l1))) {
        printf("After pop: %d\n", first->key_);
        dlist_print(&l1);
    }
    }
    return 0;
}

