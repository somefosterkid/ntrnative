#include "global.h"

#include "bg_window.h"
#include "msgdata.h"
#include "message_format.h"
#include "overlay_83_battle_castle_self_app.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "string_util.h"

void ov83_02240C48(BattleCastleSelfApp *app, u32 idx, s32 num, u32 numDigits, enum PrintingMode strconvmode);

void ov83_02241DD8(BattleCastleSelfApp *app, Window *window, MsgData *msgData, s32 strno, s32 x, s32 y, s32 fontId, s32 color, s32 alignment);

void ov83_022479E4(Window *window, MsgData *msgData, s32 strno, u32 x, u32 y, u32 fontId, u32 color, s32 alignment);

void ov83_02240984(BattleCastleSelfApp *app);

void ov83_02240984(BattleCastleSelfApp *app) {
    BoxPokemon *boxmon;
    Window *pWVar2;
    u32 uVar3;
    s32 new_var;
    uVar3 = 0x3e;
    pWVar2 = &app->windows[62];
    do {
        FillWindowPixelBuffer(pWVar2, 0);
        uVar3 = uVar3 + 1;
        pWVar2 = pWVar2 + 1;
    } while (uVar3 <= (0x46 - 1));
    ov83_022479E4(&app->windows[64], app->msgLoader, 0x58, 0, 0, 0, 0x10200, 0);
    ov83_022479E4(&app->windows[68], app->msgLoader, 0x46, 0, 0, 0, 0x10200, 0);
    ov83_022479E4(&app->windows[66], app->msgLoader, 0x59, 0, 0, 0, 0x10200, 0);
    ov83_02240C48(app, 0, app->stats[0], 3, PRINTING_MODE_LEFT_ALIGN);
    ov83_02240C48(app, 1, app->stats[1], 3, PRINTING_MODE_LEFT_ALIGN);
    new_var = GetWindowWidth(&app->windows[67]);
    ov83_02241DD8(app, &app->windows[67], app->msgLoader, 0x5f, (8 * new_var) / 2, 0, 0, 0x10200, 2);
    boxmon = Mon_GetBoxMon(app->mon);
    BufferBoxMonNickname(app->strTemplate, 0, boxmon);
    ov83_02241DD8(app, &app->windows[62], app->msgLoader, 0x5b, 0, 0, 0, 0x10200, 0);
    if (app->noPrintGender == 0) {
        if (app->gender == 0) {
            ov83_022479E4(&app->windows[63], app->msgLoader, 0x56, 0, 0, 0, 0x50600, 0);
        } else if (app->gender == 1) {
            ov83_022479E4(&app->windows[63], app->msgLoader, 0x57, 0, 0, 0, 0x30400, 0);
        }
    }
    ov83_02240C48(app, 0, app->level, 3, PRINTING_MODE_LEFT_ALIGN);
    ov83_02241DD8(app, &app->windows[65], app->msgLoader, 0x5e, 0, 0, 0, 0x10200, 0);
    BufferItemName(app->strTemplate, 0, app->heldItem);
    ov83_02241DD8(app, &app->windows[69], app->msgLoader, 0x47, 0, 0, 0, 0x10200, 0);
    uVar3 = 0x3e;
    pWVar2 = &app->windows[62];
    do {
        ScheduleWindowCopyToVram(pWVar2);
        uVar3 = uVar3 + 1;
        pWVar2 = pWVar2 + 1;
    } while (uVar3 <= (0x46 - 1));
    return;
}
