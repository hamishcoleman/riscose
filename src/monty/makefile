#! /bin/make -f

# $Id$

C_FILES = \
    mem.c \
    monty.c \

H_FILES = \
    mem.h \
    monty.h \

LIB = libmonty.a

OBJ = \
    $(LIB)(mem.o) \
    $(LIB)(monty.o) \

# ----------------------------------------------------------------------

all: $(LIB)

# ----------------------------------------------------------------------

$(LIB): $(OBJ)

clean::
	rm -f $(LIB)
	rm -f `echo "$(OBJ)" | sed 's/$(LIB)(\([^.]*\.o\))/\1/g'`
