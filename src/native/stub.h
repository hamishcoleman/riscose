#ifdef NATIVE_FASTCALL

#define STUB_BASE	0x03f00000
#define STUB_OFFSET	0x1000
#define STUB_SIZE	12
#define STUB_DESTINATION 0x03f10000
#define CLIB_STUB(a,n)  (0xea000000 | (((STUB_BASE + (STUB_SIZE * (n - STUB_OFFSET))) - (a + 8)) / 4))

#else

#define CLIB_STUB(a,n)  (0xef000000 | (SWI_OS_TRAP<<20) | (n))

#endif
