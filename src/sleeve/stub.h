#define CLIB_STUB(a,n)  (0xef000000 | (SWI_OS_TRAP<<20) | (n))
