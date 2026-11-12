// Black's code at ov021 0x0219b8bc-0x0219c4ac: pokebw2's fldmmdl_movecode_pair.c

#include "types.h"
#include "field/event_trainer_eye.h"
#include "field/field_actor.h"
#include "field/field_effect.h"
#include "gfl/blact.h"
#include "gfl/key.h"
#include "gfl/random.h"
#include "gfl/sound.h"
#include "constants/directions.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "struct_decls.h"
#include "field/field.h"
#include "field/ov021_021942c8.h"
#include "field/ov021_021d046c.h"
#include "field/field_actor_internal.h"
#include "field/field_map.h"
#include "field/fldmmdl_movecode_pair.h"

u32 func_ov021_02197edc(FieldActor *param_1);

// The works of the move codes, in the actor's rail-position block (pokebw2's)
// The codes that follow the player (the first 8 bytes) or the other Trainer of a pair
typedef struct {
    u8 state;
    u8 active;
    s16 x;
    s16 z;
    s16 unk6;
    FieldActor *target;
} ActorFollowWork;

WORK_AREA_CHECK(ActorFollowWork, FIELD_ACTOR_MOVE_WORK_SIZE);

// The codes that hide the actor under a field effect (func_ov021_0219bdc4)
typedef struct {
    u8 state;
    u8 kind;
    u8 hasEffect;
    FieldEffectTask *task;
} ActorEffectWork;

WORK_AREA_CHECK(ActorEffectWork, FIELD_ACTOR_MOVE_WORK_SIZE);

// The codes that turn the actor (func_ov021_0219bf60)
typedef struct {
    u8 state;
    s8 dir;
    u8 flag;
} ActorTurnWork;

WORK_AREA_CHECK(ActorTurnWork, FIELD_ACTOR_MOVE_WORK_SIZE);

// The wall-following codes (func_ov021_0219c0d4)
typedef struct {
    u8 state;
    s32 mode;
    s32 variant;
} ActorHopWork;

WORK_AREA_CHECK(ActorHopWork, FIELD_ACTOR_MOVE_WORK_SIZE);

u32 func_ov021_0219b984(FieldActor *param_1, ActorFollowWork *param_2);
u32 func_ov021_0219b9d8(FieldActor *param_1, ActorFollowWork *param_2);
void func_ov021_0219ba0c(FieldActor *param_1, ActorFollowWork *param_2);
u32 func_ov021_0219ba6c(FieldActor *param_1);
void func_ov021_0219b9b0(FieldActor *param_1, ActorFollowWork *param_2);
u32 func_ov021_0219bc48(FieldActor *param_1, ActorFollowWork *param_2);
u32 func_ov021_0219bce4(FieldActor *param_1, ActorFollowWork *param_2);
u32 func_ov021_0219bd30(FieldActor *param_1, ActorFollowWork *work);
void func_ov021_0219bdc4(FieldActor *param_1, u8 param_2);
void func_ov021_0219c0d4(FieldActor *param_1, u32 param_2, u32 param_3);
u32 func_ov021_0219c338(FieldActor *param_1, ActorHopWork *work, u32 param_3);
u32 func_ov021_0219c180(FieldActor *param_1, const VecFx32 *param_2, s32 param_3, s32 param_4);
u32 func_ov021_0219c20c(FieldActor *param_1, const VecFx32 *param_2, s32 param_3, s32 param_4);
s32 func_ov021_0219c2e0(FieldActor *param_1, s32 param_2, s32 param_3);

