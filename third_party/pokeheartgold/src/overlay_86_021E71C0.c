#include "global.h"

#include "bg_window.h"
#include "dex_mon_measures.h"
#include "gf_gfx_loader.h"
#include "overlay_86_frontier_records_app.h"
#include "unk_020163E0.h"
#include "unk_02019BA4.h"

void *ov86_021E73EC(s32 arg0, u32 *arg1);

void ov86_021E71C0(FrontierRecordsApp *arg0) {
    arg0->flash.isBg = 0;
    arg0->flash.index = 0;
    arg0->flash.palA = 1;
    arg0->flash.palB = 0;
    arg0->flash.state = 0;
    arg0->flash.timer = 0;
}

s32 ov86_021E71FC(FrontierRecordsApp *arg0);

s32 ov86_021E71FC(FrontierRecordsApp *arg0) {
    if (arg0->facility == FACILITY_HALL) {
        return 1;
    }
    return 0;
}

void ov86_021E720C(FrontierRecordsApp *arg0) {
    ov86_021E74F0(arg0);
    ov86_021E7418(arg0);
    ov86_021E7598(arg0);
    ov86_021E7258(arg0, 1);
    arg0->unk_330 = sub_020163E0(NULL, PM_LCD_BOTTOM, 0xDU, HEAP_ID_121);
}

void ov86_021E723C(FrontierRecordsApp *arg0) {
    sub_020164C4(arg0->unk_330);
    ov86_021E74D0(arg0);
    ov86_021E73CC(arg0);
}

void ov86_021E7258(FrontierRecordsApp *arg0, s32 arg1) {
    switch (arg1) {
    case 0:
        ScheduleSetBgPosText(arg0->bgConfig, 2U, BG_POS_OP_SET_ROT, 0);
        ScheduleSetBgPosText(arg0->bgConfig, 2U, BG_POS_OP_SET_XSCALE, 0);
        ov86_021E7C70(arg0);
        ov86_021E73CC(arg0);
        ov86_021E7D8C(arg0);
        ov86_021E7DA8(arg0);
        ov86_021E757C(arg0);
        break;
    case 1:
        ScheduleSetBgPosText(arg0->bgConfig, 2U, BG_POS_OP_SET_ROT, 0);
        ScheduleSetBgPosText(arg0->bgConfig, 2U, BG_POS_OP_SET_XSCALE, 0x100);
        ov86_021E7C70(arg0);
        ScheduleWindowCopyToVram(&arg0->windows[4]);
        ov86_021E73CC(arg0);
        ov86_021E7630(arg0);
        ov86_021E77BC(arg0);
        ov86_021E752C(arg0);
        break;
    case 2:
        ScheduleSetBgPosText(arg0->bgConfig, 2U, BG_POS_OP_SET_ROT, 0x100);
        ScheduleSetBgPosText(arg0->bgConfig, 2U, BG_POS_OP_SET_XSCALE, 0x100);
        ClearWindowTilemapAndScheduleTransfer(&arg0->windows[4]);
        ov86_021E7DD4(arg0);
        ov86_021E7B68(arg0);
        ov86_021E7CF8(arg0);
        ov86_021E7344(arg0);
        ov86_021E7CA0(arg0);
        ov86_021E73CC(arg0);
        ov86_021E7860(arg0);
        ov86_021E752C(arg0);
        break;
    }
    ScheduleBgTilemapBufferTransfer(arg0->bgConfig, 2U);
}

void ov86_021E7344(FrontierRecordsApp *arg0) {
    if (arg0->lastPage == 0) {
        LoadRectToBgTilemapRect(arg0->bgConfig, 2U, arg0->tilemapRects[2], 0x22U, 0x34U, 4U, 4U);
        LoadRectToBgTilemapRect(arg0->bgConfig, 2U, arg0->tilemapRects[2], 0x26U, 0x34U, 4U, 4U);
    } else {
        LoadRectToBgTilemapRect(arg0->bgConfig, 2U, arg0->tilemapRects[0], 0x22U, 0x34U, 4U, 4U);
        LoadRectToBgTilemapRect(arg0->bgConfig, 2U, arg0->tilemapRects[1], 0x26U, 0x34U, 4U, 4U);
    }
    ScheduleBgTilemapBufferTransfer(arg0->bgConfig, 2U);
}

void ov86_021E73CC(FrontierRecordsApp *arg0) {
    GridInputHandler *inputHandler;

    inputHandler = arg0->inputHandler;
    if (inputHandler != NULL) {
        GridInputHandler_Free(inputHandler);
        arg0->inputHandler = NULL;
    }
}

void ov86_021E73E8(void) {
}

void *ov86_021E73EC(s32 arg0, u32 *arg1) {
    u32 sizeOut;
    void *data = GfGfxLoader_LoadFromNarc_GetSizeOut(GetPokedexDataNarcID(), arg0 + 0x12, 0, HEAP_ID_121, 1, &sizeOut);
    *arg1 = sizeOut >> 1;
    return data;
}
