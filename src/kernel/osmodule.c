/* osmodule.c
**
** (c) Matthew Bloch 2001
**
** See http://riscose.sourceforge.net/ for terms of distribution, and to
** pick up a later version of the software.
**
**   Emulation of the kernel provided OS_ SWIs; most of the `biggies' like
**   OS_File or OS_Word ought to be farmed out to separate files to avoid
**   clutter.
**
**   $Revision$
**   $Date$
*/
#include "arm.h"
#include "swi.h"

void
osmodule(void)
{
  switch (ARM_R0)
    {
      case 0 : /* Run 1-227 */
      case 1 : /* Load 1-228 */
      case 2 : /* Enter 1-229 */
      case 3 : /* ReInit 1-230 */
      case 4 : /* Delete 1-231 */
      case 5 : /* Describe RMA 1-232 */
      case 6 : /* Claim 1-233 */
      case 7 : /* Free 1-234 */
      case 8 : /* Tidy 1-235 */
      case 9 : /* Clear 1-236 */
      case 10 : /* Insert module from memory 1-237 */
      case 11 : /* Insert module from memory and move into RMA 1-238 */
      case 12 : /* Extract module information 1-239 */
      case 13 : /* Extend block 1-240 */
      case 14 : /* Create new instantiation 1-241 */
      case 15 : /* Rename instantiation 1-242 */
      case 16 : /* Make preferred instantiation 1-243 */
      case 17 : /* Add expansion card module 1-244 */
      case 18 : /* Lookup module name 1-245 */
      case 19 : /* Enumerate ROM modules 1-246 */
      case 20 : /* Enumerate ROM modules with version 1-248 */
    }
}
