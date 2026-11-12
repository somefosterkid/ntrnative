// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov168/btlv_b_gauge.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// A side's gauge of party balls, which slides in at a trainer battle's start. The name is the ROM's string, from
// GFL_HeapAllocate's asserts. The names are ours; swan has none for this file

#include "battle/btlv_b_gauge.h"
#include "types.h"
#include "battle/btl_pokeparam.h"
#include "battle/btlv_effect.h"
#include "constants/sound.h"
#include "gfl/arc.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "gfl/sound.h"
#include "gfl/tcb.h"
#include "system/palanm.h"

// A ball, or the gauge's frame as the last, 0x1c bytes
typedef struct {
    ClActor *actor; // 0x00
    s32 slideEndX;  // 0x04  where the slide stops, past restX
    s32 restX;      // 0x08  where it bounces back to
    s32 speed;      // 0x0c  0 once it stopped
    u32 sequence;   // 0x10  its animation once it stopped
    s32 wait;       // 0x14  frames before it starts
    ClActorPos pos; // 0x18
} BtlvBGaugeBall;

struct BtlvBGauge {
    ClActUnit *unit;         // 0x00
    u32 chars;               // 0x04
    u32 cellAnims;           // 0x08
    u32 palette;             // 0x0c
    BtlvBGaugeBall balls[7]; // 0x10  the frame last
    s32 seq;                 // 0xd4  0 slide in, 1 bounce back
    BOOL busy;               // 0xd8
    s32 sePlayer;            // 0xdc  alternates between 0 and 1
    HeapID heapId;           // 0xe0
#if !defined(__MWERKS__) && !defined(__arm__)
    TCB *slideTask; // PC only: the slide task, ended by func_ov094_02209068 when it still runs
#endif
};

typedef struct {
    BtlvBGauge *gauge;
} BtlvBGaugeTask;

// Where a side's balls start and where its frame rests
typedef struct {
    s16 ballX;
    s16 ballY;
    s16 frameX;
    s16 frameY;
} BtlvBGaugePos;

static void func_ov094_022090b4(TCB *tcb, void *data);
static void func_ov094_02209214(TCB *tcb);

static const BtlvBGaugePos data_ov094_0220ab6c[2] = { { 166, 112, 184, 120 }, { 90, 40, 56, 48 } };

// The animations by ball state: a ball's while it slides on each side, then once it stopped; column 4 is the frame's
static const u32 data_ov094_0220ab7c[3][5] = {
#include "asset/btlv_b_gauge_data_ov094_0220ab7c.inc"
};

// pokebw2's BtlvBGauge_Delete

BtlvBGauge *func_ov094_02208dac(const BtlvBGaugeParam *param, HeapID heapId) {
    BtlvBGauge *work;
    ArcTool *arc;
    BtlvBGaugeTask *task;
    u32 tail;
    s32 x;
    int i;

    tail = (heapId & (HEAPID_TAIL_BIT - 1)) | HEAPID_TAIL_BIT;
    work = GFL_HeapAllocate(tail, sizeof(BtlvBGauge), TRUE, "btlv_b_gauge.c", 0x7a);
    arc = GFL_ArcSysCreateFileHandle(11, tail);
    work->heapId = tail;
    work->unit = func_0204b100(7, 0, work->heapId);
    work->chars = func_0204a6c8(arc, 0xbd, FALSE, CLACT_VRAM_MAIN, work->heapId);
    work->cellAnims = func_0204af28(arc, 0xbf, 0xc0, work->heapId);
    work->palette = func_0204abf0(arc, 0xbe, CLACT_VRAM_MAIN, 0xc0, work->heapId);
    func_020216d4(func_ov094_021f86e0(), 2, func_0204aef4(work->palette, FALSE) / 2, 0x20);

    {
    ClActorSetup setup = { 0 };
    x = data_ov094_0220ab6c[param->side].ballX;
    setup.x = x + (param->side == 0 ? 128 : -128);
    setup.y = data_ov094_0220ab6c[param->side].ballY;
    for (i = 0; i < 6; i++) {
        work->balls[i].actor = func_0204b294(work->unit, work->chars, work->palette, work->cellAnims, &setup, 0, work->heapId);
        func_0204b454(work->balls[i].actor, &work->balls[i].pos, 0);
        func_0204b92c(work->balls[i].actor, TRUE);
        func_0204b878(work->balls[i].actor, data_ov094_0220ab7c[param->side][param->balls[i]]);
        work->balls[i].sequence = data_ov094_0220ab7c[2][param->balls[i]];
        work->balls[i].wait = 6 * (i + 1);
        work->balls[i].restX = x;
        work->balls[i].slideEndX = x + (param->side == 0 ? -2 : 2) * (i + 1);
        work->balls[i].speed = param->side == 0 ? -12 : 12;
        x += param->side == 0 ? 15 : -15;
    }

    setup.x = data_ov094_0220ab6c[param->side].frameX + (param->side == 0 ? 128 : -128);
    setup.y = data_ov094_0220ab6c[param->side].frameY;
    work->balls[i].actor = func_0204b294(work->unit, work->chars, work->palette, work->cellAnims, &setup, 0, work->heapId);
    func_0204b454(work->balls[i].actor, &work->balls[i].pos, 0);
    func_0204b92c(work->balls[i].actor, TRUE);
    func_0204b878(work->balls[i].actor, data_ov094_0220ab7c[param->side][4]);
    work->balls[i].sequence = data_ov094_0220ab7c[param->side][4];
    work->balls[i].wait = 0;
    work->balls[i].restX = work->balls[i].slideEndX = data_ov094_0220ab6c[param->side].frameX;
    work->balls[i].speed = param->side == 0 ? -16 : 16;

    }
    task = GFL_HeapAllocate(tail, sizeof(BtlvBGaugeTask), FALSE, "btlv_b_gauge.c", 0xec);
    task->gauge = work;
#if !defined(__MWERKS__) && !defined(__arm__)
    work->slideTask = GFL_TCBMgrAddTask(func_ov094_021f86d4(), func_ov094_022090b4, task, 0);
    func_ov094_021f896c(work->slideTask, func_ov094_02209214, 0);
#else
    func_ov094_021f896c(GFL_TCBMgrAddTask(func_ov094_021f86d4(), func_ov094_022090b4, task, 0), func_ov094_02209214, 0);
#endif
    work->busy = TRUE;
    GFL_ArcToolFree(arc);
    GFL_SndSEPlay(SEQ_SE_TB_START);
    return work;
}

