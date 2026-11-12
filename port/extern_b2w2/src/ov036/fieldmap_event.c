// Black 2's and White 2's twin of port/extern/src/ov021/fieldmap_event.c: pokebw2's src/ov036/fieldmap_event.c compiled through this file,
// its check of the player's command (a trigger that fires when the player runs onto it) sees the game's command for the port's faster steps (port/extern_b2w2/src/ov036/field_actor_acmd.c), so a
// faster run still counts as running.
#define FldAct_GetAcmd PortAcmd_GetGameAcmd
#include "../../../../third_party/pokebw2/src/ov036/fieldmap_event.c"
