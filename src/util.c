/* util.c
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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include "util.h"

void *xmalloc(size_t size)
{
  void *p = malloc(size);
  if (p)
    {
     bzero(p, size);
     return p;
    }
  fprintf(stderr, "*** Couldn't allocate %d bytes\n", size);
  exit(1);
}

char *xstrncpy(char *src, int n)
{
  char *dst = xmalloc(n+1);
  strncpy(dst, src, n);
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
