/* os/territory.c - implementation of the Territory module. */

/* See http://riscose.sf.net/ for terms of distribution, and to
 * pick up a later version of the software.
 *
 * Created by defmod, riscose version 1.01. */

#include <stdio.h>

#include "monty/monty.h"
#include "types.h"
#include "territory.h"

/* ---- territory_swi_register_extra -------------------------------- */

void territory_swi_register_extra(void)
{
    return;
}

/* ---- xterritory_number ------------------------------------------- */

os_error *xterritory_number(territory_t *territory)
{
    error("swi XTerritory_Number unimplemented.\n");

    return NULL;
}

/* ---- xterritory_register ----------------------------------------- */

os_error *xterritory_register(territory_t territory,
    byte *entries,
    void *workspace)
{
    error("swi XTerritory_Register unimplemented.\n");

    return NULL;
}

/* ---- xterritory_deregister --------------------------------------- */

os_error *xterritory_deregister(territory_t territory)
{
    error("swi XTerritory_Deregister unimplemented.\n");

    return NULL;
}

/* ---- xterritory_number_to_name ----------------------------------- */

os_error *xterritory_number_to_name(territory_t territory,
    char *name,
    int size)
{
    error("swi XTerritory_NumberToName unimplemented.\n");

    return NULL;
}

/* ---- xterritory_exists ------------------------------------------- */

os_error *xterritory_exists(territory_t territory,
    bits *psr)
{
    error("swi XTerritory_Exists unimplemented.\n");

    return NULL;
}

/* ---- xterritory_alphabet_number_to_name -------------------------- */

os_error *xterritory_alphabet_number_to_name(territory_alphabet_number alphabet,
    char *name,
    int size)
{
    error("swi XTerritory_AlphabetNumberToName unimplemented.\n");

    return NULL;
}

/* ---- xterritory_select_alphabet ---------------------------------- */

os_error *xterritory_select_alphabet(territory_t territory)
{
    error("swi XTerritory_SelectAlphabet unimplemented.\n");

    return NULL;
}

/* ---- xterritory_set_time ----------------------------------------- */

os_error *xterritory_set_time(os_date_and_time *date_and_time)
{
    error("swi XTerritory_SetTime unimplemented.\n");

    return NULL;
}

/* ---- xterritory_read_current_time_zone --------------------------- */

os_error *xterritory_read_current_time_zone(char **timezone,
    int *offset)
{
    error("swi XTerritory_ReadCurrentTimeZone unimplemented.\n");

    return NULL;
}

/* ---- xterritory_convert_time_to_utc_ordinals --------------------- */

os_error *xterritory_convert_time_to_utc_ordinals(os_date_and_time *date_and_time,
    territory_ordinals *ordinals)
{
    error("swi XTerritory_ConvertTimeToUTCOrdinals unimplemented.\n");

    return NULL;
}

/* ---- xterritory_read_time_zones ---------------------------------- */

os_error *xterritory_read_time_zones(territory_t territory,
    char **timezone,
    char **dst_timezone,
    int *offset,
    int *dst_offset)
{
    error("swi XTerritory_ReadTimeZones unimplemented.\n");

    return NULL;
}

/* ---- xterritory_convert_date_and_time ---------------------------- */

os_error *xterritory_convert_date_and_time(territory_t territory,
    os_date_and_time *date_and_time,
    char *buffer,
    int size,
    char *format,
    char **end)
{
    error("swi XTerritory_ConvertDateAndTime unimplemented.\n");

    return NULL;
}

/* ---- xterritory_convert_standard_date_and_time ------------------- */

os_error *xterritory_convert_standard_date_and_time(territory_t territory,
    os_date_and_time *date_and_time,
    char *buffer,
    int size,
    char **end)
{
    error("swi XTerritory_ConvertStandardDateAndTime unimplemented.\n");

    return NULL;
}

/* ---- xterritory_convert_standard_date ---------------------------- */

os_error *xterritory_convert_standard_date(territory_t territory,
    os_date_and_time *date,
    char *buffer,
    int size,
    char **end)
{
    error("swi XTerritory_ConvertStandardDate unimplemented.\n");

    return NULL;
}

/* ---- xterritory_convert_standard_time ---------------------------- */

os_error *xterritory_convert_standard_time(territory_t territory,
    os_date_and_time *time,
    char *buffer,
    int size,
    char **end)
{
    error("swi XTerritory_ConvertStandardTime unimplemented.\n");

    return NULL;
}

/* ---- xterritory_convert_time_to_ordinals ------------------------- */

os_error *xterritory_convert_time_to_ordinals(territory_t territory,
    os_date_and_time *date_and_time,
    territory_ordinals *ordinals)
{
    error("swi XTerritory_ConvertTimeToOrdinals unimplemented.\n");

    return NULL;
}

/* ---- xterritory_convert_time_string_to_ordinals ------------------ */

os_error *xterritory_convert_time_string_to_ordinals(territory_t territory,
    territory_time_string_flags format,
    char *t,
    territory_ordinals *ordinals)
{
    error("swi XTerritory_ConvertTimeStringToOrdinals unimplemented.\n");

    return NULL;
}

