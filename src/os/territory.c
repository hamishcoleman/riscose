/* os/territory.c - implementation of the Territory module. */

/* See http://riscose.sf.net/ for terms of distribution, and to
 * pick up a later version of the software.
 *
 * Created by defmod, riscose version 1.01. */

#include <stdio.h>
#include <assert.h>
#include <ctype.h>

#include "monty/monty.h"
#include "types.h"
#include "territory.h"
#include "mem.h"
#include "rom/rom.h"


/* ---- territory_swi_register_extra -------------------------------- */

void territory_swi_register_extra(void)
{
    return;
}

/* ---- xterritory_number ------------------------------------------- */

os_error *xterritory_number(territory_t *territory)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xterritory_register ----------------------------------------- */

os_error *xterritory_register(territory_t territory,
    byte *entries,
    void *workspace)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xterritory_deregister --------------------------------------- */

os_error *xterritory_deregister(territory_t territory)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xterritory_number_to_name ----------------------------------- */

os_error *xterritory_number_to_name(territory_t territory,
    char *name,
    int size)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xterritory_exists ------------------------------------------- */

os_error *xterritory_exists(territory_t territory,
    bits *psr)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xterritory_alphabet_number_to_name -------------------------- */

os_error *xterritory_alphabet_number_to_name(territory_alphabet_number alphabet,
    char *name,
    int size)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xterritory_select_alphabet ---------------------------------- */

os_error *xterritory_select_alphabet(territory_t territory)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xterritory_set_time ----------------------------------------- */

os_error *xterritory_set_time(os_date_and_time *date_and_time)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xterritory_read_current_time_zone --------------------------- */

os_error *xterritory_read_current_time_zone(char **timezone,
    int *offset)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xterritory_convert_time_to_utc_ordinals --------------------- */

os_error *xterritory_convert_time_to_utc_ordinals(os_date_and_time *date_and_time,
    territory_ordinals *ordinals)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xterritory_read_time_zones ---------------------------------- */

os_error *xterritory_read_time_zones(territory_t territory,
    char **timezone,
    char **dst_timezone,
    int *offset,
    int *dst_offset)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xterritory_convert_date_and_time ---------------------------- */

os_error *xterritory_convert_date_and_time(territory_t territory,
    os_date_and_time *date_and_time,
    char *buffer,
    int size,
    char *format,
    char **end)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xterritory_convert_standard_date_and_time ------------------- */

os_error *xterritory_convert_standard_date_and_time(territory_t territory,
    os_date_and_time *date_and_time,
    char *buffer,
    int size,
    char **end)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xterritory_convert_standard_date ---------------------------- */

os_error *xterritory_convert_standard_date(territory_t territory,
    os_date_and_time *date,
    char *buffer,
    int size,
    char **end)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xterritory_convert_standard_time ---------------------------- */

os_error *xterritory_convert_standard_time(territory_t territory,
    os_date_and_time *time,
    char *buffer,
    int size,
    char **end)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xterritory_convert_time_to_ordinals ------------------------- */

os_error *xterritory_convert_time_to_ordinals(territory_t territory,
    os_date_and_time *date_and_time,
    territory_ordinals *ordinals)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xterritory_convert_time_string_to_ordinals ------------------ */

os_error *xterritory_convert_time_string_to_ordinals(territory_t territory,
    territory_time_string_flags format,
    char *t,
    territory_ordinals *ordinals)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xterritory_convert_ordinals_to_time ------------------------- */

os_error *xterritory_convert_ordinals_to_time(territory_t territory,
    os_date_and_time *buffer,
    territory_ordinals *ordinals)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xterritory_alphabet ----------------------------------------- */

os_error *xterritory_alphabet(territory_t territory,
    territory_alphabet_number *alphabet)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xterritory_alphabet_identifier ------------------------------ */

os_error *xterritory_alphabet_identifier(territory_t territory,
    char **alphabet_name)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xterritory_select_keyboard_handler -------------------------- */

os_error *xterritory_select_keyboard_handler(territory_t territory)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xterritory_write_direction ---------------------------------- */

