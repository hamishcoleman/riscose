/* instr.h : macros etc for disassembling individual instructions
 *
 * This file is part of ObjDism.
 * (c) 1998 Gareth McCaughan.
 * See the file LICENCE for distribution conditions.
 */

_NEED_def_h

typedef enum {
  target_None,		/* instruction doesn't refer to an address */
  target_Data,		/* instruction refers to address of data */
  target_FloatS,	/* instruction refers to address of single-float */
  target_FloatD,	/* instruction refers to address of double-float */
  target_FloatE,	/* blah blah extended-float */
  target_FloatP,	/* blah blah packed decimal float */
  target_Code,		/* instruction refers to address of code */
  target_Unknown	/* instruction refers to address of *something* */
} eTargetType;

defstruct(Instruction) {
  char text[128];	/* the disassembled instruction */
  int undefined;	/* non-0 iff it's an undefined instr */
  int badbits;		/* non-0 iff something reserved has the wrong value */
  int oddbits;		/* non-0 iff something unspecified isn't 0 */
  int is_SWI;		/* non-0 iff it's a SWI */
  word swinum;		/* only set for SWIs */
  address target;	/* address instr refers to */
  eTargetType target_type;	/* and what we expect to be there */
  long offset;		/* offset from register in LDR or STR or similar */
  char * addrstart;	/* start of address part of instruction, or 0 */
};

#define disopt_SWInames		1	/* use names, not &nnnn */
#define disopt_CommaSpace	2	/* put spaces after commas */
#define disopt_FIXS		4	/* bogus FIX syntax for ObjAsm */

defstruct(DisOptions) {
  word flags;
  char * * regnames;	/* pointer to 16 |char *|s: register names */
};

extern pInstruction instr_disassemble(word, address, pDisOptions);

#define _NEED_instr_h
