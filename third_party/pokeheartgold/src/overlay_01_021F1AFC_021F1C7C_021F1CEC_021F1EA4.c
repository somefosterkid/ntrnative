#include "global.h"

#include "overlay_01_021F1ECC.h"

void *ov01_021F30D0(u32 size);

void ov01_021F1EA4(int dir, FieldSystem *fieldSystem, PlayerAvatar *playerAvatar, LocalMapObject *object) {
    UnkStruct_ov01_021F1ECC *env;

    env = ov01_021F30D0(sizeof(UnkStruct_ov01_021F1ECC));
    env->dir = dir;
    env->fieldSystem = fieldSystem;
    env->object = object;
    env->playerAvatar = playerAvatar;
    FieldSystem_CreateTask(fieldSystem, ov01_021F1ECC, env);
    return;
}
