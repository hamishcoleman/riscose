/* bdt.c
 *
 * Block data transfer instructions (from genbdt.c)
 *
 * (c) 2000 Chris Rutter <chris@armlinux.org>
 */

#include "arm.h"
#include "mm.h"

static unsigned int bitsSet[] = {0
, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8};

static void loadInc (arm_word *br, arm_word pat, register arm_byte *rbv)
{
  switch (pat)
  {
    case 0:
      break;

    case 1:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 2:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 3:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 4:
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 5:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 6:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 7:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 8:
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 9:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 10:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 11:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 12:
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 13:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 14:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 15:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 16:
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 17:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 18:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 19:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 20:
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 21:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 22:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 23:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 24:
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 25:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 26:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 27:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 28:
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 29:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 30:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 31:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 32:
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 33:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 34:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 35:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 36:
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 37:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 38:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 39:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 40:
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 41:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 42:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 43:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 44:
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 45:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 46:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 47:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 48:
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 49:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 50:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 51:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 52:
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 53:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 54:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 55:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 56:
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 57:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 58:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 59:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 60:
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 61:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 62:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 63:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 64:
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 65:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 66:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 67:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 68:
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 69:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 70:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 71:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 72:
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 73:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 74:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 75:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 76:
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 77:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 78:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 79:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 80:
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 81:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 82:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 83:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 84:
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 85:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 86:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 87:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 88:
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 89:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 90:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 91:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 92:
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 93:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 94:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 95:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 96:
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 97:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 98:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 99:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 100:
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 101:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 102:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 103:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 104:
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 105:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 106:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 107:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 108:
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 109:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 110:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 111:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 112:
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 113:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 114:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 115:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 116:
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 117:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 118:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 119:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 120:
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 121:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 122:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 123:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 124:
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 125:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 126:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 127:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 128:
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 129:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 130:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 131:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 132:
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 133:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 134:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 135:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 136:
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 137:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 138:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 139:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 140:
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 141:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 142:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 143:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 144:
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 145:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 146:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 147:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 148:
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 149:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 150:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 151:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 152:
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 153:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 154:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 155:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 156:
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 157:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 158:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 159:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 160:
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 161:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 162:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 163:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 164:
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 165:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 166:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 167:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 168:
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 169:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 170:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 171:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 172:
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 173:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 174:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 175:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 176:
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 177:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 178:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 179:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 180:
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 181:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 182:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 183:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 184:
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 185:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 186:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 187:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 188:
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 189:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 190:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 191:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 192:
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 193:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 194:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 195:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 196:
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 197:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 198:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 199:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 200:
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 201:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 202:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 203:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 204:
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 205:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 206:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 207:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 208:
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 209:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 210:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 211:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 212:
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 213:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 214:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 215:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 216:
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 217:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 218:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 219:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 220:
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 221:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 222:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 223:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 224:
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 225:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 226:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 227:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 228:
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 229:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 230:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 231:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 232:
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 233:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 234:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 235:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 236:
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 237:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 238:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 239:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 240:
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 241:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 242:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 243:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 244:
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 245:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 246:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 247:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 248:
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 249:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 250:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 251:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 252:
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 253:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 254:
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

    case 255:
      br[0] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv += 4));
      br[7] = mm_LOAD32 ((arm_word *) (rbv += 4));
      break;

  }
}

