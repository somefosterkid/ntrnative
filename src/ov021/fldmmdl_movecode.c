// Black's code at ov021 0x0219a098-0x0219b414: pokebw2's fldmmdl_movecode.c

#include "types.h"
#include "constants/directions.h"
#include "field/event_trainer_eye.h"
#include "field/field_actor.h"
#include "field/field_effect.h"
#include "gfl/blact.h"
#include "gfl/key.h"
#include "gfl/random.h"
#include "gfl/sound.h"
#include "struct_decls.h"
#include "field/field.h"
#include "field/ov021_021942c8.h"
#include "field/ov021_021d046c.h"
#include "field/field_actor_internal.h"
#include "field/field_map.h"
#define DIR_NONE 9

void func_ov021_0219a098(FieldActor *param_1, u32 param_2);
void func_ov021_0219a290(FieldActor *param_1, u32 param_2, u32 param_3, u32 param_4);
u32 func_ov021_02197edc(FieldActor *param_1);
u32 func_ov021_0219b0c4(void);
u32 func_ov021_0219b200(FieldActor *param_1, s32 param_2, s32 param_3);
u32 func_ov021_0219b358(FieldActor *param_1);
// The spin sub-work of the trainer-eye move codes (ev types 7, 8, 0x11, 0x12): the saved face
// direction, the turn direction (func_ov021_0219b374) and whether actor flag 8 was already set
typedef struct {
    u8 dir;
    s8 turn;
    u8 hadFlag;
    u8 pad3;
} Ov021SpinWork;

// The work of the back-and-forth move code (func_ov021_0219a87c), in the actor's rail-position block
typedef struct {
    u8 state;
    u8 inverse;
    u8 wait;
    u8 pad3;
    Ov021SpinWork spin;
} Ov021BackForthWork;

// The work of the rectangle-route move codes (func_ov021_0219aa38), in the actor's rail-position block
typedef struct {
    u8 state;
    u8 step;
    u8 stepMax;
    u8 axis;
    u8 route;
    u8 wait;
    u8 pad6[2];
    Ov021SpinWork spin;
} Ov021RouteWork;

// The work of the rectangle-route move codes that can reverse (func_ov021_0219ad18), in the actor's
// rail-position block
typedef struct {
    u8 state;
    s8 step;
    u8 stepMax;
    u8 axis;
    u8 route;
    u8 reverse;
    u8 wait;
    u8 pad7;
    Ov021SpinWork spin;
} Ov021RouteRevWork;

// The work of the spin and swing move codes (func_ov021_0219a5a4), in the actor's rail-position block: turn is 2
// for a clockwise turn, state picks the step of the table that runs
typedef struct {
    s8 turn;
    u8 pad1;
    s8 state;
    u8 pad3;
    s32 count;
} Ov021TurnWork;

// A pair of move codes that the reversing route codes swap
typedef struct {
    u16 first;
    u16 second;
} ActorMoveCodePair;

// A set of directions and the key that func_ov021_0219b120 finds it by
typedef struct {
    s32 key;
    const s32 *dirs;
} ActorDirTableEntry;

void func_ov021_0219b374(FieldActor *param_1, Ov021SpinWork *spin);
void func_ov021_0219b390(FieldActor *param_1, Ov021SpinWork *work);
void func_ov021_0219b404(FieldActor *param_1, Ov021SpinWork *spin);
void func_ov021_0219aa38(FieldActor *param_1, u8 param_2, u8 param_3, int param_4);
void func_ov021_0219ad18(FieldActor *param_1, u8 param_2, u8 param_3, int param_4);
const s32 *func_ov021_0219b120(s32 key);
u32 func_ov021_0219a604(FieldActor *param_1, Ov021TurnWork *work);
u32 func_ov021_0219a638(FieldActor *param_1, Ov021TurnWork *work);
u32 func_ov021_0219a660(FieldActor *param_1, Ov021TurnWork *work);
u32 func_ov021_0219a6cc(FieldActor *param_1, Ov021TurnWork *work);
u32 func_ov021_0219a780(FieldActor *param_1, Ov021TurnWork *work);
u32 func_ov021_0219a7a8(FieldActor *param_1, Ov021TurnWork *work);
u32 func_ov021_0219a7c4(FieldActor *param_1, Ov021TurnWork *work);
u32 func_ov021_0219a7dc(FieldActor *param_1, Ov021TurnWork *work);
u32 func_ov021_0219a8c0(FieldActor *param_1, Ov021BackForthWork *work);
u32 func_ov021_0219a900(FieldActor *param_1, Ov021BackForthWork *work);
u32 func_ov021_0219aa08(FieldActor *param_1, Ov021BackForthWork *work);
u32 func_ov021_0219ab8c(FieldActor *param_1, Ov021RouteWork *work);
u32 func_ov021_0219ace8(FieldActor *param_1, Ov021RouteWork *work);
u32 func_ov021_0219ae10(FieldActor *param_1, Ov021RouteRevWork *work);
u32 func_ov021_0219ae68(FieldActor *param_1, Ov021RouteRevWork *work);
u32 func_ov021_0219aec8(FieldActor *param_1, Ov021RouteRevWork *work);
u32 func_ov021_0219b030(FieldActor *param_1, Ov021RouteRevWork *work);

s32 func_ov021_0219b0f0(const s32 *param_1, s32 param_2);
u16 func_ov021_0219a1e8(u16 code, u16 dir);
s32 func_ov021_0219b104(s32 param_1, s32 param_2);
u32 func_ov021_0219a4cc(FieldActor *param_1, u32 param_2);
u32 func_ov021_0219b2e8(FieldActor *param_1, u8 *param_2);
u32 func_ov021_0219b334(FieldActor *param_1);
u32 func_ov021_0219b060(void);

// MWCC lays out objects of the same size by where each was declared: pokebw2's order of the tables, the step tables
// of the back-and-forth code, the move codes of trainers that see the player and the direction sets after
// func_ov021_0219a7dc, gives Black's .rodata
static u32 (*const data_ov021_021d4d04[])(FieldActor *actor,
                                           Ov021RouteWork *work) = { func_ov021_0219ab8c,
                                                                     func_ov021_0219ace8 };

static const s32 data_ov021_021d4e78[] = { DIR_LEFT, DIR_UP, DIR_DOWN, DIR_RIGHT };

static const s32 data_ov021_021d4d6c[] = { DIR_UP, DIR_DOWN, DIR_NONE };

static const s32 data_ov021_021d4d3c[] = { DIR_DOWN, DIR_RIGHT, DIR_NONE };

static const s32 data_ov021_021d4d0c[] = { DIR_UP, DIR_LEFT, DIR_NONE };

static const s32 data_ov021_021d4d54[] = { DIR_LEFT, DIR_RIGHT, DIR_NONE };

static const s32 data_ov021_021d4d18[] = { DIR_DOWN, DIR_LEFT, DIR_NONE };