// BUG: btlv_scu.c deletes the opponent's gauge (side 1) after its messages without asking func_ov094_022090a4, so
// with quick text the slide task still runs and reads the freed work the next frame. The DS gets away with it (its
// stale actor pointers point at reset actors and address 0 is readable); on PC the freed block is reused and the task
// follows a garbage actor pointer. Black 2 ends the task when the gauge is gone (its `loaded`); the PC ends it here
void func_ov094_02209068(BtlvBGauge *work) {
    int i;
#if !defined(__MWERKS__) && !defined(__arm__)
    if (work->busy) {
        func_ov094_021f89bc(work->slideTask);
    }
#endif
    for (i = 0; i < 7; i++) {
        func_0204b3b4(work->balls[i].actor);
    }
    func_0204a8d4(work->chars);
    func_0204ada4(work->palette);
    func_0204afd8(work->cellAnims);
    func_0204b1cc(work->unit);
    GFL_HeapFree(work);
}

// pokebw2's BtlvBGauge_IsBusy
BOOL func_ov094_022090a4(BtlvBGauge *work) {
    if (work->busy) {
        return TRUE;
    }
    return FALSE;
}

static void func_ov094_022090b4(TCB *tcb, void *data) {
    BtlvBGaugeTask *task = data;
    BOOL moving = FALSE;
    int i;
    u32 se;

    switch (task->gauge->seq) {
    case 0:
        for (i = 0; i < 7; i++) {
            if (task->gauge->balls[i].wait) {
                moving = TRUE;
                task->gauge->balls[i].wait--;
            } else if (task->gauge->balls[i].speed) {
                task->gauge->balls[i].pos.x += task->gauge->balls[i].speed;
                moving = TRUE;
                if (task->gauge->balls[i].speed > 0) {
                    if (task->gauge->balls[i].pos.x >= task->gauge->balls[i].slideEndX) {
                        task->gauge->balls[i].pos.x = task->gauge->balls[i].slideEndX;
                        task->gauge->balls[i].speed = 0;
                    }
                } else if (task->gauge->balls[i].pos.x <= task->gauge->balls[i].slideEndX) {
                    task->gauge->balls[i].pos.x = task->gauge->balls[i].slideEndX;
                    task->gauge->balls[i].speed = 0;
                }
                func_0204b404(task->gauge->balls[i].actor, &task->gauge->balls[i].pos, 0);
                if (task->gauge->balls[i].speed == 0) {
                    func_0204b878(task->gauge->balls[i].actor, task->gauge->balls[i].sequence);
                    switch (task->gauge->balls[i].sequence) {
                    case 9:
                        se = SEQ_SE_TB_KARA;
                        break;
                    case 10:
                    case 11:
                        se = 0;
                        break;
                    default:
                        se = SEQ_SE_TB_KON;
                        break;
                    }
                    if (se) {
                        GFL_SEPlayKeepVol(se, task->gauge->sePlayer);
                        task->gauge->sePlayer ^= 1;
                    }
                }
            }
        }
        if (moving == FALSE) {
            task->gauge->seq++;
        }
        break;
    case 1:
        for (i = 0; i < 7; i++) {
            if (task->gauge->balls[i].pos.x < task->gauge->balls[i].restX) {
                task->gauge->balls[i].pos.x += 2;
                if (task->gauge->balls[i].pos.x >= task->gauge->balls[i].restX) {
                    task->gauge->balls[i].pos.x = task->gauge->balls[i].restX;
                }
                moving = TRUE;
            } else if (task->gauge->balls[i].pos.x > task->gauge->balls[i].restX) {
                task->gauge->balls[i].pos.x -= 2;
                if (task->gauge->balls[i].pos.x <= task->gauge->balls[i].restX) {
                    task->gauge->balls[i].pos.x = task->gauge->balls[i].restX;
                }
                moving = TRUE;
            }
            func_0204b404(task->gauge->balls[i].actor, &task->gauge->balls[i].pos, 0);
        }
        if (moving == FALSE) {
            func_ov094_021f89bc(tcb);
        }
        break;
    }
}


// pokebw2's BtlvBGauge_SlideTaskEnd
static void func_ov094_02209214(TCB *tcb) {
    BtlvBGaugeTask *task = GFL_TCBGetData(tcb);
    task->gauge->busy = FALSE;
}
