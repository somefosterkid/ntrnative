#include "global.h"

#include "field_system.h"
#include "field_types_def.h"

void ov35_02259D80(void *poketchSys, u32 cmd);

void ov01_021F6830(FieldSystem *fieldSystem, s32 a1, s32 a2);

void ov01_021F6830(FieldSystem *fieldSystem, s32 a1, s32 a2) {
    if (fieldSystem->unk4->unk14 != 0) {
        ov35_02259D80(fieldSystem->unk4->unk14, a1);
    }
    return;
}