static const s32 data_ov021_021d4e48[] = { DIR_UP, DIR_RIGHT, DIR_LEFT, DIR_DOWN };

static const s32 data_ov021_021d4d30[] = { DIR_UP, DIR_RIGHT, DIR_NONE };

static const s32 data_ov021_021d4d24[] = { DIR_LEFT, DIR_RIGHT, DIR_NONE };

static const s32 data_ov021_021d4e18[] = { DIR_UP, DIR_LEFT, DIR_DOWN, DIR_RIGHT };

static const s32 data_ov021_021d4dd8[] = { DIR_DOWN, DIR_UP, DIR_LEFT, DIR_RIGHT };

static const s32 data_ov021_021d4d48[] = { DIR_UP, DIR_DOWN, DIR_NONE };

static const s32 data_ov021_021d4e08[] = { DIR_LEFT, DIR_UP, DIR_RIGHT, DIR_DOWN };

static const s32 data_ov021_021d4e38[] = { DIR_DOWN, DIR_LEFT, DIR_RIGHT, DIR_UP };

static const s32 data_ov021_021d4fa4[] = { 0x10, 0x20, 0x30, 0x40, -1 };

static const s32 data_ov021_021d4f18[] = { DIR_RIGHT, DIR_UP, DIR_LEFT, DIR_DOWN };

static const s32 data_ov021_021d4f08[] = { DIR_DOWN, DIR_RIGHT, DIR_LEFT, DIR_UP };

static const s32 data_ov021_021d4e88[] = { DIR_UP, DIR_DOWN, DIR_RIGHT, DIR_LEFT };

static const s32 data_ov021_021d4ec8[] = { DIR_UP, DIR_LEFT, DIR_RIGHT, DIR_NONE };

static const s32 data_ov021_021d4ed8[] = { DIR_RIGHT, DIR_LEFT, DIR_UP, DIR_DOWN };

static const s32 data_ov021_021d4ef8[] = { DIR_UP, DIR_DOWN, DIR_LEFT, DIR_NONE };

static const s32 data_ov021_021d4f38[] = { DIR_LEFT, DIR_DOWN, DIR_UP, DIR_RIGHT };

static const s32 data_ov021_021d4ea8[] = { DIR_LEFT, DIR_DOWN, DIR_RIGHT, DIR_UP };

static const s32 data_ov021_021d4e98[] = { DIR_RIGHT, DIR_UP, DIR_DOWN, DIR_LEFT };

static const s32 data_ov021_021d4d98[] = { DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT };

static u32 (*const data_ov021_021d4d78[])(FieldActor *actor, Ov021RouteRevWork *work) = {
    func_ov021_0219ae10, func_ov021_0219ae68,
    func_ov021_0219aec8, func_ov021_0219b030
};

static const s32 data_ov021_021d4e68[] = { DIR_DOWN, DIR_UP, DIR_RIGHT, DIR_LEFT };

static const s32 data_ov021_021d4e58[] = { DIR_RIGHT, DIR_LEFT, DIR_DOWN, DIR_UP };

static const s32 data_ov021_021d4fcc[][4] = { { DIR_UP, DIR_LEFT, DIR_DOWN, DIR_RIGHT },
                                              { DIR_UP, DIR_RIGHT, DIR_DOWN, DIR_LEFT } };

static const s32 data_ov021_021d4dc8[] = { DIR_LEFT, DIR_RIGHT, DIR_DOWN, DIR_UP };

static const s32 data_ov021_021d4e28[] = { DIR_LEFT, DIR_RIGHT, DIR_DOWN, DIR_UP };

static u32 (*const data_ov021_021d4de8[])(FieldActor *actor, Ov021TurnWork *work) = {
    func_ov021_0219a604,
    func_ov021_0219a638,
    func_ov021_0219a660,
    func_ov021_0219a6cc,
};

static const s32 data_ov021_021d4df8[] = { DIR_DOWN, DIR_LEFT, DIR_UP, DIR_RIGHT };

static const s32 data_ov021_021d4f7c[] = { DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT, DIR_NONE };

static const s32 data_ov021_021d4f28[] = { DIR_RIGHT, DIR_DOWN, DIR_LEFT, DIR_UP };

static const s32 data_ov021_021d4eb8[] = { DIR_UP, DIR_DOWN, DIR_RIGHT, DIR_NONE };

static const s32 data_ov021_021d4ee8[] = { DIR_DOWN, DIR_LEFT, DIR_RIGHT, DIR_NONE };

static const s32 data_ov021_021d4f48[] = { DIR_RIGHT, DIR_DOWN, DIR_UP, DIR_LEFT };

static const s32 data_ov021_021d4da8[] = { DIR_UP, DIR_RIGHT, DIR_DOWN, DIR_LEFT };

static const s32 data_ov021_021d4f68[] = { DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT, DIR_NONE };

static const s32 data_ov021_021d4d88[] = { DIR_LEFT, DIR_RIGHT, DIR_UP, DIR_DOWN };

static u32 (*const data_ov021_021d4db8[])(FieldActor *actor, Ov021TurnWork *work) = {
    func_ov021_0219a780,
    func_ov021_0219a7a8,
    func_ov021_0219a7c4,
    func_ov021_0219a7dc,
};

// The directions that each move code turns between, in turn, after the move code and ended by DIR_NONE. A move code
// of 0xff ends the table
static const u16 data_ov021_021d5074[][6] = {
    { 4, DIR_UP, DIR_DOWN, DIR_NONE },
    { 5, DIR_LEFT, DIR_RIGHT, DIR_NONE },
    { 6, DIR_UP, DIR_LEFT, DIR_NONE },
    { 7, DIR_UP, DIR_RIGHT, DIR_NONE },
    { 8, DIR_DOWN, DIR_LEFT, DIR_NONE },
    { 9, DIR_DOWN, DIR_RIGHT, DIR_NONE },
    { 10, DIR_UP, DIR_DOWN, DIR_LEFT, DIR_NONE },
    { 11, DIR_UP, DIR_RIGHT, DIR_DOWN, DIR_NONE },
    { 12, DIR_UP, DIR_RIGHT, DIR_LEFT, DIR_NONE },
    { 13, DIR_DOWN, DIR_LEFT, DIR_RIGHT, DIR_NONE },
    { 0xff },
};

static const ActorMoveCodePair data_ov021_021d4f90[] = {
    { 0x25, 0x2b }, { 0x26, 0x2c }, { 0x27, 0x2a }, { 0x28, 0x29 }, { 0xff, 0xff },
};

static const s32 data_ov021_021d4fb8[] = { DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT, DIR_NONE };

static const s32 data_ov021_021d4f58[] = { DIR_DOWN, DIR_RIGHT, DIR_UP, DIR_LEFT };


