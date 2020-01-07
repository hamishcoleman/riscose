/*defmod.y---grammar for a .swi file*/

/*OSLib---efficient, type-safe, transparent, extensible,\n"
   register-safe A P I coverage of RISC O S*/
/*Copyright � 1994 Jonathan Coxhead*/

/*
      OSLib is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 1, or (at your option)
   any later version.

      OSLib is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

      You should have received a copy of the GNU General Public License
   along with this programme; if not, write to the Free Software
   Foundation, Inc, 675 Mass Ave, Cambridge, MA 02139, U S A.
*/

/* Revision History:
JRC            Original Release
TV    19990418    Modification originally devised by Tom Hughes:
                  Allows Constants to be declared
TV    20000503    |bool| replaced by |osbool|
*/

%{ /*C declarations*/

   /*From CLib*/
   #include <ctype.h>
   #include <stddef.h>
   #include <stdlib.h>
#include <stdio.h>
   #include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>

#include "monty/monty.h"
#include "monty/mem.h"
#include "monty/file.h"
#include "monty/hexdump.h"

   /*From OSLib*/
   #include "types.h"

   /*From Support*/
   #include "lookup.h"

   /*Local*/
   #include "def.h"
   #include "riscoseapi.h"
   #include "riscosetpl.h"
   #include "riscosehdr.h"
   #include "riscoseerr.h"

#define tracef(a)

/* The larger of two values. */
#define MAX(a, b) ((a) > (b) ? (a) : (b))

/* The hexadecimal digit corresponding to a character. */
#define XDIGIT(c) ((byte) (c) < 'A'? (c) - '0': \
      (byte) (c) < 'a'? (c) - 'A' + 10: (c) - 'a' + 10)

#define Module_MajorVersion_CMHG        6.22
#define Module_MinorVersion_CMHG        
#define Module_Date_CMHG                07 Jun 2001

#define Module_MajorVersion             "6.22"
#define Module_Version                  622
#define Module_MinorVersion             ""
#define Module_Date                     "07 Jun 2001"

#define Module_ApplicationDate2         "07-Jun-01"
#define Module_ApplicationDate4         "07-Jun-2001"

#define Module_ComponentName            "defmod"
#define Module_ComponentPath            "RiscOS/Tools/Sources/GNU/defmod"

#define Module_FullVersion              "6.22"
#define Module_HelpVersion              "6.22 (07 Jun 2001)"

#define VERSION_INFO Module_FullVersion " [" Module_Date "]"

int yydebug;

   static lookup_t needses, needsatends, consts, types, swis;
   static char Title [256],
      Author [256] = "Anon",
      Description [def_DESCRIPTION_LIMIT + 1] = "";

   static osbool Verbose = FALSE, Parse_Error = FALSE;

   int Quiet = 0;

   static int yylex (void);

   static void yyerror (char *);

   static int yyparse(void);    /* TV 990128 */


   static int Line_No = 1;

   static struct def_s Union (struct def_s, struct def_s), Empty;

   #define DISJUNCTION(p1, p2) (p1 != 0 ? p1: p2)
%}

/*Bison declarations*/
%start file

%union
{  /*type for a single character*/
   char c;

   /*token types*/
   int num;
   char id [def_ID_LIMIT + 1];
   int reg;
   osbool option;
   char description [def_DESCRIPTION_LIMIT + 1];

   /*definition-in-progress types*/
   struct def_t type_defn;
   struct def_s swi_defn;
}

%type <swi_defn> swi number_part condition_part_OPTION condition_part
      entry_part exit_part entry_condition_LIST entry_condition
      exit_condition_LIST exit_condition absent_part description
      description_OPTION

%type <type_defn> base_type base_type_OPTION type toid typed_var_LIST
      typed_var toided_var_LIST toided_var

%type <description> wordchar_SEQUENCE word word_SEQUENCE word_SEQUENCE_OPTION
      DESCRIPTION DESCRIPTION_OPTION

%type <option> ellipsis_OPTION pling_OPTION

%type <num> NUM num dec_num hex_num bin_num char_num digit_SEQUENCE
      hexit_SEQUENCE bit_SEQUENCE numchars const

%type <id> ID id_cont_SEQUENCE id_cont_SEQUENCE_OPTION

%type <reg> REG

%type <c> a b c d e f g h i j k l m n o p q r s t u v w x y z
      letter bit digit hexit wordchar numchar simplechar compoundchar
      escapedchar id_start id_cont spacechar

%%
/*Grammar rules*/

/*Metarules (written in yet another form of B N F!):

   EMPTY:             ;
   <notion>_OPTION:   <notion> | EMPTY;
   <notion>_SEQUENCE: <notion> | <notion>_SEQUENCE <notion>;
   <notion>_LIST:     <notion> | <notion>_LIST     COMMA          <notion>;
   <notion>_SERIES:   <notion> | <notion>_SERIES   SEMICOLON      <notion>;

   <token>: token ws;
*/

file: ws decl_SERIES_OPTION;

decl: title_decl | author_decl | needs_decl | needsatend_decl
      | const_decl | type_decl | swi_decl;

title_decl: TITLE ID DESCRIPTION_OPTION
      {strcpy (Title, $2); strcpy (Description, $3);};

author_decl: AUTHOR DESCRIPTION {strcpy (Author, $2);};

needs_decl: NEEDS needs_LIST {};

needs: ID { lookup_insert(needses, $1, NULL); };

needsatend_decl: NEEDSATEND needsatend_LIST {};

needsatend: ID { lookup_insert(needsatends, $1, NULL); };