static void loadDec (arm_word *br, arm_word pat, register arm_byte *rbv)
{
  switch (pat)
  {
    case 0:
      break;

    case 1:
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 2:
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 3:
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 4:
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 5:
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 6:
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 7:
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 8:
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 9:
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 10:
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 11:
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 12:
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 13:
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 14:
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 15:
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 16:
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 17:
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 18:
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 19:
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 20:
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 21:
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 22:
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 23:
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 24:
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 25:
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 26:
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 27:
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 28:
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 29:
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 30:
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 31:
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 32:
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 33:
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 34:
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 35:
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 36:
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 37:
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 38:
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 39:
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 40:
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 41:
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 42:
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 43:
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 44:
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 45:
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 46:
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 47:
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 48:
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 49:
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 50:
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 51:
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 52:
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 53:
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 54:
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 55:
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 56:
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 57:
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 58:
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 59:
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 60:
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 61:
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 62:
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 63:
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 64:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 65:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 66:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 67:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 68:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 69:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 70:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 71:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 72:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 73:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 74:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 75:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 76:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 77:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 78:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 79:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 80:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 81:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 82:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 83:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 84:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 85:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 86:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 87:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 88:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 89:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 90:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 91:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 92:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 93:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 94:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 95:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 96:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 97:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 98:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 99:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 100:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 101:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 102:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 103:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 104:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 105:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 106:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 107:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 108:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 109:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 110:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 111:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 112:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 113:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 114:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 115:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 116:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 117:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 118:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 119:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 120:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 121:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 122:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 123:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 124:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 125:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 126:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 127:
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 128:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 129:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 130:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 131:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 132:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 133:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 134:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 135:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 136:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 137:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 138:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 139:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 140:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 141:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 142:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 143:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 144:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 145:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 146:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 147:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 148:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 149:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 150:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 151:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 152:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 153:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 154:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 155:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 156:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 157:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 158:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 159:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 160:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 161:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 162:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 163:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 164:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 165:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 166:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 167:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 168:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 169:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 170:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 171:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 172:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 173:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 174:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 175:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 176:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 177:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 178:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 179:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 180:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 181:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 182:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 183:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 184:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 185:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 186:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 187:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 188:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 189:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 190:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 191:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 192:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 193:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 194:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 195:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 196:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 197:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 198:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 199:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 200:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 201:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 202:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 203:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 204:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 205:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 206:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 207:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 208:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 209:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 210:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 211:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 212:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 213:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 214:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 215:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 216:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 217:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 218:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 219:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 220:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 221:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 222:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 223:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 224:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 225:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 226:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 227:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 228:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 229:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 230:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 231:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 232:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 233:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 234:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 235:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 236:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 237:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 238:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 239:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 240:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 241:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 242:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 243:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 244:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 245:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 246:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 247:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 248:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 249:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 250:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 251:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 252:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 253:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 254:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

    case 255:
      br[7] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[6] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[5] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[4] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[3] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[2] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[1] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      br[0] = mm_LOAD32 ((arm_word *) (rbv -= 4));
      break;

  }
}

static void storeInc (arm_word *br, arm_word pat, register arm_byte *rbv)
{
  switch (pat)
  {
    case 0:
      break;

    case 1:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      break;

    case 2:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      break;

    case 3:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      break;

    case 4:
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      break;

    case 5:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      break;

    case 6:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      break;

    case 7:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      break;

    case 8:
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      break;

    case 9:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      break;

    case 10:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      break;

    case 11:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      break;

    case 12:
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      break;

    case 13:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      break;

    case 14:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      break;

    case 15:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      break;

    case 16:
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      break;

    case 17:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      break;

    case 18:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      break;

    case 19:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      break;

    case 20:
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      break;

    case 21:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      break;

    case 22:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      break;

    case 23:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      break;

    case 24:
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      break;

    case 25:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      break;

    case 26:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      break;

    case 27:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      break;

    case 28:
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      break;

    case 29:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      break;

    case 30:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      break;

    case 31:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      break;

    case 32:
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      break;

    case 33:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      break;

    case 34:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      break;

    case 35:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      break;

    case 36:
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      break;

    case 37:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      break;

    case 38:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      break;

    case 39:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      break;

    case 40:
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      break;

    case 41:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      break;

    case 42:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      break;

    case 43:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      break;

    case 44:
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      break;

    case 45:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      break;

    case 46:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      break;

    case 47:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      break;

    case 48:
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      break;

    case 49:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      break;

    case 50:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      break;

    case 51:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      break;

    case 52:
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      break;

    case 53:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      break;

    case 54:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      break;

    case 55:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      break;

    case 56:
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      break;

    case 57:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      break;

    case 58:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      break;

    case 59:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      break;

    case 60:
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      break;

    case 61:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      break;

    case 62:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      break;

    case 63:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      break;

    case 64:
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 65:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 66:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 67:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 68:
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 69:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 70:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 71:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 72:
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 73:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 74:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 75:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 76:
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 77:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 78:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 79:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 80:
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 81:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 82:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 83:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 84:
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 85:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 86:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 87:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 88:
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 89:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 90:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 91:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 92:
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 93:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 94:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 95:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 96:
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 97:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 98:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 99:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 100:
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 101:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 102:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 103:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 104:
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 105:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 106:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 107:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 108:
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 109:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 110:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 111:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 112:
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 113:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 114:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 115:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 116:
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 117:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 118:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 119:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 120:
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 121:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 122:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 123:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 124:
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 125:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 126:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 127:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      break;

    case 128:
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 129:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 130:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 131:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 132:
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 133:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 134:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 135:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 136:
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 137:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 138:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 139:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 140:
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 141:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 142:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 143:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 144:
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 145:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 146:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 147:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 148:
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 149:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 150:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 151:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 152:
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 153:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 154:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 155:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 156:
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 157:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 158:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 159:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 160:
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 161:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 162:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 163:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 164:
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 165:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 166:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 167:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 168:
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 169:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 170:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 171:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 172:
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 173:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 174:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 175:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 176:
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 177:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 178:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 179:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 180:
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 181:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 182:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 183:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 184:
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 185:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 186:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 187:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 188:
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 189:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 190:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 191:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 192:
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 193:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 194:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 195:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 196:
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 197:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 198:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 199:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 200:
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 201:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 202:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 203:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 204:
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 205:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 206:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 207:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 208:
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 209:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 210:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 211:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 212:
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 213:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 214:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 215:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 216:
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 217:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 218:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 219:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 220:
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 221:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 222:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 223:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 224:
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 225:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 226:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 227:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 228:
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 229:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 230:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 231:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 232:
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 233:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 234:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 235:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 236:
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 237:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 238:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 239:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 240:
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 241:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 242:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 243:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 244:
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 245:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 246:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 247:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 248:
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 249:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 250:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 251:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 252:
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 253:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 254:
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

    case 255:
      mm_STORE32 ((arm_word *) (rbv += 4), br[0]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv += 4), br[7]);
      break;

  }
}

