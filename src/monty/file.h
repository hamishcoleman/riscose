/* file.h. */

/* $Id$ */

/* returns true if path exists and it is a regular file. */
int file_exists(char *path);

/* searches path looking for file.  returns NULL if not found, else the
 * full path to file which should be efree'd. */
char *search_path(char **path, char *file);

/* returns a malloc'd string containing a temporary file name based
 * around our progname and process id and containing use. */
char *temp_file(char *use);

/* returns a malloc'd string containing the directory name in path
 * or an empty string if there isn't one. */
char *dir_of_file(char *path);

/* returns a malloc'd lump of memory containing the contents of path of
 * length *len. */
char *file_read(char *path, int *len);

/* open path for reading.  if path looks like a compressed file then
 * popen is called and *ispipe will be true meaning pclose should be
 * called instead of fclose.  an error will cause NULL to be returned. */
FILE *fopenrz(char *path, int *ispipe);

/* unlink path, checking error return. */
int eunlink(char *path);

/* open a file for reading. */
FILE *efopenr(char *path);

/* open a file for writing. */
FILE *efopenw(char *path);

/* close a file. */
void efclose(FILE *f);