const_decl: CONST const_defn_LIST;

const_defn:
   ID EQUALS type COLON const DESCRIPTION_OPTION
   {
      def_c c = emalloc(sizeof *c);
      def_t t = emalloc(def_sizeof_TYPE ($3.tag));

      memcpy (t, &$3, def_sizeof_TYPE ($3.tag));
      c->type  = t;
      c->value = $5;
      c->description = *$6 ? estrdup($6) : NULL;

      lookup_insert(consts, $1, c);
   };

const:
   NUM |
   ID
   {
      def_c c;

      if (!lookup(consts, $1, (void **)&c)) {
         yyerror("duplicate");
         YYERROR;
      }

      if (c == NULL)
      {  char errmess [274];

         sprintf (errmess, "const \"%s\" undefined", $1);
         yyerror (errmess);
         YYERROR;
      }

      $$ = c->value;
   };

type_decl: TYPE type_defn_LIST;

type_defn:
   ID DESCRIPTION_OPTION
   {
      def_t t = emalloc(def_sizeof_TYPE (def_TYPE_ABSTRACT));

      t->tag = def_TYPE_ABSTRACT;
      t->name = NULL;
      t->value = def_VALUE_REGISTER;
      t->description = *$2 ? estrdup($2) : NULL;

      lookup_insert(types, $1, t);
   } |
   ID EQUALS type DESCRIPTION_OPTION
   {
      def_t t = emalloc(def_sizeof_TYPE ($3.tag));

      memcpy (t, &$3, def_sizeof_TYPE ($3.tag));
      t->description = *$4 ? estrdup($4) : NULL;

      lookup_insert(types, $1, t);
   };

base_type:
   COLON ID
   {  /*Use it verbatim, but check the type does exist (and find out if
         it's a struct type or not).*/
      int found;
      def_t t;

      found = lookup(types, $2, (void **)&t);

      $$.tag = def_TYPE_ID;
      $$.name = NULL;
      if (!found)
      {  /*have to make an optimistic assumption*/
         $$.value = def_VALUE_VARIABLE;
         if (Verbose && !Quiet)
            fprintf (stderr, "warning: type \"%s\" undefined at line "
                  "%d\n", $2, Line_No);
      }
      else
         $$.value = t != NULL? t->value: def_VALUE_VARIABLE;
      $$.description = NULL;
      strcpy ($$.data AS id, $2);
   };

type:
   INT
   {$$.tag = def_TYPE_INT;   $$.name = NULL;
         $$.value = def_VALUE_REGISTER; $$.description = NULL;} |
   SHORT
   {$$.tag = def_TYPE_SHORT; $$.name = NULL;
         $$.value = def_VALUE_REGISTER; $$.description = NULL;} |
   BYTE
   {$$.tag = def_TYPE_BYTE;  $$.name = NULL;
         $$.value = def_VALUE_REGISTER; $$.description = NULL;} |
   CHAR
   {$$.tag = def_TYPE_CHAR;  $$.name = NULL;
         $$.value = def_VALUE_REGISTER; $$.description = NULL;} |
   BITS
   {$$.tag = def_TYPE_BITS;  $$.name = NULL;
         $$.value = def_VALUE_REGISTER; $$.description = NULL;} |
   BYTES
   {$$.tag = def_TYPE_BYTES; $$.name = NULL;
         $$.value = def_VALUE_REGISTER; $$.description = NULL;} |
   BOOL
   {$$.tag = def_TYPE_BOOL;  $$.name = NULL;
         $$.value = def_VALUE_REGISTER; $$.description = NULL;} |
   REF type
   {  def_t t = emalloc(def_sizeof_TYPE ($2.tag));

      memcpy (t, &$2, def_sizeof_TYPE ($2.tag));
      $$.tag = def_TYPE_REF;
      $$.name = NULL;
      $$.value = def_VALUE_REGISTER;
      $$.description = NULL;
      $$.data AS ref = t;
   } |
   STRING
   {$$.tag = def_TYPE_STRING; $$.name = NULL;
         $$.value = def_VALUE_VARIABLE; $$.description = NULL;} |
   ASM
   {$$.tag = def_TYPE_ASM; $$.name = NULL; $$.value = def_VALUE_VARIABLE;
         $$.description = NULL;} |
   DATA
   {$$.tag = def_TYPE_DATA; $$.name = NULL; $$.value = def_VALUE_VARIABLE;
         $$.description = NULL;} |
   STRUCT base_type_OPTION OPEN typed_var_LIST ellipsis_OPTION CLOSE
   {  $$ = $4;
      $$.tag = def_TYPE_STRUCT;
      if ($5)
         $$.value = def_VALUE_VARIABLE;
      else if ($4.value == def_VALUE_REGISTER)
         $$.value = def_VALUE_FIXED;
      $$.description = NULL;
      if ($2.tag != def_TYPE_VOID)
      {  $$.data AS list.base = emalloc(def_sizeof_TYPE ($2.tag));
         memcpy ($$.data AS list.base, &$2, def_sizeof_TYPE ($2.tag));
      }
      else
         $$.data AS list.base = NULL;
      $$.data AS list.ellipsis = $5;
   } |
   UNION OPEN toided_var_LIST CLOSE
   {  $$ = $3;
      $$.tag = def_TYPE_UNION;
      $$.description = NULL;
   } |
   SUB const BUS type
   {  def_t t = emalloc(def_sizeof_TYPE ($4.tag));

      if ($4.value == def_VALUE_VARIABLE)
      {  yyerror ("type cannot occur in an array");
         YYERROR;
      }

      memcpy (t, &$4, def_sizeof_TYPE ($4.tag));
      $$.tag = def_TYPE_ROW;
      $$.name = NULL;
      $$.value = def_VALUE_FIXED; /*there are no arrays of undefined size*/
      $$.description = NULL;
      $$.data AS row.count = $2;
      $$.data AS row.base = t;
   } |
   ID
   {  /*Use it verbatim, but check the type does exist (and find out if
         it's a register type or not).*/
      int found;
      def_t t;

      found = lookup(types, $1, (void **)&t);

      $$.tag = def_TYPE_ID;
      $$.name = NULL;
      if (!found)
      {  /*have to make an optimistic assumption*/
         $$.value = def_VALUE_REGISTER;
         if (Verbose && !Quiet)
            fprintf (stderr, "warning: type \"%s\" undefined at line "
                  "%d\n", $1, Line_No);
      }
      else
         $$.value = t != NULL? t->value: def_VALUE_REGISTER;
      $$.description = NULL;
      strcpy ($$.data AS id, $1);
   };

