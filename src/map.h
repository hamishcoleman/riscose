/*
RISC OS logical memory map
(cheers Robin!)

0x00000000
-> 16k 		system workspace
-> 32k  	scratch space
-> 28MB 	app space
-> 28MB + 8k	SVC stack
-> 30MB 	system heap (dynamic area)
-> 30MB + 8k 	undefined-mode stack
-> 31MB 	soft CAM map
-> 31MB + 32k 	cursor / system space / sound DMA
-> 31MB + 64k 	Nowhere
-> 33MB 	Fake screen (emulation of VIDC1)
-> 44MB 	RMA
-> 48MB 	Reserved for system use (RMA under RISC OS 4?)
-> 52MB 	I/O space
-> 53MB 	Reserved for VIDC1 emulation
-> 54MB 	VIDC20
-> 55MB 	Reserved for future expansion
-> 56MB 	Reserved for 2nd processor control regs
-> 64MB 	ROM
-> 2GB  	Dynamic areas
-> 2.5GB 	Copy of physical space
-> 4GB 		More dynamic areas

Because the ARM program counter (when in USR_26 mode, as RISC OS nearly always is, and riscose always is) is limited to 26 bits wide, code must be kept below the 64MB limit.  Because we can use the full 32 bit address space for indirect addressing (i.e. LDR R0,[R1]) data loading + storing can access the full 4 gig.
*/

#define KB  (1<<10)
#define MEG (1<<20)
#define GIG (1<<30)

#define MMAP_SCRATCH_BASE 0
#define MMAP_SCRATCH_SIZE 0x8000
#define MMAP_APP_BASE (32*KB)
#define MMAP_APP_SIZE (28*MEG-(32*KB))
#define MMAP_SVCSTACK_BASE (28*MEG)
#define MMAP_SVCSTACK_SIZE (64*KB)
#define MMAP_USRSTACK_BASE (29*MEG)
#define MMAP_USRSTACK_SIZE (64*KB)
#define MMAP_RMA_BASE (33*MEG)
#define MMAP_RMA_SIZE (11*MEG)
#define MMAP_ROM_BASE (56*MEG)
#define MMAP_ROM_SIZE (64*KB) /* actually 8MB, but hey... */
#define MMAP_SLOT_BASE	(128*MEG)