static void storeDec (arm_word *br, arm_word pat, register arm_byte *rbv)
{
  switch (pat)
  {
    case 0:
      break;

    case 1:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 2:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 3:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 4:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      break;

    case 5:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 6:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 7:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 8:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      break;

    case 9:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 10:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 11:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 12:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      break;

    case 13:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 14:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 15:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 16:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      break;

    case 17:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 18:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 19:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 20:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      break;

    case 21:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 22:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 23:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 24:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      break;

    case 25:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 26:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 27:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 28:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      break;

    case 29:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 30:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 31:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 32:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      break;

    case 33:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 34:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 35:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 36:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      break;

    case 37:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 38:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 39:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 40:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      break;

    case 41:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 42:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 43:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 44:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      break;

    case 45:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 46:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 47:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 48:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      break;

    case 49:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 50:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 51:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 52:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      break;

    case 53:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 54:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 55:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 56:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      break;

    case 57:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 58:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 59:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 60:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      break;

    case 61:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 62:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 63:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 64:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      break;

    case 65:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 66:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 67:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 68:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      break;

    case 69:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 70:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 71:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 72:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      break;

    case 73:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 74:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 75:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 76:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      break;

    case 77:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 78:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 79:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 80:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      break;

    case 81:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 82:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 83:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 84:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      break;

    case 85:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 86:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 87:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 88:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      break;

    case 89:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 90:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 91:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 92:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      break;

    case 93:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 94:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 95:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 96:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      break;

    case 97:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 98:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 99:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 100:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      break;

    case 101:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 102:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 103:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 104:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      break;

    case 105:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 106:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 107:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 108:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      break;

    case 109:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 110:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 111:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 112:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      break;

    case 113:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 114:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 115:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 116:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      break;

    case 117:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 118:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 119:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 120:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      break;

    case 121:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 122:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 123:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 124:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      break;

    case 125:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 126:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 127:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 128:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      break;

    case 129:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 130:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 131:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 132:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      break;

    case 133:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 134:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 135:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 136:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      break;

    case 137:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 138:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 139:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 140:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      break;

    case 141:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 142:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 143:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 144:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      break;

    case 145:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 146:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 147:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 148:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      break;

    case 149:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 150:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 151:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 152:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      break;

    case 153:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 154:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 155:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 156:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      break;

    case 157:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 158:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 159:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 160:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      break;

    case 161:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 162:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 163:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 164:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      break;

    case 165:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 166:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 167:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 168:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      break;

    case 169:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 170:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 171:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 172:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      break;

    case 173:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 174:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 175:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 176:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      break;

    case 177:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 178:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 179:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 180:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      break;

    case 181:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 182:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 183:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 184:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      break;

    case 185:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 186:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 187:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 188:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      break;

    case 189:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 190:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 191:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 192:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      break;

    case 193:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 194:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 195:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 196:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      break;

    case 197:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 198:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 199:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 200:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      break;

    case 201:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 202:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 203:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 204:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      break;

    case 205:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 206:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 207:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 208:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      break;

    case 209:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 210:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 211:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 212:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      break;

    case 213:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 214:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 215:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 216:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      break;

    case 217:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 218:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 219:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 220:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      break;

    case 221:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 222:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 223:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 224:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      break;

    case 225:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 226:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 227:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 228:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      break;

    case 229:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 230:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 231:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 232:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      break;

    case 233:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 234:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 235:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 236:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      break;

    case 237:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 238:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 239:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 240:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      break;

    case 241:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 242:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 243:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 244:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      break;

    case 245:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 246:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 247:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 248:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      break;

    case 249:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 250:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 251:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 252:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      break;

    case 253:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

    case 254:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      break;

    case 255:
      mm_STORE32 ((arm_word *) (rbv -= 4), br[7]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[6]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[5]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[4]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[3]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[2]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[1]);
      mm_STORE32 ((arm_word *) (rbv -= 4), br[0]);
      break;

  }
}