toid: type | VOID {$$.tag = def_TYPE_VOID; $$.name = NULL;
      $$.description = NULL;};

typed_var: type COLON ID DESCRIPTION_OPTION
   {  $$ = $1;
      $$.name = estrdup($3);
      $$.description = *$4 ? estrdup($4) : NULL;
   };

toided_var: toid COLON ID DESCRIPTION_OPTION
   {  $$ = $1;
      $$.name = estrdup($3);
      $$.description = *$4 ? estrdup($4) : NULL;
   };

swi_decl: SWI swi_defn_LIST;

swi_defn:
   ID EQUALS swi
   {
      def_s s = emalloc(sizeof *s);

      *s = $3;

      lookup_insert(swis, $1, s);
   };

swi: OPEN number_part condition_part_OPTION CLOSE {$$ = Union ($2, $3);};

number_part: NUMBER NUM description_OPTION
      {$$ = $3; $$.swi = $2;};

condition_part: entry_part | entry_part exit_part {$$ = Union ($1, $2);} |
      entry_part absent_part {$$ = $1; $$.absent = TRUE;} |
      exit_part | absent_part;

entry_part: COMMA ENTRY OPEN entry_condition_LIST CLOSE {$$ = $4;};

exit_part: COMMA EXIT OPEN exit_condition_LIST CLOSE {$$ = $4;};

absent_part: COMMA ABSENT {$$ = Empty; $$.absent = TRUE;};

entry_condition:
   REG CONTAINS typed_var
      {  if ($3.value != def_VALUE_REGISTER)
         {  yyerror ("type cannot occur in a register");
            YYERROR;
         }
         $$ = Empty;
         $$.i |= 1 << $1;
         $$.inputs [$1] = emalloc(def_sizeof_TYPE ($3.tag));
         memcpy ($$.inputs [$1], &$3, def_sizeof_TYPE ($3.tag));
      } |
   REG REFERENCES typed_var
      {  $$ = Empty;
         $$.i |= 1 << $1;
         $$.ri |= 1 << $1;
         $$.inputs [$1] = emalloc(def_sizeof_TYPE ($3.tag));
         memcpy ($$.inputs [$1], &$3, def_sizeof_TYPE ($3.tag));
      } |
/*   REG CONSTANT NUM description_OPTION  */    /* TV 990418 */
   REG CONSTANT const description_OPTION
      {  $$ = $4;
         if ($$.starred_swi)
            /*Move the flag to the right place.*/
            $$.starred_constants [$1] = TRUE, $$.starred_swi = FALSE;
         $$.k |= 1 << $1;
         $$.constants [$1] = $3;
      } |
   REG DISJOINS typed_var
      {  if ($3.value != def_VALUE_REGISTER)
         {  yyerror ("type cannot occur in a register");
            YYERROR;
         }
         $$ = Empty;
         $$.i |= 1 << $1;
         $$.inputs [$1] = emalloc(def_sizeof_TYPE ($3.tag));
         memcpy ($$.inputs [$1], &$3, def_sizeof_TYPE ($3.tag));
         $$.op [$1] = def_OP_DISJOINS;
      } |
   REG CONJOINS typed_var
      {  yyerror ("& not supported");
         YYERROR;
      } |
   REG ADDS typed_var
      {  if ($3.value != def_VALUE_REGISTER)
         {  yyerror ("type cannot occur in a register");
            YYERROR;
         }
         $$ = Empty;
         $$.i |= 1 << $1;
         $$.inputs [$1] = emalloc(def_sizeof_TYPE ($3.tag));
         memcpy ($$.inputs [$1], &$3, def_sizeof_TYPE ($3.tag));
         $$.op [$1] = def_OP_ADDS;
      } |
   REG EXCLUSIVELY_DISJOINS typed_var
      {  if ($3.value != def_VALUE_REGISTER)
         {  yyerror ("type cannot occur in a register");
            YYERROR;
         }
         $$ = Empty;
         $$.i |= 1 << $1;
         $$.inputs [$1] = emalloc(def_sizeof_TYPE ($3.tag));
         memcpy ($$.inputs [$1], &$3, def_sizeof_TYPE ($3.tag));
         $$.op [$1] = def_OP_EXCLUSIVELY_DISJOINS;
      } |
   FLAGS
      {  $$ = Empty;
         $$.f_in = 1;
      };

