/* Inadequate program for testing heap.c */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <monty/monty.h>
#include <monty/mem.h>
#include "heap.h"

#define HEAP_SIZE 1024

typedef struct {
    BYTE *data;
    ULONG size;
    BYTE content;
} test;

int main(void)
{
    test block[100];
    test *b;
    heap_t *h;
    int size;
    BYTE *p;
    BYTE *new;

    debugf = verbosef = stderr;

    for (b = block; b < END(block); b++) {
        b->data = NULL;
        b->size = 0;
        b->content = 0;
    }

    h = emalloc(HEAP_SIZE);
    heap_init(h, HEAP_SIZE);

    while (1) {
        b = block + rand() % DIM(block);
        size = ((rand() % (HEAP_SIZE / 8)) + 3) & ~3;

        debug("block %2d, size %3d", b - block, size);

        if (b->data) {
            for (p = b->data; p < b->data + b->size; p++) {
                if (*p != b->content) {
                    error("content wrong: block %p byte %p was "
                        "%ud not %ud\n", b->data, p, *p, b->content);
                }
            }

            switch (rand() % 2) {
            case 0:
                debug(", free\n");
                heap_block_free(h, b->data);
                b->data = NULL;
                break;
            case 1:
                if ((new = heap_block_resize(h, b->data, size))) {
                    debug(", resized %p\n", new);
                    b->data = new;
                    b->size = size;
                    memset(b->data, b->content, b->size);
                } else {
                    debug(", resize failed\n");
                }
                break;
            }
        } else {
            if ((new = heap_block_alloc(h, size))) {
                debug(", allocated %p\n", new);
                b->data = new;
                b->size = size;
                b->content = rand() % 0x100;
                memset(b->data, b->content, b->size);
            } else {
                debug(", allocation failed\n");
            }
        }
    }

    return 0;
}
