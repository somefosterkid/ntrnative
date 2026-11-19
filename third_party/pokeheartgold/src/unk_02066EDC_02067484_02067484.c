#include "global.h"

#include "field_system.h"
#include "field_types_def.h"
#include "scrcmd_9.h"
#include "unk_02066EDC.h"

void sub_02067484(FieldSystem *fieldSystem, void *arg1) {
    FrontierFieldSystem *frontierFieldSystem;

    frontierFieldSystem = fieldSystem->frontierFsys;
    frontierFieldSystem->unk24 += *(s32 *)arg1;
    frontierFieldSystem->unk28 += *(s32 *)((u8 *)arg1 + 4);
    frontierFieldSystem->unk26 += *(s32 *)((u8 *)arg1 + 8);
}
