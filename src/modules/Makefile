#! /bin/make -f

LIB = libmodules.a
OBJ = \
    $(LIB)(BasicTrans.o) \
    $(LIB)(FileSwitch.o) \
    $(LIB)(SharedCLibrary.o) \
    $(LIB)(UtilityModule.o) \

all: $(LIB)

$(LIB): $(OBJ)

clean::
	rm -f $(LIB)
	rm -f `echo "$(OBJ)" | sed 's/$(LIB)(\([^.]*\.o\))/\1/g'`
