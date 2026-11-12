// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/player_volume_fader.h: the prototypes
// of Black's named functions and the types they need.
#ifndef BWPORT_SYSTEM_PLAYER_VOLUME_FADER_H
#define BWPORT_SYSTEM_PLAYER_VOLUME_FADER_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

PlayerVolumeFader *PlayerVolumeFader_Create(HeapID heapId, u8 player);
// Fades to volume over duration frames, or sets it at once if duration is 0
void PlayerVolumeFader_SetFade(PlayerVolumeFader *fader, u8 volume, u16 duration);

// pokebw2's PlayerVolumeFader_SetMuted
// While muted, the player's volume is 0, and the fade goes on
void func_020287a8(PlayerVolumeFader *fader, u8 muted);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void PlayerVolumeFader_Free(PlayerVolumeFader *fader);
// Advances the fade a frame; called every frame
void PlayerVolumeFader_Update(PlayerVolumeFader *fader);

#endif // BWPORT_SYSTEM_PLAYER_VOLUME_FADER_H
