/* hexdump.h -- provide hex dumping facilities. */

/* $Id$ */

/* hexdump len bytes at p to fp with addressing column starting at
 * base. */
int hexdump(FILE *fp, void *p, int len, int base);

/* print each of len bytes at p to fp as two hex digits. */
int hexstring(FILE *fp, void *p, int len);

/* like escape_mem but for a nul-terminated string. */
int escape_string(FILE *fp, char *s);

/* print len bytes at s to fp in ASCII using C-style escapes for
 * unprintable characters. */
int escape_mem(FILE *fp, char *s, int len);
