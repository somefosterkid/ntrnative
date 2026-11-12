// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov168/btlv_finger_cursor.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The pointing finger that the catching demonstration and the battle bag show tapping the touch screen. The name is
// the ROM's string, from GFL_HeapAllocate's assert. BtlvFingerCursor_Create is swan's name; the other names are ours

#include "battle/btlv_finger_cursor.h"
#include "types.h"
#include "battle/btl_pokeparam.h"
#include "battle/btlv_effect.h"
#include "gfl/arc.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "gfl/tcb.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "system/palanm.h"

// The finger's work, 0x30 bytes
struct BtlvFingerCursor {
    ClActUnit *unit; // 0x00
    ClActor *actor;  // 0x04
    TCB *task;       // 0x08
    u32 chars;       // 0x0c
    u32 palette;     // 0x10
    u32 cellAnims;   // 0x14
    s32 seq;         // 0x18  0 waits, 1 taps, 2 hides, 3 done
    u32 wait;        // 0x1c  frames before the tap
    s32 tapFrame;    // 0x20  the frame of the tap animation that touches the screen
    u32 hideWait;    // 0x24  frames the finger stays on the screen after touching it
    BOOL touched;    // 0x28
    u16 heapId;      // 0x2c
};

static void func_ov094_02209c30(uintptr_t param, fx32 frame);
void func_ov094_02209cc4(TCB *tcb, void *data);

static const ClActorSetupEx data_ov094_0220ac10 = { { 0, 0, 0, 0, 0 }, { 0, 0 }, FX32_ONE, FX32_ONE, 0, 2 };

// pokebw2's BtlvFingerCursor_Delete

BtlvFingerCursor *BtlvFingerCursor_Create(PaletteFade *fade, u32 paletteRow, HeapID heapId) {
    BtlvFingerCursor *cursor = GFL_HeapAllocate(heapId, sizeof(BtlvFingerCursor), TRUE, "btlv_finger_cursor.c", 0x52);
    ArcTool *arc = GFL_ArcSysCreateFileHandle(11, HEAPID_TAIL(heapId));

    cursor->heapId = heapId;
    cursor->chars = func_0204a6c8(arc, 0xd9, FALSE, CLACT_VRAM_SUB, heapId);
    cursor->cellAnims = func_0204af28(arc, 0xdb, 0xdc, heapId);
    cursor->palette = func_0204ac18(arc, 0xda, CLACT_VRAM_SUB, paletteRow * 32, 0, 1, heapId);
    func_020216d4(fade, PALFADE_VRAM_SUB_OBJ, func_0204aef4(cursor->palette, TRUE) / 2, 0x20);
    cursor->unit = func_0204b100(1, 0, heapId);
    GFL_ArcToolFree(arc);
    cursor->task = GFL_TCBMgrAddTask(func_ov094_021f86d4(), func_ov094_02209cc4, cursor, 0);
    return cursor;
}

void func_ov094_02209b30(BtlvFingerCursor *cursor) {
    GFL_TCBRemove(cursor->task);
    func_ov094_02209c18(cursor);
    func_0204a8d4(cursor->chars);
    func_0204afd8(cursor->cellAnims);
    func_0204ada4(cursor->palette);
    func_0204b1cc(cursor->unit);
    GFL_HeapFree(cursor);
}

// pokebw2's BtlvFingerCursor_Start
BOOL func_ov094_02209b60(BtlvFingerCursor *cursor, s32 x, s32 y, u32 wait, s32 tapFrame, u32 hideWait) {
    ClActorSetupEx setup = data_ov094_0220ac10;
    ClActorCallback callback;

    if (cursor->touched) {
        return FALSE;
    }
    if (cursor->actor != NULL) {
        func_ov094_02209c18(cursor);
        cursor->actor = NULL;
    }
    setup.base.x = x;
    setup.base.y = y;
    cursor->actor =
        func_0204b324(cursor->unit, cursor->chars, cursor->palette, cursor->cellAnims, &setup, 1, cursor->heapId);
    func_0204b878(cursor->actor, 0);
    func_0204b8c4(cursor->actor, 0);
    func_0204b92c(cursor->actor, TRUE);
    func_0204b3dc(cursor->actor, TRUE);
    if (wait != 0) {
        cursor->wait = wait;
    } else {
        cursor->wait = 1;
    }
    if (hideWait != 0) {
        cursor->hideWait = hideWait;
    } else {
        cursor->hideWait = 1;
    }
    cursor->seq = 0;
    cursor->tapFrame = tapFrame;
    cursor->touched = FALSE;
    callback.param = (uintptr_t)cursor;
    callback.type = CLACT_CALLBACK_LAST_FRAME;
    callback.frame = 0;
    callback.func = func_ov094_02209c30;
    func_0204ba00(cursor->actor, &callback);
    return TRUE;
}

// pokebw2's BtlvFingerCursor_RemoveActor
void func_ov094_02209c18(BtlvFingerCursor *cursor) {
    if (cursor->actor != NULL) {
        func_0204b3b4(cursor->actor);
        cursor->actor = NULL;
    }
}

// pokebw2's BtlvFingerCursor_IsTouched
BOOL func_ov094_02209c2c(BtlvFingerCursor *cursor) {
    return cursor->touched;
}

// The actor's animation callback, at the end of each animation and on the tap's frame
// pokebw2's BtlvFingerCursor_AnimCallback
static void func_ov094_02209c30(uintptr_t param, fx32 frame) {
    BtlvFingerCursor *cursor = (BtlvFingerCursor *)param;

    switch (cursor->seq) {
    case 0:
        if (--cursor->wait == 0) {
            ClActorCallback callback;

            func_0204b878(cursor->actor, 1);
            cursor->seq++;
            callback.param = (uintptr_t)cursor;
            callback.type = CLACT_CALLBACK_FRAME;
            callback.frame = cursor->tapFrame;
            callback.func = func_ov094_02209c30;
            func_0204ba00(cursor->actor, &callback);
        }
        break;
    case 1: {
        ClActorCallback callback;

        cursor->touched = TRUE;
        cursor->seq++;
        callback.param = (uintptr_t)cursor;
        callback.type = CLACT_CALLBACK_EVERY_FRAME;
        callback.frame = 0;
        callback.func = func_ov094_02209c30;
        func_0204ba00(cursor->actor, &callback);
        break;
    }
    case 2:
        func_0204b3dc(cursor->actor, FALSE);
        cursor->touched = FALSE;
        cursor->seq++;
        break;
    case 3:
        break;
    }
}

// pokebw2's BtlvFingerCursor_Task
void func_ov094_02209cc4(TCB *tcb, void *data) {
    BtlvFingerCursor *cursor = data;

    if (cursor->actor != NULL && cursor->touched && --cursor->hideWait == 0) {
        func_0204b3dc(cursor->actor, FALSE);
        cursor->touched = FALSE;
    }
}
