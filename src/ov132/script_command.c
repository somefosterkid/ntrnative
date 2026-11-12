// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov209/script_command.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/musical/sta_act_effect.h"
#include "app/musical/sta_act_light.h"
#include "app/musical/sta_act_obj.h"
#include "app/musical/sta_act_poke.h"
#include "app/musical/sta_act_script.h"
#include "app/musical/sta_acting.h"
#include "gfl/heap.h"
#include "gfl/random.h"
#include "gfl/tcb.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/fx/fx_vec.h"
#include "nitro/gx.h"
#include "nitro/math.h"
#include "nnsys/snd.h"
#include "system/gf_font.h"
#include "system/vm.h"

int StaScriptCmd_StopSeq(struct VM *vm, void *arg);
BOOL func_ov132_021fb698(VM *vm, void *work);

// Overlay 209's script_command.c: the commands of the stage's scripts. Each reads its arguments from the script as
// 32-bit words. A command that moves something over time starts a task, which frees its work when it is done, and
// most of them can make the script wait for it

// The target of func_ov132_021fb1f4 that means the script's own Pokémon
#define STA_SCRIPT_OWN_POKE ((u32) - 1)

// Frames of each hop of the Pokémon as they line up at the end
#define STA_SCRIPT_LINE_UP_JUMP_PERIOD 15

// The file name of the allocations, which the original has in .data ahead of the command table rather than as
// string literals after it
static char data_ov132_021fdd00[] = "script_command.c";

// A value that goes from start to end over frames
typedef struct {
    StaActing *stage;
    int start;
    int end;
    u16 frames;
    u16 count;
} StaScriptMoveValue;

// A position that goes from start to end over frames, by step each frame
typedef struct {
    StaActing *stage;
    VecFx32 start;
    VecFx32 end;
    VecFx32 step;
    u16 frames;
    u16 count;
} StaScriptMoveVec;

// A position that follows a Pokémon at an offset for frames
typedef struct {
    StaActing *stage;
    StaActPoke *poke;
    VecFx32 offset;
    u16 frames;
    u16 count;
} StaScriptFollow;

// A period that repeats a number of times
typedef struct {
    StaActing *stage;
    u32 count;
    u32 period;
    u32 repeat;
} StaScriptRepeat;

// The work of the curtain and scroll tasks
typedef struct {
    StaScriptMoveValue move;
    StaActScriptSys *sys;
    StaActScriptTask *task;
} StaScriptValueWork;

// The work of the tasks that move a Pokémon, an object or a light
typedef struct {
    void *target;
    StaScriptMoveVec move;
    StaActScriptSys *sys;
    StaActScriptTask *task;
} StaScriptMoveWork;

typedef struct {
    StaScriptRepeat repeat;
    StaActPoke *poke;
    fx32 height;
    StaActScriptSys *sys;
    StaActScriptTask *task;
} StaScriptJumpWork;

// Black 2's; twinned code confirms the offsets of move, poke, sys and task in Black
typedef struct {
    StaScriptMoveValue move;
    u32 unused[2];
    StaActPoke *poke;
    StaActScriptSys *sys;
    StaActScriptTask *task;
} StaScriptRotateWork;

// Creates an emitter at a random position in a box every period
typedef struct {
    StaScriptRepeat repeat;
    StaActEffect *effect;
    VecFx32 min;
    VecFx32 range;
    u16 emitterNo;
    StaActScriptSys *sys;
    StaActScriptTask *task;
} StaScriptEmitterWork;

// Black 2's; twinned code confirms the offsets of light, follow, sys and task in Black
typedef struct {
    u8 pokePos;
    StaActLight *light;
    StaScriptFollow follow;
    u32 unused[2];
    StaActScriptSys *sys;
    StaActScriptTask *task;
} StaScriptLightFollowWork;

// pokebw2's StaScript_UpdateMoveValue
static BOOL func_ov132_021fae1c(StaScriptMoveValue *move, int *value) {
    int diff;

    move->count++;
    if (move->count >= move->frames) {
        *value = move->end;
        return TRUE;
    }
    diff = move->end - move->start;
    *value = move->start + diff * move->count / move->frames;
    return FALSE;
}

// pokebw2's StaScript_UpdateMoveVec
static BOOL func_ov132_021fae4c(StaScriptMoveVec *move, VecFx32 *pos) {
    move->count++;
    if (move->count >= move->frames) {
        pos->x = move->end.x;
        pos->y = move->end.y;
        pos->z = move->end.z;
        return TRUE;
    }
    VEC_MultAdd(FX32_CONST(move->count), &move->step, &move->start, pos);
    return FALSE;
}

// pokebw2's StaScript_UpdateFollow
static BOOL func_ov132_021faea8(StaScriptFollow *follow, VecFx32 *pos) {
    VecFx32 pokePos;

    func_ov112_021f8044(func_ov132_021f9da0(follow->stage), follow->poke, &pokePos);
    VEC_Add(&pokePos, &follow->offset, pos);
    follow->count++;
    if (follow->count >= follow->frames) {
        return TRUE;
    }
    return FALSE;
}

// 1 when a period ends, 2 when the last one does, else 0
// pokebw2's StaScript_UpdateRepeat
static u32 func_ov132_021faee8(StaScriptRepeat *repeat) {
    repeat->count++;
    if (repeat->count >= repeat->period) {
        repeat->repeat--;
        repeat->count = 0;
        if (repeat->repeat == 0) {
            return 2;
        }
        return 1;
    }
    return 0;
}

// pokebw2's StaScriptCmd_End
static BOOL func_ov132_021faf10(VM *vm, void *work) {
    StaActScript *script = work;

    script->flags |= STA_SCRIPT_FLAG_END;
    return TRUE;
}

// pokebw2's StaScriptCmd_Nop
static BOOL func_ov132_021faf1c(VM *vm, void *work) {
    StaActScript *script = work;

    VM_Read32(script->vm);
    return FALSE;
}

// pokebw2's StaScriptCmd_Wait
static BOOL func_ov132_021faf28(VM *vm, void *work) {
    StaActScript *script = work;

    script->wait = VM_Read32(script->vm);
    return TRUE;
}

// pokebw2's StaScriptCmd_WaitFrame
static BOOL func_ov132_021faf38(VM *vm, void *work) {
    StaActScript *script = work;
    u32 frame = VM_Read32(script->vm);

    if (script->frame >= frame) {
        return FALSE;
    }
    script->wait = frame - script->frame;
    return TRUE;
}

// pokebw2's StaScriptCmd_SyncWait
static BOOL func_ov132_021faf54(VM *vm, void *work) {
    StaActScript *script = work;

    VM_Read32(script->vm);
    if (script->flags & STA_SCRIPT_FLAG_SYNC) {
        script->flags |= STA_SCRIPT_FLAG_WAIT_SYNC;
    }
    return TRUE;
}

