/* osmisc.c
**
** (c) Matthew Bloch 2000
**
** See http://riscose.sourceforge.net/ for terms of distribution, and to
** pick up a later version of the software.
**
**   Emulation of the kernel provided OS_ SWIs; most of the `biggies' like
**   OS_File or OS_Word ought to be farmed out to separate files to avoid
**   clutter.
**
**   $Revision$
**   $Date$
*/
#include <stdio.h>
#include <stdlib.h>

#include "mem.h"
#include "arm.h"
#include "swi.h"
#include "rom.h"
#include "vdu.h"
#include "map.h"

#include <readline/readline.h>

WORD
swih_os_writei(WORD num)
{
  vdu(num & 0xff);
  return 0;
}

WORD
swih_os(WORD num)
{
  WORD c;
  
  switch(SWI_NUM(num))
    {
    
    case 0x0 : /* OS_WriteC */
      vdu(ARM_R0);
      return 0;

    case 0x1 : /* OS_WriteS */
      {
	WORD pc = ARM_R15 & 0x02fffffc, psr = ARM_R15 & 0xfc000003;
	char *p = (char *)MEM_TOHOST(pc);
	while (*p)
	  {
	    vdu(*(p++));
	    pc++;
	  }
	pc = (pc + 3) & ~3;
	arm_set_pc(pc);
	return 0;
      }
    
    case 0x2 : /* OS_Write0 */
      {
	char *p = (char *)MEM_TOHOST(ARM_R0);
	while (*p)
	  vdu(*(p++));
      }
      return 0;
    
    case 0x3 : /* OS_NewLine */
      vdu(10);
      vdu(13);
      return 0;
    
    case 0x6:
      osbyte();
      return 0;
    
    case 0x7:
      fprintf(stderr, "[OS_Word %ld]", ARM_R0);
      return 0;

    case 0x8: /* OS_File */
      osfile();
      return 0;
    
    case 0x9: /* OS_Find */
      switch(ARM_R0 & 0xf0)
      {
      }
      return 0;
    
    case 0xe: /* OS_ReadLine */
      {
        /* FIXME: not quite right! */
        char *l = readline(NULL);
        strncpy(MEM_TOHOST(ARM_R0 & 0x3fffffff), l, ARM_R1);
        ARM_SET_R1(strlen(l));
        free(l);
      }
      return 0;
    
    case 0xf:
      fprintf(stderr, "OS_Control called");
      return 0;
    
    case 0x1e:
      osmodule();
      return 0;
    
    case 0x46 : /* OS_WriteN */
      for(c=0; c!=arm_get_reg(1); c++)
        putc(* ((char*)MEM_TOHOST(ARM_R0)+c), stdout);
      
    case 0x10: /* OS_GetEnv */
      arm_set_reg(0, (WORD) MMAP_USRSTACK_BASE);
      arm_set_reg(1, 0x8000 + mem_get_wimpslot());
      arm_set_reg(2, (WORD) MMAP_USRSTACK_BASE+256);
      return 0;
    
    case 0x11: /* OS_Exit */
      printf("*** finished!\n");
      exit(arm_get_reg(0));
      return 0;
    
    case 0x16: /* OS_EnterOS (eeek!) */
      return 0;
    
    case 0x2b: /* OS_GenerateError */ 
      
      if (!SWI_X(num)) {
        fprintf(stderr, "*** Error from RISC OS (%08x): %s\n",
	   (unsigned) *((WORD*)MEM_TOHOST(ARM_R0)),
	   (char*) MEM_TOHOST(ARM_R0)+4);
        exit(1); /* FIXME: Sort the error handlers! */
      }
      return ARM_R0;
      
    case 0x6f: /* OS_CallASWI */
      printf("OS_CallASWI\n");
      swi_trap(ARM_R10);
      return 0;
    case 0x71: /* OS_CallASWIR12 */
      printf("OS_CallASWI %x\n", ARM_R12);
      swi_trap(ARM_R12);
      return 0;
    }
  return ERR_EM_UNHANDLEDSWI;
}