// The work of an actor that turns at random, in the actor's rail-position block (pokebw2's RailWanderWork): pause
// is set while it is held, mask picks the directions it may turn to
typedef struct {
    u16 pause;
    s16 timer;
    u32 mask;
} RailWanderWork;

void func_ov021_0219a098(FieldActor *param_1, u32 param_2) {
    RailWanderWork *work;

    work = (RailWanderWork *)ClearActorPositionBlock(param_1, sizeof(RailWanderWork));
    work->timer = func_ov021_0219b0f0(data_ov021_021d4fa4, -1);
    work->mask = param_2;
    func_ov010_0216d674(param_1, 0);
    func_ov010_0216da50(param_1);
}

void func_ov021_0219a0cc(FieldActor *param_1) {
    func_ov021_0219a098(param_1, 0);
}

void func_ov021_0219a0d8(FieldActor *param_1) {
    func_ov021_0219a098(param_1, 1);
}

void func_ov021_0219a0e4(FieldActor *param_1) {
    func_ov021_0219a098(param_1, 2);
}

void func_ov021_0219a0f0(FieldActor *param_1) {
    func_ov021_0219a098(param_1, 3);
}

void func_ov021_0219a0fc(FieldActor *param_1) {
    func_ov021_0219a098(param_1, 4);
}

void func_ov021_0219a108(FieldActor *param_1) {
    func_ov021_0219a098(param_1, 5);
}

void func_ov021_0219a114(FieldActor *param_1) {
    func_ov021_0219a098(param_1, 6);
}

void func_ov021_0219a120(FieldActor *param_1) {
    func_ov021_0219a098(param_1, 7);
}

void func_ov021_0219a12c(FieldActor *param_1) {
    func_ov021_0219a098(param_1, 8);
}

void func_ov021_0219a138(FieldActor *param_1) {
    func_ov021_0219a098(param_1, 9);
}

void func_ov021_0219a144(FieldActor *param_1) {
    func_ov021_0219a098(param_1, 10);
}

void func_ov021_0219a150(FieldActor *param_1) {
    RailWanderWork *work;
    u16 dir;

    work = (RailWanderWork *)GetNPCRailPosPtrAddr(param_1);
    if (GetActorEvType(param_1) == 0xb) {
        if (func_ov010_02164168(param_1) == 1 && func_ov021_0219b0c4() == 1) {
            u16 r;
            dir = GetActorFaceDir(param_1);
            r = func_ov021_0219a1e8(GetActorMoveCode(param_1), dir);
            if (r != 9) {
                CheckSetActorFaceDir(param_1, r);
            }
        }
    } else {
        int r = func_ov021_0219b200(param_1, work->mask, 9);
        if (r != 9) {
            CheckSetActorFaceDir(param_1, r);
        } else if (work->pause == 0) {
            work->timer--;
            if (work->timer <= 0) {
                work->timer = func_ov021_0219b0f0(data_ov021_021d4fa4, -1);
                r = func_ov021_0219b104(work->mask, 9);
                CheckSetActorFaceDir(param_1, r);
            }
        }
    }
    SetActorInitialGPosToNowGPos(param_1);
}

static inline u16 Ov021NextDirInCycle(const u16 *e, u16 dir) {
    s32 i;

    for (i = 1; e[i] != 9; i++) {
        if (dir == e[i]) {
            i++;
            if (e[i] == 9) {
                i = 1;
            }
            return e[i];
        }
    }
    return 9;
}
u16 func_ov021_0219a1e8(u16 moveCode, u16 faceDir) {
    s32 i = 0;
    do {
        if (moveCode == data_ov021_021d5074[i][0]) {
            const u16 *entry;
            s32 j = 1;

            entry = data_ov021_021d5074[i];
            if (entry[1] != DIR_NONE) {
                while (TRUE) {
                    if (faceDir == entry[j]) {
                        j++;
                        if (entry[j] == DIR_NONE) {
                            j = 1;
                        }
                        return entry[j];
                    }
                    j++;
                    if (entry[j] == DIR_NONE) {
                        break;
                    }
                }
            }
            return DIR_NONE;
        }
        i++;
    } while (data_ov021_021d5074[i][0] != 0xff);
    return DIR_NONE;
}

void func_ov021_0219a24c(FieldActor *actor) {
}

void func_ov021_0219a250(FieldActor *param_1) {
    func_ov021_0219a290(param_1, 0xc, 0xb, 0);
}

void func_ov021_0219a260(FieldActor *param_1) {
    func_ov021_0219a290(param_1, 0xc, 0xc, 0);
}

void func_ov021_0219a270(FieldActor *param_1) {
    func_ov021_0219a290(param_1, 0xc, 0xd, 0);
}

void func_ov021_0219a280(FieldActor *param_1) {
    func_ov021_0219a290(param_1, 0xc, 0xd, 2);
}

// The work of the random-walk move codes (func_ov021_0219a290), in the actor's rail-position block
typedef struct {
    s16 state;
    s16 wait;
    s32 mode;
    s32 acmd;
    s32 dirs;
} Ov021RandWalkWork;

void func_ov021_0219a290(FieldActor *param_1, u32 param_2, u32 param_3, u32 param_4) {
    Ov021RandWalkWork *work;

    work = (Ov021RandWalkWork *)ClearActorPositionBlock(param_1, sizeof(Ov021RandWalkWork));
    work->mode = param_4;
    work->acmd = param_2;
    work->dirs = param_3;
    func_ov010_0216d674(param_1, 0);
    func_ov010_0216da50(param_1);
}

void func_ov021_0219a2b8(FieldActor *param_1) {
    Ov021RandWalkWork *work;
    u32 dir;
    u32 acmd;
    u32 blocked;

    work = (Ov021RandWalkWork *)GetNPCRailPosPtrAddr(param_1);
    switch ((u32)work->state) {
    case 0:
        func_ov010_0216da50(param_1);
        func_ov010_0216da7c(param_1);
        dir = GetActorFaceDir(param_1);
        acmd = GetAcmdForDir(dir, 0);
        func_ov010_0216d438(param_1, acmd);
        work->state++;
        break;
    case 1:
        if (func_ov021_02197edc(param_1) == 0) {
            break;
        }
        work->wait = func_ov021_0219b0f0(data_ov021_021d4fa4, -1);
        work->state++;
        /* fall through */
    case 2:
        work->wait--;
        if (work->wait != 0) {
            break;
        }
        work->state++;
        /* fall through */
    case 3:
        dir = func_ov021_0219b104(work->dirs, 9);
        ChangeActorDirection(param_1, dir);
        if (work->mode == 1 && func_ov021_0219a4cc(param_1, dir) == 0) {
            work->state = 0;
            return;
        }
        blocked = ActorRouteCollCheckOneTileInDir(param_1, dir);
        if (blocked != 0) {
            if (work->mode == 2) {
                if (blocked & 1) {
                    work->state = 0;
                    return;
                }
            } else {
                work->state = 0;
                return;
            }
        }
        acmd = GetAcmdForDir(dir, work->acmd);
        func_ov010_0216d438(param_1, acmd);
        func_ov010_0216da44(param_1);
        work->state++;
        /* fall through */
    case 4:
        if (func_ov021_02197edc(param_1) == 0) {
            break;
        }
        func_ov010_0216da50(param_1);
        work->state = 0;
        break;
    }
}