static void func_ov132_021fb0dc(TCB *tcb, void *data);
static void func_ov132_021fb190(TCB *tcb, void *data);
static void func_ov132_021fb528(TCB *tcb, void *data);
static void func_ov132_021fb8cc(TCB *tcb, void *data);
static void func_ov132_021fba14(TCB *tcb, void *data);
static void func_ov132_021fbf74(TCB *tcb, void *data);
static void func_ov132_021fc178(TCB *tcb, void *data);
static void func_ov132_021fc34c(TCB *tcb, void *data);
static void func_ov132_021fc490(TCB *tcb, void *data);

// pokebw2's StaScriptCmd_CurtainOpen
static BOOL func_ov132_021faf70(VM *vm, void *work) {
    StaActScript *script = work;
    StaActScriptSys *sys = script->sys;
    BOOL wait = VM_Read32(script->vm);
    StaScriptValueWork *curtain = GFL_HeapAllocate(sys->heapId, sizeof(StaScriptValueWork), FALSE, data_ov132_021fdd00, 420);

    curtain->sys = sys;
    curtain->move.stage = sys->stage;
    curtain->move.count = 0;
    curtain->move.start = func_ov132_021f9dfc(sys->stage);
    curtain->move.end = 0xe0;
    curtain->move.frames = MATH_ABS(curtain->move.end - curtain->move.start) / 2;
    curtain->task = func_ov132_021fada0(sys, func_ov132_021fb0dc, curtain, 10);
    if (wait == TRUE) {
        script->wait = curtain->move.frames;
        return TRUE;
    }
    return FALSE;
}

// pokebw2's StaScriptCmd_CurtainClose
static BOOL func_ov132_021fafdc(VM *vm, void *work) {
    StaActScript *script = work;
    StaActScriptSys *sys = script->sys;
    BOOL wait = VM_Read32(script->vm);
    StaScriptValueWork *curtain = GFL_HeapAllocate(sys->heapId, sizeof(StaScriptValueWork), FALSE, data_ov132_021fdd00, 449);

    curtain->sys = sys;
    curtain->move.stage = sys->stage;
    curtain->move.count = 0;
    curtain->move.start = func_ov132_021f9dfc(sys->stage);
    curtain->move.end = 0;
    curtain->move.frames = MATH_ABS(curtain->move.end - curtain->move.start) / 2;
    curtain->task = func_ov132_021fada0(sys, func_ov132_021fb0dc, curtain, 10);
    func_ov132_021f96d0(sys->stage);
    if (wait == TRUE) {
        script->wait = curtain->move.frames;
        return TRUE;
    }
    return FALSE;
}

// pokebw2's StaScriptCmd_CurtainMove
static BOOL func_ov132_021fb050(VM *vm, void *work) {
    StaActScript *script = work;
    StaActScriptSys *sys = script->sys;
    u32 frames = VM_Read32(script->vm);
    u32 offset = VM_Read32(script->vm);
    BOOL wait = VM_Read32(script->vm);

    if (frames == 0) {
        func_ov132_021f9e00(sys->stage, offset);
    } else {
        StaScriptValueWork *curtain = GFL_HeapAllocate(sys->heapId, sizeof(StaScriptValueWork), FALSE, data_ov132_021fdd00, 489);

        curtain->sys = sys;
        curtain->move.stage = sys->stage;
        curtain->move.count = 0;
        curtain->move.start = func_ov132_021f9dfc(sys->stage);
        curtain->move.end = offset;
        curtain->move.frames = frames;
        curtain->task = func_ov132_021fada0(sys, func_ov132_021fb0dc, curtain, 10);
        if (wait == TRUE) {
            script->wait = curtain->move.frames;
            return TRUE;
        }
    }
    return FALSE;
}

// pokebw2's StaScript_CurtainTask
static void func_ov132_021fb0dc(TCB *tcb, void *data) {
    StaScriptValueWork *curtain = data;
    int offset;
    BOOL end = func_ov132_021fae1c(&curtain->move, &offset);

    func_ov132_021f9e00(curtain->move.stage, offset);
    if (end == TRUE) {
        func_ov132_021fadf8(curtain->sys, curtain->task);
    }
}

// pokebw2's StaScriptCmd_Scroll
static BOOL func_ov132_021fb104(VM *vm, void *work) {
    StaActScript *script = work;
    StaActScriptSys *sys = script->sys;
    u32 frames = VM_Read32(script->vm);
    u32 offset = VM_Read32(script->vm);
    BOOL wait = VM_Read32(script->vm);

    if (frames == 0) {
        func_ov132_021f9e08(sys->stage, offset);
    } else {
        StaScriptValueWork *scroll = GFL_HeapAllocate(sys->heapId, sizeof(StaScriptValueWork), FALSE, data_ov132_021fdd00, 549);

        scroll->sys = sys;
        scroll->move.stage = sys->stage;
        scroll->move.count = 0;
        scroll->move.start = func_ov132_021f9e04(sys->stage);
        scroll->move.end = offset;
        scroll->move.frames = frames;
        scroll->task = func_ov132_021fada0(sys, func_ov132_021fb190, scroll, 10);
        if (wait == TRUE) {
            script->wait = scroll->move.frames;
            return TRUE;
        }
    }
    return FALSE;
}

// pokebw2's StaScript_ScrollTask
static void func_ov132_021fb190(TCB *tcb, void *data) {
    StaScriptValueWork *scroll = data;
    int offset;
    BOOL end = func_ov132_021fae1c(&scroll->move, &offset);

    func_ov132_021f9e08(scroll->move.stage, offset);
    if (end == TRUE) {
        func_ov132_021f9e40(scroll->move.stage, FALSE);
        func_ov132_021fadf8(scroll->sys, scroll->task);
    }
}

// pokebw2's StaScriptCmd_Nop2
static BOOL func_ov132_021fb1c0(VM *vm, void *work) {
    StaActScript *script = work;

    VM_Read32(script->vm);
    return FALSE;
}

// pokebw2's StaScriptCmd_ButtonEnable
static BOOL func_ov132_021fb1cc(VM *vm, void *work) {
    StaActScript *script = work;
    StaActScriptSys *sys = script->sys;

    func_ov132_021f9d80(sys->stage);
    func_ov132_021f9ad8(sys->stage, 0, 0);
    return FALSE;
}

// pokebw2's StaScriptCmd_ButtonDisable
static BOOL func_ov132_021fb1e4(VM *vm, void *work) {
    StaActScript *script = work;

    func_ov132_021f9d90(script->sys->stage);
    return FALSE;
}

// The Pokémon a command's target means, as a mask of positions
// pokebw2's StaScript_GetPokeMask
static u32 func_ov132_021fb1f4(StaActScript *script, u32 target) {
    if (target == STA_SCRIPT_OWN_POKE) {
        return script->pokeNo;
    }
    return 1 << target;
}

