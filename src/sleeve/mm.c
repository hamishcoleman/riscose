/* $Id$
 *
 * Memory interface
 *
 * (c) 2000 Chris Rutter <chris@armlinux.org>
 */

/* --- Header files --- */

#include <errno.h>
#include <unistd.h>
#include <sys/mman.h>
#include <mLib/alloc.h>
#include <mLib/exc.h>
#include <mLib/report.h>
#include <monty/mem.h>

#include "sleeve.h"
#include "mm.h"

/* --- Make sure we're supporting at least one translation mode --- */

#if !sleeve_SUPPORT_MM_DIRECT && !sleeve_SUPPORT_MM_OFFSET \
      && !sleeve_SUPPORT_MM_PAGED
  #error No memory translation modes have been selected.
#endif

/* --- No mmap, no direct translation --- */

#if sleeve_SUPPORT_DIRECT && !HAVE_MMAP
  #error This system does not support mmap(); direct memory translation will not work
#endif

/* --- Paged memory translation gubbins --- */

#if sleeve_SUPPORT_MM_PAGED
  #define mm_PAGE_ENTRY(v) ((v) >> mm_pageLog)
  #define mm_PAGE_UNMAPPED ((arm_byte *) 0)
  static arm_byte **mm_page;
  static size_t mm_pageEnd, mm_pageLog, mm_pageMask;
#endif

#if sleeve_SUPPORT_MM_OFFSET
  static ptrdiff_t mm_offset;
#endif

/* --- Internal variables --- */

static size_t mm_pageSize;
static unsigned mm_translation;

/* --- @mm_readPageSize@ --- */

size_t inline mm_readPageSize (void)
{
  return mm_pageSize;
}

/* --- @mm_init@ --- */

void mm_init (unsigned t, size_t l, unsigned p, ptrdiff_t o)
{
  switch ((mm_translation = t))
  {
    #if sleeve_SUPPORT_MM_OFFSET
    case mm_TRANSLATION_OFFSET:
      mm_offset = o;
      mm_pageSize = (size_t) sysconf (_SZ_PAGESIZE);
      sleeve_printf("mm_init: Initialised offset translation by %l.\n",
                    (long) o);
      break;
    #endif

    #if sleeve_SUPPORT_MM_DIRECT
    case mm_TRANSLATION_DIRECT:
      mm_pageSize = (size_t) sysconf (_SC_PAGESIZE);
      sleeve_printf("mm_init: Initialised direct translation.\n");
      break;
    #endif

    #if sleeve_SUPPORT_MM_PAGED
    case mm_TRANSLATION_PAGED:

      /* --- Allocate the page tables --- */

      mm_pageEnd = l;
      mm_pageLog = p + 2;
      mm_pageSize = 4 << p;
      mm_pageMask = mm_pageSize - 1;
      mm_page = emalloc((sizeof (arm_byte *)) * (mm_pageEnd / mm_pageSize));

      /* --- Set them all as unmapped --- */

      {
        size_t i, e = mm_PAGE_ENTRY(mm_pageEnd);
        for (i = 0; i < e; i++)
	 mm_page[i] = mm_PAGE_UNMAPPED;
      }

      sleeve_printf(
        "mm_init: Initialised page map of size %u bytes; pages %u bytes.\n",
        mm_pageEnd, mm_pageSize);
      break;
    #endif

    default:
      die(/*EXIT_FAILURE*/ 1, "mm_init: No support for translation type %u.", t);
  }
}

/* --- @mm_map@ --- */

arm_byte *mm_map (arm_word q, size_t l)
{
  switch (mm_translation)
  {
    default:

    #if sleeve_SUPPORT_MM_DIRECT
    case mm_TRANSLATION_DIRECT:
    {
      arm_byte *p;

      /* --- Create a map at the right place --- */

      if ((p = mmap((void *) q, l, PROT_READ | PROT_WRITE,
	       MAP_PRIVATE | MAP_FIXED | MAP_ANONYMOUS, 0, 0)) == (void *) -1)
      {
        if (errno == ENOMEM)
          THROW(EXC_NOMEM, 0);
        else
          THROW(EXC_ERRNO, errno);
      }

      sleeve_printf("mm_map: Mapping 0x%x -> 0x%x directly (%d bytes).\n",
                    q, q+l, l);

      return p;
    }
    #endif

    #if sleeve_SUPPORT_MM_PAGED
    case mm_TRANSLATION_PAGED:
    {
      arm_byte *p, *pb;
      size_t i = mm_PAGE_ENTRY(q), e = mm_PAGE_ENTRY(q + l);

      /* --- Allocate a block and patch it in --- */

      /* FIXME: does this need to be zero'd? */
      pb = ecalloc(l);

      sleeve_printf("mm_map: Mapping %p into 0x%x -> 0x%x (%d bytes).\n",
                     pb, q, q + l, l);

      for (p = pb; i < e; i++, p += mm_readPageSize())
        mm_page[i] = p;

      return pb;
    }
    #endif
  }
}

