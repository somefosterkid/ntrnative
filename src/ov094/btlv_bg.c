// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov168/btlv_bg.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The effects' moves and waves of the battle's background, BG 3. The name is the ROM's string, from
// GFL_HeapAllocate's asserts. The names are ours; swan has none for this file

#include "battle/btlv_bg.h"
#include "types.h"
#include "battle/btlv_effect.h"
#include "gfl/bg_sys.h"
#include "gfl/heap.h"
#include "gfl/tcb.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_cp.h"
#include "nitro/gx.h"
#include "system/sin_wave_table.h"

struct BtlvBg {
    TCBManager *tcbMgr; // 0x0
    u32 moving : 1;     // 0x4
    HeapID heapId; // 0x8
};

// The offset to set at the next V-blank
typedef struct {
    s32 x;
    s32 y;
} BtlvBgScroll;

// A move of the offset, by the effect tools' moving value
typedef struct {
    BtlvBg *bg;           // 0x00
    VecFx32 pos;          // 0x04
    BtlvEffToolMove move; // 0x10
} BtlvBgMove;

void func_ov094_0220986c(BtlvBg *work, u32 mode, VecFx32 *start, const VecFx32 *end, s32 frames, s32 wait, s32 count,
                         TCBFunc func);
static void func_ov094_022099e8(TCB *tcb, void *data);
static void func_ov094_02209a40(TCB *tcb);
static void func_ov094_02209a54(TCB *tcb, void *data);

// pokebw2's BtlvBg_Create
BtlvBg *func_ov094_0220975c(TCBManager *tcbMgr, HeapID heapId) {
    BtlvBg *work = GFL_HeapAllocate(heapId, sizeof(BtlvBg), TRUE, "btlv_bg.c", 0x48);
    work->tcbMgr = tcbMgr;
    work->heapId = heapId;
    return work;
}

// pokebw2's BtlvBg_Delete
void func_ov094_0220977c(BtlvBg *work) {
    GFL_HeapFree(work);
}

// pokebw2's BtlvBg_SetOffsetReq
void func_ov094_02209784(BtlvBg *work, s32 x, s32 y) {
    BtlvBgScroll *scroll = GFL_HeapAllocate(HEAPID_TAIL(work->heapId), sizeof(BtlvBgScroll), FALSE, "btlv_bg.c", 0x67);
    scroll->x = x;
    scroll->y = y;
    func_ov094_021f896c(GFL_VBlankTCBAdd(func_ov094_02209a54, scroll, 0), NULL, 0);
}

// pokebw2's BtlvBg_StartMove
void func_ov094_022097cc(BtlvBg *work, u32 pos, u32 mode, s32 x, s32 y, s32 frames, s32 wait, s32 count) {
    VecFx32 start;
    VecFx32 end;

    end.x = x << FX32_SHIFT;
    end.y = y << FX32_SHIFT;
    end.z = 0;
    start.x = GFL_BGSysGetBGOffsetX(3) << FX32_SHIFT;
    start.y = GFL_BGSysGetBGOffsetY(3) << FX32_SHIFT;
    start.z = 0;
    if (mode == 4 && (pos & 1)) {
        end.x *= -1;
    }
    if (mode == 1) {
        end.x += start.x;
        end.y += start.y;
        end.z += start.z;
    }
    func_ov094_0220986c(work, mode, &start, &end, frames, wait, count, func_ov094_022099e8);
    work->moving = TRUE;
}

// pokebw2's BtlvBg_IsMoving
BOOL func_ov094_0220985c(BtlvBg *work) {
    if (work->moving) {
        return TRUE;
    }
    return FALSE;
}

void func_ov094_0220986c(BtlvBg *work, u32 mode, VecFx32 *start, const VecFx32 *end, s32 frames, s32 wait,
                         s32 count, TCBFunc func) {
    BtlvBgMove *move = GFL_HeapAllocate(work->heapId, sizeof(BtlvBgMove), FALSE, "btlv_bg.c", 0xb8);
    move->bg = work;
    move->pos.x = start->x;
    move->pos.y = start->y;
    move->pos.z = start->z;
    move->move.type = mode;
    move->move.stepTime = frames;
    move->move.stepTimeReset = frames;
    move->move.wait = 0;
    move->move.waitReset = wait;
    move->move.count = count * 2;
    move->move.start.x = start->x;
    move->move.start.y = start->y;
    move->move.start.z = start->z;
    move->move.end.x = end->x;
    move->move.end.y = end->y;
    move->move.end.z = end->z;
    switch (mode) {
    case 0:
        break;
    case 1:
        func_ov094_021f918c(&move->move.start, end, &move->move.step, FX32_CONST(frames));
        break;
    case 3:
        move->move.stepTimeReset *= 2;
        move->move.count *= 2;
    case 2:
        move->move.step.x = FX_Div(end->x, FX32_CONST(frames));
        move->move.step.y = FX_Div(end->y, FX32_CONST(frames));
        move->move.step.z = FX_Div(end->z, FX32_CONST(frames));
        break;
    }
    func_ov094_021f896c(GFL_TCBMgrAddTask(work->tcbMgr, func, move, 0), func_ov094_02209a40, 0);
}


// pokebw2's BtlvBg_MoveTask
static void func_ov094_022099e8(TCB *tcb, void *data) {
    BtlvBgMove *move = data;
    BtlvBg *work = move->bg;
    BOOL done;

    if (move->move.type == 4) {
        move->pos.x += move->move.end.x;
        move->pos.y += move->move.end.y;
        if (move->move.stepTime == 0) {
            done = TRUE;
        } else {
            move->move.stepTime--;
            done = FALSE;
        }
    } else {
        done = func_ov094_021f9260(&move->move, &move->pos);
    }
    func_ov094_02209784(work, move->pos.x >> FX32_SHIFT, move->pos.y >> FX32_SHIFT);
    if (done == TRUE) {
        func_ov094_021f89bc(tcb);
    }
}

// pokebw2's BtlvBg_MoveTaskEnd
static void func_ov094_02209a40(TCB *tcb) {
    BtlvBgMove *move = GFL_TCBGetData(tcb);
    move->bg->moving = FALSE;
}

// pokebw2's BtlvBg_SetOffsetVBlank
static void func_ov094_02209a54(TCB *tcb, void *data) {
    BtlvBgScroll *scroll = GFL_TCBGetData(tcb);
    GFL_BGSysMoveBG(3, BG_MOVE_SET_X, scroll->x);
    GFL_BGSysMoveBG(3, BG_MOVE_SET_Y, scroll->y);
    func_ov094_021f89bc(tcb);
}
