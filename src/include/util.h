/* util.h
**
** (c) Matthew Bloch 2000
**
** See http://riscose.sourceforge.net/ for terms of distribution, and to
** pick up a later version of the software.
**
**   Just a noddy xmalloc function for the moment.
**
**   $Revision$
**   $Date$
*/
#ifndef _RISCOSE_UTIL_H
#define _RISCOSE_UTIL_H

#include <stddef.h>

void *xmalloc(size_t size);
char *xstrncpy(char *src, int n);
#define xstrcpybetween(start, end) xstrncpy((start), (end)-(start))

#define OBJECT_NOTFOUND	0
#define OBJECT_FILE	    1
#define OBJECT_DIR	    2
#define OBJECT_IMAGE	3
int file_objecttype(char *name);
#define file_isfile(x) (file_objecttype((x)) == OBJECT_FILE)
size_t file_size(char *name);
int file_loadat(char *name, void *addr);

#endif
