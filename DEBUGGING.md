# Debugging

riscose provides a remote gdb interface for debugging code running on the
emulated ARM. It supports;

 * Hardware breakpoints
 * Simple watchpoints
 * Read and writing memory
 * Faking a CPSR so 26 bit ARM code can be debugged without debugger support.

It announces itself as much as the protocol allows, although your debugger is
probably expecting an ELF executable, not raw 26 bit ARM code that ignores
APCS.

To run the debugger, supply `-g`, and it will listen for a connection on port
53415.

## gdb

riscose not appear to work with the gdb distributed by Ubuntu 22.04, but does work
with current git master branch. (Version 12.1 attempts to use software
breakpoints for single stepping, which are not implemented.)

To connect type the command, `target remote tcp:127.0.0.1:53415`. Breakpoints
default to software breakpoints, you need to explicitly use hardware
breakpoints. eg., `hb *0x8000`.

## lldb

lldb can connect to riscose, but is slow due to unimplemented commands. To
connect use `gdb-remote 127.0.0.1:53415`. lldb ignores software breakpoints
being disavowed so you to type
`settings set target.require-hardware-breakpoint true` before using
breakpoints.

# Future support

 * Breakpointing SWIs.
 * Presenting ELF executables to gdb as such.
 * Translating ASD debug tables into something gdb can follow.
