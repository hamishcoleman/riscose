/* module.h
**
** (c) Matthew Bloch 2000
**
** See http://riscose.sourceforge.net/ for terms of distribution, and to
** pick up a later version of the software.
**
**   Facilities for loading RISC OS modules, running them, using their
**   CLI commands, SWIs etc.
*/

/* arm address `a', byte offset `o'. */
#define MODULE_ENTRY(a, o) \
    (MEM_READ_WORD((a) + (o)) ? (a) + MEM_READ_WORD((a) + (o)) : 0)

#define MODULE_START(a)			MODULE_ENTRY((a), 0)
#define MODULE_INIT(a) 			MODULE_ENTRY((a), 4)
#define MODULE_FINAL(a) 		MODULE_ENTRY((a), 8)
#define MODULE_SERVICE(a) 		MODULE_ENTRY((a), 12)
#define MODULE_TITLE(a)			MODULE_ENTRY((a), 16)
#define MODULE_HELP(a) 			MODULE_ENTRY((a), 20)
#define MODULE_COMMAND(a) 		MODULE_ENTRY((a), 24)
#define MODULE_SWI_BASE(a) 		MEM_READ_WORD((a)+28)
#define MODULE_SWI_HANDLER(a) 		MODULE_ENTRY((a), 32)
#define MODULE_SWI_DECODING_TABLE(a) 	MODULE_ENTRY((a), 36)
#define MODULE_SWI_DECODING_CODE(a) 	MODULE_ENTRY((a), 40)
#define MODULE_MESSAGES(a) 		MODULE_ENTRY((a), 44)

void	module_init(void);
int	module_numberofmodules(void);
WORD    module_base(int num);
WORD    module_private_word_ptr(int);
int	module_lookup(char *name);
int	module_load(char *name);
int	module_kill(int num);
