#ifndef macros_H
#define macros_H

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

/************************
 * Constant definitions *
 ************************/
#ifndef DEC_WIDTH
#define DEC_WIDTH                               10
#endif
#ifndef SHORT_DEC_WIDTH
#define SHORT_DEC_WIDTH                         5
#endif
#ifndef LONG_DEC_WIDTH
#define LONG_DEC_WIDTH                          10
#endif
#ifndef OCT_WIDTH
#define OCT_WIDTH                               11
#endif
#ifndef SHORT_OCT_WIDTH
#define SHORT_OCT_WIDTH                         6
#endif
#ifndef LONG_OCT_WIDTH
#define LONG_OCT_WIDTH                          11
#endif
#ifndef UNSIGNED_WIDTH
#define UNSIGNED_WIDTH                          10
#endif
#ifndef SHORT_UNSIGNED_WIDTH
#define SHORT_UNSIGNED_WIDTH                    5
#endif
#ifndef LONG_UNSIGNED_WIDTH
#define LONG_UNSIGNED_WIDTH                     10
#endif
#ifndef HEX_WIDTH
#define HEX_WIDTH                               8
#endif
#ifndef SHORT_HEX_WIDTH
#define SHORT_HEX_WIDTH                         4
#endif
#ifndef LONG_HEX_WIDTH
#define LONG_HEX_WIDTH                          8
#endif
#ifndef FLT_WIDTH
#define FLT_WIDTH                               7
#endif
#ifndef DBL_WIDTH
#define DBL_WIDTH                               17
#endif
#ifndef LDBL_WIDTH
#define LDBL_WIDTH                              17
#endif
#ifndef FLT_EXP_WIDTH
#define FLT_EXP_WIDTH                           2
#endif
#ifndef DBL_EXP_WIDTH
#define DBL_EXP_WIDTH                           3
#endif
#ifndef LDBL_EXP_WIDTH
#define LDBL_EXP_WIDTH                          3
#endif

/*********************
 * Macro definitions *
 *********************/

/* ------------------------------------------------------------------------
 * Macro:         DIGIT()
 *
 * Description:   The decimal digit corresponding to a character
 */

#define DIGIT(c) ((c) - '0')

/* ------------------------------------------------------------------------
 * Macro:         MAX()
 *
 * Description:   The larger of two values
 */

#define MAX(a, b) ((a) > (b)? (a): (b))

/* ------------------------------------------------------------------------
 * Macro:         NCOPY()
 *
 * Description:   Copy a string of limited length
 */

#define NCOPY(s1, s2, n) (sprintf (s1, "%.*s", n, s2), s1)

/* ------------------------------------------------------------------------
 * Macro:         XDIGIT()
 *
 * Description:   The hexadecimal digit corresponding to a character
 */

#define XDIGIT(c) ((byte) (c) < 'A'? (c) - '0': \
      (byte) (c) < 'a'? (c) - 'A' + 10: (c) - 'a' + 10)

#endif
