// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov293/demo3d_capture.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/demo3d.h"
#include "app/demo3d/demo3d_capture.h"
#include "gfl/graphics.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "gfl/tcb.h"
#include "gfl/tcbl.h"
#include "nitro/gx.h"
#include "nitro/mi.h"

// The 3D demos' motion blur: every frame is captured to a VRAM bank, blended with the frame captured before it, and
// the bank is what the upper screen shows. The ROM has no name for this file, which lies between demo3d_data.c and
// the commands that use it; demo3d_capture.c is a guess, and it may be the start of the commands' file instead

struct Demo3DCapture {
    u32 lcdcBanks;
    Demo3DCaptureParam param;
    // The display shows the bank: the capture runs
    BOOL started;
    TCBEx *task;
};

void func_ov203_021beab8(TCBEx *task, void *data);
void func_ov203_021beaf8(TCB *tcb, void *data);
static void func_ov203_021beb48(const Demo3DCaptureParam *param);

// pokebw2's Demo3DCapture_Stop

Demo3DCapture *func_ov203_021be9b8(TCBExManager *manager, int eva, int evb) {
    Demo3DCaptureParam param = {
        GX_DISPMODE_VRAM_D,
        GX_BGMODE_0,
        GX_BG0_AS_3D,
        GX_CAPTURE_SIZE_256x192,
        GX_CAPTURE_MODE_AB,
        GX_CAPTURE_SRCA_2D3D,
        GX_CAPTURE_SRCB_VRAM_0x00000,
        GX_CAPTURE_DEST_VRAM_D_0x00000,
        0,
        0,
        HEAPID_DEMO3D,
    };

    param.eva = eva;
    param.evb = evb;
    return func_ov203_021be9f8(manager, &param);
}

void func_ov203_021be9e8(Demo3DCapture *capture) {
    func_ov203_021bea48(capture, GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BG0_AS_3D);
}

// pokebw2's Demo3DCapture_Create
Demo3DCapture *func_ov203_021be9f8(TCBExManager *manager, const Demo3DCaptureParam *param) {
    TCBEx *task = GFL_TCBExMgrAddTask(manager, func_ov203_021beab8, sizeof(Demo3DCapture), 5);
    Demo3DCapture *capture = GFL_TCBExGetData(task);

    capture->param = *param;
    capture->task = task;
    capture->started = FALSE;
    capture->lcdcBanks = GX_GetBankForLCDC();
    func_ov203_021beb48(&capture->param);
    GFL_VBlankTCBAdd(func_ov203_021beaf8, capture, 0);
    return capture;
}

// pokebw2's Demo3DCapture_End
void func_ov203_021bea48(Demo3DCapture *capture, int dispMode, int bgMode, int bg0As3D) {
    GX_SetGraphicsMode(dispMode, bgMode, bg0As3D);
    GX_SetBankForLCDC(capture->lcdcBanks);
    switch (capture->param.dispMode) {
    case GX_DISPMODE_VRAM_A:
        MIi_CpuClearFast(0, (void *)HW_LCDC_VRAM_A, 0x20000);
        break;
    case GX_DISPMODE_VRAM_B:
        MIi_CpuClearFast(0, (void *)HW_LCDC_VRAM_B, 0x20000);
        break;
    case GX_DISPMODE_VRAM_C:
        MIi_CpuClearFast(0, (void *)HW_LCDC_VRAM_C, 0x20000);
        break;
    case GX_DISPMODE_VRAM_D:
        MIi_CpuClearFast(0, (void *)HW_LCDC_VRAM_D, 0x20000);
        break;
    default:
        GX_SetBankForLCDC(GX_VRAM_LCDC_NONE);
        break;
    }
    GFL_TCBExRequestEnd(capture->task);
}

// pokebw2's Demo3DCapture_Task
void func_ov203_021beab8(TCBEx *task, void *data) {
    Demo3DCapture *capture = data;

    if (capture->started) {
        GX_SetCapture(capture->param.size, capture->param.mode, capture->param.sourceA, capture->param.sourceB,
                      capture->param.dest, capture->param.eva, capture->param.evb);
    }
}

// Shows the bank that the frames are captured to, from the first vertical blank on
// pokebw2's Demo3DCapture_VBlank
void func_ov203_021beaf8(TCB *tcb, void *data) {
    Demo3DCapture *capture = data;

    switch (capture->param.dispMode) {
    case GX_DISPMODE_VRAM_A:
        GX_SetBankForLCDC(GX_VRAM_A);
        break;
    case GX_DISPMODE_VRAM_B:
        GX_SetBankForLCDC(GX_VRAM_B);
        break;
    case GX_DISPMODE_VRAM_C:
        GX_SetBankForLCDC(GX_VRAM_C);
        break;
    case GX_DISPMODE_VRAM_D:
        GX_SetBankForLCDC(GX_VRAM_D);
        break;
    default:
        GX_SetBankForLCDC(GX_VRAM_LCDC_NONE);
        break;
    }
    GX_SetGraphicsMode(capture->param.dispMode, capture->param.bgMode, capture->param.bg0As3D);
    capture->started = TRUE;
    GFL_TCBRemove(tcb);
}

// Clears the bank and captures the first frame alone
// pokebw2's Demo3DCapture_Begin
static void func_ov203_021beb48(const Demo3DCaptureParam *param) {
    switch (param->dispMode) {
    case GX_DISPMODE_VRAM_A:
        MIi_CpuClearFast(0, (void *)HW_LCDC_VRAM_A, 0x20000);
        break;
    case GX_DISPMODE_VRAM_B:
        MIi_CpuClearFast(0, (void *)HW_LCDC_VRAM_B, 0x20000);
        break;
    case GX_DISPMODE_VRAM_C:
        MIi_CpuClearFast(0, (void *)HW_LCDC_VRAM_C, 0x20000);
        break;
    case GX_DISPMODE_VRAM_D:
        MIi_CpuClearFast(0, (void *)HW_LCDC_VRAM_D, 0x20000);
        break;
    }
    GX_SetCapture(param->size, param->mode, param->sourceA, param->sourceB, param->dest, 16, 0);
}