FieldEffectTask *func_ov021_021d046c(FieldEffects *effects, FieldActor *actor, u32 index);
void func_ov021_0219bcc0(FieldActor *param_1, ActorFollowWork *param_2, FieldActor *param_3);
u32 func_ov021_0219b924(FieldActor *param_1, ActorFollowWork *param_2);
u32 func_ov021_0219b968(FieldActor *param_1, ActorFollowWork *param_2);
u32 func_ov021_0219bb48(FieldActor *param_1, ActorFollowWork *param_2);
u32 func_ov021_0219bb84(FieldActor *param_1, ActorFollowWork *param_2);
u32 func_ov021_0219bed0(FieldActor *param_1, ActorEffectWork *param_2);
u32 func_ov021_0219bf00(FieldActor *param_1, ActorEffectWork *param_2);
u32 func_ov021_0219c000(FieldActor *param_1, ActorTurnWork *param_2);
u32 func_ov021_0219c034(FieldActor *param_1, ActorTurnWork *param_2);
u32 func_ov021_0219c04c(FieldActor *param_1, ActorTurnWork *param_2);
u32 func_ov021_0219c080(FieldActor *param_1, ActorTurnWork *param_2);
u32 func_ov021_0219c0ac(FieldActor *param_1, ActorTurnWork *param_2);
u32 func_ov021_0219c13c(FieldActor *param_1, ActorHopWork *work);
u32 func_ov021_0219c150(FieldActor *param_1, ActorHopWork *work);
u32 func_ov021_0219c164(FieldActor *param_1, ActorHopWork *work);

// The step tables in pokebw2's order of definition, which gives Black's .rodata layout
static u32 (*const data_ov021_021d5374[])(FieldActor *, ActorFollowWork *) = { func_ov021_0219b924,
                                                                                  func_ov021_0219b968 };

static u32 (*const data_ov021_021d537c[])(FieldActor *, ActorEffectWork *) = { func_ov021_0219bed0, func_ov021_0219bf00 };

static u32 (*const data_ov021_021d53a0[])(FieldActor *, ActorTurnWork *) = { func_ov021_0219c000, func_ov021_0219c034,
                                                                  func_ov021_0219c04c, func_ov021_0219c080,
                                                                  func_ov021_0219c0ac };

static u32 (*const data_ov021_021d538c[])(FieldActor *, ActorFollowWork *) = { func_ov021_0219bb48, func_ov021_0219bb84 };

static u32 (*const data_ov021_021d5394[])(FieldActor *, ActorHopWork *) = { func_ov021_0219c13c,
                                                                                func_ov021_0219c150,
                                                                                func_ov021_0219c164 };

static const s32 data_ov021_021d5434[][2] = { { 1, 1 }, { -1, -1 }, { 1, -1 }, { -1, 1 } };

static const s32 data_ov021_021d5384[] = { 1, 0 };

static const s32 data_ov021_021d53b4[][2] = { { -1, 1 }, { 1, -1 }, { 0, 0 }, { 0, 0 } };

static const s32 data_ov021_021d53d4[][2] = { { 0, 0 }, { 0, 0 }, { 1, -1 }, { -1, 1 } };

static const s32 data_ov021_021d5414[][2] = { { -1, 1 }, { 1, -1 }, { 1, 1 }, { -1, -1 } };

static const s32 data_ov021_021d5454[][2] = {
    { DIR_RIGHT, DIR_LEFT }, { DIR_LEFT, DIR_RIGHT }, { DIR_UP, DIR_DOWN }, { DIR_DOWN, DIR_UP }
};

static const s32 data_ov021_021d53f4[][2] = {
    { DIR_LEFT, DIR_RIGHT }, { DIR_RIGHT, DIR_LEFT }, { DIR_DOWN, DIR_UP }, { DIR_UP, DIR_DOWN }
};


void func_ov021_0219b8bc(FieldActor *param_1) {
    ActorFollowWork *work;

    work = (ActorFollowWork *)ClearActorPositionBlock(param_1, 8);
    func_ov021_0219b984(param_1, work);
    func_ov010_0216d674(param_1, 0);
    func_ov010_0216da50(param_1);
    func_ov010_0216dae0(param_1, 0);
}

void func_ov021_0219b8e8(FieldActor *param_1) {
    ActorFollowWork *work;
    u32 r;

    work = (ActorFollowWork *)GetNPCRailPosPtrAddr(param_1);
    if (func_ov021_0219b984(param_1, work) != 0) {
        func_ov010_0216dae0(param_1, 0);
        do {
            r = data_ov021_021d5374[work->state](param_1, work);
        } while (r == 1);
    }
}

void func_ov021_0219b920(FieldActor *actor) {
}

