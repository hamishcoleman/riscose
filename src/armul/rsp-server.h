/* rsp-server.c -- Remote Serial Protocol server for GDB

   Copyright (C) 2008 Embecosm Limited

   Contributor Jeremy Bennett <jeremy.bennett@embecosm.com>

   This file is part of Or1ksim, the OpenRISC 1000 Architectural Simulator.

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the Free
   Software Foundation; either version 3 of the License, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
   more details.

   You should have received a copy of the GNU General Public License along
   with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* This program is commented throughout in a fashion suitable for processing
   with Doxygen. */


#ifndef RSP_SERVER__H
#define RSP_SERVER__H


/* Function prototypes for external use */
extern void  rsp_init ();
extern void  handle_rsp ();
extern void  rsp_exception (unsigned long int  except);

extern void  rsp_check_instruction ( ARMword pc );
extern void  rsp_check_memory_read( ARMword addr );
extern void  rsp_check_memory_write( ARMword addr );

#endif	/* RSP_SERVER__H */
