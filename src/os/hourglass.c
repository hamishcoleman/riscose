/* os/hourglass.c
**
** See http://riscose.sourceforge.net/ for terms of distribution, and to
** pick up a later version of the software.
**
**   Emulation of the Hourglass SWIs.
**
**   Template written by defmod (riscose version 1.00)
*/

#include <stdio.h>
#include <monty/monty.h>
#include "types.h"
#include "hourglass.h"

void hourglass_swi_register_extra(void)
{

}

/* ------------------------------------------------------------------------
 * Function:      hourglass_on()
 *
 * Description:   Turns on the hourglass
 *
 * Other notes:   Emulation of SWI 0x406C0.
 */

os_error *xhourglass_on (void)
{
  fprintf(stderr, "hourglass_on\n");
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      hourglass_off()
 *
 * Description:   Turns off the hourglass
 *
 * Other notes:   Emulation of SWI 0x406C1.
 */

os_error *xhourglass_off (void)
{
  fprintf(stderr, "hourglass_off\n");
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      hourglass_smash()
 *
 * Description:   Turns off the hourglass immediately
 *
 * Other notes:   Emulation of SWI 0x406C2.
 */

os_error *xhourglass_smash (void)
{
  fprintf(stderr, "hourglass_smash\n");
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      hourglass_start()
 *
 * Description:   Turns on the hourglass after a given delay
 *
 * Input:         delay - value of R0 on entry
 *
 * Other notes:   Emulation of SWI 0x406C3.
 */

os_error *xhourglass_start (int delay)
{
  fprintf(stderr, "hourglass_start\n");
  fprintf(stderr, "  In: delay = %x\n", (int) delay);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      hourglass_percentage()
 *
 * Description:   Displays a percentage below the hourglass
 *
 * Input:         percent - value of R0 on entry
 *
 * Other notes:   Emulation of SWI 0x406C4.
 */

os_error *xhourglass_percentage (int percent)
{
  fprintf(stderr, "hourglass_percentage\n");
  fprintf(stderr, "  In: percent = %x\n", (int) percent);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      hourglass_leds()
 *
 * Description:   Controls the display indicators above and below the
 *                hourglass
 *
 * Input:         eor_mask - value of R0 on entry
 *                and_mask - value of R1 on entry
 *
 * Output:        old_leds - value of R0 on exit (X version only)
 *
 * Returns:       R0 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x406C5.
 */

os_error *xhourglass_leds (bits eor_mask,
      bits and_mask,
      bits *old_leds)
{
  fprintf(stderr, "hourglass_leds\n");
  fprintf(stderr, "  In: eor_mask = %x\n", (int) eor_mask);
  fprintf(stderr, "  In: and_mask = %x\n", (int) and_mask);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: old_leds = %x\n", (int) *old_leds);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      hourglass_colours()
 *
 * Description:   Sets the colours used to display the hourglass
 *
 * Input:         sand - value of R0 on entry
 *                glass - value of R1 on entry
 *
 * Output:        old_sand - value of R0 on exit
 *                old_glass - value of R1 on exit
 *
 * Other notes:   Emulation of SWI 0x406C6.
 */

os_error *xhourglass_colours (os_colour sand,
      os_colour glass,
      os_colour *old_sand,
      os_colour *old_glass)
{
  fprintf(stderr, "hourglass_colours\n");
  fprintf(stderr, "  In: sand = %x\n", (int) sand);
  fprintf(stderr, "  In: glass = %x\n", (int) glass);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: old_sand = %x\n", (int) *old_sand);
  fprintf(stderr, "  Out: old_glass = %x\n", (int) *old_glass);
  return 0;
}

