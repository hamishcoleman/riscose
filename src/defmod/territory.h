#ifndef territory_H
#define territory_H

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

#ifndef types_H
   #include "types.h"
#endif

typedef int territory_t;

#define territory_CURRENT ((territory_t) -1)

#define territory_IGNORE_CASE    (1U << 0)
#define territory_IGNORE_ACCENTS (1U << 1)

#define territory_SYMBOL_DECIMAL_POINT 0

/* No UNIX definitions required for this component */

#endif