u32 func_ov021_0219b924(FieldActor *param_1, ActorFollowWork *param_2) {
    func_ov010_0216da50(param_1);
    func_ov010_0216da7c(param_1);
    if (func_ov021_0219b9d8(param_1, param_2) == 1) {
        func_ov021_0219ba0c(param_1, param_2);
        if (func_ov021_0219ba6c(param_1) == 1) {
            func_ov010_0216da44(param_1);
            param_2->state = param_2->state + 1;
            return 1;
        }
    }
    return 0;
}

u32 func_ov021_0219b968(FieldActor *param_1, ActorFollowWork *param_2) {
    if (func_ov021_02197edc(param_1) == 1) {
        func_ov010_0216da50(param_1);
        param_2->state = 0;
    }
    return 0;
}

u32 func_ov021_0219b984(FieldActor *param_1, ActorFollowWork *param_2) {
    MMSys *mmSys;
    FieldActor *pFVar1;

    mmSys = GetActorMModelSystem(param_1);
    pFVar1 = FindPlayerFieldActor(mmSys);
    if (pFVar1 == (FieldActor *)0x0) {
        param_2->active = 0;
        return 0;
    }
    if (param_2->active == 0) {
        func_ov021_0219b9b0(param_1, param_2);
    }
    return 1;
}

void func_ov021_0219b9b0(FieldActor *param_1, ActorFollowWork *param_2) {
    s16 gPosX;
    MMSys *mmSys;
    FieldActor *actor;

    mmSys = GetActorMModelSystem(param_1);
    actor = FindPlayerFieldActor(mmSys);
    param_2->active = 1;
    gPosX = GetGPosX(actor);
    param_2->x = gPosX;
    gPosX = GetGPosZ(actor);
    param_2->z = gPosX;
    param_2->unk6 = 0xff;
}

u32 func_ov021_0219b9d8(FieldActor *param_1, ActorFollowWork *param_2) {
    s16 gPosX;
    s16 gPosZ;
    MMSys *mmSys;
    FieldActor *actor;

    mmSys = GetActorMModelSystem(param_1);
    actor = FindPlayerFieldActor(mmSys);
    if (actor != (FieldActor *)0x0) {
        gPosX = GetGPosX(actor);
        gPosZ = GetGPosZ(actor);
        if ((gPosX != param_2->x) || (gPosZ != param_2->z)) {
            return 1;
        }
    }
    return 0;
}

void func_ov021_0219ba0c(FieldActor *param_1, ActorFollowWork *param_2) {
    s16 gPosX;
    MMSys *mmSys;
    FieldActor *actor;

    mmSys = GetActorMModelSystem(param_1);
    actor = FindPlayerFieldActor(mmSys);
    gPosX = GetGPosX(actor);
    param_2->x = gPosX;
    gPosX = GetGPosZ(actor);
    param_2->z = gPosX;
}

u16 func_ov021_0219ba2c(FieldActor *param_1) {
    u16 acmd;
    MMSys *mmSys;

    mmSys = GetActorMModelSystem(param_1);
    FindPlayerFieldActor(mmSys);
    acmd = FldAct_GetAcmd(param_1);
    switch(acmd) {
    case 0x58:
        return 0x10;
    case 0x59:
        return 0x11;
    case 0x5a:
        return 0x12;
    case 0x5b:
        acmd = 0x13;
    }
    return acmd;
}

u32 func_ov021_0219ba6c(FieldActor *param_1) {
    FieldActor *player;
    s16 x;
    u32 acmd;
    s16 z;
    s16 px;
    s16 pz;
    u32 dir;

    player = FindPlayerFieldActor(GetActorMModelSystem(param_1));
    x = GetGPosX(param_1);
    z = GetGPosZ(param_1);
    px = FldAct_GetInitGPosX(player);
    pz = FldAct_GetInitGPosZ(player);
    if (x != px || z != pz) {
        acmd = func_ov021_0219ba2c(param_1);
        dir = GetDirFromPosToPos(x, z, px, pz);
        acmd = GetAcmdForDir(dir, acmd);
        func_ov010_0216d438(param_1, acmd);
        return 1;
    }
    return 0;
}

