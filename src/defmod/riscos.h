#ifndef riscos_H
#define riscos_H

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

/*From OSLib*/
#ifndef os_H
   #include "os.h"
#endif

 /* Macro to provide an assertion facility using Wimp_ReportError.
  */

#define riscos_assert(c) \
   (!(c)? riscos__assert (__FILE__, __LINE__, #c): (void) SKIP)

 /* Function to be used by |riscos_assert()| - not for external use.
  */

extern void riscos__assert (char *file, int line, char *msg);

 /* Trivial functions to do things "the RISC O S way" rather than "the C
  * way." This means treating strings as control-char terminated, rather
  * than 0-terminated. These can be printed using a format of
  * "%.*s" and passing in the length of the string, as calculated by
  * |riscos_strlen|, as a argument to the printing function. Note that
  * riscos_strncpy() is not like strncpy(): it always terminates its output.
  */

extern int riscos_strlen (char *s);

extern char *riscos_strcpy (char *s1, char *s);

extern int riscos_strcmp (char *s0, char *s1);

extern char *riscos_strncpy (char *s1, char *s, int n);

 /* Functions to read back the items above - these return the number of
  * characters successfully read, 0 for error.
  */

extern int riscos_var_len (char *var, os_var_type);
      /*length of variable value, or -1 if not found*/

extern os_error *riscos_vdu (int vdu, ...);
      /*Performs a single V D U sequence, as follows.*/

      /*riscos_vdu (os_VDU_CHAR_TO_PRINTER, c)*/
      /*riscos_vdu (os_VDU_SET_TEXT_COLOUR, colour)*/
      /*riscos_vdu (os_VDU_MODE, mode)*/
      /*riscos_vdu (os_VDU_SET_GCOL, action, colour)*/
      /*riscos_vdu (os_VDU_SET_TEXT_CURSOR, x, y)*/
      /*riscos_vdu (os_VDU_SET_PALETTE, colour, mode, red, green, blue)*/
      /*riscos_vdu (os_VDU_MISC, os_MISC_INTERLACE, action, mode)*/
      /*riscos_vdu (os_VDU_MISC, os_MISC_SET_CURSOR_MOVEMENT, eor_value, and_value)*/
      /*riscos_vdu (os_VDU_MISC, os_MISC_CURSOR, mode)*/
      /*riscos_vdu (os_VDU_MISC, os_MISC_SET_ON, duration)*/
      /*riscos_vdu (os_VDU_MISC, os_MISC_SET_OFF, duration)*/
      /*riscos_vdu (os_VDU_MISC, os_MISC_SET_ECFx, ecf)*/
      /*riscos_vdu (os_VDU_MISC, os_MISC_SET_DOT_STYLE, style)*/
      /*riscos_vdu (os_VDU_MISC, os_MISC_SET_UNPACKED_ECFx, ecf)*/
      /*riscos_vdu (os_VDU_MISC, os_MISC_SCROLL, extent, direction, movement)*/
      /*riscos_vdu (os_VDU_MISC, os_MISC_CLEAR_REGION, start, end, x0, y0, x1, y1)*/
      /*riscos_vdu (os_VDU_MISC, os_MISC_SET_BBCEOF)*/
      /*riscos_vdu (os_VDU_MISC, os_MISC_MISC, os_MISC_SET_TEXT_FG_TINT, tint)*/
      /*riscos_vdu (os_VDU_MISC, os_MISC_MISC, os_MISC_SET_TEXT_FG_TINT, tint)*/
      /*riscos_vdu (os_VDU_MISC, os_MISC_MISC, os_MISC_SET_TEXT_FG_TINT, tint)*/
      /*riscos_vdu (os_VDU_MISC, os_MISC_MISC, os_MISC_SET_TEXT_FG_TINT, tint)*/
      /*riscos_vdu (os_VDU_MISC, os_MISC_MISC, os_MISC_SET_ECF_INTERPRETATION, patterns)*/
      /*riscos_vdu (os_VDU_MISC, os_MISC_MISC, os_MISC_INVERT_TEXT)*/
      /*riscos_vdu (os_VDU_MISC, os_MISC_MISC, os_MISC_SET_ECF_ORIGIN, x, y)*/
      /*riscos_vdu (os_VDU_MISC, os_MISC_MISC, os_MISC_SET_CHAR_SIZE, flags, x, y)*/
      /*riscos_vdu (os_VDU_MISC, c, definition)*/
      /*riscos_vdu (os_VDU_SET_GRAPHICS_WINDOW, x0, y0, x1, y1)*/
      /*riscos_vdu (os_VDU_PLOT, plot_type, x, y)*/
      /*riscos_vdu (os_VDU_SET_TEXT_WINDOW, x0, y0, x1, y1)*/
      /*riscos_vdu (os_VDU_SET_GRAPHICS_ORIGIN, x, y)*/
#endif