/* ---- xterritory_convert_ordinals_to_time ------------------------- */

os_error *xterritory_convert_ordinals_to_time(territory_t territory,
    os_date_and_time *buffer,
    territory_ordinals *ordinals)
{
    error("swi XTerritory_ConvertOrdinalsToTime unimplemented.\n");

    return NULL;
}

/* ---- xterritory_alphabet ----------------------------------------- */

os_error *xterritory_alphabet(territory_t territory,
    territory_alphabet_number *alphabet)
{
    error("swi XTerritory_Alphabet unimplemented.\n");

    return NULL;
}

/* ---- xterritory_alphabet_identifier ------------------------------ */

os_error *xterritory_alphabet_identifier(territory_t territory,
    char **alphabet_name)
{
    error("swi XTerritory_AlphabetIdentifier unimplemented.\n");

    return NULL;
}

/* ---- xterritory_select_keyboard_handler -------------------------- */

os_error *xterritory_select_keyboard_handler(territory_t territory)
{
    error("swi XTerritory_SelectKeyboardHandler unimplemented.\n");

    return NULL;
}

/* ---- xterritory_write_direction ---------------------------------- */

os_error *xterritory_write_direction(territory_t territory,
    territory_write_flags *flags)
{
    error("swi XTerritory_WriteDirection unimplemented.\n");

    return NULL;
}

/* ---- xterritory_character_property_table ------------------------- */

os_error *xterritory_character_property_table(territory_t territory,
    territory_character_property property,
    territory_property_table **table)
{
    error("swi XTerritory_CharacterPropertyTable unimplemented.\n");

    return NULL;
}

/* ---- xterritory_lower_case_table --------------------------------- */

os_error *xterritory_lower_case_table(territory_t territory,
    char **table)
{
    error("swi XTerritory_LowerCaseTable unimplemented.\n");

    return NULL;
}

/* ---- xterritory_upper_case_table --------------------------------- */

os_error *xterritory_upper_case_table(territory_t territory,
    char **table)
{
    error("swi XTerritory_UpperCaseTable unimplemented.\n");

    return NULL;
}

/* ---- xterritory_control_table ------------------------------------ */

os_error *xterritory_control_table(territory_t territory,
    char **table)
{
    error("swi XTerritory_ControlTable unimplemented.\n");

    return NULL;
}

/* ---- xterritory_plain_table -------------------------------------- */

os_error *xterritory_plain_table(territory_t territory,
    char **table)
{
    error("swi XTerritory_PlainTable unimplemented.\n");

    return NULL;
}

/* ---- xterritory_value_table -------------------------------------- */

os_error *xterritory_value_table(territory_t territory,
    char **table)
{
    error("swi XTerritory_ValueTable unimplemented.\n");

    return NULL;
}

/* ---- xterritory_representation_table ----------------------------- */

os_error *xterritory_representation_table(territory_t territory,
    char **table)
{
    error("swi XTerritory_RepresentationTable unimplemented.\n");

    return NULL;
}

/* ---- xterritory_collate ------------------------------------------ */

os_error *xterritory_collate(territory_t territory,
    char *s1,
    char *s2,
    territory_collate_flags flags,
    int *collate)
{
    error("swi XTerritory_Collate unimplemented.\n");

    return NULL;
}

/* ---- xterritory_read_boolean_symbols ----------------------------- */

os_error *xterritory_read_boolean_symbols(territory_t territory,
    territory_symbol_no symbol_no,
    osbool *symbol)
{
    error("swi XTerritory_ReadBooleanSymbols unimplemented.\n");

    return NULL;
}

/* ---- xterritory_read_integer_symbols ----------------------------- */

os_error *xterritory_read_integer_symbols(territory_t territory,
    territory_symbol_no symbol_no,
    int *symbol)
{
    error("swi XTerritory_ReadIntegerSymbols unimplemented.\n");

    return NULL;
}

/* ---- xterritory_read_string_symbols ------------------------------ */

os_error *xterritory_read_string_symbols(territory_t territory,
    territory_symbol_no symbol_no,
    char **symbol)
{
    error("swi XTerritory_ReadStringSymbols unimplemented.\n");

    return NULL;
}

/* ---- xterritory_read_calendar_information ------------------------ */

os_error *xterritory_read_calendar_information(territory_t territory,
    os_date_and_time *date_and_time,
    territory_calendar *calendar)
{
    error("swi XTerritory_ReadCalendarInformation unimplemented.\n");

    return NULL;
}

/* ---- xterritory_name_to_number ----------------------------------- */

os_error *xterritory_name_to_number(territory_t territory,
    char *name,
    territory_t *territory_out)
{
    error("swi XTerritory_NameToNumber unimplemented.\n");

    return NULL;
}

/* ---- xterritory_transform_string --------------------------------- */

os_error *xterritory_transform_string(territory_t territory,
    char *buffer,
    char *s,
    int size,
    int *used)
{
    error("swi XTerritory_TransformString unimplemented.\n");

    return NULL;
}

