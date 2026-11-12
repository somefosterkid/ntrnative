// The PC build compiles src/ov021/fldmmdl_movecode.c through this file: its check of the player's command (an NPC
// that reacts to the player running) sees the game's command for the port's faster steps
// (port/extern/src/ov021/fldmmdl_acmd.c), so a faster run still counts as running.
#define FldAct_GetAcmd PortAcmd_GetGameAcmd
#include "../../../../src/ov021/fldmmdl_movecode.c"
