/* Inadequate program for testing heap.c */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include <monty/monty.h>
#include <monty/mem.h>
#include "heap.h"

#define HEAP_SIZE 1024

typedef struct {
    BYTE *data;
    ULONG size;
    BYTE content;
} test;

#define D if (optd) debug
#define O(p) ((char *)(p) - (char *)h)
#define IN_RANGE(x, l, h) ((x) >= (l) && (x) <= (h))
#define CHECK_PTR(p) \
    IN_RANGE((char *)(p), (char *)h, (char *)h + HEAP_SIZE - 1)

int main(int argc, char *argv[])
{
    int optd;
    int testnum;
    int testdec;
    char **argp;
    test block[100];
    test *b;
    heap_t *h;
    int size;
    BYTE *p;
    BYTE *new;

    (progname = strrchr(*argv, '/')) ? progname++ : (progname = *argv);
    debugf = verbosef = stderr;

    optd = 0;
    testnum = 1;
    testdec = 0;

    argp = argv + 1;
    if (*argp && !strcmp(*argp, "-d")) {
        optd = 1;
        argp++;
    }
    if (*argp) {
        testnum = strtol(*argp, NULL, 0);
        testdec = 1;
        argp++;
    }
    if (*argp) {
        error("usage: %s [-d] [numtest]\n", progname);
    }

    for (b = block; b < END(block); b++) {
        b->data = NULL;
        b->size = 0;
        b->content = 0;
    }

    h = emalloc(HEAP_SIZE);
    D("heap %#p, size %d\n", h, HEAP_SIZE);
    heap_init(h, HEAP_SIZE);

    do {
        b = block + rand() % DIM(block);
        size = ((rand() % (HEAP_SIZE / 8)) + 3) & ~3;

        D("block %2d, size %3d", b - block, size);

        if (b->data) {
            for (p = b->data; p < b->data + b->size; p++) {
                if (*p != b->content) {
                    error("content wrong: block +%x byte +%x was "
                        "%ud not %ud\n", O(b->data), O(p), *p, b->content);
                }
            }

            switch (rand() % 2) {
            case 0:
                D(", free\n");
                heap_block_free(h, b->data);
                b->data = NULL;
                break;
            case 1:
                if ((new = heap_block_resize(h, b->data, size))) {
                    D(", resized +%d\n", O(new));
                    assert(CHECK_PTR(new));
                    assert(CHECK_PTR((char *)new + size - 1));

                    b->data = new;
                    b->size = size;
                    memset(b->data, b->content, b->size);
                } else {
                    D(", resize failed\n");
                }
                break;
            }
        } else {
            if ((new = heap_block_alloc(h, size))) {
                D(", allocated +%d\n", O(new));
                assert(CHECK_PTR(new));
                assert(CHECK_PTR((char *)new + size - 1));

                b->data = new;
                b->size = size;
                b->content = rand() % 0x100;
                memset(b->data, b->content, b->size);
            } else {
                D(", allocation failed\n");
            }
        }
    } while (testnum -= testdec);

    efree(h);

    return 0;
}
