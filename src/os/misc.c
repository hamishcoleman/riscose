/* FileSwitch.c
**
** (c) Matthew Bloch 2001
**
** See http://riscose.sourceforge.net/ for terms of distribution, and to
** pick up a later version of the software.
**
**   Emulation of FileSwitch, the RISC OS filesystem registry and core FS
**   services module.  This implements OS_File, OS_Args, OS_Find and basically
**   everything described in 2-9 to 2-194.  Previously known as osfileio.c
**   osfile.c osargs.c etc.
*/

#include <config.h>

#ifndef RISCOSEFSNAME
#  define RISCOSEFSNAME "RiscoseFS"
#endif
#ifndef MAXFILESYSTEMS
#  define MAXFILESYSTEMS 32
#endif
#ifndef MAXMAPPINGS
#  define MAXMAPPINGS 32
#endif
#ifndef MAXDRIVES
#  define MAXDRIVES 32
#endif
#ifndef PATHSEP
#  define PATHSEP "/"
#endif
#ifndef PATHPARENT
#  define PATHPARENT ".."
#endif
#ifndef PATHCURRENT
#  define PATHCURRENT "."
#endif

#include "riscostypes.h"
#include "arm.h"
#include "mem.h"
#include <rom/rom.h>
#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <monty/monty.h>
#include <monty/mem.h>

typedef union {
  /* Use number if number < MAXDRIVES, otherwise assume string pointer */
  int number;
  char *spec;
}
fileSystem_drivespec;

typedef struct {
  /* This is riscose's idea of a fileSystem, very much based on the FS 
  ** information block passed to OS_FSControl 12; riscosefs:: drives are
  ** represented separately
  */
  char *name, *startupText;
  
  /* Absolute addresses */ 
  WORD Open, GetBytes, PutBytes, Args, Close, File, Func, GBPB;
  WORD FSInfo1, FSInfo2;
  
  /* Keep track of selected drive, directories etc. */
  
  fileSystem_drivespec drvSelected;  
  char *csd; /* Currently selected directory         */
  char *csl; /* Currently selected library directory */
  char *psd; /* Previously selected directory        */
  char *urd; /* User root directory                  */
}
fileSystem_info;

typedef struct {
  char*                 special;
  char*                 filesystem;
  fileSystem_drivespec  drive;
  int                   pathElements;
  char**                path;
}
fileSystem_nameParsed;

static int              fsRegistered = 0;
static fileSystem_info *fileSystem[MAXFILESYSTEMS];
static int              fsSelected   = -1;

static int   emulatedDrives = 0;
static char* emulatedDrive[MAXMAPPINGS];
static char* emulatedPath[MAXMAPPINGS];

static FILE* fsHandles[255];
static int   fsNextHandle;

static void osfind_compact_handles(void)
{
    int i;
    
    for (i = 254; i >= 0; i--)
        if (fsHandles[i])
        {
            fsNextHandle = i+1;
            return;
        }
        
    fsNextHandle = 0;
    return;
}

static WORD osfind_allocate_fh(FILE *fh)
{
    if (fsNextHandle == 255)
    {
        osfind_compact_handles();
        if (fsNextHandle == 255)
            return 0;
    }
    
    fsHandles[fsNextHandle] = fh;
    return ++fsNextHandle;
}

static void osfind_deallocate_fh(WORD idx)
{
    fsHandles[idx-1] = NULL;
}

static FILE* osfind_lookup_fh(WORD idx)
{
    return fsHandles[idx-1];
}

void filehandles_init(void)
{
    int i;
    for (i = 0; i != 255; i++)
        fsHandles[i] = NULL;
    fsNextHandle = 0;
}

static int
allocNewFS(char *name, char *startup)
{
  if (fsRegistered == MAXFILESYSTEMS)
    return -1;
	
  NEW0(fileSystem[fsRegistered]);
  
  if (fsSelected == -1)
    fsSelected = fsRegistered;
	
  fsRegistered++;
  return fsRegistered-1;
}

static int
initEmulatedFS(void)
{
  /* Register emulated FS as first fileSystem; this doesn't have to happen
  ** if riscose can depend on 'native' fileSystems, but I feel this is unlikely
  ** to be a usable scenario for a while.
  */
  int reg = allocNewFS(RISCOSEFSNAME, RISCOSEFSNAME);
  
  if (reg == -1)
    return reg;

  fileSystem[reg]->csd = estrdup("$");
  fileSystem[reg]->csl = estrdup("$");
  fileSystem[reg]->psd = estrdup("$");
  fileSystem[reg]->urd = estrdup("$");

  return reg;
}

int
fileSystem_addriscosefsdrive(char *spec, char *path)
{
  if (emulatedDrives == 0)
    initEmulatedFS();
	
  if (emulatedDrives < MAXMAPPINGS)
  {
    emulatedDrive[emulatedDrives] = estrdup(spec);    
    emulatedPath[emulatedDrives]  = estrdup(path);
  
    emulatedDrives++;
  
    return emulatedDrives-1;
  }
  
  return -1;    
} 

