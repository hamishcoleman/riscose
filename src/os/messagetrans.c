/* os/messagetrans.c
**
** See http://riscose.sourceforge.net/ for terms of distribution, and to
** pick up a later version of the software.
**
**   Emulation of the MessageTrans SWIs.
**
**   Template written by defmod (riscose version 1.00)
*/

#include <stdio.h>
#include <monty/monty.h>
#include "types.h"
#include "messagetrans.h"

void messagetrans_swi_register_extra(void)
{

}

/* ------------------------------------------------------------------------
 * Function:      messagetrans_file_info()
 *
 * Description:   Gives information about a message file
 *
 * Input:         file_name - value of R1 on entry
 *
 * Output:        flags - value of R0 on exit
 *                size - value of R2 on exit
 *
 * Other notes:   Emulation of SWI 0x41500.
 */

os_error *xmessagetrans_file_info (char const *file_name,
      messagetrans_file_flags *flags,
      int *size)
{
  fprintf(stderr, "messagetrans_file_info\n");
  fprintf(stderr, "  In: file_name = %x\n", (int) file_name);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: flags = %x\n", (int) *flags);
  fprintf(stderr, "  Out: size = %x\n", (int) *size);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      messagetrans_open_file()
 *
 * Description:   Opens a message file
 *
 * Input:         cb - value of R0 on entry
 *                file_name - value of R1 on entry
 *                buffer - value of R2 on entry
 *
 * Other notes:   Emulation of SWI 0x41501.
 */

os_error *xmessagetrans_open_file (messagetrans_control_block *cb,
      char const *file_name,
      char *buffer)
{
  fprintf(stderr, "messagetrans_open_file\n");
  fprintf(stderr, "  In: cb = %x\n", (int) cb);
  fprintf(stderr, "  In: file_name = %x\n", (int) file_name);
  fprintf(stderr, "  In: buffer = %x\n", (int) buffer);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      messagetrans_lookup()
 *
 * Description:   Translates a message token into a string
 *
 * Input:         cb - value of R0 on entry
 *                token - value of R1 on entry
 *                buffer - value of R2 on entry
 *                size - value of R3 on entry
 *                arg0 - value of R4 on entry
 *                arg1 - value of R5 on entry
 *                arg2 - value of R6 on entry
 *                arg3 - value of R7 on entry
 *
 * Output:        terminator - value of R1 on exit (X version only)
 *                result - value of R2 on exit
 *                used - value of R3 on exit
 *
 * Returns:       R1 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x41502.
 */

os_error *xmessagetrans_lookup (messagetrans_control_block const *cb,
      char const *token,
      char *buffer,
      int size,
      char const *arg0,
      char const *arg1,
      char const *arg2,
      char const *arg3,
      char **terminator,
      char **result,
      int *used)
{
  fprintf(stderr, "messagetrans_lookup\n");
  fprintf(stderr, "  In: cb = %x\n", (int) cb);
  fprintf(stderr, "  In: token = %x\n", (int) token);
  fprintf(stderr, "  In: buffer = %x\n", (int) buffer);
  fprintf(stderr, "  In: size = %x\n", (int) size);
  fprintf(stderr, "  In: arg0 = %x\n", (int) arg0);
  fprintf(stderr, "  In: arg1 = %x\n", (int) arg1);
  fprintf(stderr, "  In: arg2 = %x\n", (int) arg2);
  fprintf(stderr, "  In: arg3 = %x\n", (int) arg3);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: terminator = %x\n", (int) *terminator);
  fprintf(stderr, "  Out: result = %x\n", (int) *result);
  fprintf(stderr, "  Out: used = %x\n", (int) *used);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      messagetrans_make_menus()
 *
 * Description:   Sets up a menu structure from a definition containing
 *                references to tokens
 *
 * Input:         cb - value of R0 on entry
 *                menus - value of R1 on entry
 *                buffer - value of R2 on entry
 *                size - value of R3 on entry
 *
 * Output:        end - value of R2 on exit (X version only)
 *                used - value of R3 on exit
 *
 * Returns:       R2 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x41503.
 */

os_error *xmessagetrans_make_menus (messagetrans_control_block const *cb,
      byte const *menus,
      char *buffer,
      int size,
      byte **end,
      int *used)
{
  fprintf(stderr, "messagetrans_make_menus\n");
  fprintf(stderr, "  In: cb = %x\n", (int) cb);
  fprintf(stderr, "  In: menus = %x\n", (int) menus);
  fprintf(stderr, "  In: buffer = %x\n", (int) buffer);
  fprintf(stderr, "  In: size = %x\n", (int) size);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: end = %x\n", (int) *end);
  fprintf(stderr, "  Out: used = %x\n", (int) *used);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      messagetrans_close_file()
 *
 * Description:   Closes a message file
 *
 * Input:         cb - value of R0 on entry
 *
 * Other notes:   Emulation of SWI 0x41504.
 */

os_error *xmessagetrans_close_file (messagetrans_control_block const *cb)
{
  fprintf(stderr, "messagetrans_close_file\n");
  fprintf(stderr, "  In: cb = %x\n", (int) cb);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      messagetrans_enumerate_tokens()
 *
 * Description:   Enumerates tokens that match a wildcarded token
 *
 * Input:         cb - value of R0 on entry
 *                token - value of R1 on entry
 *                buffer - value of R2 on entry
 *                size - value of R3 on entry
 *                context - value of R4 on entry
 *
 * Output:        more - value of R2 on exit (X version only)
 *                used - value of R3 on exit
 *                context_out - value of R4 on exit
 *
 * Returns:       R2 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x41505.
 */

os_error *xmessagetrans_enumerate_tokens (messagetrans_control_block const *cb,
      char const *token,
      char *buffer,
      int size,
      int context,
      osbool *more,
      int *used,
      int *context_out)
{
  fprintf(stderr, "messagetrans_enumerate_tokens\n");
  fprintf(stderr, "  In: cb = %x\n", (int) cb);
  fprintf(stderr, "  In: token = %x\n", (int) token);
  fprintf(stderr, "  In: buffer = %x\n", (int) buffer);
  fprintf(stderr, "  In: size = %x\n", (int) size);
  fprintf(stderr, "  In: context = %x\n", (int) context);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: more = %x\n", (int) *more);
  fprintf(stderr, "  Out: used = %x\n", (int) *used);
  fprintf(stderr, "  Out: context_out = %x\n", (int) *context_out);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      messagetrans_error_lookup()
 *
 * Description:   Translates a message token within an error block
 *
 * Input:         err - value of R0 on entry
 *                cb - value of R1 on entry
 *                buffer - value of R2 on entry
 *                size - value of R3 on entry
 *                arg0 - value of R4 on entry
 *                arg1 - value of R5 on entry
 *                arg2 - value of R6 on entry
 *                arg3 - value of R7 on entry
 *
 * Output:        out_buffer - value of R0 on exit (X version only)
 *
 * Returns:       R0 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x41506.
 */

os_error *xmessagetrans_error_lookup (os_error const *err,
      messagetrans_control_block const *cb,
      os_error *buffer,
      int size,
      char const *arg0,
      char const *arg1,
      char const *arg2,
      char const *arg3,
      os_error **out_buffer)
{
  fprintf(stderr, "messagetrans_error_lookup\n");
  fprintf(stderr, "  In: err = %x\n", (int) err);
  fprintf(stderr, "  In: cb = %x\n", (int) cb);
  fprintf(stderr, "  In: buffer = %x\n", (int) buffer);
  fprintf(stderr, "  In: size = %x\n", (int) size);
  fprintf(stderr, "  In: arg0 = %x\n", (int) arg0);
  fprintf(stderr, "  In: arg1 = %x\n", (int) arg1);
  fprintf(stderr, "  In: arg2 = %x\n", (int) arg2);
  fprintf(stderr, "  In: arg3 = %x\n", (int) arg3);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: out_buffer = %x\n", (int) *out_buffer);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      messagetrans_gs_lookup()
 *
 * Description:   Translates a message token into a string and calls
 *                OS_GSTrans
 *
 * Input:         cb - value of R0 on entry
 *                token - value of R1 on entry
 *                buffer - value of R2 on entry
 *                size - value of R3 on entry
 *                arg0 - value of R4 on entry
 *                arg1 - value of R5 on entry
 *                arg2 - value of R6 on entry
 *                arg3 - value of R7 on entry
 *
 * Output:        result - value of R2 on exit (X version only)
 *                used - value of R3 on exit
 *
 * Returns:       R2 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x41507.
 */

os_error *xmessagetrans_gs_lookup (messagetrans_control_block const *cb,
      char const *token,
      char *buffer,
      int size,
      char const *arg0,
      char const *arg1,
      char const *arg2,
      char const *arg3,
      char **result,
      int *used)
{
  fprintf(stderr, "messagetrans_gs_lookup\n");
  fprintf(stderr, "  In: cb = %x\n", (int) cb);
  fprintf(stderr, "  In: token = %x\n", (int) token);
  fprintf(stderr, "  In: buffer = %x\n", (int) buffer);
  fprintf(stderr, "  In: size = %x\n", (int) size);
  fprintf(stderr, "  In: arg0 = %x\n", (int) arg0);
  fprintf(stderr, "  In: arg1 = %x\n", (int) arg1);
  fprintf(stderr, "  In: arg2 = %x\n", (int) arg2);
  fprintf(stderr, "  In: arg3 = %x\n", (int) arg3);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: result = %x\n", (int) *result);
  fprintf(stderr, "  Out: used = %x\n", (int) *used);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      messagetrans_copy_error()
 *
 * Description:   Copies an error into one of the MessageTrans internal
 *                buffers
 *
 * Input:         err - value of R0 on entry
 *
 * Output:        copy - value of R0 on exit (X version only)
 *
 * Returns:       R0 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x41508.
 */

os_error *xmessagetrans_copy_error (os_error const *err,
      os_error **copy)
{
  fprintf(stderr, "messagetrans_copy_error\n");
  fprintf(stderr, "  In: err = %x\n", (int) err);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: copy = %x\n", (int) *copy);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      messagetrans_dictionary()
 *
 * Description:   Returns a pointer to MessageTrans's internal dictionary
 *
 * Output:        dictionary - value of R0 on exit (X version only)
 *
 * Returns:       R0 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x41509.
 */

os_error *xmessagetrans_dictionary (byte **dictionary)
{
  fprintf(stderr, "messagetrans_dictionary\n");
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: dictionary = %x\n", (int) *dictionary);
  return 0;
}

