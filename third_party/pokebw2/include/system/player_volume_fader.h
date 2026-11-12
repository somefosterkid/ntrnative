#ifndef POKEBW2_SYSTEM_PLAYER_VOLUME_FADER_H
#define POKEBW2_SYSTEM_PLAYER_VOLUME_FADER_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// Fades a sound player's volume to a target over a number of frames (player_volume_fader.c), for the field's sound
// system. Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0), except PlayerVolumeFader_SetMuted

PlayerVolumeFader *PlayerVolumeFader_Create(HeapID heapId, u8 player);
void PlayerVolumeFader_Free(PlayerVolumeFader *fader);
// Advances the fade a frame; called every frame
void PlayerVolumeFader_Update(PlayerVolumeFader *fader);
// Fades to volume over duration frames, or sets it at once if duration is 0
void PlayerVolumeFader_SetFade(PlayerVolumeFader *fader, u8 volume, u16 duration);
// While muted, the player's volume is 0, and the fade goes on
void PlayerVolumeFader_SetMuted(PlayerVolumeFader *fader, u8 muted);

#endif // POKEBW2_SYSTEM_PLAYER_VOLUME_FADER_H