void func_ov021_0219bae0(FieldActor *param_1) {
    ActorFollowWork *work;

    work = (ActorFollowWork *)ClearActorPositionBlock(param_1, sizeof(ActorFollowWork));
    func_ov021_0219bc48(param_1, work);
    func_ov010_0216d674(param_1, 0);
    func_ov010_0216da50(param_1);
    work->active = 0;
}

void func_ov021_0219bb08(FieldActor *param_1) {
    ActorFollowWork *work;
    u32 r;

    work = (ActorFollowWork *)GetNPCRailPosPtrAddr(param_1);
    if (func_ov021_0219bc48(param_1, work) != 0) {
        do {
            r = data_ov021_021d538c[work->state](param_1, work);
        } while (r == 1);
    }
}

void func_ov021_0219bb38(FieldActor *actor) {
}

void func_ov021_0219bb3c(FieldActor *param_1) {
    ActorFollowWork *work;

    work = (ActorFollowWork *)GetNPCRailPosPtrAddr(param_1);
    work->active = 0;
}

u32 func_ov021_0219bb48(FieldActor *param_1, ActorFollowWork *param_2) {
    s32 result;

    func_ov010_0216da50(param_1);
    func_ov010_0216da7c(param_1);
    result = func_ov021_0219bce4(param_1, param_2);
    if ((result == 1) && (result = func_ov021_0219bd30(param_1, param_2), result == 1)) {
        func_ov010_0216da44(param_1);
        param_2->state = param_2->state + 1;
        return 1;
    }
    return 0;
}

u32 func_ov021_0219bb84(FieldActor *param_1, ActorFollowWork *param_2) {
    if (func_ov021_02197edc(param_1) == 0) {
        return 0;
    }
    func_ov010_0216da50(param_1);
    param_2->state = 0;
    return 0;
}

FieldActor *FindPairedTrainerActor(FieldActor *param_1) {
    FieldActor *actor;
    u32 index;
    u32 evType;
    u16 zone;
    MMSys *mmSys;
    u16 trainerId;

    index = 0;
    evType = GetActorEvType(param_1);
    zone = GetActorZoneID(param_1);
    mmSys = GetActorMModelSystem(param_1);
    trainerId = GetNPCTrainerID(param_1);
    switch (evType) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 20:
        if (NextActor(mmSys, &actor, &index) == 1) {
            do {
                if (param_1 != actor && GetActorZoneID(actor) == zone && GetNPCTrainerID(actor) == trainerId) {
                    return actor;
                }
            } while (NextActor(mmSys, &actor, &index) == 1);
        }
        break;
    }
    return NULL;
}

u32 func_ov021_0219bc48(FieldActor *param_1, ActorFollowWork *param_2) {
    u32 index;
    FieldActor *actor;
    u16 zone;
    u16 trainerId;
    MMSys *mmSys;

    mmSys = GetActorMModelSystem(param_1);
    index = 0;
    zone = GetActorZoneID(param_1);
    trainerId = GetNPCTrainerID(param_1);
    if (NextActor(mmSys, &actor, &index) == 1) {
        do {
            if (param_1 != actor && zone == GetActorZoneID(actor) && trainerId == GetNPCTrainerID(actor)) {
                if (param_2->active == 0) {
                    func_ov021_0219bcc0(param_1, param_2, actor);
                }
                return 1;
            }
        } while (NextActor(mmSys, &actor, &index) == 1);
    }
    param_2->active = 0;
    return 0;
}

void func_ov021_0219bcc0(FieldActor *param_1, ActorFollowWork *param_2, FieldActor *param_3) {
    s16 gPosX;

    param_2->active = 1;
    gPosX = GetGPosX(param_3);
    param_2->x = gPosX;
    gPosX = GetGPosZ(param_3);
    param_2->z = gPosX;
    param_2->unk6 = 0xff;
    param_2->target = param_3;
}

