#ifndef osfile_H
#define osfile_H

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

#include <swis.h>

#ifndef types_H
   #include "types.h"
#endif

#ifndef fileswitch_H
   #include "bootstrap/fileswitch.h"
#endif

#define OSFile_Load               255
#define OSFile_Save                 0
#define OSFile_WriteInfo            1
#define OSFile_WriteLoad            2
#define OSFile_WriteExec            3
#define OSFile_WriteAttr            4
#define OSFile_Read                 5
#define OSFile_Delete               6
#define OSFile_Create               7
#define OSFile_CreateDir            8
#define OSFile_SetStamp             9
#define OSFile_SaveStamp           10
#define OSFile_CreateStamp         11
#define OSFile_LoadPath            12
#define OSFile_ReadPath            13
#define OSFile_LoadPathVar         14
#define OSFile_ReadPathVar         15
#define OSFile_LoadNoPath          16
#define OSFile_ReadNoPath          17
#define OSFile_SetType             18
#define OSFile_MakeError           19
#define OSFile_ReadWithType        20
#define OSFile_ReadWithTypePath    21
#define OSFile_ReadWithTypePathVar 22
#define OSFile_ReadWithTypeNoPath  23
#define OSFile_ReadBlockSize       24

#define osfile_NOT_FOUND 0
#define osfile_IS_FILE   1
#define osfile_IS_DIR    2

#define osfile_TYPE_DRAW         0xAFF
#define osfile_TYPE_BBC_ROM      0xBBC
#define osfile_TYPE_DOS_DISC     0xFC8
#define osfile_TYPE_DEVICE       0xFCC
#define osfile_TYPE_CACHE        0xFCF
#define osfile_TYPE_TASK_EXEC    0xFD6
#define osfile_TYPE_TASK_OBEY    0xFD7
#define osfile_TYPE_DOS          0xFE4
#define osfile_TYPE_DESKTOP      0xFEA
#define osfile_TYPE_OBEY         0xFEB
#define osfile_TYPE_TEMPLATE     0xFEC
#define osfile_TYPE_PALETTE      0xFED
#define osfile_TYPE_CONFIG       0xFF2
#define osfile_TYPE_PRINTOUT     0xFF4
#define osfile_TYPE_POSTSCRIPT   0xFF5
#define osfile_TYPE_FONT         0xFF6
#define osfile_TYPE_BBC_FONT     0xFF7
#define osfile_TYPE_ABSOLUTE     0xFF8
#define osfile_TYPE_SPRITE       0xFF9
#define osfile_TYPE_MODULE       0xFFA
#define osfile_TYPE_BASIC        0xFFB
#define osfile_TYPE_UTILITY      0xFFC
#define osfile_TYPE_DATA         0xFFD
#define osfile_TYPE_COMMAND      0xFFE
#define osfile_TYPE_TEXT         0xFFF

#define osfile_TYPE_DIRECTORY   0x1000
#define osfile_TYPE_APPLICATION 0x2000
#define osfile_TYPE_UNTYPED 0xFFFFFFFF

#define osfile_TYPE(load_addr) ((load_addr & 0xFFF00000) == 0xFFF00000? \
      (load_addr) >> 8 & 0xFFF: osfile_TYPE_UNTYPED)

extern os_error *xosfile_create_dir(char *, int);
extern void      osfile_create_dir(char *, int);
extern void      os_generate_error(_kernel_oserror *);

#endif