exit_condition:
   REG pling_OPTION CONTAINS typed_var
      {  if ($4.value != def_VALUE_REGISTER)
         {  yyerror ("type cannot occur in a register");
            YYERROR;
         }
         $$ = Empty;
         if ($2) $$.value = 1 << $1;
         $$.o |= 1 << $1;
         $$.outputs [$1] = emalloc(def_sizeof_TYPE ($4.tag));
         memcpy ($$.outputs [$1], &$4, def_sizeof_TYPE ($4.tag));
      } |
   REG pling_OPTION REFERENCES typed_var
      {  $$ = Empty;
         if ($2) $$.value = 1 << $1;
         $$.o |= 1 << $1;
         $$.ro |= 1 << $1;
         $$.outputs [$1] = emalloc(def_sizeof_TYPE ($4.tag));
         memcpy ($$.outputs [$1], &$4, def_sizeof_TYPE ($4.tag));
      } |
   REG CORRUPTED /*a corrupted register can't be returned*/
      {$$ = Empty; $$.c |= 1 << $1;} |
   REG PLING
      {$$ = Empty; $$.value = 1 << $1;} |
   FLAGS pling_OPTION
      {  $$ = Empty;
         if ($2) $$.value = 1 << def_FLAGS;
         $$.f_out = 1;
      };

description:
   DESCRIPTION
      {  tracef ("DESCRIPTION \"%s\"\n" _ $1);
         $$ = Empty;
         $$.description = *$1 ? estrdup($1) : NULL;
         $$.starred_swi = TRUE;
      } |
   STAR
      {  $$ = Empty;
         $$.description = NULL;
         $$.starred_swi = TRUE;
      };

/*------------------------------------------------------------------------*/
/*Lexical rules*/

ws: ws_item_SEQUENCE_OPTION {};
nl: '\n' {Line_No++;} | '\r' {Line_No++;};
space: spacechar {} | nl {};
ws_item: space {} | comment {};
comment: '/' '/' commentchar_SEQUENCE_OPTION nl {};

TITLE:  t i t l e   ws {tracef ("TITLE\n");};
AUTHOR: a u t h o r ws {tracef ("AUTHOR\n");};
NEEDS:  n e e d s   ws {tracef ("NEEDS\n");};
NEEDSATEND:  n e e d s a t e n d  ws {tracef ("NEEDSATEND\n");};
CONST:  c o n s t   ws {tracef ("CONST\n");};
TYPE:   t y p e     ws {tracef ("TYPE\n");};
SWI:    s w i       ws {tracef ("SWI\n");};

NUMBER: n u m b e r ws {tracef ("NUMBER\n");};
ENTRY:  e n t r y   ws {tracef ("ENTRY\n");};
EXIT:   e x i t     ws {tracef ("EXIT\n");};
ABSENT: a b s e n t ws {tracef ("ABSENT\n");};

FLAGS:  f l a g s   ws {tracef ("FLAGS\n");};

CONTAINS:             '='     ws {};
REFERENCES:           '-' '>' ws {};
CONSTANT:             '#'     ws {};
DISJOINS:             '|'     ws {};
CONJOINS:             '&'     ws {};
ADDS:                 '+'     ws {};
EXCLUSIVELY_DISJOINS: '^'     ws {};
CORRUPTED:            '?'     ws {};
STAR:                 '*'     ws {};
PLING:                '!'     ws {};

INT:   '.' i n t        ws {};
SHORT: '.' s h o r t    ws {};
BYTE:  '.' b y t e      ws {};
CHAR:  '.' c h a r      ws {};
BITS:  '.' b i t s      ws {};
BYTES: '.' b y t e s    ws {};
BOOL:  '.' b o o l      ws {};
VOID:  '.' v o i d      ws {};

STRING: '.' s t r i n g ws {};
ASM:    '.' a s m       ws {};
DATA:   '.' d a t a     ws {};

STRUCT:   '.' s t r u c t ws {};
UNION:    '.' u n i o n   ws {};
REF:      '.' r e f       ws {};
ELLIPSIS: '.' '.' '.'     ws {};

SEMICOLON: ';' ws {};
COMMA:     ',' ws {};
EQUALS:    '=' ws {};
OPEN:      '(' ws {};
CLOSE:     ')' ws {};
COLON:     ':' ws {};
SUB:       '[' ws {};
BUS:       ']' ws {};

ID: id_start id_cont_SEQUENCE_OPTION ws
      {$$ [0] = $1; strcpy (&$$ [1], $2);};

NUM: num ws;

REG: r digit ws {$$ = $2 - '0';};

DESCRIPTION: '"' wordchar_SEQUENCE word_SEQUENCE_OPTION '"' ws
      {sprintf($$, "%s %s", $2, $3);};
word: space_SEQUENCE wordchar_SEQUENCE {strcpy ($$, $2);};
wordchar: simplechar | '\\' {$$ = '\\';}| '\'' {$$ = '\'';};

a: 'a' {$$ = 'a';} |
   'A' {$$ = 'A';} |
   /*�*/ '\xC2' {$$ = 0xC2;} |
   /*�*/ '\xE2' {$$ = 0xE2;} |
   /*�*/ '\xC4' {$$ = 0xC4;} |
   /*�*/ '\xE4' {$$ = 0xE4;} |
   /*�*/ '\xC1' {$$ = 0xC1;} |
   /*�*/ '\xE1' {$$ = 0xE1;} |
   /*�*/ '\xC0' {$$ = 0xC0;} |
   /*�*/ '\xE0' {$$ = 0xE0;} |
   /*�*/ '\xC3' {$$ = 0xC3;} |
   /*�*/ '\xE3' {$$ = 0xE3;} |
   /*�*/ '\xC5' {$$ = 0xC5;} |
   /*�*/ '\xE5' {$$ = 0xE5;} |
   /*�*/ '\xC6' {$$ = 0xC6;} |
   /*�*/ '\xE6' {$$ = 0xE6;};
