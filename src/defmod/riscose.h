/* defmod/riscose.h
**
** See http://riscose.sourceforge.net/ for terms of distribution, and to
** pick up a later version of the software.
**
**   riscose-specific bits of defmod that are used in more than one backend.
*/



/* Return TRUE if the given "SWI" name is actually a SWI rather than an upcall/event/
** etc. etc.
*/
extern osbool is_swi(char* s);



/* Given a context into a list of SWIs, check the next SWI to see if it is
** a "wide" version of the current one.  If so, update swi, s and context to
** look at the wide version rather than the non-wide one.  Otherwise leave
** everything unchanged.
*/
extern void check_for_wide_version(lookup_t swis, char** swi, def_s* s, void** context);
