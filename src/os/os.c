/* os/os.c
**
** See http://riscose.sourceforge.net/ for terms of distribution, and to
** pick up a later version of the software.
**
**   Emulation of the OS SWIs.
**
**   Template written by defmod (riscose version 1.00)
*/

#include <stdio.h>
#include <stdlib.h>
#include <monty/monty.h>
#include "types.h"
#include "os.h"

/* riscose */
#include "vdu.h"
#include "map.h"
#include "mem.h"
#include "arm.h"

WORD os_call_a_swi(WORD n)
{
  swi_trap(ARM_R10);
  return 0;
}

WORD os_call_a_swi_r12(WORD n)
{
  swi_trap(ARM_R12);
  return 0;
}

WORD os_write_s(WORD n)
{
  WORD pc = ARM_R15 & 0x02fffffc;
  char* p = (char *)MEM_TOHOST(pc);
  while (*p)
  {
    vdu(*(p++));
    pc++;
  }
  pc = (pc + 3) & ~3;
  arm_set_pc(pc);

  return 0;
}

void os_swi_register_extra(void)
{
  swi_register(0x1, "OS_WriteS", os_write_s);
  swi_register(0x6f, "OS_CallASWI", os_call_a_swi);
  swi_register(0x71, "OS_CallASWIR12", os_call_a_swi_r12);
}




/* ------------------------------------------------------------------------
 * Function:      os_read_var_val_size()
 *
 * Description:   Gets size of system variable or checks for its existance
 *                (PRM 1-309, 5a-661)
 *
 * Input:         var - value of R0 on entry
 *                context - value of R3 on entry
 *                var_type - value of R4 on entry
 *
 * Output:        used - 0 if not found or, if var_type_out != 3, then NOT the number of bytes required for the variable
 *                context_out - value of R3 on exit (X version only)
 *                var_type_out - value of R4 on exit
 *
 * Returns:       R3 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x23 with R1 = 0x0, R2 = 0x80000000.
 */