// pokebw2's StaScriptCmd_PokeShow
static BOOL func_ov132_021fb208(VM *vm, void *work) {
    StaActScript *script = work;
    StaActScriptSys *sys = script->sys;
    u32 mask;
    StaActPokeSys *pokeSys;
    BOOL show;
    u8 i;

    mask = func_ov132_021fb1f4(script, VM_Read32(script->vm));
    show = VM_Read32(script->vm);
    pokeSys = func_ov132_021f9da0(sys->stage);

    for (i = 0; i < 4; i++) {
        if ((1 << i) & mask) {
            func_ov112_021f80d4(pokeSys, func_ov132_021f9da4(sys->stage, i), show);
        }
    }
    return FALSE;
}

// pokebw2's StaScriptCmd_PokeFlip
static BOOL func_ov132_021fb258(VM *vm, void *work) {
    StaActScript *script = work;
    StaActScriptSys *sys = script->sys;
    u32 mask = func_ov132_021fb1f4(script, VM_Read32(script->vm));
    u32 flip = VM_Read32(script->vm);
    StaActPokeSys *pokeSys = func_ov132_021f9da0(sys->stage);
    u8 i;

    for (i = 0; i < 4; i++) {
        if ((1 << i) & mask) {
            StaActPoke *poke = func_ov132_021f9da4(sys->stage, i);
            BOOL isFlip = FALSE;

            if (flip) {
                isFlip = TRUE;
            }
            func_ov112_021f812c(pokeSys, poke, isFlip);
        }
    }
    return FALSE;
}

// pokebw2's StaScriptCmd_PokeMove
static BOOL func_ov132_021fb2b0(VM *vm, void *work) {
    StaActScript *script = work;
    StaActScriptSys *sys = script->sys;
    u32 mask;
    int frames;
    BOOL wait;
    StaActPokeSys *pokeSys;
    fx32 x;
    fx32 y;
    fx32 z;
    u8 i;

    mask = func_ov132_021fb1f4(script, VM_Read32(script->vm));
    frames = VM_Read32(script->vm);
    x = VM_Read32(script->vm);
    y = VM_Read32(script->vm);
    z = VM_Read32(script->vm);
    wait = VM_Read32(script->vm);
    pokeSys = func_ov132_021f9da0(sys->stage);

    for (i = 0; i < 4; i++) {
        if ((1 << i) & mask) {
            StaActPoke *poke = func_ov132_021f9da4(sys->stage, i);

            if (frames == 0) {
                VecFx32 pos;

                pos.x = x;
                pos.y = y;
                pos.z = z;
                func_ov112_021f8054(pokeSys, poke, &pos);
            } else {
                VecFx32 diff;
                StaScriptMoveWork *move = GFL_HeapAllocate(sys->heapId, sizeof(StaScriptMoveWork), FALSE, data_ov132_021fdd00, 730);

                move->sys = sys;
                move->target = poke;
                move->move.stage = sys->stage;
                move->move.count = 0;
                func_ov112_021f8044(pokeSys, poke, &move->move.start);
                move->move.end.x = x;
                move->move.end.y = y;
                move->move.end.z = z;
                move->move.frames = frames;
                VEC_Subtract(&move->move.end, &move->move.start, &diff);
                move->move.step.x = diff.x / frames;
                move->move.step.y = diff.y / frames;
                move->move.step.z = diff.z / frames;
                move->task = func_ov132_021fada0(sys, func_ov132_021fb528, move, 10);
            }
        }
    }
    if (wait == TRUE) {
        script->wait = frames;
        return TRUE;
    }
    return FALSE;
}

// pokebw2's StaScriptCmd_PokeMoveBy
static BOOL func_ov132_021fb3e0(VM *vm, void *work) {
    StaActScript *script = work;
    StaActScriptSys *sys = script->sys;
    u32 mask = func_ov132_021fb1f4(script, VM_Read32(script->vm));
    int frames = VM_Read32(script->vm);
    fx32 x = VM_Read32(script->vm);
    fx32 y = VM_Read32(script->vm);
    fx32 z = VM_Read32(script->vm);
    BOOL wait = VM_Read32(script->vm);
    u8 i;
    StaActPokeSys *pokeSys = func_ov132_021f9da0(sys->stage);

    for (i = 0; i < 4; i++) {
        if ((1 << i) & mask) {
            StaActPoke *poke = func_ov132_021f9da4(sys->stage, i);

            if (frames == 0) {
                VecFx32 pos;

                func_ov112_021f8044(pokeSys, poke, &pos);
                pos.x += x;
                pos.y += y;
                pos.z += z;
                func_ov112_021f8054(pokeSys, poke, &pos);
            } else {
                StaScriptMoveWork *move = GFL_HeapAllocate(sys->heapId, sizeof(StaScriptMoveWork), FALSE, data_ov132_021fdd00, 795);

                move->sys = sys;
                move->target = poke;
                move->move.stage = sys->stage;
                move->move.count = 0;
                func_ov112_021f8044(pokeSys, poke, &move->move.start);
                VEC_Set(&move->move.end, move->move.start.x + x, move->move.start.y + y, move->move.start.z + z);
                move->move.frames = frames;
                move->move.step.x = x / frames;
                move->move.step.y = y / frames;
                move->move.step.z = z / frames;
                move->task = func_ov132_021fada0(sys, func_ov132_021fb528, move, 10);
            }
        }
    }
    if (wait == TRUE) {
        script->wait = frames;
        return TRUE;
    }
    return FALSE;
}

// pokebw2's StaScript_PokeMoveTask
static void func_ov132_021fb528(TCB *tcb, void *data) {
    StaScriptMoveWork *move = data;
    StaActPokeSys *pokeSys = func_ov132_021f9da0(move->move.stage);
    VecFx32 pos;
    BOOL end = func_ov132_021fae4c(&move->move, &pos);

    func_ov112_021f8054(pokeSys, move->target, &pos);
    if (end == TRUE) {
        func_ov132_021fadf8(move->sys, move->task);
    }
}

// pokebw2's StaScriptCmd_PokeStopAnime
static BOOL func_ov132_021fb55c(VM *vm, void *work) {
    StaActScript *script = work;
    StaActScriptSys *sys = script->sys;
    u32 mask = func_ov132_021fb1f4(script, VM_Read32(script->vm));
    StaActPokeSys *pokeSys = func_ov132_021f9da0(sys->stage);
    u8 i;

    for (i = 0; i < 4; i++) {
        if ((1 << i) & mask) {
            func_ov112_021f80b0(pokeSys, func_ov132_021f9da4(sys->stage, i));
        }
    }
    return FALSE;
}

// pokebw2's StaScriptCmd_PokeStartAnime
static BOOL func_ov132_021fb5a4(VM *vm, void *work) {
    StaActScript *script = work;
    StaActScriptSys *sys = script->sys;
    u32 mask = func_ov132_021fb1f4(script, VM_Read32(script->vm));
    StaActPokeSys *pokeSys = func_ov132_021f9da0(sys->stage);
    u8 i;

    for (i = 0; i < 4; i++) {
        if ((1 << i) & mask) {
            func_ov112_021f80a0(pokeSys, func_ov132_021f9da4(sys->stage, i));
        }
    }
    return FALSE;
}

