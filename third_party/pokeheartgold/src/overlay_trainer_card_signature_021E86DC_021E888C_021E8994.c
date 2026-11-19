#include "global.h"
#include "sprite_resources_header.h"

#include "options.h"
#include "unk_0200A090.h"
#include "bg_window.h"
#include "pm_string.h"
#include "font_types_def.h"
#include "font.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "game_stats.h"
#include "msgdata.h"
#include "overlay_manager.h"
#include "yes_no_prompt.h"
#include "signature_app.h"

struct BgConfig;

struct TextPrinterTemplate;

typedef u16 charcode_t;

typedef BOOL (*TextPrinterCallback)(TextPrinterTemplate *, u16);


typedef u32 TextColor;

u8 AddTextPrinterParameterizedWithColor(Window *window, u32 fontID, const String *string, u32 xOffset, u32 yOffset, u32 renderDelay, TextColor color, TextPrinterCallback callback);

static void *ov52_021E8994(Window *window, String *string, int yOffset, u8 fontID, TextColor color);

static void *ov52_021E8994(Window *window, String *string, int yOffset, u8 fontID, TextColor color) {
    int stringWidth = FontID_String_GetWidth(FONT_SUBSCREEN, string, 0);
    int xOffset = (window->width * 8 - stringWidth) / 2;

    AddTextPrinterParameterizedWithColor(window, fontID, string, xOffset, yOffset, 0xFF, color, 0L);

    return window->pixelBuffer;
}

struct OverlayManager;

struct GameStats;

struct MsgData;

struct SpriteList;

struct SpriteResourceCollection;

struct MessageFormat;

struct YesNoPrompt;

enum BgLayer {
    BG_LAYER_MAIN_0 = 0,
    BG_LAYER_MAIN_1,
    BG_LAYER_MAIN_2,
    BG_LAYER_MAIN_3,
    BG_LAYER_SUB_0,
    BG_LAYER_SUB_1,
    BG_LAYER_SUB_2,
    BG_LAYER_SUB_3,

    BG_LAYER_MAX,
};

void AddWindowParameterized(BgConfig *bgConfig, Window *window, u8 bgLayer, u8 tilemapLeft, u8 tilemapTop, u8 width, u8 height, u8 palette, u16 baseTile);

void FillWindowPixelBuffer(Window *window, u8 val);

enum SpriteResourceType {
    SPRITE_RESOURCE_CHAR = 0,
    SPRITE_RESOURCE_PLTT,
    SPRITE_RESOURCE_CELL,
    SPRITE_RESOURCE_ANIM,
    SPRITE_RESOURCE_MULTI_CELL,
    SPRITE_RESOURCE_MULTI_ANIM,
    SPRITE_RESOURCE_MAX,
};

typedef BOOL (*OverlayFunc)(OverlayManager *appMan, int *state);

void sub_02013A50(const Window *window, int width, int height, int x, int y, char *output);

void ov52_021E925C(Window *unused0, int sLotoId, TextColor unused2, SignatureApp *unused3);

void ov52_021E89D4(SignatureApp *signatureApp, OverlayManager *appMan);

void ov52_021E89D4(SignatureApp *signatureApp, OverlayManager *appMan) {
    AddWindowParameterized(signatureApp->bgConfig, &signatureApp->messageWindow, BG_LAYER_MAIN_0, 2, 1, 27, 4, 13, ((1 + ((6 * 3) + ((2 * 2) * 3))) + (3 * 3)));
    FillWindowPixelBuffer(&signatureApp->messageWindow, 15);
    AddWindowParameterized(signatureApp->bgConfig, &signatureApp->signatureWindow, BG_LAYER_MAIN_1, 4, 9, 24, 8, 1, 1);
    FillWindowPixelBuffer(&signatureApp->signatureWindow, 2);
    AddWindowParameterized(signatureApp->bgConfig, &signatureApp->buttonWindow, BG_LAYER_MAIN_1, 26, 21, 8, 2, 2, (1 + 24 * 8));
    FillWindowPixelBuffer(&signatureApp->buttonWindow, 0);

    void *pixels = ov52_021E8994(&signatureApp->buttonWindow, signatureApp->buttonText, 1, FONT_SUBSCREEN, ((TextColor)(((14 & 0xFF) << 16) | ((7 & 0xFF) << 8) | ((2 & 0xFF) << 0))));

    DC_FlushRange(pixels, (8 * (8 / 2)) * 8 * 2);

    for (int i = 0; i < 2; i++) {
        sub_02013A50(&signatureApp->buttonWindow, 8 / 2, 2, (8 / 2) * i, 0, (char *)signatureApp->buttonPixelBuffer);
        DC_FlushRange(signatureApp->buttonPixelBuffer, (8 * (8 / 2)) * (8 / 2) * 2);
        GX_LoadOBJ(signatureApp->buttonPixelBuffer, i * (8 * (8 / 2)) * (8 / 2) * 2, (8 * (8 / 2)) * (8 / 2) * 2);
    }

    AddWindowParameterized(signatureApp->bgConfig, &signatureApp->descriptionWindow, BG_LAYER_MAIN_1, 2, 2, 28, 2, 13, (1 + 24 * 8) + 8 * 2);

    int stringWidth = FontID_String_GetWidth(FONT_MESSAGE, signatureApp->descriptionText, 0);
    int xOffset = (28 * 8 - stringWidth) / 2;

    FillWindowPixelBuffer(&signatureApp->descriptionWindow, 0);
    AddTextPrinterParameterizedWithColor(&signatureApp->descriptionWindow, FONT_MESSAGE, signatureApp->descriptionText, xOffset, 0, 0, ((TextColor)(((1 & 0xFF) << 16) | ((2 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);

    for (int i = 0; i < 5; i++) {
        AddWindowParameterized(signatureApp->bgConfig, &signatureApp->dummyWindows[i], BG_LAYER_SUB_0, 5, 3 + i * 4, 10, 2, 13, 1 + i * 20);
        FillWindowPixelBuffer(&signatureApp->dummyWindows[i], 0);
    }

    ov52_021E925C(signatureApp->dummyWindows, 0, ((TextColor)(((14 & 0xFF) << 16) | ((13 & 0xFF) << 8) | ((15 & 0xFF) << 0))), signatureApp);
}
