/* hash.c. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "monty.h"
#include "mem.h"
#include "hash.h"
#include "hexdump.h"

#define INITIAL_NUM_BUCKETS 8

struct hash {
    /* the number of elements in the hash table. */
    int numelem;
    /* the number of buckets currently allocated. */
    int numbucket;
    hash_elem **bucket;
};

static unsigned int key_to_hash(char *key, int keylen);
static void enlarge_hash(hash *orig);
static void dump_hash(hash *hash);

#define SEARCH_CHAIN(bucket) \
    for (e = (bucket); e; e = e->next) { \
        if (keyhash == e->hash && keylen == e->keylen && \
            !memcmp(key, e->key, keylen)) { \
            break; \
        } \
    }

hash *create_hash(void)
{
    hash *h;

    NEW(h);
    h->numelem = 0;
    h->numbucket = INITIAL_NUM_BUCKETS;
    h->bucket = ecalloc(sizeof *h->bucket * h->numbucket);
    DEBUG(HASH, ("create_hash returns %x\n", h));

    return h;
}

void delete_hash(hash *hash)
{
    int bucket;
    hash_elem *e;
    hash_elem *next;

    DEBUG(HASH, ("delete_hash(%x)\n", hash));

    for (bucket = 0; bucket < hash->numbucket; bucket++) {
        for (e = hash->bucket[bucket]; e; e = next) {
            next = e->next;
            efree(e);
        }
    }
    efree(hash->bucket);
    efree(hash);

    return;
}

hash_elem *hash_add(hash *hash, char *key, int keylen, void *datum)
{
    hash_elem *e;
    unsigned int keyhash;
    hash_elem **bucket;

    if (DEBUG_HASH) {
        debug("hash_add(%x, %x, %d, %x)\n", hash, key, keylen, datum);
        hexdump(debugf, key, keylen, (int)key);
    }

    if (hash->numelem >= hash->numbucket * 2) {
        enlarge_hash(hash);
    }

    keyhash = key_to_hash(key, keylen);
    bucket = hash->bucket + (keyhash & hash->numbucket - 1);
    SEARCH_CHAIN(*bucket);
    if (e) {
        error("hash_add: key already present in hash\n");
    }

    NEW(e);
    e->key = key;
    e->keylen = keylen;
    e->hash = keyhash;
    e->datum = datum;
    e->next = *bucket;
    *bucket = e;
    hash->numelem++;

    DEBUG(HASH, ("hash_add added %x\n", e));

    return e;
}

static unsigned int key_to_hash(char *key, int keylen)
{
    char *k;
    int l;
    unsigned int h;

    for (k = key, l = keylen, h = 0; l--; k++) {
        h = h * 33 + *k;
    }
    h += h >> 5;

    DEBUG(HASH, ("key_to_hash(%x, %d) returns %d\n", key, keylen, h));

    return h;
}

/* double the number of buckets and re-distribute about half the hash
 * elements. */

static void enlarge_hash(hash *hash)
{
    unsigned int old_numbucket;
    hash_elem **new;
    unsigned int mask;
    unsigned int bucket;
    hash_elem **prev;
    hash_elem *e;

    if (DEBUG_HASH) {
        debug("enlarge_hash(%x)\nbefore:\n", hash);
        dump_hash(hash);
    }

    old_numbucket = hash->numbucket;
    hash->numbucket *= 2;
    hash->bucket = erealloc(hash->bucket, sizeof *hash->bucket *
        hash->numbucket);

    new = hash->bucket + old_numbucket;
    memset(new, 0, sizeof *new * old_numbucket);

    mask = hash->numbucket - 1;
    for (bucket = 0; bucket < old_numbucket; bucket++) {
        prev = hash->bucket + bucket;
        for (e = *prev; e; e = *prev) {
            if ((e->hash & mask) == bucket) {
                prev = &e->next;
            } else {
                *prev = e->next;
                e->next = *new;
                *new = e;
            }
        }
        new++;
    }

    if (DEBUG_HASH) {
        debug("now %d buckets, %d elements\nafter:\n", hash->numbucket,
            hash->numelem);
        dump_hash(hash);
    }

    return;
}

hash_elem *hash_adds(hash *hash, char *key, void *datum)
{
    DEBUG(HASH, ("hash_adds(%x, \"%s\", %x)\n", hash, key, datum));

    return hash_add(hash, key, strlen(key) + 1, datum);
}

hash_elem *hash_lookup(hash *hash, char *key, int keylen)
{
    unsigned int keyhash;
    hash_elem *e;

    if (DEBUG_HASH) {
        hexdump(debugf, key, keylen, (int)key);
    }
    keyhash = key_to_hash(key, keylen);
    SEARCH_CHAIN(hash->bucket[keyhash & hash->numbucket - 1]);
    DEBUG(HASH, ("hash_lookup returns %x\n", e));

    return e;
}

hash_elem *hash_lookups(hash *hash, char *key)
{
    DEBUG(HASH, ("hash_lookups(%x, \"%s\")\n", hash, key));

    return hash_lookup(hash, key, strlen(key) + 1);
}

void *hash_lookup_get_datum(hash *hash, char *key, int keylen)
{
    hash_elem *e;
    void *datum;

    e = hash_lookup(hash, key, keylen);
    datum = e ? e->datum : NULL;

    DEBUG(HASH, ("hash_lookup_get_datum(%x, %x, %d) returns %x\n",
        hash, key, keylen, datum));

    return datum;
}

void *hash_lookups_get_datum(hash *hash, char *key)
{
    hash_elem *e;
    void *datum;

    e = hash_lookups(hash, key);
    datum = e ? e->datum : NULL;

    DEBUG(HASH, ("hash_lookups_get_datum(%x, \"%s\") returns %x\n",
        hash, key, datum));

    return datum;
}

int walk_hash(hash *hash, int (*visit)(hash_elem *elem, void *closure),
    void *closure)
{
    int bucket;
    hash_elem *e;
    int i;

    for (bucket = 0; bucket < hash->numbucket; bucket++) {
        for (e = hash->bucket[bucket]; e; e = e->next) {
            if (i = (*visit)(e, closure)) {
                return i;
            }
        }
    }

    return 0;
}

static void dump_hash(hash *hash)
{
    int found;
    int bucket;
    hash_elem *e;

    debug("dump_hash(%x)\n", hash);
    debug("    numelem: %d numbucket: %d bucket: %x\n", hash->numelem,
        hash->numbucket, hash->bucket);

    found = 0;
    for (bucket = 0; bucket < hash->numbucket; bucket++) {
        debug("    bucket: %d\n", bucket);
        for (e = hash->bucket[bucket]; e; e = e->next) {
            debug("        e: %x\n", e);
            debug("        key: %x keylen: %d hash: %x datum: %x\n",
                e->key, e->keylen, e->hash, e->datum);
            hexdump(debugf, e->key, e->keylen, (int)e->key);
            found++;
        }
    }

    debug("    found: %d\n", found);

    return;
}

#define FLATTEN(member) \
    void **array; \
    int bucket; \
    hash_elem *e; \
    void **a; \
    \
    *numelem = hash->numelem; \
    array = a = emalloc(sizeof *array * (*numelem + 1)); \
    \
    for (bucket = 0; bucket < hash->numbucket; bucket++) { \
        for (e = hash->bucket[bucket]; e; e = e->next) { \
            *a++ = e->member; \
        } \
    } \
    *a = NULL; \
    \
    return array;

void **hash_keys(hash *hash, int *numelem)
{
    FLATTEN(key);
}

void **hash_datums(hash *hash, int *numelem)
{
    FLATTEN(datum);
}
