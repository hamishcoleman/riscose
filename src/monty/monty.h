/* monty.h -- central routines for the monty library. */

#undef EXTERN
#ifdef DEFINE_MONTY_GLOBALS
#define EXTERN
#else
#define EXTERN extern
#endif

/* make it clear when a boolean is being returned. */
#if !defined(TRUE)
#define FALSE 0
#define TRUE 1
#endif

/* the basename of *argv.  used in diagnostics. */
EXTERN char *progname;

/* verbose and debug output controlled via monty_options should be
 * written to these. */
EXTERN FILE *verbosef;
EXTERN FILE *debugf;

/* options that affect all parts of the code. */
typedef struct {
    /* true if the user would like to know what is going on. */
    int verbose;
    /* a set of bits indicating the areas that should output debug. */
    int debug;
} monty_options;

/* export montyopt */
EXTERN monty_options montyopt;

/* FIXME: other parts of monty aren't required at the moment so these
 * aren't relevant. */

/* these macros allow easy changing of what areas use what bit. */
#if CONF_DEBUGGING
#define DEBUG_POOL (montyopt.debug & 0x01)
#define DEBUG_MEM (montyopt.debug & 0x02)
#define DEBUG_HASH (montyopt.debug & 0x04)
#define DEBUG_FILE (montyopt.debug & 0x08)
#else
#define DEBUG_POOL FALSE
#define DEBUG_MEM FALSE
#define DEBUG_HASH FALSE
#define DEBUG_FILE FALSE
#endif

/* the symbolic debug codes that correspond to the above expressions.
 * a minus sign is a placeholder for future use. */
#define MONTY_DEBUG_CODES "omhf----"

/* a description of MONTY_DEBUG_CODES, suitable for output with -h. */
#define MONTY_DEBUG_HELP \
    "            1 o -- pool memory routines.\n" \
    "            2 m -- memory routines.\n" \
    "            4 h -- hash routines.\n" \
    "            8 f -- file handling routines.\n" \

/* debug code shouldn't disappear if CONF_DEBUGGING is false since the
 * compiler won't check it and turning on CONF_DEBUGGING would create
 * new compile errors. */
#define DEBUG(type, a) \
    if (DEBUG_ ## type) { \
        debug a; \
    }

/* printf's to the debug output file.  only call if montyopt.debug is
 * non-zero. */
void debug(char *fmt, ...);

/* printf's to the verbose output file.  only call if montyopt.verbose
 * is true.  output is prefixed with `progname: '. */
void verbose(char *fmt, ...);
#define VERBOSE(a) \
    if (montyopt.verbose) { \
        verbose a; \
    }

/* printf's a warning to stderr.  output is prefixed with `progname:
 * warn: '. */
void warn(char *fmt, ...);

/* printf's an error to stderr.  output is prefixed with `progname:
 * error: '.  exit(1) is then called. */
void error(char *fmt, ...);

/* printf's an error to stderr.  output is prefixed with `progname: '.
 * abort is then called to produce a core dump (unless something is
 * catching SIGABRT). */
void dump_core(char *fmt, ...);
