#!/bin/make -f

LIB = libkernel.a
OBJ = \
    $(LIB)(module.o) \
    $(LIB)(osbyte.o) \
    $(LIB)(osmisc.o) \
    $(LIB)(osmodule.o) \
    $(LIB)(vdu.o) \

all: $(LIB)

$(LIB): $(OBJ)

clean::
	rm -f $(LIB)
	rm -f `echo "$(OBJ)" | sed 's/$(LIB)(\([^.]*\.o\))/\1/g'`