b: 'b' {$$ = 'b';} |
   'B' {$$ = 'B';};
c: 'c' {$$ = 'c';} |
   'C' {$$ = 'C';} |
   /*�*/ '\xC7' {$$ = 0xC7;} |
   /*�*/ '\xE7' {$$ = 0xE7;};
d: 'd' {$$ = 'd';} |
   'D' {$$ = 'D';} |
   /*�*/ '\xD0' {$$ = 0xD0;} |
   /*�*/ '\xF0' {$$ = 0xF0;};
e: 'e' {$$ = 'e';} | 'E' {$$ = 'E';} |
   /*�*/ '\xCA' {$$ = 0xCA;} |
   /*�*/ '\xEA' {$$ = 0xEA;} |
   /*�*/ '\xCB' {$$ = 0xCB;} |
   /*�*/ '\xEB' {$$ = 0xEB;} |
   /*�*/ '\xC9' {$$ = 0xC9;} |
   /*�*/ '\xE9' {$$ = 0xE9;} |
   /*�*/ '\xC8' {$$ = 0xC8;} |
   /*�*/ '\xE8' {$$ = 0xE8;};
f: 'f' {$$ = 'f';} | 'F' {$$ = 'F';} |
   /*�*/ '\x9E' {$$ = 0x9E;} |
   /*�*/ '\x9F' {$$ = 0x9F;};
g: 'g' {$$ = 'g';} | 'G' {$$ = 'G';};
h: 'h' {$$ = 'h';} | 'H' {$$ = 'H';};
i: 'i' {$$ = 'i';} | 'I' {$$ = 'I';} |
   /*�*/ '\xCE' {$$ = 0xCE;} |
   /*�*/ '\xEE' {$$ = 0xEE;} |
   /*�*/ '\xCF' {$$ = 0xCF;} |
   /*�*/ '\xEF' {$$ = 0xEF;} |
   /*�*/ '\xCD' {$$ = 0xCD;} |
   /*�*/ '\xED' {$$ = 0xED;} |
   /*�*/ '\xCC' {$$ = 0xCC;} |
   /*�*/ '\xEC' {$$ = 0xEC;};
j: 'j' {$$ = 'j';} | 'J' {$$ = 'J';};
k: 'k' {$$ = 'k';} | 'K' {$$ = 'K';};
l: 'l' {$$ = 'l';} | 'L' {$$ = 'L';};
m: 'm' {$$ = 'm';} | 'M' {$$ = 'M';};
n: 'n' {$$ = 'n';} | 'N' {$$ = 'N';} |
   /*�*/ '\xD1' {$$ = 0xD1;} |
   /*�*/ '\xF1' {$$ = 0xF1;};
o: 'o' {$$ = 'o';} | 'O' {$$ = 'O';} |
   /*�*/ '\xD4' {$$ = 0xD4;} |
   /*�*/ '\xF4' {$$ = 0xF4;} |
   /*�*/ '\xD6' {$$ = 0xD6;} |
   /*�*/ '\xF6' {$$ = 0xF6;} |
   /*�*/ '\xD3' {$$ = 0xD3;} |
   /*�*/ '\xF3' {$$ = 0xF3;} |
   /*�*/ '\xD2' {$$ = 0xD2;} |
   /*�*/ '\xF2' {$$ = 0xF2;} |
   /*�*/ '\xD5' {$$ = 0xD5;} |
   /*�*/ '\xF5' {$$ = 0xF5;} |
   /*�*/ '\x9A' {$$ = 0x9A;} |
   /*�*/ '\x9B' {$$ = 0x9B;} |
   /*�*/ '\xD8' {$$ = 0xD8;} |
   /*�*/ '\xF8' {$$ = 0xF8;};
p: 'p' {$$ = 'p';} | 'P' {$$ = 'P';};
q: 'q' {$$ = 'q';} | 'Q' {$$ = 'Q';};
r: 'r' {$$ = 'r';} | 'R' {$$ = 'R';};
s: 's' {$$ = 's';} | 'S' {$$ = 'S';} |
   /*�*/ '\xDF' {$$ = 0xDF;};
t: 't' {$$ = 't';} | 'T' {$$ = 'T';} |
   /*�*/ '\xDE' {$$ = 0xDE;} |
   /*�*/ '\xFE' {$$ = 0xFE;};
u: 'u' {$$ = 'u';} | 'U' {$$ = 'U';} |
   /*�*/ '\xDB' {$$ = 0xDB;} |
   /*�*/ '\xFB' {$$ = 0xFB;} |
   /*�*/ '\xDC' {$$ = 0xDC;} |
   /*�*/ '\xFC' {$$ = 0xFC;} |
   /*�*/ '\xDA' {$$ = 0xDA;} |
   /*�*/ '\xFA' {$$ = 0xFA;} |
   /*�*/ '\xD9' {$$ = 0xD9;} |
   /*�*/ '\xF9' {$$ = 0xF9;};
v: 'v' {$$ = 'v';} | 'V' {$$ = 'V';};
w: 'w' {$$ = 'w';} | 'W' {$$ = 'W';} |
   /*�*/ '\x81' {$$ = 0x81;} |
   /*�*/ '\x82' {$$ = 0x82;};
