/* def.h : basic types and macros
 *
 * This file is part of ObjDism.
 * (c) 1998 Gareth McCaughan.
 * See the file LICENCE for distribution conditions.
 */

#include <stdio.h>
#include <stdlib.h>

typedef unsigned long word;
typedef unsigned long address;
typedef unsigned long addrdiff;
#define W(x) ((word*)(x))

#define declstruct(name) typedef struct name s##name, * p##name
#define defstruct(name) struct name
#define defequiv(new,old) typedef struct old s##new, * p##new

declstruct(AOFfile);
declstruct(Area);
declstruct(Chunk);
declstruct(ChunkFile);
declstruct(Feature);
declstruct(SWIref);
declstruct(DisOptions);
declstruct(Instruction);

/* It's helpful to use suitable names for specific kinds of
 * feature. Unfortunately C's losing type-checking loses too
 * badly to make this *really* helpful.
 */
defequiv(FuncName,   Feature);
defequiv(LabelDef,   Feature);
defequiv(LabelRef,   Feature);
defequiv(Symbol,     Feature);
defequiv(Relocation, Feature);

#define _NEED_def_h
