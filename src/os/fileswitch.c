/* os/fileswitch.c
**
** See http://riscose.sourceforge.net/ for terms of distribution, and to
** pick up a later version of the software.
**
**   Emulation of the FileSwitch SWIs.
**
**   Template written by defmod (riscose version 1.00)
*/

#include <stdio.h>
#include <monty/monty.h>
#include "types.h"
#include "fileswitch.h"

void fileswitch_swi_register_extra(void)
{

}

/* ------------------------------------------------------------------------
 * Function:      os_bgetw()
 *
 * Description:   Reads a byte from an open file. Uses 32-bit file handle
 *
 * Input:         file - value of R1 on entry
 *
 * Output:        c - value of R0 on exit
 *                psr - processor status register on exit (X version only)
 *
 * Returns:       psr (non-X version only)
 *
 * Other notes:   Emulation of SWI 0xA.
 */

os_error *xos_bgetw (os_fw file,
      char *c,
      bits *psr)
{
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      bgetvw()
 *
 * Description:   OS_BGet vector
 *
 * Input:         file - value of R1 on entry
 *
 * Output:        c - value of R0 on exit
 *                psr - processor status register on exit (X version only)
 *
 * Returns:       psr (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x34 with R9 = 0xA.
 */

os_error *xbgetvw (os_fw file,
      char *c,
      bits *psr)
{
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_bputw()
 *
 * Description:   Writes a byte to an open file. Uses 32-bit file handle
 *
 * Input:         c - value of R0 on entry
 *                file - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0xB.
 */

os_error *xos_bputw (char c,
      os_fw file)
{
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      bputvw()
 *
 * Description:   OS_BPut vector. Uses 32-bit file handle
 *
 * Input:         c - value of R0 on entry
 *                file - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x34 with R9 = 0xB.
 */

os_error *xbputvw (char c,
      os_fw file)
{
  error("*** SWI unimplemented\n");
  return 0;
}

