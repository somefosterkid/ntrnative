// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov168/btlv_timer.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The two countdowns of a timed battle on the upper screen: the battle's time and the time to choose a command. The
// name is the ROM's string, from GFL_HeapAllocate's assert. The names are ours; swan has none for this file

#include "battle/btlv_timer.h"
#include "types.h"
#include "battle/btl_pokeparam.h"
#include "battle/btlv_effect.h"
#include "gfl/arc.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "gfl/tcb.h"
#include "nitro/os.h"
#include "system/palanm.h"

// A countdown's actors: its label, the minutes' tens and ones, the colon and the seconds' tens and ones
#define TIMER_ACTOR_COUNT 6
// The highest limit, 99:59
#define TIMER_MAX_SECONDS (99 * 60 + 59)

// The label actor's sequences: the battle's time and the time to choose a command
typedef enum {
    TIMER_SEQ_LABEL_BATTLE,
    TIMER_SEQ_LABEL_COMMAND,
} TimerLabelSeq;

// The timer's work, 0x78 bytes
struct BtlvTimer {
    ClActUnit *units[2];   // 0x00  one per countdown
    ClActor *actors[2][7]; // 0x08  the last of each row is never used
    u32 chars;             // 0x40
    u32 cellAnims;         // 0x44
    u32 palette;           // 0x48
    s32 limits[2];         // 0x4c  seconds
    u64 startTicks[2];     // 0x54
    u32 seqBase[2];        // 0x64  0, or 13 for the warning digits once the countdown reaches its warning time
    TCB *task;             // 0x6c
    TCB *task70;           // 0x70  removed with the actors, but never added
    u16 heapId;            // 0x74
};

static void func_ov094_02209458(BtlvTimer *timer);
static void func_ov094_02209540(BtlvTimer *timer, int which);
static void func_ov094_02209744(TCB *tcb, void *data);

// pokebw2's BtlvTimer_Delete

BtlvTimer *func_ov094_02209224(HeapID heapId) {
    BtlvTimer *timer;
    ArcTool *arc;
    u32 pal;
    PaletteFade *tcf;

    timer = GFL_HeapAllocate(heapId, sizeof(BtlvTimer), TRUE, "btlv_timer.c", 0x81);
    timer->heapId = heapId;
    arc = GFL_ArcSysCreateFileHandle(11, HEAPID_TAIL(timer->heapId));
    timer->units[0] = func_0204b100(TIMER_ACTOR_COUNT, 0, timer->heapId);
    timer->units[1] = func_0204b100(TIMER_ACTOR_COUNT, 0, timer->heapId);
    timer->chars = func_0204a6c8(arc, 0x7a, FALSE, CLACT_VRAM_MAIN, timer->heapId);
    timer->cellAnims = func_0204af28(arc, 0x7b, 0x7c, timer->heapId);
    timer->palette = func_0204abf0(arc, 0xa2, CLACT_VRAM_MAIN, 0xe0, timer->heapId);
    tcf = func_ov094_021f86e0();
    pal = func_0204aef4(timer->palette, FALSE);
    func_020216d4(tcf, 2, pal / 2, 0x20);
    GFL_ArcToolFree(arc);
    return timer;
}

void func_ov094_022092e8(BtlvTimer *timer) {
    func_ov094_02209458(timer);
    func_0204a8d4(timer->chars);
    func_0204afd8(timer->cellAnims);
    func_0204ada4(timer->palette);
    func_0204b1cc(timer->units[0]);
    func_0204b1cc(timer->units[1]);
    GFL_HeapFree(timer);
}

// pokebw2's BtlvTimer_Start
void func_ov094_02209318(BtlvTimer *timer, s32 battleLimit, s32 commandLimit) {
    ClActorSetup setup = { 0, 0, 0, 0, 0 };
    const s32 xs[TIMER_ACTOR_COUNT] = { 0, 64, 68, 72, 78, 82 };
    const s32 ys[2] = { 128, 136 };
    const s32 sequences[2][TIMER_ACTOR_COUNT] = {
#include "asset/btlv_timer_sequences.inc"
};
    int i, j;

    for (i = 0; i < 2; i++) {
        if ((i == 0 && battleLimit != 0) || (i == 1 && commandLimit != 0)) {
            for (j = 0; j < TIMER_ACTOR_COUNT; j++) {
                setup.x = xs[j];
                setup.y = ys[i];
                setup.sequence = sequences[i][j];
                timer->actors[i][j] = func_0204b294(timer->units[i], timer->chars, timer->palette, timer->cellAnims,
                                                    &setup, 0, timer->heapId);
                func_0204b92c(timer->actors[i][j], TRUE);
            }
        }
    }
    if (battleLimit > TIMER_MAX_SECONDS) {
        battleLimit = TIMER_MAX_SECONDS;
    }
    if (commandLimit > TIMER_MAX_SECONDS) {
        commandLimit = TIMER_MAX_SECONDS;
    }
    timer->limits[0] = battleLimit;
    timer->limits[1] = commandLimit;
    timer->startTicks[0] = timer->startTicks[1] = OS_GetTick();
    func_ov094_022094bc(timer, 0, FALSE, FALSE);
    func_ov094_022094bc(timer, 1, FALSE, FALSE);
    timer->task = GFL_TCBMgrAddTask(func_ov094_021f86d4(), func_ov094_02209744, timer, 0);
}

