// The PC build compiles src/ov021/fieldmap_event.c through this file: its check of the player's command (a trigger
// that fires when the player runs onto it) sees the game's command for the port's faster steps
// (port/extern/src/ov021/fldmmdl_acmd.c), so a faster run still counts as running.
#define FldAct_GetAcmd PortAcmd_GetGameAcmd
#include "../../../../src/ov021/fieldmap_event.c"
