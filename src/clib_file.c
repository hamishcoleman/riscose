#include "clib_file.h"

WORD clib_file_new(FILE *real)
{
  WORD rof_arm = mem_rma_alloc(sizeof(riscos_FILE));
  riscos_FILE* rof = (riscos_FILE*) MEM_TOHOST(rof_arm);
  
  rof->real   = real;
  rof->__base = mem_rma_alloc(1);
  rof->__bufsiz = 0;
  rof->__icnt = 0;
  rof->__ocnt = 0;
  clib_file_sync(rof_arm);
  
  return rof_arm;
}

FILE* clib_file_real(WORD arm_addr)
{
  riscos_FILE* rof = (riscos_FILE*) MEM_TOHOST(arm_addr);

  clib_file_sync(arm_addr);
  return rof->real;
}

void clib_file_dispose(WORD arm_addr)
{
  riscos_FILE* rof = (riscos_FILE*) MEM_TOHOST(arm_addr);

  mem_rma_free(rof->__base);
  mem_rma_free(arm_addr);
}

void clib_file_sync(WORD arm_addr)
{
  riscos_FILE* rof = (riscos_FILE*) MEM_TOHOST(arm_addr);
  FILE *real = rof->real;
  
  rof->__pos  = ftell(real);
  rof->__file = fileno(real);
}