static const char *os_names[64] =
{
  "WriteC",  "WriteS", "Write0", "NewLine", "ReadC", "CLI", "Byte", "Word",
  "File", "Args", "BGet", "BPut", "GBPB", "Find", "ReadLine", "Control",  
  "GetEnv", "Exit", "SetEnv", "IntOn", "IntOff", "CallBack", "EnterOS",
  "BreakPt", "BreakCtrl", "UnusedSWI", "UpdateMEMC", "SetCallBack", "Mouse",  
  "Heap", "Module", "Claim", "Release", "ReadUnsigned", "GenerateEvent",
  "ReadVarVal", "SetVarVal", "GSInit", "GSRead", "GSTrans", "BinaryToDecimal",
  "FSControl", "ChangeDyamicArea", "GenerateError", "ReadEscapeState",
  "EvaluateExpression", "SpriteOp", "ReadPalette", "ServiceCall",  
  "ReadVduVariables", "ReadPoint", "UpCall", "CallAVector", "ReadModeVariable",
  "RemoveCursors", "RestoreCursors", "SWINumberToString", "SWINumberFromString",
  "ValidateAddress", "CallAfter", "CallEvery", "RemoveTickerEvent",
  "InstallKeyHandler", "CheckModeValid"
};

static const char *os_names_40[64] =
{
  "ChangeEnvironment", "ClaimScreenMemory", "ReadMonotonicTime", "SubstituteArgs",
  "PrettyPrint","Plot", "WriteN", "AddToVector", "WriteEnv", "ReadArgs", 
  "ReadRAMFsLimits", "ClaimDeviceVector", "ReleaseDeviceVector", 
  "DelinkApplication", "RelinkApplication", "HeapSort", "ExitAndDie", 
  "ReadMemMapInfo", "ReadMemMapEntries", "SetMemMapEntries", "AddCallBack", 
  "ReadDefaultHandler", "SetECFOrigin", "SerialOp", "ReadSysInfo", "Confirm", 
  "ChangedBox", "CRC", "ReadDynamicArea", "PrintChar", "ChangeRedirection", 
  "RemoveCallBack", "FindMemMapEntries", "SetColour"
};

static const char *os_names_c0[64] =
{
  "ConvertStandardDateAndTime", "ConvertDateAndTime",
  NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  "ConvertHex1", "ConvertHex2", "ConvertHex4", "ConvertHex6", "ConvertHex8",
  "ConvertCardinal1", "ConvertCardinal2", "ConvertCardinal3", "ConvertCardinal4",
  "ConvertInteger1", "ConvertInteger2", "ConvertInteger3", "ConvertInteger4",
  "ConvertBinary1", "ConvertBinary2", "ConvertBinary3", "ConvertBinary4",
  "ConvertSpacedCardinal1", "ConvertSpacedCardinal2", "ConvertSpacedCardinal3", 
  "ConvertSpacedCardinal4", "ConvertSpacedInteger1", "ConvertSpacedInteger2",
  "ConvertSpacedInteger3", "ConvertSpacedInteger4", "ConvertFixedNetStation",
  "ConvertNetStation", "ConvertFixedFileSize", "ConvertFileSize", NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
};

DECLARE_SWI_CHUNK(00000000, "OS", os_names, swih_os);
DECLARE_SWI_CHUNK(00000040, "OS", os_names_40, swih_os);
DECLARE_SWI_CHUNK(000000C0, "OS", os_names_c0, swih_os);
DECLARE_SWI_CHUNK(00000100, "OS_WriteI", NULL, swih_os_writei);
DECLARE_SWI_CHUNK(00000140, "OS_WriteI", NULL, swih_os_writei);
DECLARE_SWI_CHUNK(00000180, "OS_WriteI", NULL, swih_os_writei);
DECLARE_SWI_CHUNK(000001C0, "OS_WriteI", NULL, swih_os_writei);
