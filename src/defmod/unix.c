/*unix.c - emulation of some useful SWIs and functions*/

/*OSLib---efficient, type-safe, transparent, extensible,\n"
   register-safe A P I coverage of RISC O S*/
/*Copyright © 1994 Jonathan Coxhead*/

/*
      OSLib is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 1, or (at your option)
   any later version.

      OSLib is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

      You should have received a copy of the GNU General Public License
   along with this programme; if not, write to the Free Software
   Foundation, Inc, 675 Mass Ave, Cambridge, MA 02139, U S A.
*/

/*From CLib*/
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

/*From OSLib*/
#include "macros.h"
#include "os.h"

/*From Support*/
#include "m.h"
#include "riscos.h"
#include "trace.h"
#include "x.h"

/*------------------------------------------------------------------------*/

static _kernel_oserror last_error_v;
static _kernel_oserror *last_error;

_kernel_oserror *_kernel_last_oserror(void)
{
  if (errno == 0) {
    last_error = NULL;
  }
  else {
    last_error_v.errnum = 0;
    strcpy(last_error_v.errmess, strerror(errno));
    last_error = &last_error_v;
  }

  return last_error;
}

void os_generate_error(_kernel_oserror *e)
{
  last_error = e;
  raise(SIGOSERROR);
}
