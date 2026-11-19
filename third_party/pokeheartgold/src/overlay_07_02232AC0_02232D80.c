#include "global.h"

#include "overlay_07_unk_struct_ov07_02232D20.h"
#include "unk_02014DA0.h"

s32 ov07_02232644(s32);

void ov07_02232CD8(SPLEmitter *);

void ov07_02232D80(UnkStruct_ov07_02232D20 *arg);

void ov07_02232D80(UnkStruct_ov07_02232D20 *arg) {
    s32 res_no;
    s32 v0;
    s32 v1;

    res_no = arg->unk0.unkC;
    if (res_no == 0xFF) {
        if (arg->unk0.unk10 != 0) {
            v0 = 0;
            if (arg->resourceCount > 0) {
                do {
                    sub_02015494(arg->particleSystem, v0, ov07_02232CD8, arg);
                    v0 += 1;
                } while (v0 < arg->resourceCount);
            }
        } else {
            v1 = 0;
            if (arg->resourceCount > 0) {
                do {
                    if (v1 != ov07_02232644(arg->unk0.unk4)) {
                        sub_02015494(arg->particleSystem, v1, ov07_02232CD8, arg);
                    }
                    v1 += 1;
                } while (v1 < arg->resourceCount);
            }
        }
    } else {
        sub_02015494(arg->particleSystem, res_no, ov07_02232CD8, arg);
    }
    sub_02015528(arg->particleSystem, 1);
}

void *ov07_0221FF2C(ParticleSystem *);

s32 ov07_02232DF4(UnkStruct_ov07_02232D20 *arg0);

s32 ov07_02232DF4(UnkStruct_ov07_02232D20 *arg0) {
    if (sub_020154B0(arg0->particleSystem) == 0) {
        ov07_0221FF2C(arg0->particleSystem);
        return 0;
    }
    return 1;
}
