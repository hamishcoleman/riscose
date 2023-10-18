/* rsp-server.c -- Remote Serial Protocol server for GDB

   Copyright (C) 2008 Embecosm Limited

   Contributor Jeremy Bennett <jeremy.bennett@embecosm.com>

   This file was originally part of Or1ksim, the OpenRISC 1000 Architectural
   Simulator.

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


#include "riscostypes.h"
#include "mem.h"
#include "armdefs.h"
#include "arm.h"
#include "rom/rom.h"

/* System includes */
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <poll.h>
#include <netinet/tcp.h>
#include <signal.h>
#include <string.h>
#include <assert.h>

/* Define to log each packet */
#define RSP_TRACE  0

/*! Thread ID used by Or1ksim */
#define OR1KSIM_TID  1

#define NUM_REGS 17 /*!< Total GDB registers */

/*! Definition of GDB target signals. Data taken from the GDB 6.8
    source. Only those we use defined here. */
enum target_signal {
  TARGET_SIGNAL_NONE =  0,
  TARGET_SIGNAL_INT  =  2,
  TARGET_SIGNAL_ILL  =  4,
  TARGET_SIGNAL_TRAP =  5,
  TARGET_SIGNAL_FPE  =  8,
  TARGET_SIGNAL_BUS  = 10,
  TARGET_SIGNAL_SEGV = 11,
  TARGET_SIGNAL_ALRM = 14,
  TARGET_SIGNAL_USR2 = 31,
  TARGET_SIGNAL_PWR  = 32
};

/*! The maximum number of characters in inbound/outbound buffers.  The largest
    packets are the 'G' packet, which must hold the 'G' and all the registers
    with two hex digits per byte and the 'g' reply, which must hold all the
    registers, and (in our implementation) an end-of-string (0)
    character. Adding the EOS allows us to print out the packet as a
    string. So at least NUMREGBYTES*2 + 1 (for the 'G' or the EOS) are needed
    for register packets */
#define GDB_BUF_MAX 1024

/*! Size of the matchpoint hash table. Largest prime < 2^10 */
#define MP_HASH_SIZE  1021

/*! String to map hex digits to chars */
static const char hexchars[]="0123456789abcdef";

/*! Data structure for RSP buffers. Can't be null terminated, since it may
  include zero bytes */
struct rsp_buf
{
  char  data[GDB_BUF_MAX];
  int   len;
};

/*! Enumeration of different types of matchpoint. These have explicit values
    matching the second digit of 'z' and 'Z' packets. */
enum mp_type {
  // No need for software breakpoints, we can hook into the ARM emulator. */
  //BP_MEMORY   = 0,
  BP_HARDWARE = 1,
  WP_WRITE    = 2,
  WP_READ     = 3,
  WP_ACCESS   = 4
};

/*! Data structure for a matchpoint hash table entry */
struct mp_entry
{
  enum mp_type       type;		/*!< Type of matchpoint */
  ARMword            addr;		/*!< Address with the matchpoint */
  struct mp_entry   *next;		/*!< Next entry with this hash */
};

/*! Central data for the RSP connection */
static struct
{
  int                client_waiting;	/*!< Is client waiting a response? */
  int                single_stepping;
  int                stalled;
  int                proto_num;		/*!< Number of the protocol used */
  int                client_fd;		/*!< FD for talking to GDB */
  int                sigval;		/*!< GDB signal for any exception */
  struct mp_entry   *mp_hash[MP_HASH_SIZE];	/*!< Matchpoint hash table */
} rsp;

/* Forward declarations of static functions */
static void               rsp_get_client ();
static void               rsp_client_request ();
static void               rsp_client_close ();
static void               put_packet (struct rsp_buf *buf);
static void               put_str_packet (const char *str);
static struct rsp_buf    *get_packet ();
static void               put_rsp_char (char  c);
static int                get_rsp_char ();
static int                rsp_unescape (char *data,
					int   len);
static void               mp_hash_init ();
static void               mp_hash_add (enum mp_type       type,
				       unsigned long int  addr);
static struct mp_entry   *mp_hash_lookup (enum mp_type       type,
					  unsigned long int  addr);
static struct mp_entry   *mp_hash_delete (enum mp_type       type,
					  unsigned long int  addr);
static int                hex (int  c);
static void               reg2hex (ARMword  val,
				   char              *buf);
static ARMword  hex2reg (char *buf);
static void               set_npc (ARMword  addr);
static void               rsp_report_exception ();
static void               rsp_continue (struct rsp_buf *buf);
static void               rsp_read_all_regs ();
static void               rsp_write_all_regs (struct rsp_buf *buf);
static void               rsp_read_mem (struct rsp_buf *buf);
static void               rsp_write_mem (struct rsp_buf *buf);
static void               rsp_read_reg (struct rsp_buf *buf);
static void               rsp_write_reg (struct rsp_buf *buf);
static void               rsp_query (struct rsp_buf *buf);
static void               rsp_set (struct rsp_buf *buf);
static void               rsp_step (struct rsp_buf *buf);
static void               rsp_vpkt (struct rsp_buf *buf);
static void               rsp_write_mem_bin (struct rsp_buf *buf);
static void               rsp_remove_matchpoint (struct rsp_buf *buf);
static void               rsp_insert_matchpoint (struct rsp_buf *buf);

extern char _binary_arm_core_xml_start[];
extern char _binary_arm_core_xml_end[];


typedef struct
{
  uint32_t a_type;              /* Entry type */
  union
    {
      uint32_t a_val;           /* Integer value */
      /* We use to have pointer elements added here.  We cannot do that,
         though, since it does not work when using 32-bit definitions
         on 64-bit platforms and vice versa.  */
    } a_un;
} Elf32_auxv_t;


Elf32_auxv_t auxv[] = {
    { 16, { 1<<3 } },  // AT_HWCAP (26 bit)
    { 15, { STRING_auxv_platform_string } },        // AT_PLATFORM
    { 9, { 0x8000 } }, // AT_ENTRY
    { 10, { 0x1} },    // AT_NOTELF
    { 0, { 0 } },
};

static void set_stall_state(int state) {
    rsp.stalled = state;
}

/*---------------------------------------------------------------------------*/
/*!Initialize the Remote Serial Protocol connection

   Set up the central data structures.                                       */
/*---------------------------------------------------------------------------*/
void
rsp_init ()
{
  assert(((NUM_REGS) * 8 + 1) < GDB_BUF_MAX);
  /* Clear out the central data structure */
  rsp.client_waiting =  0;		/* GDB client is not waiting for us */
  rsp.single_stepping=  1;
  rsp.stalled        =  0;
  rsp.client_fd      = -1;		/* i.e. invalid */
  rsp.sigval         =  0;		/* No exception */

  /* Set up the matchpoint hash table */
  mp_hash_init ();

}	/* rsp_init () */


/*---------------------------------------------------------------------------*/
/*!Look for action on RSP

   This function is called when the processor has stalled, which, except for
   initialization, must be due to an interrupt.

   If we have no RSP client, we get one. We can make no progress until the
   client is available.

   Then if the cause is an exception following a step or continue command, and
   the exception not been notified to GDB, a packet reporting the cause of the
   exception is sent.

   The next client request is then processed.                                */
/*---------------------------------------------------------------------------*/
void
handle_rsp ()
{
  /* If we have no RSP client, wait until we get one. */
  set_stall_state(1);
  while (-1 == rsp.client_fd)
    {
      rsp_get_client ();
      rsp.client_waiting = 0;		/* No longer waiting */
    }

  /* If we have an unacknowledged exception tell the GDB client. If this
     exception was a trap due to a memory breakpoint, then adjust the NPC. */
  if (rsp.client_waiting)
    {
      rsp_report_exception();
      rsp.client_waiting = 0;		/* No longer waiting */
    }

  /* Get a RSP client request */
  while (rsp.stalled && -1 != rsp.client_fd) {
    rsp_client_request ();
  }

}	/* handle_rsp () */


