// Black's code at ov021 0x0219b414-0x0219b8bc: pokebw2's fldmmdl_evtype.c

#include "types.h"
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

u32 func_ov021_02197edc(FieldActor *param_1);

typedef void (*ActorEvTypeInitFunc)(FieldActor *actor);

typedef u32 (*ActorEvTypeFunc)(FieldActor *actor);

// The functions of each event type: its start, its check and its update. Defined after the functions, as pokebw2 has
// them: there MWCC's size sort lays the local initializers of func_ov021_0219b59c and func_ov021_0219b75c out in
// Black's order
static const ActorEvTypeInitFunc data_ov021_021d5278[21];
static const ActorEvTypeFunc data_ov021_021d52cc[21];
static const ActorEvTypeFunc data_ov021_021d5320[21];

void func_ov021_0219b440(FieldActor *param_1);
u32 func_ov021_0219b45c(FieldActor *param_1);
u32 func_ov021_0219b478(FieldActor *param_1);
u32 func_ov021_0219b8a8(void);

void func_ov021_0219b414(FieldActor *param_1) {
    func_ov021_0219b440(param_1);
}

BOOL func_ov021_0219b41c(FieldActor *param_1) {
    if (!func_ov021_0219b45c(param_1)) {
        return FALSE;
    }
    if (func_ov021_0219b478(param_1)) {
        return TRUE;
    }
    return FALSE;
}

void func_ov021_0219b440(FieldActor *param_1) {
    data_ov021_021d5278[GetActorEvType(param_1)](param_1);
}

u32 func_ov021_0219b45c(FieldActor *param_1) {
    return data_ov021_021d52cc[GetActorEvType(param_1)](param_1);
}

u32 func_ov021_0219b478(FieldActor *param_1) {
    return data_ov021_021d5320[GetActorEvType(param_1)](param_1);
}

u32 func_ov021_0219b494(FieldActor *param_1) {
    s16 gPosX;
    s16 initGPosX;

    gPosX = GetGPosX(param_1);
    initGPosX = FldAct_GetInitGPosX(param_1);
    if (gPosX != initGPosX) {
        return 1;
    }
    gPosX = GetGPosZ(param_1);
    initGPosX = FldAct_GetInitGPosZ(param_1);
    if (gPosX != initGPosX) {
        return 1;
    }
    return 0;
}

u32 func_ov021_0219b4c8(FieldActor *param_1) {
    s16 gPosX;
    s16 initGPosX;

    gPosX = GetGPosX(param_1);
    initGPosX = FldAct_GetInitGPosX(param_1);
    if (gPosX != initGPosX) {
        return 0;
    }
    gPosX = GetGPosZ(param_1);
    initGPosX = FldAct_GetInitGPosZ(param_1);
    if (gPosX == initGPosX) {
        return 1;
    }
    return 0;
}

void func_ov021_0219b4fc(FieldActor *param_1) {
}

u32 func_ov021_0219b500(FieldActor *param_1) {
    return 0;
}

u32 func_ov021_0219b504(FieldActor *param_1) {
    return 0;
}

// The 9-byte block of the codes that turn in place (pokebw2's ActorSpinWork): a state machine in
// state, one in phase
typedef struct {
    u8 state;
    u8 phase;
    s8 count;
    s8 limit;
    s8 dir;
    s8 variant;
    s8 turn;
    s8 turns;
    s8 wait;
} ActorSpinWork;

void func_ov021_0219b508(FieldActor *param_1) {
    ActorSpinWork *work = func_ov010_0216d6a0(param_1, sizeof(ActorSpinWork));

    work->limit = GetActorUserParam(param_1, 1);
}

u32 func_ov021_0219b520(FieldActor *param_1) {
    ActorSpinWork *s;

    s = func_ov010_0216d6b8(param_1);
    switch (s->state) {
    case 0:
        if (func_ov021_0219b494(param_1) == 1) {
            s->state = s->state + 1;
        }
        break;
    case 1:
        if (func_ov021_0219b4c8(param_1) == 0) {
            break;
        }
        s->count = s->count + 1;
        if (s->count < s->limit) {
            s->state = 0;
            break;
        }
        s->state = s->state + 1;
        /* fall through */
    case 2:
        if (IsActorFlag16(param_1) == 1) {
            break;
        }
        s->state = s->state + 1;
        s->count = 0;
        s->phase = 0;
        /* fall through */
    case 3:
        return 1;
    }
    return 0;
}

u32 func_ov021_0219b59c(FieldActor *param_1) {
    ActorSpinWork *work = func_ov010_0216d6b8(param_1);
    s32 wait;

    switch (work->phase) {
    case 0: {
        s32 firstDirs[4] = { 0, 0, 1, 1 };
        u16 face = GetActorFaceDir(param_1);

        work->dir = face;
        work->variant = firstDirs[face];
        work->phase++;
    }
    case 1: {
        s32 turnDirs[2][2] = { { 2, 3 }, { 0, 1 } };

        StartDirAcmd(param_1, turnDirs[work->variant][work->turn], 0);
        work->phase++;
    }
    case 2:
        if (func_ov021_02197edc(param_1) == FALSE) {
            return TRUE;
        }
        work->phase++;
    case 3:
        wait = 8;
        if (GetActorEvType(param_1) == 14 && func_ov021_0219b8a8() == TRUE && func_ov010_02164168(param_1) == TRUE) {
            wait = 4;
        }
        work->wait++;
        if (work->wait < wait) {
            return TRUE;
        }
        work->wait = 0;
        work->turns++;
        if (work->turns < 4) {
            work->turn = (work->turn + 1) & 1;
            work->phase = 1;
            return TRUE;
        }
        CheckSetActorFaceDir(param_1, work->dir);
        work->phase++;
        work->turns = 0;
        work->state = 0;
    }
    return FALSE;
}

