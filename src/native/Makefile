#! /bin/make -f

%.o: %.S
	$(CC) -c -o $@ $< 

LIB = libnative.a
OBJ = \
    $(LIB)(mremap2.o) \
    $(LIB)(native-arm.o) \
    $(LIB)(run.o) \

all: $(LIB)

$(LIB): $(OBJ)

clean::
	rm -f $(LIB)
	rm -f `echo "$(OBJ)" | sed 's/$(LIB)(\([^.]*\.o\))/\1/g'`
