/* hash.h. */

/* $Id$ */

/* a hash table implementation.  a key is a variable length lump of
 * memory.  the hash routines don't make a copy of the key so make sure
 * it hangs around for as long as the hash entry exists.  the hash
 * expands as necessary to keep the bucket chains short.  */

typedef struct hash hash;

/* you're allowed to read key, keylen, and datum. */

typedef struct hash_elem {
    /* the location and length of the key. */
    char *key;
    int keylen;
    /* the hash of the key. */
    unsigned int hash;
    /* the user's datum. */
    void *datum;
    struct hash_elem *next;
} hash_elem;

hash *create_hash(void);

/* free all memory allocated for a hash by this package. */
void delete_hash(hash *hash);

/* add datum to hash under key. */
hash_elem *hash_add(hash *hash, char *key, int keylen, void *datum);
hash_elem *hash_adds(hash *hash, char *key, void *datum);

#define hash_addi(h, k, d) hash_add((h), (char *)&(k), sizeof (k), (d))

/* search hash for key.  return NULL if not found. */
hash_elem *hash_lookup(hash *hash, char *key, int keylen);
hash_elem *hash_lookups(hash *hash, char *key);

#define hash_lookupi(h, k) hash_lookup((h), (char *)&(k), sizeof (k))

/* search hash for key returning datum or NULL if not found.  if datum
 * is NULL you won't be able to tell if it was found or not. */
void *hash_lookup_get_datum(hash *hash, char *key, int keylen);
void *hash_lookups_get_datum(hash *hash, char *key);

#define hash_lookupi_get_datum(h, k) \
    hash_lookup_get_datum((h), (char *)&(k), sizeof (k))

/* call visit for every hash element.  if visit returns non-zero then
 * immediately return the same value to the caller.  modifying the hash
 * from within the visit routine isn't wise. */
int walk_hash(hash *hash, int (*visit)(hash_elem *elem, void *closure),
    void *closure);

/* returns a pointer to a malloc'd area containing an array of pointers
 * to keys in a non-predictable order.  the list is NULL terminated.
 * numelem is the number of keys. */
void **hash_keys(hash *hash, int *numelem);

/* as hash_keys except the datums are returned. */
void **hash_datums(hash *hash, int *numelem);