void func_ov021_0219a3d8(FieldActor *param_1, s32 *param_2) {
    s32 z;
    s32 x = GetActorDefaultGPosX(param_1);
    s32 w;
    s32 h;

    z = GetActorDefaultGPosZ(param_1);
    w = GetActorWalkAreaW(param_1);
    h = GetActorWalkAreaH(param_1);
    switch (GetActorMoveCode(param_1)) {
    case 6:
        param_2[0] = x - w;
        param_2[2] = x;
        param_2[1] = z - h;
        param_2[3] = z;
        break;
    case 7:
        param_2[0] = x;
        param_2[2] = x + w;
        param_2[1] = z - h;
        param_2[3] = z;
        break;
    case 8:
        param_2[0] = x - w;
        param_2[2] = x;
        param_2[1] = z;
        param_2[3] = z + h;
        break;
    case 9:
        param_2[2] = x + w;
        param_2[0] = x;
        param_2[1] = z;
        param_2[3] = z + h;
        break;
    case 10:
        param_2[0] = x - w;
        param_2[2] = x;
        param_2[1] = z - h;
        param_2[3] = z + h;
        break;
    case 11:
        param_2[2] = x + w;
        param_2[0] = x;
        param_2[1] = z - h;
        param_2[3] = z + h;
        break;
    case 12:
        param_2[0] = x - w;
        param_2[2] = x + w;
        param_2[1] = z - h;
        param_2[3] = z;
        break;
    case 13:
        param_2[0] = x - w;
        param_2[2] = x + w;
        param_2[1] = z;
        param_2[3] = z + h;
        break;
    }
}

u32 func_ov021_0219a4cc(FieldActor *param_1, u32 param_2) {
    s32 rect[4];
    s32 x;
    s32 z;

    func_ov021_0219a3d8(param_1, rect);
    x = GetGPosX(param_1) + GetDirectionVectorCompX((u16)param_2);
    z = GetGPosZ(param_1) + GetDirectionVectorCompZ((u16)param_2);
    if (rect[0] > x || rect[2] < x) {
        return 0;
    }
    if (rect[1] > z || rect[3] < z) {
        return 0;
    }
    return 1;
}

// The work of the face-direction move codes, in the actor's rail-position block
typedef struct {
    u32 dir;
    u32 state;
} Ov021FaceDirWork;

void func_ov021_0219a528(FieldActor *param_1, u32 param_2) {
    ActorPositionRail *pAVar1;

    pAVar1 = ClearActorPositionBlock(param_1, 8);
    pAVar1->unk0 = param_2;
    func_ov010_0216d674(param_1, 0);
    func_ov010_0216da50(param_1);
    SetActorInitialGPosToNowGPos(param_1);
}

void func_ov021_0219a54c(FieldActor *param_1) {
    Ov021FaceDirWork *work = (Ov021FaceDirWork *)GetNPCRailPosPtrAddr(param_1);

    switch (work->state) {
    case 0:
        CheckSetActorFaceDir(param_1, work->dir);
        work->state++;
        break;
    case 1:
        break;
    }
}

void func_ov021_0219a574(FieldActor *param_1) {
    func_ov021_0219a528(param_1, 0);
}

void func_ov021_0219a580(FieldActor *param_1) {
    func_ov021_0219a528(param_1, 1);
}

void func_ov021_0219a58c(FieldActor *param_1) {
    func_ov021_0219a528(param_1, 2);
}

void func_ov021_0219a598(FieldActor *param_1) {
    func_ov021_0219a528(param_1, 3);
}

void func_ov021_0219a5a4(FieldActor *param_1, u8 param_2) {
    Ov021TurnWork *work;

    work = (Ov021TurnWork *)ClearActorPositionBlock(param_1, sizeof(Ov021TurnWork));
    work->turn = param_2;
    func_ov010_0216d674(param_1, 0);
    func_ov010_0216da50(param_1);
    SetActorInitialGPosToNowGPos(param_1);
}

void func_ov021_0219a5c8(FieldActor *param_1) {
    func_ov021_0219a5a4(param_1, 2);
}

void func_ov021_0219a5d4(FieldActor *param_1) {
    func_ov021_0219a5a4(param_1, 3);
}

void func_ov021_0219a5e0(FieldActor *param_1) {
    Ov021TurnWork *work;
    u32 r;

    work = (Ov021TurnWork *)GetNPCRailPosPtrAddr(param_1);
    do {
        r = data_ov021_021d4de8[work->state](param_1, work);
    } while (r == 1);
}

u32 func_ov021_0219a604(FieldActor *param_1, Ov021TurnWork *work) {
    s32 dir;

    dir = func_ov021_0219b200(param_1, 0x26, 9);
    if (dir == 9) {
        dir = GetActorFaceDir(param_1);
    }
    dir = GetAcmdForDir(dir, 0);
    func_ov010_0216d438(param_1, dir);
    work->state = 1;
    return 1;
}

u32 func_ov021_0219a638(FieldActor *param_1, Ov021TurnWork *work) {
    if (func_ov021_02197edc(param_1) == 0) {
        return 0;
    }
    if (GetActorEvType(param_1) != 0xb) {
        work->count = 0;
    }
    work->state = 2;
    return 1;
}

u32 func_ov021_0219a660(FieldActor *param_1, Ov021TurnWork *work) {
    BOOL BVar2;
    u32 result;
    s32 val;

    if (GetActorEvType(param_1) == 0xb) {
        if (work->count != 0) {
            work->count = 0;
            return 0;
        }
        BVar2 = func_ov010_02164168(param_1);
        if (BVar2 == 0) {
            return 0;
        }
        if (func_ov021_0219b0c4() == 0) {
            return 0;
        }
        work->count = work->count + 1;
    }
    else {
        if ((work->count != 0) && (result = func_ov021_0219b200(param_1, 0x26, 9), result != 9)) {
            work->state = 0;
            return 1;
        }
        val = work->count + 1;
        work->count = val;
        if (val < 0x18) {
            return 0;
        }
    }
    work->state = 3;
    return 1;
}

u32 func_ov021_0219a6cc(FieldActor *param_1, Ov021TurnWork *work) {
    s32 a[5] = { 0, 2, 1, 3, 9 };
    s32 b[5] = { 0, 3, 1, 2, 9 };
    s32 *t;
    s32 i;
    u16 dir;

    t = a;
    if (work->turn != 2) {
        t = b;
    }
    dir = GetActorFaceDir(param_1);
    i = 0;
    if (t[0] != 9) {
        while (1) {
            if (dir == t[i]) {
                break;
            }
            i++;
            if (t[i] == 9) {
                break;
            }
        }
    }
    i++;
    if (t[i] == 9) {
        i = 0;
    }
    CheckSetActorFaceDir(param_1, t[i]);
    work->state = 0;
    return TRUE;
}

