/* riscose.h -- riscose specific definitions. */

/* these macros allow easy changing of what areas use what bit. */
#if CONF_DEBUGGING
#define DEBUG_SWI (montyopt.debug & 0x100)
#else
#define DEBUG_SWI FALSE
#endif

/* the symbolic debug codes that correspond to the above expressions.
 * a minus sign is a placeholder for future use. */
#define RISCOSE_DEBUG_CODES "s"

/* a description of MONTY_DEBUG_CODES, suitable for output with -h. */
#define RISCOSE_DEBUG_HELP \
    "        0x100 s -- swi calls.\n"

/* these macros allow easy changing of what areas use what bit. */
#if CONF_DEBUGGING
#define DEBUG_SWI (montyopt.debug & 0x100)
#else
#define DEBUG_SWI FALSE
#endif
