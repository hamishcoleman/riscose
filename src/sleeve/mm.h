/* mm.h
 *
 * Memory interface
 *
 * (c) 2000 Chris Rutter <chris@armlinux.org>
 */

#ifndef SLEEVE_MM_H
#define SLEEVE_MM_H

#ifdef __cplusplus
  extern "C" {
#endif

/* --- Header files --- */

#include "sleeve.h"
#include <stddef.h>
#include <mLib/exc.h>
#include <sys/types.h>
#include <unistd.h>
#include "arm.h"

/* --- @mm_init@ --- *
 *
 * Initialises the memory management system.  No other functions may be called
 * before this one.
 *
 * Three methods are available for the mapping between the virtual ARM address
 * space and physical memory, denoted by @t@:
 *
 *   - @mm_TRANSLATION_DIRECT@, where no translation is performed: ARM
 *     addresses should coincide with the native physical addresses; here
 *     @mm_map@ can be used as a veneer onto @mmap@.  This method has no
 *     support for trapping aborts; all illegal memory accesses will cause
 *     an underlying segmentation fault.
 *
 *   - @mm_TRANSLATION_PAGED@, a simple conventional paged memory system,
 *     where the address space runs from 0 to @l@ bytes, divided into pages
 *     of size @4 << p@ bytes; @mm_map@ should be used to setup the address
 *     space.  Aborts are trapped with this method.  @EXC_NOMEM@ may be thrown
 *     if space for the page tables couldn't be allocated.
 *
 *   - @mm_TRANSLATION_OFFSET@, where the offset @o@ is added to ARM addresses
 *     to locate their corresponding physical addresses.  There is neither
 *     support for mapping via @mm_map@ nor aborts in this method.
 *
 * Note that @mm_TRANSLATION_DIRECT@ may not work on all platforms if you
 * need a certain address range that the OS uses for something else, or the
 * underlying operating system doesn't support @mremap@ and you need to use
 * it.
 */

enum {
  mm_TRANSLATION_DIRECT,
  mm_TRANSLATION_PAGED,
  mm_TRANSLATION_OFFSET
};

extern void mm_init (unsigned t, size_t l, unsigned p, ptrdiff_t o);

/* --- @mm_readPageSize@ --- *
 *
 * Returns the underlying page size in direct translation modes, or the
 * emulated page size in paged translation modes, in bytes.  For compatibility,
 * you must use this call to discover the actual page size, if you need it,
 * rather than relying on what you passed to @mm_init@.
 */

extern size_t mm_readPageSize (void);

/* --- @mm_map@ --- *
 *
 * Claims an area of physical memory of length @l@, and maps it into the
 * ARM's memory map at @q@.  @l@ should be a multiple of @mm_readPageSize()@.
 * The address of the claimed area is returned.  @EXC_NOMEM@ will be thrown
 * if there was insufficient memory.
 *
 * In direct translation mode, uses @mmap@ instead to try to change the
 * underlying process page map; if this fails, @EXC_ERRNO@ is thrown (and
 * there isn't a particularly graceful way to recover: you should only use
 * this if you're sure your architecture can cope).
 *
 * This call has no meaning in offset translation modes.
 */

extern arm_byte *mm_map (arm_word q, size_t l);

/* --- @mm_mapFile@ --- *
 *
 * Maps range @o@ to @o + l@ in the file open on @fd@ into ARM memory at
 * virtual address @q@, returning the actual physical address.  @o@ should
 * be on a page boundary.  @l@ will be rounded up to the nearest page, or
 * if it is zero, the length of the rest of the file from @o@ will be taken.
 * @EXC_NOMEM@ and @EXC_ERRNO@ may be thrown.
 *
 * The area will be readable only; writes will generate an underlying
 * permission fault.  If you want to write to the area in any form, allocate
 * the area the conventional way and read the file in.  There are currently
 * no flags defined.
 *
 * This will be useful for mapping in ROM images.
 *
 * This function doesn't work in offset translation modes.
 */

extern arm_byte *mm_mapFile (arm_word q, int fd, off_t o, size_t l,
                             unsigned flags);

/* --- @mm_unmapFile@ --- *
 *
 * Unmaps the file mapped in between physical addresses @p@ and @p + l@.
 * May throw @EXC_ERRNO@.
 */

extern void mm_unmapFile(arm_word *p, size_t l);

/* --- @mm_resize@ --- *
 *
 * Remaps the area at @q@ of length @o@ to be length @n@, which should be a
 * multiple of @mm_readPageSize()@.  In direct translation modes, @EXC_ERRNO@
 * may be thrown if this fails.  @EXC_NOMEM@ will be thrown if there was
 * insufficient memory, but the status quo will be preserved.  A pointer to
 * the new area is returned: beware that areas can move.
 *
 * This call has no meaning in offset translation modes.
 */

extern arm_byte *mm_resize (arm_word q, size_t o, size_t n);

/* --- @mm_syncmap@ --- *
 *
 * Maps in the physical memory between @p@ and @p + l@ to the corresponding
 * space in the ARM memory map.
 *
 * This call has no meaning in offset translation modes.
 */

extern void mm_syncmap (arm_byte *p, size_t l);

/* --- @mm_unmap@ --- *
 *
 * Unmaps the whole region between @q@ and @q + l@.  In direct translation
 * mode, @EXC_ERRNO@ may be thrown if this fails.  You should only call this
 * on one area at a time, with @q@ as the area base.
 *
 * This call has no meaning in offset translation modes.
 */

extern void mm_unmap (arm_word q, size_t l);

/* --- Reading and writing emulated memory --- *
 *
 * The `N' variants of the load and store functions return an @mm_result@ to
 * indicate whether a transaction was to a valid or undefined memory address,
 * if supported by the current translation mode.
 */

typedef enum { mm_SUCCESS, mm_ABORT } mm_result;

/* --- @mm_load{8,32}[N]@ --- *
 *
 * Loads a word or byte from ARM memory location @p@.
 *
 * @mm_load{8,32}N@ store the result (if successful) in @*v@, and return an
 * @mm_result@ to indicate whether a data abort occurred.  @mm_load{8,32}@
 * simply return the value directly, and throw @mm_EXC_ABORT@ if an abort
 * occurred.  If aborts are not supported in the current translation method,
 * an underlying segmentation fault will be generated in either case.
 */

extern arm_word mm_load32 (arm_word p);
extern arm_byte mm_load8 (arm_word p);

#if sleeve_SUPPORT_MM_PAGED
  extern mm_result mm_load32N (arm_word p, arm_word *v);
  extern mm_result mm_load8N (arm_word p, arm_word *v);
#else
  #define mm_load32N(p, v)  ((*((arm_word *) (v)) = mm_load32(p)), mm_SUCCESS)
  #define mm_load8N(p, v)   ((*((arm_byte *) (v)) = mm_load8(p)), mm_SUCCESS)
  #if !sleeve_SUPPORT_MM_OFFSET
    #define mm_load32(p)    mm_LOAD32((arm_word *) mm_translate(p))
    #define mm_load8(p)     mm_LOAD8(mm_translate(p))
  #endif
#endif

/* --- @mm_store{8,32}[N]@ --- *
 *
 * Stores word or byte @v@ at ARM memory location @p@.  Similar exception
 * rules apply as for @mm_load@.
 */

extern void mm_store32 (arm_word p, arm_word v);
extern void mm_store8 (arm_word p, arm_byte v);

#if sleeve_SUPPORT_MM_PAGED
  extern mm_result mm_store32N (arm_word p, arm_word v);
  extern mm_result mm_store8N (arm_word p, arm_byte v);
#else
  #define mm_store32N(p, v)	mm_store32(p, v)
  #define mm_store8N(p, v)	mm_store8(p, v)
  #if !sleeve_SUPPORT_MM_OFFSET
    #define mm_store32(p, v)	mm_STORE32((arm_word *) mm_translate(p), v)
    #define mm_store8(p, v)     mm_STORE8(mm_translate(p), v)
  #endif
#endif

/* --- @mm_chunk@ --- *
 *
 * Returns the physical memory address of the base of the page corresponding
 * to the ARM memory page containing address @p@, or 0 if there's no memory
 * mapped in there.
 *
 * You can assume that addresses within a page have a linear relationship
 * between physical and virtual equivalents, and thus do @mm_LOAD32@ and
 * @mm_STORE32@ instructions directly.
 *
 * This call has no meaning outside paged translation modes.
 */

extern arm_byte *mm_chunk (arm_word p);

/* --- @mm_translate[N]@ --- *
 *
 * Returns the physical memory address corresponding to the ARM memory address
 * @p@.  @mm_translateN@ returns zero if the ARM address was invalid, whereas
 * @mm_translate@ throws @mm_EXC_ABORT@.  In direct translation modes this
 * becomes a no-op.
 */

extern arm_byte *mm_translate (arm_word p);
extern arm_byte *mm_translateN (arm_word p);

#if !sleeve_SUPPORT_MM_PAGED && !sleeve_SUPPORT_MM_OFFSET
#define mm_translate(p)		((arm_byte *) (p))
#define mm_translateN(p)	((arm_byte *) (p))
#endif

/* --- Exception handling --- *
 *
 * We have one exception, @mm_EXC_ABORT@, thrown when an attempt is made to
 * dereference an ARM address with no corresponding physical memory page.
 * If an attempt is made to access memory off the end of the size of virtual
 * memory (argument @l@ to @mm_init@), we treat it in the same way, as this
 * situation doesn't arise on RISC OS platforms, and there's no need to
 * distinguish it here.
 */

#define mm_EXC EXC_PAIR('s', 'l')
#define mm_EXC_ABORT EXC_ALLOCI(mm_EXC, 0u)

/* --- Memory access macros --- *
 *
 * *** NOTE: LIABLE TO IMMEDIATE CHANGE ***
 *
 * If @mm_NATURAL@ is defined, and the underlying architecture is big endian,
 * then the ARM's little endian words will actually be stored in memory in
 * big endian order, thus avoiding the need to permute the bytes with a costly
 * series of logical operations and shifts; the corresponding byte-access
 * macros are defined to reverse the order of the bytes within a word.
 *
 * So long as @arm_word@ is 32 bits wide and packs with no wasteage in between,
 * this is probably the most efficient and effective way to store memory:
 * all native C emulation code can be written to directly access the memory
 * values.  However, permutation will need to be performed upon file access.
 *
 * Alternatively, leave @mm_NATURAL@ undefined and the memory will be
 * guaranteed to be laid out just as on the native ARM platform.
 */

#ifdef mm_NATURAL
#  define mm_LOAD32(p) (*((arm_word *) (p)))
#  define mm_LOAD8(p) (*(((arm_byte *) (p)) ^ 3))
#  define mm_STORE32(p, v) ((*((arm_word *) (p))) = ((arm_word) (v)))
#  define mm_STORE8(p, v) ((*(((arm_byte *) (p)) ^ 3)) = ((arm_byte) (v)))
#else
#  define mm_LOAD32(p) arm_LOAD32(p)
#  define mm_LOAD8(p) (*(((arm_byte *) (p))))
#  define mm_STORE32(p, v) arm_STORE32(p, v)
#  define mm_STORE8(p, v) ((*(((arm_byte *) (p)))) = ((arm_byte) (v)))
#endif /* mm_NATURAL */

#ifdef __cplusplus
  }
#endif

#endif /* SLEEVE_MM_H */