x: 'x' {$$ = 'x';} | 'X' {$$ = 'X';};
y: 'y' {$$ = 'y';} | 'Y' {$$ = 'Y';} |
   /*�*/ '\x85' {$$ = 0x85;} |
   /*�*/ '\x86' {$$ = 0x86;} |
   /*�*/ '\xFF' {$$ = 0xFF;} |
   /*�*/ '\xDD' {$$ = 0xDD;} |
   /*�*/ '\xFD' {$$ = 0xFD;};
z: 'z' {$$ = 'z';} | 'Z' {$$ = 'Z';};

letter: a | b | c | d | e | f | g | h | i | j | k | l | m | n | o | p | q |
      r | s | t | u | v | w | x | y | z;
bit: '0' {$$ = '0';}| '1' {$$ = '1';};
digit: bit | '2' {$$ = '2';} | '3' {$$ = '3';} |
      '4' {$$ = '4';} | '5' {$$ = '5';} | '6' {$$ = '6';} |
      '7' {$$ = '7';} | '8' {$$ = '8';} | '9' {$$ = '9';};
hexit: digit | a | b | c | d | e | f;

id_start: letter;

id_cont: letter | digit | '_' {$$ = '_';};

num: dec_num | hex_num | bin_num | char_num;

dec_num: digit_SEQUENCE {$$ = $1;} | '-' digit_SEQUENCE {$$ = -$2;};

hex_num: '0' x hexit_SEQUENCE {$$ = $3;} | '&' hexit_SEQUENCE {$$ = $2;};

bin_num: '0' b bit_SEQUENCE {$$ = $3;} | '%' bit_SEQUENCE {$$ = $2;};

char_num: '\'' numchars '\'' {$$ = $2;};

numchars: numchar {$$ = (int) $1;} |
      numchar numchar {$$ = $1 | $2 << 8;} |
      numchar numchar numchar {$$ = $1 | $2 << 8 | $3 << 16;} |
      numchar numchar numchar numchar
            {$$ = $1 | $2 << 8 | $3 << 16 | $4 << 24;};

numchar: simplechar | compoundchar | spacechar | '"' {$$ = '"';};
commentchar: simplechar {} | spacechar {} | '\\' {} | '"' {} | '\'' {};
spacechar: ' ' {$$ = ' ';} | '\xA0' {$$ = 0xA0;} | '\t' {$$ = '\t';};
simplechar: letter | digit |
      '!' {$$ = '!';} | '#' {$$ = '#';} | '$' {$$ = '$';} |
      '%' {$$ = '%';} | '&' {$$ = '&';} | '(' {$$ = '(';} |
      ')' {$$ = ')';} | '*' {$$ = '*';} | '+' {$$ = '+';} |
      ',' {$$ = ',';} | '-' {$$ = '-';} | '.' {$$ = '.';} |
      '/' {$$ = '/';} | ':' {$$ = ':';} | ';' {$$ = ';';} |
      '<' {$$ = '<';} | '=' {$$ = '=';} | '>' {$$ = '>';} |
      '?' {$$ = '?';} | '@' {$$ = '@';} | '[' {$$ = '[';} |
      ']' {$$ = ']';} | '^' {$$ = '^';} | '_' {$$ = '_';} |
      '`' {$$ = '`';} | '{' {$$ = '{';} | '|' {$$ = '|';} |
      '}' {$$ = '}';} | '~' {$$ = '~';} |
   /*�*/ '\x8C' {$$ = 0x8C;} |
   /*�*/ '\x8D' {$$ = 0x8D;} |
   /*�*/ '\x8E' {$$ = 0x8E;} |
   /*�*/ '\x8F' {$$ = 0x8F;} |
   /*�*/ '\x90' {$$ = 0x90;} |
   /*�*/ '\x91' {$$ = 0x91;} |
   /*�*/ '\x92' {$$ = 0x92;} |
   /*�*/ '\x93' {$$ = 0x93;} |
   /*�*/ '\x94' {$$ = 0x94;} |
   /*�*/ '\x95' {$$ = 0x95;} |
   /*�*/ '\x96' {$$ = 0x96;} |
   /*�*/ '\x97' {$$ = 0x97;} |
   /*�*/ '\x98' {$$ = 0x98;} |
   /*�*/ '\x99' {$$ = 0x99;} |
   /*�*/ '\x9C' {$$ = 0x9C;} |
   /*�*/ '\x9D' {$$ = 0x9D;} |
   /*�*/ '\xA1' {$$ = 0xA1;} |
   /*�*/ '\xA2' {$$ = 0xA2;} |
   /*�*/ '\xA3' {$$ = 0xA3;} |
   /*�*/ '\xA4' {$$ = 0xA4;} |
   /*�*/ '\xA5' {$$ = 0xA5;} |
   /*�*/ '\xA6' {$$ = 0xA6;} |
   /*�*/ '\xA7' {$$ = 0xA7;} |
   /*�*/ '\xA8' {$$ = 0xA8;} |
   /*�*/ '\xA9' {$$ = 0xA9;} |
   /*�*/ '\xAA' {$$ = 0xAA;} |
   /*�*/ '\xAB' {$$ = 0xAB;} |
   /*�*/ '\xAC' {$$ = 0xAC;} |
   /*�*/ '\xAD' {$$ = 0xAD;} |
   /*�*/ '\xAE' {$$ = 0xAE;} |
   /*�*/ '\xAF' {$$ = 0xAF;} |
   /*�*/ '\xB0' {$$ = 0xB0;} |
   /*�*/ '\xB1' {$$ = 0xB1;} |
   /*�*/ '\xB9' {$$ = 0xB9;} |
   /*�*/ '\xB2' {$$ = 0xB2;} |
   /*�*/ '\xB3' {$$ = 0xB3;} |
   /*�*/ '\xB4' {$$ = 0xB4;} |
   /*�*/ '\xB5' {$$ = 0xB5;} |
   /*�*/ '\xB6' {$$ = 0xB6;} |
   /*�*/ '\xB7' {$$ = 0xB7;} |
   /*�*/ '\xB8' {$$ = 0xB8;} |
   /*�*/ '\xBA' {$$ = 0xBA;} |
   /*�*/ '\xBB' {$$ = 0xBB;} |
   /*�*/ '\xBC' {$$ = 0xBC;} |
   /*�*/ '\xBD' {$$ = 0xBD;} |
   /*�*/ '\xBE' {$$ = 0xBE;} |
   /*�*/ '\xBF' {$$ = 0xBF;} |
   /*�*/ '\xD7' {$$ = 0xD7;} |
   /*�*/ '\xF7' {$$ = 0xF7;};

