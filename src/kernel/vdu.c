/* xvdu.c
**
** (c) Phil Blundell 2000
**
** See http://riscose.sourceforge.net/ for terms of distribution, and to
** pick up a later version of the software.
**
**   Implementation of BBC-compatible graphics
*/

#include <stdio.h>
#include <ncurses.h>

#include "vdu.h"

static BYTE vdubuf[32];

int vducmd, vduqlen = 0, vduqptr = 0, vdu_mode = -1;
int fgcol = 1, bgcol = 0;

int
vdu_getch(void)
{
  int c = getch();
  return (c == ERR) ? -1 : c;
}

void
start_curses(void)
{
  initscr();
  start_color();
  cbreak();
  noecho();
  nonl();
  intrflush(stdscr, FALSE);
  keypad(stdscr, FALSE);
}

void
initpalette(void)
{
  switch (vdu_mode)
    {
    case 0:
    case 3:
      init_pair(1, COLOR_BLACK, COLOR_BLACK);
      init_pair(2, COLOR_WHITE, COLOR_WHITE);
      init_pair(3, COLOR_BLACK, COLOR_WHITE);
      break;
    }
}

void
vdu_complete(void)
{
  unsigned int i;
  switch (vducmd)
    {
    case 17:
      if (vdubuf[0] >= 128)
	bgcol = vdubuf[0] & 15;
      else
	fgcol = vdubuf[0] & 15;
      break;
    case 22:
      fprintf(stderr, "[MODE %d]\n", vdubuf[0]);
      if (vdu_mode == -1)
	start_curses();
      vdu_mode = vdubuf[0];
      initpalette();
      break;
    case 31:
      if (vdu_mode != -1)
	move(vdubuf[1], vdubuf[0]);
      break;
    default:
      fprintf(stderr, "[vdu %d", vducmd);
      for (i = 0; i < vduqlen; i++)
	fprintf(stderr, ", %d", vdubuf[i]);
      fprintf(stderr, "]\n");
      break;
    }
  vduqlen = 0;
}

void
vdu(BYTE c)
{
  if (vduqlen)
    {
      vdubuf[vduqptr++] = c;
      if (vduqptr == vduqlen)
	vdu_complete();
      return;
    }

  vducmd = c;
  vduqptr = 0;

  switch (c)
    {
    case 0:	/* no op */
      break;
    case 1:	/* next to printer */
      break;
    case 2:	/* printer on */
      break;
    case 3:	/* printer off */
      break;
    case 4:	/* split cursors */
      break;
    case 5:	/* join cursors */
      break;
    case 6:	/* screen on */
      break;
    case 7:	/* bell */
      break;
    case 17:	/* text colour */
      vduqlen = 1;
      break;
    case 22:	/* text mode */
      vduqlen = 1;
      break;
    case 23:
      vduqlen = 9;
      break;
    case 26:	/* restore default text windows */
      fprintf(stderr, "[vdu 26]\n");
      break;
    case 27:	/* no op */
      break;
    case 28:	/* set text window */
      vduqlen = 4;
      break;
    case 29:	/* set graphics origin */
      vduqlen = 2;
      break;
    case 30:	/* home text cursor */
      break;
    case 31:	/* position text cursor */
      vduqlen = 2;
      break;
    default:
      if (vdu_mode == -1)
	putchar(c);
      else
	addch(c | COLOR_PAIR((!(fgcol & 1)) + ((bgcol & 1) << 1)));
      break;
    }
  if (vdu_mode != -1)
    refresh();
}