void func_ov021_0219a750(FieldActor *param_1) {
    func_ov021_0219a5a4(param_1, 3);
}

void func_ov021_0219a75c(FieldActor *param_1) {
    Ov021TurnWork *work;
    u32 r;

    work = (Ov021TurnWork *)GetNPCRailPosPtrAddr(param_1);
    do {
        r = data_ov021_021d4db8[work->state](param_1, work);
    } while (r == 1);
}

u32 func_ov021_0219a780(FieldActor *param_1, Ov021TurnWork *work) {
    u32 acmd;
    int dir;

    dir = GetActorFaceDir(param_1);
    acmd = GetAcmdForDir(dir, 0);
    func_ov010_0216d438(param_1, acmd);
    work->state = 1;
    return 1;
}

u32 func_ov021_0219a7a8(FieldActor *param_1, Ov021TurnWork *work) {
    if (func_ov021_02197edc(param_1) == 0) {
        return 0;
    }
    work->count = 0;
    work->state = 2;
    return 1;
}

u32 func_ov021_0219a7c4(FieldActor *param_1, Ov021TurnWork *work) {
    s32 val;

    val = work->count + 1;
    work->count = val;
    if (val < 0x18) {
        return 0;
    }
    work->state = 3;
    return 1;
}

u32 func_ov021_0219a7dc(FieldActor *param_1, Ov021TurnWork *work) {
    s32 a[5] = { 0, 2, 1, 3, 9 };
    s32 b[5] = { 0, 3, 1, 2, 9 };
    s32 *t;
    s32 i;
    u16 dir;

    t = a;
    if (work->turn != 2) {
        t = b;
    }
    dir = GetActorFaceDir(param_1);
    i = 0;
    if (t[0] != 9) {
        while (1) {
            if (dir == t[i]) {
                break;
            }
            i++;
            if (t[i] == 9) {
                break;
            }
        }
    }
    i++;
    if (t[i] == 9) {
        i = 0;
    }
    CheckSetActorFaceDir(param_1, t[i]);
    if (GetActorFaceDir(param_1) == GetDefaultActorDir(param_1)) {
        work->turn = GetInverseDirection(work->turn);
    }
    work->state = 0;
    return TRUE;
}

static u32 (*const data_ov021_021d4d60[])(FieldActor *actor, Ov021BackForthWork *work) = {
    func_ov021_0219a8c0,
    func_ov021_0219a900,
    func_ov021_0219aa08,
};

static const s32 data_ov021_021d4fec[] = {
#include "asset/fldmmdl_movecode_data_ov021_021d4fec.inc"
};

static const ActorDirTableEntry data_ov021_021d50f8[] = {
    { 0, data_ov021_021d4fb8 },  { 1, data_ov021_021d4d0c },  { 2, data_ov021_021d4d30 },  { 3, data_ov021_021d4d18 },
    { 4, data_ov021_021d4d3c },  { 5, data_ov021_021d4ef8 },  { 6, data_ov021_021d4eb8 },  { 7, data_ov021_021d4ec8 },
    { 8, data_ov021_021d4ee8 },  { 9, data_ov021_021d4d6c },  { 10, data_ov021_021d4d54 }, { 11, data_ov021_021d4f68 },
    { 12, data_ov021_021d4d48 }, { 13, data_ov021_021d4d24 }, { 14, data_ov021_021d4e48 }, { 15, data_ov021_021d4e58 },
    { 16, data_ov021_021d4e68 }, { 17, data_ov021_021d4f38 }, { 18, data_ov021_021d4e28 }, { 19, data_ov021_021d4dc8 },
    { 20, data_ov021_021d4dd8 }, { 21, data_ov021_021d4f48 }, { 22, data_ov021_021d4e78 }, { 23, data_ov021_021d4e88 },
    { 24, data_ov021_021d4ed8 }, { 25, data_ov021_021d4f08 }, { 26, data_ov021_021d4e98 }, { 27, data_ov021_021d4d98 },
    { 28, data_ov021_021d4d88 }, { 29, data_ov021_021d4e38 }, { 30, data_ov021_021d4e18 }, { 31, data_ov021_021d4f58 },
    { 32, data_ov021_021d4ea8 }, { 33, data_ov021_021d4f18 }, { 34, data_ov021_021d4da8 }, { 35, data_ov021_021d4df8 },
    { 36, data_ov021_021d4e08 }, { 37, data_ov021_021d4f28 }, { 38, data_ov021_021d4f7c }, { 0x27, NULL },
};


void func_ov021_0219a87c(FieldActor *param_1) {
    Ov021BackForthWork *work;

    work = (Ov021BackForthWork *)ClearActorPositionBlock(param_1, sizeof(Ov021BackForthWork));
    if (func_ov021_0219b358(param_1) == 1) {
        func_ov021_0219b374(param_1, &work->spin);
    }
}

void func_ov021_0219a89c(FieldActor *param_1) {
    Ov021BackForthWork *work;
    u32 r;

    work = (Ov021BackForthWork *)GetNPCRailPosPtrAddr(param_1);
    do {
        r = data_ov021_021d4d60[work->state](param_1, work);
    } while (r == 1);
}

u32 func_ov021_0219a8c0(FieldActor *param_1, Ov021BackForthWork *work) {
    int dir;

    dir = GetDefaultActorDir(param_1);
    if (work->inverse == 1) {
        dir = GetInverseDirection((u16)dir);
    }
    SetActorMotionDir(param_1, dir);
    if (func_ov021_0219b358(param_1) == 0) {
        CheckSetActorFaceDir(param_1, dir);
    }
    work->state = 1;
    return 1;
}

u32 func_ov021_0219a900(FieldActor *param_1, Ov021BackForthWork *work) {
    u32 dir;
    s16 dx;
    s16 dz;
    s16 x;
    s16 z;
    u32 blocked;
    u32 acmd;
    u32 code;

    if (func_ov021_0219b2e8(param_1, &work->wait) == 0) {
        return 0;
    }
    if (work->inverse != 0) {
        dx = GetActorDefaultGPosX(param_1);
        dz = GetActorDefaultGPosZ(param_1);
        x = GetGPosX(param_1);
        z = GetGPosZ(param_1);
        if (dx == x && dz == z) {
            dir = GetInverseDirection(GetActorMotionDir(param_1));
            SetActorMotionDir(param_1, dir);
            if (func_ov021_0219b358(param_1) == 0) {
                CheckSetActorFaceDir(param_1, dir);
            }
            work->inverse = 0;
        }
    }
    dir = GetActorMotionDir(param_1);
    blocked = ActorRouteCollCheckOneTileInDir(param_1, dir);
    if (blocked & 1) {
        work->inverse = 1;
        dir = GetInverseDirection((u16)dir);
        blocked = ActorRouteCollCheckOneTileInDir(param_1, dir);
    }
    acmd = 0xc;
    if (func_ov021_0219b334(param_1) != 0 && func_ov021_0219b060() == 1 && func_ov010_02164168(param_1) == 1) {
        acmd = 0x10;
    }
    if (blocked != 0) {
        acmd = 0x20;
    }
    code = GetAcmdForDir(dir, acmd);
    func_ov010_0216d438(param_1, code);
    if (func_ov021_0219b358(param_1) == 1) {
        func_ov021_0219b390(param_1, &work->spin);
    }
    func_ov010_0216da44(param_1);
    work->state = 2;
    return 1;
}

