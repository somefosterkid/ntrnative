#include "global.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "list_menu_items.h"
#include "overlay_83_battle_castle_self_app.h"
#include "pm_string.h"
#include "string_util.h"
#include "text.h"

void ov83_02242894(u32 arg0, s16 *arg1, s16 *arg2);

void ov83_02240C48(BattleCastleSelfApp *app, u32 idx, s32 num, u32 numDigits, enum PrintingMode strconvmode);
u16 ov83_02240EC4(BattleCastleSelfApp *app, u16 value, s32 challengeType);
void ov83_02241DD8(BattleCastleSelfApp *app, Window *window, MsgData *msgData, s32 strno, s32 x, s32 y, s32 fontId, s32 color, s32 alignment);
void ov83_0224755C(struct BattleCastleAppSprite *sprite, BOOL flag);
void ov83_02247568(struct BattleCastleAppSprite *sprite, u16 x, u16 y);
void ov83_022428A8(BattleCastleSelfApp *app, u32 param_2);

void ov83_02242894(u32 arg0, s16 *arg1, s16 *arg2) {
    *arg1 = (u8)(arg0 << 7) + 0x14;
    *arg2 = (0x28 * (arg0 >> 1)) + 0x38;
}

void ov83_022428A8(BattleCastleSelfApp *app, u32 param_2) {
    void *ptr;
    u32 uVar2;
    s32 idx;
    NNSG2dScreenData *pNStack_18;

    uVar2 = 0;
    do {
        ClearWindowTilemapAndScheduleTransfer(&app->windows[uVar2 + 0x30]);
        if (uVar2 != param_2) {
            ov83_0224755C(app->listItemSprites[uVar2], 0);
        } else {
            ov83_02247568(app->listItemSprites[uVar2], 0x44, 0x4c);
        }
        uVar2 = uVar2 + 1;
    } while (uVar2 < 6);
    ov83_0224755C(app->itemSelectCursorSprite, 0);
    ptr = GfGfxLoader_GetScrnDataFromOpenNarc(app->narc, 0x27, 1, &pNStack_18, HEAP_ID_107);
    LoadRectToBgTilemapRect(app->bgConfig, 2, pNStack_18->rawData, 0, 0, 0x20, 0x18);
    ScheduleBgTilemapBufferTransfer(app->bgConfig, 2);
    Heap_Free(ptr);
    FillWindowPixelBuffer(&app->windows[54], 0);
    FillWindowPixelBuffer(&app->windows[55], 0);
    idx = app->page * 6;
    idx += param_2;
    AddTextPrinterParameterizedWithColor(&app->windows[54], FONT_SYSTEM, app->listItems[idx].text, 0, 0, 0xff, 0x10200, (PrinterCallback_t)0x0);
    ov83_02240C48(app, 0, ov83_02240EC4(app, app->listItems[idx].value & 0xffff, app->selectedMenuEntry), 2, PRINTING_MODE_LEFT_ALIGN);
    ov83_02241DD8(app, &app->windows[55], app->msgLoader, 0x68, 0, 0, 0, 0x10200, 0);
    ScheduleWindowCopyToVram(&app->windows[54]);
    ScheduleWindowCopyToVram(&app->windows[55]);
    return;
}
