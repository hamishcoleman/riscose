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

os_error *xmessagetrans_file_info (char *file_name,
      messagetrans_file_flags *flags,
      int *size)
{
  error("*** SWI unimplemented\n");
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
      char *file_name,
      char *buffer)
{
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

os_error *xmessagetrans_lookup (messagetrans_control_block *cb,
      char *token,
      char *buffer,
      int size,
      char *arg0,
      char *arg1,
      char *arg2,
      char *arg3,
      char **terminator,
      char **result,
      int *used)
{
  error("*** SWI unimplemented\n");
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

os_error *xmessagetrans_make_menus (messagetrans_control_block *cb,
      byte *menus,
      char *buffer,
      int size,
      byte **end,
      int *used)
{
  error("*** SWI unimplemented\n");
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

os_error *xmessagetrans_close_file (messagetrans_control_block *cb)
{
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

os_error *xmessagetrans_enumerate_tokens (messagetrans_control_block *cb,
      char *token,
      char *buffer,
      int size,
      int context,
      osbool *more,
      int *used,
      int *context_out)
{
  error("*** SWI unimplemented\n");
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

os_error *xmessagetrans_error_lookup (os_error *err,
      messagetrans_control_block *cb,
      os_error *buffer,
      int size,
      char *arg0,
      char *arg1,
      char *arg2,
      char *arg3,
      os_error **out_buffer)
{
  error("*** SWI unimplemented\n");
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

os_error *xmessagetrans_gs_lookup (messagetrans_control_block *cb,
      char *token,
      char *buffer,
      int size,
      char *arg0,
      char *arg1,
      char *arg2,
      char *arg3,
      char **result,
      int *used)
{
  error("*** SWI unimplemented\n");
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

os_error *xmessagetrans_copy_error (os_error *err,
      os_error **copy)
{
  error("*** SWI unimplemented\n");
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
  error("*** SWI unimplemented\n");
  return 0;
}

