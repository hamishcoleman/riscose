/* file.c. */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>

#include "config.h"
#include "monty.h"
#include "mem.h"
#include "array.h"
#include "str.h"
#include "file.h"

/* ------------------------------------------------------------------ */

int file_exists(char *path, struct stat *stp)
{
    struct stat st;
    int exists;

    if (!stp) {
        stp = &st;
    }
    exists = stat(path, stp) == 0 && S_ISREG(stp->st_mode);

    DEBUG(FILE, ("file_exists(\"%s\") returns %d\n", path, exists));

    return exists;
}

/* ------------------------------------------------------------------ */

char *search_path(char **path, char *file)
{
    char *try;

    DEBUG(FILE, ("search_path(%x, \"%s\")\n", path, file));

    while (*path) {
        try = mstrprintf("%s/%s", **path ? *path : ".", file);
        if (file_exists(try, NULL)) {
            return try;
        }
        efree(try);
        path++;
    }

    return NULL;
}

/* ------------------------------------------------------------------ */

char *temp_file(char *use)
{
    static char *dir;
    static char pid[7];
    char *tmp;

    if (dir == NULL) {
        if ((dir = getenv("TMPDIR")) == NULL) {
            dir = "/tmp";
        }
        sprintf(pid, "%06d", getpid());
    }

    tmp = mstrprintf("%s/%s.%s.%s", dir, progname, pid, use);

    DEBUG(FILE, ("temp_file(\"%s\") returns \"%s\"\n", use, tmp));

    return tmp;
}

/* ------------------------------------------------------------------ */

char *dir_of_file(char *path)
{
    char *slash;
    int len;
    char *dir;

    slash = strrchr(path, '/');
    len = slash ? slash - path : 0;
    dir = ememdup(path, len + 1);
    dir[len] = '\0';

    DEBUG(FILE, ("dir_of_file(\"%s\") returns \"%s\"\n", path, dir));

    return dir;
}

/* ------------------------------------------------------------------ */

char *file_read(char *path, int *len)
{
    struct stat st;
    char *buf;
    FILE *fp;

    if (stat(path, &st)) {
        error("file_read: couldn't stat %s: %s\n", path, strerror(errno));
    }
    if (!S_ISREG(st.st_mode)) {
        error("file_read: not a plain file: %s\n", path);
    }

    fp = efopenr(path);
    buf = emalloc(*len = st.st_size);
    if (fread(buf, *len, 1, fp) != 1) {
        error("file_read: couldn't read %u bytes from %s: %s\n", *len,
            path, strerror(errno));
    }
    efclose(fp);

    DEBUG(FILE, ("file_read(\"%s\", %u) returns %#x\n", path, *len, buf));

    return buf;
}

/* ------------------------------------------------------------------ */

FILE *fopenrz(char *path, int *ispipe)
{
    typedef struct {
        char *suffix;
        char *zcat;
    } compressor;
    static compressor catter[] = {
        {".Z", "compress -dc"},
        {".gz", "GZIP= gzip -dc"},
        {".bz2", "bzip2 -dc"},
        {".lzo", "lzop -dc"},
    };
    char *end;
    compressor *cat;
    char *command;
    FILE *fp;

    end = path + strlen(path);
    for (cat = catter; cat < END(catter); cat++) {
        if (!strcmp(end - strlen(cat->suffix), cat->suffix)) {
            *ispipe = TRUE;
            command = mstrprintf("%s '%s'", cat->zcat, path);
            fp = popen(command, "r");
            DEBUG(FILE, ("fopenrz(\"%s\") -> popen(\"%s\") returns "
                "%#x\n", path, command, fp));
            efree(command);

            return fp;
        }
    }

    *ispipe = FALSE;
    fp = fopen(path, "r");
    DEBUG(FILE, ("fopenrz(\"%s\") -> fopen() returns %#x\n",
        path, fp));

    return fp;
}

/* ------------------------------------------------------------------ */

int eunlink(char *path)
{
    if (unlink(path)) {
        error("couldn't unlink %s\n", path);
    }

    return 0;
}

/* ------------------------------------------------------------------ */

FILE *efopenr(char *path)
{
    FILE *f;

    if ((f = fopen(path, "r")) == NULL) {
        error("couldn't open %s for reading\n", path);
    }

    return f;
}

/* ------------------------------------------------------------------ */

FILE *efopenw(char *path)
{
    FILE *f;

    if ((f = fopen(path, "w")) == NULL) {
        error("couldn't open %s for writing\n", path);
    }

    return f;
}

/* ------------------------------------------------------------------ */

void efclose(FILE *f)
{
    if (fclose(f)) {
        error("error closing file\n");
    }

    return;
}
