/* UtilityModule.c
**
** (c) Matthew Bloch 2000
**
** See http://riscose.sourceforge.net/ for terms of distribution, and to
** pick up a later version of the software.
**
**   Module emulation for riscose
**
**   $Revision$
**   $Date$
*/
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <readline/readline.h>
#include <gdbm.h>
#include <getopt.h>
#include <stdlib.h>

#ifndef TEMPDIR
#  define TEMPDIR "/tmp"
#endif

char *prompt = "*\0";

GDBM_FILE cmds;

void
cli_init(void)
{
  char filename[256] = TEMPDIR;
  
  sprintf(filename, "%s/riscose.cli.%d", TEMPDIR, getppid());

  cmds = gdbm_open(filename, 4096, GDBM_NEWDB | GDBM_FAST,
                                      S_IRUSR | S_IWUSR, NULL);
  if (!cmds)
    {
     perror("gdbm_open cli");
     exit(1);
    }
  unlink(filename);  
}

void
cli_parse(char *line)
{
/*  char **argv, argc, argc_max;
  int c, found;
  
  argc_max = 16;
  argc = 0;
  argv = NULL;
  found = 0;
  
  for (c=0; line[c] != 0; c++)
    {
     if (line[c] == 32)
     if (line[c] == 32));
		memcpy(fpu,&tsk
     argv = realloc(argv, argc_max * sizeof(char*));
     
    }*/
}

void cmd_gos(void)
{
  printf("Supervisor\n");
  while(1)
    {
     char *line = readline(prompt);
     if (line && *line)
      {
       /*add_history(line);*/
       cli_parse(line);
      }
     free(line);
    }
}

void cmd_set(void)
{
  
}