u32 func_ov021_0219aa08(FieldActor *param_1, Ov021BackForthWork *work) {
    if (func_ov021_02197edc(param_1) == 1) {
        func_ov010_0216da50(param_1);
        if (func_ov021_0219b358(param_1) == 1) {
            func_ov021_0219b404(param_1, &work->spin);
        }
        work->state = 0;
    }
    return 0;
}

void func_ov021_0219aa38(FieldActor *param_1, u8 param_2, u8 param_3, int param_4) {
    Ov021RouteWork *work;

    work = (Ov021RouteWork *)ClearActorPositionBlock(param_1, sizeof(Ov021RouteWork));
    work->stepMax = param_2;
    work->axis = param_3;
    work->route = param_4;
    if (func_ov021_0219b358(param_1) == 1) {
        func_ov021_0219b374(param_1, &work->spin);
    }
}

void func_ov021_0219aa68(FieldActor *param_1) {
    func_ov021_0219aa38(param_1, 2, 0, 0xe);
}

void func_ov021_0219aa78(FieldActor *param_1) {
    func_ov021_0219aa38(param_1, 2, 0, 0xf);
}

void func_ov021_0219aa88(FieldActor *param_1) {
    func_ov021_0219aa38(param_1, 2, 1, 0x10);
}

void func_ov021_0219aa98(FieldActor *param_1) {
    func_ov021_0219aa38(param_1, 2, 1, 0x11);
}

void func_ov021_0219aaa8(FieldActor *param_1) {
    func_ov021_0219aa38(param_1, 2, 0, 0x12);
}

void func_ov021_0219aab8(FieldActor *param_1) {
    func_ov021_0219aa38(param_1, 2, 0, 0x13);
}

void func_ov021_0219aac8(FieldActor *param_1) {
    func_ov021_0219aa38(param_1, 2, 1, 0x14);
}

void func_ov021_0219aad8(FieldActor *param_1) {
    func_ov021_0219aa38(param_1, 2, 1, 0x15);
}

void func_ov021_0219aae8(FieldActor *param_1) {
    func_ov021_0219aa38(param_1, 2, 1, 0x16);
}

void func_ov021_0219aaf8(FieldActor *param_1) {
    func_ov021_0219aa38(param_1, 2, 1, 0x17);
}

void func_ov021_0219ab08(FieldActor *param_1) {
    func_ov021_0219aa38(param_1, 2, 0, 0x18);
}

void func_ov021_0219ab18(FieldActor *param_1) {
    func_ov021_0219aa38(param_1, 2, 0, 0x19);
}

void func_ov021_0219ab28(FieldActor *param_1) {
    func_ov021_0219aa38(param_1, 2, 1, 0x1a);
}

void func_ov021_0219ab38(FieldActor *param_1) {
    func_ov021_0219aa38(param_1, 2, 1, 0x1b);
}

void func_ov021_0219ab48(FieldActor *param_1) {
    func_ov021_0219aa38(param_1, 2, 0, 0x1c);
}

void func_ov021_0219ab58(FieldActor *param_1) {
    func_ov021_0219aa38(param_1, 2, 0, 0x1d);
}

void func_ov021_0219ab68(FieldActor *param_1) {
    Ov021RouteWork *work;
    u32 r;

    work = (Ov021RouteWork *)GetNPCRailPosPtrAddr(param_1);
    do {
        r = data_ov021_021d4d04[work->state](param_1, work);
    } while (r == 1);
}

u32 func_ov021_0219ab8c(FieldActor *param_1, Ov021RouteWork *work) {
    const s32 *p;
    u32 dir;
    s16 dx;
    s16 dz;
    s16 x;
    s16 z;
    u32 blocked;
    u32 acmd;
    u32 code;

    if (func_ov021_0219b2e8(param_1, &work->wait) == 0) {
        return 0;
    }
    if (work->step == work->stepMax) {
        if (work->axis == 0) {
            if (GetActorDefaultGPosX(param_1) == GetGPosX(param_1)) {
                work->step++;
            }
        } else if (GetActorDefaultGPosZ(param_1) == GetGPosZ(param_1)) {
            work->step++;
        }
    }
    if (work->step == 3) {
        dx = GetActorDefaultGPosX(param_1);
        dz = GetActorDefaultGPosZ(param_1);
        x = GetGPosX(param_1);
        z = GetGPosZ(param_1);
        if (dx == x && dz == z) {
            work->step = 0;
        }
    }
    p = func_ov021_0219b120(work->route);
    dir = p[work->step];
    SetActorMotionDir(param_1, dir);
    if (func_ov021_0219b358(param_1) == 0) {
        CheckSetActorFaceDir(param_1, dir);
    }
    blocked = ActorRouteCollCheckOneTileInDir(param_1, dir);
    if (blocked & 1) {
        work->step++;
        dir = p[work->step];
        SetActorMotionDir(param_1, dir);
        if (func_ov021_0219b358(param_1) == 0) {
            CheckSetActorFaceDir(param_1, dir);
        }
        blocked = ActorRouteCollCheckOneTileInDir(param_1, dir);
    }
    acmd = 0xc;
    if (func_ov021_0219b334(param_1) != 0 && func_ov021_0219b060() == 1 && func_ov010_02164168(param_1) == 1) {
        acmd = 0x10;
    }
    if (blocked != 0) {
        acmd = 0x20;
    }
    code = GetAcmdForDir(dir, acmd);
    func_ov010_0216d438(param_1, code);
    if (func_ov021_0219b358(param_1) == 1) {
        func_ov021_0219b390(param_1, &work->spin);
    }
    func_ov010_0216da44(param_1);
    work->state = 1;
    return 1;
}

u32 func_ov021_0219ace8(FieldActor *param_1, Ov021RouteWork *work) {
    if (func_ov021_02197edc(param_1) == 1) {
        func_ov010_0216da50(param_1);
        if (func_ov021_0219b358(param_1) == 1) {
            func_ov021_0219b404(param_1, &work->spin);
        }
        work->state = 0;
    }
    return 0;
}

