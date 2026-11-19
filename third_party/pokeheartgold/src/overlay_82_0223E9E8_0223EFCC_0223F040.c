#include "global.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "gf_gfx_planes.h"
#include "msgdata.h"
#include "bg_window.h"
#include "font_types_def.h"
#include "overlay_82_battle_hall_app.h"

struct BattleHallApp;

struct MsgData;

struct String;

struct Window;

void String_Delete(String *string);

void String_SetEmpty(String *string);

void CopyWindowToVram(Window *window);

void FillWindowPixelBuffer(Window *window, u8 val);

void DestroyMsgData(MsgData *loader);

typedef u32 TextColor;

u8 AddTextPrinterParameterizedWithColor(Window *window, u32 fontID, const String *string, u32 xOffset, u32 yOffset, u32 renderDelay, TextColor color, PrinterCallback_t callback);

u8 ov80_02237920(u8 cursorPos);

enum BattleHallAppState {
    STATE_FADE_IN = 0,
    STATE_MON_SUMMARY,
    STATE_SELECT_BATTLE,
    STATE_MULTI_CONFIRM,
    STATE_SYNC_BEFORE_EXIT,
    STATE_FADE_OUT,
    STATE_CLOSE_MESSAGE,
};

void LoadMainScreenBackground(BattleHallApp *app, enum GFBgLayer bgLayer);

void LoadSubScreenBackground(BattleHallApp *app, enum GFBgLayer bgLayer);

void BattleArcadeGameBoard_SetState(BattleHallApp *app, int *state, enum BattleHallAppState nextState);

void ov82_0223F040(BattleHallApp *app, Window *window, u8 textColor, u8 shadowColor, u8 bgColor, u8 fontID);

void ov82_0223F040(BattleHallApp *app, Window *window, u8 textColor, u8 shadowColor, u8 bgColor, u8 fontID) {
    int row, col;

    FillWindowPixelBuffer(window, bgColor);

    MsgData *loader = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, (enum NarcId)27, 735, HEAP_ID_105);
    String *strBuf = String_New(11, HEAP_ID_105);

    for (row = 0; row < 5; row++) {
        for (col = 0; col < 4; col++) {
            if (ov80_02237920(row * 4 + col) != 0xfe) {
                String_SetEmpty(strBuf);
                ReadMsgDataIntoString(loader, ov80_02237920(row * 4 + col), strBuf);
                AddTextPrinterParameterizedWithColor(window, fontID, strBuf, 1 + (64 * col), 16 + (36 * row), 0, ((TextColor)(((textColor & 0xFF) << 16) | ((shadowColor & 0xFF) << 8) | ((bgColor & 0xFF) << 0))), 0L);
            }
        }
    }

    String_Delete(strBuf);
    DestroyMsgData(loader);
    CopyWindowToVram(window);
}
