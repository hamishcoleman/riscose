/* str.h -- provide an expanding string based on array.h. */

/* a str is an expanding array of char from array.h.  you can access
 * all the bits of str and use array functions on it.  the typedef is just
 * for convenience, not abstraction. */
typedef DEFINE_ARRAY(char) str;

/* append, not write, the printf style format string to s.  not all
 * %-format specifiers are supported. */
void strprintf(str *s, char *fmt, ...);

/* like strprintf except it takes a va_list instead of variadic
 * arguments. */
void vstrprintf(str *s, char *fmt, va_list args);

/* calls vstrprintf returning the address of the resulting string.
 * doesn't require you to pass in a str.  useful for one off uses of a
 * string. */
char *mstrprintf(char *fmt, ...);
