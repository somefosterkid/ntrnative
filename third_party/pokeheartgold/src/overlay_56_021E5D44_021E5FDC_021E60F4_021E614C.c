#include "global.h"
#include "sprite_resources_header.h"
#include "constants/heap.h"
#include "palette.h"
#include "struct_defs/resdat_narc_structs.h"
#include "font.h"
#include "pokemon_types_def.h"
#include "options.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "sprite_system.h"
#include "msgdata.h"
#include "list_menu_items.h"
#include "overlay_55.h"
#include "overlay_56.h"
#include "overlay_56_mail_viewer_app.h"

struct BgConfig;

struct ListMenuCursor;

struct MailMessage;

struct ManagedSprite;

struct ListMenu2D;

struct ListMenu2DTemplate;

struct MessageBank;

struct MessageBankEntry;

struct MsgData;

struct Options;

struct PaletteData;

struct PaletteFadeControl;

struct SpriteManager;

struct SpriteResourcesHeader;

struct SpriteResourcesHeaderList;

struct SpriteSystem;

struct String;

struct ListMenuItem;

struct Window;

enum MailViewerResult {
    MAIL_VIEWER_RESULT_EDIT_SENTENCE_1 = 0,
    MAIL_VIEWER_RESULT_EDIT_SENTENCE_2,
    MAIL_VIEWER_RESULT_EDIT_SENTENCE_3,
    MAIL_VIEWER_RESULT_CONFIRM,
    MAIL_VIEWER_RESULT_CANCEL = 0xFFFF,
};

void String_Delete(String *string);

typedef struct PaletteFadeControl {
    u16 unfadedMask;
    u16 wait : 6;
    u16 cur : 5;
    u16 end : 5;
    u16 target : 15;
    u16 sign : 1;
    u16 step : 4;
    u16 waitStep : 6;
    u16 unused : 6;
} PaletteFadeControl;


typedef struct SpriteResourcesHeaderList {
    SpriteResourcesHeader *headers;
    int length;
} SpriteResourcesHeaderList;

void ClearWindowTilemapAndCopyToVram(Window *window);

void FillWindowPixelBuffer(Window *window, u8 val);

typedef u32 TextColor;

u8 TextPrinterCheckActive(u8 printerID);

u8 AddTextPrinterParameterizedWithColor(Window *window, u32 fontID, const String *string, u32 xOffset, u32 yOffset, u32 renderDelay, TextColor color, PrinterCallback_t callback);

typedef struct ListMenuCursor {
    TextColor color;
    String *string;
} ListMenuCursor;

typedef struct MessageBankEntry {
    u32 offset;
    u32 length;
} MessageBankEntry;

typedef struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
} MessageBank;

typedef struct ListMenu2DTemplate {
    ListMenuItem *choices;
    Window *window;
    u8 fontID;
    u8 xSize;
    u8 ySize;
    u8 lineSpacing : 4;
    u8 suppressCursor : 2;
    u8 loopAround : 2;
} ListMenu2DTemplate;

typedef struct ListMenu2D {
    ListMenu2DTemplate template;
    ListMenuCursor *cursor;
    u32 cancelKeys;
    u8 dummy14;
    u8 cursorPos;
    u8 width;
    u8 xOffset;
    u8 yOffset;
    u8 letterWidth;
    u8 lineHeight;
    u8 lastAction;
    u8 heapID;
} ListMenu2D;

void DrawFrameAndWindow2(Window *window, s32 skipTransfer, u16 tile, u8 palette);

void ClearFrameAndWindow2(Window *window, u8 skipTransfer);

void ov56_021E609C(MailViewerApp *mailViewerApp);

int ov56_021E60F4(MailViewerApp *mailViewerApp);

BOOL ov56_021E614C(MailViewerApp *mailViewerApp);

BOOL ov56_021E614C(MailViewerApp *mailViewerApp) {
    switch (mailViewerApp->subState) {
    case 0:
        mailViewerApp->pauseGlowEffect = 1;
        DrawFrameAndWindow2(&mailViewerApp->windows[MAIL_VIEWER_WINDOW_MESSAGE], 1, 1 + (3 * 3), 6);
        FillWindowPixelBuffer(&mailViewerApp->windows[MAIL_VIEWER_WINDOW_MESSAGE], (15 << 4) | 15);
        String *string = String_New(38 * 2, mailViewerApp->heapID);
        ReadMsgDataIntoString(mailViewerApp->loader, 3, string);
        mailViewerApp->printerID = AddTextPrinterParameterizedWithColor(&mailViewerApp->windows[MAIL_VIEWER_WINDOW_MESSAGE], FONT_MESSAGE, string, 0, 0, mailViewerApp->textFrameDelay, (TextColor)((((1 & 0xFF) << 16) | ((2 & 0xFF) << 8)) | ((15 & 0xFF) << 0)), 0L);
        String_Delete(string);
        ov56_021E5D08(mailViewerApp);
        mailViewerApp->prevBlendIndex = mailViewerApp->blendIndex;
        break;

    case 1:
        if (TextPrinterCheckActive(mailViewerApp->printerID)) {
            return 0;
        }
        ov56_021E609C(mailViewerApp);
        break;

    case 2:;
        ;
        ;
        ;
        ;
        int result = ov56_021E60F4(mailViewerApp);
        if (result < 0) {
            return 0;
        }
        ClearFrameAndWindow2(&mailViewerApp->windows[MAIL_VIEWER_WINDOW_MESSAGE], 1);
        ClearWindowTilemapAndCopyToVram(&mailViewerApp->windows[MAIL_VIEWER_WINDOW_MESSAGE]);
        mailViewerApp->subState = 0;
        if (result) {
            mailViewerApp->args->result = MAIL_VIEWER_RESULT_CANCEL;
            return 1;
        } else {
            mailViewerApp->mode = mailViewerApp->initialMode;
            mailViewerApp->pauseGlowEffect = 0;
            return 0;
        }
    }

    mailViewerApp->subState++;
    return 0;
}
