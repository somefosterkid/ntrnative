// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov168/btlv_clact.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The battle view's 2D cell actors: up to three OBJ actors the effects load, move, scale and fade, and in mode 1 a
// turn gauge with a popup that shows a message. The name is the ROM's string, from GFL_HeapAllocate's asserts. The
// names are ours; swan has none for this file

#include "battle/btlv_clact.h"
#include "types.h"
#include "battle/btl_pokeparam.h"
#include "battle/btlv_effect.h"
#include "gfl/arc.h"
#include "gfl/bmp.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_cp.h"
#include "system/palanm.h"
#include "system/printsys.h"

// An actor with the resources it was created from
typedef struct {
    ClActor *actor; // 0x0
    u32 chars;      // 0x4
    u32 palette;    // 0x8
    u32 cellAnims;  // 0xc
} BtlvClactEntry;

// An entry's size in u32s and in pointers: func_ov094_02201b20 reaches entry index as word index * 4 from entry 0's
// field (4 on the DS; 6 and 3 on a 64-bit target, where the entry is 24 bytes and index * 4 wrote into the entries
// after it: the capture's ball scale task read a garbage actor)
#define BTLV_CLACT_ENTRY_U32S ((int)(sizeof(BtlvClactEntry) / sizeof(u32)))
#define BTLV_CLACT_ENTRY_PTRS ((int)(sizeof(BtlvClactEntry) / sizeof(ClActor *)))

// 0x84 bytes
struct BtlvClact {
    TCBManager *tcbManager;    // 0x000
    ClActUnit *unit;           // 0x004
    BtlvClactEntry entries[7]; // 0x008
    u32 moveFlags;             // 0x078
    u32 scaleFlags;            // 0x07c
    u16 mode;                  // 0x080
};

// A move or scaling of an actor, 0x50 bytes.
typedef struct {
    BtlvClact *wk;        // 0x00
    VecFx32 value;        // 0x04
    int index;            // 0x10
    BtlvEffToolMove move; // 0x14
} BtlvClactMove;

int func_ov094_02201b20(BtlvClact *wk, u32 arcId, u32 param3, u32 param4, u32 param5, u32 param6, s16 x, s16 y,
                        fx32 param9, fx32 param10);
static void func_ov094_02201e04(BtlvClact *wk, int index, int type, VecFx32 *start, VecFx32 *end, int frames,
                                  int wait, int count, TCBFunc func, void (*endFunc)(TCB *tcb));
static void func_ov094_02201f98(TCB *tcb, void *data);
static void func_ov094_02201fd8(TCB *tcb);
static void func_ov094_02201ff4(TCB *tcb, void *data);

static const ClActorSetupEx data_ov094_0220a368 = { { 0, 0, 0, 0, 1 }, { 0, 0 }, FX32_ONE, FX32_ONE, 0, 1 };

// pokebw2's BtlvClact_Delete
void func_ov094_02201ca4(BtlvClact *wk, int index);

// pokebw2's sFile
static char data_ov094_0220ae88[] = "btlv_clact.c";

BtlvClact *func_ov094_02201a8c(TCBManager *tcbManager, HeapID heapId) {
    BtlvClact *wk = GFL_HeapAllocate(heapId, sizeof(BtlvClact), TRUE, data_ov094_0220ae88, 79);

    wk->tcbManager = tcbManager;
    wk->mode = heapId;
    wk->unit = func_0204b100(7, 0, heapId);
    return wk;
}

void func_ov094_02201ac4(BtlvClact *wk) {
    int i;

    for (i = 0; i < 7; i++) {
        if (wk->entries[i].actor != NULL) {
            func_ov094_02201ca4(wk, i);
        }
    }
    func_0204b1cc(wk->unit);
    GFL_HeapFree(wk);
}

// pokebw2's BtlvClact_Draw
void func_ov094_02201af0(BtlvClact *wk) {
    func_0204a600();
}

// pokebw2's BtlvClact_AddActor
int func_ov094_02201af8(BtlvClact *wk, u32 arcId, u32 fileId, s16 x, s16 y, fx32 scaleX, fx32 scaleY) {
    return func_ov094_02201b20(wk, arcId, fileId, fileId + 1, fileId + 2, fileId + 3, x, y, scaleX, scaleY);
}

