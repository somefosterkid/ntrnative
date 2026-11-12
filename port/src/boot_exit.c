// The game's exit(): MSL's as Black has it (lib/msl/src/exit.c, not compiled for PC) only keeps whether its status was a
// failure and returns. GFL_HeapDumpOnFailure (lib/gfl/src/heap.c) calls it where a debug build stopped: a heap deleted
// with blocks still allocated (the intro's heap at its end), a failed allocation. The host's exit() ended the program
// there; port_prefix.h makes the game's calls this one, which says so on stderr and returns as on the DS.
#include <stdio.h>

int PORT_MslExitFailed;

void PORT_MslExit(int status) {
    PORT_MslExitFailed = status != 0;
    fprintf(stderr, "bwport: the game's exit(%d) (MSL's: it returns)\n", status);
}
