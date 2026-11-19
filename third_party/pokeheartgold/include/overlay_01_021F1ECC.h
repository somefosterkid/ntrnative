#ifndef POKEHEARTGOLD_OVERLAY_01_021F1ECC_H
#define POKEHEARTGOLD_OVERLAY_01_021F1ECC_H

#include "field_types_def.h"
#include "player_avatar.h"
#include "task.h"

// Environment of the ov01_021F1ECC field task (ov01_021F1EA4 allocates it; DS size 0x14, pointers are 8 bytes on the host).
typedef struct UnkStruct_ov01_021F1ECC {
    int state;
    int dir; // direction handed to ov01_021F1EA4; the movement actions face it
    FieldSystem *fieldSystem;
    PlayerAvatar *playerAvatar;
    LocalMapObject *object;
} UnkStruct_ov01_021F1ECC;

void ov01_021F1EA4(int dir, FieldSystem *fieldSystem, PlayerAvatar *playerAvatar, LocalMapObject *object);
BOOL ov01_021F1ECC(TaskManager *taskManager);
s32 ov01_021F2004(TaskManager *taskManager, FieldSystem *fieldSystem, LocalMapObject *object);

#endif // POKEHEARTGOLD_OVERLAY_01_021F1ECC_H
