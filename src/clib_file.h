#include "riscostypes.h"

#include <stdio.h>
#include "mem.h"
#include "arm.h"

typedef struct {
  WORD __ptr;
  WORD __icnt;
  WORD __ocnt;
  WORD __flag;
  WORD __base;
  WORD __file;
  WORD __pos;
  WORD __bufsiz;
  WORD __signature;
  FILE *real;
}
riscos_FILE;

WORD	clib_file_new(FILE *real);
FILE*   clib_file_real(WORD arm_addr);
void	clib_file_dispose(WORD arm_addr);
void	clib_file_sync(WORD arm_addr);
