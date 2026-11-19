#include "global.h"
#include "overlay_87_scratch_off_card_app.h"
#include "save.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "bg_window.h"
#include "pm_string.h"
#include "font_types_def.h"
#include "options.h"
#include "palette.h"
#include "overlay_manager.h"
#include "message_printer.h"
#include "yes_no_prompt.h"

struct OverlayManager;

struct OverlayManagerTemplate;

struct BgConfig;

struct MessagePrinter;

struct G2dRenderer;

struct MessageLoader;

struct NARC;

struct Options;

struct PaletteAnimator;

struct PaletteData;

struct SaveData;

struct ScratchOffCardApp;

struct ScratchOffCardsAppSprite;

struct ScratchOffCardsAppSpriteManager;

struct Sprite;

struct SpriteList;

struct SpriteResource;

struct SpriteResourceCollection;

struct String;

struct StringTemplate;

struct SysTask;

struct SysTaskQueue;

struct TextPrinterTemplate;

struct TouchPadDataBuffer;

struct Window;

struct YesNoPrompt;

typedef BOOL (*OverlayFunc)(OverlayManager *appMan, int *state);


void FillWindowPixelBuffer(Window *window, u8 val);

typedef BOOL (*TextPrinterCallback)(TextPrinterTemplate *, u16);

typedef struct MessageLoader MessageLoader;


enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

typedef struct StringTemplate StringTemplate;


typedef u32 TextColor;

u8 AddTextPrinterParameterizedWithColor(Window *window, u32 fontID, const String *string, u32 xOffset, u32 yOffset, u32 renderDelay, TextColor color, TextPrinterCallback callback);

typedef struct PaletteAnimator PaletteAnimator;

typedef void (*UnkFuncPtr_020157E4)(PaletteAnimator *, u16 *);

struct PaletteAnimator {
    SysTask *unk_00;
    UnkFuncPtr_020157E4 unk_04;
    PaletteData *unk_08;
    u16 unk_0C[16];
    u16 unk_2C[16];
    u8 unk_4C;
    u8 unk_4D;
    u8 unk_4E;
    u8 unk_4F;
};

u8 ov87_021E6F38(ScratchOffCardApp *app, Window *window, int entryID, u32 xOffset, u32 yOffset, u32 renderDelay, u8 fgColor, u8 shadowColor, u8 bgColor, u8 fontID);

struct MsgData;
struct MessageFormat;
void ReadMsgDataIntoString(struct MsgData *loader, u32 entryID, String *string);
void StringExpandPlaceholders(struct MessageFormat *template, String *dst, String *fmtString);

u8 ov87_021E6F38(ScratchOffCardApp *app, Window *window, int entryID, u32 xOffset, u32 yOffset, u32 renderDelay, u8 fgColor, u8 shadowColor, u8 bgColor, u8 fontID) {
    FillWindowPixelBuffer(window, bgColor);
    ReadMsgDataIntoString(app->msgLoader, entryID, app->fmtStr);
    StringExpandPlaceholders(app->strTemplate, app->displayStr, app->fmtStr);

    return AddTextPrinterParameterizedWithColor(window, fontID, app->displayStr, xOffset, yOffset, renderDelay, ((TextColor)(((fgColor & 0xFF) << 16) | ((shadowColor & 0xFF) << 8) | ((bgColor & 0xFF) << 0))), 0L);
}