void func_ov021_0219ad18(FieldActor *param_1, u8 param_2, u8 param_3, int param_4) {
    Ov021RouteRevWork *work;

    work = (Ov021RouteRevWork *)ClearActorPositionBlock(param_1, sizeof(Ov021RouteRevWork));
    work->stepMax = param_2;
    work->axis = param_3;
    work->route = param_4;
    if (func_ov021_0219b358(param_1) == 1) {
        func_ov021_0219b374(param_1, &work->spin);
    }
}

void func_ov021_0219ad48(FieldActor *param_1) {
    func_ov021_0219ad18(param_1, 2, 1, 0x1e);
}

void func_ov021_0219ad58(FieldActor *param_1) {
    func_ov021_0219ad18(param_1, 2, 1, 0x1f);
}

void func_ov021_0219ad68(FieldActor *param_1) {
    func_ov021_0219ad18(param_1, 2, 0, 0x20);
}

void func_ov021_0219ad78(FieldActor *param_1) {
    func_ov021_0219ad18(param_1, 2, 0, 0x21);
}

void func_ov021_0219ad88(FieldActor *param_1) {
    func_ov021_0219ad18(param_1, 2, 1, 0x22);
}

void func_ov021_0219ad98(FieldActor *param_1) {
    func_ov021_0219ad18(param_1, 2, 1, 0x23);
}

void func_ov021_0219ada8(FieldActor *param_1) {
    func_ov021_0219ad18(param_1, 2, 0, 0x24);
}

void func_ov021_0219adb8(FieldActor *param_1) {
    func_ov021_0219ad18(param_1, 2, 0, 0x25);
}

void func_ov021_0219adc8(FieldActor *param_1) {
    Ov021RouteRevWork *work;
    u32 r;

    work = (Ov021RouteRevWork *)GetNPCRailPosPtrAddr(param_1);
    do {
        r = data_ov021_021d4d78[work->state](param_1, work);
    } while (r == 1);
}

void func_ov021_0219adec(Ov021RouteRevWork *work) {
    if (work->reverse == 1) {
        work->step = work->step - 1;
        if (work->step < 0) {
            work->step = 3;
        }
    } else {
        work->step = work->step + 1;
    }
}

u32 func_ov021_0219ae10(FieldActor *param_1, Ov021RouteRevWork *work) {
    u16 dir;

    dir = GetActorEvType(param_1);
    if (dir == 10) {
        dir = GetActorFaceDir(param_1);
        if (func_ov021_021ae198(param_1, dir, (u32)GetActorUserParam(param_1, 0), (EncountSystem *)0x0) != -1) {
            dir = GetAcmdForDir(dir, 0x30);
            func_ov010_0216d438(param_1, dir);
            func_ov010_0216da44(param_1);
            work->state = 1;
            return 1;
        }
    }
    work->state = 2;
    return 1;
}

u32 func_ov021_0219ae68(FieldActor *param_1, Ov021RouteRevWork *work) {
    if (func_ov021_02197edc(param_1) == 1) {
        u16 found = 0xff;
        u16 code = GetActorMoveCode(param_1);
        const ActorMoveCodePair *pair;

        for (pair = data_ov021_021d4f90; pair->first != 0xff; pair++) {
            if (pair->first == code) {
                found = pair->second;
            } else if (pair->second == code) {
                found = pair->first;
            }
            if (found != 0xff) {
                s32 step = 4 - (work->step + 1);

                ChangeActorMoveCodeSeq(param_1, found);
                work->step = step;
                break;
            }
        }
        func_ov010_0216da50(param_1);
        work->state = 2;
    }
    return 0;
}

u32 func_ov021_0219aec8(FieldActor *param_1, Ov021RouteRevWork *work) {
    s32 blocked;
    const s32 *p;
    s32 dir;

    if (!func_ov021_0219b2e8(param_1, &work->wait)) {
        return 0;
    }
    if (work->step == work->stepMax) {
        if (work->axis == 0) {
            if (GetActorDefaultGPosX(param_1) == GetGPosX(param_1)) {
                func_ov021_0219adec(work);
            }
        } else if (GetActorDefaultGPosZ(param_1) == GetGPosZ(param_1)) {
            func_ov021_0219adec(work);
        }
    }
    if (work->step == 3) {
        s16 defaultX = GetActorDefaultGPosX(param_1);
        s16 defaultZ = GetActorDefaultGPosZ(param_1);
        s16 x = GetGPosX(param_1);
        s16 z = GetGPosZ(param_1);
        if (defaultX == x && defaultZ == z) {
            work->step = 0;
        }
    }
    p = func_ov021_0219b120(work->route);
    dir = p[work->step];
    SetActorMotionDir(param_1, dir);
    if (func_ov021_0219b358(param_1) == 0) {
        CheckSetActorFaceDir(param_1, dir);
    }
    blocked = ActorRouteCollCheckOneTileInDir(param_1, dir);
    if (blocked & 1) {
        func_ov021_0219adec(work);
        dir = p[work->step];
        SetActorMotionDir(param_1, dir);
        if (func_ov021_0219b358(param_1) == 0) {
            CheckSetActorFaceDir(param_1, dir);
        }
        blocked = ActorRouteCollCheckOneTileInDir(param_1, dir);
    }
    u32 speed = 12;

    if (func_ov021_0219b334(param_1)) {
        if (func_ov021_0219b060() == TRUE && func_ov010_02164168(param_1) == TRUE) {
            speed = 16;
        }
    }
    if (blocked) {
        speed = 32;
    }
    StartDirAcmd(param_1, dir, speed);
    if (func_ov021_0219b358(param_1) == TRUE) {
        func_ov021_0219b390(param_1, &work->spin);
    }
    func_ov010_0216da44(param_1);
    work->state = 3;
    return TRUE;
}

u32 func_ov021_0219b030(FieldActor *param_1, Ov021RouteRevWork *work) {
    if (func_ov021_02197edc(param_1) == 1) {
        func_ov010_0216da50(param_1);
        if (func_ov021_0219b358(param_1) == 1) {
            func_ov021_0219b404(param_1, &work->spin);
        }
        work->state = 0;
    }
    return 0;
}

u32 func_ov021_0219b060(void) {
    if ((GCTX_HIDGetHeldKeys() & 2) != 0) {
        return 1;
    }
    return 0;
}

u32 func_ov021_0219b074(FieldActor *param_1) {
    FieldActor *player;
    u16 acmd;

    player = FindPlayerFieldActor(GetActorMModelSystem(param_1));
    if (player != NULL) {
        acmd = FldAct_GetAcmd(player);
        switch (acmd) {
        case 0x58:
        case 0x59:
        case 0x5a:
        case 0x5b:
        case 0xa3:
        case 0xa4:
        case 0xa5:
        case 0xa6:
            return 1;
        }
    }
    return 0;
}

