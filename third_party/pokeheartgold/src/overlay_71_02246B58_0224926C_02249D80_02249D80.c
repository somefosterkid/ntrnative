#include "global.h"
#include "sys_task.h"
#include "overlay_71_trade_sequence_data.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "screen_fade.h"

struct BgConfig;

struct SysTask;

struct SysTaskQueue;

struct TradeSequenceData;

struct TradeWormholePhase;

void BeginNormalPaletteFade(enum FadeMode mode, enum FadeType typeMain, enum FadeType typeSub, u16 color, int steps, int framesPerStep, enum HeapID heapID);

BOOL IsPaletteFadeFinished(void);

SysTask * SysTask_CreateOnVBlankQueue(SysTaskFunc func, void *data, u32 priority);

BOOL PlaySE(u16 seqID, int *);

typedef struct TradeWormholePhase {
    TradeSequenceData *sequenceData;
    int subStepCounter;
    int framesRemaining;
    BgConfig *bgConfig;
    fx32 bg1Scroll;
    fx32 bg2Scroll;
    fx32 bg1ScrollStep;
    fx32 bg2ScrollStep;
    BOOL scrollDone;
} TradeWormholePhase;

void ov71_02249E24(SysTask *task, void *param);

int ov71_02249D80(TradeWormholePhase *twPhase, int *subStepCounter);

int ov71_02249D80(TradeWormholePhase *twPhase, int *subStepCounter) {
    switch (*subStepCounter) {
    case 0:
        if (IsPaletteFadeFinished()) {
            twPhase->bg1Scroll = -384 << 12;
            twPhase->bg2Scroll = 80 << 12;
            twPhase->bg1ScrollStep = ((-48 - -384) << 12) / 60;
            twPhase->bg2ScrollStep = ((-48 - 80) << 12) / 60;
            twPhase->framesRemaining = 60;
            twPhase->scrollDone = 0;
            SysTask_CreateOnVBlankQueue(ov71_02249E24, twPhase, 0);
            (*subStepCounter)++;
        }
        break;
    case 1:
        if (twPhase->scrollDone) {
            PlaySE(1704, subStepCounter);
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, (((31) << 10) | ((31) << 5) | (31)), 16, 1, (enum HeapID)57);
            (*subStepCounter)++;
        }
        break;
    case 2:
        if (IsPaletteFadeFinished()) {
            return 1;
        }
        break;
    }

    return 0;
}