os_error *xos_read_var_val_size (char const *var,
      int context,
      os_var_type var_type,
      int *used,
      int *context_out,
      os_var_type *var_type_out)
{
  fprintf(stderr, "os_read_var_val_size\n");
  fprintf(stderr, "  In: var = %x\n", (int) var);
  fprintf(stderr, "  In: context = %x\n", (int) context);
  fprintf(stderr, "  In: var_type = %x\n", (int) var_type);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: used = %x\n", (int) *used);
  fprintf(stderr, "  Out: context_out = %x\n", (int) *context_out);
  fprintf(stderr, "  Out: var_type_out = %x\n", (int) *var_type_out);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_writec()
 *
 * Description:   Writes a character to all of the active output streams
 *
 * Input:         c - value of R0 on entry
 *
 * Other notes:   Emulation of SWI 0x0.
 */

os_error *xos_writec (char c)
{
  fprintf(stderr, "os_writec\n");
  fprintf(stderr, "  In: c = %x\n", (int) c);

  vdu(c);

  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_write0()
 *
 * Description:   Writes a string to all of the active output streams
 *
 * Input:         s - value of R0 on entry
 *
 * Other notes:   Emulation of SWI 0x2.
 */

os_error *xos_write0 (char const *s)
{
  fprintf(stderr, "os_write0\n");
  fprintf(stderr, "  In: s = %x\n", (int) s);

  while (*s)
    vdu(*(s++));

  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_new_line()
 *
 * Description:   Writes a line feed followed by a carriage return to all
 *                of the active output streams
 *
 * Other notes:   Emulation of SWI 0x3.
 */

os_error *xos_new_line (void)
{
  fprintf(stderr, "os_new_line\n");

  vdu(10);
  vdu(13);

  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_readc()
 *
 * Description:   Reads a character from the input stream
 *
 * Output:        c - value of R0 on exit
 *                psr - processor status register on exit (X version only)
 *
 * Returns:       psr (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x4.
 */

os_error *xos_readc (char *c,
      bits *psr)
{
  fprintf(stderr, "os_readc\n");
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: c = %x\n", (int) *c);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_cli()
 *
 * Description:   Processes a supervisor command
 *
 * Input:         command - value of R0 on entry
 *
 * Other notes:   Emulation of SWI 0x5.
 */

os_error *xos_cli (char const *command)
{
  fprintf(stderr, "os_cli\n");
  fprintf(stderr, "  In: command = %x\n", (int) command);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_read_line()
 *
 * Description:   Reads a line from the input stream
 *
 * Input:         buffer - value of R0 on entry
 *                size - value of R1 on entry
 *                min_char - value of R2 on entry
 *                max_char - value of R3 on entry
 *
 * Output:        used - value of R1 on exit
 *                psr - processor status register on exit (X version only)
 *
 * Returns:       psr (non-X version only)
 *
 * Other notes:   Emulation of SWI 0xE.
 */

os_error *xos_read_line (char *buffer,
      int size,
      char min_char,
      char max_char,
      int *used,
      bits *psr)
{
  char* l;
  int len;

  fprintf(stderr, "os_read_line\n");
  fprintf(stderr, "  In: buffer = %x\n", (int) buffer);
  fprintf(stderr, "  In: size = %x\n", (int) size);
  fprintf(stderr, "  In: min_char = %x\n", (int) min_char);
  fprintf(stderr, "  In: max_char = %x\n", (int) max_char);

  /* FIXME: not quite right! */
  l = readline(NULL);
  strncpy(buffer, l, size);

  len = strlen(l);
  if (len < size)
    buffer[len] = '\r';

  *used = len;
  free(l);

  if (psr)
    *psr = 0;

  fprintf(stderr, "  Out: used = %x\n", (int) *used);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_read_line_given_echo()
 *
 * Description:   Reads a line from the input stream, using the given echo
 *                character
 *
 * Input:         buffer - value of R0 on entry
 *                size - value of R1 on entry
 *                min_char - value of R2 on entry
 *                max_char - value of R3 on entry
 *                echo - value of R4 on entry
 *
 * Output:        used - value of R1 on exit
 *                psr - processor status register on exit (X version only)
 *
 * Returns:       psr (non-X version only)
 *
 * Other notes:   Emulation of SWI 0xE with R0 |= 0x40000000.
 */

os_error *xos_read_line_given_echo (char *buffer,
      int size,
      char min_char,
      char max_char,
      char echo,
      int *used,
      bits *psr)
{
  fprintf(stderr, "os_read_line_given_echo\n");
  fprintf(stderr, "  In: buffer = %x\n", (int) buffer);
  fprintf(stderr, "  In: size = %x\n", (int) size);
  fprintf(stderr, "  In: min_char = %x\n", (int) min_char);
  fprintf(stderr, "  In: max_char = %x\n", (int) max_char);
  fprintf(stderr, "  In: echo = %x\n", (int) echo);

  /* FIXME --- not sure if there should be two readline SWIs in the .osl */
  return xos_read_line(buffer, size, min_char, max_char, used, psr);

  /* FIXME --- unreachable */
  fprintf(stderr, "  Out: used = %x\n", (int) *used);
}

/* ------------------------------------------------------------------------
 * Function:      os_read_line_suppress_invalid()
 *
 * Description:   Reads a line from the input stream, not echoing invalid
 *                characters
 *
 * Input:         buffer - value of R0 on entry
 *                size - value of R1 on entry
 *                min_char - value of R2 on entry
 *                max_char - value of R3 on entry
 *
 * Output:        used - value of R1 on exit
 *                psr - processor status register on exit (X version only)
 *
 * Returns:       psr (non-X version only)
 *
 * Other notes:   Emulation of SWI 0xE with R0 |= 0x80000000.
 */

os_error *xos_read_line_suppress_invalid (char *buffer,
      int size,
      char min_char,
      char max_char,
      int *used,
      bits *psr)
{
  fprintf(stderr, "os_read_line_suppress_invalid\n");
  fprintf(stderr, "  In: buffer = %x\n", (int) buffer);
  fprintf(stderr, "  In: size = %x\n", (int) size);
  fprintf(stderr, "  In: min_char = %x\n", (int) min_char);
  fprintf(stderr, "  In: max_char = %x\n", (int) max_char);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: used = %x\n", (int) *used);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_read_line_given_echo_suppress_invalid()
 *
 * Description:   Reads a line from the input stream, using the given echo
 *                character and not echoing invalid characters
 *
 * Input:         buffer - value of R0 on entry
 *                size - value of R1 on entry
 *                min_char - value of R2 on entry
 *                max_char - value of R3 on entry
 *                echo - value of R4 on entry
 *
 * Output:        used - value of R1 on exit
 *                psr - processor status register on exit (X version only)
 *
 * Returns:       psr (non-X version only)
 *
 * Other notes:   Emulation of SWI 0xE with R0 |= 0xC0000000.
 */

os_error *xos_read_line_given_echo_suppress_invalid (char *buffer,
      int size,
      char min_char,
      char max_char,
      char echo,
      int *used,
      bits *psr)
{
  fprintf(stderr, "os_read_line_given_echo_suppress_invalid\n");
  fprintf(stderr, "  In: buffer = %x\n", (int) buffer);
  fprintf(stderr, "  In: size = %x\n", (int) size);
  fprintf(stderr, "  In: min_char = %x\n", (int) min_char);
  fprintf(stderr, "  In: max_char = %x\n", (int) max_char);
  fprintf(stderr, "  In: echo = %x\n", (int) echo);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: used = %x\n", (int) *used);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_control()
 *
 * Description:   Reads/writes handler addresses - prefer
 *                OS_ChangeEnvironment
 *
 * Input:         error_handler - value of R0 on entry
 *                error_buffer - value of R1 on entry
 *                escape_handler - value of R2 on entry
 *                event_handler - value of R3 on entry
 *
 * Output:        old_error_handler - value of R0 on exit
 *                old_error_buffer - value of R1 on exit
 *                old_escape_handler - value of R2 on exit
 *                old_event_handler - value of R3 on exit
 *
 * Other notes:   Emulation of SWI 0xF.
 */

os_error *xos_control (void const *error_handler,
      os_error *error_buffer,
      void const *escape_handler,
      void const *event_handler,
      void **old_error_handler,
      os_error **old_error_buffer,
      void **old_escape_handler,
      void **old_event_handler)
{
  fprintf(stderr, "os_control\n");
  fprintf(stderr, "  In: error_handler = %x\n", (int) error_handler);
  fprintf(stderr, "  In: error_buffer = %x\n", (int) error_buffer);
  fprintf(stderr, "  In: escape_handler = %x\n", (int) escape_handler);
  fprintf(stderr, "  In: event_handler = %x\n", (int) event_handler);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: old_error_handler = %x\n", (int) *old_error_handler);
  fprintf(stderr, "  Out: old_error_buffer = %x\n", (int) *old_error_buffer);
  fprintf(stderr, "  Out: old_escape_handler = %x\n", (int) *old_escape_handler);
  fprintf(stderr, "  Out: old_event_handler = %x\n", (int) *old_event_handler);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_get_env()
 *
 * Description:   Reads environment parameters
 *
 * Output:        command - value of R0 on exit (X version only)
 *                ram_limit - value of R1 on exit
 *                start - value of R2 on exit
 *
 * Returns:       R0 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x10.
 */

os_error *xos_get_env (char **command,
      byte **ram_limit,
      os_date_and_time **start)
{
  fprintf(stderr, "os_get_env\n");

  *command = MMAP_USRSTACK_BASE;
  *ram_limit = 0x8000 + mem_get_wimpslot();
  *start = MMAP_USRSTACK_BASE + 256;

  fprintf(stderr, "  Out: command = %x\n", (int) *command);
  fprintf(stderr, "  Out: ram_limit = %x\n", (int) *ram_limit);
  fprintf(stderr, "  Out: start = %x\n", (int) *start);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_exit()
 *
 * Description:   Passes control to the most recent exit handler
 *
 * Input:         e - value of R0 on entry
 *                rc - value of R2 on entry
 *
 * Other notes:   Emulation of SWI 0x11 with R1 = 0x58454241.
 */

os_error *xos_exit (os_error const *e,
      int rc)
{
  fprintf(stderr, "os_exit\n");
  fprintf(stderr, "  In: e = %x\n", (int) e);
  fprintf(stderr, "  In: rc = %x\n", (int) rc);

  exit(e);

  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_set_env()
 *
 * Description:   Sets environment parameters - prefer OS_ChangeEnvironment
 *
 * Input:         exit_handler - value of R0 on entry
 *                ram_limit - value of R1 on entry
 *                undefined_instruction_handler - value of R4 on entry
 *                prefetch_abort_handler - value of R5 on entry
 *                data_abort_handler - value of R6 on entry
 *                address_exception_handler - value of R7 on entry
 *
 * Output:        old_exit_handler - value of R0 on exit
 *                old_ram_limit - value of R1 on exit
 *                old_undefined_instruction_handler - value of R4 on exit
 *                old_prefetch_abort_handler - value of R5 on exit
 *                old_data_abort_handler - value of R6 on exit
 *                old_address_exception_handler - value of R7 on exit
 *
 * Other notes:   Emulation of SWI 0x12.
 */

os_error *xos_set_env (void const *exit_handler,
      byte *ram_limit,
      void const *undefined_instruction_handler,
      void const *prefetch_abort_handler,
      void const *data_abort_handler,
      void const *address_exception_handler,
      void **old_exit_handler,
      byte **old_ram_limit,
      void **old_undefined_instruction_handler,
      void **old_prefetch_abort_handler,
      void **old_data_abort_handler,
      void **old_address_exception_handler)
{
  fprintf(stderr, "os_set_env\n");
  fprintf(stderr, "  In: exit_handler = %x\n", (int) exit_handler);
  fprintf(stderr, "  In: ram_limit = %x\n", (int) ram_limit);
  fprintf(stderr, "  In: undefined_instruction_handler = %x\n", (int) undefined_instruction_handler);
  fprintf(stderr, "  In: prefetch_abort_handler = %x\n", (int) prefetch_abort_handler);
  fprintf(stderr, "  In: data_abort_handler = %x\n", (int) data_abort_handler);
  fprintf(stderr, "  In: address_exception_handler = %x\n", (int) address_exception_handler);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: old_exit_handler = %x\n", (int) *old_exit_handler);
  fprintf(stderr, "  Out: old_ram_limit = %x\n", (int) *old_ram_limit);
  fprintf(stderr, "  Out: old_undefined_instruction_handler = %x\n", (int) *old_undefined_instruction_handler);
  fprintf(stderr, "  Out: old_prefetch_abort_handler = %x\n", (int) *old_prefetch_abort_handler);
  fprintf(stderr, "  Out: old_data_abort_handler = %x\n", (int) *old_data_abort_handler);
  fprintf(stderr, "  Out: old_address_exception_handler = %x\n", (int) *old_address_exception_handler);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_int_on()
 *
 * Description:   Enables interrupts - cannot be called via APCS26
 *
 * Other notes:   Emulation of SWI 0x13.
 */

os_error *xos_int_on (void)
{
  fprintf(stderr, "os_int_on\n");
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_int_off()
 *
 * Description:   Disables interrupts - cannot be called via APCS26
 *
 * Other notes:   Emulation of SWI 0x14.
 */

os_error *xos_int_off (void)
{
  fprintf(stderr, "os_int_off\n");
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_call_back()
 *
 * Description:   Sets up the callback handler - prefer
 *                OS_ChangeEnvironment
 *
 * Input:         register_block - value of R0 on entry
 *                call_back_handler - value of R1 on entry
 *
 * Output:        old_register_block - value of R0 on exit
 *                old_call_back_handler - value of R1 on exit
 *
 * Other notes:   Emulation of SWI 0x15.
 */

os_error *xos_call_back (os_register_block const *register_block,
      void const *call_back_handler,
      os_register_block **old_register_block,
      void **old_call_back_handler)
{
  fprintf(stderr, "os_call_back\n");
  fprintf(stderr, "  In: register_block = %x\n", (int) register_block);
  fprintf(stderr, "  In: call_back_handler = %x\n", (int) call_back_handler);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: old_register_block = %x\n", (int) *old_register_block);
  fprintf(stderr, "  Out: old_call_back_handler = %x\n", (int) *old_call_back_handler);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_break_pt()
 *
 * Description:   Causes a break point trap to occur and the break point
 *                handler to be entered
 *
 * Other notes:   Emulation of SWI 0x17.
 */

os_error *xos_break_pt (void)
{
  fprintf(stderr, "os_break_pt\n");
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_break_ctrl()
 *
 * Description:   Sets up the break point handler - prefer
 *                OS_ChangeEnvironment
 *
 * Input:         register_block - value of R0 on entry
 *                break_pt_handler - value of R1 on entry
 *
 * Output:        old_register_block - value of R0 on exit
 *                old_break_pt_handler - value of R1 on exit
 *
 * Other notes:   Emulation of SWI 0x18.
 */

os_error *xos_break_ctrl (os_register_block *register_block,
      void const *break_pt_handler,
      os_register_block **old_register_block,
      void **old_break_pt_handler)
{
  fprintf(stderr, "os_break_ctrl\n");
  fprintf(stderr, "  In: register_block = %x\n", (int) register_block);
  fprintf(stderr, "  In: break_pt_handler = %x\n", (int) break_pt_handler);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: old_register_block = %x\n", (int) *old_register_block);
  fprintf(stderr, "  Out: old_break_pt_handler = %x\n", (int) *old_break_pt_handler);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_unused_swi()
 *
 * Description:   Sets up the handler for unused SWI's - prefer
 *                OS_ChangeEnvironment
 *
 * Input:         unused_swi_handler - value of R0 on entry
 *
 * Output:        old_unused_swi_handler - value of R0 on exit (X version only)
 *
 * Returns:       R0 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x19.
 */

os_error *xos_unused_swi (void const *unused_swi_handler,
      void **old_unused_swi_handler)
{
  fprintf(stderr, "os_unused_swi\n");
  fprintf(stderr, "  In: unused_swi_handler = %x\n", (int) unused_swi_handler);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: old_unused_swi_handler = %x\n", (int) *old_unused_swi_handler);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_update_memc()
 *
 * Description:   Reads or alters the contents of the MEMC control register
 *
 * Input:         value - value of R0 on entry
 *                mask - value of R1 on entry
 *
 * Output:        old_value - value of R0 on exit (X version only)
 *
 * Returns:       R0 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x1A.
 */

os_error *xos_update_memc (bits value,
      bits mask,
      bits *old_value)
{
  fprintf(stderr, "os_update_memc\n");
  fprintf(stderr, "  In: value = %x\n", (int) value);
  fprintf(stderr, "  In: mask = %x\n", (int) mask);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: old_value = %x\n", (int) *old_value);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_set_call_back()
 *
 * Description:   Causes a call to the callback handler
 *
 * Other notes:   Emulation of SWI 0x1B.
 */

os_error *xos_set_call_back (void)
{
  fprintf(stderr, "os_set_call_back\n");
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_mouse()
 *
 * Description:   Reads a mouse state from the buffer
 *
 * Output:        x - value of R0 on exit
 *                y - value of R1 on exit
 *                buttons - value of R2 on exit
 *                t - value of R3 on exit
 *
 * Other notes:   Emulation of SWI 0x1C.
 */

os_error *xos_mouse (int *x,
      int *y,
      bits *buttons,
      os_t *t)
{
  fprintf(stderr, "os_mouse\n");
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: x = %x\n", (int) *x);
  fprintf(stderr, "  Out: y = %x\n", (int) *y);
  fprintf(stderr, "  Out: buttons = %x\n", (int) *buttons);
  fprintf(stderr, "  Out: t = %x\n", (int) *t);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_claim()
 *
 * Description:   Adds a routine to the list of those that claim a vector
 *
 * Input:         vector - value of R0 on entry
 *                routine - value of R1 on entry
 *                handle - value of R2 on entry
 *
 * Other notes:   Emulation of SWI 0x1F.
 */

os_error *xos_claim (int vector,
      void const *routine,
      byte *handle)
{
  fprintf(stderr, "os_claim\n");
  fprintf(stderr, "  In: vector = %x\n", (int) vector);
  fprintf(stderr, "  In: routine = %x\n", (int) routine);
  fprintf(stderr, "  In: handle = %x\n", (int) handle);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_release()
 *
 * Description:   Removes a routine from the list of those that claim a
 *                vector
 *
 * Input:         vector - value of R0 on entry
 *                routine - value of R1 on entry
 *                handle - value of R2 on entry
 *
 * Other notes:   Emulation of SWI 0x20.
 */

os_error *xos_release (int vector,
      void const *routine,
      byte *handle)
{
  fprintf(stderr, "os_release\n");
  fprintf(stderr, "  In: vector = %x\n", (int) vector);
  fprintf(stderr, "  In: routine = %x\n", (int) routine);
  fprintf(stderr, "  In: handle = %x\n", (int) handle);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_read_unsigned()
 *
 * Description:   Converts a string to an unsigned number
 *
 * Input:         flags - value of R0 on entry
 *                number - value of R1 on entry
 *                limit - value of R2 on entry
 *
 * Output:        end - value of R1 on exit
 *                value - value of R2 on exit (X version only)
 *
 * Returns:       R2 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x21.
 */

os_error *xos_read_unsigned (os_read_unsigned_flags flags,
      char const *number,
      bits limit,
      char **end,
      bits *value)
{
  fprintf(stderr, "os_read_unsigned\n");
  fprintf(stderr, "  In: flags = %x\n", (int) flags);
  fprintf(stderr, "  In: number = %x\n", (int) number);
  fprintf(stderr, "  In: limit = %x\n", (int) limit);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: end = %x\n", (int) *end);
  fprintf(stderr, "  Out: value = %x\n", (int) *value);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_read_var_val()
 *
 * Description:   Reads a variable value (PRM 1-309, 5a-661) - Prefer
 *                OS_ReadVarValSize to read size of variable
 *
 * Input:         var - value of R0 on entry
 *                value - value of R1 on entry
 *                size - value of R2 on entry
 *                context - value of R3 on entry
 *                var_type - value of R4 on entry
 *
 * Output:        used - value of R2 on exit
 *                context_out - value of R3 on exit (X version only)
 *                var_type_out - value of R4 on exit
 *
 * Returns:       R3 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x23.
 */

os_error *xos_read_var_val (char const *var,
      char *value,
      int size,
      int context,
      os_var_type var_type,
      int *used,
      int *context_out,
      os_var_type *var_type_out)
{
  fprintf(stderr, "os_read_var_val\n");
  fprintf(stderr, "  In: var = %x\n", (int) var);
  fprintf(stderr, "  In: value = %x\n", (int) value);
  fprintf(stderr, "  In: size = %x\n", (int) size);
  fprintf(stderr, "  In: context = %x\n", (int) context);
  fprintf(stderr, "  In: var_type = %x\n", (int) var_type);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: used = %x\n", (int) *used);
  fprintf(stderr, "  Out: context_out = %x\n", (int) *context_out);
  fprintf(stderr, "  Out: var_type_out = %x\n", (int) *var_type_out);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_set_var_val()
 *
 * Description:   Writes a variable value
 *
 * Input:         var - value of R0 on entry
 *                value - value of R1 on entry
 *                size - value of R2 on entry
 *                context - value of R3 on entry
 *                var_type - value of R4 on entry
 *
 * Output:        context_out - value of R3 on exit (X version only)
 *                var_type_out - value of R4 on exit
 *
 * Returns:       R3 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x24.
 */

os_error *xos_set_var_val (char const *var,
      byte const *value,
      int size,
      int context,
      os_var_type var_type,
      int *context_out,
      os_var_type *var_type_out)
{
  fprintf(stderr, "os_set_var_val\n");
  fprintf(stderr, "  In: var = %x\n", (int) var);
  fprintf(stderr, "  In: value = %x\n", (int) value);
  fprintf(stderr, "  In: size = %x\n", (int) size);
  fprintf(stderr, "  In: context = %x\n", (int) context);
  fprintf(stderr, "  In: var_type = %x\n", (int) var_type);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: context_out = %x\n", (int) *context_out);
  fprintf(stderr, "  Out: var_type_out = %x\n", (int) *var_type_out);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_gs_init()
 *
 * Description:   Initialises context for use by OS_GSRead
 *
 * Input:         s - value of R0 on entry
 *                flags - value of R2 on entry
 *
 * Output:        context1 - value of R0 on exit
 *                context2 - value of R2 on exit
 *
 * Other notes:   Emulation of SWI 0x25.
 */

os_error *xos_gs_init (char const *s,
      os_gs_flags flags,
      int *context1,
      int *context2)
{
  fprintf(stderr, "os_gs_init\n");
  fprintf(stderr, "  In: s = %x\n", (int) s);
  fprintf(stderr, "  In: flags = %x\n", (int) flags);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: context1 = %x\n", (int) *context1);
  fprintf(stderr, "  Out: context2 = %x\n", (int) *context2);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_gs_read()
 *
 * Description:   Returns a character from a string which has been
 *                initialised by OS_GSInit
 *
 * Input:         context1 - value of R0 on entry
 *                context2 - value of R2 on entry
 *
 * Output:        context1_out - value of R0 on exit
 *                c - value of R1 on exit
 *                context2_out - value of R2 on exit
 *                psr - processor status register on exit (X version only)
 *
 * Returns:       psr (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x26.
 */

os_error *xos_gs_read (int context1,
      int context2,
      int *context1_out,
      char *c,
      int *context2_out,
      bits *psr)
{
  fprintf(stderr, "os_gs_read\n");
  fprintf(stderr, "  In: context1 = %x\n", (int) context1);
  fprintf(stderr, "  In: context2 = %x\n", (int) context2);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: context1_out = %x\n", (int) *context1_out);
  fprintf(stderr, "  Out: c = %x\n", (int) *c);
  fprintf(stderr, "  Out: context2_out = %x\n", (int) *context2_out);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_gs_trans()
 *
 * Description:   Equivalent to a call to OS_GSInit and repeated calls to
 *                OS_GSRead
 *
 * Input:         s - value of R0 on entry
 *                buffer - value of R1 on entry
 *                size - value of R2 on entry
 *
 * Output:        used - value of R2 on exit
 *                psr - processor status register on exit (X version only)
 *
 * Returns:       psr (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x27.
 */

os_error *xos_gs_trans (char const *s,
      char *buffer,
      int size,
      int *used,
      bits *psr)
{
  fprintf(stderr, "os_gs_trans\n");
  fprintf(stderr, "  In: s = %x\n", (int) s);
  fprintf(stderr, "  In: buffer = %x\n", (int) buffer);
  fprintf(stderr, "  In: size = %x\n", (int) size);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: used = %x\n", (int) *used);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_binary_to_decimal()
 *
 * Description:   Converts a signed number to a string
 *
 * Input:         i - value of R0 on entry
 *                buffer - value of R1 on entry
 *                size - value of R2 on entry
 *
 * Output:        used - value of R2 on exit (X version only)
 *
 * Returns:       R2 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x28.
 */

os_error *xos_binary_to_decimal (int i,
      char *buffer,
      int size,
      int *used)
{
  fprintf(stderr, "os_binary_to_decimal\n");
  fprintf(stderr, "  In: i = %x\n", (int) i);
  fprintf(stderr, "  In: buffer = %x\n", (int) buffer);
  fprintf(stderr, "  In: size = %x\n", (int) size);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: used = %x\n", (int) *used);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_change_dynamic_area()
 *
 * Description:   Alters the space allocation of a dynamic area
 *
 * Input:         area_type - value of R0 on entry
 *                change - value of R1 on entry
 *
 * Output:        change_out - value of R1 on exit (X version only)
 *
 * Returns:       R1 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x2A.
 */

os_error *xos_change_dynamic_area (os_dynamic_area_no area_type,
      int change,
      int *change_out)
{
  fprintf(stderr, "os_change_dynamic_area\n");
  fprintf(stderr, "  In: area_type = %x\n", (int) area_type);
  fprintf(stderr, "  In: change = %x\n", (int) change);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: change_out = %x\n", (int) *change_out);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_generate_error()
 *
 * Description:   Generates an error and invokes the error handler
 *
 * Input:         e - value of R0 on entry
 *
 * Other notes:   Emulation of SWI 0x2B.
 */

os_error *xos_generate_error (os_error const *e)
{
  fprintf(stderr, "os_generate_error\n");
  fprintf(stderr, "  In: e = %x\n", (int) e);

  fprintf(stderr, "*** Error from RISC OS (%08x): %s\n", e->errnum, e->errmess);

  /*  exit(1);  */ /* FIXME: Sort the error handlers! */

  return e;
}

/* ------------------------------------------------------------------------
 * Function:      os_read_escape_state()
 *
 * Description:   Checks whether an escape condition has occurred
 *
 * Output:        psr - processor status register on exit (X version only)
 *
 * Returns:       psr (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x2C.
 */

os_error *xos_read_escape_state (bits *psr)
{
  fprintf(stderr, "os_read_escape_state\n");
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_evaluate_expression()
 *
 * Description:   Evaluates a string expression and returns an integer or
 *                string result
 *
 * Input:         expr - value of R0 on entry
 *                buffer - value of R1 on entry
 *                size - value of R2 on entry
 *
 * Output:        expr_is_str - value of R1 on exit (X version only)
 *                int_value_or_str_used - value of R2 on exit
 *
 * Returns:       R1 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x2D.
 */

os_error *xos_evaluate_expression (char const *expr,
      char *buffer,
      int size,
      osbool *expr_is_str,
      int *int_value_or_str_used)
{
  fprintf(stderr, "os_evaluate_expression\n");
  fprintf(stderr, "  In: expr = %x\n", (int) expr);
  fprintf(stderr, "  In: buffer = %x\n", (int) buffer);
  fprintf(stderr, "  In: size = %x\n", (int) size);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: expr_is_str = %x\n", (int) *expr_is_str);
  fprintf(stderr, "  Out: int_value_or_str_used = %x\n", (int) *int_value_or_str_used);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_read_palette()
 *
 * Description:   Reads the palette setting of a colour
 *
 * Input:         entry - value of R0 on entry
 *                colour_type - value of R1 on entry
 *
 * Output:        on - value of R2 on exit
 *                off - value of R3 on exit
 *
 * Other notes:   Emulation of SWI 0x2F.
 */

os_error *xos_read_palette (os_colour_number entry,
      int colour_type,
      os_colour *on,
      os_colour *off)
{
  fprintf(stderr, "os_read_palette\n");
  fprintf(stderr, "  In: entry = %x\n", (int) entry);
  fprintf(stderr, "  In: colour_type = %x\n", (int) colour_type);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: on = %x\n", (int) *on);
  fprintf(stderr, "  Out: off = %x\n", (int) *off);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_read_vdu_variables()
 *
 * Description:   Reads a series of VDU variables
 *
 * Input:         var_list - value of R0 on entry
 *                value_list - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x31.
 */

os_error *xos_read_vdu_variables (os_vdu_var_list const *var_list,
      int *value_list)
{
  fprintf(stderr, "os_read_vdu_variables\n");
  fprintf(stderr, "  In: var_list = %x\n", (int) var_list);
  fprintf(stderr, "  In: value_list = %x\n", (int) value_list);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_read_point()
 *
 * Description:   Reads the colour of a point
 *
 * Input:         x - value of R0 on entry
 *                y - value of R1 on entry
 *
 * Output:        gcol - value of R2 on exit
 *                tint - value of R3 on exit
 *                off_screen - value of R4 on exit (X version only)
 *
 * Returns:       R4 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x32.
 */

os_error *xos_read_point (int x,
      int y,
      os_gcol *gcol,
      os_tint *tint,
      osbool *off_screen)
{
  fprintf(stderr, "os_read_point\n");
  fprintf(stderr, "  In: x = %x\n", (int) x);
  fprintf(stderr, "  In: y = %x\n", (int) y);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: gcol = %x\n", (int) *gcol);
  fprintf(stderr, "  Out: tint = %x\n", (int) *tint);
  fprintf(stderr, "  Out: off_screen = %x\n", (int) *off_screen);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_read_mode_variable()
 *
 * Description:   Reads information about a screen mode
 *
 * Input:         mode - value of R0 on entry
 *                var - value of R1 on entry
 *
 * Output:        var_val - value of R2 on exit
 *                psr - processor status register on exit (X version only)
 *
 * Returns:       psr (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x35.
 */

os_error *xos_read_mode_variable (os_mode mode,
      os_mode_var var,
      int *var_val,
      bits *psr)
{
  fprintf(stderr, "os_read_mode_variable\n");
  fprintf(stderr, "  In: mode = %x\n", (int) mode);
  fprintf(stderr, "  In: var = %x\n", (int) var);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: var_val = %x\n", (int) *var_val);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_remove_cursors()
 *
 * Description:   Removes the cursors from the screen
 *
 * Other notes:   Emulation of SWI 0x36.
 */

os_error *xos_remove_cursors (void)
{
  fprintf(stderr, "os_remove_cursors\n");
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_restore_cursors()
 *
 * Description:   Restores the cursors to the screen
 *
 * Other notes:   Emulation of SWI 0x37.
 */

os_error *xos_restore_cursors (void)
{
  fprintf(stderr, "os_restore_cursors\n");
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_swi_number_to_string()
 *
 * Description:   Converts a SWI number to a string containing its name
 *
 * Input:         swi - value of R0 on entry
 *                buffer - value of R1 on entry
 *                size - value of R2 on entry
 *
 * Output:        used - value of R2 on exit (X version only)
 *
 * Returns:       R2 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x38.
 */

os_error *xos_swi_number_to_string (int swi,
      char *buffer,
      int size,
      int *used)
{
  fprintf(stderr, "os_swi_number_to_string\n");
  fprintf(stderr, "  In: swi = %x\n", (int) swi);
  fprintf(stderr, "  In: buffer = %x\n", (int) buffer);
  fprintf(stderr, "  In: size = %x\n", (int) size);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: used = %x\n", (int) *used);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_swi_number_from_string()
 *
 * Description:   Converts a string to a SWI number if valid
 *
 * Input:         swi_name - value of R1 on entry
 *
 * Output:        swi - value of R0 on exit (X version only)
 *
 * Returns:       R0 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x39.
 */

os_error *xos_swi_number_from_string (char const *swi_name,
      int *swi)
{
  fprintf(stderr, "os_swi_number_from_string\n");
  fprintf(stderr, "  In: swi_name = %x\n", (int) swi_name);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: swi = %x\n", (int) *swi);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_validate_address()
 *
 * Description:   Checks that a range of address are in logical RAM
 *
 * Input:         min - value of R0 on entry
 *                max - value of R1 on entry
 *
 * Output:        psr - processor status register on exit (X version only)
 *
 * Returns:       psr (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x3A.
 */

os_error *xos_validate_address (byte *min,
      byte *max,
      bits *psr)
{
  fprintf(stderr, "os_validate_address\n");
  fprintf(stderr, "  In: min = %x\n", (int) min);
  fprintf(stderr, "  In: max = %x\n", (int) max);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_call_after()
 *
 * Description:   Calls a specified address after a delay
 *
 * Input:         delay - value of R0 on entry
 *                code - value of R1 on entry
 *                handle - value of R2 on entry
 *
 * Other notes:   Emulation of SWI 0x3B.
 */

os_error *xos_call_after (int delay,
      void const *code,
      byte *handle)
{
  fprintf(stderr, "os_call_after\n");
  fprintf(stderr, "  In: delay = %x\n", (int) delay);
  fprintf(stderr, "  In: code = %x\n", (int) code);
  fprintf(stderr, "  In: handle = %x\n", (int) handle);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_call_every()
 *
 * Description:   Calls a specified address every time a delay elapses
 *
 * Input:         delay - value of R0 on entry
 *                code - value of R1 on entry
 *                handle - value of R2 on entry
 *
 * Other notes:   Emulation of SWI 0x3C.
 */

os_error *xos_call_every (int delay,
      void const *code,
      byte *handle)
{
  fprintf(stderr, "os_call_every\n");
  fprintf(stderr, "  In: delay = %x\n", (int) delay);
  fprintf(stderr, "  In: code = %x\n", (int) code);
  fprintf(stderr, "  In: handle = %x\n", (int) handle);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_remove_ticker_event()
 *
 * Description:   Removes a given call address and handle value from the
 *                ticker event list
 *
 * Input:         code - value of R0 on entry
 *                handle - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x3D.
 */

os_error *xos_remove_ticker_event (void const *code,
      byte *handle)
{
  fprintf(stderr, "os_remove_ticker_event\n");
  fprintf(stderr, "  In: code = %x\n", (int) code);
  fprintf(stderr, "  In: handle = %x\n", (int) handle);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_install_key_handler()
 *
 * Description:   Installs a key handler or reads the address of the
 *                current one
 *
 * Input:         key_handler - value of R0 on entry
 *
 * Output:        old_key_handler - value of R0 on exit (X version only)
 *
 * Returns:       R0 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x3E.
 */

os_error *xos_install_key_handler (os_key_handler const *key_handler,
      os_key_handler **old_key_handler)
{
  fprintf(stderr, "os_install_key_handler\n");
  fprintf(stderr, "  In: key_handler = %x\n", (int) key_handler);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: old_key_handler = %x\n", (int) *old_key_handler);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_check_mode_valid()
 *
 * Description:   Checks if it is possible to change into a specified mode
 *
 * Input:         mode - value of R0 on entry
 *
 * Output:        mode_status - value of R0 on exit
 *                alternative_mode - value of R1 on exit
 *                psr - processor status register on exit (X version only)
 *
 * Returns:       psr (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x3F.
 */

os_error *xos_check_mode_valid (os_mode mode,
      int *mode_status,
      os_mode *alternative_mode,
      bits *psr)
{
  fprintf(stderr, "os_check_mode_valid\n");
  fprintf(stderr, "  In: mode = %x\n", (int) mode);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: mode_status = %x\n", (int) *mode_status);
  fprintf(stderr, "  Out: alternative_mode = %x\n", (int) *alternative_mode);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_change_environment()
 *
 * Description:   Installs a handler
 *
 * Input:         handler_type - value of R0 on entry
 *                handler - value of R1 on entry
 *                handle - value of R2 on entry
 *                buffer - value of R3 on entry
 *
 * Output:        old_handler - value of R1 on exit (X version only)
 *                old_handle - value of R2 on exit
 *                old_buffer - value of R3 on exit
 *
 * Returns:       R1 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x40.
 */

os_error *xos_change_environment (os_handler_type handler_type,
      void const *handler,
      byte *handle,
      byte *buffer,
      void **old_handler,
      byte **old_handle,
      byte **old_buffer)
{
  fprintf(stderr, "os_change_environment\n");
  fprintf(stderr, "  In: handler_type = %x\n", (int) handler_type);
  fprintf(stderr, "  In: handler = %x\n", (int) handler);
  fprintf(stderr, "  In: handle = %x\n", (int) handle);
  fprintf(stderr, "  In: buffer = %x\n", (int) buffer);

  fprintf(stderr, "FIXME --- OS_ChangeEnvironment unimplemented\n");
  
  fprintf(stderr, "  Out: old_handler = %x\n", (int) *old_handler);
  fprintf(stderr, "  Out: old_handle = %x\n", (int) *old_handle);
  fprintf(stderr, "  Out: old_buffer = %x\n", (int) *old_buffer);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osclaimscreenmemory_alloc()
 *
 * Description:   Claims spare screen memory - Deprecated under RISC OS
 *                3.7+
 *
 * Input:         size - value of R1 on entry
 *
 * Output:        size_out - value of R1 on exit
 *                blk - value of R2 on exit
 *                psr - processor status register on exit (X version only)
 *
 * Returns:       psr (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x41 with R0 = 0x1.
 */

os_error *xosclaimscreenmemory_alloc (int size,
      int *size_out,
      void **blk,
      bits *psr)
{
  fprintf(stderr, "osclaimscreenmemory_alloc\n");
  fprintf(stderr, "  In: size = %x\n", (int) size);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: size_out = %x\n", (int) *size_out);
  fprintf(stderr, "  Out: blk = %x\n", (int) *blk);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osclaimscreenmemory_free()
 *
 * Description:   Releases spare screen memory - Deprecated under RISC OS
 *                3.7+
 *
 * Other notes:   Emulation of SWI 0x41 with R0 = 0x0.
 */

os_error *xosclaimscreenmemory_free (void)
{
  fprintf(stderr, "osclaimscreenmemory_free\n");
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_read_monotonic_time()
 *
 * Description:   Returns the number of centiseconds since the last hard
 *                reset
 *
 * Output:        t - value of R0 on exit (X version only)
 *
 * Returns:       R0 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x42.
 */

os_error *xos_read_monotonic_time (os_t *t)
{
  fprintf(stderr, "os_read_monotonic_time\n");
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: t = %x\n", (int) *t);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_substitute_args()
 *
 * Description:   Substitutes command line arguments
 *
 * Input:         args - value of R0 on entry
 *                buffer - value of R1 on entry
 *                size - value of R2 on entry
 *                source - value of R3 on entry
 *                source_size - value of R4 on entry
 *
 * Output:        used - value of R2 on exit (X version only)
 *
 * Returns:       R2 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x43.
 */

os_error *xos_substitute_args (char const *args,
      char *buffer,
      int size,
      char const *source,
      int source_size,
      int *used)
{
  fprintf(stderr, "os_substitute_args\n");
  fprintf(stderr, "  In: args = %x\n", (int) args);
  fprintf(stderr, "  In: buffer = %x\n", (int) buffer);
  fprintf(stderr, "  In: size = %x\n", (int) size);
  fprintf(stderr, "  In: source = %x\n", (int) source);
  fprintf(stderr, "  In: source_size = %x\n", (int) source_size);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: used = %x\n", (int) *used);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_pretty_print()
 *
 * Description:   Writes a string with some formatting to all of the active
 *                output streams
 *
 * Input:         string - value of R0 on entry
 *                dictionary - value of R1 on entry
 *                special - value of R2 on entry
 *
 * Other notes:   Emulation of SWI 0x44.
 */

os_error *xos_pretty_print (char const *string,
      byte const *dictionary,
      char const *special)
{
  fprintf(stderr, "os_pretty_print\n");
  fprintf(stderr, "  In: string = %x\n", (int) string);
  fprintf(stderr, "  In: dictionary = %x\n", (int) dictionary);
  fprintf(stderr, "  In: special = %x\n", (int) special);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_plot()
 *
 * Description:   Direct VDU call to general plot command
 *
 * Input:         plot_code - value of R0 on entry
 *                x - value of R1 on entry
 *                y - value of R2 on entry
 *
 * Other notes:   Emulation of SWI 0x45.
 */

os_error *xos_plot (os_plot_code plot_code,
      int x,
      int y)
{
  fprintf(stderr, "os_plot\n");
  fprintf(stderr, "  In: plot_code = %x\n", (int) plot_code);
  fprintf(stderr, "  In: x = %x\n", (int) x);
  fprintf(stderr, "  In: y = %x\n", (int) y);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_writen()
 *
 * Description:   Writes a counted string to all of the active output
 *                streams
 *
 * Input:         s - value of R0 on entry
 *                size - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x46.
 */

os_error *xos_writen (char const *s,
      int size)
{
  int c;

  fprintf(stderr, "os_writen\n");
  fprintf(stderr, "  In: s = %x\n", (int) s);
  fprintf(stderr, "  In: size = %x\n", (int) size);

  for (c = 0; c < size; c++)
    vdu(s[c]);

  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_add_to_vector()
 *
 * Description:   Adds a routine to the list of those that claim a vector
 *
 * Input:         vector - value of R0 on entry
 *                routine - value of R1 on entry
 *                handle - value of R2 on entry
 *
 * Other notes:   Emulation of SWI 0x47.
 */

os_error *xos_add_to_vector (int vector,
      void const *routine,
      byte *handle)
{
  fprintf(stderr, "os_add_to_vector\n");
  fprintf(stderr, "  In: vector = %x\n", (int) vector);
  fprintf(stderr, "  In: routine = %x\n", (int) routine);
  fprintf(stderr, "  In: handle = %x\n", (int) handle);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_write_env()
 *
 * Description:   Sets the program environment command string and start
 *                time
 *
 * Input:         command - value of R0 on entry
 *                start - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x48.
 */

os_error *xos_write_env (char const *command,
      os_date_and_time const *start)
{
  fprintf(stderr, "os_write_env\n");
  fprintf(stderr, "  In: command = %x\n", (int) command);
  fprintf(stderr, "  In: start = %x\n", (int) start);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_read_args()
 *
 * Description:   Given a keyword definition, scans a command string
 *
 * Input:         keywords - value of R0 on entry
 *                input - value of R1 on entry
 *                buffer - value of R2 on entry
 *                size - value of R3 on entry
 *
 * Output:        spare - value of R3 on exit (X version only)
 *
 * Returns:       R3 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x49.
 */

os_error *xos_read_args (char const *keywords,
      char const *input,
      char *buffer,
      int size,
      int *spare)
{
  fprintf(stderr, "os_read_args\n");
  fprintf(stderr, "  In: keywords = %x\n", (int) keywords);
  fprintf(stderr, "  In: input = %x\n", (int) input);
  fprintf(stderr, "  In: buffer = %x\n", (int) buffer);
  fprintf(stderr, "  In: size = %x\n", (int) size);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: spare = %x\n", (int) *spare);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_read_ram_fs_limits()
 *
 * Description:   Gets the current limits of the RAM filing system
 *
 * Output:        start - value of R0 on exit
 *                end - value of R1 on exit
 *
 * Other notes:   Emulation of SWI 0x4A.
 */

os_error *xos_read_ram_fs_limits (byte **start,
      char **end)
{
  fprintf(stderr, "os_read_ram_fs_limits\n");
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: start = %x\n", (int) *start);
  fprintf(stderr, "  Out: end = %x\n", (int) *end);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_claim_device_vector()
 *
 * Description:   Claims a device vector
 *
 * Input:         device - value of R0 on entry
 *                driver - value of R1 on entry
 *                handle - value of R2 on entry
 *                status - value of R3 on entry
 *                mask - value of R4 on entry
 *
 * Other notes:   Emulation of SWI 0x4B.
 */

os_error *xos_claim_device_vector (os_device_type device,
      void const *driver,
      byte *handle,
      int *status,
      bits mask)
{
  fprintf(stderr, "os_claim_device_vector\n");
  fprintf(stderr, "  In: device = %x\n", (int) device);
  fprintf(stderr, "  In: driver = %x\n", (int) driver);
  fprintf(stderr, "  In: handle = %x\n", (int) handle);
  fprintf(stderr, "  In: status = %x\n", (int) status);
  fprintf(stderr, "  In: mask = %x\n", (int) mask);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_release_device_vector()
 *
 * Description:   Releases a device vector
 *
 * Input:         device - value of R0 on entry
 *                driver - value of R1 on entry
 *                handle - value of R2 on entry
 *                status - value of R3 on entry
 *                mask - value of R4 on entry
 *
 * Other notes:   Emulation of SWI 0x4C.
 */

os_error *xos_release_device_vector (os_device_type device,
      void const *driver,
      byte *handle,
      int *status,
      bits mask)
{
  fprintf(stderr, "os_release_device_vector\n");
  fprintf(stderr, "  In: device = %x\n", (int) device);
  fprintf(stderr, "  In: driver = %x\n", (int) driver);
  fprintf(stderr, "  In: handle = %x\n", (int) handle);
  fprintf(stderr, "  In: status = %x\n", (int) status);
  fprintf(stderr, "  In: mask = %x\n", (int) mask);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_delink_application()
 *
 * Description:   Removes any vectors that an application is using
 *
 * Input:         vector_details - value of R0 on entry
 *                size - value of R1 on entry
 *
 * Output:        used - value of R1 on exit (X version only)
 *
 * Returns:       R1 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x4D.
 */

os_error *xos_delink_application (byte *vector_details,
      int size,
      int *used)
{
  fprintf(stderr, "os_delink_application\n");
  fprintf(stderr, "  In: vector_details = %x\n", (int) vector_details);
  fprintf(stderr, "  In: size = %x\n", (int) size);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: used = %x\n", (int) *used);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_relink_application()
 *
 * Description:   Restores from a buffer any vectors that an application is
 *                using
 *
 * Input:         vector_details - value of R0 on entry
 *
 * Other notes:   Emulation of SWI 0x4E.
 */

os_error *xos_relink_application (byte const *vector_details)
{
  fprintf(stderr, "os_relink_application\n");
  fprintf(stderr, "  In: vector_details = %x\n", (int) vector_details);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_heap_sort()
 *
 * Description:   Heap-sorts a list of objects
 *
 * Input:         count - value of R0 on entry
 *                array - value of R1 on entry
 *                compare_fn - value of R2 on entry
 *                handle - value of R3 on entry
 *                objects - value of R4 on entry
 *                object_size - value of R5 on entry
 *                workspace - value of R6 on entry
 *
 * Other notes:   Emulation of SWI 0x4F.
 */

os_error *xos_heap_sort (int count,
      int *array,
      os_sort_type compare_fn,
      byte *handle,
      byte *objects,
      int object_size,
      void *workspace)
{
  fprintf(stderr, "os_heap_sort\n");
  fprintf(stderr, "  In: count = %x\n", (int) count);
  fprintf(stderr, "  In: array = %x\n", (int) array);
  fprintf(stderr, "  In: compare_fn = %x\n", (int) compare_fn);
  fprintf(stderr, "  In: handle = %x\n", (int) handle);
  fprintf(stderr, "  In: objects = %x\n", (int) objects);
  fprintf(stderr, "  In: object_size = %x\n", (int) object_size);
  fprintf(stderr, "  In: workspace = %x\n", (int) workspace);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_exit_and_die()
 *
 * Description:   Kills a module and passes control to the most recent exit
 *                handler
 *
 * Input:         e - value of R0 on entry
 *                rc - value of R2 on entry
 *                module_name - value of R3 on entry
 *
 * Other notes:   Emulation of SWI 0x50 with R1 = 0x58454241.
 */

os_error *xos_exit_and_die (os_error const *e,
      int rc,
      char const *module_name)
{
  fprintf(stderr, "os_exit_and_die\n");
  fprintf(stderr, "  In: e = %x\n", (int) e);
  fprintf(stderr, "  In: rc = %x\n", (int) rc);
  fprintf(stderr, "  In: module_name = %x\n", (int) module_name);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_read_mem_map_info()
 *
 * Description:   Reads the page size and count
 *
 * Output:        page_size - value of R0 on exit
 *                page_count - value of R1 on exit
 *
 * Other notes:   Emulation of SWI 0x51.
 */

os_error *xos_read_mem_map_info (int *page_size,
      int *page_count)
{
  fprintf(stderr, "os_read_mem_map_info\n");
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: page_size = %x\n", (int) *page_size);
  fprintf(stderr, "  Out: page_count = %x\n", (int) *page_count);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_read_mem_map_entries()
 *
 * Description:   Reads by page number the logical to physical memory
 *                mapping used by MEMC
 *
 * Input:         request_list - value of R0 on entry
 *
 * Other notes:   Emulation of SWI 0x52.
 */

os_error *xos_read_mem_map_entries (os_mem_map_request_list *request_list)
{
  fprintf(stderr, "os_read_mem_map_entries\n");
  fprintf(stderr, "  In: request_list = %x\n", (int) request_list);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_set_mem_map_entries()
 *
 * Description:   Writes the logical to physical memory mapping used by
 *                MEMC
 *
 * Input:         request_list - value of R0 on entry
 *
 * Other notes:   Emulation of SWI 0x53.
 */

os_error *xos_set_mem_map_entries (os_mem_map_request_list const *request_list)
{
  fprintf(stderr, "os_set_mem_map_entries\n");
  fprintf(stderr, "  In: request_list = %x\n", (int) request_list);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_add_call_back()
 *
 * Description:   Adds a transient callback to the list
 *
 * Input:         call_back - value of R0 on entry
 *                handle - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x54.
 */

os_error *xos_add_call_back (void const *call_back,
      byte *handle)
{
  fprintf(stderr, "os_add_call_back\n");
  fprintf(stderr, "  In: call_back = %x\n", (int) call_back);
  fprintf(stderr, "  In: handle = %x\n", (int) handle);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_read_default_handler()
 *
 * Description:   Gets the address of the default handler
 *
 * Input:         handler_type - value of R0 on entry
 *
 * Output:        handler - value of R1 on exit (X version only)
 *                handle - value of R2 on exit
 *                buffer - value of R3 on exit
 *
 * Returns:       R1 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x55.
 */

os_error *xos_read_default_handler (int handler_type,
      void **handler,
      byte **handle,
      byte **buffer)
{
  fprintf(stderr, "os_read_default_handler\n");
  fprintf(stderr, "  In: handler_type = %x\n", (int) handler_type);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: handler = %x\n", (int) *handler);
  fprintf(stderr, "  Out: handle = %x\n", (int) *handle);
  fprintf(stderr, "  Out: buffer = %x\n", (int) *buffer);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_set_ecf_origin()
 *
 * Description:   Sets the origin of the ECF patterns
 *
 * Input:         x - value of R0 on entry
 *                y - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x56.
 */

os_error *xos_set_ecf_origin (int x,
      int y)
{
  fprintf(stderr, "os_set_ecf_origin\n");
  fprintf(stderr, "  In: x = %x\n", (int) x);
  fprintf(stderr, "  In: y = %x\n", (int) y);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_confirm()
 *
 * Description:   Gets a yes or no answer
 *
 * Output:        key - value of R0 on exit
 *                psr - processor status register on exit (X version only)
 *
 * Returns:       psr (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x59.
 */

os_error *xos_confirm (char *key,
      bits *psr)
{
  fprintf(stderr, "os_confirm\n");
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: key = %x\n", (int) *key);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_changed_box()
 *
 * Description:   Determines which area of the screen has changed
 *
 * Input:         state - value of R0 on entry
 *
 * Output:        enabled - value of R0 on exit (X version only)
 *                change_box - value of R1 on exit
 *
 * Returns:       R0 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x5A.
 */

os_error *xos_changed_box (os_changed_box_state state,
      osbool *enabled,
      os_change_box **change_box)
{
  fprintf(stderr, "os_changed_box\n");
  fprintf(stderr, "  In: state = %x\n", (int) state);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: enabled = %x\n", (int) *enabled);
  fprintf(stderr, "  Out: change_box = %x\n", (int) *change_box);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_crc()
 *
 * Description:   Calculates the cyclic redundancy check for a block of
 *                data
 *
 * Input:         crc_in - value of R0 on entry
 *                block - value of R1 on entry
 *                end - value of R2 on entry
 *                stride - value of R3 on entry
 *
 * Output:        crc - value of R0 on exit (X version only)
 *
 * Returns:       R0 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x5B.
 */

os_error *xos_crc (int crc_in,
      byte const *block,
      char const *end,
      int stride,
      int *crc)
{
  fprintf(stderr, "os_crc\n");
  fprintf(stderr, "  In: crc_in = %x\n", (int) crc_in);
  fprintf(stderr, "  In: block = %x\n", (int) block);
  fprintf(stderr, "  In: end = %x\n", (int) end);
  fprintf(stderr, "  In: stride = %x\n", (int) stride);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: crc = %x\n", (int) *crc);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_read_dynamic_area()
 *
 * Description:   Reads the space allocation of a dynamic area
 *
 * Input:         area - value of R0 on entry
 *
 * Output:        area_start - value of R0 on exit (X version only)
 *                size - value of R1 on exit
 *                size_limit - value of R2 on exit
 *
 * Returns:       R0 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x5C.
 */

os_error *xos_read_dynamic_area (os_dynamic_area_no area,
      byte **area_start,
      int *size,
      int *size_limit)
{
  fprintf(stderr, "os_read_dynamic_area\n");
  fprintf(stderr, "  In: area = %x\n", (int) area);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: area_start = %x\n", (int) *area_start);
  fprintf(stderr, "  Out: size = %x\n", (int) *size);
  fprintf(stderr, "  Out: size_limit = %x\n", (int) *size_limit);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_print_char()
 *
 * Description:   Sends a character to the printer stream
 *
 * Input:         c - value of R0 on entry
 *
 * Other notes:   Emulation of SWI 0x5D.
 */

os_error *xos_print_char (char c)
{
  fprintf(stderr, "os_print_char\n");
  fprintf(stderr, "  In: c = %x\n", (int) c);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_change_redirectionw()
 *
 * Description:   Reads or writes OS_CLI input/output redirection. Uses
 *                32-bit file handles
 *
 * Input:         input - value of R0 on entry
 *                output - value of R1 on entry
 *
 * Output:        old_input - value of R0 on exit
 *                old_output - value of R1 on exit
 *
 * Other notes:   Emulation of SWI 0x5E.
 */

os_error *xos_change_redirectionw (os_fw input,
      os_fw output,
      os_fw *old_input,
      os_fw *old_output)
{
  fprintf(stderr, "os_change_redirectionw\n");
  fprintf(stderr, "  In: input = %x\n", (int) input);
  fprintf(stderr, "  In: output = %x\n", (int) output);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: old_input = %x\n", (int) *old_input);
  fprintf(stderr, "  Out: old_output = %x\n", (int) *old_output);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_remove_call_back()
 *
 * Description:   Removes a transient callback from the list
 *
 * Input:         call_back - value of R0 on entry
 *                handle - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x5F.
 */

os_error *xos_remove_call_back (void const *call_back,
      byte *handle)
{
  fprintf(stderr, "os_remove_call_back\n");
  fprintf(stderr, "  In: call_back = %x\n", (int) call_back);
  fprintf(stderr, "  In: handle = %x\n", (int) handle);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_find_mem_map_entries()
 *
 * Description:   Reads by logical address the logical to physical memory
 *                mapping used by MEMC
 *
 * Input:         request_list - value of R0 on entry
 *
 * Other notes:   Emulation of SWI 0x60.
 */

os_error *xos_find_mem_map_entries (os_mem_map_request_list *request_list)
{
  fprintf(stderr, "os_find_mem_map_entries\n");
  fprintf(stderr, "  In: request_list = %x\n", (int) request_list);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_set_colour()
 *
 * Description:   Sets the current colour or colour pattern
 *
 * Input:         flags - value of R0 on entry
 *                colour_number - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x61.
 */

os_error *xos_set_colour (os_colour_flags flags,
      os_colour_number colour_number)
{
  fprintf(stderr, "os_set_colour\n");
  fprintf(stderr, "  In: flags = %x\n", (int) flags);
  fprintf(stderr, "  In: colour_number = %x\n", (int) colour_number);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_read_colour()
 *
 * Description:   Reads the current colour or colour pattern - RISC O S
 *                3.5+
 *
 * Input:         flags - value of R0 on entry
 *                pattern - value of R1 on entry
 *
 * Output:        flags_out - value of R0 on exit
 *                colour_number - value of R1 on exit (X version only)
 *
 * Returns:       R1 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x61 with R0 |= 0x80.
 */

os_error *xos_read_colour (os_colour_flags flags,
      os_ecf *pattern,
      os_colour_flags *flags_out,
      os_colour_number *colour_number)
{
  fprintf(stderr, "os_read_colour\n");
  fprintf(stderr, "  In: flags = %x\n", (int) flags);
  fprintf(stderr, "  In: pattern = %x\n", (int) pattern);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: flags_out = %x\n", (int) *flags_out);
  fprintf(stderr, "  Out: colour_number = %x\n", (int) *colour_number);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      ospointer_get()
 *
 * Description:   Gets the currently selected pointer device type - RISC O
 *                S 3.5+
 *
 * Output:        pointer_type - value of R0 on exit (X version only)
 *
 * Returns:       R0 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x64 with R0 = 0x0.
 */

os_error *xospointer_get (os_pointer_type *pointer_type)
{
  fprintf(stderr, "ospointer_get\n");
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: pointer_type = %x\n", (int) *pointer_type);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      ospointer_set()
 *
 * Description:   Sets the currently selected pointer device type - RISC O
 *                S 3.5+
 *
 * Input:         pointer_type - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x64 with R0 = 0x1.
 */

os_error *xospointer_set (os_pointer_type pointer_type)
{
  fprintf(stderr, "ospointer_set\n");
  fprintf(stderr, "  In: pointer_type = %x\n", (int) pointer_type);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osscreenmode_select()
 *
 * Description:   Selects a screen mode - RISC O S 3.5+
 *
 * Input:         mode - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x65 with R0 = 0x0.
 */

os_error *xosscreenmode_select (os_mode mode)
{
  fprintf(stderr, "osscreenmode_select\n");
  fprintf(stderr, "  In: mode = %x\n", (int) mode);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osscreenmode_current()
 *
 * Description:   Returns the mode specifier for the current mode - RISC O
 *                S 3.5+
 *
 * Output:        mode - value of R1 on exit (X version only)
 *
 * Returns:       R1 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x65 with R0 = 0x1.
 */

os_error *xosscreenmode_current (os_mode *mode)
{
  fprintf(stderr, "osscreenmode_current\n");
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: mode = %x\n", (int) *mode);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osscreenmode_enumerate()
 *
 * Description:   Enumerates the available screen modes - RISC O S 3.5+
 *
 * Input:         context - value of R2 on entry
 *                block - value of R6 on entry
 *                block_size - value of R7 on entry
 *
 * Output:        complete - value of R1 on exit (X version only)
 *                context_out - value of R2 on exit
 *                end - value of R6 on exit
 *                free - value of R7 on exit
 *
 * Returns:       R1 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x65 with R0 = 0x2.
 */

os_error *xosscreenmode_enumerate (int context,
      os_mode_block *block,
      int block_size,
      osbool *complete,
      int *context_out,
      byte **end,
      int *free)
{
  fprintf(stderr, "osscreenmode_enumerate\n");
  fprintf(stderr, "  In: context = %x\n", (int) context);
  fprintf(stderr, "  In: block = %x\n", (int) block);
  fprintf(stderr, "  In: block_size = %x\n", (int) block_size);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: complete = %x\n", (int) *complete);
  fprintf(stderr, "  Out: context_out = %x\n", (int) *context_out);
  fprintf(stderr, "  Out: end = %x\n", (int) *end);
  fprintf(stderr, "  Out: free = %x\n", (int) *free);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osscreenmode_cache_control()
 *
 * Description:   Controls cacheing of screen memory and screen cleaning -
 *                RISC O S 4+
 *
 * Input:         flags - value of R1 on entry
 *                laziness - value of R2 on entry
 *
 * Output:        current_flags - value of R1 on exit (X version only)
 *                current_laziness - value of R2 on exit
 *
 * Returns:       R1 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x65 with R0 = 0x4.
 */

os_error *xosscreenmode_cache_control (osscreenmode_cache_flags flags,
      osscreenmode_cleaner_laziness laziness,
      osscreenmode_cache_flags *current_flags,
      osscreenmode_cleaner_laziness *current_laziness)
{
  fprintf(stderr, "osscreenmode_cache_control\n");
  fprintf(stderr, "  In: flags = %x\n", (int) flags);
  fprintf(stderr, "  In: laziness = %x\n", (int) laziness);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: current_flags = %x\n", (int) *current_flags);
  fprintf(stderr, "  Out: current_laziness = %x\n", (int) *current_laziness);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osscreenmode_request_clean()
 *
 * Description:   Request a foreground screen clean - RISC O S 4+
 *
 * Other notes:   Emulation of SWI 0x65 with R0 = 0x5.
 */

os_error *xosscreenmode_request_clean (void)
{
  fprintf(stderr, "osscreenmode_request_clean\n");
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osscreenmode_force_clean()
 *
 * Description:   Force a screen clean if needed - RISC O S 4+
 *
 * Other notes:   Emulation of SWI 0x65 with R0 = 0x6.
 */

os_error *xosscreenmode_force_clean (void)
{
  fprintf(stderr, "osscreenmode_force_clean\n");
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osdynamicarea_create()
 *
 * Description:   Creates a new dynamic area - RISC O S 3.5+
 *
 * Input:         area - value of R1 on entry
 *                size - value of R2 on entry
 *                base_address - value of R3 on entry
 *                flags - value of R4 on entry
 *                size_limit - value of R5 on entry
 *                handler - value of R6 on entry
 *                workspace - value of R7 on entry
 *                description - value of R8 on entry
 *
 * Output:        area_out - value of R1 on exit (X version only)
 *                base_address_out - value of R3 on exit
 *                size_limit_out - value of R5 on exit
 *
 * Returns:       R1 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x66 with R0 = 0x0.
 */

os_error *xosdynamicarea_create (os_dynamic_area_no area,
      int size,
      byte const *base_address,
      bits flags,
      int size_limit,
      void const *handler,
      void *workspace,
      char const *description,
      os_dynamic_area_no *area_out,
      byte **base_address_out,
      int *size_limit_out)
{
  fprintf(stderr, "osdynamicarea_create\n");
  fprintf(stderr, "  In: area = %x\n", (int) area);
  fprintf(stderr, "  In: size = %x\n", (int) size);
  fprintf(stderr, "  In: base_address = %x\n", (int) base_address);
  fprintf(stderr, "  In: flags = %x\n", (int) flags);
  fprintf(stderr, "  In: size_limit = %x\n", (int) size_limit);
  fprintf(stderr, "  In: handler = %x\n", (int) handler);
  fprintf(stderr, "  In: workspace = %x\n", (int) workspace);
  fprintf(stderr, "  In: description = %x\n", (int) description);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: area_out = %x\n", (int) *area_out);
  fprintf(stderr, "  Out: base_address_out = %x\n", (int) *base_address_out);
  fprintf(stderr, "  Out: size_limit_out = %x\n", (int) *size_limit_out);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osdynamicarea_delete()
 *
 * Description:   Removes a previously created dynamic area - RISC O S 3.5+
 *
 * Input:         area - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x66 with R0 = 0x1.
 */

os_error *xosdynamicarea_delete (os_dynamic_area_no area)
{
  fprintf(stderr, "osdynamicarea_delete\n");
  fprintf(stderr, "  In: area = %x\n", (int) area);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osdynamicarea_read()
 *
 * Description:   Returns information on a dynamic area - RISC O S 3.5+
 *
 * Input:         area - value of R1 on entry
 *
 * Output:        size - value of R2 on exit
 *                base_address - value of R3 on exit
 *                flags - value of R4 on exit
 *                size_limit - value of R5 on exit
 *                handler - value of R6 on exit
 *                workspace - value of R7 on exit
 *                description - value of R8 on exit
 *
 * Other notes:   Emulation of SWI 0x66 with R0 = 0x2.
 */

os_error *xosdynamicarea_read (os_dynamic_area_no area,
      int *size,
      byte **base_address,
      bits *flags,
      int *size_limit,
      void **handler,
      void **workspace,
      char **description)
{
  fprintf(stderr, "osdynamicarea_read\n");
  fprintf(stderr, "  In: area = %x\n", (int) area);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: size = %x\n", (int) *size);
  fprintf(stderr, "  Out: base_address = %x\n", (int) *base_address);
  fprintf(stderr, "  Out: flags = %x\n", (int) *flags);
  fprintf(stderr, "  Out: size_limit = %x\n", (int) *size_limit);
  fprintf(stderr, "  Out: handler = %x\n", (int) *handler);
  fprintf(stderr, "  Out: workspace = %x\n", (int) *workspace);
  fprintf(stderr, "  Out: description = %x\n", (int) *description);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osdynamicarea_enumerate()
 *
 * Description:   Enumerates dynamic areas - RISC O S 3.5+
 *
 * Input:         context - value of R1 on entry
 *
 * Output:        context_out - value of R1 on exit (X version only)
 *
 * Returns:       R1 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x66 with R0 = 0x3.
 */

os_error *xosdynamicarea_enumerate (int context,
      int *context_out)
{
  fprintf(stderr, "osdynamicarea_enumerate\n");
  fprintf(stderr, "  In: context = %x\n", (int) context);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: context_out = %x\n", (int) *context_out);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osdynamicarea_renumber()
 *
 * Description:   Renumbers dynamic areas - RISC O S 3.5+
 *
 * Input:         old_area - value of R1 on entry
 *                new_area - value of R2 on entry
 *
 * Other notes:   Emulation of SWI 0x66 with R0 = 0x4.
 */

os_error *xosdynamicarea_renumber (os_dynamic_area_no old_area,
      os_dynamic_area_no new_area)
{
  fprintf(stderr, "osdynamicarea_renumber\n");
  fprintf(stderr, "  In: old_area = %x\n", (int) old_area);
  fprintf(stderr, "  In: new_area = %x\n", (int) new_area);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osdynamicarea_free_space()
 *
 * Description:   Return total free space, allowing for shrinkable areas -
 *                RISC O S 4+
 *
 * Input:         exclude_area - value of R1 on entry
 *
 * Output:        free_space - value of R1 on exit (X version only)
 *
 * Returns:       R1 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x66 with R0 = 0x5.
 */

os_error *xosdynamicarea_free_space (os_dynamic_area_no exclude_area,
      int *free_space)
{
  fprintf(stderr, "osdynamicarea_free_space\n");
  fprintf(stderr, "  In: exclude_area = %x\n", (int) exclude_area);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: free_space = %x\n", (int) *free_space);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osdynamicarea_set_clamps()
 *
 * Description:   Set clamps on maximum sizes for subsequently created
 *                dynamic areas - RISC O S 4+
 *
 * Input:         unlimited_area_clamp - value of R1 on entry
 *                limited_area_clamp - value of R2 on entry
 *                sparse_area_clamp - value of R3 on entry
 *
 * Output:        old_unlimited_area_clamp - value of R1 on exit
 *                old_limited_area_clamp - value of R2 on exit
 *                old_sparse_area_clamp - value of R3 on exit
 *
 * Other notes:   Emulation of SWI 0x66 with R0 = 0x8.
 */

os_error *xosdynamicarea_set_clamps (int unlimited_area_clamp,
      int limited_area_clamp,
      int sparse_area_clamp,
      int *old_unlimited_area_clamp,
      int *old_limited_area_clamp,
      int *old_sparse_area_clamp)
{
  fprintf(stderr, "osdynamicarea_set_clamps\n");
  fprintf(stderr, "  In: unlimited_area_clamp = %x\n", (int) unlimited_area_clamp);
  fprintf(stderr, "  In: limited_area_clamp = %x\n", (int) limited_area_clamp);
  fprintf(stderr, "  In: sparse_area_clamp = %x\n", (int) sparse_area_clamp);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: old_unlimited_area_clamp = %x\n", (int) *old_unlimited_area_clamp);
  fprintf(stderr, "  Out: old_limited_area_clamp = %x\n", (int) *old_limited_area_clamp);
  fprintf(stderr, "  Out: old_sparse_area_clamp = %x\n", (int) *old_sparse_area_clamp);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osdynamicarea_ensure_region()
 *
 * Description:   Ensures that a region of a sparse area is mapped to valid
 *                memory - RISC O S 4+
 *
 * Input:         area - value of R1 on entry
 *                base_address - value of R2 on entry
 *                size - value of R3 on entry
 *
 * Other notes:   Emulation of SWI 0x66 with R0 = 0x9.
 */

os_error *xosdynamicarea_ensure_region (os_dynamic_area_no area,
      byte const *base_address,
      int size)
{
  fprintf(stderr, "osdynamicarea_ensure_region\n");
  fprintf(stderr, "  In: area = %x\n", (int) area);
  fprintf(stderr, "  In: base_address = %x\n", (int) base_address);
  fprintf(stderr, "  In: size = %x\n", (int) size);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osdynamicarea_release_region()
 *
 * Description:   Allows a region of a sparse area to be released as free
 *                memory - RISC O S 4+
 *
 * Input:         area - value of R1 on entry
 *                base_address - value of R2 on entry
 *                size - value of R3 on entry
 *
 * Other notes:   Emulation of SWI 0x66 with R0 = 0xA.
 */

os_error *xosdynamicarea_release_region (os_dynamic_area_no area,
      byte const *base_address,
      int size)
{
  fprintf(stderr, "osdynamicarea_release_region\n");
  fprintf(stderr, "  In: area = %x\n", (int) area);
  fprintf(stderr, "  In: base_address = %x\n", (int) base_address);
  fprintf(stderr, "  In: size = %x\n", (int) size);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osmemory_page_op()
 *
 * Description:   General page block operations - RISC O S 3.5+
 *
 * Input:         flags - value of R0 on entry
 *                page_block - value of R1 on entry
 *                page_count - value of R2 on entry
 *
 * Other notes:   Emulation of SWI 0x68 with R0 |= 0x0.
 */

os_error *xosmemory_page_op (osmemory_flags flags,
      os_page_block const *page_block,
      int page_count)
{
  fprintf(stderr, "osmemory_page_op\n");
  fprintf(stderr, "  In: flags = %x\n", (int) flags);
  fprintf(stderr, "  In: page_block = %x\n", (int) page_block);
  fprintf(stderr, "  In: page_count = %x\n", (int) page_count);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osmemory_read_arrangement_table_size()
 *
 * Description:   Reads the size of the physical memory arrangement table -
 *                RISC O S 3.5+
 *
 * Output:        table_size - value of R1 on exit (X version only)
 *                page_size - value of R2 on exit
 *
 * Returns:       R1 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x68 with R0 = 0x6.
 */

os_error *xosmemory_read_arrangement_table_size (int *table_size,
      int *page_size)
{
  fprintf(stderr, "osmemory_read_arrangement_table_size\n");
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: table_size = %x\n", (int) *table_size);
  fprintf(stderr, "  Out: page_size = %x\n", (int) *page_size);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osmemory_read_arrangement_table()
 *
 * Description:   Read the physical memory arrangement table - RISC O S
 *                3.5+
 *
 * Input:         arrangement_table - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x68 with R0 = 0x7.
 */

os_error *xosmemory_read_arrangement_table (byte *arrangement_table)
{
  fprintf(stderr, "osmemory_read_arrangement_table\n");
  fprintf(stderr, "  In: arrangement_table = %x\n", (int) arrangement_table);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osmemory_read_size()
 *
 * Description:   Reads the amount of a specified type of memory available
 *                on the computer - RISC O S 3.5+
 *
 * Input:         type - value of R0 on entry
 *
 * Output:        page_count - value of R1 on exit
 *                page_size - value of R2 on exit
 *
 * Other notes:   Emulation of SWI 0x68 with R0 |= 0x8.
 */

os_error *xosmemory_read_size (osmemory_type_flags type,
      int *page_count,
      int *page_size)
{
  fprintf(stderr, "osmemory_read_size\n");
  fprintf(stderr, "  In: type = %x\n", (int) type);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: page_count = %x\n", (int) *page_count);
  fprintf(stderr, "  Out: page_size = %x\n", (int) *page_size);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osmemory_read_controller()
 *
 * Description:   Reads controller presence and base address - RISC O S
 *                3.5+
 *
 * Input:         controller - value of R1 on entry
 *
 * Output:        controller_addr - value of R1 on exit
 *
 * Other notes:   Emulation of SWI 0x68 with R0 = 0x9.
 */

os_error *xosmemory_read_controller (osmemory_controller_flags controller,
      void **controller_addr)
{
  fprintf(stderr, "osmemory_read_controller\n");
  fprintf(stderr, "  In: controller = %x\n", (int) controller);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: controller_addr = %x\n", (int) *controller_addr);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osmemory_find_contiguous()
 *
 * Description:   Recommends a base page for a currently available (not
 *                locked down) region of physically contiguous RAM - RISC O
 *                S 4+
 *
 * Input:         size - value of R1 on entry
 *
 * Output:        page_number - value of R3 on exit (X version only)
 *
 * Returns:       R3 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x68 with R0 = 0xC.
 */

os_error *xosmemory_find_contiguous (int size,
      int *page_number)
{
  fprintf(stderr, "osmemory_find_contiguous\n");
  fprintf(stderr, "  In: size = %x\n", (int) size);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: page_number = %x\n", (int) *page_number);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osclaimprocessorvector_alloc()
 *
 * Description:   Claims a processor vector - RISC O S 3.5+
 *
 * Input:         vector - value of R0 on entry
 *                routine - value of R1 on entry
 *
 * Output:        old_routine - value of R1 on exit (X version only)
 *
 * Returns:       R1 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x69 with R0 |= 0x100.
 */

os_error *xosclaimprocessorvector_alloc (bits vector,
      void const *routine,
      void **old_routine)
{
  fprintf(stderr, "osclaimprocessorvector_alloc\n");
  fprintf(stderr, "  In: vector = %x\n", (int) vector);
  fprintf(stderr, "  In: routine = %x\n", (int) routine);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: old_routine = %x\n", (int) *old_routine);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osclaimprocessorvector_free()
 *
 * Description:   Releases a processor vector - RISC O S 3.5+
 *
 * Input:         vector - value of R0 on entry
 *                old_routine - value of R2 on entry
 *
 * Other notes:   Emulation of SWI 0x69 with R0 |= 0x0.
 */

os_error *xosclaimprocessorvector_free (bits vector,
      void const *old_routine)
{
  fprintf(stderr, "osclaimprocessorvector_free\n");
  fprintf(stderr, "  In: vector = %x\n", (int) vector);
  fprintf(stderr, "  In: old_routine = %x\n", (int) old_routine);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_reset()
 *
 * Description:   Performs a hard reset - RISC O S 3.5+
 *
 * Other notes:   Emulation of SWI 0x6A.
 */

os_error *xos_reset (void)
{
  fprintf(stderr, "os_reset\n");
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_mmu_control()
 *
 * Description:   Modifies the ARM MMU - prefer Cache_Control; RISC O S
 *                3.5+
 *
 * Input:         eor_mask - value of R1 on entry
 *                and_mask - value of R2 on entry
 *
 * Output:        old_value - value of R1 on exit (X version only)
 *
 * Returns:       R1 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x6B with R0 = 0x0.
 */

os_error *xos_mmu_control (bits eor_mask,
      bits and_mask,
      bits *old_value)
{
  fprintf(stderr, "os_mmu_control\n");
  fprintf(stderr, "  In: eor_mask = %x\n", (int) eor_mask);
  fprintf(stderr, "  In: and_mask = %x\n", (int) and_mask);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: old_value = %x\n", (int) *old_value);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osplatformfeatures_get_features()
 *
 * Description:   Reports platform features - RISC O S 3.7+
 *
 * Output:        flags - value of R0 on exit (X version only)
 *                predisable_fn - value of R1 on exit
 *
 * Returns:       R0 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x6D with R0 = 0x0.
 */

os_error *xosplatformfeatures_get_features (os_platform_feature_flags *flags,
      void **predisable_fn)
{
  fprintf(stderr, "osplatformfeatures_get_features\n");
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: flags = %x\n", (int) *flags);
  fprintf(stderr, "  Out: predisable_fn = %x\n", (int) *predisable_fn);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_synchronise_code_areas()
 *
 * Description:   Informs the O S that code has been newly generated or
 *                modified in memory - RISC O S 3.7+
 *
 * Input:         flags - value of R0 on entry
 *                start - value of R1 on entry
 *                end - value of R2 on entry
 *
 * Other notes:   Emulation of SWI 0x6E.
 */

os_error *xos_synchronise_code_areas (bits flags,
      void const *start,
      void const *end)
{
  fprintf(stderr, "os_synchronise_code_areas\n");
  fprintf(stderr, "  In: flags = %x\n", (int) flags);
  fprintf(stderr, "  In: start = %x\n", (int) start);
  fprintf(stderr, "  In: end = %x\n", (int) end);

  /* FIXME --- I don't think we need to do anything here */
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_enter_usr32()
 *
 * Description:   Enter 32 bit user mode - RISC O S 4+
 *
 * Other notes:   Emulation of SWI 0x73.
 */

os_error *xos_enter_usr32 (void)
{
  fprintf(stderr, "os_enter_usr32\n");
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_enter_usr26()
 *
 * Description:   Enter 26 bit user mode - RISC O S 4+
 *
 * Other notes:   Emulation of SWI 0x74.
 */

os_error *xos_enter_usr26 (void)
{
  fprintf(stderr, "os_enter_usr26\n");
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_convert_standard_date_and_time()
 *
 * Description:   Converts a 5-byte time into a string - prefer
 *                Territory_ConvertStandardDateAndTime
 *
 * Input:         date_and_time - value of R0 on entry
 *                buffer - value of R1 on entry
 *                size - value of R2 on entry
 *
 * Output:        end - value of R1 on exit (X version only)
 *
 * Returns:       R1 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0xC0.
 */

os_error *xos_convert_standard_date_and_time (os_date_and_time const *date_and_time,
      char *buffer,
      int size,
      char **end)
{
  fprintf(stderr, "os_convert_standard_date_and_time\n");
  fprintf(stderr, "  In: date_and_time = %x\n", (int) date_and_time);
  fprintf(stderr, "  In: buffer = %x\n", (int) buffer);
  fprintf(stderr, "  In: size = %x\n", (int) size);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: end = %x\n", (int) *end);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_convert_date_and_time()
 *
 * Description:   Converts a 5-byte time into a string using a supplied
 *                format string - prefer Territory_ConvertDateAndTime
 *
 * Input:         date_and_time - value of R0 on entry
 *                buffer - value of R1 on entry
 *                size - value of R2 on entry
 *                format - value of R3 on entry
 *
 * Output:        end - value of R1 on exit (X version only)
 *
 * Returns:       R1 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0xC1.
 */

os_error *xos_convert_date_and_time (os_date_and_time const *date_and_time,
      char *buffer,
      int size,
      char const *format,
      char **end)
{
  fprintf(stderr, "os_convert_date_and_time\n");
  fprintf(stderr, "  In: date_and_time = %x\n", (int) date_and_time);
  fprintf(stderr, "  In: buffer = %x\n", (int) buffer);
  fprintf(stderr, "  In: size = %x\n", (int) size);
  fprintf(stderr, "  In: format = %x\n", (int) format);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: end = %x\n", (int) *end);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_convert_hex1()
 *
 * Description:   Converts a number into a 1-digit hexadecimal string
 *
 * Input:         value - value of R0 on entry
 *                buffer - value of R1 on entry
 *                size - value of R2 on entry
 *
 * Output:        end - value of R1 on exit (X version only)
 *
 * Returns:       R1 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0xD0.
 */

os_error *xos_convert_hex1 (int value,
      char *buffer,
      int size,
      char **end)
{
  fprintf(stderr, "os_convert_hex1\n");
  fprintf(stderr, "  In: value = %x\n", (int) value);
  fprintf(stderr, "  In: buffer = %x\n", (int) buffer);
  fprintf(stderr, "  In: size = %x\n", (int) size);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: end = %x\n", (int) *end);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_convert_hex2()
 *
 * Description:   Converts a number into a 2-digit hexadecimal string
 *
 * Input:         value - value of R0 on entry
 *                buffer - value of R1 on entry
 *                size - value of R2 on entry
 *
 * Output:        end - value of R1 on exit (X version only)
 *
 * Returns:       R1 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0xD1.
 */

os_error *xos_convert_hex2 (int value,
      char *buffer,
      int size,
      char **end)
{
  fprintf(stderr, "os_convert_hex2\n");
  fprintf(stderr, "  In: value = %x\n", (int) value);
  fprintf(stderr, "  In: buffer = %x\n", (int) buffer);
  fprintf(stderr, "  In: size = %x\n", (int) size);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: end = %x\n", (int) *end);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_convert_hex4()
 *
 * Description:   Converts a number into a 4-digit hexadecimal string
 *
 * Input:         value - value of R0 on entry
 *                buffer - value of R1 on entry
 *                size - value of R2 on entry
 *
 * Output:        end - value of R1 on exit (X version only)
 *
 * Returns:       R1 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0xD2.
 */

os_error *xos_convert_hex4 (int value,
      char *buffer,
      int size,
      char **end)
{
  fprintf(stderr, "os_convert_hex4\n");
  fprintf(stderr, "  In: value = %x\n", (int) value);
  fprintf(stderr, "  In: buffer = %x\n", (int) buffer);
  fprintf(stderr, "  In: size = %x\n", (int) size);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: end = %x\n", (int) *end);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_convert_hex6()
 *
 * Description:   Converts a number into a 6-digit hexadecimal string
 *
 * Input:         value - value of R0 on entry
 *                buffer - value of R1 on entry
 *                size - value of R2 on entry
 *
 * Output:        end - value of R1 on exit (X version only)
 *
 * Returns:       R1 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0xD3.
 */

os_error *xos_convert_hex6 (int value,
      char *buffer,
      int size,
      char **end)
{
  fprintf(stderr, "os_convert_hex6\n");
  fprintf(stderr, "  In: value = %x\n", (int) value);
  fprintf(stderr, "  In: buffer = %x\n", (int) buffer);
  fprintf(stderr, "  In: size = %x\n", (int) size);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: end = %x\n", (int) *end);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_convert_hex8()
 *
 * Description:   Converts a number into an 8-digit hexadecimal string
 *
 * Input:         value - value of R0 on entry
 *                buffer - value of R1 on entry
 *                size - value of R2 on entry
 *
 * Output:        end - value of R1 on exit (X version only)
 *
 * Returns:       R1 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0xD4.
 */

os_error *xos_convert_hex8 (int value,
      char *buffer,
      int size,
      char **end)
{
  fprintf(stderr, "os_convert_hex8\n");
  fprintf(stderr, "  In: value = %x\n", (int) value);
  fprintf(stderr, "  In: buffer = %x\n", (int) buffer);
  fprintf(stderr, "  In: size = %x\n", (int) size);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: end = %x\n", (int) *end);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_convert_cardinal1()
 *
 * Description:   Converts a 1-byte number into an unsigned decimal string
 *
 * Input:         value - value of R0 on entry
 *                buffer - value of R1 on entry
 *                size - value of R2 on entry
 *
 * Output:        end - value of R1 on exit (X version only)
 *
 * Returns:       R1 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0xD5.
 */

os_error *xos_convert_cardinal1 (int value,
      char *buffer,
      int size,
      char **end)
{
  fprintf(stderr, "os_convert_cardinal1\n");
  fprintf(stderr, "  In: value = %x\n", (int) value);
  fprintf(stderr, "  In: buffer = %x\n", (int) buffer);
  fprintf(stderr, "  In: size = %x\n", (int) size);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: end = %x\n", (int) *end);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_convert_cardinal2()
 *
 * Description:   Converts a 2-byte number into an unsigned decimal string
 *
 * Input:         value - value of R0 on entry
 *                buffer - value of R1 on entry
 *                size - value of R2 on entry
 *
 * Output:        end - value of R1 on exit (X version only)
 *
 * Returns:       R1 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0xD6.
 */

os_error *xos_convert_cardinal2 (int value,
      char *buffer,
      int size,
      char **end)
{
  fprintf(stderr, "os_convert_cardinal2\n");
  fprintf(stderr, "  In: value = %x\n", (int) value);
  fprintf(stderr, "  In: buffer = %x\n", (int) buffer);
  fprintf(stderr, "  In: size = %x\n", (int) size);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: end = %x\n", (int) *end);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_convert_cardinal3()
 *
 * Description:   Converts a 3-byte number into an unsigned decimal string
 *
 * Input:         value - value of R0 on entry
 *                buffer - value of R1 on entry
 *                size - value of R2 on entry
 *
 * Output:        end - value of R1 on exit (X version only)
 *
 * Returns:       R1 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0xD7.
 */

os_error *xos_convert_cardinal3 (int value,
      char *buffer,
      int size,
      char **end)
{
  fprintf(stderr, "os_convert_cardinal3\n");
  fprintf(stderr, "  In: value = %x\n", (int) value);
  fprintf(stderr, "  In: buffer = %x\n", (int) buffer);
  fprintf(stderr, "  In: size = %x\n", (int) size);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: end = %x\n", (int) *end);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_convert_cardinal4()
 *
 * Description:   Converts a 4-byte number into an unsigned decimal string
 *
 * Input:         value - value of R0 on entry
 *                buffer - value of R1 on entry
 *                size - value of R2 on entry
 *
 * Output:        end - value of R1 on exit (X version only)
 *
 * Returns:       R1 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0xD8.
 */

os_error *xos_convert_cardinal4 (int value,
      char *buffer,
      int size,
      char **end)
{
  fprintf(stderr, "os_convert_cardinal4\n");
  fprintf(stderr, "  In: value = %x\n", (int) value);
  fprintf(stderr, "  In: buffer = %x\n", (int) buffer);
  fprintf(stderr, "  In: size = %x\n", (int) size);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: end = %x\n", (int) *end);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_convert_integer1()
 *
 * Description:   Converts a 1-byte number into a signed decimal string
 *
 * Input:         value - value of R0 on entry
 *                buffer - value of R1 on entry
 *                size - value of R2 on entry
 *
 * Output:        end - value of R1 on exit (X version only)
 *
 * Returns:       R1 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0xD9.
 */

os_error *xos_convert_integer1 (int value,
      char *buffer,
      int size,
      char **end)
{
  fprintf(stderr, "os_convert_integer1\n");
  fprintf(stderr, "  In: value = %x\n", (int) value);
  fprintf(stderr, "  In: buffer = %x\n", (int) buffer);
  fprintf(stderr, "  In: size = %x\n", (int) size);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: end = %x\n", (int) *end);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_convert_integer2()
 *
 * Description:   Converts a 2-byte number into a signed decimal string
 *
 * Input:         value - value of R0 on entry
 *                buffer - value of R1 on entry
 *                size - value of R2 on entry
 *
 * Output:        end - value of R1 on exit (X version only)
 *
 * Returns:       R1 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0xDA.
 */

os_error *xos_convert_integer2 (int value,
      char *buffer,
      int size,
      char **end)
{
  fprintf(stderr, "os_convert_integer2\n");
  fprintf(stderr, "  In: value = %x\n", (int) value);
  fprintf(stderr, "  In: buffer = %x\n", (int) buffer);
  fprintf(stderr, "  In: size = %x\n", (int) size);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: end = %x\n", (int) *end);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_convert_integer3()
 *
 * Description:   Converts a 3-byte number into a signed decimal string
 *
 * Input:         value - value of R0 on entry
 *                buffer - value of R1 on entry
 *                size - value of R2 on entry
 *
 * Output:        end - value of R1 on exit (X version only)
 *
 * Returns:       R1 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0xDB.
 */

os_error *xos_convert_integer3 (int value,
      char *buffer,
      int size,
      char **end)
{
  fprintf(stderr, "os_convert_integer3\n");
  fprintf(stderr, "  In: value = %x\n", (int) value);
  fprintf(stderr, "  In: buffer = %x\n", (int) buffer);
  fprintf(stderr, "  In: size = %x\n", (int) size);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: end = %x\n", (int) *end);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_convert_integer4()
 *
 * Description:   Converts a 4-byte number into a signed decimal string
 *
 * Input:         value - value of R0 on entry
 *                buffer - value of R1 on entry
 *                size - value of R2 on entry
 *
 * Output:        end - value of R1 on exit (X version only)
 *
 * Returns:       R1 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0xDC.
 */

os_error *xos_convert_integer4 (int value,
      char *buffer,
      int size,
      char **end)
{
  fprintf(stderr, "os_convert_integer4\n");
  fprintf(stderr, "  In: value = %x\n", (int) value);
  fprintf(stderr, "  In: buffer = %x\n", (int) buffer);
  fprintf(stderr, "  In: size = %x\n", (int) size);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: end = %x\n", (int) *end);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_convert_binary1()
 *
 * Description:   Converts a 1-byte number into an unsigned binary string
 *
 * Input:         value - value of R0 on entry
 *                buffer - value of R1 on entry
 *                size - value of R2 on entry
 *
 * Output:        end - value of R1 on exit (X version only)
 *
 * Returns:       R1 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0xDD.
 */

os_error *xos_convert_binary1 (int value,
      char *buffer,
      int size,
      char **end)
{
  fprintf(stderr, "os_convert_binary1\n");
  fprintf(stderr, "  In: value = %x\n", (int) value);
  fprintf(stderr, "  In: buffer = %x\n", (int) buffer);
  fprintf(stderr, "  In: size = %x\n", (int) size);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: end = %x\n", (int) *end);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_convert_binary2()
 *
 * Description:   Converts a 2-byte number into an unsigned binary string
 *
 * Input:         value - value of R0 on entry
 *                buffer - value of R1 on entry
 *                size - value of R2 on entry
 *
 * Output:        end - value of R1 on exit (X version only)
 *
 * Returns:       R1 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0xDE.
 */

os_error *xos_convert_binary2 (int value,
      char *buffer,
      int size,
      char **end)
{
  fprintf(stderr, "os_convert_binary2\n");
  fprintf(stderr, "  In: value = %x\n", (int) value);
  fprintf(stderr, "  In: buffer = %x\n", (int) buffer);
  fprintf(stderr, "  In: size = %x\n", (int) size);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: end = %x\n", (int) *end);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_convert_binary3()
 *
 * Description:   Converts a 3-byte number into an unsigned binary string
 *
 * Input:         value - value of R0 on entry
 *                buffer - value of R1 on entry
 *                size - value of R2 on entry
 *
 * Output:        end - value of R1 on exit (X version only)
 *
 * Returns:       R1 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0xDF.
 */

os_error *xos_convert_binary3 (int value,
      char *buffer,
      int size,
      char **end)
{
  fprintf(stderr, "os_convert_binary3\n");
  fprintf(stderr, "  In: value = %x\n", (int) value);
  fprintf(stderr, "  In: buffer = %x\n", (int) buffer);
  fprintf(stderr, "  In: size = %x\n", (int) size);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: end = %x\n", (int) *end);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_convert_binary4()
 *
 * Description:   Converts a 4-byte number into an unsigned binary string
 *
 * Input:         value - value of R0 on entry
 *                buffer - value of R1 on entry
 *                size - value of R2 on entry
 *
 * Output:        end - value of R1 on exit (X version only)
 *
 * Returns:       R1 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0xE0.
 */

os_error *xos_convert_binary4 (int value,
      char *buffer,
      int size,
      char **end)
{
  fprintf(stderr, "os_convert_binary4\n");
  fprintf(stderr, "  In: value = %x\n", (int) value);
  fprintf(stderr, "  In: buffer = %x\n", (int) buffer);
  fprintf(stderr, "  In: size = %x\n", (int) size);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: end = %x\n", (int) *end);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_convert_spaced_cardinal1()
 *
 * Description:   Converts a 1-byte number into a spaced unsigned decimal
 *                string
 *
 * Input:         value - value of R0 on entry
 *                buffer - value of R1 on entry
 *                size - value of R2 on entry
 *
 * Output:        end - value of R1 on exit (X version only)
 *
 * Returns:       R1 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0xE1.
 */

os_error *xos_convert_spaced_cardinal1 (int value,
      char *buffer,
      int size,
      char **end)
{
  fprintf(stderr, "os_convert_spaced_cardinal1\n");
  fprintf(stderr, "  In: value = %x\n", (int) value);
  fprintf(stderr, "  In: buffer = %x\n", (int) buffer);
  fprintf(stderr, "  In: size = %x\n", (int) size);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: end = %x\n", (int) *end);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_convert_spaced_cardinal2()
 *
 * Description:   Converts a 2-byte number into a spaced unsigned decimal
 *                string
 *
 * Input:         value - value of R0 on entry
 *                buffer - value of R1 on entry
 *                size - value of R2 on entry
 *
 * Output:        end - value of R1 on exit (X version only)
 *
 * Returns:       R1 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0xE2.
 */

os_error *xos_convert_spaced_cardinal2 (int value,
      char *buffer,
      int size,
      char **end)
{
  fprintf(stderr, "os_convert_spaced_cardinal2\n");
  fprintf(stderr, "  In: value = %x\n", (int) value);
  fprintf(stderr, "  In: buffer = %x\n", (int) buffer);
  fprintf(stderr, "  In: size = %x\n", (int) size);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: end = %x\n", (int) *end);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_convert_spaced_cardinal3()
 *
 * Description:   Converts a 3-byte number into a spaced unsigned decimal
 *                string
 *
 * Input:         value - value of R0 on entry
 *                buffer - value of R1 on entry
 *                size - value of R2 on entry
 *
 * Output:        end - value of R1 on exit (X version only)
 *
 * Returns:       R1 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0xE3.
 */

os_error *xos_convert_spaced_cardinal3 (int value,
      char *buffer,
      int size,
      char **end)
{
  fprintf(stderr, "os_convert_spaced_cardinal3\n");
  fprintf(stderr, "  In: value = %x\n", (int) value);
  fprintf(stderr, "  In: buffer = %x\n", (int) buffer);
  fprintf(stderr, "  In: size = %x\n", (int) size);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: end = %x\n", (int) *end);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_convert_spaced_cardinal4()
 *
 * Description:   Converts a 4-byte number into a spaced unsigned decimal
 *                string
 *
 * Input:         value - value of R0 on entry
 *                buffer - value of R1 on entry
 *                size - value of R2 on entry
 *
 * Output:        end - value of R1 on exit (X version only)
 *
 * Returns:       R1 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0xE4.
 */

os_error *xos_convert_spaced_cardinal4 (int value,
      char *buffer,
      int size,
      char **end)
{
  fprintf(stderr, "os_convert_spaced_cardinal4\n");
  fprintf(stderr, "  In: value = %x\n", (int) value);
  fprintf(stderr, "  In: buffer = %x\n", (int) buffer);
  fprintf(stderr, "  In: size = %x\n", (int) size);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: end = %x\n", (int) *end);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_convert_spaced_integer1()
 *
 * Description:   Converts a 1-byte number into a spaced signed decimal
 *                string
 *
 * Input:         value - value of R0 on entry
 *                buffer - value of R1 on entry
 *                size - value of R2 on entry
 *
 * Output:        end - value of R1 on exit (X version only)
 *
 * Returns:       R1 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0xE5.
 */

os_error *xos_convert_spaced_integer1 (int value,
      char *buffer,
      int size,
      char **end)
{
  fprintf(stderr, "os_convert_spaced_integer1\n");
  fprintf(stderr, "  In: value = %x\n", (int) value);
  fprintf(stderr, "  In: buffer = %x\n", (int) buffer);
  fprintf(stderr, "  In: size = %x\n", (int) size);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: end = %x\n", (int) *end);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_convert_spaced_integer2()
 *
 * Description:   Converts a 2-byte number into a spaced signed decimal
 *                string
 *
 * Input:         value - value of R0 on entry
 *                buffer - value of R1 on entry
 *                size - value of R2 on entry
 *
 * Output:        end - value of R1 on exit (X version only)
 *
 * Returns:       R1 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0xE6.
 */

os_error *xos_convert_spaced_integer2 (int value,
      char *buffer,
      int size,
      char **end)
{
  fprintf(stderr, "os_convert_spaced_integer2\n");
  fprintf(stderr, "  In: value = %x\n", (int) value);
  fprintf(stderr, "  In: buffer = %x\n", (int) buffer);
  fprintf(stderr, "  In: size = %x\n", (int) size);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: end = %x\n", (int) *end);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_convert_spaced_integer3()
 *
 * Description:   Converts a 3-byte number into a spaced signed decimal
 *                string
 *
 * Input:         value - value of R0 on entry
 *                buffer - value of R1 on entry
 *                size - value of R2 on entry
 *
 * Output:        end - value of R1 on exit (X version only)
 *
 * Returns:       R1 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0xE7.
 */

os_error *xos_convert_spaced_integer3 (int value,
      char *buffer,
      int size,
      char **end)
{
  fprintf(stderr, "os_convert_spaced_integer3\n");
  fprintf(stderr, "  In: value = %x\n", (int) value);
  fprintf(stderr, "  In: buffer = %x\n", (int) buffer);
  fprintf(stderr, "  In: size = %x\n", (int) size);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: end = %x\n", (int) *end);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_convert_spaced_integer4()
 *
 * Description:   Converts a 4-byte number into a spaced signed decimal
 *                string
 *
 * Input:         value - value of R0 on entry
 *                buffer - value of R1 on entry
 *                size - value of R2 on entry
 *
 * Output:        end - value of R1 on exit (X version only)
 *
 * Returns:       R1 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0xE8.
 */

os_error *xos_convert_spaced_integer4 (int value,
      char *buffer,
      int size,
      char **end)
{
  fprintf(stderr, "os_convert_spaced_integer4\n");
  fprintf(stderr, "  In: value = %x\n", (int) value);
  fprintf(stderr, "  In: buffer = %x\n", (int) buffer);
  fprintf(stderr, "  In: size = %x\n", (int) size);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: end = %x\n", (int) *end);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_convert_fixed_net_station()
 *
 * Description:   Converts from an Econet station/network number pair to a
 *                string of fixed length
 *
 * Input:         station_number - value of R0 on entry
 *                buffer - value of R1 on entry
 *                size - value of R2 on entry
 *
 * Output:        end - value of R1 on exit (X version only)
 *
 * Returns:       R1 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0xE9.
 */

os_error *xos_convert_fixed_net_station (os_station_number const *station_number,
      char *buffer,
      int size,
      char **end)
{
  fprintf(stderr, "os_convert_fixed_net_station\n");
  fprintf(stderr, "  In: station_number = %x\n", (int) station_number);
  fprintf(stderr, "  In: buffer = %x\n", (int) buffer);
  fprintf(stderr, "  In: size = %x\n", (int) size);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: end = %x\n", (int) *end);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_convert_net_station()
 *
 * Description:   Converts from an Econet station/network number pair to a
 *                string
 *
 * Input:         station_number - value of R0 on entry
 *                buffer - value of R1 on entry
 *                size - value of R2 on entry
 *
 * Output:        end - value of R1 on exit (X version only)
 *
 * Returns:       R1 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0xEA.
 */

os_error *xos_convert_net_station (os_station_number const *station_number,
      char *buffer,
      int size,
      char **end)
{
  fprintf(stderr, "os_convert_net_station\n");
  fprintf(stderr, "  In: station_number = %x\n", (int) station_number);
  fprintf(stderr, "  In: buffer = %x\n", (int) buffer);
  fprintf(stderr, "  In: size = %x\n", (int) size);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: end = %x\n", (int) *end);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_convert_fixed_file_size()
 *
 * Description:   Converts an integer into a filesize string of fixed
 *                length
 *
 * Input:         file_size - value of R0 on entry
 *                buffer - value of R1 on entry
 *                size - value of R2 on entry
 *
 * Output:        end - value of R1 on exit (X version only)
 *
 * Returns:       R1 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0xEB.
 */

os_error *xos_convert_fixed_file_size (int file_size,
      char *buffer,
      int size,
      char **end)
{
  fprintf(stderr, "os_convert_fixed_file_size\n");
  fprintf(stderr, "  In: file_size = %x\n", (int) file_size);
  fprintf(stderr, "  In: buffer = %x\n", (int) buffer);
  fprintf(stderr, "  In: size = %x\n", (int) size);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: end = %x\n", (int) *end);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_convert_file_size()
 *
 * Description:   Converts an integer into a filesize string
 *
 * Input:         file_size - value of R0 on entry
 *                buffer - value of R1 on entry
 *                size - value of R2 on entry
 *
 * Output:        end - value of R1 on exit (X version only)
 *
 * Returns:       R1 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0xEC.
 */

os_error *xos_convert_file_size (int file_size,
      char *buffer,
      int size,
      char **end)
{
  fprintf(stderr, "os_convert_file_size\n");
  fprintf(stderr, "  In: file_size = %x\n", (int) file_size);
  fprintf(stderr, "  In: buffer = %x\n", (int) buffer);
  fprintf(stderr, "  In: size = %x\n", (int) size);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: end = %x\n", (int) *end);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_null()
 *
 * Description:   VDU command for nul (no operation)
 *
 * Other notes:   Emulation of SWI 0x100.
 */

os_error *xos_null (void)
{
  fprintf(stderr, "os_null\n");
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_char_to_printer()
 *
 * Description:   VDU command to send the next character to the printer
 *                only - must be followed by 'char'
 *
 * Other notes:   Emulation of SWI 0x101.
 */

os_error *xos_char_to_printer (void)
{
  fprintf(stderr, "os_char_to_printer\n");
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_printer_on()
 *
 * Description:   VDU command to enable the printer stream - prefer PDriver
 *
 * Other notes:   Emulation of SWI 0x102.
 */

os_error *xos_printer_on (void)
{
  fprintf(stderr, "os_printer_on\n");
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_printer_off()
 *
 * Description:   VDU command to disable the printer stream - prefer
 *                PDriver
 *
 * Other notes:   Emulation of SWI 0x103.
 */

os_error *xos_printer_off (void)
{
  fprintf(stderr, "os_printer_off\n");
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_split_cursors()
 *
 * Description:   VDU command to split the text and graphics cursors
 *
 * Other notes:   Emulation of SWI 0x104.
 */

os_error *xos_split_cursors (void)
{
  fprintf(stderr, "os_split_cursors\n");
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_join_cursors()
 *
 * Description:   VDU command to join the text and graphics cursors
 *
 * Other notes:   Emulation of SWI 0x105.
 */

os_error *xos_join_cursors (void)
{
  fprintf(stderr, "os_join_cursors\n");
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_vdu_on()
 *
 * Description:   VDU command to enable screen output
 *
 * Other notes:   Emulation of SWI 0x106.
 */

os_error *xos_vdu_on (void)
{
  fprintf(stderr, "os_vdu_on\n");
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_bell()
 *
 * Description:   VDU command to ring the bell
 *
 * Other notes:   Emulation of SWI 0x107.
 */

os_error *xos_bell (void)
{
  fprintf(stderr, "os_bell\n");
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_backspace()
 *
 * Description:   VDU command to perform a backspace
 *
 * Other notes:   Emulation of SWI 0x108.
 */

os_error *xos_backspace (void)
{
  fprintf(stderr, "os_backspace\n");
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_tab()
 *
 * Description:   VDU command to perform a horizontal tab
 *
 * Other notes:   Emulation of SWI 0x109.
 */

os_error *xos_tab (void)
{
  fprintf(stderr, "os_tab\n");
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_line_feed()
 *
 * Description:   VDU command to perform a linefeed
 *
 * Other notes:   Emulation of SWI 0x10A.
 */

os_error *xos_line_feed (void)
{
  fprintf(stderr, "os_line_feed\n");
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_vertical_tab()
 *
 * Description:   VDU command to perform a vertical tab
 *
 * Other notes:   Emulation of SWI 0x10B.
 */

os_error *xos_vertical_tab (void)
{
  fprintf(stderr, "os_vertical_tab\n");
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_cls()
 *
 * Description:   VDU command to clear the text window
 *
 * Other notes:   Emulation of SWI 0x10C.
 */

os_error *xos_cls (void)
{
  fprintf(stderr, "os_cls\n");
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_return()
 *
 * Description:   VDU command to perform a return
 *
 * Other notes:   Emulation of SWI 0x10D.
 */

os_error *xos_return (void)
{
  fprintf(stderr, "os_return\n");
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_page_mode_on()
 *
 * Description:   VDU command to turn paged mode on
 *
 * Other notes:   Emulation of SWI 0x10E.
 */

os_error *xos_page_mode_on (void)
{
  fprintf(stderr, "os_page_mode_on\n");
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_page_mode_off()
 *
 * Description:   VDU command to turn paged mode off
 *
 * Other notes:   Emulation of SWI 0x10F.
 */

os_error *xos_page_mode_off (void)
{
  fprintf(stderr, "os_page_mode_off\n");
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_clg()
 *
 * Description:   VDU command to clear the graphics window
 *
 * Other notes:   Emulation of SWI 0x110.
 */

os_error *xos_clg (void)
{
  fprintf(stderr, "os_clg\n");
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_set_text_colour()
 *
 * Description:   VDU command to set the text colour - must be followed by
 *                'colour' - prefer OS_SetColour
 *
 * Other notes:   Emulation of SWI 0x111.
 */

os_error *xos_set_text_colour (void)
{
  fprintf(stderr, "os_set_text_colour\n");
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_set_gcol()
 *
 * Description:   VDU command to set the graphics colour and action - must
 *                be followed by 'action,' 'colour' - prefer OS_SetColour
 *
 * Other notes:   Emulation of SWI 0x112.
 */

os_error *xos_set_gcol (void)
{
  fprintf(stderr, "os_set_gcol\n");
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_set_palette()
 *
 * Description:   VDU command to set the palette - must be followed by
 *                'colour,' 'mode,' 'red,' 'green,' 'blue'
 *
 * Other notes:   Emulation of SWI 0x113.
 */

os_error *xos_set_palette (void)
{
  fprintf(stderr, "os_set_palette\n");
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_reset_colours()
 *
 * Description:   VDU command to restore default colours
 *
 * Other notes:   Emulation of SWI 0x114.
 */

os_error *xos_reset_colours (void)
{
  fprintf(stderr, "os_reset_colours\n");
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_vdu_off()
 *
 * Description:   VDU command to disable screen output
 *
 * Other notes:   Emulation of SWI 0x115.
 */

os_error *xos_vdu_off (void)
{
  fprintf(stderr, "os_vdu_off\n");
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_set_mode()
 *
 * Description:   VDU command to change display mode - must be followed by
 *                'mode' - prefer OS_ScreenMode
 *
 * Other notes:   Emulation of SWI 0x116.
 */

os_error *xos_set_mode (void)
{
  fprintf(stderr, "os_set_mode\n");
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_misc()
 *
 * Description:   Miscellaneous VDU commands - must be followed by
 *                'command' and 8 more arguments
 *
 * Other notes:   Emulation of SWI 0x117.
 */

os_error *xos_misc (void)
{
  fprintf(stderr, "os_misc\n");
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_set_graphics_window()
 *
 * Description:   VDU command to define the graphics window - must be
 *                followed by 'x0,' 'y0,' 'x1,' 'y1' (2 bytes each)
 *
 * Other notes:   Emulation of SWI 0x118.
 */

os_error *xos_set_graphics_window (void)
{
  fprintf(stderr, "os_set_graphics_window\n");
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_plot_vdu()
 *
 * Description:   General VDU plot commands - must be followed by
 *                'plot_type' and 'x,' 'y' (2 bytes each) - prefer OS_Plot
 *
 * Other notes:   Emulation of SWI 0x119.
 */

os_error *xos_plot_vdu (void)
{
  fprintf(stderr, "os_plot_vdu\n");
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_reset_windows()
 *
 * Description:   VDU command to restore default windows
 *
 * Other notes:   Emulation of SWI 0x11A.
 */

os_error *xos_reset_windows (void)
{
  fprintf(stderr, "os_reset_windows\n");
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_escape()
 *
 * Description:   VDU command for escape (no operation)
 *
 * Other notes:   Emulation of SWI 0x11B.
 */

os_error *xos_escape (void)
{
  fprintf(stderr, "os_escape\n");
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_set_text_window()
 *
 * Description:   VDU command to define the text window - must be followed
 *                by 'x0,' 'y0,' 'x1,' 'y1'
 *
 * Other notes:   Emulation of SWI 0x11C.
 */

os_error *xos_set_text_window (void)
{
  fprintf(stderr, "os_set_text_window\n");
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_set_graphics_origin()
 *
 * Description:   VDU command to set the graphics origin - must be followed
 *                by 'x,' 'y' (2 bytes each)
 *
 * Other notes:   Emulation of SWI 0x11D.
 */

os_error *xos_set_graphics_origin (void)
{
  fprintf(stderr, "os_set_graphics_origin\n");
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_home_text_cursor()
 *
 * Description:   VDU command to home the text cursor
 *
 * Other notes:   Emulation of SWI 0x11E.
 */

os_error *xos_home_text_cursor (void)
{
  fprintf(stderr, "os_home_text_cursor\n");
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_set_text_cursor()
 *
 * Description:   VDU command to position the text cursor - must be
 *                followed by 'x,' 'y'
 *
 * Other notes:   Emulation of SWI 0x11F.
 */

os_error *xos_set_text_cursor (void)
{
  fprintf(stderr, "os_set_text_cursor\n");
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_space()
 *
 * Description:   VDU command to perform a space
 *
 * Other notes:   Emulation of SWI 0x120.
 */

os_error *xos_space (void)
{
  fprintf(stderr, "os_space\n");
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      os_delete()
 *
 * Description:   VDU command to perform a delete
 *
 * Other notes:   Emulation of SWI 0x17F.
 */

os_error *xos_delete (void)
{
  fprintf(stderr, "os_delete\n");
  error("*** SWI unimplemented\n");
  return 0;
}