/* --- @mm_mapFile@ --- */

arm_byte *mm_mapFile (arm_word q, int fd, off_t o, size_t l, unsigned flags)
{
  #if sleeve_SUPPORT_MM_DIRECT || sleeve_SUPPORT_MM_PAGED

  void *p;

  sleeve_printf("mm_mapFile: Mapping in offset %u in fd %d to 0x%x -> 0x%x"
                " (%d bytes).\n", (unsigned) o, fd, q, q + l, l);

  /* --- Snarfle the length --- *
   *
   * If it goes wrong, you're screwed, and nothing'll work anyway.
   */

  if (l == 0)
  {
    off_t c = lseek(fd, 0, SEEK_CUR);
    l = (size_t) lseek(fd, 0, SEEK_END) - c;
  }

  /* --- Make the mapping --- */

  switch (mm_translation)
  {
    default:

    #if sleeve_SUPPORT_MM_DIRECT
    case mm_TRANSLATION_DIRECT:
      p = mmap((void *) q, l, PROT_READ, MAP_PRIVATE | MAP_FIXED, fd, o);
      break;
    #endif

    #if sleeve_SUPPORT_MM_PAGED
    case mm_TRANSLATION_PAGED:
      p = mmap(0, l, PROT_READ, MAP_PRIVATE, fd, o);
      break;
    #endif
  }

  /* --- Check if something fubared --- */
  
  if (p == (void *) -1)
  {
    if (errno == ENOMEM)
      THROW(EXC_NOMEM, 0);
    else
      THROW(EXC_ERRNO, errno);
  }

  /* --- Synchronise page tables --- */

  #if sleeve_SUPPORT_MM_PAGED

  if (mm_translation == mm_TRANSLATION_PAGED)
  {
    arm_byte *pp;
    size_t i = mm_PAGE_ENTRY(p), e = mm_PAGE_ENTRY(p + l);

    for (pp = p; i < e; i++, pp += mm_readPageSize())
      mm_page[i] = pp;
  }

  #endif        
  #endif
}

/* --- @mm_unmapFile@ --- */

void mm_unmapFile (arm_word *p, size_t l)
{
  #if sleeve_SUPPORT_MM_DIRECT || sleeve_SUPPORT_MM_PAGED
  sleeve_printf("mm_unmapFile: Unmapping file at %p -> %p.\n", p, p + l);
  if (munmap(p, l) == (void *) -1)
    THROW(EXC_ERRNO, errno);
  #endif
}

/* --- @mm_resize@ --- */

arm_byte *mm_resize (arm_word q, size_t o, size_t n)
{
  switch (mm_translation)
  {
    default:

    #if sleeve_SUPPORT_MM_DIRECT
    case mm_TRANSLATION_DIRECT:
      if (mremap((void *) q, o, n, 0) == (void *) -1)
      {
        if (errno == ENOMEM)
          THROW(EXC_NOMEM, 0);
        else
          THROW(EXC_ERRNO, errno);
      }

      sleeve_printf("mm_remap: Resizing 0x%x from %d to %d bytes.\n", q, o, n);

      return (arm_byte *) q;
    #endif

    #if sleeve_SUPPORT_MM_PAGED
    case mm_TRANSLATION_PAGED:
    {
      arm_byte *pb, *p;
      size_t i = mm_PAGE_ENTRY(q), e = mm_PAGE_ENTRY(q + n);

      /* --- Reallocate the block and patch it in --- */

      pb = xrealloc((void *) q, n, o);

      sleeve_printf("mm_remap: Resizing 0x%x from %d to %d bytes; at %p.\n",
                     q, o, n, pb);

      for (p = pb; i < e; i++, p += mm_readPageSize())
        mm_page[i] = p;
 
      return pb;
    }
    #endif
  }
}

/* --- @mm_unmap@ --- */

extern void mm_unmap (arm_word q, size_t l)
{
  sleeve_printf("mm_unmap: Unmapping 0x%x -> 0x%x (%l bytes).\n", q, q + l, l);

  switch (mm_translation)
  {
    default:

    #if sleeve_SUPPORT_MM_DIRECT
    case mm_TRANSLATION_DIRECT:
      if (munmap((void *) q, l) == -1)
        THROW(EXC_ERRNO, errno);
      break;
    #endif

    #if sleeve_SUPPORT_MM_PAGED
    case mm_TRANSLATION_PAGED:
      xfree((void *) q);
      break;
    #endif
  }
}

