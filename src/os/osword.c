/* os/osword.c
**
** See http://riscose.sourceforge.net/ for terms of distribution, and to
** pick up a later version of the software.
**
**   Emulation of the OSWord SWIs.
**
**   Template written by defmod (riscose version 1.00)
*/

#include <stdio.h>
#include <monty/monty.h>
#include "types.h"
#include "osword.h"

void osword_swi_register_extra(void)
{

}

/* ------------------------------------------------------------------------
 * Function:      osword_read_system_clock()
 *
 * Description:   Reads the system clock
 *
 * Input:         clock - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x7 with R0 = 0x1.
 */

os_error *xosword_read_system_clock (osword_timer_block *clock)
{
  fprintf(stderr, "osword_read_system_clock\n");
  fprintf(stderr, "  In: clock = %x\n", (int) clock);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osword_write_system_clock()
 *
 * Description:   Writes the system clock
 *
 * Input:         clock - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x7 with R0 = 0x2.
 */

os_error *xosword_write_system_clock (osword_timer_block const *clock)
{
  fprintf(stderr, "osword_write_system_clock\n");
  fprintf(stderr, "  In: clock = %x\n", (int) clock);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osword_read_interval_timer()
 *
 * Description:   Reads the interval timer
 *
 * Input:         timer - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x7 with R0 = 0x3.
 */

os_error *xosword_read_interval_timer (osword_timer_block *timer)
{
  fprintf(stderr, "osword_read_interval_timer\n");
  fprintf(stderr, "  In: timer = %x\n", (int) timer);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osword_write_interval_timer()
 *
 * Description:   Writes the interval timer
 *
 * Input:         timer - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x7 with R0 = 0x4.
 */

os_error *xosword_write_interval_timer (osword_timer_block const *timer)
{
  fprintf(stderr, "osword_write_interval_timer\n");
  fprintf(stderr, "  In: timer = %x\n", (int) timer);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osword_read_char_definition()
 *
 * Description:   Reads a character definition - Client must partly
 *                initialise the parameter block
 *
 * Input:         definition - Partly filled in on entry
 *
 * Other notes:   Emulation of SWI 0x7 with R0 = 0xA.
 */

os_error *xosword_read_char_definition (osword_char_definition_block *definition)
{
  fprintf(stderr, "osword_read_char_definition\n");
  fprintf(stderr, "  In: definition = %x\n", (int) definition);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osword_read_palette()
 *
 * Description:   Reads the palette - Client must partly initialise the
 *                parameter block - prefer ColourTrans_ReadPalette
 *
 * Input:         entry - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x7 with R0 = 0xB.
 */

os_error *xosword_read_palette (osword_palette_block *entry)
{
  fprintf(stderr, "osword_read_palette\n");
  fprintf(stderr, "  In: entry = %x\n", (int) entry);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osword_write_palette()
 *
 * Description:   Writes the palette - Client must initialise the parameter
 *                block - prefer ColourTrans_WritePalette
 *
 * Input:         entry - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x7 with R0 = 0xC.
 */

os_error *xosword_write_palette (osword_palette_block const *entry)
{
  fprintf(stderr, "osword_write_palette\n");
  fprintf(stderr, "  In: entry = %x\n", (int) entry);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osword_read_cursor_position()
 *
 * Description:   Reads the current and previous graphics cursor positions
 *                - prefer OS_ReadVduVariables
 *
 * Input:         position - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x7 with R0 = 0xD.
 */

os_error *xosword_read_cursor_position (osword_cursor_position_block *position)
{
  fprintf(stderr, "osword_read_cursor_position\n");
  fprintf(stderr, "  In: position = %x\n", (int) position);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      oswordreadclock_local_string()
 *
 * Description:   Reads the real-time clock as a string, converting to
 *                local time - Client must partly initialise the parameter
 *                block
 *
 * Input:         string - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x7 with R0 = 0xE.
 */

os_error *xoswordreadclock_local_string (oswordreadclock_local_string_block *string)
{
  fprintf(stderr, "oswordreadclock_local_string\n");
  fprintf(stderr, "  In: string = %x\n", (int) string);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      oswordreadclock_local_bcd()
 *
 * Description:   Reads the real-time clock in BCD format, converting to
 *                local time - Client must partly initialise the parameter
 *                block
 *
 * Input:         bcd - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x7 with R0 = 0xE.
 */

os_error *xoswordreadclock_local_bcd (oswordreadclock_local_bcd_block *bcd)
{
  fprintf(stderr, "oswordreadclock_local_bcd\n");
  fprintf(stderr, "  In: bcd = %x\n", (int) bcd);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      oswordreadclock_convert_bcd_to_string()
 *
 * Description:   Converts a BCD clock value into string format - Client
 *                must partly initialise the parameter block
 *
 * Input:         convert_bcd_to_String - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x7 with R0 = 0xE.
 */

os_error *xoswordreadclock_convert_bcd_to_string (oswordreadclock_convert_bcd_to_string_block *convert_bcd_to_String)
{
  fprintf(stderr, "oswordreadclock_convert_bcd_to_string\n");
  fprintf(stderr, "  In: convert_bcd_to_String = %x\n", (int) convert_bcd_to_String);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      oswordreadclock_utc()
 *
 * Description:   Reads the real-time clock in 5-byte format - Client must
 *                partly initialise the parameter block
 *
 * Input:         utc - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x7 with R0 = 0xE.
 */

os_error *xoswordreadclock_utc (oswordreadclock_utc_block *utc)
{
  fprintf(stderr, "oswordreadclock_utc\n");
  fprintf(stderr, "  In: utc = %x\n", (int) utc);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      oswordwriteclock_time_local_string()
 *
 * Description:   Writes the real-time clock with a time string, converting
 *                from local time - Client must partly initialise the
 *                parameter block
 *
 * Input:         time - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x7 with R0 = 0xF.
 */

os_error *xoswordwriteclock_time_local_string (oswordwriteclock_block *time)
{
  fprintf(stderr, "oswordwriteclock_time_local_string\n");
  fprintf(stderr, "  In: time = %x\n", (int) time);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      oswordwriteclock_date_local_string()
 *
 * Description:   Writes the real-time clock with a date string, converting
 *                from local time - Client must partly initialise the
 *                parameter block
 *
 * Input:         date - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x7 with R0 = 0xF.
 */

os_error *xoswordwriteclock_date_local_string (oswordwriteclock_block *date)
{
  fprintf(stderr, "oswordwriteclock_date_local_string\n");
  fprintf(stderr, "  In: date = %x\n", (int) date);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      oswordwriteclock_date_and_time_local_string()
 *
 * Description:   Writes the real-time clock with a time and date string,
 *                converting from local time - Client must partly
 *                initialise the parameter block
 *
 * Input:         date_and_time - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x7 with R0 = 0xF.
 */

os_error *xoswordwriteclock_date_and_time_local_string (oswordwriteclock_block *date_and_time)
{
  fprintf(stderr, "oswordwriteclock_date_and_time_local_string\n");
  fprintf(stderr, "  In: date_and_time = %x\n", (int) date_and_time);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      oswordpointer_define()
 *
 * Description:   Defines the pointer size, shape and active point - Client
 *                must partly initialise the parameter block - prefer
 *                OSSpriteOp_SetPointerShape
 *
 * Input:         define - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x7 with R0 = 0x15, R1 += 0x2.
 */

os_error *xoswordpointer_define (oswordpointer_define_block *define)
{
  fprintf(stderr, "oswordpointer_define\n");
  fprintf(stderr, "  In: define = %x\n", (int) define);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      oswordpointer_set_bbox()
 *
 * Description:   Defines the mouse coordinate bounding box - Client must
 *                partly initialise the parameter block
 *
 * Input:         bbox - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x7 with R0 = 0x15, R1 += 0x3.
 */

os_error *xoswordpointer_set_bbox (oswordpointer_bbox_block *bbox)
{
  fprintf(stderr, "oswordpointer_set_bbox\n");
  fprintf(stderr, "  In: bbox = %x\n", (int) bbox);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      oswordpointer_step()
 *
 * Description:   Defines the mouse multipliers. - Client must partly
 *                initialise the parameter block
 *
 * Input:         step - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x7 with R0 = 0x15.
 */

os_error *xoswordpointer_step (oswordpointer_step_block const *step)
{
  fprintf(stderr, "oswordpointer_step\n");
  fprintf(stderr, "  In: step = %x\n", (int) step);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      oswordpointer_set_position()
 *
 * Description:   Sets the mouse position. - Client must partly initialise
 *                the parameter block
 *
 * Input:         pos - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x7 with R0 = 0x15, R1 += 0x1.
 */

os_error *xoswordpointer_set_position (oswordpointer_position_block *pos)
{
  fprintf(stderr, "oswordpointer_set_position\n");
  fprintf(stderr, "  In: pos = %x\n", (int) pos);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      oswordpointer_read_position()
 *
 * Description:   Reads the unbuffered mouse position. - Client must partly
 *                initialise the parameter block
 *
 * Input:         pos - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x7 with R0 = 0x15, R1 += 0x1.
 */

os_error *xoswordpointer_read_position (oswordpointer_position_block *pos)
{
  fprintf(stderr, "oswordpointer_read_position\n");
  fprintf(stderr, "  In: pos = %x\n", (int) pos);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      oswordpointer_set_unlinked_position()
 *
 * Description:   Sets the pointer position. - Client must partly
 *                initialise the parameter block
 *
 * Input:         pos - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x7 with R0 = 0x15, R1 += 0x1.
 */

os_error *xoswordpointer_set_unlinked_position (oswordpointer_position_block *pos)
{
  fprintf(stderr, "oswordpointer_set_unlinked_position\n");
  fprintf(stderr, "  In: pos = %x\n", (int) pos);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      oswordpointer_read_unlinked_position()
 *
 * Description:   Reads the pointer position. - Client must partly
 *                initialise the parameter block
 *
 * Input:         pos - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x7 with R0 = 0x15, R1 += 0x1.
 */

os_error *xoswordpointer_read_unlinked_position (oswordpointer_position_block *pos)
{
  fprintf(stderr, "oswordpointer_read_unlinked_position\n");
  fprintf(stderr, "  In: pos = %x\n", (int) pos);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osword_write_screen_base()
 *
 * Description:   Writes the screen base address. - Client must partly
 *                initialise the parameter block
 *
 * Input:         screen_base - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x7 with R0 = 0x16, R1 += 0x3.
 */

os_error *xosword_write_screen_base (osword_screen_base_block *screen_base)
{
  fprintf(stderr, "osword_write_screen_base\n");
  fprintf(stderr, "  In: screen_base = %x\n", (int) screen_base);
  error("*** SWI unimplemented\n");
  return 0;
}

