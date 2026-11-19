#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "font.h"
#include "font_types_def.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "sys_task.h"
#include "text.h"
#include "constants/player_avatar.h"
#include "field_types_def.h"
#include "overlay_102_easy_chat_app.h"
#include "mail_misc.h"
#include "overlay_102_easy_chat_categories_context.h"
#include "gf_gfx_planes.h"
#include "touch_hitbox_controller.h"

struct EasyChatWordList;

typedef struct EasyChatWordList EasyChatWordList;

u32 sub_02091C74(const EasyChatWordList *wordList, u32 letter);

u32 ov102_021EC4A4(void);

void ov102_021EC4A8(u32 index, String *out);

void ov102_021EC4CC(u32 index, u32 *outX, u32 *outY);

typedef u32 TextColor;

u32 ov102_021EB880(EasyChatCategoriesContext *ctx, BgConfig *bgConfig, u32 baseTile);

struct LocalMapObject;
int ov102_021EB94C(LocalMapObject *mapObj, u8 tileBehavior, int movementAction);

u32 ov102_021EB880(EasyChatCategoriesContext *ctx, BgConfig *bgConfig, u32 baseTile) {
    ((baseTile < 1024 - 25 * 8) ? (void)0 : GF_AssertFail());

    Window window;
    AddWindowParameterized(bgConfig, &window, GF_BG_LYR_MAIN_1, 34, 10, 25, 8, 11, baseTile);
    baseTile += 25 * 8;

    String *string = String_New(4, HEAP_ID_35);

    FillWindowPixelBuffer(&window, 15);

    u32 count = ov102_021EC4A4();

    for (u32 i = 0; i < count; i++) {
        u32 x, y;
        ov102_021EC4A8(i, string);
        ov102_021EC4CC(i, &x, &y);

        TextColor color;
        if (sub_02091C74(ctx->unk_08, i)) {
            color = ((TextColor)(((1 & 0xFF) << 16) | ((2 & 0xFF) << 8) | ((15 & 0xFF) << 0)));
        } else {
            color = ((TextColor)(((3 & 0xFF) << 16) | ((4 & 0xFF) << 8) | ((15 & 0xFF) << 0)));
        }

        AddTextPrinterParameterizedWithColor(&window, FONT_SYSTEM, string, x + 3, y, 0xFF, color, 0L);
    }

    CopyWindowPixelsToVram_TextMode(&window);
    PutWindowTilemap(&window);

    String_Delete(string);
    RemoveWindow(&window);

    return baseTile;
}

int ov102_021EB94C(LocalMapObject *mapObj, u8 tileBehavior, int movementAction) {
    return movementAction;
}