void arm_executeMemBlock (arm_context *c, arm_word insn)
{
  arm_word rb = (insn & arm_INSN_RN) >> arm_INSN_RN_SHIFT;
  arm_byte *rbp = mm_translate(c->r[rb]);

  if (insn & arm_INSN_BDT_L)
  {
    /* --- Loading --- */

    arm_word pc;
    if (insn & (1 << arm_PC))
      if (!(insn & arm_INSN_BDT_S)) pc = c->r[arm_PC];

    if (insn & arm_INSN_BDT_U)
    {
      /* --- Incrementing --- */

      if (!(insn & arm_INSN_BDT_P)) rbp = rbp + (- 4);

      loadInc(&c->r[0], insn & 0xff, rbp + 0);
      loadInc(&c->r[8], (insn & 0xff00) >> 8, rbp + bitsSet[insn & 0xff]);

      if (insn & arm_INSN_BDT_W)
        c->r[rb] += (bitsSet[insn & 0xff] + bitsSet[(insn >> 8) & 0xff])
                       << 2;
    }
    else
    {
      /* --- Decrementing --- */

      if (!(insn & arm_INSN_BDT_P)) rbp = rbp - (- 4);

      loadDec(&c->r[8], (insn & 0xff00) >> 8, rbp - 0);
      loadDec(&c->r[0], insn & 0xff, rbp - bitsSet[(insn & 0xff00) >> 8]);

      if (insn & arm_INSN_BDT_W)
        c->r[rb] -= (bitsSet[insn & 0xff] + bitsSet[(insn >> 8) & 0xff])
                       << 2;
    }

    if (insn & (1 << arm_PC))
      if (!(insn & arm_INSN_BDT_S))
        c->r[arm_PC] = (c->r[arm_PC] & arm_PC_ADDRESS) | 
          (pc & arm_PC_FLAGS);
  }
  else
  {
    /* --- Storing --- */

    if (insn & (1 << arm_PC))
      c->r[arm_PC] += 12;

    if (insn & arm_INSN_BDT_U)
    {
      /* --- Incrementing --- */

      if (!(insn & arm_INSN_BDT_P)) rbp = rbp + (- 4);

      storeInc(&c->r[0], insn & 0xff, rbp + 0);
      storeInc(&c->r[8], (insn & 0xff00) >> 8, rbp + bitsSet[insn & 0xff]);

      if (insn & arm_INSN_BDT_W)
        c->r[rb] += (bitsSet[insn & 0xff] + bitsSet[(insn >> 8) & 0xff])
                       << 2;
    }
    else
    {
      /* --- Decrementing --- */

      if (!(insn & arm_INSN_BDT_P)) rbp = rbp - (- 4);

      storeDec(&c->r[8], (insn & 0xff00) >> 8, rbp - 0);
      storeDec(&c->r[0], insn & 0xff, rbp - bitsSet[(insn & 0xff00) >> 8]);

      if (insn & arm_INSN_BDT_W)
        c->r[rb] -= (bitsSet[insn & 0xff] + bitsSet[(insn >> 8) & 0xff])
                       << 2;
    }

    if (insn & (1 << arm_PC))
      c->r[arm_PC] -= 12;
  }

}
