/* $Id$
 *
 * Generate C code for the block data transfer instructions
 *
 * (c) 2000 Chris Rutter <chris@armlinux.org>
 */

/* --- Rationale --- *
 *
 * Rather than having to bit-unwind every BDT instruction with a load of
 * conditional clauses for pre- and post-increment, we use a program to
 * generate exact sequences for every instruction permutation.
 *
 * We have two basic variants: incrementing vs. decrementing and load vs.
 * store.   We fix up P and W at the end.  Then, we take the registers in
 * two 8-bit chunks, and switch on the corresponding bit pattern.  We deal
 * with the PC inline, too.
 */

/* --- Header files --- */

#include <stdio.h>

#define LOAD 0
#define STORE 1
#define INC 2
#define DEC 3

void gen3(int ls, int id, int br, char *pat, char *rbp)
{
  printf("      %s%s(&c->r[%d], %s, rbp %s %s);\n",
	 ls == LOAD ? "load" : "store", id == INC ? "Inc" : "Dec", br, pat,
	 id == INC ? "+" : "-", rbp);
}

void gen4(int ls, int id)
{
  unsigned p;
  char *op = (id == INC) ? "+" : "-";

  printf("static void %s%s (arm_word *br, arm_word pat, register arm_byte *rbv)\n{\n",
    ls == LOAD ? "load" : "store", id == INC ? "Inc" : "Dec");

  printf("  switch (pat)\n  {\n");

  for (p=0; p < 256; p++)
  {
    unsigned q, s, e, d, n;
    printf("    case %d:\n", p);
    if (id == INC) s = 0, e = 8, d = 1;
    else s = 7, e = -1, d = -1;
    for (q = s, n = 0; q != e; q += d)
    {
      if (p & (1 << q))
      {
        if (ls == LOAD)
	  printf("      br[%d] = mm_LOAD32 ((arm_word *) (rbv %s= 4));\n",
                 q, op /*, n++*/);
        else
	  printf("      mm_STORE32 ((arm_word *) (rbv %s= 4), br[%d]);\n",
                 op, /*n++,*/ q);
      }
    }
    printf("      break;\n\n");
  }
  printf("  }\n}\n\n");  
}

void gen2(int ls, int id)
{
  char *i;

  if (id == INC)
    printf("      /* --- Incrementing --- */\n\n"), i = "+";
  else
    printf("      /* --- Decrementing --- */\n\n"), i = "-";

  printf("      if (!(insn & arm_INSN_BDT_P)) rbp = rbp %s (- 4);\n\n", i);

  if (id == INC)
  {
    gen3(ls, id, 0, "insn & 0xff", "0");
    gen3(ls, id, 8, "(insn & 0xff00) >> 8", "bitsSet[insn & 0xff]");
  }
  else
  {
    gen3(ls, id, 8, "(insn & 0xff00) >> 8", "0");
    gen3(ls, id, 0, "insn & 0xff", "bitsSet[(insn & 0xff00) >> 8]");
  }

  /* --- Cope with writeback --- */

  printf("\n"
"      if (insn & arm_INSN_BDT_W)\n"
"        c->r[rb] %s= (bitsSet[insn & 0xff] + bitsSet[(insn >> 8) & 0xff])\n"
"                       << 2;\n", i);

}

void gen (int type)
{
  if (type == LOAD)
    printf("    /* --- Loading --- */\n\n");
  else
    printf("    /* --- Storing --- */\n\n");

  /* --- Do funky things if we're using the PC --- */

  if (type == LOAD) printf("    arm_word pc;\n");

  if (type == LOAD)
    /* printf("      if (insn & arm_INSN_BDT_S) pc = c->r[arm_PC];\n\n") */;
  else
  {
    printf("    if (insn & (1 << arm_PC))\n");
    printf("      c->r[arm_PC] += 12;\n\n");
  }

  /* --- Do the actual work --- */

  printf("    if (insn & arm_INSN_BDT_U)\n    {\n");
  gen2(type, INC);
  printf("    }\n    else\n    {\n");
  gen2(type, DEC);
  printf("    }\n\n");

  /* --- Further PC funkiness --- */

  if (type == LOAD)
    /* printf("      /f (insn & arm_INSN_BDT_S)\n"
           "        c->r[arm_PC] = (c->r[arm_PC] & arm_PC_ADDRESS) | \n"
           "          (pc & arm_PC_FLAGS);\n") */;
  else
  { 
    printf("    if (insn & (1 << arm_PC))\n");
    printf("      c->r[arm_PC] -= 12;\n");
  }
}

int main (int argc, char *argv[])
{
  printf(
"/* bdt.c\n"
" *\n"
" * Block data transfer instructions (from genbdt.c)\n"
" *\n"
" * (c) 2000 Chris Rutter <chris@armlinux.org>\n"
" */\n\n#include \x22""arm.h\x22\n#include \x22mm.h\x22\n\n");

  /* --- Make the bits-set table --- */

  {
    unsigned n;
    printf("static unsigned int bitsSet[] = {0\n");
    for (n = 1; n < 256; n++)
    {
      unsigned m = n, c = 0;
      while (m)
      {
        if (m & 1) c++;
        m >>= 1;
      }
      printf(", %d", c);
    }
    printf("};\n\n");
  }

  gen4(LOAD, INC); gen4(LOAD, DEC); gen4(STORE, INC); gen4(STORE, DEC);

  printf(
"void arm_executeMemBlock (arm_context *c, arm_word insn)\n"
"{\n"
"  arm_word rb = (insn & arm_INSN_RN) >> arm_INSN_RN_SHIFT;\n"
"  arm_byte *rbp = mm_translate(c->r[rb]);\n"
"\n"
"  if (insn & arm_INSN_BDT_L)\n"
"  {\n");

  gen(LOAD);

  printf(
"  }\n"
"  else\n"
"  {\n");

  gen(STORE);

  printf("  }\n\n}\n");

  return 0;
}