int func_ov094_02201b20(BtlvClact *wk, u32 arcId, u32 param3, u32 param4, u32 param5, u32 param6, s16 x, s16 y,
                        fx32 param9, fx32 param10) {
    ClActorSetupEx setup = data_ov094_0220a368;
    ArcTool *arc;
    PaletteFade *fade;
    u32 *chars;
    u32 *palette;
    u32 *cellAnims;
    ClActor **actor;
    ClActor **last;
    int index;

    arc = GFL_ArcSysCreateFileHandle(arcId, (wk->mode & 0x7fff) | 0x8000);
    for (index = 0; index < 7; index++) {
        if (wk->entries[index].actor == NULL) {
            break;
        }
    }
    chars = &wk->entries[0].chars;
    chars[index * BTLV_CLACT_ENTRY_U32S] = func_0204a6c8(arc, param3, 0, 0, wk->mode);
    palette = &wk->entries[0].palette;
    palette[index * BTLV_CLACT_ENTRY_U32S] = func_0204acec(arc, param4, 0, index * 0x20 + 0x140, wk->mode);
    cellAnims = &wk->entries[0].cellAnims;
    cellAnims[index * BTLV_CLACT_ENTRY_U32S] = func_0204af28(arc, param5, param6, wk->mode);
    fade = func_ov094_021f86e0();
    func_02021640(fade, arcId, param4, wk->mode, 2, 0x20, (func_0204aef4(palette[index * BTLV_CLACT_ENTRY_U32S], 0) & 0x3ff) >> 1);
    setup.base.x = x;
    setup.base.y = y;
    if (param9 == 0 && param10 == 0) {
        actor = &wk->entries[0].actor;
        last = &actor[index * BTLV_CLACT_ENTRY_PTRS];
        actor[index * BTLV_CLACT_ENTRY_PTRS] = func_0204b294(wk->unit, chars[index * BTLV_CLACT_ENTRY_U32S], palette[index * BTLV_CLACT_ENTRY_U32S], cellAnims[index * BTLV_CLACT_ENTRY_U32S],
                                         (ClActorSetup *)&setup, 0, wk->mode);
    } else {
        setup.scaleX = param9;
        setup.scaleY = param10;
        actor = &wk->entries[0].actor;
        last = &actor[index * BTLV_CLACT_ENTRY_PTRS];
        actor[index * BTLV_CLACT_ENTRY_PTRS] = func_0204b324(wk->unit, chars[index * BTLV_CLACT_ENTRY_U32S], palette[index * BTLV_CLACT_ENTRY_U32S], cellAnims[index * BTLV_CLACT_ENTRY_U32S],
                                         &setup, 0, wk->mode);
    }
    func_0204b92c(*last, 1);
    GFL_ArcToolFree(arc);
    return index;
}


void func_ov094_02201ca4(BtlvClact *wk, int index) {
    func_0204a8d4(wk->entries[index].chars);
    func_0204ada4(wk->entries[index].palette);
    func_0204afd8(wk->entries[index].cellAnims);
    func_0204b3b4(wk->entries[index].actor);
    wk->entries[index].actor = NULL;
}

void func_ov094_02201cd0(BtlvClact *wk, int index, int type, ClActorPos *pos, int frames, int wait, int count) {
    VecFx32 start;
    VecFx32 end;
    ClActorPos current;

    end.x = pos->x << FX32_SHIFT;
    end.y = pos->y << FX32_SHIFT;
    end.z = 0;
    func_0204b454(wk->entries[index].actor, &current, CLACT_SURFACE_MAIN);
    start.x = current.x << FX32_SHIFT;
    start.y = current.y << FX32_SHIFT;
    start.z = 0;
    if (type == 1) {
        end.x += start.x;
        end.y += start.y;
        end.z += start.z;
    }
    func_ov094_02201e04(wk, index, type, &start, &end, frames, wait, count, func_ov094_02201f98,
                          func_ov094_02201fd8);
    wk->moveFlags |= 1 << index;
}


// pokebw2's BtlvClact_ScaleTaskEnd
static void func_ov094_0220203c(TCB *tcb);
void func_ov094_02201d5c(BtlvClact *wk, int index, int type, VecFx32 *scale, int frames, int wait, int count) {
    VecFx32 start;

    start.x = FX32_ONE;
    start.y = FX32_ONE;
    start.z = FX32_ONE;
    func_ov094_02201e04(wk, index, type, &start, scale, frames, wait, count, func_ov094_02201ff4,
                          func_ov094_0220203c);
    wk->scaleFlags |= 1 << index;
}

