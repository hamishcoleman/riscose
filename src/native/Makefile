OBJS = mremap2.o run.o

all:	libnative.a

%.o:	%.S
	$(CC) -c -o $@ $< 

libnative.a:	$(OBJS)
	ar cr $@ $(OBJS)
	ranlib $@
