/* $Id$
 *
 * Sleeve ARM emulator
 *
 * (c) 2000 Chris Rutter <chris@armlinux.org>
 */

#ifndef SLEEVE_H
#define SLEEVE_H

#ifdef __cplusplus
  extern "C" {
#endif

/* --- Header files --- */

#include "config.h"
#include <stdio.h>

/* --- @sleeve_init@ --- *
 *
 * This should be called once before the emulator is used.
 */

extern void sleeve_init (void);

/* --- Informational messages --- *
 *
 * Some fairly sparse informational messages will be sent to the stream below,
 * set to @stderr@ by default.  Set it to @NULL@ to disable them.
 */

extern FILE *sleeve_infoStream;
extern void sleeve_printf (const char *f, ...);

/* --- Debugging output --- *
 *
 * If @sleeve_DEBUG@ is @1@ upon compilation, the emulator will generate
 * debugging output in the categories listed below.  OR together the categories
 * you wish to receive into @sleeve_debugWhich@, and set @sleeve_debugStream@
 * to the stream you wish them to be sent to.
 */

enum {
  sleeve_OWNER_ALU = 1,		/* Data processing instructions */
  sleeve_OWNER_SDT = 2,		/* Single data transfers */
  sleeve_OWNER_BDT = 4,		/* Block data transfers */
  sleeve_OWNER_MUL = 8,		/* Multiplies */
  sleeve_OWNER_B = 16,		/* Branches */
  sleeve_OWNER_SWI = 32,	/* SWIs */
  sleeve_OWNER_MM = 64,		/* Memory operations */
  sleeve_OWNER_DECODE = 128	/* Instruction decoding */
};

extern void sleeve_dprintf(unsigned o, const char *f, ...);
#if sleeve_DEBUG
  extern FILE *sleeve_debugStream;
  extern unsigned sleeve_debugWhich;
#else
  #define sleeve_dprintf if (1) {} else sleeve_dprintf
#endif

/* --- Statistics collection --- *
 *
 * If @sleeve_SUPPORT_STATS@ is  @1@ upon compilation, statistics-gathering
 * code will be compiled in.
 *
 * These are simple counts of the number of each type of instruction that has
 * occured.  @sleeve_resetStats@ resets the counters to zero.
 * @sleeve_showStats@ pretty-prints the counters.
 */

extern unsigned sleeve_statsALU, sleeve_statsSDT, sleeve_statsBDT,
  sleeve_statsMul, sleeve_statsBranch, sleeve_statsSWI, sleeve_statsUndef;

#if sleeve_SUPPORT_STATS
  #define sleeve_STATS_IF
  extern void sleeve_resetStats (void);
  extern void sleeve_showStats (FILE *f);
#else
  #define sleeve_STATS_IF if (1) {} else
#endif /* sleeve_STATS */

#ifdef __cplusplus
  }
#endif

#endif /* SLEEVE_H */