/*---------------------------------------------------------------------------*/
/*!Note an exception for future processing

   The simulator has encountered an exception. Record it here, so that a
   future call to handle_exception will report it back to the client. The
   signal is supplied in Or1ksim form and recorded in GDB form.

   We flag up a warning if an exception is already pending, and ignore the
   earlier exception.

   @param[in] except  The exception (Or1ksim form)                           */
/*---------------------------------------------------------------------------*/
void
rsp_exception (unsigned long int  except)
{
  int  sigval;			/* GDB signal equivalent to exception */

  switch (except)
    {
    default:
      fprintf (stderr, "Warning: Unknown RSP exception %lu: Ignored\n", except);
      return;
    }

  if ((0 != rsp.sigval) && (sigval != rsp.sigval))
    {
      fprintf (stderr, "Warning: RSP signal %d received while signal "
	       "%d pending: Pending exception replaced\n", sigval, rsp.sigval);
    }

  rsp.sigval         = sigval;		/* Save the signal value */

}	/* rsp_exception () */


/*---------------------------------------------------------------------------*/
/*!Get a new client connection.

   Blocks until the client connection is available.

   A lot of this code is copied from remote_open in gdbserver remote-utils.c.

   This involves setting up a socket to listen on a socket for attempted
   connections from a single GDB instance (we couldn't be talking to multiple
   GDBs at once!).

   The service is specified either as a port number in the Or1ksim configuration
   (parameter rsp_port in section debug, default 51000) or as a service name
   in the constant OR1KSIM_RSP_SERVICE.

   The protocol used for communication is specified in OR1KSIM_RSP_PROTOCOL. */
