// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov264/cdemo_control.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "app/cdemo.h"
#include "constants/arc.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/sound.h"
#include "nitro/gx.h"
#include "nitro/os.h"
#include "system/wipe.h"

// Descriptive name: these callbacks control the demo process and movie playback.
typedef struct {
    u16 arcId;
    u16 frameCount;
    u32 frameSize;
    u32 screenOffset;
} CDemoMovie;
typedef u32 (*CDemoStateFunc)(CDemoWork *work);
static u32 func_ov179_021b63d4(CDemoWork *work);
static u32 func_ov179_021b6448(CDemoWork *work);
static u32 func_ov179_021b6490(CDemoWork *work);
static u32 func_ov179_021b64ac(CDemoWork *work);
static u32 func_ov179_021b64b0(CDemoWork *work);
static u32 func_ov179_021b64e0(CDemoWork *work);
static void func_ov179_021b669c(ArcTool *arc, u32 fileId, u32 offset, u32 size, u32 buffer);

// pokebw2's CDEMO_STATES
// pokebw2's CDemo_StateExit
// pokebw2's CDemo_StateInit
// pokebw2's CDemo_StateWaitFade
static const CDemoStateFunc data_ov179_021b6a04[] = { func_ov179_021b63d4,       func_ov179_021b6448,  func_ov179_021b6490,
                                               func_ov179_021b64ac, func_ov179_021b64e0, func_ov179_021b64b0 };
// pokebw2's CDEMO_MOVIES
static const CDemoMovie data_ov179_021b6a1c[] = { { 0xe1, 151, 0x18000, 0 },
                                           { 0x86, 670, 0x15400, 5632 },
                                           { 0xe2, 174, 0x18000, 0 } };

// pokebw2's CDemo_Main
BOOL func_ov179_021b6380(CDemoWork *work) {
    if (work->param->canSkip) {
        if (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B | PAD_BUTTON_START)) {
            if (GFL_SndBGMIsPlaying() == TRUE) {
                GFL_SndBGMFadeOut(8);
            }
            work->state = 1;
            work->param->skipped = TRUE;
        }
    }
    work->state = data_ov179_021b6a04[work->state](work);
    return work->state != 6;
}
// pokebw2's CDemo_StateInit
static u32 func_ov179_021b63d4(CDemoWork *work) {
    GFL_BGSysSetEnabledBGsA(0);
    GFL_BGSysSetEnabledBGsB(0);
    reg_G2_BLDCNT = 0;
    reg_G2S_DB_BLDCNT = 0;
    GFL_BGSysSetDisplayLayout(GX_DISP_SELECT_MAIN_SUB);
    func_ov179_021b615c();
    func_ov179_021b616c(work);
    work->arc = GFL_ArcSysCreateFileHandle(data_ov179_021b6a1c[work->param->demo].arcId, HEAPID_CDEMO);
    func_ov179_021b622c(work);
    func_ov179_021b624c(work);
    func_ov179_021b6290(work, 1, 1, 5);
    work->initialized = TRUE;
    return 2;
}
static u32 func_ov179_021b6448(CDemoWork *work) {
    if (!GFL_WipeIsFinished())
        return 1;
    if (GFL_SndBGMIsFading() == TRUE)
        return 1;
    func_02005d28();
    if (work->initialized == TRUE) {
        func_ov179_021b6268(work);
        GFL_ArcToolFree(work->arc);
        func_ov179_021b6248(work);
        func_ov179_021b6220();
    }
    return 6;
}
static u32 func_ov179_021b6490(CDemoWork *work) {
    if (GFL_WipeIsFinished() == TRUE)
        return work->resumeState;
    return 2;
}
// pokebw2's CDemo_StateTransition
static u32 func_ov179_021b64ac(CDemoWork *work) {
    return 3;
}
// pokebw2's CDemo_StateScript
static u32 func_ov179_021b64b0(CDemoWork *work) {
    while (TRUE) {
        if (*work->scriptCursor == 20) {
            work->param->skipped = FALSE;
            return 1;
        }
        if (func_ov179_021b66dc(work) != 1)
            break;
    }
    return work->state;
}
// pokebw2's CDemo_StateMovie
static u32 func_ov179_021b64e0(CDemoWork *work) {
    const CDemoMovie *movie = &data_ov179_021b6a1c[work->param->demo];
    if (work->lastTick == 0) {
        work->lastTick = OS_GetTick();
    } else {
        u64 now = OS_GetTick();
        work->elapsedFrameTime += OS_TicksToMicroSeconds(now - work->lastTick);
        work->lastTick = now;
        if (work->elapsedFrameTime < 0x1046b)
            return 4;
        work->elapsedFrameTime -= 0x1046b;
    }
    switch (work->frameState) {
    case 0:
        func_ov179_021b669c(work->arc, work->nextFrame, movie->screenOffset, movie->frameSize, 0);
        work->nextFrame++;
        func_ov179_021b669c(work->arc, work->nextFrame, movie->screenOffset, movie->frameSize, 1);
        work->nextFrame++;
        GFL_BGSysSetBGPriority(2, 0);
        GFL_BGSysSetBGPriority(3, 1);
        work->frameState = 1;
        break;
    case 1:
        GFL_BGSysSetBGPriority((u8)((work->currentBuffer ^ 1) + 2), 0);
        GFL_BGSysSetBGPriority((u8)(work->currentBuffer + 2), 1);
        func_ov179_021b669c(work->arc, work->nextFrame, movie->screenOffset, movie->frameSize, work->currentBuffer);
        work->nextFrame++;
        if (work->nextFrame >= movie->frameCount)
            work->frameState = 2;
        else
            work->currentBuffer ^= 1;
        break;
    case 2:
        GFL_BGSysSetBGPriority((u8)(work->currentBuffer + 2), 0);
        GFL_BGSysSetBGPriority((u8)((work->currentBuffer ^ 1) + 2), 1);
        work->frameState = 3;
        break;
    case 3:
        return 5;
    }
    return 4;
}
// pokebw2's CDemo_LoadMovieFrame
static void func_ov179_021b669c(ArcTool *arc, u32 fileId, u32 offset, u32 size, u32 buffer) {
    void *data = GFL_ArcToolReadHeapNewLZ(arc, fileId, TRUE, HEAPID_TAIL(HEAPID_CDEMO));
    DC_FlushRange(data, size);
    if (buffer == 0) {
        GX_LoadBG2Scr(data, offset, size);
    } else {
        GX_LoadBG3Scr(data, offset, size);
    }
    GFL_HeapFree(data);
}