// pokebw2's StaScriptCmd_PokeChangeAnime
static BOOL func_ov132_021fb5ec(VM *vm, void *work) {
    StaActScript *script = work;
    StaActScriptSys *sys = script->sys;
    u32 mask = func_ov132_021fb1f4(script, VM_Read32(script->vm));
    u32 anime = VM_Read32(script->vm);
    StaActPokeSys *pokeSys = func_ov132_021f9da0(sys->stage);
    u8 i;

    for (i = 0; i < 4; i++) {
        if ((1 << i) & mask) {
            func_ov112_021f80c8(pokeSys, func_ov132_021f9da4(sys->stage, i), anime);
        }
    }
    return FALSE;
}

// pokebw2's StaScriptCmd_PokeFront
static BOOL func_ov132_021fb640(VM *vm, void *work) {
    StaActScript *script = work;
    StaActScriptSys *sys = script->sys;
    u32 mask = func_ov132_021fb1f4(script, VM_Read32(script->vm));
    u32 front = VM_Read32(script->vm);
    StaActPokeSys *pokeSys = func_ov132_021f9da0(sys->stage);
    u8 i;

    for (i = 0; i < 4; i++) {
        if ((1 << i) & mask) {
            func_ov112_021f8140(pokeSys, func_ov132_021f9da4(sys->stage, i), front != 0);
        }
    }
    return FALSE;
}

BOOL func_ov132_021fb698(VM *vm, void *work) {
    StaActScript *script = work;
    StaActScriptSys *sys = script->sys;
    u32 mask = func_ov132_021fb1f4(script, VM_Read32(script->vm));
    u32 front = VM_Read32(script->vm);
    StaActPokeSys *pokeSys = func_ov132_021f9da0(sys->stage);
    u8 i;

    for (i = 0; i < 4; i++) {
        if ((1 << i) & mask) {
            func_ov112_021f8154(pokeSys, func_ov132_021f9da4(sys->stage, i), front != 0);
        }
    }
    return FALSE;
}


// pokebw2's StaScriptCmd_PokeAppeal
static BOOL func_ov132_021fb6f0(VM *vm, void *work) {
    StaActScript *script = work;
    StaActScriptSys *sys = script->sys;
    u32 mask = func_ov132_021fb1f4(script, VM_Read32(script->vm));
    func_ov132_021f9da0(sys->stage);
    u8 i;

    for (i = 0; i < 4; i++) {
        if ((1 << i) & mask) {
            func_ov132_021f9da4(sys->stage, i);

            func_ov132_021f9d18(sys->stage, i);
        }
    }
    return FALSE;
}

// pokebw2's StaScriptCmd_AudienceFocus
static BOOL func_ov132_021fb738(VM *vm, void *work) {
    StaActScript *script = work;
    StaActScriptSys *sys = script->sys;
    u32 mask = func_ov132_021fb1f4(script, VM_Read32(script->vm));
    u8 i;

    for (i = 0; i < 4; i++) {
        if ((1 << i) & mask) {
            func_ov132_021f95cc(sys->stage, i, TRUE);
        }
    }
    return FALSE;
}

// pokebw2's StaScriptCmd_AudienceUnfocus
static BOOL func_ov132_021fb774(VM *vm, void *work) {
    StaActScript *script = work;
    StaActScriptSys *sys = script->sys;

    func_ov132_021f95cc(sys->stage, 0, FALSE);
    func_ov132_021f95cc(sys->stage, 1, FALSE);
    func_ov132_021f95cc(sys->stage, 2, FALSE);
    func_ov132_021f95cc(sys->stage, 3, FALSE);
    return FALSE;
}

// pokebw2's StaScriptCmd_AppealScript
static BOOL func_ov132_021fb7a4(VM *vm, void *work) {
    StaActScript *script = work;
    StaActScriptSys *sys = script->sys;
    u32 pos = VM_Read32(script->vm);
    u32 file = VM_Read32(script->vm);

    func_ov132_021f9140(sys->stage, file, pos);
    func_ov132_021f9ad8(sys->stage, 1, pos);
    return FALSE;
}

// pokebw2's StaScriptCmd_PokeScript
static BOOL func_ov132_021fb7d8(VM *vm, void *work) {
    StaActScript *script = work;
    StaActScriptSys *sys = script->sys;
    u32 mask = func_ov132_021fb1f4(script, VM_Read32(script->vm));
    u32 file = VM_Read32(script->vm);

    func_ov132_021f91a8(sys->stage, file, mask);
    return FALSE;
}

// pokebw2's StaScriptCmd_PokeJump
static BOOL func_ov132_021fb808(VM *vm, void *work) {
    StaActScript *script = work;
    StaActScriptSys *sys = script->sys;
    u32 mask;
    BOOL wait;
    fx32 height;
    u32 period;
    u32 count;
    StaActPokeSys *pokeSys;
    u8 i;

    mask = func_ov132_021fb1f4(script, VM_Read32(script->vm));
    period = VM_Read32(script->vm);
    count = VM_Read32(script->vm);
    height = VM_Read32(script->vm);
    wait = VM_Read32(script->vm);
    pokeSys = func_ov132_021f9da0(sys->stage);

    for (i = 0; i < 4; i++) {
        if ((1 << i) & mask) {
            StaScriptJumpWork *jump = GFL_HeapAllocate(sys->heapId, sizeof(StaScriptJumpWork), FALSE, data_ov132_021fdd00, 1091);

            jump->sys = sys;
            jump->poke = func_ov132_021f9da4(sys->stage, i);
            jump->height = height;
            jump->repeat.stage = sys->stage;
            jump->repeat.count = 0;
            jump->repeat.period = period;
            jump->repeat.repeat = count;
            jump->task = func_ov132_021fada0(sys, func_ov132_021fb8cc, jump, 10);
        }
    }
    if (wait == TRUE) {
        script->wait = count * period;
        return TRUE;
    }
    return FALSE;
}

// pokebw2's StaScript_PokeJumpTask
static void func_ov132_021fb8cc(TCB *tcb, void *data) {
    StaScriptJumpWork *jump = data;
    StaActPokeSys *pokeSys = func_ov132_021f9da0(jump->repeat.stage);

    if (func_ov132_021faee8(&jump->repeat) == 2) {
        VecFx32 offset = { 0, 0, 0 };

        func_ov112_021f8084(pokeSys, jump->poke, &offset);
        func_ov132_021fadf8(jump->sys, jump->task);
    } else {
        VecFx32 offset;

        offset.x = 0;
        offset.y = -FX_Mul(FX_SinIdx((jump->repeat.count << 15) / jump->repeat.period), jump->height);
        offset.z = 0;
        func_ov112_021f8084(pokeSys, jump->poke, &offset);
    }
}