fileSystem_info*
fileSystem_register(WORD fsinfoblock)
{
  return NULL;
}

static void
freeFileParsed(fileSystem_nameParsed *p)
{
  int el;
  
  if (p->special)
    free(p->special);
  if (p->filesystem)
    free(p->filesystem);
  for (el = 0; el < p->pathElements; el++)
    free(p->path[el]);
	
  free(p); 
}

static int
countDots(char *src)
{
  int at = 0, count = 0;
  
  while (src[at])
    if (src[at++] == '.')
	  count++;
	  
  return count;
}

#define BADPATH(c) if ((c)) { badPathLine = __LINE__; goto badpathfound; }
static fileSystem_nameParsed*
parseFilename(char *src)
{
  int badPathLine = 0, pathCount;
  char *scan;
  fileSystem_nameParsed* p;

  assert(src);
  
  /* Start with a blank parse structure */
  NEW0(p);
  
  /* Check for obscure syntaxes we don't support yet, see 2-11 -- NB warnings
  ** in this function are to be removed when we're surer that the code parses
  ** things successfully (except for those that warn about unsupported 
  ** features).  We don't want warnings about RISC OS programs legitimately
  ** passing us bad filenames.
  */
  
  BADPATH(src[0] == '-');
  BADPATH(strchr(src, '#') || strchr(src, '*'))
    
  if ( (scan = strchr(src, ':')) )
  {
    /* We've found a drive or filesystem name separator */
	
    if (scan[1] == ':')
	{
	  /* We've got both drive and filesystem specified */
	  char *endDriveSpec = strchr(scan+2, '.');
	  
	  /* FIXME: the parsing seems ad hoc.  e.g. this second BADPATH
	   * can't be true because otherwise we wouldn't have got past
	   * the first.  */
	  BADPATH(!endDriveSpec);
	  BADPATH(scan[2] == 0);
	  
	  p->filesystem     = xstrcpybetween(src, scan);
	  p->drive.spec = xstrcpybetween(scan+2, strchr(scan+2, '.'));
	  scan++;
	}
	else
	{
	  /* We've got only the drive spec; this must be at the start of the
	  ** string since we don't support anything fancy yet.
	  */
	  BADPATH(scan != src);
	  BADPATH(scan[1] == '.');
	  
	  p->drive.spec = xstrcpybetween(scan+1, strchr(scan+2, '.')); 
	}
	
	/* Assumes we're pointing to the colon preceeding the drive specifier */
	scan += 2 + strlen(p->drive.spec);
  }
  else
  {
    /* No drive or FS specifier, just a path */
    scan = src;
  }
  
  BADPATH(!*scan);
  
  /* Split path up */
  
  p->pathElements = countDots(scan)+1;
  p->path = emalloc(sizeof(char*) * p->pathElements);
  for (pathCount = 0; pathCount < p->pathElements; pathCount++)
  {
    char *scanEnd = strchr(scan, '.');
	if (!scanEnd)
	  scanEnd = scan+strlen(scan);
	p->path[pathCount] = xstrcpybetween(scan, scanEnd);
	scan = scanEnd+1;
  }
  
badpathfound:
  if (badPathLine)
  {
    warn("Bad path at line %d '%s'\n", badPathLine, src);
    freeFileParsed(p);
	p = NULL;
  }
  
  return p;
}
#undef BADPATH

static __inline__ char*
replacePathElement(char *src)
{
  if (src[1])
    return src;
  if (src[0] == '^')
    return PATHPARENT;
  if (src[0] == '@')
    return PATHCURRENT;

  return src;
}

static char*
fileSystem_constructPath(char *base, int pathElements, char** path)
{
  char* dst;
  int c, totalLen;
  
  totalLen = base ? 0 : strlen(base);
  
  for (c = 0; c < pathElements; c++)
    /* FIXME: shouldn't +1 be outside strlen? */
    totalLen += strlen(replacePathElement(path[c])+1);
	
  dst = emalloc(totalLen);
  /* FIXME: base might be NULL else why above test? */
  strcpy(dst, base);
  for (c = 0; c < pathElements; c++)
  {
	strcat(dst, PATHSEP);
	strcat(dst, replacePathElement(path[c]));
  }
  
  return dst;
}

static int
lookupFSname(char *name)
{
  int c;
  
  for (c = 0; c < fsRegistered; c++)
    if (!strcmp(fileSystem[c]->name, name))
	  return c;

  return -1;
}

static int
lookupEmulatedPath(fileSystem_drivespec spec)
{
  int c;
  
  for (c = 0; c < emulatedDrives; c++)
    if (!strcmp(emulatedDrive[c], spec.spec))
	  return c;

  return -1;
}

