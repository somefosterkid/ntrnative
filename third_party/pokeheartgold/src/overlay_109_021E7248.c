#include "global.h"

#include "bg_window.h"
#include "message_format.h"
#include "player_data.h"
#include "pm_string.h"
#include "text.h"
#include "unk_02068F84.h"
#include "font.h"
#include "sprite.h"
#include "render_window.h"
#include "sprite_system.h"
#include "overlay_109_photo_album_app.h"

void ov109_021E7248(PhotoAlbumApp *app, Photo *photo) {
    MessageFormat *messageFormat;

    ScheduleBgTilemapBufferTransfer(app->bgConfig, 6U);
    ScheduleBgTilemapBufferTransfer(app->bgConfig, 7U);
    FillWindowPixelBuffer(&app->windows[2], 0U);
    BgClearTilemapBufferAndCommit(app->bgConfig, 4U);
    BgClearTilemapBufferAndCommit(app->bgConfig, 7U);
    if (photo == NULL) {
        ScheduleWindowCopyToVram(&app->windows[2]);
        return;
    }
    BufferPlayersName(app->msgFmt, 0U, Save_PlayerData_GetProfile(app->args->saveData));
    MapID_GetLandmarkName((u32)photo->mapId, app->heapID, app->tmpString);
    BufferString(app->msgFmt, 1U, app->tmpString, 2, 0, 2);
    CopyU16ArrayToString(app->tmpString, photo->leadMonNick);
    BufferString(app->msgFmt, 2U, app->tmpString, 2, 0, 2);
    BufferIntegerAsString(app->msgFmt, 3U, ((u8)(((u32)photo->date) >> 0x18)) + 0x7D0, 4U, PRINTING_MODE_LEADING_ZEROS, 1);
    BufferIntegerAsString(app->msgFmt, 4U, (s32)((u8)(((u32)photo->date) >> 0x10)), 2U, PRINTING_MODE_LEADING_ZEROS, 1);
    BufferIntegerAsString(app->msgFmt, 5U, (s32)((u8)(((u32)photo->date) >> 8)), 2U, PRINTING_MODE_LEADING_ZEROS, 1);
    messageFormat = app->msgFmt;
    if (ov109_021E7850(photo) > 1U) {
        StringExpandPlaceholders(app->msgFmt, app->tmpString, app->photoStrings[1]);
    } else {
        StringExpandPlaceholders(app->msgFmt, app->tmpString, app->photoStrings[0]);
    }
    AddTextPrinterParameterizedWithColor(&app->windows[2], FONT_SYSTEM, app->tmpString, 0U, 0U, 0xFFU, 0x30200U, 0L);
    ScheduleWindowCopyToVram(&app->windows[2]);
    ov109_021E7114(app, (u8)(photo->iconId + 1), 7, 0xD, 8);
}

void ov109_021E7388(PhotoAlbumApp *app, s32 arg1) {
    FillWindowPixelBuffer(&app->windows[0], 0U);
    if (arg1 != 0) {
        Sprite_SetAnimCtrlSeq(app->sprites[4], 9);
        Sprite_ResetAnimCtrlState(app->sprites[4]);
    }
    AddTextPrinterParameterizedWithColor(&app->windows[0], FONT_UNK_4, app->nameString, (u32)(0x40 - FontID_String_GetWidth(FONT_UNK_4, app->nameString, 0U)) >> 1, 0U, 0xFFU, 0x10500U, NULL);
    ScheduleWindowCopyToVram(&app->windows[0]);
    ScheduleBgTilemapBufferTransfer(app->bgConfig, 1U);
}

void ov109_021E73F8(PhotoAlbumApp *app, s32 arg1, s32 arg2, u32 textSpeed) {
    if (arg2 != 0) {
        DrawFrameAndWindow2(&app->windows[1], 1, 1U, 0xDU);
    }
    FillWindowPixelBuffer(&app->windows[1], 0xFU);
    if (textSpeed == 0) {
        AddTextPrinterParameterizedWithColor(&app->windows[1], FONT_SYSTEM, app->msgStrings[arg1], 0U, 0U, 0xFFU, 0x1020FU, NULL);
    } else {
        app->textPrinterId = AddTextPrinterParameterizedWithColor(&app->windows[1], FONT_SYSTEM, app->msgStrings[arg1], 0U, 0U, textSpeed, 0x1020FU, NULL);
    }
    ScheduleWindowCopyToVram(&app->windows[1]);
}

void ov109_021E7474(PhotoAlbumApp *app, u32 arg1, s32 arg2, s32 flag) {
    s16 y;
    s16 x;

    Sprite_SetDrawFlag(app->sprites[0], flag);
    if (arg2 == 3) {
        x = 0xC0;
        y = 0xA0;
        Sprite_SetAnimationFrame(app->sprites[0], 1U);
    } else {
        x = (arg1 * 0x30) + 0x1C + ((arg1 >> 1) * 8);
        y = (0x28 * arg2) + 8;
        Sprite_SetAnimationFrame(app->sprites[0], 0U);
    }
    Sprite_SetPositionXY(app->sprites[0], x, y);
}