u32 func_ov021_0219bce4(FieldActor *param_1, ActorFollowWork *param_2) {
    s16 gPosX;
    s16 gPosZ;
    s16 initGPosX;
    s16 initGPosZ;
    FieldActor *actor;
    actor = param_2->target;
    gPosX = GetGPosX(param_1);
    gPosZ = GetGPosZ(param_1);
    initGPosX = FldAct_GetInitGPosX(actor);
    initGPosZ = FldAct_GetInitGPosZ(actor);
    if (((gPosX != initGPosX) || (gPosZ != initGPosZ)) && ((IsActorFlag16(actor) == 1 || (CheckActorMovementFlag(actor, 0x1808) == 0)))) {
        return 1;
    }
    return 0;
}

u32 func_ov021_0219bd30(FieldActor *param_1, ActorFollowWork *work) {
    s32 x;
    s32 z;
    int ox;
    s16 oz;
    s16 ix;
    s16 iz;
    u32 dir;
    u32 acmd;

    x = GetGPosX(param_1);
    z = GetGPosZ(param_1);
    ox = GetGPosX(work->target);
    oz = GetGPosZ(work->target);
    ix = FldAct_GetInitGPosX(work->target);
    iz = FldAct_GetInitGPosZ(work->target);
    if (x == ox && z == oz) {
        return 0;
    }
    dir = GetDirFromPosToPos(x, z, ix, iz);
    x += GetDirectionVectorCompX((u16)dir);
    z += GetDirectionVectorCompZ((u16)dir);
    if (x != ox || z != oz) {
        acmd = GetAcmdForDir(dir, 0xc);
        func_ov010_0216d438(param_1, acmd);
        return 1;
    }
    return 0;
}

void func_ov021_0219bdc4(FieldActor *param_1, u8 param_2) {
    ActorEffectWork *work;

    work = (ActorEffectWork *)ClearActorPositionBlock(param_1, sizeof(ActorEffectWork));
    work->kind = param_2;
    func_ov010_0216d674(param_1, 0);
    func_ov010_0216da50(param_1);
    SetActorMovementFlag(param_1, 8 << 12);
    VecFx32 vec = { 0, -32 * FX32_ONE, 0 };

    SetActorWPosOffset(param_1, &vec);
}

void func_ov021_0219be0c(FieldActor *param_1) {
    func_ov021_0219bdc4(param_1, 0);
}

void func_ov021_0219be18(FieldActor *param_1) {
    func_ov021_0219bdc4(param_1, 1);
}

void func_ov021_0219be24(FieldActor *param_1) {
    func_ov021_0219bdc4(param_1, 1);
}

void func_ov021_0219be30(FieldActor *param_1) {
    func_ov021_0219bdc4(param_1, 0);
}

void func_ov021_0219be3c(FieldActor *param_1) {
    ActorEffectWork *work;
    u32 r;

    work = (ActorEffectWork *)GetNPCRailPosPtrAddr(param_1);
    do {
        r = data_ov021_021d537c[work->state](param_1, work);
    } while (r == 1);
}

void func_ov021_0219be60(FieldActor *param_1) {
    FieldEffectTask *task;

    task = func_ov021_0219bf48(param_1);
    if (task != NULL) {
        func_ov021_021ac154(task);
    }
}

void func_ov021_0219be70(FieldActor *param_1) {
    ActorEffectWork *work;

    work = (ActorEffectWork *)GetNPCRailPosPtrAddr(param_1);
    work->state = 0;
    func_ov021_0219bf3c(param_1, NULL);
    if (work->hasEffect == 0) {
        VecFx32 vec = { 0, -32 * FX32_ONE, 0 };

        SetActorWPosOffset(param_1, &vec);
        SetActorMovementFlag(param_1, 2 << 14);
    }
}

FieldEffectTask *func_ov021_0219beb4(FieldActor *param_1, u32 param_2) {
    MMSys *mmSys;
    Field *field;
    FieldEffects *pFVar1;

    mmSys = GetActorMModelSystem(param_1);
    field = GetMMSysField(mmSys);
    pFVar1 = Field_GetFieldEffects(field);
    return func_ov021_021d046c(pFVar1, param_1, param_2);
}