// pokebw2's StaScriptCmd_PokeRotate
static BOOL func_ov132_021fb948(VM *vm, void *work) {
    StaActScript *script = work;
    StaActScriptSys *sys = script->sys;
    u32 mask = func_ov132_021fb1f4(script, VM_Read32(script->vm));
    u32 frames = VM_Read32(script->vm);
    int start = VM_Read32(script->vm);
    int angle = VM_Read32(script->vm);
    BOOL wait = VM_Read32(script->vm);
    func_ov132_021f9da0(sys->stage);
    u8 i;

    for (i = 0; i < 4; i++) {
        if ((1 << i) & mask) {
            StaScriptRotateWork *rotate =
                GFL_HeapAllocate(sys->heapId, sizeof(StaScriptRotateWork), FALSE, data_ov132_021fdd00, 1171);

            rotate->sys = sys;
            rotate->poke = func_ov132_021f9da4(sys->stage, i);
            rotate->move.stage = sys->stage;
            rotate->move.start = start;
            rotate->move.end = start + angle;
            rotate->move.frames = frames;
            rotate->move.count = 0;
            rotate->task = func_ov132_021fada0(sys, func_ov132_021fba14, rotate, 10);
        }
    }
    if (wait == TRUE) {
        script->wait = frames;
        return TRUE;
    }
    return FALSE;
}

// pokebw2's StaScript_PokeRotateTask
static void func_ov132_021fba14(TCB *tcb, void *data) {
    StaScriptRotateWork *rotate = data;
    StaActPokeSys *pokeSys = func_ov132_021f9da0(rotate->move.stage);
    int angle;

    if (func_ov132_021fae1c(&rotate->move, &angle) == TRUE) {
        func_ov112_021f8068(pokeSys, rotate->poke, DEG_TO_IDX(rotate->move.end));
        func_ov132_021fadf8(rotate->sys, rotate->task);
    } else {
        func_ov112_021f8068(pokeSys, rotate->poke, DEG_TO_IDX(angle));
    }
}

// Lines the Pokémon up around the one with the most points, the others ahead of it in order of where they stand,
// hopping as they go, and scrolls to them
// pokebw2's StaScriptCmd_LineUp
static BOOL func_ov132_021fba6c(VM *vm, void *work) {
    StaActScript *script = work;
    StaActScriptSys *sys = script->sys;
    int frames = VM_Read32(script->vm);
    BOOL wait = VM_Read32(script->vm);
    StaActPokeSys *pokeSys = func_ov132_021f9da0(sys->stage);
    u8 maxRank = 0;
    s8 ranks[4] = { 0, 0, 0, 0 };
    u16 maxPoints = 0;
    u8 top;
    fx32 totalX;
    fx32 topX;
    fx32 x[4];
    VecFx32 pos;
    u8 i;
    u8 j;

    for (i = 0; i < 4; i++) {
        u16 points = func_ov132_021f9e34(sys->stage, i) + func_ov132_021f9e20(sys->stage, i);

        if (maxPoints < points) {
            maxPoints = points;
        }
    }
    for (i = 0; i < 4; i++) {
        u16 points = func_ov132_021f9e34(sys->stage, i) + func_ov132_021f9e20(sys->stage, i);

        if (maxPoints == points) {
            top = i;
        }
    }
    for (i = 0; i < 4; i++) {
        func_ov112_021f8044(pokeSys, func_ov132_021f9da4(sys->stage, i), &pos);
        x[i] = pos.x;
        if (i == top) {
            topX = pos.x;
        }
    }
    for (i = 0; i < 4; i++) {
        x[i] -= topX;
    }
    for (i = 0; i < 4; i++) {
        if (i != top) {
            for (j = 0; j < 4; j++) {
                if (i != j) {
                    if (x[i] > 0 && x[j] >= 0 && x[i] > x[j]) {
                        ranks[i]++;
                    } else if (x[i] < 0 && x[j] <= 0 && x[i] < x[j]) {
                        ranks[i]--;
                    }
                }
            }
            if (maxRank < MATH_ABS(ranks[i])) {
                maxRank = MATH_ABS(ranks[i]);
            }
        }
    }

    totalX = 0;
    for (i = 0; i < 4; i++) {
        StaScriptMoveWork *move = GFL_HeapAllocate(sys->heapId, sizeof(StaScriptMoveWork), FALSE, data_ov132_021fdd00, 1328);

        move->sys = sys;
        move->target = func_ov132_021f9da4(sys->stage, i);
        move->move.stage = sys->stage;
        move->move.count = 0;
        func_ov112_021f8044(pokeSys, move->target, &move->move.start);
        move->move.end.x = topX + ranks[i] * FX32_CONST(64);
        move->move.end.y = move->move.start.y;
        move->move.end.z = move->move.start.z;
        move->move.frames = MATH_ABS(ranks[i]) * frames / maxRank;
        move->move.step.x = (move->move.end.x - move->move.start.x) / move->move.frames;
        move->move.step.y = (move->move.end.y - move->move.start.y) / move->move.frames;
        move->move.step.z = (move->move.end.z - move->move.start.z) / move->move.frames;
        move->task = func_ov132_021fada0(sys, func_ov132_021fb528, move, 10);
        if (move->move.frames != 0) {
            StaScriptJumpWork *jump = GFL_HeapAllocate(sys->heapId, sizeof(StaScriptJumpWork), FALSE, data_ov132_021fdd00, 1355);
            u16 jumpFrames = move->move.frames;
            u8 rest = jumpFrames % STA_SCRIPT_LINE_UP_JUMP_PERIOD;
            int extra = rest != 0 ? 1 : 0;

            jump->sys = sys;
            jump->poke = func_ov132_021f9da4(sys->stage, i);
            jump->height = FX32_CONST(8);
            jump->repeat.stage = sys->stage;
            jump->repeat.count = 0;
            jump->repeat.period = STA_SCRIPT_LINE_UP_JUMP_PERIOD;
            jump->repeat.repeat = (u8)(jumpFrames / STA_SCRIPT_LINE_UP_JUMP_PERIOD + extra);
            jump->task = func_ov132_021fada0(sys, func_ov132_021fb8cc, jump, 10);
        }
        totalX += move->move.end.x;
    }

    {
        StaScriptValueWork *scroll = GFL_HeapAllocate(sys->heapId, sizeof(StaScriptValueWork), FALSE, data_ov132_021fdd00, 1379);

        scroll->sys = sys;
        scroll->move.stage = sys->stage;
        scroll->move.count = 0;
        scroll->move.start = func_ov132_021f9e04(sys->stage);
        scroll->move.end = FX_FX32_TO_F32(totalX / 4) - 128.0f;
        scroll->move.frames = frames;
        scroll->task = func_ov132_021fada0(sys, func_ov132_021fb190, scroll, 10);
    }
    func_ov132_021f9e40(sys->stage, TRUE);
    func_ov132_021f9e3c(sys->stage, TRUE);
    if (wait == TRUE) {
        script->wait = frames;
        return TRUE;
    }
    return FALSE;
}