char*
fileSystem_mapname(char *src, int tryReverseExtensionHack)
{ 
  fileSystem_nameParsed* parsed   = parseFilename(src);
  char *dst;
  int fsNum, pathNum;
   
  if (!parsed)
    return NULL;

  fsNum = parsed->filesystem ? lookupFSname(parsed->filesystem) : fsSelected;
  
  if (fsSelected < 0)
  {
    warn("No filesystem selected, can't deduce relative pathname");
	freeFileParsed(parsed);
	return NULL;
  }
  
  if (fsNum < 0)
  {
    warn("Can't locate filesystem %s", parsed->filesystem);
	freeFileParsed(parsed);
	return NULL;
  }
  
  pathNum = lookupEmulatedPath(parsed->drive);
  
  if (pathNum < 0)
  {
    warn("Unknown emulated drive specified: %s", src);
	freeFileParsed(parsed);
	return NULL;
  }
  
  dst = fileSystem_constructPath(emulatedPath[pathNum], parsed->pathElements, parsed->path);
  freeFileParsed(parsed);
  return dst;
}

void osfile(void)
{
  WORD reason = ARM_R0;
    
  switch (reason)
    {
    case 1:
    case 2:
    case 3:
    case 4:
    case 9:
    case 18:
      fprintf(stderr, "FIXME: OS_File: Catalogue information NOT written\n");
      return;
    
    case 12:
    case 14:
    case 16:
    case 255: /* Load file into memory */
      if (file_objecttype(MEM_TOHOST(ARM_R1)) == OBJECT_NOTFOUND)
        { printf("fixme: OS_File error handling\n"); exit(1); }
      file_loadat(MEM_TOHOST(ARM_R1), MEM_TOHOST(ARM_R2));
      ARM_SET_R0(file_objecttype(MEM_TOHOST(ARM_R1)));
      printf("loaded object size %d\n", (unsigned)ARM_R4);
      /* FIXME: access bits */
      return;
    
    case 5:
    case 13:
    case 15:
    case 17: /* FIXME: implement various path options */
      fprintf(stderr, "FIXME: OS_File: Path options unimplemented\n");
      ARM_SET_R0(file_objecttype(MEM_TOHOST(ARM_R1)));
      if (ARM_R0 != OBJECT_NOTFOUND)
        ARM_SET_R4(file_size(MEM_TOHOST(ARM_R1)));
      printf("`%s' size %d\n", MEM_TOHOST(ARM_R1), (unsigned)ARM_R4);
      return;
      
    default:
      printf("Unemulated OS_File %d\n", reason);
      exit(1);
    }
}
void osfind(void)
{
    WORD reason = ARM_R0;
    
    if (reason == 0) /* close filehandle R1 */
    {
        fclose(fsHandles[ARM_R1-1]);
        osfind_deallocate_fh(ARM_R1);
    }
    else
    {
        char *flags = "";
        FILE *fh;
        int riscos_fh;
        
        switch (reason & 0xc0)
        {
            case 0x40 : flags = "rb"; break;
            case 0x80 : flags = "wb"; break;
            case 0xC0 : flags = "w+b"; break;
        }
        
        fh = fopen(MEM_TOHOST(ARM_R1), flags);
        if (fh == NULL || !(riscos_fh = osfind_allocate_fh(fh)))
        {
            if (reason & 8)
            {
                fprintf(stderr, "OS_Find open error for file %s\n", MEM_TOHOST(ARM_R1));
                exit(1);
            }
            else
            {
                ARM_SET_R0(0);
            }
        }
        else
        {
            ARM_SET_R0(riscos_fh);
        }
        /* FIXME: implement bit 2 for opening directories */
        /* FIXME: implement bits 0-1 for scannig paths */
    }
}

void osgbpb(void)
{
    int transferred;
    FILE *fh = NULL; /* avoid compiler warning */
   
    if (ARM_R0 >=1 || ARM_R0 <= 4)
      fh = osfind_lookup_fh(ARM_R1);
      
    switch (ARM_R0)
    {
        case 3:
            /* FIXME: check to see whether we hit end of file, use OS_Args
            ** to extend file instead
            */
            if (fseek(fh, ARM_R4, SEEK_SET) == -1)
                return;
        case 4:
            transferred = fread(MEM_TOHOST(ARM_R2), 1, ARM_R3, fh);
            ARM_SET_R3(ARM_R3 - transferred);
            ARM_SET_R2(ARM_R3 + transferred);
            ARM_SET_R4(ftell(fh));
            if (ARM_R3 == 0)
                arm_set_c();
            else
                arm_clear_c();
            return;
            
        default:
          printf("osgbpb %d\n", (int) ARM_R0);
          exit(1);
          break;
    }
}

void osargs(void)
{
    FILE *fh = ARM_R1 ? osfind_lookup_fh(ARM_R1) : NULL;
      
    switch (ARM_R0)
    {
        case 1: /* write an open file's sequential pointer */
            fseek(fh, ARM_R2, SEEK_SET);
            break;
            
        default:
            printf("Unhandled: OS_Args %ud\n", ARM_R0);
            exit(1);
    }
}