u32 func_ov021_0219bed0(FieldActor *param_1, ActorEffectWork *param_2) {
    if (param_2->hasEffect == 0) {
        func_ov021_0219bf3c(param_1, func_ov021_0219beb4(param_1, param_2->kind));
    }
    func_ov010_0216da50(param_1);
    func_ov010_0216da7c(param_1);
    param_2->state = param_2->state + 1;
    return 0;
}

u32 func_ov021_0219bf00(FieldActor *param_1, ActorEffectWork *param_2) {
    if (param_2->hasEffect == 0) {
        if (func_ov021_0219bf48(param_1) == NULL && func_ov010_0216db24(param_1) == 1) {
            func_ov021_0219bf3c(param_1, func_ov021_0219beb4(param_1, param_2->kind));
        }
        SetActorMovementFlag(param_1, 2 << 14);
    }
    return 0;
}

void func_ov021_0219bf3c(FieldActor *param_1, FieldEffectTask *param_2) {
    ((ActorEffectWork *)GetNPCRailPosPtrAddr(param_1))->task = param_2;
}

FieldEffectTask *func_ov021_0219bf48(FieldActor *param_1) {
    return ((ActorEffectWork *)GetNPCRailPosPtrAddr(param_1))->task;
}

void func_ov021_0219bf54(FieldActor *param_1) {
    ActorEffectWork *work;

    work = (ActorEffectWork *)GetNPCRailPosPtrAddr(param_1);
    work->hasEffect = 1;
}

void func_ov021_0219bf60(FieldActor *param_1, u32 param_2, u8 param_3) {
    ActorTurnWork *work;

    work = (ActorTurnWork *)ClearActorPositionBlock(param_1, 4);
    work->dir = 9;
    work->flag = param_3;
    CheckSetActorFaceDir(param_1, 0);
}

void func_ov021_0219bf7c(FieldActor *param_1) {
    func_ov021_0219bf60(param_1, 0, 0);
}

void func_ov021_0219bf88(FieldActor *param_1) {
    func_ov021_0219bf60(param_1, 1, 0);
}

void func_ov021_0219bf94(FieldActor *param_1) {
    func_ov021_0219bf60(param_1, 2, 0);
}

void func_ov021_0219bfa0(FieldActor *param_1) {
    func_ov021_0219bf60(param_1, 3, 0);
}

void func_ov021_0219bfac(FieldActor *param_1) {
    func_ov021_0219bf60(param_1, 0, 1);
}

void func_ov021_0219bfb8(FieldActor *param_1) {
    func_ov021_0219bf60(param_1, 1, 1);
}

void func_ov021_0219bfc4(FieldActor *param_1) {
    func_ov021_0219bf60(param_1, 2, 1);
}

void func_ov021_0219bfd0(FieldActor *param_1) {
    func_ov021_0219bf60(param_1, 3, 1);
}

void func_ov021_0219bfdc(FieldActor *param_1) {
    ActorTurnWork *work;
    u32 r;

    work = (ActorTurnWork *)GetNPCRailPosPtrAddr(param_1);
    do {
        r = data_ov021_021d53a0[work->state](param_1, work);
    } while (r == 1);
}

u32 func_ov021_0219c000(FieldActor *param_1, ActorTurnWork *param_2) {
    int dir;
    int acmd;

    dir = GetActorFaceDir(param_1);
    acmd = GetAcmdForDir(dir, 0);
    func_ov010_0216d438(param_1, acmd);
    func_ov010_0216da50(param_1);
    func_ov010_0216da7c(param_1);
    param_2->state = 1;
    return 1;
}

u32 func_ov021_0219c034(FieldActor *param_1, ActorTurnWork *param_2) {
    if (func_ov021_02197edc(param_1) == 1) {
        param_2->state = 2;
        return 1;
    }
    return 0;
}

u32 func_ov021_0219c04c(FieldActor *param_1, ActorTurnWork *param_2) {
    FieldActor *actor;
    actor = param_1;
    if (param_2->dir == 9) {
        actor = FindPlayerFieldActor(GetActorMModelSystem(actor));
        if (actor != NULL) {
            param_2->dir = GetActorMotionDir(actor);
        }
    }
    func_ov010_0216da50(param_1);
    func_ov010_0216da7c(param_1);
    param_2->state = 3;
    return 1;
}