compoundchar: '\\' escapedchar {$$ = $2;} |
      '\\' x hexit hexit {$$ = 16*XDIGIT ($3) + XDIGIT ($4);};

escapedchar: '\'' {$$ = '\'';} | '"' {$$ = '"';} |
      '\\' {$$ = '\\';} | n {$$ = '\n';} | '0' {$$ = '\0';};

/*------------------------------------------------------------------------*/
/*Metarules expanded*/

EMPTY:;

/*option*/
base_type_OPTION: base_type | EMPTY {$$.tag = def_TYPE_VOID;};
commentchar_SEQUENCE_OPTION: commentchar_SEQUENCE {} | EMPTY {};
condition_part_OPTION: condition_part | EMPTY {$$ = Empty;};
decl_SERIES_OPTION: decl_SERIES | EMPTY;
description_OPTION: description | EMPTY {$$ = Empty;};
DESCRIPTION_OPTION: DESCRIPTION {strcpy ($$, $1);} | EMPTY {*$$ = '\0';};
ellipsis_OPTION: ELLIPSIS {$$ = TRUE;} | EMPTY {$$ = FALSE;};
id_cont_SEQUENCE_OPTION: id_cont_SEQUENCE {strcpy ($$, $1);} |
      EMPTY {*$$ = '\0';};
pling_OPTION: PLING {$$ = TRUE;} | EMPTY {$$ = FALSE;};
word_SEQUENCE_OPTION: word_SEQUENCE {strcpy ($$, $1);} |
      EMPTY {*$$ = '\0';};
ws_item_SEQUENCE_OPTION: ws_item_SEQUENCE {} | EMPTY {};

/*sequence*/
bit_SEQUENCE: bit {$$ = $1 - '0';} |
      bit_SEQUENCE bit {$$ = ($1 << 1) | ($2 - '0');};
commentchar_SEQUENCE: commentchar {} | commentchar_SEQUENCE commentchar {};
digit_SEQUENCE: digit {$$ = $1 - '0';} |
      digit_SEQUENCE digit {$$ = 10*$1 + $2 - '0';};
hexit_SEQUENCE: hexit {$$ = XDIGIT ($1);} |
      hexit_SEQUENCE hexit {$$ = 16*$1 + XDIGIT ($2);};
id_cont_SEQUENCE: id_cont {$$ [0] = $1; $$ [1] = '\0';} |
      id_cont_SEQUENCE id_cont {sprintf($$, "%s%c", $1, $2);};
space_SEQUENCE: space {} | space space_SEQUENCE {};
word_SEQUENCE: word {strcpy ($$, $1);} |
      word word_SEQUENCE {sprintf($$, "%s %s", $1, $2);};
wordchar_SEQUENCE: wordchar {$$ [0] = $1; $$ [1] = '\0';} |
      wordchar_SEQUENCE wordchar {sprintf($$, "%s%c", $1, $2);};
ws_item_SEQUENCE: ws_item {} | ws_item_SEQUENCE ws_item {};

/*list*/
const_defn_LIST: const_defn {} |
      const_defn_LIST COMMA const_defn {};
entry_condition_LIST: entry_condition |
      entry_condition_LIST COMMA entry_condition {$$ = Union ($1, $3);};
exit_condition_LIST: exit_condition |
      exit_condition_LIST COMMA exit_condition {$$ = Union ($1, $3);};
needs_LIST: needs {} | needs_LIST COMMA needs;
needsatend_LIST: needsatend {} | needsatend_LIST COMMA needsatend;
typed_var_LIST:
   typed_var
      {  def_t t = emalloc(def_sizeof_TYPE ($1.tag));

         memcpy (t, &$1, def_sizeof_TYPE ($1.tag));
         $$.tag = def_TYPE_LIST;
         $$.name = NULL;
         $$.value = $1.value;
         $$.description = NULL;
         $$.data AS list.members [0] = t;
         $$.data AS list.count = 1;
      } |
   typed_var_LIST COMMA typed_var
      {  def_t t = emalloc(def_sizeof_TYPE ($3.tag));

         $$ = $1;
         memcpy (t, &$3, def_sizeof_TYPE ($3.tag));
         $$.value = MAX ($1.value, $3.value); /*only used if it's a union*/
         $$.description = NULL;
         $$.data AS list.members [$$.data AS list.count] = t;
         $$.data AS list.count++;
      };