void func_ov021_0219b6b0(FieldActor *param_1) {
    ActorSpinWork *work;
    u8 flag;
    u16 ev;

    work = func_ov010_0216d6a0(param_1, sizeof(ActorSpinWork));
    work->limit = GetActorUserParam(param_1, 1);
    ev = GetActorEvType(param_1);
    flag = 1;
    if (ev == 5 || ev == 0x14 || ev == 0xf) {
        flag = 0;
    }
    work->variant = flag;
}

u32 func_ov021_0219b6e0(FieldActor *param_1) {
    ActorSpinWork *s;

    s = func_ov010_0216d6b8(param_1);
    switch (s->state) {
    case 0:
        if (func_ov021_0219b494(param_1) == 1) {
            s->state = s->state + 1;
        }
        break;
    case 1:
        if (func_ov021_0219b4c8(param_1) == 0) {
            break;
        }
        s->count = s->count + 1;
        if (s->count < s->limit) {
            s->state = 0;
            break;
        }
        s->state = s->state + 1;
        /* fall through */
    case 2:
        if (IsActorFlag16(param_1) == 1) {
            break;
        }
        s->state = s->state + 1;
        s->count = 0;
        s->phase = 0;
        /* fall through */
    case 3:
        return 1;
    }
    return 0;
}

u32 func_ov021_0219b75c(FieldActor *param_1) {
    s32 dirs[2][4] = { { 0, 2, 1, 3 }, { 0, 3, 1, 2 } };
    ActorSpinWork *work = func_ov010_0216d6b8(param_1);
    s32 wait;
    s32 i;
    u32 face;

    switch (work->phase) {
    case 0:
        face = GetActorFaceDir(param_1);
        i = 0;
        while (i < 4 && face != dirs[work->variant][i]) {
            i++;
        }
        i++;
        work->dir = face;
        work->turn = i % 4;
        work->phase++;
    case 1:
        StartDirAcmd(param_1, dirs[work->variant][work->turn], 0);
        work->phase++;
    case 2:
        if (func_ov021_02197edc(param_1) == FALSE) {
            return TRUE;
        }
        work->phase++;
    case 3:
        wait = 8;
        {
            u16 evType = GetActorEvType(param_1);

            if ((evType == 15 || evType == 16) && func_ov021_0219b8a8() == TRUE &&
                func_ov010_02164168(param_1) == TRUE) {
                wait = 4;
            }
        }
        work->wait++;
        if (work->wait < wait) {
            return TRUE;
        }
        work->wait = 0;
        work->turns++;
        if (work->turns < 4) {
            work->turn = (work->turn + 1) % 4;
            work->phase = 1;
            return TRUE;
        }
        CheckSetActorFaceDir(param_1, work->dir);
        work->phase++;
        work->turns = 0;
        work->state = 0;
    }
    return FALSE;
}

u32 func_ov021_0219b8a8(void) {
    if ((GCTX_HIDGetHeldKeys() & 2) != 0) {
        return 1;
    }
    return 0;
}

static const ActorEvTypeInitFunc data_ov021_021d5278[] = {
    func_ov021_0219b4fc, func_ov021_0219b4fc, func_ov021_0219b4fc,
    func_ov021_0219b4fc, func_ov021_0219b508, func_ov021_0219b6b0,
    func_ov021_0219b6b0, func_ov021_0219b4fc, func_ov021_0219b4fc,
    func_ov021_0219b4fc, func_ov021_0219b4fc, func_ov021_0219b4fc,
    func_ov021_0219b4fc, func_ov021_0219b4fc, func_ov021_0219b508,
    func_ov021_0219b6b0, func_ov021_0219b6b0, func_ov021_0219b4fc,
    func_ov021_0219b4fc, func_ov021_0219b4fc, func_ov021_0219b6b0,
};

static const ActorEvTypeFunc data_ov021_021d52cc[] = {
    func_ov021_0219b500, func_ov021_0219b500, func_ov021_0219b500,
    func_ov021_0219b500, func_ov021_0219b520, func_ov021_0219b6e0,
    func_ov021_0219b6e0, func_ov021_0219b500, func_ov021_0219b500,
    func_ov021_0219b500, func_ov021_0219b500, func_ov021_0219b500,
    func_ov021_0219b500, func_ov021_0219b500, func_ov021_0219b520,
    func_ov021_0219b6e0, func_ov021_0219b6e0, func_ov021_0219b500,
    func_ov021_0219b500, func_ov021_0219b500, func_ov021_0219b6e0,
};

static const ActorEvTypeFunc data_ov021_021d5320[] = {
    func_ov021_0219b504, func_ov021_0219b504, func_ov021_0219b504,
    func_ov021_0219b504, func_ov021_0219b59c, func_ov021_0219b75c,
    func_ov021_0219b75c, func_ov021_0219b504, func_ov021_0219b504,
    func_ov021_0219b504, func_ov021_0219b504, func_ov021_0219b504,
    func_ov021_0219b504, func_ov021_0219b504, func_ov021_0219b59c,
    func_ov021_0219b75c, func_ov021_0219b75c, func_ov021_0219b504,
    func_ov021_0219b504, func_ov021_0219b504, func_ov021_0219b75c,
};