u32 func_ov021_0219c080(FieldActor *param_1, ActorTurnWork *param_2) {
    u32 acmd = GetAcmdForDir(GetActorMotionDir(FindPlayerFieldActor(GetActorMModelSystem(param_1))), 0);
    func_ov010_0216d438(param_1, acmd);
    param_2->state = 4;
    return 1;
}

u32 func_ov021_0219c0ac(FieldActor *param_1, ActorTurnWork *param_2) {
    if (func_ov021_02197edc(param_1) == 0) {
        return 0;
    }
    func_ov010_0216da50(param_1);
    func_ov010_0216da7c(param_1);
    param_2->state = 2;
    return 0;
}

void func_ov021_0219c0d4(FieldActor *param_1, u32 param_2, u32 param_3) {
    ActorHopWork *work;

    work = (ActorHopWork *)ClearActorPositionBlock(param_1, sizeof(ActorHopWork));
    work->mode = param_2;
    work->variant = param_3;
}

void func_ov021_0219c0e8(FieldActor *param_1) {
    func_ov021_0219c0d4(param_1, 0, 0);
}

void func_ov021_0219c0f4(FieldActor *param_1) {
    func_ov021_0219c0d4(param_1, 1, 1);
}

void func_ov021_0219c100(FieldActor *param_1) {
    func_ov021_0219c0d4(param_1, 2, 0);
}

void func_ov021_0219c10c(FieldActor *param_1) {
    func_ov021_0219c0d4(param_1, 2, 1);
}

void func_ov021_0219c118(FieldActor *param_1) {
    ActorHopWork *work;
    u32 r;

    work = (ActorHopWork *)GetNPCRailPosPtrAddr(param_1);
    do {
        r = data_ov021_021d5394[work->state](param_1, work);
    } while (r == 1);
}

u32 func_ov021_0219c13c(FieldActor *param_1, ActorHopWork *work) {
    func_ov010_0216da50(param_1);
    work->state = work->state + 1;
    return 1;
}

u32 func_ov021_0219c150(FieldActor *param_1, ActorHopWork *work) {
    func_ov021_0219c338(param_1, work, 0xc);
    work->state = work->state + 1;
    return 1;
}

u32 func_ov021_0219c164(FieldActor *param_1, ActorHopWork *work) {
    if (func_ov021_02197edc(param_1) == 1) {
        func_ov010_0216da50(param_1);
        work->state = 0;
    }
    return 0;
}

u32 func_ov021_0219c180(FieldActor *param_1, const VecFx32 *param_2, s32 param_3, s32 param_4) {
    VecFx32 pos;
    u32 tileType;
    u32 result = 0;
    s16 gx;
    s16 gz;

    pos = *param_2;
    gx = (s16)((param_2->x >> 4) / FX32_ONE);
    gz = (s16)((param_2->z >> 4) / FX32_ONE);
    gx += (s16)data_ov021_021d53b4[param_3][param_4];
    gz += (s16)data_ov021_021d53d4[param_3][param_4];
    pos.x = gx * (16 * FX32_ONE);
    pos.z = gz * (16 * FX32_ONE);
    if (GetTileTypeAtPosByActor(param_1, &pos, &tileType) == 1) {
        if (MapTile_BlocksCollision(tileType)) {
            result = 1;
        }
    }
    return result;
}

// A world position's grid coordinate, and the world position of a grid coordinate moved by an offset
static inline s16 Ov021WorldToGrid(fx32 world) {
    return (world >> 4) / FX32_ONE;
}