// pokebw2's StaScriptCmd_SetPokeMask
static BOOL func_ov132_021fbd78(VM *vm, void *work) {
    StaActScript *script = work;
    u32 poke0 = VM_Read32(script->vm);
    u32 poke1 = VM_Read32(script->vm);
    u32 poke2 = VM_Read32(script->vm);
    u32 poke3 = VM_Read32(script->vm);

    script->pokeNo = 0;
    if (poke0 == TRUE) {
        script->pokeNo += 1;
    }
    if (poke1 == TRUE) {
        script->pokeNo += 2;
    }
    if (poke2 == TRUE) {
        script->pokeNo += 4;
    }
    if (poke3 == TRUE) {
        script->pokeNo += 8;
    }
    return FALSE;
}

// pokebw2's StaScriptCmd_ObjAdd
static BOOL func_ov132_021fbdcc(VM *vm, void *work) {
    StaActScript *script = work;
    StaActScriptSys *sys = script->sys;
    u32 index = VM_Read32(script->vm);
    u32 objId = VM_Read32(script->vm);

    func_ov132_021f9db8(sys->stage, func_ov112_021f6b18(func_ov132_021f9dac(sys->stage), objId), index);
    return FALSE;
}

// pokebw2's StaScriptCmd_ObjDel
static BOOL func_ov132_021fbe00(VM *vm, void *work) {
    StaActScript *script = work;
    StaActScriptSys *sys = script->sys;
    u32 index = VM_Read32(script->vm);
    StaActObjSys *objSys = func_ov132_021f9dac(sys->stage);

    func_ov112_021f6b80(objSys, func_ov132_021f9db0(sys->stage, index));
    func_ov132_021f9db8(sys->stage, NULL, index);
    return FALSE;
}

// pokebw2's StaScriptCmd_ObjShow
static BOOL func_ov132_021fbe38(VM *vm, void *work) {
    StaActScript *script = work;
    StaActScriptSys *sys = script->sys;
    u32 index = VM_Read32(script->vm);
    StaActObjSys *objSys = func_ov132_021f9dac(sys->stage);

    func_ov112_021f6bc0(objSys, func_ov132_021f9db0(sys->stage, index), TRUE);
    return FALSE;
}

// pokebw2's StaScriptCmd_ObjHide
static BOOL func_ov132_021fbe64(VM *vm, void *work) {
    StaActScript *script = work;
    StaActScriptSys *sys = script->sys;
    u32 index = VM_Read32(script->vm);
    StaActObjSys *objSys = func_ov132_021f9dac(sys->stage);

    func_ov112_021f6bc0(objSys, func_ov132_021f9db0(sys->stage, index), FALSE);
    return FALSE;
}

// pokebw2's StaScriptCmd_ObjMove
static BOOL func_ov132_021fbe90(VM *vm, void *work) {
    StaActScript *script = work;
    StaActScriptSys *sys = script->sys;
    u32 index = VM_Read32(script->vm);
    int frames = VM_Read32(script->vm);
    fx32 x = VM_Read32(script->vm);
    fx32 y = VM_Read32(script->vm);
    fx32 z = VM_Read32(script->vm);
    StaActObjSys *objSys = func_ov132_021f9dac(sys->stage);
    StaActObj *obj = func_ov132_021f9db0(sys->stage, index);

    if (frames == 0) {
        VecFx32 pos;

        pos.x = x;
        pos.y = y;
        pos.z = z;
        func_ov112_021f6b9c(objSys, obj, &pos);
    } else {
        VecFx32 diff;
        StaScriptMoveWork *move = GFL_HeapAllocate(sys->heapId, sizeof(StaScriptMoveWork), FALSE, data_ov132_021fdd00, 1537);

        move->sys = sys;
        move->target = obj;
        move->move.stage = sys->stage;
        move->move.count = 0;
        func_ov112_021f6bb0(objSys, obj, &move->move.start);
        move->move.end.x = x;
        move->move.end.y = y;
        move->move.end.z = z;
        move->move.frames = frames;
        VEC_Subtract(&move->move.end, &move->move.start, &diff);
        move->move.step.x = diff.x / frames;
        move->move.step.y = diff.y / frames;
        move->move.step.z = diff.z / frames;
        move->task = func_ov132_021fada0(sys, func_ov132_021fbf74, move, 10);
    }
    return FALSE;
}

// pokebw2's StaScript_ObjMoveTask
static void func_ov132_021fbf74(TCB *tcb, void *data) {
    StaScriptMoveWork *move = data;
    StaActObjSys *objSys = func_ov132_021f9dac(move->move.stage);
    VecFx32 pos;
    BOOL end = func_ov132_021fae4c(&move->move, &pos);

    func_ov112_021f6b9c(objSys, move->target, &pos);
    if (end == TRUE) {
        func_ov132_021fadf8(move->sys, move->task);
    }
}

// pokebw2's StaScriptCmd_EffectAdd
static BOOL func_ov132_021fbfa8(VM *vm, void *work) {
    StaActScript *script = work;
    StaActScriptSys *sys = script->sys;
    u32 index = VM_Read32(script->vm);
    u32 effectNo = VM_Read32(script->vm);

    func_ov132_021f9dcc(sys->stage, func_ov132_021f9edc(func_ov132_021f9dc0(sys->stage), effectNo + 51), index);
    return FALSE;
}

// pokebw2's StaScriptCmd_EffectDel
static BOOL func_ov132_021fbfdc(VM *vm, void *work) {
    StaActScript *script = work;
    StaActScriptSys *sys = script->sys;
    u32 index = VM_Read32(script->vm);
    StaActEffectSys *effectSys = func_ov132_021f9dc0(sys->stage);

    func_ov132_021f9f58(effectSys, func_ov132_021f9dc4(sys->stage, index));
    func_ov132_021f9dcc(sys->stage, NULL, index);
    return FALSE;
}

// Stage positions are in fixed point pixels from the top left, the particles' from the bottom left in 16ths
// pokebw2's StaScriptCmd_EmitterCreate
static BOOL func_ov132_021fc014(VM *vm, void *work) {
    StaActScript *script = work;
    StaActScriptSys *sys = script->sys;
    u32 index = VM_Read32(script->vm);
    u32 emitterNo = VM_Read32(script->vm);
    fx32 x = VM_Read32(script->vm);
    fx32 y = VM_Read32(script->vm);
    fx32 z = VM_Read32(script->vm);
    StaActEffect *effect = func_ov132_021f9dc4(sys->stage, index);
    VecFx32 pos;

    pos.x = x / 16;
    pos.y = (FX32_CONST(192) - y) / 16;
    pos.z = z;
    func_ov132_021f9f74(effect, emitterNo, &pos);
    return FALSE;
}

// pokebw2's StaScriptCmd_EmitterDelete
static BOOL func_ov132_021fc080(VM *vm, void *work) {
    StaActScript *script = work;
    StaActScriptSys *sys = script->sys;
    u32 index = VM_Read32(script->vm);
    u32 emitterNo = VM_Read32(script->vm);

    func_ov132_021f9fb4(func_ov132_021f9dc4(sys->stage, index), emitterNo);
    return FALSE;
}

