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
int    sysvars_init();
void   sysvars_set(char *var, char *val);
char * sysvars_get(char *var);
char * sysvars_unset(char *var);
char * sysvars_list_first();
char * sysvars_list_next(char *last);
