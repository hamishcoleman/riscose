/* Copyright 1998 Acorn Computers Ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/*
 * unmodsqz - reverses the effect of modsqz
 *
 * Copyright (C) Acorn Computers Ltd. 1998
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include "VersionNum.h"
//#include "CLX/bytesex.h"
#include "CLX/err.h"
#include "CLX/prgname.h"
//#include "CLX/wholefls.h"
#include "unmodsqz.h"

#ifdef __riscos
#define TARGET_IS_RISCOS
#endif

#ifdef TARGET_IS_RISCOS
#include "kernel.h"
#include "swis.h"
#endif

#define Program_Title "unmodsqz"

static size_t wf_filesize(const char *filename) {
    struct stat statbuf;
    if (stat(filename, &statbuf)==0) {
        return statbuf.st_size;
    }
    else {
        fprintf(stderr, "cannot open '%s'\n", filename);
        abort();
    }
}

static int wf_load(const char *filename, void *ptr, size_t size) {
    int fd;
    fd = open(filename, O_RDONLY);
    if (fd<0) {
        return -1;
    }
    else {
        if (read(fd, ptr, size)==size) {
            return 0;
        }
        else {
            return -1;
        }
    }
}

static int wf_save(const char *filename, void *ptr, size_t size) {
    int fd;
    fd = open(filename, O_CREAT | O_WRONLY, 0666);
    if (fd<0) {
        return -1;
    }
    else {
        if (write(fd, ptr, size)==size) {
            return 0;
        }
        else {
            return -1;
        }
    }
}

static int verbose = 0;

/* Error messages */
static char *errors[] = {
  "",
  "This module is not squeezed",
  "Out of memory",
  "internal error: decode failed",
  "missing filename",
  "unrecognised option '%c'",
  "too many parameters",
  "not a relocatable module",
  "Unable to load input file %s",
  "Unable to save output file %s",
  "missing filenames"
};

static void banner(const char *prog)
{
  fprintf(stderr,
    "Module Unsqueeze Utility version " Module_MajorVersion " ["
     Module_Date "] " Module_MinorVersion "\n\n"
    "Usage: %s [options] infile [outfile]\n\n"
    "Options:\n"
    "  -o output    Writes unsqueezed module to output\n"
    "  -v           Verbose\n"
    "  -h -?        Display help message\n",
    prog);
   exit(EXIT_FAILURE);
}

static int32_t file_length(char *filename, int32_t *load, int32_t *exec, int32_t *attr)
{
#ifdef TARGET_IS_RISCOS
        _kernel_oserror *e;
        int size;
        int objtype, filetype;
        e = _swix(OS_File, _INR(0,1)|_OUT(0)|_OUTR(2,6), 20, filename,
                &objtype, load, exec, &size, attr, &filetype);
        if (e != NULL) {
                err_fail("%s", e->errmess);
        }
        else {
                if (!(objtype & 1) ||  filetype != 0xFFA) {
                        err_fail(errors[errcode_NOT_MODULE]);
                }
        }
        return size;
#else
        int32_t size = wf_filesize(filename);
        *load = *exec = *attr = 0;
        if (size == -1) {
          err_fail(errors[errcode_NO_LOAD], filename);
        }
        return size;
#endif
}

#ifdef TARGET_IS_RISCOS
static void restamp(const char *filename, int32_t load, int32_t exec, int32_t attr)
{
        load |= 0xFFFFFA00;
        (void) _swix(OS_File, _INR(0,3)|_IN(5), 1, filename, load, exec, attr);
}
#endif

int main(int argc, char *argv[])
{
  int32_t load, exec, attr, size, realsize;
  void  *realdata;
  void  *data;
  char *outfile = 0, *infile = 0;
  int ac;
  char *arg;
  error_code e;
  char program[32];

  err_init(program_name(argv[0], program, sizeof(program)));

  for (arg=argv[ac=1]; ac<argc; arg=argv[++ac]) if (arg[0] == '-') {
    while (*++arg) switch (*arg) {
      case 'v':
        ++verbose;
        break;
      case 'o':
        if ((ac+1)>=argc) err_fail(errors[errcode_NO_FILENAME]);
        if (outfile) err_fail(errors[errcode_BAD_PARAMETERS]);
        outfile = argv[++ac];
        break;
      case 'h':
      case '?':
        banner(program);
        break;
      default:
        err_fail(errors[errcode_BAD_OPTION], *arg);
        break;
    }
  }
  else {
    if (!infile) infile = arg;
    else if (!outfile) outfile = arg;
    else err_fail(errors[errcode_BAD_PARAMETERS]);
  }

  if (!infile) err_fail(errors[errcode_NO_PARAMS]);
  if (!outfile) outfile = infile;

  size = file_length(infile, &load, &exec, &attr);
  if (verbose) err_report("-- original file is %ld bytes", size);

  data = malloc((size_t) size);
  if (data == NULL) err_fail(errors[errcode_NO_MEM]);
  if (-1 == wf_load(infile, data, size)) err_fail(errors[errcode_NO_LOAD], infile);

  e = unsqueeze_module((void *)data, &realdata, (size_t *) &realsize);
  if (e == errcode_NOT_SQUEEZED) {
    if (verbose && outfile != infile) {
      err_report("-- source was not squeezed - module copied");
    }
    realdata = data;
    realsize = size;
  }
  else if (e != ok) {
    err_fail(errors[e]);
  }
  else {
    free(data);
  }

  if (-1 == wf_save(outfile, realdata, realsize)) {
    err_fail(errors[errcode_NO_SAVE], outfile);
  }

#ifdef TARGET_IS_RISCOS
  restamp(outfile, load, exec, attr);
#endif

  if (verbose) err_report("-- output size %ld bytes", realsize);

  return EXIT_SUCCESS;
}
