#! /bin/make -f

LIB = libarmul.a
OBJ = \
    $(LIB)(arm.o) \
    $(LIB)(armemu.o) \
    $(LIB)(arminit.o) \
    $(LIB)(armsupp.o) \
    $(LIB)(instr.o) \

all: $(LIB)

$(LIB): $(OBJ)

clean::
	rm -f $(LIB)
	rm -f `echo "$(OBJ)" | sed 's/$(LIB)(\([^.]*\.o\))/\1/g'`
