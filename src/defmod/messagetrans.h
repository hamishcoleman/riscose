#ifndef messagetrans_H
#define messagetrans_H

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

#include <kernel.h>

#ifndef types_H
   #include "types.h"
#endif

#define messagetrans_DIRECT_ACCESS (1 << 0)

typedef struct {int cb [4];} messagetrans_control_block;

extern void messagetrans_error_lookup(_kernel_oserror *, void *, _kernel_oserror *,
  int, char *, char *, char *, char *);

#endif