static inline fx32 Ov021GridOffsetToWorld(s16 grid, s16 offset) {
    s16 g = grid + offset;
    return g * (16 * FX32_ONE);
}
u32 func_ov021_0219c20c(FieldActor *param_1, const VecFx32 *param_2, s32 param_3, s32 param_4) {
    VecFx32 pos;
    u32 tileType;
    u32 result = 0;
    s16 gx;
    s16 gz;

    pos = *param_2;
    gx = (s16)((param_2->x >> 4) / FX32_ONE);
    gz = (s16)((param_2->z >> 4) / FX32_ONE);
    gx += (s16)data_ov021_021d5414[param_3][param_4];
    gz += (s16)data_ov021_021d5434[param_3][param_4];
    pos.x = gx * (16 * FX32_ONE);
    pos.z = gz * (16 * FX32_ONE);
    if (GetTileTypeAtPosByActor(param_1, &pos, &tileType) == 1) {
        if (MapTile_BlocksCollision(tileType)) {
            result = 1;
        }
    }
    return result;
}

u32 func_ov021_0219c298(FieldActor *param_1, s32 param_2, s32 param_3) {
    VecFx32 VStack_20;

    CopyActorWPos(param_1, &VStack_20);
    return func_ov021_0219c180(param_1, &VStack_20, param_2, param_3);
}

u32 func_ov021_0219c2bc(FieldActor *param_1, s32 param_2, s32 param_3) {
    VecFx32 VStack_20;

    CopyActorWPos(param_1, &VStack_20);
    return func_ov021_0219c20c(param_1, &VStack_20, param_2, param_3);
}

s32 func_ov021_0219c2e0(FieldActor *param_1, s32 param_2, s32 param_3) {
    if (func_ov021_0219c298(param_1, param_2, param_3) == 0) {
        if (func_ov021_0219c2bc(param_1, param_2, param_3) == 0) {
            return 9;
        }
        param_2 = data_ov021_021d53f4[param_2][param_3];
    }
    return param_2;
}

u32 func_ov021_0219c314(FieldActor *param_1, s32 *param_2, s32 param_3) {
    s32 dir;

    dir = func_ov021_0219c2e0(param_1, *param_2, param_3);
    *param_2 = dir;
    if (dir != 9) {
        return ActorRouteCollCheckOneTileInDir(param_1, (u16)dir);
    }
    return 0;
}

u32 func_ov021_0219c338(FieldActor *param_1, ActorHopWork *work, u32 param_3) {
    s32 side;
    s32 dir;
    u32 r;
    u32 code;

    side = work->variant;
    dir = GetActorMotionDir(param_1);
    r = func_ov021_0219c314(param_1, &dir, side);
    if (dir == 9) {
        dir = GetActorMotionDir(param_1);
        code = GetAcmdForDir(dir, 0x1c);
        func_ov010_0216d438(param_1, code);
        return 0;
    }
    if (r == 0) {
        code = GetAcmdForDir(dir, param_3);
        func_ov010_0216da44(param_1);
        func_ov010_0216d438(param_1, code);
        return 1;
    }
    if ((r & 1) && work->mode == 2) {
        dir = GetInverseDirection(GetActorMotionDir(param_1));
        side = data_ov021_021d5384[side];
        work->variant = side;
        r = func_ov021_0219c314(param_1, &dir, side);
        if (dir == 9) {
            dir = GetActorMotionDir(param_1);
            code = GetAcmdForDir(dir, 0x1c);
            func_ov010_0216d438(param_1, code);
            return 0;
        }
        if (r == 0) {
            code = GetAcmdForDir(dir, param_3);
            func_ov010_0216da44(param_1);
            func_ov010_0216d438(param_1, code);
            return 1;
        }
    }
    if (r & 2) {
        dir = data_ov021_021d5454[dir][side];
        r = func_ov021_0219c314(param_1, &dir, side);
        if (dir == 9) {
            dir = GetActorMotionDir(param_1);
            code = GetAcmdForDir(dir, 0x1c);
            func_ov010_0216d438(param_1, code);
            return 0;
        }
        if (r == 0) {
            code = GetAcmdForDir(dir, param_3);
            func_ov010_0216da44(param_1);
            func_ov010_0216d438(param_1, code);
            return 1;
        }
    }
    dir = GetActorMotionDir(param_1);
    code = GetAcmdForDir(dir, 0x1c);
    func_ov010_0216d438(param_1, code);
    return 0;
}
