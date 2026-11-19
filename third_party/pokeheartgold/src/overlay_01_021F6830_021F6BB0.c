#include "global.h"
struct UnkStruct_ov5_021EAE78;

#include "overlay_01.h"
#include "overlay_01_unk_struct_ov5_021_eae78.h"

extern u32 ov01_02206C08;

u32 ov01_021F6BB0(u32 param_1);

s32 ov01_021F6BD0(s32 arg0);
s32 ov01_021F6C28(struct UnkStruct_ov5_021EAE78 *arg0);
s32 ov01_021F6C4C(void *arg0);
s32 ov01_021F6C7C(struct UnkStruct_ov5_021EAE78 *arg0);

u32 ov01_021F6BB0(u32 param_1) {
    u32 uVar1;
    u16 *puVar2;

    puVar2 = (u16 *)&ov01_02206C08;
    uVar1 = 0;
    do {
        if (param_1 == *puVar2) {
            return 1;
        }
        uVar1 = uVar1 + 1;
        puVar2 = puVar2 + 1;
    } while (uVar1 < 0x2b);
    return 0;
}

s32 ov01_021F6BD0(s32 arg0) {
    if ((arg0 >= 0x1B58) && (arg0 <= 0x225F)) {
        return 1;
    }
    if ((arg0 >= 0xAF0) && (arg0 <= 0xBB7)) {
        return 1;
    }
    if ((arg0 >= 0x2710) && (arg0 <= 0x2773)) {
        return 1;
    }
    if ((arg0 >= 0x2774) && (arg0 <= 0x27A5)) {
        return 1;
    }
    return 0;
}

s32 ov01_021F6C28(UnkStruct_ov5_021EAE78 *arg0) {
    s32 v0;
    s32 v1;

    v1 = ov01_021F6B00(*((FieldSystem * *)&arg0->fieldSystem));
    v0 = ov01_021F6B10(*((FieldSystem * *)&arg0->fieldSystem));
    if ((v1 == 6) && (v0 == 1)) {
        return 1;
    }
    return 0;
}

s32 ov01_021F6C4C(void *arg0) {
    s32 v0;
    s32 v1;

    v1 = ov01_021F6B00((((UnkStruct_ov5_021EAE78 *)arg0)->fieldSystem));
    v0 = ov01_021F6B10((((UnkStruct_ov5_021EAE78 *)arg0)->fieldSystem));
    if ((v1 == 3) && (v0 == 1)) {
        ov01_021F6ABC((((UnkStruct_ov5_021EAE78 *)arg0)->fieldSystem), 3, 3, arg0 + 0x90);
        return 1;
    }
    return 0;
}

s32 ov01_021F6C7C(UnkStruct_ov5_021EAE78 *arg0) {
    s32 v0;
    s32 v1;

    v1 = ov01_021F6B00(*((FieldSystem * *)&arg0->fieldSystem));
    v0 = ov01_021F6AEC(*((FieldSystem * *)&arg0->fieldSystem));
    if ((v1 == 3) && (v0 == 6)) {
        return 1;
    }
    return 0;
}
