/* swi.h
**
** (c) Matthew Bloch 2000
**
** See http://riscose.sourceforge.net/ for terms of distribution, and to
** pick up a later version of the software.
**
**   Constants and macros helpful when writing SWI handlers.
**
**   $Revision$
**   $Date$
*/
#define SWI_OS(x)    (((x)>>20)&15)
#define SWI_WHERE(x) (((x)>>18)&3)
#define SWI_X(x)     (((x)>>17)&1)
#define SWI_NUM(x)    ((x) & 0xdffff) /* as given in PRMs */

#define SWI_WHERE_OS       0
#define SWI_WHERE_OSEXT    1
#define SWI_WHERE_3RDPARTY 2
#define SWI_WHERE_USER     3

#define SWI_OS_RISCOS      0
#define SWI_OS_RISCIX      1
#define SWI_OS_TRAP        3 /* unofficial; should never appear in RISC OS */

/* Calling this SWI causes arm_run_routine() to return to whoever called it
** and restore the virtual ARM to wherever it was before.
*/
#define SWI_MAGIC_RETURN 0x3dffff

/* Under RISC OS, the SharedCLibrary module interfaces with applications
** by filling in two `jump tables' full of branch instructions into the
** library code in the module.  Rather than use branches, we'll use a bunch of
** unused SWIs for each library call; this would be much more of an overhead
** in RISC OS, but we want to be able to jump out of the ARM emulation ASAP
** to call faster native code.
*/
#define SWITRAP_KERN_BASE  0x1000
#define SWITRAP_CLIB_BASE  0x1100

/* SWI handlers can muck about with registers and memory all they like; they
** should return with 0 if there's no error to report, or >0 if they have.  In
** addition, SWIs that `don't return' or otherwise mess around with program
** flow should set the SWIH_EXIT_HANDLED bit.  Error blocks from the emulator
** always lie on word boundaries, so we can use the bottom two bits to indicate
** this kind of thing.
*/
#define SWIH_EXIT_HANDLED 1
typedef WORD (*swi_handler)(WORD);
#define SWI_EXIT_ERROR(c) { ARM_SET_R0((c)); arm_set_v();

void filehandles_init(void); /* FIXME: wrong implementation, wrong place... */
void osfile(void);
void osfind(void);
void osgbpb(void);
void osargs(void);

void osbyte(void);

void swi_init(void);
void swi_trap(WORD num);
void swi_register(WORD number, char *name, swi_handler handler);
