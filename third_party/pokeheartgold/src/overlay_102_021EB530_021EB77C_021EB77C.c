#include "global.h"
#include "overlay_102_easy_chat_app.h"
#include "sys_task.h"
#include "overlay_102_easy_chat_categories_context.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "msgdata.h"
#include "gf_gfx_planes.h"
#include "font.h"
#include "sprite.h"
#include "mail_misc.h"
#include "touch_hitbox_controller.h"

struct BgConfig;

struct EasyChatWordList;

struct MsgData;

struct String;

struct SysTask;

struct SysTaskQueue;

struct Window;

enum EasyChatGroup {
    GROUP_POKEMON = 0,
    GROUP_POKEMON_2,
    GROUP_MOVE,
    GROUP_MOVE_2,
    GROUP_STATUS,
    GROUP_TRAINER,
    GROUP_PEOPLE,
    GROUP_GREETINGS,
    GROUP_LIFESTYLE,
    GROUP_FEELINGS,
    GROUP_TOUGH_WORDS,
    GROUP_UNION,
    EASY_CHAT_GROUP_COUNT,
    GROUP_MODE_CANCEL_INDEX = EASY_CHAT_GROUP_COUNT,
};

void String_Delete(String *string);

void DestroyMsgData(MsgData *loader);

typedef struct EasyChatWordList EasyChatWordList;

u32 sub_02091C38(const EasyChatWordList *wordList, u32 group);

void AddWindowParameterized(BgConfig *bgConfig, Window *window, u8 bgLayer, u8 tilemapLeft, u8 tilemapTop, u8 width, u8 height, u8 palette, u16 baseTile);

void RemoveWindow(Window *window);

void PutWindowTilemap(Window *window);

void CopyWindowPixelsToVram_TextMode(Window *window);

void FillWindowPixelBuffer(Window *window, u8 val);

typedef u32 TextColor;

u8 AddTextPrinterParameterizedWithColor(Window *window, u32 fontID, const String *string, u32 xOffset, u32 yOffset, u32 renderDelay, TextColor color, PrinterCallback_t callback);

u32 ov102_021EB77C(EasyChatCategoriesContext *ctx, BgConfig *bgConfig, u32 baseTile);

u32 ov102_021EB77C(EasyChatCategoriesContext *ctx, BgConfig *bgConfig, u32 baseTile) {
    int tilemapLeft, tilemapTop;

    MsgData *loader = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, (enum NarcId)27, 282, HEAP_ID_35);

    tilemapLeft = 1;
    tilemapTop = 9;

    for (int i = 0; i < EASY_CHAT_GROUP_COUNT; i++) {
        if (i != 0 && i % 3 == 0) {
            tilemapLeft = 1;
            tilemapTop += 2 + 1;
        }

        String *string;
        TextColor color;

        if (sub_02091C38(ctx->unk_08, i)) {
            string = NewString_ReadMsgData(loader, i);
            color = ((TextColor)(((1 & 0xFF) << 16) | ((2 & 0xFF) << 8) | ((15 & 0xFF) << 0)));
        } else {
            string = NewString_ReadMsgData(loader, 12);
            color = ((TextColor)(((3 & 0xFF) << 16) | ((4 & 0xFF) << 8) | ((15 & 0xFF) << 0)));
        }

        ((baseTile < 1024 - 9 * 2) ? (void)0 : GF_AssertFail());

        Window window;

        AddWindowParameterized(bgConfig, &window, GF_BG_LYR_MAIN_1, tilemapLeft, tilemapTop, 9, 2, 11, baseTile);
        FillWindowPixelBuffer(&window, 15);

        u32 xOffset = (9 * 8 - FontID_String_GetWidth(FONT_SYSTEM, string, 0)) / 2;

        AddTextPrinterParameterizedWithColor(&window, FONT_SYSTEM, string, xOffset, 0, 0xFF, color, 0L);
        PutWindowTilemap(&window);
        CopyWindowPixelsToVram_TextMode(&window);
        String_Delete(string);
        RemoveWindow(&window);

        baseTile += 9 * 2;
        tilemapLeft += 9 + 1;
    }

    DestroyMsgData(loader);

    return baseTile;
}