toided_var_LIST:
   toided_var
      {  def_t t = emalloc(def_sizeof_TYPE ($1.tag));

         memcpy (t, &$1, def_sizeof_TYPE ($1.tag));
         $$.tag = def_TYPE_LIST;
         $$.name = NULL;
         $$.value = $1.value;
         $$.description = NULL;
         $$.data AS list.members [0] = t;
         $$.data AS list.count = 1;
      } |
   toided_var_LIST COMMA toided_var
      {  def_t t = emalloc(def_sizeof_TYPE ($3.tag));

         $$ = $1;
         memcpy (t, &$3, def_sizeof_TYPE ($3.tag));
         $$.value = MAX ($1.value, $3.value); /*only used if it's a union*/
         $$.description = NULL;
         $$.data AS list.members [$$.data AS list.count] = t;
         $$.data AS list.count++;
      };
swi_defn_LIST: swi_defn {} |
      swi_defn_LIST COMMA swi_defn {};
type_defn_LIST: type_defn {} |
      type_defn_LIST COMMA type_defn {};

/*series*/
decl_SERIES: decl {} | decl_SERIES SEMICOLON decl {};
/*------------------------------------------------------------------------*/
%%

struct def_s Union (struct def_s a, struct def_s b)
{  struct def_s u;
   int i;

   u.swi = a.swi | b.swi;

   u.i     = a.i     | b.i;
   u.o     = a.o     | b.o;
   u.c     = a.c     | b.c;
   u.k     = a.k     | b.k;
   u.f_in  = a.f_in  | b.f_in;
   u.f_out = a.f_out | b.f_out;

   for (i = 0; i < 10; i++)
   {  u.constants [i] = DISJUNCTION (a.constants [i], b.constants [i]);
      u.inputs    [i] = DISJUNCTION (a.inputs    [i], b.inputs    [i]);
      u.outputs   [i] = DISJUNCTION (a.outputs   [i], b.outputs   [i]);
   }
   u.ri = a.ri | b.ri;
   u.ro = a.ro | b.ro;

   u.starred_swi = a.starred_swi || b.starred_swi;
   for (i = 0; i < 10; i++)
      u.starred_constants [i] =
            a.starred_constants [i] || b.starred_constants [i];

   u.absent = a.absent || b.absent;

   for (i = 0; i < 10; i++)
      u.op [i] = DISJUNCTION (a.op [i], b.op [i]);

   u.value = DISJUNCTION (a.value, b.value);

   u.description = DISJUNCTION (a.description, b.description);

   return u;
}

int main (int argc, char *argv [])
{
   int i;
   enum {None, Riscose_OSAPI, Riscose_Template, Riscose_Header, Riscose_Errors} option = None;

    (progname = strrchr(*argv, '/')) ? progname++ : (progname = *argv);
    *argv = progname;
    debugf = verbosef = stderr;

    lookup_new(&needses, 16);
    lookup_new(&needsatends, 16);
    lookup_new(&consts, 16);
    lookup_new(&types, 16);
    lookup_new(&swis, 16);

   yydebug = 0;

   i = 1;
   while (i < argc && argv [i] [0] == '-')
   {
      if (strcmp (argv [i], "-riscose_osapi") == 0)
         option = Riscose_OSAPI;
      else if (strcmp (argv [i], "-riscose_template") == 0)
         option = Riscose_Template;
      else if (strcmp (argv [i], "-riscose_header") == 0)
         option = Riscose_Header;
      else if (strcmp (argv [i], "-riscose_errors") == 0)
         option = Riscose_Errors;
      else if (strcmp (argv [i], "-v") == 0)
         Verbose = TRUE;
      else if (strcmp (argv [i], "-q") == 0)
         Quiet = TRUE;
      else if (strcmp (argv [i], "-help") == 0)
      {  fputs("DefMod " VERSION_INFO
               ". Copyright � Jonathan Coxhead, 1994.\n"
               "DefMod comes with ABSOLUTELY NO WARRANTY.\n"
               "This is free software, and you are welcome to redistribute it "
               "under certain conditions. See the file Copying for details.\n"
               "Usage: DefMod <type> [-v] [-help] "
                     "[-26bit | -32bit] "
                     "< <module-interface-file>\n"
               "Purpose: generate output from a module interface file\n"
               "-v\t\t"             "verbose\n"
               "-q\t\t"             "quiet\n"

               "<type> is one of:\n"
               "\t-riscose_osapi\t\t"     "riscose osapi .c file\n"
               "\t-riscose_template\t\t"     "riscose template .c file\n"
               "\t-riscose_header\t\t"     "riscose osapi .h file\n"
               "\t-riscose_errors\t\t"     "riscose error definitions\n",
               stdout);
      } else {
            error("unknown option: %s\n", argv[i]);
        }

      i++;
   }

   if (option == None)
   {  Parse_Error = TRUE;
      goto finish;
   }

   yyparse ();
   if (Parse_Error) goto finish;

   switch (option)
   {
      case Riscose_OSAPI:
        riscose_osapi_output(Title, Author, needses,
            needsatends, consts, types, swis);
      break;

      case Riscose_Template:
        riscose_template_output(Title, Author, needses,
            needsatends, consts, types, swis);
      break;

      case Riscose_Header:
        riscose_header_output(Title, Author, needses,
            needsatends, consts, types, swis);
      break;

      case Riscose_Errors:
        riscose_errors_output(Title, Author, needses,
            needsatends, consts, types, swis);
      break;

      case None:
         /* Keep the compiler quiet */
      break;
   }

finish:
   exit(!!Parse_Error);
}

int yylex(void)
{
    int c;
    char s[1];

    if ((c = getchar()) == EOF) {
        return 0;
    }

    if (Verbose) {
        *s = c;
        escape_mem(stderr, s, 1);
    }

    return c;
}

void yyerror(char *s)
{
    error("%s at line %d\n", s, Line_No);
}