// pokebw2's StaScriptCmd_EmitterRandom
static BOOL func_ov132_021fc0ac(VM *vm, void *work) {
    StaActScript *script = work;
    StaActScriptSys *sys = script->sys;
    u32 index = VM_Read32(script->vm);
    u32 emitterNo = VM_Read32(script->vm);
    u32 period = VM_Read32(script->vm);
    u32 count = VM_Read32(script->vm);
    fx32 minX = VM_Read32(script->vm);
    fx32 minY = VM_Read32(script->vm);
    fx32 minZ = VM_Read32(script->vm);
    fx32 maxX = VM_Read32(script->vm);
    fx32 maxY = VM_Read32(script->vm);
    fx32 maxZ = VM_Read32(script->vm);
    StaActEffect *effect = func_ov132_021f9dc4(sys->stage, index);
    StaScriptEmitterWork *emitter = GFL_HeapAllocate(sys->heapId, sizeof(StaScriptEmitterWork), FALSE, data_ov132_021fdd00, 1681);

    emitter->sys = sys;
    emitter->effect = effect;
    emitter->emitterNo = emitterNo;
    emitter->min.x = minX;
    emitter->min.y = minY;
    emitter->min.z = minZ;
    emitter->range.x = maxX - minX;
    emitter->range.y = maxY - minY;
    emitter->range.z = maxZ - minZ;
    emitter->repeat.stage = sys->stage;
    emitter->repeat.count = 0;
    emitter->repeat.period = period;
    emitter->repeat.repeat = count;
    emitter->task = func_ov132_021fada0(sys, func_ov132_021fc178, emitter, 10);
    return FALSE;
}

// pokebw2's StaScript_EmitterTask
static void func_ov132_021fc178(TCB *tcb, void *data) {
    StaScriptEmitterWork *emitter = data;
    u32 ret = func_ov132_021faee8(&emitter->repeat);

    if (ret == 2) {
        func_ov132_021fadf8(emitter->sys, emitter->task);
    } else if (ret == 1) {
        VecFx32 pos;

        pos.x = (emitter->min.x + GFL_RandomLCAlt(emitter->range.x)) / 16;
        pos.y = (FX32_CONST(192) - (emitter->min.y + GFL_RandomLCAlt(emitter->range.y))) / 16;
        pos.z = emitter->min.z + GFL_RandomLCAlt(emitter->range.z);
        func_ov132_021f9f74(emitter->effect, emitter->emitterNo, &pos);
    }
}

// pokebw2's StaScriptCmd_LightAdd
static BOOL func_ov132_021fc1d4(VM *vm, void *work) {
    StaActScript *script = work;
    StaActScriptSys *sys = script->sys;
    u32 index = VM_Read32(script->vm);
    u32 unk = VM_Read32(script->vm);
    StaActLightSys *lightSys = func_ov132_021f9dd4(sys->stage);
    StaActLight *light = func_ov112_021f6e78(lightSys, 1);

    func_ov112_021f6f14(lightSys, light, GX_RGB(31, 31, 0), 16);
    func_ov112_021f6f1c(lightSys, light, unk, 0);
    func_ov132_021f9de8(sys->stage, light, index);
    func_ov132_021f95e8(sys->stage);
    return FALSE;
}

// pokebw2's StaScriptCmd_LightDel
static BOOL func_ov132_021fc22c(VM *vm, void *work) {
    StaActScript *script = work;
    StaActScriptSys *sys = script->sys;
    u32 index = VM_Read32(script->vm);
    StaActLightSys *lightSys = func_ov132_021f9dd4(sys->stage);

    func_ov112_021f6ee4(lightSys, func_ov132_021f9ddc(sys->stage, index));
    func_ov132_021f9de8(sys->stage, NULL, index);
    func_ov132_021f95e8(sys->stage);
    return FALSE;
}

// pokebw2's StaScriptCmd_LightMove
static BOOL func_ov132_021fc268(VM *vm, void *work) {
    StaActScript *script = work;
    StaActScriptSys *sys = script->sys;
    u32 index = VM_Read32(script->vm);
    int frames = VM_Read32(script->vm);
    fx32 x = VM_Read32(script->vm);
    fx32 y = VM_Read32(script->vm);
    fx32 z = VM_Read32(script->vm);
    StaActLightSys *lightSys = func_ov132_021f9dd4(sys->stage);
    StaActLight *light = func_ov132_021f9ddc(sys->stage, index);

    if (frames == 0) {
        VecFx32 pos;

        pos.x = x;
        pos.y = y;
        pos.z = z;
        func_ov112_021f6ef4(lightSys, light, &pos);
    } else {
        VecFx32 diff;
        StaScriptMoveWork *move = GFL_HeapAllocate(sys->heapId, sizeof(StaScriptMoveWork), FALSE, data_ov132_021fdd00, 1796);

        move->sys = sys;
        move->target = light;
        move->move.stage = sys->stage;
        move->move.count = 0;
        func_ov112_021f6f04(lightSys, light, &move->move.start);
        move->move.end.x = x;
        move->move.end.y = y;
        move->move.end.z = z;
        move->move.frames = frames;
        VEC_Subtract(&move->move.end, &move->move.start, &diff);
        move->move.step.x = diff.x / frames;
        move->move.step.y = diff.y / frames;
        move->move.step.z = diff.z / frames;
        move->task = func_ov132_021fada0(sys, func_ov132_021fc34c, move, 10);
    }
    return FALSE;
}

// pokebw2's StaScript_LightMoveTask
static void func_ov132_021fc34c(TCB *tcb, void *data) {
    StaScriptMoveWork *move = data;
    StaActLightSys *lightSys = func_ov132_021f9dd4(move->move.stage);
    VecFx32 pos;
    BOOL end = func_ov132_021fae4c(&move->move, &pos);

    func_ov112_021f6ef4(lightSys, move->target, &pos);
    if (end == TRUE) {
        func_ov132_021fadf8(move->sys, move->task);
    }
}

// pokebw2's StaScriptCmd_LightFollow
static BOOL func_ov132_021fc380(VM *vm, void *work) {
    StaActScript *script = work;
    StaActScriptSys *sys = script->sys;
    u32 index;
    u32 mask;
    u32 frames;
    StaActLightSys *lightSys;
    StaActLight *light;
    StaActPokeSys *pokeSys;
    fx32 x;
    fx32 y;
    fx32 z;
    u8 i;

    index = VM_Read32(script->vm);
    mask = func_ov132_021fb1f4(script, VM_Read32(script->vm));
    frames = VM_Read32(script->vm);
    x = VM_Read32(script->vm);
    y = VM_Read32(script->vm);
    z = VM_Read32(script->vm);
    lightSys = func_ov132_021f9dd4(sys->stage);
    light = func_ov132_021f9ddc(sys->stage, index);
    pokeSys = func_ov132_021f9da0(sys->stage);

    for (i = 0; i < 4; i++) {
        StaActPoke *poke = func_ov132_021f9da4(sys->stage, i);

        if ((1 << i) & mask) {
            if (frames == 0) {
                VecFx32 pos;
                VecFx32 offset;

                func_ov112_021f8044(pokeSys, poke, &pos);
                offset.x = x;
                offset.y = y;
                offset.z = z;
                VEC_Add(&pos, &offset, &pos);
                func_ov112_021f6ef4(lightSys, light, &pos);
            } else {
                StaScriptLightFollowWork *follow;

                func_ov132_021f9df4(sys->stage);
                follow = GFL_HeapAllocate(sys->heapId, sizeof(StaScriptLightFollowWork), FALSE, data_ov132_021fdd00, 1869);

                follow->sys = sys;
                follow->pokePos = i;
                follow->light = light;
                follow->follow.stage = sys->stage;
                follow->follow.poke = poke;
                follow->follow.count = 0;
                follow->follow.offset.x = x;
                follow->follow.offset.y = y;
                follow->follow.offset.z = z;
                follow->follow.frames = frames;
                follow->task = func_ov132_021fada0(sys, func_ov132_021fc490, follow, 7);
            }
        }
    }
    return FALSE;
}

