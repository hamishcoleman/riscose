/* pool.c -- maintain pools of memory ready for a quick dip. */

#include <stdlib.h>
#include <stdio.h>

#include "config.h"
#include "monty.h"
#include "mem.h"
#include "pool.h"

pool *pool_create(size_t size, int maxfree)
{
    pool *p;

    NEW(p);
    p->size = size >= sizeof(pool_block) ? size : sizeof(pool_block);
    p->maxfree = maxfree;
    p->avail = 0;
    p->head = NULL;

    DEBUG(POOL, ("pool_create(%u, %d) returns %x\n", size, maxfree, p));

    return p;
}

void *pool_alloc(pool *p)
{
    pool_block *b;

    if (p->head) {
        b = p->head;
        p->head = b->next;
        p->avail--;
        DEBUG(POOL, ("pool_alloc(%x) returns head %x, %d left\n",
            p, b, p->avail));

        return b;
    }

    b = emalloc(p->size);
    DEBUG(POOL, ("pool_alloc(%x) returns malloc %x\n", p, b));

    return b;
}

void pool_free(pool *p, void *block)
{
    pool_block *b;

    if (p->avail == p->maxfree) {
        efree(block);
        DEBUG(POOL, ("pool_free(%x, %x) frees\n", p, block));
        return;
    }

    (b = block)->next = p->head;
    p->head = b;
    p->avail++;
    DEBUG(POOL, ("pool_free(%x, %x) pushes, %d now avail\n",
        p, block, p->avail));

    return;
}

void pool_destroy(pool *p)
{
    pool_block *b;
    pool_block *next;

    for (b = p->head; b; b = next) {
        next = b->next;
        efree(b);
    }

    efree(p);

    DEBUG(POOL, ("pool_destroy(%x)\n", p));

    return;
}
