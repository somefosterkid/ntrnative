// Black's code at ov021 0x021d0750-0x021d096c. No name is known: the module and address.

#include "types.h"
#include "gfl/std.h"
#include "gfl/g3d.h"
#include "field/field_actor.h"
#include "field/zone.h"
#include "nitro/fx/fx_vec.h"

// One of the 6 slots of Ov021Ctx, 0x14 bytes.
typedef struct {
    s32 unk_0;
    fx32 unk_4;
    VecFx32 pos;
} Ov021Slot;

// The actor's owner: its actor sits at 0x1c.
typedef struct {
    u32 unk_0[7];
    G3DActor *actor;
} Ov021Owner;

typedef struct {
    Ov021Owner *owner;
    FieldActor *actor;
    u8 unk_8;
    s8 unk_9;
    s8 unk_a;
    Ov021Slot slots[6];
} Ov021Ctx;

extern SRTMatrix data_ov021_021de628;  // ov021_021d046c.c's .data, before fldeff_bubble.c's

void func_ov021_021d08d8(Ov021Slot *param_1, G3DActor *param_2);
u16 func_ov021_021d0914(FieldActor *actor, VecFx16 *dest);
Ov021Slot *func_ov021_021d07fc(Ov021Ctx *param_1);
void func_ov021_021d0824(Ov021Slot *param_1, FieldActor *param_2);
void func_ov021_021d08bc(Ov021Slot *param_1, fx32 param_2);


void func_ov021_021d0750(u32 param_1, Ov021Ctx *param_2) {
    s32 frames;
    s32 i;
    Ov021Slot *slot;

    param_2->unk_a++;
    if (param_2->unk_a >= 0x78) {
        slot = func_ov021_021d07fc(param_2);
        if (slot != NULL) {
            func_ov021_021d0824(slot, param_2->actor);
        }
        param_2->unk_a = 0;
    }
    if (param_2->unk_8 != IsAllActorAcmdFinished(param_2->actor)) {
        param_2->unk_9++;
        if (param_2->unk_9 >= 5) {
            slot = func_ov021_021d07fc(param_2);
            if (slot != NULL) {
                func_ov021_021d0824(slot, param_2->actor);
            }
            param_2->unk_9 = 0;
        }
        param_2->unk_8 = IsAllActorAcmdFinished(param_2->actor);
    }
    GFL_G3DActorGetAnmFrameCount(param_2->owner->actor, 0, &frames);
    for (i = 0; i < 6; i++) {
        func_ov021_021d08bc(&param_2->slots[i], frames);
    }
}

void func_ov021_021d07d8(u32 param_1, Ov021Ctx *param_2) {
    s32 i;

    for (i = 0; i < 6; i++) {
        func_ov021_021d08d8(&param_2->slots[i], param_2->owner->actor);
    }
}

Ov021Slot *func_ov021_021d07fc(Ov021Ctx *param_1) {
    s32 i;

    for (i = 0; i < 6; i++) {
        if (param_1->slots[i].unk_0 == 0) {
            return &param_1->slots[i];
        }
    }
    return NULL;
}

void func_ov021_021d0824(Ov021Slot *param_1, FieldActor *param_2) {
    VecFx16 dir;
    fx32 speed;
    s32 flag;

    flag = 1;
    param_1->unk_0 = flag;
    param_1->unk_4 = 0;
    CopyActorWPos(param_2, &param_1->pos);
    if (func_ov021_021d0914(param_2, &dir) == 0) {
        speed = 0xe000;
    } else {
        speed = flag << 14;
    }
    param_1->pos.y += 0x7000;
    param_1->pos.x += FX_Mul(dir.x, speed);
    param_1->pos.z += FX_Mul(dir.z, speed);
}


void func_ov021_021d08b0(Ov021Slot *param_1) {
    MI_CpuFill8(param_1, 0, sizeof(Ov021Slot));
}

void func_ov021_021d08bc(Ov021Slot *param_1, fx32 param_2) {
    if (param_1->unk_0 != 0) {
        param_1->unk_4 += 0x1000;
        if (param_1->unk_4 > param_2) {
            func_ov021_021d08b0(param_1);
        }
    }
}

void func_ov021_021d08d8(Ov021Slot *param_1, G3DActor *param_2) {
    int i;

    if (param_1->unk_0 != 0) {
        data_ov021_021de628.translation = param_1->pos;
        for (i = 0; i < 2; i++) {
            GFL_G3DActorSetAnmFrame(param_2, i, &param_1->unk_4);
        }
        GFL_G3DSysDrawObjBBoxCull(param_2, &data_ov021_021de628);
    }
}

u16 func_ov021_021d0914(FieldActor *actor, VecFx16 *dest) {
    u16 dir;

    dir = GetActorFaceDir(actor);
    if (!CheckActorFlag(actor, 0x2000)) {
        dest->y = 0;
        dest->x = GetDirectionVectorCompX(dir);
        dest->z = GetDirectionVectorCompZ(dir);
        dest->x = dest->x << 16;
        dest->z = dest->z << 16;
    } else {
        func_ov021_0219e784(actor, dir, dest);
        dest->y = 0;
    }
    VEC_Fx16Normalize(dest, dest);
    return dir;
}



