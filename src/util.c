/* util.c
**
** (c) Matthew Bloch 2000
**
** See http://riscose.sourceforge.net/ for terms of distribution, and to
** pick up a later version of the software.
**
**   $Revision$
**   $Date$
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <monty/mem.h>

#include "util.h"

/* FIXME:  poor name.  strncpy doesn't malloc, this does.  strncpy
 * doesn't always terminate the new string, this does. */

char *xstrncpy(char *src, int n)
{
    char *dst;

    dst = emalloc(n + 1);
    strncpy(dst, src, n);
    dst[n] = '\0';

    return dst;
}

int file_objecttype(char *name)
{
  struct stat file_info;
  if (stat(name, &file_info) == -1)
    return OBJECT_NOTFOUND;
  if ((file_info.st_mode & S_IFDIR) != 0)
    return OBJECT_DIR;
  return OBJECT_FILE;
}

size_t file_size(char *name)
{
  struct stat file_info;
  if (stat(name, &file_info) == -1)
    return -1;
  return file_info.st_size;
}

int file_loadat(char *name, void *addr)
{
  FILE *fh = fopen(name, "r");
  if (fh == NULL)
    return -1;
  fread(addr, 1, file_size(name), fh);
  fclose(fh);
  return 0;
}