os_error *xterritory_write_direction(territory_t territory,
    territory_write_flags *flags)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xterritory_character_property_table ------------------------- */

os_error *xterritory_character_property_table(territory_t territory,
    territory_character_property property,
    territory_property_table **table)
{
    static territory_property_table *map[10];

    if (property < 0 || property > 10) {
        error("Bad character property\n");
        return NULL;
    }
    if (!map[property]) {
        map[property] = (territory_property_table *) mem_rma_alloc(sizeof(territory_property_table));

        for (int c=0; c<256; c++) {
            int out;
            switch (property) {
                case 0:
                    out = iscntrl(c);
                    break;
                case 1:
                    out = isupper(c);
                    break;
                case 2:
                    out = islower(c);
                    break;
                case 3:
                    out = isalpha(c);
                    break;
                case 4:
                    out = ispunct(c);
                    break;
                case 5:
                    out = isspace(c);
                    break;
                case 6:
                    out = isdigit(c);
                    break;
                case 7:
                    out = isxdigit(c);
                    break;
                case 8:
                    out = isalpha(c) && (c > 127);
                    break;
                case 9:
                    out = 1;
                    break;
                case 10:
                    out = 0;
                    break;
                default:
                    assert(1==0);
                    break;
            }
            map[property]->p[c] = out ? 0xff : 0x00;
        }
    }
    *table = map[property];

    return NULL;
}

/* ---- xterritory_lower_case_table --------------------------------- */

os_error *xterritory_lower_case_table(territory_t territory,
    char **table)
{
    static char *lower_case_table = NULL;
    if (!lower_case_table) {
        lower_case_table = mem_rma_alloc(sizeof(char)*256);
        for (int c=0; c<256; c++) {
            lower_case_table[c] = tolower(c);
        }
    }
    *table = lower_case_table;

    return NULL;
}

/* ---- xterritory_upper_case_table --------------------------------- */


os_error *xterritory_upper_case_table(territory_t territory,
    char **table)
{
    static char *upper_case_table = NULL;
    if (!upper_case_table) {
        upper_case_table = mem_rma_alloc(sizeof(char)*256);
        for (int c=0; c<256; c++) {
            upper_case_table[c] = toupper(c);
        }
    }
    *table = upper_case_table;

    return NULL;
}

/* ---- xterritory_control_table ------------------------------------ */

os_error *xterritory_control_table(territory_t territory,
    char **table)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xterritory_plain_table -------------------------------------- */

os_error *xterritory_plain_table(territory_t territory,
    char **table)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xterritory_value_table -------------------------------------- */

os_error *xterritory_value_table(territory_t territory,
    char **table)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xterritory_representation_table ----------------------------- */

os_error *xterritory_representation_table(territory_t territory,
    char **table)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xterritory_collate ------------------------------------------ */

os_error *xterritory_collate(territory_t territory,
    char *s1,
    char *s2,
    territory_collate_flags flags,
    int *collate)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xterritory_read_boolean_symbols ----------------------------- */

os_error *xterritory_read_boolean_symbols(territory_t territory,
    territory_symbol_no symbol_no,
    osbool *symbol)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xterritory_read_integer_symbols ----------------------------- */

os_error *xterritory_read_integer_symbols(territory_t territory,
    territory_symbol_no symbol_no,
    int *symbol)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xterritory_read_string_symbols ------------------------------ */

os_error *xterritory_read_string_symbols(territory_t territory,
    territory_symbol_no symbol_no,
    char **symbol)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xterritory_read_calendar_information ------------------------ */

os_error *xterritory_read_calendar_information(territory_t territory,
    os_date_and_time *date_and_time,
    territory_calendar *calendar)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xterritory_name_to_number ----------------------------------- */

os_error *xterritory_name_to_number(territory_t territory,
    char *name,
    territory_t *territory_out)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xterritory_transform_string --------------------------------- */

os_error *xterritory_transform_string(territory_t territory,
    char *buffer,
    char *s,
    int size,
    int *used)
{
    return ERR_NO_SUCH_SWI();
}

