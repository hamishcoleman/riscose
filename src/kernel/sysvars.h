/* sysvars.h
**
** (c) Matthew Bloch 2000
**
** See http://riscose.sourceforge.net/ for terms of distribution, and to
** pick up a later version of the software.
**
**   Emulation of the system variable list; lazy reliance on gdbm.
**
**   $Revision$
**   $Date$
*/

#define SYSVAR_TYPE_STRING  0
#define SYSVAR_TYPE_INTEGER 1
#define SYSVAR_TYPE_MACRO   2

void sysvars_init();

WORD osreadvarval();
WORD ossetvarval();