// pokebw2's BtlvTimer_DeleteActors
static void func_ov094_02209458(BtlvTimer *timer) {
    int i, j;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < TIMER_ACTOR_COUNT; j++) {
            if (timer->actors[i][j] != NULL) {
                func_0204b3b4(timer->actors[i][j]);
                timer->actors[i][j] = NULL;
            }
        }
    }
    if (timer->task != NULL) {
        GFL_TCBRemove(timer->task);
        timer->task = NULL;
    }
    if (timer->task70 != NULL) {
        GFL_TCBRemove(timer->task70);
        timer->task70 = NULL;
    }
}

// pokebw2's BtlvTimer_SetVisible
void func_ov094_022094bc(BtlvTimer *timer, int which, BOOL visible, BOOL restart) {
    if (timer->actors[which][0] != NULL) {
        if (visible == TRUE && restart == TRUE) {
            timer->startTicks[which] = OS_GetTick();
            timer->seqBase[which] = 0;
            func_ov094_02209540(timer, which);
        }
        func_0204b22c(timer->units[which], visible);
    }
}

// pokebw2's BtlvTimer_IsTimeUp
BOOL func_ov094_02209500(BtlvTimer *timer, int which) {
    s32 left = timer->limits[which] - (s32)OS_TicksToSeconds(OS_GetTick() - timer->startTicks[which]);

    return left <= 0;
}

// Draws a countdown's time left
// NONMATCHING: the original tests which with bne and sets 0 first, where MWCC turns label's conditional the other way
// round (beq, 1 first); the rest matches
// pokebw2's BtlvTimer_Draw
static void func_ov094_02209540(BtlvTimer *timer, int which) {
    const s32 warnTimes[2] = { 300, 15 };
    TimerLabelSeq label = which == 0 ? TIMER_SEQ_LABEL_BATTLE : TIMER_SEQ_LABEL_COMMAND;
    s32 left = timer->limits[which] - (s32)OS_TicksToSeconds(OS_GetTick() - timer->startTicks[which]);
    int minutes, seconds, minuteTens, minuteOnes, secondTens, secondOnes;

    if (timer->actors[which][0] == NULL) {
        return;
    }
    if (left < 0) {
        left = 0;
    }
    minutes = left / 60;
    seconds = left % 60;
    minuteTens = minutes / 10;
    minuteOnes = minutes % 10;
    secondTens = seconds / 10;
    secondOnes = seconds % 10;
    if (left == warnTimes[which]) {
        timer->seqBase[which] = 13;
    }
    func_0204b878(timer->actors[which][0], label + timer->seqBase[which]);
    if (minuteTens == 0) {
        func_0204b3dc(timer->actors[which][1], FALSE);
    } else {
        func_0204b3dc(timer->actors[which][1], TRUE);
        func_0204b878(timer->actors[which][1], minuteTens + 3 + timer->seqBase[which]);
    }
    if (minuteTens == 0 && minuteOnes == 0) {
        func_0204b3dc(timer->actors[which][2], FALSE);
        func_0204b3dc(timer->actors[which][3], FALSE);
    } else {
        func_0204b3dc(timer->actors[which][2], TRUE);
        func_0204b3dc(timer->actors[which][3], TRUE);
        func_0204b878(timer->actors[which][2], minuteOnes + 3 + timer->seqBase[which]);
        func_0204b878(timer->actors[which][3], timer->seqBase[which] + 2);
    }
    if (minuteTens == 0 && minuteOnes == 0 && secondTens == 0) {
        func_0204b3dc(timer->actors[which][4], FALSE);
    } else {
        func_0204b3dc(timer->actors[which][4], TRUE);
        func_0204b878(timer->actors[which][4], secondTens + 3 + timer->seqBase[which]);
    }
    func_0204b878(timer->actors[which][5], secondOnes + 3 + timer->seqBase[which]);
}

// pokebw2's BtlvTimer_Task
static void func_ov094_02209744(TCB *tcb, void *data) {
    BtlvTimer *timer = data;

    func_ov094_02209540(timer, 0);
    func_ov094_02209540(timer, 1);
}