void func_ov094_02201da0(BtlvClact *wk, int index, u32 sequence) {
    func_0204b878(wk->entries[index].actor, sequence);
}

void func_ov094_02201db4(BtlvClact *wk, int index, u8 param3, u8 param4, s8 param5, u16 param6) {
    u16 mask;
    PaletteFade *fade;
    TCBManager *tcbManager;

    mask = 1 << ((func_0204aef4(wk->entries[index].palette, 0) & 0x3ff) >> 5);
    fade = func_ov094_021f86e0();
    tcbManager = func_ov094_021f86d4();
    func_0202173c(fade, 4, mask, param5, param3, param4, param6, tcbManager);
}

static void func_ov094_02201e04(BtlvClact *wk, int index, int type, VecFx32 *start, VecFx32 *end, int frames,
                                  int wait, int count, TCBFunc func, void (*endFunc)(TCB *tcb)) {
    BtlvClactMove *task =
        GFL_HeapAllocate(HEAPID_TAIL(wk->mode), sizeof(BtlvClactMove), FALSE, data_ov094_0220ae88, 0x197);

    task->wk = wk;
    task->index = index;
    task->value.x = start->x;
    task->value.y = start->y;
    task->value.z = start->z;
    task->move.type = type;
    task->move.stepTime = frames;
    task->move.stepTimeReset = frames;
    task->move.wait = 0;
    task->move.waitReset = wait;
    task->move.count = count * 2;
    task->move.start.x = start->x;
    task->move.start.y = start->y;
    task->move.start.z = start->z;
    task->move.end.x = end->x;
    task->move.end.y = end->y;
    task->move.end.z = end->z;

    switch (type) {
    case 0:
        break;
    case 1:
        func_ov094_021f918c(&task->move.start, end, &task->move.step, FX32_CONST(frames));
        break;
    case 3:
        task->move.stepTimeReset *= 2;
        task->move.count *= 2;
    case 2:
        task->move.step.x = FX_Div(end->x, FX32_CONST(frames));
        task->move.step.y = FX_Div(end->y, FX32_CONST(frames));
        task->move.step.z = FX_Div(end->z, FX32_CONST(frames));
        break;
    }

    func_ov094_021f896c(GFL_TCBMgrAddTask(wk->tcbManager, func, task, 0), endFunc, 0);
}

// pokebw2's BtlvClact_MoveTask
static void func_ov094_02201f98(TCB *tcb, void *data) {
    BtlvClactMove *task = data;
    BtlvClact *wk = task->wk;
    BOOL done = func_ov094_021f9260(&task->move, &task->value);
    ClActorPos pos;

    pos.x = FX_Whole(task->value.x);
    pos.y = FX_Whole(task->value.y);
    func_0204b404(wk->entries[task->index].actor, &pos, CLACT_SURFACE_MAIN);
    if (done == TRUE) {
        func_ov094_021f89bc(tcb);
    }
}

// pokebw2's BtlvClact_MoveTaskEnd
static void func_ov094_02201fd8(TCB *tcb) {
    BtlvClactMove *task = GFL_TCBGetData(tcb);

    task->wk->moveFlags &= (1 << task->index) ^ 0xffffffff;
}

// pokebw2's BtlvClact_ScaleTask
static void func_ov094_02201ff4(TCB *tcb, void *data) {
    BtlvClactMove *task = data;
    BtlvClact *wk = task->wk;
    BOOL done = func_ov094_021f9260(&task->move, &task->value);
    ClActorScale scale;

    scale.x = task->value.x;
    scale.y = task->value.y;
    func_0204b570(wk->entries[task->index].actor, 1);
    func_0204b5ac(wk->entries[task->index].actor, &scale);
    if (done == TRUE) {
        func_ov094_021f89bc(tcb);
    }
}

// pokebw2's BtlvClact_ScaleTaskEnd
static void func_ov094_0220203c(TCB *tcb) {
    BtlvClactMove *task = GFL_TCBGetData(tcb);

    task->wk->scaleFlags &= (1 << task->index) ^ 0xffffffff;
}
