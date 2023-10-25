/* os/wimpreadsysinfo.c - implementation of the WimpReadSysInfo module. */

/* See http://riscose.sf.net/ for terms of distribution, and to
 * pick up a later version of the software.
 *
 * Created by defmod, riscose version 1.01. */

#include <stdio.h>

#include "monty/monty.h"
#include "types.h"
#include "wimpreadsysinfo.h"
#include "rom/rom.h"

/* ---- wimpreadsysinfo_swi_register_extra -------------------------- */

void wimpreadsysinfo_swi_register_extra(void)
{
    return;
}

/* ---- xwimpreadsysinfo_task_count --------------------------------- */

os_error *xwimpreadsysinfo_task_count(int *task_count)
{

    return ERR_NO_SUCH_SWI();
}

/* ---- xwimpreadsysinfo_wimp_mode ---------------------------------- */

os_error *xwimpreadsysinfo_wimp_mode(os_mode *mode)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xwimpreadsysinfo_sprite_suffix ------------------------------ */

os_error *xwimpreadsysinfo_sprite_suffix(char **suffix)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xwimpreadsysinfo_desktop_state ------------------------------ */

os_error *xwimpreadsysinfo_desktop_state(wimpreadsysinfo_state *state)
{
    *state = wimpreadsysinfo_STATE_COMMANDS;
    return NULL;
}

/* ---- xwimpreadsysinfo_write_dir ---------------------------------- */

os_error *xwimpreadsysinfo_write_dir(wimpreadsysinfo_direction *write_dir)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xwimpreadsysinfo_task --------------------------------------- */

os_error *xwimpreadsysinfo_task(wimp_t *task,
    wimp_version_no *version)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xwimpreadsysinfo_version ------------------------------------ */

os_error *xwimpreadsysinfo_version(wimp_version_no *version)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xwimpreadsysinfo_font --------------------------------------- */

os_error *xwimpreadsysinfo_font(font_f *font,
    font_f *symbol_font)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xwimpreadsysinfo_tool_sprites ------------------------------- */

os_error *xwimpreadsysinfo_tool_sprites(osspriteop_area **toolsprites)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xwimpreadsysinfo_application_limit -------------------------- */

os_error *xwimpreadsysinfo_application_limit(int *size_limit)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xwimpreadsysinfo_sprite_pools ------------------------------- */

os_error *xwimpreadsysinfo_sprite_pools(osspriteop_area **low_priority,
    osspriteop_area **high_priority)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xwimpreadsysinfo_auto_scroll_pause_delay -------------------- */

os_error *xwimpreadsysinfo_auto_scroll_pause_delay(int *delay)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xwimpreadsysinfo_sprite_pools_extended ---------------------- */

os_error *xwimpreadsysinfo_sprite_pools_extended(osspriteop_area **priority,
    osspriteop_area **high_priority,
    osspriteop_area **low_priority)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xwimpreadsysinfo_special_highlighting_colours --------------- */

os_error *xwimpreadsysinfo_special_highlighting_colours(wimp_colour *bg,
    wimp_colour *fg)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xwimpreadsysinfo_text_selection ----------------------------- */

os_error *xwimpreadsysinfo_text_selection(wimp_colour *bg,
    wimp_colour *fg,
    wimpreadsysinfotextselection_flags *flags)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xwimpreadsysinfo_caret_colour ------------------------------- */

os_error *xwimpreadsysinfo_caret_colour(wimp_colour *def,
    wimp_colour *actual)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xwimpreadsysinfo_drag_settings ------------------------------ */

os_error *xwimpreadsysinfo_drag_settings(int *os_units_move,
    int *delay)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xwimpreadsysinfo_dbl_click_settings ------------------------- */

os_error *xwimpreadsysinfo_dbl_click_settings(int *os_units_move,
    int *delay)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xwimpreadsysinfo_sub_menu_open_settings --------------------- */

os_error *xwimpreadsysinfo_sub_menu_open_settings(int *delay_for_open,
    int *delay_mouse_ignore)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xwimpreadsysinfo_icon_bar_scroll_settings ------------------- */

os_error *xwimpreadsysinfo_icon_bar_scroll_settings(int *speed,
    int *acceleration)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xwimpreadsysinfo_edge_settings ------------------------------ */

os_error *xwimpreadsysinfo_edge_settings(int *delay)
{
    return ERR_NO_SUCH_SWI();
}