// pokebw2's StaScript_LightFollowTask
static void func_ov132_021fc490(TCB *tcb, void *data) {
    StaScriptLightFollowWork *follow = data;
    StaActLightSys *lightSys = func_ov132_021f9dd4(follow->follow.stage);
    func_ov132_021f9df4(follow->follow.stage);
    VecFx32 pos;
    BOOL end = func_ov132_021faea8(&follow->follow, &pos);

    func_ov112_021f6ef4(lightSys, follow->light, &pos);
    if (end == TRUE) {
        func_ov132_021fadf8(follow->sys, follow->task);
    }
}

// pokebw2's StaScriptCmd_LightColor
static BOOL func_ov132_021fc4cc(VM *vm, void *work) {
    StaActScript *script = work;
    StaActScriptSys *sys = script->sys;
    u32 index = VM_Read32(script->vm);
    u32 r = VM_Read32(script->vm);
    u32 g = VM_Read32(script->vm);
    u32 b = VM_Read32(script->vm);
    u32 alpha = VM_Read32(script->vm);
    StaActLightSys *lightSys = func_ov132_021f9dd4(sys->stage);

    func_ov112_021f6f14(lightSys, func_ov132_021f9ddc(sys->stage, index), GX_RGB((u8)r, (u8)g, (u8)b), alpha);
    return FALSE;
}

// pokebw2's StaScriptCmd_PrintMessage
static BOOL func_ov132_021fc53c(VM *vm, void *work) {
    StaActScript *script = work;
    StaActScriptSys *sys = script->sys;
    u32 msgId = VM_Read32(script->vm);
    u32 wait = VM_Read32(script->vm);

    func_ov132_021f93d8(sys->stage, msgId, wait);
    return FALSE;
}

// pokebw2's StaScriptCmd_ClearMessage
static BOOL func_ov132_021fc560(VM *vm, void *work) {
    StaActScript *script = work;

    func_ov132_021f94b0(script->sys->stage);
    return FALSE;
}

// pokebw2's StaScriptCmd_MessageColor
static BOOL func_ov132_021fc570(VM *vm, void *work) {
    StaActScript *script = work;
    u32 letter = VM_Read32(script->vm);
    u32 shadow = VM_Read32(script->vm);
    u32 background = VM_Read32(script->vm);

    GFL_TextRndUpdateColorIndexLUT(letter, shadow, background);
    return FALSE;
}

// pokebw2's StaScriptCmd_PlayStrm (Black passes the number as is)
BOOL func_ov132_021fc5a0(VM *vm, void *work) {
    StaActScript *script = work;
    StaActScriptSys *sys = script->sys;
    u32 seq = VM_Read32(script->vm);

    func_ov132_021f9664(sys->stage, seq);
    return FALSE;
}

BOOL func_ov132_021fc5bc(VM *vm, void *work) {
    StaActScript *script = work;
    StaActScriptSys *sys = script->sys;

    func_ov132_021f9680(sys->stage, VM_Read32(script->vm));
    return FALSE;
}

BOOL StaScriptCmd_StopSeq(VM *vm, void *work)
{
    StaActScript *script = work;

    func_ov132_021f9690(script->sys->stage);
    return FALSE;
}


BOOL func_ov132_021fc5e8(VM *vm, void *work) {
    StaActScript *script = work;
    StaActScriptSys *sys = script->sys;
    u32 seq = VM_Read32(script->vm);
    u32 a1 = VM_Read32(script->vm);
    u32 a2 = VM_Read32(script->vm);

    func_ov132_021f969c(sys->stage, seq, a1, a2);
    return FALSE;
}




// pokebw2's StaScriptCmd_PlayBgm
static BOOL func_ov132_021fc61c(VM *vm, void *work) {
    StaActScript *script = work;
    StaActScriptSys *sys = script->sys;

    func_ov132_021f96b0(sys->stage, VM_Read32(script->vm));
    return FALSE;
}

// pokebw2's StaScriptCmd_StopBgm
static BOOL func_ov132_021fc634(VM *vm, void *work) {
    StaActScript *script = work;

    func_ov132_021f96c4(script->sys->stage);
    return FALSE;
}

VMCommand STA_SCRIPT_COMMANDS[] = {
    func_ov132_021faf10,          func_ov132_021faf28,          func_ov132_021faf38,      func_ov132_021faf54,
    func_ov132_021faf70,  func_ov132_021fafdc,  func_ov132_021fb050,    func_ov132_021fb104,
    func_ov132_021fb1c0,         func_ov132_021fb208,      func_ov132_021fb258,       func_ov132_021fb2b0,
    func_ov132_021fb3e0,   func_ov132_021fb55c, func_ov132_021fb5a4, func_ov132_021fb5ec,
    func_ov132_021fb808,     func_ov132_021fbd78,   func_ov132_021fbdcc,         func_ov132_021fbe00,
    func_ov132_021fbe38,      func_ov132_021fbe64,       func_ov132_021fbe90,        func_ov132_021fbfa8,
    func_ov132_021fbfdc,    func_ov132_021fc014, func_ov132_021fc080,  func_ov132_021fc0ac,
    func_ov132_021fc1d4,     func_ov132_021fc22c,      func_ov132_021fc268,      func_ov132_021fc380,
    func_ov132_021fc4cc,   func_ov132_021fc53c,  func_ov132_021fc560,   func_ov132_021fc570,
    func_ov132_021fc5bc,      StaScriptCmd_StopSeq,       func_ov132_021fb948,     func_ov132_021fb640,
    func_ov132_021fb698, func_ov132_021fb6f0,    func_ov132_021fb738,  func_ov132_021fb774,
    func_ov132_021fb1cc, func_ov132_021fb1e4, func_ov132_021fba6c,         func_ov132_021faf1c,
    func_ov132_021fc61c,      func_ov132_021fc634,       func_ov132_021fb7a4,   func_ov132_021fb7d8,
    func_ov132_021fc5e8,     func_ov132_021fc5a0,
};