/*---------------------------------------------------------------------------*/
static void
rsp_get_client ()
{
  int                 tmp_fd;		/* Temporary descriptor for socket */
  int                 optval;		/* Socket options */
  struct sockaddr_in  sock_addr;	/* Socket address */
  socklen_t           len;		/* Size of the socket address */

  /* Open a socket on which we'll listen for clients */
  tmp_fd = socket (PF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (tmp_fd < 0)
    {
      fprintf (stderr, "ERROR: Cannot open RSP socket\n");
      abort();
    }

  /* Allow rapid reuse of the port on this socket */
  optval = 1;
  setsockopt (tmp_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&optval,
	      sizeof (optval));

  int rsp_port = 53415;

  /* Bind the port to the socket */
  sock_addr.sin_family      = PF_INET;
  sock_addr.sin_port        = htons (rsp_port);
  sock_addr.sin_addr.s_addr = INADDR_ANY;
  if (bind (tmp_fd, (struct sockaddr *) &sock_addr, sizeof (sock_addr)))
    {
      fprintf (stderr, "ERROR: Cannot bind to RSP socket\n");
      abort();
    }
      
  /* Listen for (at most one) client */
  if (listen (tmp_fd, 1))
    {
      fprintf (stderr, "ERROR: Cannot listen on RSP socket\n");
      abort();
    }

  printf ("Listening for RSP on port %d\n", rsp_port);
  fflush (stdout);

  /* Accept a client which connects */
  len = sizeof (socklen_t);		/* Bug fix by Julius Baxter */
  rsp.client_fd = accept (tmp_fd, (struct sockaddr *)&sock_addr, &len);

  if (-1 == rsp.client_fd)
    {
      fprintf (stderr, "Warning: Failed to accept RSP client\n");
      return;
    }

  /* Enable TCP keep alive process */
  optval = 1;
  setsockopt (rsp.client_fd, SOL_SOCKET, SO_KEEPALIVE, (char *)&optval,
	      sizeof (optval));

  /* Don't delay small packets, for better interactive response (disable
     Nagel's algorithm) */
  optval = 1;
  setsockopt (rsp.client_fd, IPPROTO_TCP, TCP_NODELAY, (char *)&optval,
	      sizeof (optval));

  /* Socket is no longer needed */
  close (tmp_fd);			/* No longer need this */
  signal (SIGPIPE, SIG_IGN);		/* So we don't exit if client dies */

  printf ("Remote debugging from host %s\n", inet_ntoa (sock_addr.sin_addr));

  /* If fcntl () is available, use it to enable async I/O
#if defined(F_SETFL) && defined (FASYNC)
  save_fcntl_flags = fcntl (remote_desc, F_GETFL, 0);
  fcntl (remote_desc, F_SETFL, save_fcntl_flags | FASYNC);
#if defined (F_SETOWN)
  fcntl (remote_desc, F_SETOWN, getpid ());
#endif
#endif */

}	/* rsp_get_client () */


/*---------------------------------------------------------------------------*/
/*!Deal with a request from the GDB client session

   In general, apart from the simplest requests, this function replies on
   other functions to implement the functionality.                           */
/*---------------------------------------------------------------------------*/
static void
rsp_client_request ()
{
  struct rsp_buf *buf = get_packet ();	/* Message sent to us */

  // Null packet means we hit EOF or the link was closed for some other
  // reason. Close the client and return
  if (NULL == buf)
    {
      rsp_client_close ();
      return;
    }

#if RSP_TRACE
  printf ("Packet received %s: %d chars\n", buf->data, buf->len );
  fflush (stdout);
#endif

  switch (buf->data[0])
    {
    case '!':
      /* Request for extended remote mode */
      put_str_packet ("OK");
      return;

    case '?':
      /* Return last signal ID */
      rsp_report_exception();
      return;

    case 'A':
      /* Initialization of argv not supported */
      fprintf (stderr, "Warning: RSP 'A' packet not supported: ignored\n");
      put_str_packet ("E01");
      return;

    case 'b':
      /* Setting baud rate is deprecated */
      fprintf (stderr, "Warning: RSP 'b' packet is deprecated and not "
	       "supported: ignored\n");
      return;

    case 'B':
      /* Breakpoints should be set using Z packets */
      fprintf (stderr, "Warning: RSP 'B' packet is deprecated (use 'Z'/'z' "
	       "packets instead): ignored\n");
      return;

    case 'c':
      /* Continue */
      rsp_continue (buf);
      return;

//    case 'C':
//      /* Continue with signal */
//      rsp_continue_with_signal (buf);
//      return;

    case 'd':
      /* Disable debug using a general query */
      fprintf (stderr, "Warning: RSP 'd' packet is deprecated (define a 'Q' "
	       "packet instead: ignored\n");
      set_stall_state(0);
      return;

    case 'D':
      /* Detach GDB. Do this by closing the client. The rules say that
	 execution should continue. TODO. Is this really then intended
	 meaning? Or does it just mean that only vAttach will be recognized
	 after this? */
      put_str_packet ("OK");
      rsp_client_close ();
      set_stall_state (0);
      rsp.sigval = TARGET_SIGNAL_NONE;	/* No signal now */
      return;

    case 'F':
      /* File I/O is not currently supported */
      fprintf (stderr, "Warning: RSP file I/O not currently supported: 'F' "
	       "packet ignored\n");
      return;

    case 'g':
      rsp_read_all_regs ();
      return;

    case 'G':
      rsp_write_all_regs (buf);
      return;
      
    case 'H':
      /* Set the thread number of subsequent operations. For now ignore
	 silently and just reply "OK" */
      put_str_packet ("OK");
      return;

    case 'i':
      /* Single instruction step */
      rsp_step (buf);
      return;

    case 'I':
      /* Single instruction step with signal */
      fprintf (stderr, "Warning: RSP cycle stepping not supported: target "
	       "stopped immediately\n");
      rsp.client_waiting = 1;			/* Stop reply will be sent */
      return;

    case 'k':
      /* Kill request. Do nothing for now. */
      return;

    case 'm':
      /* Read memory (symbolic) */
      rsp_read_mem (buf);
      return;

    case 'M':
      /* Write memory (symbolic) */
      rsp_write_mem (buf);
      return;

    case 'p':
      /* Read a register */
      rsp_read_reg (buf);
      return;

    case 'P':
      /* Write a register */
      rsp_write_reg (buf);
      return;

    case 'q':
      /* Any one of a number of query packets */
      rsp_query (buf);
      return;

    case 'Q':
      /* Any one of a number of set packets */
      rsp_set (buf);
      return;

    case 'r':
      /* Reset the system. Deprecated (use 'R' instead) */
      fprintf (stderr, "Warning: RSP 'r' packet is deprecated (use 'R' "
	       "packet instead): ignored\n");
      return;

    case 's':
      /* Single step (one high level instruction). This could be hard without
	 DWARF2 info */
      rsp_step (buf);
      return;

    case 't':
      /* Search. This is not well defined in the manual and for now we don't
	 support it. No response is defined. */
      fprintf (stderr, "Warning: RSP 't' packet not supported: ignored\n");
      return;

    case 'T':
      /* Is the thread alive. We are bare metal, so don't have a thread
	 context. The answer is always "OK". */
      put_str_packet ("OK");
      return;

    case 'v':
      /* Any one of a number of packets to control execution */
      rsp_vpkt (buf);
      return;

    case 'X':
      /* Write memory (binary) */
      rsp_write_mem_bin (buf);
      return;

    case 'z':
      /* Remove a breakpoint/watchpoint. */
      rsp_remove_matchpoint (buf);
      return;

    case 'Z':
      /* Insert a breakpoint/watchpoint. */
      rsp_insert_matchpoint (buf);
      return;

    default:
      /* Unknown commands are ignored */
      fprintf (stderr, "Warning: Unknown RSP request %s\n", buf->data);
      return;
    }
}	/* rsp_client_request () */


/*---------------------------------------------------------------------------*/
/*!Close the connection to the client if it is open                          */
/*---------------------------------------------------------------------------*/
static void
rsp_client_close ()
{
  if (-1 != rsp.client_fd)
    {
      close (rsp.client_fd);
      rsp.client_fd = -1;
    }
}	/* rsp_client_close () */


/*---------------------------------------------------------------------------*/
/*!Send a packet to the GDB client

   Modeled on the stub version supplied with GDB. Put out the data preceded by
   a '$', followed by a '#' and a one byte checksum. '$', '#', '*' and '}' are
   escaped by preceding them with '}' and then XORing the character with
   0x20.

   @param[in] buf  The data to send                                          */
/*---------------------------------------------------------------------------*/
static void
put_packet (struct rsp_buf *buf)
{
  int  ch;				/* Ack char */

  /* Construct $<packet info>#<checksum>. Repeat until the GDB client
     acknowledges satisfactory receipt. */
  do
    {
      unsigned char checksum = 0;	/* Computed checksum */
      int           count    = 0;	/* Index into the buffer */

#if RSP_TRACE
      printf ("Putting %s\n", buf->data);
      fflush (stdout);
#endif

      put_rsp_char ('$');		/* Start char */

      /* Body of the packet */
      for (count = 0; count < buf->len; count++)
	{
	  unsigned char  ch = buf->data[count];

	  /* Check for escaped chars */
	  if (('$' == ch) || ('#' == ch) || ('*' == ch) || ('}' == ch))
	    {
	      ch       ^= 0x20;
	      checksum += (unsigned char)'}';
	      put_rsp_char ('}');
	    }

	  checksum += ch;
	  put_rsp_char (ch);
	}

      put_rsp_char ('#');		/* End char */

      /* Computed checksum */
      put_rsp_char (hexchars[checksum >> 4]);
      put_rsp_char (hexchars[checksum % 16]);

      /* Check for ack of connection failure */
      ch = get_rsp_char ();
      if (-1 == ch)
	{
	  return;			/* Fail the put silently. */
	}
    }
  while ('+' != ch);

}	/* put_packet () */


/*---------------------------------------------------------------------------*/
/*!Convenience to put a constant string packet

   param[in] str  The text of the packet                                     */
/*---------------------------------------------------------------------------*/
static void
put_str_packet (const char *str)
{
  struct rsp_buf  buf;
  int             len = strlen (str);

  /* Construct the packet to send, so long as string is not too big,
     otherwise truncate. Add EOS at the end for convenient debug printout */

  if (len >= GDB_BUF_MAX)
    {
      fprintf (stderr, "Warning: String %s too large for RSP packet: "
	       "truncated\n", str);
      len = GDB_BUF_MAX - 1;
    }

  strncpy (buf.data, str, len);
  buf.data[len] = 0;
  buf.len       = len;

  put_packet (&buf);

}	/* put_str_packet () */


/*---------------------------------------------------------------------------*/
/*!Get a packet from the GDB client
  
   Modeled on the stub version supplied with GDB. The data is in a static
   buffer. The data should be copied elsewhere if it is to be preserved across
   a subsequent call to get_packet().

   Unlike the reference implementation, we don't deal with sequence
   numbers. GDB has never used them, and this implementation is only intended
   for use with GDB 6.8 or later. Sequence numbers were removed from the RSP
   standard at GDB 5.0.

   @return  A pointer to the static buffer containing the data                */
/*---------------------------------------------------------------------------*/
static struct rsp_buf *
get_packet ()
{
  static struct rsp_buf  buf;		/* Survives the return */

  /* Keep getting packets, until one is found with a valid checksum */
  while (1)
    {
      unsigned char  checksum;		/* The checksum we have computed */
      int            count;		/* Index into the buffer */
      int 	     ch;		/* Current character */

      /* Wait around for the start character ('$'). Ignore all other
	 characters */
      ch = get_rsp_char ();
      while (ch != '$')
	{
	  if (-1 == ch)
	    {
	      return  NULL;		/* Connection failed */
	    }

	  ch = get_rsp_char ();
	}

      /* Read until a '#' or end of buffer is found */
      checksum =  0;
      count    =  0;
      while (count < GDB_BUF_MAX - 1)
	{
	  ch = get_rsp_char ();

	  /* Check for connection failure */
	  if (-1 == ch)
	    {
	      return  NULL;
	    }

	  /* If we hit a start of line char begin all over again */
	  if ('$' == ch)
	    {
	      checksum =  0;
	      count    =  0;

	      continue;
	    }

	  /* Break out if we get the end of line char */
	  if ('#' == ch)
	    {
	      break;
	    }

	  /* Update the checksum and add the char to the buffer */

	  checksum        = checksum + (unsigned char)ch;
	  buf.data[count] = (char)ch;
	  count           = count + 1;
	}

      /* Mark the end of the buffer with EOS - it's convenient for non-binary
	 data to be valid strings. */
      buf.data[count] = 0;
      buf.len         = count;

      /* If we have a valid end of packet char, validate the checksum */
      if ('#' == ch)
	{
	  unsigned char  xmitcsum;	/* The checksum in the packet */

	  ch = get_rsp_char ();
	  if (-1 == ch)
	    {
	      return  NULL;		/* Connection failed */
	    }
	  xmitcsum = hex (ch) << 4;

	  ch = get_rsp_char ();
	  if (-1 == ch)
	    {
	      return  NULL;		/* Connection failed */
	    }

	  xmitcsum += hex (ch);

	  /* If the checksums don't match print a warning, and put the
	     negative ack back to the client. Otherwise put a positive ack. */
	  if (checksum != xmitcsum)
	    {
	      fprintf (stderr, "Warning: Bad RSP checksum: Computed "
		       "0x%02x, received 0x%02x\n", checksum, xmitcsum);

	      put_rsp_char ('-');	/* Failed checksum */
	    }
	  else
	    {
	      put_rsp_char ('+');	/* successful transfer */
	      break;
	    }
	}
      else
	{
	  fprintf (stderr, "Warning: RSP packet overran buffer\n");
	}
    }

  return &buf;				/* Success */

}	/* get_packet () */


/*---------------------------------------------------------------------------*/
/*!Put a single character out onto the client socket

   This should only be called if the client is open, but we check for safety.

   @param[in] c  The character to put out                                    */
/*---------------------------------------------------------------------------*/
static void
put_rsp_char (char  c)
{
  if (-1 == rsp.client_fd)
    {
      fprintf (stderr, "Warning: Attempt to write '%c' to unopened RSP "
	       "client: Ignored\n", c);
      return;
    }

  /* Write until successful (we retry after interrupts) or catastrophic
     failure. */
  while (1)
    {
      switch (write (rsp.client_fd, &c, sizeof (c)))
	{
	case -1:
	  /* Error: only allow interrupts or would block */
	  if ((EAGAIN != errno) && (EINTR != errno))
	    {
	      fprintf (stderr, "Warning: Failed to write to RSP client: "
		       "Closing client connection: %s\n",
		       strerror (errno));
	      rsp_client_close ();
	      return;
	    }
      
	  break;

	case 0:
	  break;		/* Nothing written! Try again */

	default:
	  return;		/* Success, we can return */
	}
    }
}	/* put_rsp_char () */


/*---------------------------------------------------------------------------*/
/*!Get a single character from the client socket

   This should only be called if the client is open, but we check for safety.

   @return  The character read, or -1 on failure                             */
/*---------------------------------------------------------------------------*/
static int
get_rsp_char ()
{
  if (-1 == rsp.client_fd)
    {
      fprintf (stderr, "Warning: Attempt to read from unopened RSP "
	       "client: Ignored\n");
      return  -1;
    }

  /* Non-blocking read until successful (we retry after interrupts) or
     catastrophic failure. */
  while (1)
    {
      unsigned char  c;

      switch (read (rsp.client_fd, &c, sizeof (c)))
	{
	case -1:
	  /* Error: only allow interrupts */
	  if ((EAGAIN != errno) && (EINTR != errno))
	    {
	      fprintf (stderr, "Warning: Failed to read from RSP client: "
		       "Closing client connection: %s\n",
		       strerror (errno));
	      rsp_client_close ();
	      return  -1;
	    }

	  break;

	case 0:
	  // EOF
	  rsp_client_close ();
	  return  -1;

	default:
	  return  c & 0xff; /* Success, we can return (no sign extend!) */
	}
    }
}	/* get_rsp_char () */


/*---------------------------------------------------------------------------*/
/*!"Unescape" RSP binary data

   '#', '$' and '}' are escaped by preceding them by '}' and oring with 0x20.

   This function reverses that, modifying the data in place.

   @param[in] data  The array of bytes to convert
   @para[in]  len   The number of bytes to be converted

   @return  The number of bytes AFTER conversion                             */
/*---------------------------------------------------------------------------*/
static int
rsp_unescape (char *data,
	      int   len)
{
  int  from_off = 0;		/* Offset to source char */
  int  to_off   = 0;		/* Offset to dest char */

  while (from_off < len)
    {
      /* Is it escaped */
      if ( '}' == data[from_off])
	{
	  from_off++;
	  data[to_off] = data[from_off] ^ 0x20;
	}
      else
	{
	  data[to_off] = data[from_off];
	}

      from_off++;
      to_off++;
    }

  return  to_off;

}	/* rsp_unescape () */


/*---------------------------------------------------------------------------*/
/*!Initialize the matchpoint hash table

   This is an open hash table, so this function clears all the links to
   NULL.                                                                     */
/*---------------------------------------------------------------------------*/
static void
mp_hash_init ()
{
  int  i;

  for (i = 0; i < MP_HASH_SIZE; i++)
    {
      rsp.mp_hash[i] = NULL;
    }
}	/* mp_hash_init () */


/*---------------------------------------------------------------------------*/
/*!Add an entry to the matchpoint hash table

   Add the entry if it wasn't already there. If it was there do nothing. The
   match just be on type and addr. The instr need not match, since if this is
   a duplicate insertion (perhaps due to a lost packet) they will be
   different.

   @param[in] type   The type of matchpoint
   @param[in] addr   The address of the matchpoint
   @para[in]  instr  The instruction to associate with the address           */
/*---------------------------------------------------------------------------*/
static void
mp_hash_add (enum mp_type       type,
	     unsigned long int  addr)
{
  int              hv    = addr % MP_HASH_SIZE;
  struct mp_entry *curr;

  /* See if we already have the entry */
  for(curr = rsp.mp_hash[hv]; NULL != curr; curr = curr->next)
    {
      if ((type == curr->type) && (addr == curr->addr))
	{
	  return;		/* We already have the entry */
	}
    }

  /* Insert the new entry at the head of the chain */
  curr = malloc (sizeof (*curr));

  curr->type  = type;
  curr->addr  = addr;
  curr->next  = rsp.mp_hash[hv];

  rsp.mp_hash[hv] = curr;

}	/* mp_hash_add () */


/*---------------------------------------------------------------------------*/
/*!Look up an entry in the matchpoint hash table

   The match must be on type AND addr.

   @param[in] type   The type of matchpoint
   @param[in] addr   The address of the matchpoint

   @return  The entry deleted, or NULL if the entry was not found            */
/*---------------------------------------------------------------------------*/
static struct mp_entry *
mp_hash_lookup (enum mp_type       type,
		unsigned long int  addr)
{
  int              hv   = addr % MP_HASH_SIZE;
  struct mp_entry *curr;

  /* Search */
  for (curr = rsp.mp_hash[hv]; NULL != curr; curr = curr->next)
    {
      if ((type == curr->type) && (addr == curr->addr))
	{
	  return  curr;		/* The entry found */
	}
    }

  /* Not found */
  return  NULL;
      
}	/* mp_hash_lookup () */


/*---------------------------------------------------------------------------*/
/*!Delete an entry from the matchpoint hash table

   If it is there the entry is deleted from the hash table. If it is not
   there, no action is taken. The match must be on type AND addr.

   The usual fun and games tracking the previous entry, so we can delete
   things.

   @note  The deletion DOES NOT free the memory associated with the entry,
          since that is returned. The caller should free the memory when they
          have used the information.

   @param[in] type   The type of matchpoint
   @param[in] addr   The address of the matchpoint

   @return  The entry deleted, or NULL if the entry was not found            */
/*---------------------------------------------------------------------------*/
static struct mp_entry *
mp_hash_delete (enum mp_type       type,
		unsigned long int  addr)
{
  int              hv   = addr % MP_HASH_SIZE;
  struct mp_entry *prev = NULL;
  struct mp_entry *curr;

  /* Search */
  for (curr  = rsp.mp_hash[hv]; NULL != curr; curr = curr->next)
    {
      if ((type == curr->type) && (addr == curr->addr))
	{
	  /* Found - delete. Method depends on whether we are the head of
	     chain. */
	  if (NULL == prev)
	    {
	      rsp.mp_hash[hv] = curr->next;
	    }
	  else
	    {
	      prev->next = curr->next;
	    }

	  return  curr;		/* The entry deleted */
	}

      prev = curr;
    }

  /* Not found */
  return  NULL;
      
}	/* mp_hash_delete () */


/*---------------------------------------------------------------------------*/
/*!Utility to give the value of a hex char

   @param[in] ch  A character representing a hexadecimal digit. Done as -1,
                  for consistency with other character routines, which can use
                  -1 as EOF.

   @return  The value of the hex character, or -1 if the character is
            invalid.                                                         */
/*---------------------------------------------------------------------------*/
static int
hex (int  c)
{
  return  ((c >= 'a') && (c <= 'f')) ? c - 'a' + 10 :
          ((c >= '0') && (c <= '9')) ? c - '0' :
          ((c >= 'A') && (c <= 'F')) ? c - 'A' + 10 : -1;

}	/* hex () */


/*---------------------------------------------------------------------------*/
/*!Convert a register to a hex digit string

   The supplied 32-bit value is converted to an 8 digit hex string according
   the target endianism. It is null terminated for convenient printing.

   @param[in]  val  The value to convert
   @param[out] buf  The buffer for the text string                           */
/*---------------------------------------------------------------------------*/
static void
reg2hex (ARMword  val,
	 char              *buf)
{
  int  n;			/* Counter for digits */

  for (n = 0; n < 8; n++)
    {
#ifdef WORDS_BIGENDIAN
      int  nyb_shift = 28 - (n * 4);
#else
      int  nyb_shift = (n ^ 1) * 4;
#endif
      buf[n] = hexchars[(val >> nyb_shift) & 0xf];
    }

  buf[8] = 0;			/* Useful to terminate as string */

}	/* reg2hex () */


/*---------------------------------------------------------------------------*/
/*!Convert a hex digit string to a register value

   The supplied 8 digit hex string is converted to a 32-bit value according
   the target endianism

   @param[in] buf  The buffer with the hex string

   @return  The value to convert                                             */
/*---------------------------------------------------------------------------*/
static ARMword
hex2reg (char *buf)
{
  int                n;		/* Counter for digits */
  unsigned long int  val = 0;	/* The result */

  for (n = 0; n < 8; n++)
    {
#ifdef WORDS_BIGENDIAN
      int  nyb_shift = 28 - (n * 4);
#else
      int  nyb_shift = (n ^ 1) * 4;
#endif
      val |= hex (buf[n]) << nyb_shift;
    }

  return val;

}	/* hex2reg () */


/*---------------------------------------------------------------------------*/
/*!Set the program counter

   This sets the value in the NPC SPR. Not completely trivial, since this is
   actually cached in cpu_state.pc. Any reset of the NPC also involves
   clearing the delay state and setting the pcnext global.

   Only actually do this if the requested address is different to the current
   NPC (avoids clearing the delay pipe).

   @param[in] addr  The address to use                                       */
/*---------------------------------------------------------------------------*/
static void
set_npc (ARMword addr)
{
  arm_set_pc(addr);
}	/* set_npc () */


/*---------------------------------------------------------------------------*/
/*!Send a packet acknowledging an exception has occurred

   This is only called if there is a client FD to talk to                    */
/*---------------------------------------------------------------------------*/
static void
rsp_report_exception ()
{
  struct rsp_buf  buf;

  /* Construct a signal received packet */
  buf.data[0] = 'S';
  buf.data[1] = hexchars[rsp.sigval >> 4];
  buf.data[2] = hexchars[rsp.sigval % 16];
  buf.data[3] = 0;
  buf.len     = strlen (buf.data);

  put_packet (&buf);

}	/* rsp_report_exception () */


/*---------------------------------------------------------------------------*/
/*!Handle a RSP continue request

   Parse the command to see if there is an address. Uses the underlying
   generic continue function, with EXCEPT_NONE.

   @param[in] buf  The full continue packet                                  */
/*---------------------------------------------------------------------------*/
static void
rsp_continue (struct rsp_buf *buf)
{
  unsigned long int  addr;		/* Address to continue from, if any */

  rsp.single_stepping = 0;

  if (0 == strcmp ("c", buf->data) || 0 == strcmp("vCont;c", buf->data))
    {
    }
  else if (1 != sscanf (buf->data, "c%lx", &addr))
    {
      fprintf (stderr,
	       "Warning: RSP continue address %s not recognized: ignored\n",
	       buf->data);
    }
  else
    {
      set_npc(addr);
    }

  /* Unstall the processor */
  set_stall_state (0);

  /* Note the GDB client is now waiting for a reply. */
  rsp.client_waiting = 1;
}	/* rsp_continue () */

/*---------------------------------------------------------------------------*/
/*!Handle a RSP read all registers request

   The registers follow the GDB sequence for OR1K: GPR0 through GPR31, PPC
   (i.e. SPR PPC), NPC (i.e. SPR NPC) and SR (i.e. SPR SR). Each register is
   returned as a sequence of bytes in target endian order.

   Each byte is packed as a pair of hex digits.                              */
/*---------------------------------------------------------------------------*/
static void
rsp_read_all_regs ()
{
  struct rsp_buf  buf;			/* Buffer for the reply */
  int             r;			/* Register index */

  for (r = 0; r < 15; r++)
    {
      reg2hex (arm_get_reg(r), &(buf.data[r * 8]));
    }

  reg2hex ((arm_get_r15_all() - 8) &  0x03fffffc, &(buf.data[15 * 8]));

  // gdb requests the CPSR separately as the register numbers are not continuous.

  /* Finalize the packet and send it */
  buf.len                = 16 * 8;

  put_packet (&buf);

}	/* rsp_read_all_regs () */


/*---------------------------------------------------------------------------*/
/*!Handle a RSP write all registers request

   The registers follow the GDB sequence for OR1K: GPR0 through GPR31, PPC
   (i.e. SPR PPC), NPC (i.e. SPR NPC) and SR (i.e. SPR SR). Each register is
   supplied as a sequence of bytes in target endian order.

   Each byte is packed as a pair of hex digits.

   @todo There is no error checking at present. Non-hex chars will generate a
         warning message, but there is no other check that the right amount
         of data is present. The result is always "OK".

   @param[in] buf  The original packet request.                              */
/*---------------------------------------------------------------------------*/
static void
rsp_write_all_regs (struct rsp_buf *buf)
{
  int             r;			/* Register index */

  /* The GPRs */
  for (r = 0; r < 15; r++)
    {
      arm_set_reg(r, hex2reg (&(buf->data[r * 8])));
    }

  arm_set_reg(15, hex2reg (&(buf->data[25 * 8])));

  /* Acknowledge. TODO: We always succeed at present, even if the data was
     defective. */
  put_str_packet ("OK");

}	/* rsp_write_all_regs () */


/*---------------------------------------------------------------------------*/
/*!Handle a RSP read memory (symbolic) request

   Syntax is:

     m<addr>,<length>:

   The response is the bytes, lowest address first, encoded as pairs of hex
   digits.

   The length given is the number of bytes to be read.

   @note This function reuses buf, so trashes the original command.

   @param[in] buf  The command received                                      */
/*---------------------------------------------------------------------------*/
static void
rsp_read_mem (struct rsp_buf *buf)
{
  unsigned int    addr;			/* Where to read the memory */
  int             len;			/* Number of bytes to read */
  unsigned int    off;			/* Offset into the memory */

  if (2 != sscanf (buf->data, "m%x,%x:", &addr, &len))
    {
      fprintf (stderr, "Warning: Failed to recognize RSP read memory "
	       "command: %s\n", buf->data);
      put_str_packet ("E01");
      return;
    }

  /* Make sure we won't overflow the buffer (2 chars per byte) */
  if ((len * 2) >= GDB_BUF_MAX)
    {
      fprintf (stderr, "Warning: Memory read %s too large for RSP packet: "
	       "truncated\n", buf->data);
      len = (GDB_BUF_MAX - 1) / 2;
    }

  /* Refill the buffer with the reply */
  for (off = 0; off < len; off++)
    {
      unsigned char  ch;		/* The byte at the address */

      /* Check memory area is valid */
      if (mem_arm_where(addr+off)==0)
	{
	  /* The error number doesn't matter. The GDB client will substitute
	     its own */
	  put_str_packet ("E01");
	  return;
	}

      ch = MEM_READ_BYTE(addr+off);

      buf->data[off * 2]     = hexchars[ch >>   4];
      buf->data[off * 2 + 1] = hexchars[ch &  0xf];
    }

  buf->data[off * 2] = 0;			/* End of string */
  buf->len           = strlen (buf->data);
  put_packet (buf);

}	/* rsp_read_mem () */


/*---------------------------------------------------------------------------*/
/*!Handle a RSP write memory (symbolic) request

   Syntax is:

     m<addr>,<length>:<data>

   The data is the bytes, lowest address first, encoded as pairs of hex
   digits.

   The length given is the number of bytes to be written.

   @note This function reuses buf, so trashes the original command.

   @param[in] buf  The command received                                      */
/*---------------------------------------------------------------------------*/
static void
rsp_write_mem (struct rsp_buf *buf)
{
  unsigned int    addr;			/* Where to write the memory */
  int             len;			/* Number of bytes to write */
  char           *symdat;		/* Pointer to the symboli data */
  int             datlen;		/* Number of digits in symbolic data */
  int             off;			/* Offset into the memory */

  if (2 != sscanf (buf->data, "M%x,%x:", &addr, &len))
    {
      fprintf (stderr, "Warning: Failed to recognize RSP write memory "
	       "command: %s\n", buf->data);
      put_str_packet ("E01");
      return;
    }

  /* Find the start of the data and check there is the amount we expect. */
  symdat = memchr ((const void *)buf->data, ':', GDB_BUF_MAX) + 1;
  datlen = buf->len - (symdat - buf->data);

  /* Sanity check */
  if (len * 2 != datlen)
    {
      fprintf (stderr, "Warning: Write of %d digits requested, but %d digits "
	       "supplied: packet ignored\n", len * 2, datlen );
      put_str_packet ("E01");
      return;
    }

  /* Write the bytes to memory */
  for (off = 0; off < len; off++)
    {
      if (mem_arm_where(addr + off)==0)
	{
	  /* The error number doesn't matter. The GDB client will substitute
	     its own */
	  put_str_packet ("E01");
	  return;
	}
      else
	{
	  unsigned char  nyb1 = hex (symdat[off * 2]);
	  unsigned char  nyb2 = hex (symdat[off * 2 + 1]);

          MEM_WRITE_BYTE(addr+off, (nyb1 << 4) | nyb2);
	}
    }

  put_str_packet ("OK");

}	/* rsp_write_mem () */


/*---------------------------------------------------------------------------*/
/*!Read a single register

   The registers follow the GDB sequence for OR1K: GPR0 through GPR31, PC
   (i.e. SPR NPC) and SR (i.e. SPR SR). The register is returned as a
   sequence of bytes in target endian order.

   Each byte is packed as a pair of hex digits.

   @param[in] buf  The original packet request. Reused for the reply.        */
/*---------------------------------------------------------------------------*/
static void
rsp_read_reg (struct rsp_buf *buf)
{
  unsigned int  regnum;

  /* Break out the fields from the data */
  if (1 != sscanf (buf->data, "p%x", &regnum))
    {
      fprintf (stderr, "Warning: Failed to recognize RSP read register "
	       "command: %s\n", buf->data);
      put_str_packet ("E01");
      return;
    }

  /* Get the relevant register */
  if (regnum < 15)
    {
      reg2hex (arm_get_reg(regnum), buf->data);
    }
  else if (regnum == 25)
    {
      reg2hex (arm_get_r15_all() & 0xfc000003, buf->data);
    }
  else
    {
      /* Error response if we don't know the register */
      fprintf (stderr, "Warning: Attempt to read unknown register 0x%x: "
	       "ignored\n", regnum);
      put_str_packet ("E01");
      return;
    }

  buf->len = strlen (buf->data);
  put_packet (buf);

}	/* rsp_write_reg () */

    
/*---------------------------------------------------------------------------*/
/*!Write a single register

   The registers follow the GDB sequence for OR1K: GPR0 through GPR31, PC
   (i.e. SPR NPC) and SR (i.e. SPR SR). The register is specified as a
   sequence of bytes in target endian order.

   Each byte is packed as a pair of hex digits.

   @param[in] buf  The original packet request.                              */
/*---------------------------------------------------------------------------*/
static void
rsp_write_reg (struct rsp_buf *buf)
{
  unsigned int  regnum;
  char          valstr[9];		/* Allow for EOS on the string */

  /* Break out the fields from the data */
  if (2 != sscanf (buf->data, "P%x=%8s", &regnum, valstr))
    {
      fprintf (stderr, "Warning: Failed to recognize RSP write register "
	       "command: %s\n", buf->data);
      put_str_packet ("E01");
      return;
    }
  
  /* Set the relevant register */
  if (regnum < 16)
    {
      arm_set_reg(regnum, hex2reg(valstr));
    }
  else
    {
      /* Error response if we don't know the register */
      fprintf (stderr, "Warning: Attempt to write unknown register 0x%x: "
	       "ignored\n", regnum);
      put_str_packet ("E01");
      return;
    }

  put_str_packet ("OK");

}	/* rsp_write_reg () */

    
/*---------------------------------------------------------------------------*/
/*!Handle a RSP query request

   @param[in] buf  The request. Reused for any packets that need to be sent
                   back.                                                     */
/*---------------------------------------------------------------------------*/
static void
rsp_query (struct rsp_buf *buf)
{
  if (0 == strcmp ("qAttached", buf->data))
    {
      /* We are always attaching to an existing process with the bare metal
	 embedded system. */
      put_str_packet ("1");
    }
  else if (0 == strcmp ("qC", buf->data))
    {
      /* Return the current thread ID (unsigned hex). A null response
	 indicates to use the previously selected thread. We use the constant
	 OR1KSIM_TID to represent our single thread of control. */
      sprintf (buf->data, "QC%x", OR1KSIM_TID);
      buf->len = strlen (buf->data);
      put_packet (buf);
    }
  else if (0 == strncmp ("qCRC", buf->data, strlen ("qCRC")))
    {
      /* Return CRC of memory area */
      fprintf (stderr, "Warning: RSP CRC query not supported\n");
      put_str_packet ("E01");
    }
  else if (0 == strcmp ("qfThreadInfo", buf->data))
    {
      /* Return info about active threads. We return just the constant
	 OR1KSIM_TID to represent our single thread of control. */
      sprintf (buf->data, "m%x", OR1KSIM_TID);
      buf->len = strlen (buf->data);
      put_packet (buf);
    }
  else if (0 == strcmp ("qsThreadInfo", buf->data))
    {
      /* Return info about more active threads. We have no more, so return the
	 end of list marker, 'l' */
      put_str_packet ("l");
    }
  else if (0 == strncmp ("qGetTLSAddr:", buf->data, strlen ("qGetTLSAddr:")))
    {
      /* We don't support this feature */
      put_str_packet ("");
    }
  else if (0 == strncmp ("qL", buf->data, strlen ("qL")))
    {
      /* Deprecated and replaced by 'qfThreadInfo' */
      fprintf (stderr, "Warning: RSP qL deprecated: no info returned\n");
      put_str_packet ("qM001");
    }
  else if (0 == strcmp ("qOffsets", buf->data))
    {
      /* Report any relocation */
      put_str_packet ("Text=0;Data=0;Bss=0");
    }
  else if (0 == strncmp ("qP", buf->data, strlen ("qP")))
    {
      /* Deprecated and replaced by 'qThreadExtraInfo' */
      fprintf (stderr, "Warning: RSP qP deprecated: no info returned\n");
      put_str_packet ("");
    }
  else if (0 == strncmp ("qSupported", buf->data, strlen ("qSupported")))
    {
      /* Report a list of the features we support. For now we just ignore any
	 supplied specific feature queries, but in the future these may be
	 supported as well. Note that the packet size allows for 'G' + all the
	 registers sent to us, or a reply to 'g' with all the registers and an
	 EOS so the buffer is a well formed string. */

      char  reply[GDB_BUF_MAX];

      sprintf (reply, "PacketSize=%x;xmlRegisters=arm;swbreak-;hwbreak+;qXfer:features:read+;qXfer:auxv:read+;qHostInfo:+", GDB_BUF_MAX);
      put_str_packet (reply);
    }
  else if (0 == strncmp ("qSymbol:", buf->data, strlen ("qSymbol:")))
    {
      /* Offer to look up symbols. Nothing we want (for now). TODO. This just
	 ignores any replies to symbols we looked up, but we didn't want to
	 do that anyway! */
      put_str_packet ("OK");
    }
  else if (0 == strncmp ("qThreadExtraInfo,", buf->data,
			 strlen ("qThreadExtraInfo,")))
    {
      /* Report that we are runnable, but the text must be hex ASCI
	 digits. For now do this by steam, reusing the original packet */
      sprintf (buf->data, "%02x%02x%02x%02x%02x%02x%02x%02x%02x",
	       'R', 'u', 'n', 'n', 'a', 'b', 'l', 'e', 0);
      buf->len = strlen (buf->data);
      put_packet (buf);
    }
  else if (0 == strncmp ("qTStatus", buf->data, strlen ("qTStatus")))  
    {
      /* We don't support tracing, so return empty packet. */
      put_str_packet ("");
    }
  else if (0 == strcmp ("qHostInfo", buf->data))
    {
      put_str_packet("cputype:12;cpusubtype:0;ostype:riscos;triple:arm-unknown-riscos;vendor:acorn;endian:little;ptrsize:4;");
    }
  else if (0 == strncmp ("qXfer:", buf->data, strlen ("qXfer:")))
    {
    unsigned int offset;
    int filesize = -1;
    unsigned int size = -1;

    const char *file;

    if (0 == strncmp ("qXfer:features:read:target.xml:", buf->data, strlen ("qXfer:features:read:target.xml:")) &&
        sscanf(buf->data + strlen ("qXfer:features:read:target.xml:"), "%x,%x", &offset, &size)==2) {
      filesize = _binary_arm_core_xml_end - _binary_arm_core_xml_start;
      file     = _binary_arm_core_xml_start;
    }
    else if (0 == strncmp ("qXfer:auxv:read::", buf->data, strlen ("qXfer:auxv:read::")) &&
        sscanf(buf->data + strlen ("qXfer:auxv:read::"), "%x,%x", &offset, &size)==2) {
      filesize = sizeof(auxv);
      file     = (char *) auxv;
    }

    if (filesize > 0) {
      if (offset >= filesize) offset = filesize;
      if (size >= GDB_BUF_MAX-3) {
          fprintf (stderr, "Warning: RSP 'qXfer' size too big\n");

          put_str_packet ("E01");
      }
      else if (offset == filesize) {
          put_str_packet("l");
      }
      else {
          if (offset+size > filesize) {
              size = filesize - offset;
          }

          struct rsp_buf buf;
          buf.data[0] = 'm';
          assert(size+1 <= sizeof(buf.data));
          memcpy(buf.data+1, file + offset, size);
          buf.data[size+1] = 0;
          buf.len = size+1;
          put_packet(&buf);
      }
    }
    else {
      fprintf (stderr, "Warning: RSP 'qXfer' bad: ignored\n");

      put_str_packet ("");
    }
  }
  else
    {
      fprintf (stderr, "Unrecognized RSP query: ignored\n");
    }
}	/* rsp_query () */


/*---------------------------------------------------------------------------*/
/*!Handle a RSP set request

   @param[in] buf  The request                                               */
/*---------------------------------------------------------------------------*/
static void
rsp_set (struct rsp_buf *buf)
{
  if (0 == strncmp ("QPassSignals:", buf->data, strlen ("QPassSignals:")))
    {
      /* Passing signals not supported */
      put_str_packet ("");
    }
  else if ((0 == strncmp ("QTDP",    buf->data, strlen ("QTDP")))   ||
	   (0 == strncmp ("QFrame",  buf->data, strlen ("QFrame"))) ||
	   (0 == strcmp  ("QTStart", buf->data))                    ||
	   (0 == strcmp  ("QTStop",  buf->data))                    ||
	   (0 == strcmp  ("QTinit",  buf->data))                    ||
	   (0 == strcmp  ("QStartNoAckMode",  buf->data))           ||
	   (0 == strncmp ("QTro",    buf->data, strlen ("QTro"))))
    {
      /* All tracepoint features are not supported. This reply is really only
	 needed to 'QTDP', since with that the others should not be
	 generated. */
      put_str_packet ("");
    }
  else
    {
      fprintf (stderr, "Unrecognized RSP set request '%s': ignored\n", buf->data);
    }
}	/* rsp_set () */


/*---------------------------------------------------------------------------*/
/*!Handle a RSP step request

   Parse the command to see if there is an address. Uses the underlying
   generic step function, with EXCEPT_NONE.

   @param[in] buf  The full step packet                          */
/*---------------------------------------------------------------------------*/
static void
rsp_step (struct rsp_buf *buf)
{
  unsigned long int  addr;		/* The address to step from, if any */

  rsp.single_stepping = 1;

  if (0 == strcmp ("s", buf->data) || 0 == strcmp("i", buf->data) || 0 == strcmp("vCont;s", buf->data) || 0 == strcmp("vCont;s:1", buf->data))
    {
    }
  else if (1 != sscanf (buf->data, "s%lx", &addr))
    {
      fprintf (stderr,
	       "Warning: RSP step address %s not recognized: ignored\n",
	       buf->data);
    }
  else
    {
      set_npc(addr);
    }
  /* Unstall the processor */
  set_stall_state (0);

  /* Note the GDB client is now waiting for a reply. */
  rsp.client_waiting = 1;

}	/* rsp_step () */


/*---------------------------------------------------------------------------*/
/*!Handle a RSP 'v' packet

   These are commands associated with executing the code on the target

   @param[in] buf  The request                                               */
/*---------------------------------------------------------------------------*/
static void
rsp_vpkt (struct rsp_buf *buf)
{
  if (0 == strncmp ("vAttach;", buf->data, strlen ("vAttach;")))
    {
      /* Attaching is a null action, since we have no other process. We just
	 return a stop packet (using TRAP) to indicate we are stopped. */
      put_str_packet ("S05");
      return;
    }
  else if (0 == strcmp ("vCont?", buf->data))
    {
      /* For now we don't support this. */
      put_str_packet ("vCont;c;s");
      return;
    }
  else if (0 == strcmp ("vCont;s", buf->data) || 0 == strcmp ("vCont;s:1", buf->data))
    {
      rsp_step(buf);
      return;
    }
  else if (0 == strcmp ("vCont;c", buf->data) || 0 ==strcmp ("vCont;c:1", buf->data))
    {
      rsp_continue(buf);
      return;
    }
  else if (0 == strncmp ("vCont", buf->data, strlen ("vCont")))
    {
      /* This shouldn't happen, because we've reported non-support via vCont?
	 above */
      fprintf (stderr, "Warning: RSP vCont not supported\n" );
      put_str_packet("E01");
      return;
    }
  else if (0 == strncmp ("vFile:", buf->data, strlen ("vFile:")))
    {
      /* For now we don't support this. */
      fprintf (stderr, "Warning: RSP vFile not supported: ignored\n" );
      put_str_packet ("");
      return;
    }
  else if (0 == strncmp ("vFlashErase:", buf->data, strlen ("vFlashErase:")))
    {
      /* For now we don't support this. */
      fprintf (stderr, "Warning: RSP vFlashErase not supported: ignored\n" );
      put_str_packet ("E01");
      return;
    }
  else if (0 == strncmp ("vFlashWrite:", buf->data, strlen ("vFlashWrite:")))
    {
      /* For now we don't support this. */
      fprintf (stderr, "Warning: RSP vFlashWrite not supported: ignored\n" );
      put_str_packet ("E01");
      return;
    }
  else if (0 == strcmp ("vFlashDone", buf->data))
    {
      /* For now we don't support this. */
      fprintf (stderr, "Warning: RSP vFlashDone not supported: ignored\n" );
      put_str_packet ("E01");
      return;
    }
  else
    {
      /* GDB expects an empty response for unknown 'v' packets. */
      if (0 != strcmp ("vMustReplyEmpty", buf->data))
	{
	  fprintf (stderr, "Warning: Unknown RSP 'v' packet type %s: ignored\n",
		   buf->data);
	}
      put_str_packet ("");
      return;
    }
}	/* rsp_vpkt () */


/*---------------------------------------------------------------------------*/
/*!Handle a RSP write memory (binary) request

   Syntax is:

     X<addr>,<length>:

   Followed by the specified number of bytes as raw binary. Response should be
   "OK" if all copied OK, E<nn> if error <nn> has occurred.

   The length given is the number of bytes to be written. However the number
   of data bytes may be greater, since '#', '$' and '}' are escaped by
   preceding them by '}' and oring with 0x20.

   @param[in] buf  The command received                                      */
/*---------------------------------------------------------------------------*/
static void
rsp_write_mem_bin (struct rsp_buf *buf)
{
  unsigned int  addr;			/* Where to write the memory */
  int           len;			/* Number of bytes to write */
  char         *bindat;			/* Pointer to the binary data */
  int           off;			/* Offset to start of binary data */
  int           newlen;			/* Number of bytes in bin data */

  if (2 != sscanf (buf->data, "X%x,%x:", &addr, &len))
    {
      fprintf (stderr, "Warning: Failed to recognize RSP write memory "
	       "command: %s\n", buf->data);
      put_str_packet ("E01");
      return;
    }

  /* Find the start of the data and "unescape" it */
  bindat = memchr ((const void *)buf->data, ':', GDB_BUF_MAX) + 1;
  off    = bindat - buf->data;
  newlen = rsp_unescape (bindat, buf->len - off);

  /* Sanity check */
  if (newlen != len)
    {
      int  minlen = len < newlen ? len : newlen;

      fprintf (stderr, "Warning: Write of %d bytes requested, but %d bytes "
	       "supplied. %d will be written\n", len, newlen, minlen);
      len = minlen;
    }

  /* Write the bytes to memory */
  for (off = 0; off < len; off++)
    {
      if (mem_arm_where(addr + off)==0)
	{
	  /* The error number doesn't matter. The GDB client will substitute
	     its own */
	  put_str_packet ("E01");
	  return;
	}
      else
	{
          MEM_WRITE_BYTE(addr + off, bindat[off]);
	}
    }

  put_str_packet ("OK");

}	/* rsp_write_mem_bin () */

      
/*---------------------------------------------------------------------------*/
/*!Handle a RSP remove breakpoint or matchpoint request

   For now only memory breakpoints are implemented, which are implemented by
   substituting a breakpoint at the specified address. The implementation must
   cope with the possibility of duplicate packets.

   @todo This doesn't work with icache/immu yet

   @param[in] buf  The command received                                      */
/*---------------------------------------------------------------------------*/
static void
rsp_remove_matchpoint (struct rsp_buf *buf)
{
  enum mp_type       type;		/* What sort of matchpoint */
  int                type_for_scanf;	/* To avoid old GCC limitations */
  unsigned long int  addr;		/* Address specified */
  int                len;		/* Matchpoint length (not used) */

  /* Break out the instruction. We have to use an intermediary for the type,
     since older GCCs do not like taking the address of an enum
     (dereferencing type-punned pointer). */
  if (3 != sscanf (buf->data, "z%1d,%lx,%1d", &type_for_scanf, &addr, &len))
    {
      fprintf (stderr, "Warning: RSP matchpoint deletion request not "
	       "recognized: ignored\n");
      put_str_packet ("E01");
      return;
    }

  type = type_for_scanf;

  /* Sanity check that the length is 4 */
  if (4 != len)
    {
      fprintf (stderr, "Warning: RSP matchpoint deletion length %d not "
	       "valid: 4 assumed\n", len);
      len = 4;
    }

  /* Sort out the type of matchpoint */
  switch (type)
    {
    case BP_HARDWARE:
    case WP_WRITE:
    case WP_READ:
    case WP_ACCESS:
      mp_hash_delete(type, addr);
      put_str_packet ("OK");
      return;

    default:
      fprintf (stderr, "Warning: RSP matchpoint type %d not "
	       "recognized: ignored\n", type);
      put_str_packet ("E01");
      return;

    }
}	/* rsp_remove_matchpoint () */

      
/*---------------------------------------------------------------------------*/
/*!Handle a RSP insert breakpoint or matchpoint request

   For now only memory breakpoints are implemented, which are implemented by
   substituting a breakpoint at the specified address. The implementation must
   cope with the possibility of duplicate packets.

   @todo This doesn't work with icache/immu yet

   @param[in] buf  The command received                                      */
/*---------------------------------------------------------------------------*/
static void
rsp_insert_matchpoint (struct rsp_buf *buf)
{
  enum mp_type       type;		/* What sort of matchpoint */
  int                type_for_scanf;	/* To avoid old GCC limitations */
  unsigned long int  addr;		/* Address specified */
  int                len;		/* Matchpoint length (not used) */

  /* Break out the instruction. We have to use an intermediary for the type,
     since older GCCs do not like taking the address of an enum
     (dereferencing type-punned pointer). */
  if (3 != sscanf (buf->data, "Z%1d,%lx,%1d", &type_for_scanf, &addr, &len))
    {
      fprintf (stderr, "Warning: RSP matchpoint insertion request not "
	       "recognized: ignored\n");
      put_str_packet ("E01");
      return;
    }

  type = type_for_scanf;

  /* Sanity check that the length is 4 */
  if (4 != len)
    {
      fprintf (stderr, "Warning: RSP matchpoint insertion length %d not "
	       "valid: 4 assumed\n", len);
      len = 4;
    }

  /* Sort out the type of matchpoint */
  switch (type)
    {
    case BP_HARDWARE:
    case WP_WRITE:
    case WP_READ:
    case WP_ACCESS:
      mp_hash_add(type, addr);
      put_str_packet("OK");
      return;

    default:
      // gdb docs say that we should return a black response for an unsupported
      // breakpoint type. However, if we do that it attempts to inject a
      // software breakpoint by writing to memory. This ends badly for us
      // because we simulate the ARM's pipeline, so both setting the breakpoint
      // and removing it afterwards can fail to take effect.
      fprintf (stderr, "Warning: RSP matchpoint type %d not "
	       "recognized: ignored\n", type);
      put_str_packet ("E01");
      return;

    }

}	/* rsp_insert_matchpoint () */

extern void  rsp_check_instruction ( ARMword pc ) {
    if (rsp.single_stepping) {
        return handle_rsp();
    }
    else {
        struct mp_entry *m = mp_hash_lookup( BP_HARDWARE, pc );
        if (m) {
            handle_rsp();
        }
    }
}

extern void  rsp_check_memory_read( ARMword addr ) {
    int m = mp_hash_lookup( WP_READ, addr )
         || mp_hash_lookup( WP_ACCESS, addr );
    if (m) {
        handle_rsp();
    }
}

extern void  rsp_check_memory_write( ARMword addr ) {
    int m = mp_hash_lookup( WP_WRITE, addr )
         || mp_hash_lookup( WP_ACCESS, addr );
    if (m) {
        handle_rsp();
    }
}
