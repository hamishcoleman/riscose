/* mem.h -- shortcuts to malloc and friends. */

#undef EXTERN
#ifdef DEFINE_MEM_GLOBALS
#define EXTERN
#else
#define EXTERN extern
#endif

/* set a to the address of some malloc'd memory of the right size. */
#define NEW(a) ((a) = emalloc(sizeof(*(a))))

/* set a to the address of some zero-filled malloc'd memory. */
#define NEW0(a) ((a) = ecalloc(sizeof(*(a))))

/* the number of elements in array a. */
#define DIM(a) (sizeof(a) / sizeof((a)[0]))

/* the address of the element past the end of array a. */
#define END(a) ((a) + DIM(a))

#if TRACK_MEM

/* used to pass the location of the mem request to the routines without
 * altering the number of parameters the routines take. */
EXTERN int mem_line;
EXTERN char *mem_file;

/* these macros turn all calls into the original routines into ones that
 * save the file name and line number first. */
#define emalloc(a) (mem_line = __LINE__, mem_file = __FILE__, (emalloc)(a))
#define ecalloc(a) (mem_line = __LINE__, mem_file = __FILE__, (ecalloc)(a))
#define erealloc(a, b) \
    (mem_line = __LINE__, mem_file = __FILE__, (erealloc)(a, b))
#define estrdup(a) (mem_line = __LINE__, mem_file = __FILE__, (estrdup)(a))
#define ememdup(a, b) \
    (mem_line = __LINE__, mem_file = __FILE__, (ememdup)(a, b))
#define efree(a) (mem_line = __LINE__, mem_file = __FILE__, (efree)(a))

#endif

/* malloc size bytes.  no memory is a fatal error. */
void *(emalloc)(int size);

/* malloc size bytes initialised to zero.  no memory is a fatal error.  */
void *(ecalloc)(int size);

/* change existing malloc'd memory to be size bytes long.  this might
 * cause it to move. */
void *(erealloc)(void *mem, int size);

/* duplicate a string in malloc'd memory.  no memory is fatal. */
char *(estrdup)(char *s);

/* duplicates the len bytes of memory at m. */
void *(ememdup)(void *m, int len);

/* free's previously malloc'd memory.  call it instead of free so that
 * debug and memory tracking will work. */
void (efree)(void *p);
