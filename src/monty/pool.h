/* pool.h -- maintain pools of memory ready for a quick dip. */

/* $Id$ */

typedef struct pool_block {
    struct pool_block *next;
} pool_block;

typedef struct {
    /* the size of each block of memory in the pool. */
    size_t size;
    /* the maximum number of free blocks to have sitting by the pool
     * before some of them get free'd. */
    int maxfree;
    /* the number of free blocks currently by the pool. */
    int avail;
    /* pointer to the first block in the pool. */
    pool_block *head;
} pool;

/* creates a pool, the blocks of memory will be size bytes big, there
 * will not be more than maxfree blocks available (excess ones will be
 * returned to the heap. */
pool *pool_create(size_t size, int maxfree);

/* returns a block from the pool. */
void *pool_alloc(pool *p);

/* return a block to the pool. */
void pool_free(pool *p, void *block);

/* releases all memory used by the blocks in the pool and the pool
 * itself. */
void pool_destroy(pool *p);