/* --- @mm_chunk@ --- */

arm_byte inline *mm_chunk (arm_word p)
{
  #if sleeve_SUPPORT_MM_PAGED
  return p < mm_pageEnd ? mm_page[mm_PAGE_ENTRY(p)] : mm_PAGE_UNMAPPED;
  #else
  return 0;
  #endif
}

/* --- @mm_translateN@ --- */

arm_byte inline *(mm_translateN) (arm_word p)
{
  switch (mm_translation)
  {
    default:

    #if sleeve_SUPPORT_MM_DIRECT
    case mm_TRANSLATION_DIRECT:
      return (arm_byte *) p;
    #endif

    #if sleeve_SUPPORT_MM_PAGED
    case mm_TRANSLATION_PAGED:
    {
      register arm_byte *c = mm_chunk(p);
      return c ? c + (p & mm_pageMask) : 0;
    }
    #endif

    #if sleeve_SUPPORT_MM_OFFSET
    case mm_TRANSLATION_OFFSET:
      return p + mm_offset;
    #endif
  }
}

/* --- @mm_translate@ --- */

arm_byte inline *(mm_translate) (arm_word p)
{
  switch (mm_translation)
  {
    default:

    #if sleeve_SUPPORT_MM_PAGED
    case mm_TRANSLATION_PAGED:
    {
      register arm_byte *q = mm_translateN(p);
      if (q)
        return q;
      else
        THROW(mm_EXC_ABORT, p);
    }
    #else
      return mm_translateN(p);
    #endif
  }
}

/* --- @mm_load32@ --- */

arm_word inline (mm_load32) (arm_word p)
{
  sleeve_dprintf(sleeve_OWNER_MM, "MM: loading 0x%x\n", p);
  return mm_LOAD32((arm_word *) mm_translate(p));
}

/* --- @mm_load32N@ --- */

mm_result inline (mm_load32N) (arm_word p, arm_word *v)
{
  register arm_word *c = (arm_word *) mm_translateN(p);

  sleeve_dprintf(sleeve_OWNER_MM, "MM: loading 0x%x = ", p);

  if (c)
  {
    *v = mm_LOAD32(c);
    sleeve_dprintf(sleeve_OWNER_MM, "0x%08x / %d\n", *v, *v);
    return mm_SUCCESS;
  }
  else
    return mm_ABORT;
}

/* --- @mm_load8@ --- */

arm_byte inline (mm_load8) (arm_word p)
{
  sleeve_dprintf(sleeve_OWNER_MM, "MM: loading 0x%x\n", p);
  return mm_LOAD8(mm_translate(p));
}

/* --- @mm_load8N@ --- */

mm_result inline (mm_load8N) (arm_word p, arm_word *v)
{
  register arm_byte *c = mm_translateN(p);

  sleeve_dprintf(sleeve_OWNER_MM, "MM: loading 0x%x = ", p);

  if (c)
  {
    *v = *c;
    sleeve_dprintf(sleeve_OWNER_MM, "0x%02x / %d\n", *v, *v);
    return mm_SUCCESS;
  }
  else
    return mm_ABORT;
}

/* --- @mm_store32N@ --- */

mm_result inline (mm_store32N) (arm_word p, arm_word v)
{
  register arm_word *c = (arm_word *) mm_translateN(p);

  sleeve_dprintf(sleeve_OWNER_MM, "MM: storing 0x%08x / %d into 0x%x\n",
                 v, v, p);

  if (c)
  {
    mm_STORE32(c, v);
    return mm_SUCCESS;
  }
  else
    return mm_ABORT;
}

/* --- @mm_store32@ --- */

void inline (mm_store32) (arm_word p, arm_word v)
{
  sleeve_dprintf(sleeve_OWNER_MM, "MM: storing 0x%08x / %d into 0x%x\n",
                 v, v, p);
  mm_STORE32((arm_word *) mm_translate(p), v);
}

/* --- @mm_store8N@ --- */

mm_result inline (mm_store8N) (arm_word p, arm_byte v)
{
  register arm_byte *c = mm_translateN(p);

  sleeve_dprintf(sleeve_OWNER_MM, "MM: storing 0x%08x / %d into 0x%x\n",
                 v, v, p);

  if (c)
  {
    mm_STORE8(c, v);
    return mm_SUCCESS;
  }
  else
    return mm_ABORT;
}

/* --- @mm_store8@ --- */

void inline (mm_store8) (arm_word p, arm_byte v)
{
  sleeve_dprintf(sleeve_OWNER_MM, "MM: storing 0x%08x / %d into 0x%x\n",
                 v, v, p);
  mm_STORE8(mm_translate(p), v);
}
