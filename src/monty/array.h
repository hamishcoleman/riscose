/* array.h -- extremely noddy, poor performance implementation. */

/* $Id$ */

/* the array routines aren't meant to hide the implementation from
 * you.  they are just providing some often needed funtionality.  so
 * feel free to access members of structs, etc.  */

#define DEFINE_ARRAY(t) \
    struct { \
        /* pointer to size * sizeof(*t) bytes. */ \
        t *mem; \
        /* number of elements allocated. */ \
        int size; \
        /* number of elements used. */ \
        int used; \
        /* how much to increase by when we run out of mem. */ \
        int delta; \
    }

/* the mem in an array is allocated using mem.h.  you are welcome to set
 * it to some memory you already had allocated if you set up the other
 * members of the struct appropriately, or to copy the pointer to
 * somewhere else so it lives on when the array goes out of scope.  it
 * follows that it is your responsability to efree the memory as well. */

/* set up the struct ready for use.  you can tweak delta manually
 * afterwards if you like. */
#define array_init(a) \
    ((a)->mem = NULL, (a)->size = (a)->used = 0, (a)->delta = 64)

/* if you've finished with an array then you can call this to free its
 * memory and get it ready for re-use.  if you're not going to re-use it
 * then just call efree(a->mem). */
#define array_fini(a) \
    if ((a)->size) { \
        efree((a)->mem); \
        (a)->mem = NULL; \
        (a)->used = (a)->size = 0; \
    }

/* add v to end of array a, checking there is space first. */
#define array_push(a, v) \
    if ((a)->used == (a)->size) { \
        (a)->size += (a)->delta; \
        (a)->mem = erealloc((a)->mem, (a)->size * sizeof(*(a)->mem)); \
    } \
    \
    (a)->mem[(a)->used++] = (v)

/* pop off the end of the array if it has anything in it. */
#define array_pop(a) \
    (void)((a)->used > 0 && (a)->used--)

/* ensure there are at least n unused elements. */
#define array_unused(a, n) \
    while ((a)->size - (a)->used < (n)) { \
        (a)->size += (a)->delta; \
    } \
    (a)->mem = erealloc((a)->mem, (a)->size * sizeof(*(a)->mem)); \

/* the address of the element after the last used one. */
#define array_end(a) \
    ((a)->mem + (a)->used)

/* shrink the malloc'd area so it is as small as possible. */
#define array_normalise(a) \
    (void)((a)->used != (a)->size && \
        ((a)->mem = erealloc((a)->mem, ((a)->size = (a)->used) * \
        sizeof(*(a)->mem))))
