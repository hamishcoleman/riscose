#ifndef os_H
#define os_H

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
#include <swis.h>

#ifndef types_H
   #include "bootstrap/types.h"
#endif

#define osbyte_IN_KEY ((byte) 129)

/*For OS_Byte 253*/
#define os_SOFT_RESET     0
#define os_POWER_ON_RESET 1
#define os_HARD_RESET     2

/*For OS_SubstituteArgs*/
#define os_NOAPPEND (1U << 31)

/*For OS_GS{Init,Read,Trans}*/
#define os_GS_TERMINATE_ON_SPACE  (1U << 29)
#define os_GS_RETAIN_STICK        (1U << 30)
#define os_GS_RETAIN_DOUBLE_QUOTE (1U << 31)

/*For OS_{Read,Set}VarVal.*/
#define os_VARTYPE_STRING   0
#define os_VARTYPE_NUMBER   1
#define os_VARTYPE_MACRO    2
#define os_VARTYPE_EXPANDED 3
#define os_VARTYPE_CODE     16

typedef _kernel_oserror os_error;
#define os_ERROR_LIMIT 252

#define os_VDU_NULL                             ((char) '\x00')
#define os_VDU_CHAR_TO_PRINTER                  ((char) '\x01')
#define os_VDU_PRINTER_ON                       ((char) '\x02')
#define os_VDU_PRINTER_OFF                      ((char) '\x03')
#define os_VDU_GRAPH_TEXT_OFF                   ((char) '\x04')
#define os_VDU_GRAPH_TEXT_ON                    ((char) '\x05')
#define os_VDU_SCREEN_ON                        ((char) '\x06')
#define os_VDU_BELL                             ((char) '\x07')
#define os_VDU_BACKSPACE                        ((char) '\x08')
#define os_VDU_TAB                              ((char) '\x09')
#define os_VDU_LINEFEED                         ((char) '\x0A')
#define os_VDU_VERTICAL_TAB                     ((char) '\x0B')
#define os_VDU_CLS                              ((char) '\x0C')
#define os_VDU_RETURN                           ((char) '\x0D')
#define os_VDU_PAGE_MODE_ON                     ((char) '\x0E')
#define os_VDU_PAGE_MODE_OFF                    ((char) '\x0F')
#define os_VDU_CLG                              ((char) '\x10')
#define os_VDU_SET_TEXT_COLOUR                  ((char) '\x11')
#define os_VDU_SET_GCOL                         ((char) '\x12')
#define os_VDU_SET_PALETTE                      ((char) '\x13')
#define os_VDU_RESET_COLOURS                    ((char) '\x14')
#define os_VDU_SCREEN_OFF                       ((char) '\x15')
#define os_VDU_MODE                             ((char) '\x16')
#define os_VDU_MISC                             ((char) '\x17')
#define os_VDU_SET_GRAPHICS_WINDOW              ((char) '\x18')
#define os_VDU_PLOT                             ((char) '\x19')
#define os_VDU_RESET_WINDOWS                    ((char) '\x1A')
#define os_VDU_ESCAPE                           ((char) '\x1B')
#define os_VDU_SET_TEXT_WINDOW                  ((char) '\x1C')
#define os_VDU_SET_GRAPHICS_ORIGIN              ((char) '\x1D')
#define os_VDU_HOME_TEXT_CURSOR                 ((char) '\x1E')
#define os_VDU_SET_TEXT_CURSOR                  ((char) '\x1F')
#define os_VDU_DELETE                           ((char) '\x7F')
#define os_MISC_INTERLACE                       ((char) '\x00')
#define os_MISC_CURSOR                          ((char) '\x01')
#define os_MISC_SET_ECF1                        ((char) '\x02')
#define os_MISC_SET_ECF2                        ((char) '\x03')
#define os_MISC_SET_ECF3                        ((char) '\x04')
#define os_MISC_SET_ECF4                        ((char) '\x05')
#define os_MISC_SET_DOT_STYLE                   ((char) '\x06')
#define os_MISC_SCROLL                          ((char) '\x07')
#define os_MISC_CLEAR_REGION                    ((char) '\x08')
#define os_MISC_SET_ON                          ((char) '\x09')
#define os_MISC_SET_OFF                         ((char) '\x0A')
#define os_MISC_SET_BBCECF                      ((char) '\x0B')
#define os_MISC_SET_UNPACKED_ECF1               ((char) '\x0C')
#define os_MISC_SET_UNPACKED_ECF2               ((char) '\x0D')
#define os_MISC_SET_UNPACKED_ECF3               ((char) '\x0E')
#define os_MISC_SET_UNPACKED_ECF4               ((char) '\x0F')
#define os_MISC_SET_CURSOR_MOVEMENT             ((char) '\x10')
#define os_MISC_MISC                            ((char) '\x11')
#define os_MISC_SET_TEXT_FG_TINT                ((char) '\x00')
#define os_MISC_SET_TEXT_BG_TINT                ((char) '\x01')
#define os_MISC_SET_GRAPHICS_FG_TINT            ((char) '\x02')
#define os_MISC_SET_GRAPHICS_BG_TINT            ((char) '\x03')
#define os_MISC_SET_ECF_INTERPRETATION          ((char) '\x04')
#define os_MISC_INVERT_TEXT                     ((char) '\x05')
#define os_MISC_SET_ECF_ORIGIN                  ((char) '\x06')
#define os_MISC_SET_CHAR_SIZE                   ((char) '\x07')

typedef int os_t;
typedef struct {char c [5];} os_clock;
typedef struct {int x, y;} os_coord;
typedef struct {int x0, y0, x1, y1;} os_box;
typedef struct {int m [3] [2];} os_trfm_mat;
typedef struct {os_coord p [4];} os_pgm;

typedef byte os_f;

typedef int os_var_type;

typedef byte os_date_and_time [5];
      /*System date and time*/

#define os_GLOBAL_NO_ANY 9
#define os_GLOBAL_NO_MEM 49

extern os_error *xos_pretty_print(char *, void *, void *);

#endif