u32 func_ov021_0219b0c4(void) {
    if ((GCTX_HIDGetPressedKeys() & 2) != 0) {
        return 1;
    }
    return 0;
}

s32 func_ov021_0219b0d8(const s32 *param_1, s32 param_2) {
    s32 val;
    s32 i;

    i = 0;
    val = *param_1;
    while (param_2 != val) {
        i = i + 1;
        val = param_1[i];
    }
    return i;
}

s32 func_ov021_0219b0f0(const s32 *param_1, s32 param_2) {
    u32 max;

    max = func_ov021_0219b0d8(param_1, param_2);
    max = GFL_RandomLC(max);
    return param_1[max];
}

s32 func_ov021_0219b104(s32 param_1, s32 param_2) {
    const s32 *ptr;
    u32 max;

    ptr = func_ov021_0219b120(param_1);
    max = func_ov021_0219b0d8(ptr, param_2);
    max = GFL_RandomLC(max);
    return ptr[max];
}

const s32 *func_ov021_0219b120(s32 key) {
    const ActorDirTableEntry *entry = data_ov021_021d50f8;

    for (; entry->key != 0x27; entry++) {
        if (entry->key == key) {
            return entry->dirs;
        }
    }
    return NULL;
}

u32 func_ov021_0219b13c(FieldActor *param_1) {
    u16 ev;
    u16 code;
    s32 v;
    u32 i;
    FieldActor *player;
    s16 px;
    s16 pz;
    s16 x;
    s16 z;
    u16 param;
    s32 minX;
    s32 maxX;
    s32 minZ;
    s32 maxZ;

    ev = GetActorEvType(param_1);
    if (ev != 1 && ev != 2) {
        return 9;
    }
    if (func_ov021_0219b074(param_1) == 0) {
        return 9;
    }
    i = 0;
    code = GetActorMoveCode(param_1);
    do {
        v = data_ov021_021d4fec[i++];
    } while (v != code && v != 0xff);
    if (code != v) {
        return 9;
    }
    player = FindPlayerFieldActor(GetActorMModelSystem(param_1));
    if (player == NULL) {
        return 9;
    }
    if (FldAct_GetGPosY(player) != FldAct_GetGPosY(param_1)) {
        return 9;
    }
    px = GetGPosX(player);
    pz = GetGPosZ(player);
    param = GetActorUserParam(param_1, 0);
    x = GetGPosX(param_1);
    z = GetGPosZ(param_1);
    minX = x - param;
    maxX = x + param;
    minZ = z - param;
    maxZ = z + param;
    if (minZ <= pz && maxZ >= pz && minX <= px && maxX >= px) {
        return GetDirFromPosToPos(x, z, px, pz);
    }
    return 9;
}

u32 func_ov021_0219b200(FieldActor *param_1, s32 param_2, s32 param_3) {
    const s32 *dirs = func_ov021_0219b120(param_2);
    s32 count = func_ov021_0219b0d8(dirs, param_3);
    FieldActor *player;
    s32 ax;
    s32 az;
    s32 px;
    s32 pz;
    s32 xDir;
    s32 dirZ;
    s32 code;
    s32 i;

    if (count == 1) {
        return 9;
    }
    code = func_ov021_0219b13c(param_1);
    if (code == 9) {
        return code;
    }
    i = 0;
    do {
        if (code == dirs[i]) {
            return code;
        }
        i++;
    } while (i < count);
    player = FindPlayerFieldActor(GetActorMModelSystem(param_1));
    xDir = DIR_NONE;
    dirZ = DIR_NONE;
    ax = GetGPosX(param_1);
    az = GetGPosZ(param_1);
    px = GetGPosX(player);
    pz = GetGPosZ(player);
    if (ax > px) {
        xDir = DIR_LEFT;
    } else if (ax < px) {
        xDir = DIR_RIGHT;
    }
    if (az > pz) {
        dirZ = DIR_UP;
    } else if (az < pz) {
        dirZ = DIR_DOWN;
    }
    i = 0;
    if (xDir == DIR_NONE) {
        do {
            if (dirZ == dirs[i]) {
                return dirZ;
            }
            i++;
        } while (i < count);
    } else if (dirZ == DIR_NONE) {
        do {
            if (xDir == dirs[i]) {
                return xDir;
            }
            i++;
        } while (i < count);
    } else {
        do {
            if (xDir == dirs[i]) {
                return xDir;
            }
            if (dirZ == dirs[i]) {
                return dirZ;
            }
            i++;
        } while (i < count);
    }
error:
    return 9;
}

u32 func_ov021_0219b2e8(FieldActor *param_1, u8 *param_2) {
    u16 actorEvType;
    BOOL BVar2;

    actorEvType = GetActorEvType(param_1);
    if (actorEvType != 0xc) {
        return 1;
    }
    if (*param_2 == 0) {
        BVar2 = func_ov010_02164168(param_1);
        if (BVar2 == 0) {
            return 0;
        }
        if (func_ov021_0219b0c4() == 0) {
            return 0;
        }
        actorEvType = GetActorUserParam(param_1, 1);
        *param_2 = (u8)actorEvType;
    }
    if (*param_2 != 0) {
        *param_2 = *param_2 + -1;
    }
    return 1;
}

u32 func_ov021_0219b334(FieldActor *param_1) {
    u16 actorEvType;

    actorEvType = GetActorEvType(param_1);
    if ((actorEvType == 0xD) || ((u32) (u16) (actorEvType + 0xFFF1) <= 4U)) {
        return 1U;
    }
    return 0U;
}

u32 func_ov021_0219b358(FieldActor *param_1) {
    u32 ev = GetActorEvType(param_1);
    if (ev == 7 || ev == 8 || ev - 0x11 <= 1) {
        return 1;
    }
    return 0;
}

void func_ov021_0219b374(FieldActor *param_1, Ov021SpinWork *spin) {
    u16 ev;

    ev = GetActorEvType(param_1);
    if (ev == 7 || ev == 0x11) {
        spin->turn = 0;
    } else {
        spin->turn = 1;
    }
}

void func_ov021_0219b390(FieldActor *param_1, Ov021SpinWork *work) {
    u16 dir;
    s32 i;
    s32 val;

    dir = GetActorFaceDir(param_1);
    for (i = 0; i < 4 && dir != data_ov021_021d4fcc[work->turn][i]; i++) {
    }
    i++;
    work->dir = dir;
    i %= 4;
    val = data_ov021_021d4fcc[work->turn][i];
    if (CheckActorFlag(param_1, 8)) {
        work->hadFlag = 1;
    } else {
        work->hadFlag = 0;
    }
    CheckSetActorFaceDir(param_1, val);
    SetActorFlag(param_1, 8);
}

void func_ov021_0219b404(FieldActor *param_1, Ov021SpinWork *spin) {
    if (spin->hadFlag == 0) {
        ClearActorFlag(param_1, 8);
    }
}
