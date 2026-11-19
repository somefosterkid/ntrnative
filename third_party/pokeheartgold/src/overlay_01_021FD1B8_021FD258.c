#include "global.h"

#include "error_handling.h"
#include "field_types_def.h"
#include "overlay_01_021F1348.h"
#include "overlay_01_021FD1B8.h"

void ov01_021FD258(FieldEffectManager *param_1, s32 param_2);

void ov01_021FD1E8(UnkStruct_021F1118 *, u32, u32, FieldEffectManager *);
UnkStruct_021F1118 *ov01_021FD244(UnkStruct_021F114C *, u32);
void ov01_021FD290(FieldEffectManager *arg0, s32 arg1, s32 arg2);
Simple3DRenderObj *ov01_021FD2CC(FieldEffectManager *arg0, s32 arg1);

void ov01_021FD258(FieldEffectManager *param_1, s32 param_2) {
    UnkStruct_021F114C *ctx;
    UnkStruct_021F1118 *slot;

    ctx = ov01_021F1450(param_1, 0x14);
    if (param_2 == 0) {
        GF_AssertFail();
    }
    ctx->unk_04 = param_2;
    slot = ov01_021F1430(param_1, param_2 * (s32)sizeof(UnkStruct_021F1118), 0, 0);
    ctx->unk_08 = slot;
    do {
        slot->unk_00 = 0xffff;
        slot++;
        param_2 = param_2 + -1;
    } while (param_2 != 0);
    return;
}

void ov01_021FD290(FieldEffectManager *fieldEffMan, s32 arg1, s32 arg2) {
    UnkStruct_021F114C *v0;
    UnkStruct_021F1118 *v1;

    v0 = ov01_021F1450(fieldEffMan, 0x14);
    if (ov01_021FD244(v0, arg1) == 0) {
        v1 = ov01_021FD244(v0, 0xFFFF);
        if (v1 == 0) {
            GF_AssertFail();
            return;
        }
        ov01_021FD1E8(v1, arg1, arg2, fieldEffMan);
    }
}

Simple3DRenderObj *ov01_021FD2CC(FieldEffectManager *fieldEffMan, s32 arg1) {
    UnkStruct_021F1118 *v0;

    v0 = ov01_021FD244(ov01_021F1450(fieldEffMan, 0x14), arg1);
    if (v0 == 0) {
        GF_AssertFail();
    }
    return &v0->unk_18;
}
