#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "msgdata.h"
#include "options.h"
#include "palette.h"
#include "pm_string.h"
#include "render_window.h"
#include "sprite.h"
#include "sprite_system.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"
#include "list_menu_items.h"
#include "overlay_56.h"
#include "overlay_56_mail_viewer_app.h"

typedef struct MessageBankEntry MessageBankEntry;

typedef struct MessageBank MessageBank;

typedef struct ColoredArrow ColoredArrow;

typedef struct MenuTemplate MenuTemplate;

typedef struct Menu Menu;

typedef union MailIconData MailIconData;

struct MessageBankEntry {
    u32 offset;
    u32 length;
};

struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
};

typedef u32 TextColor;

struct ColoredArrow {
    TextColor color;
    String *string;
};

struct MenuTemplate {
    ListMenuItem *choices;
    Window *window;
    u8 fontID;
    u8 xSize;
    u8 ySize;
    u8 lineSpacing : 4;
    u8 suppressCursor : 2;
    u8 loopAround : 2;
};

struct Menu {
    MenuTemplate template;
    ColoredArrow *cursor;
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
};

union MailIconData {
    struct {
        u16 spriteIndex : 12;
        u16 palIndex : 4;
    } asStruct;
    u16 asValue;
};


s32 ov56_021E5FDC(MailViewerApp *mailViewerApp);

s32 ov56_021E5FDC(MailViewerApp *mailViewerApp) {
    String *temp_r0;
    s32 var_r4;
    u16 temp_r1;

    temp_r1 = mailViewerApp->subState;
    var_r4 = 0;
    switch (temp_r1) {
    case 0:
        DrawFrameAndWindow2(&mailViewerApp->windows[MAIL_VIEWER_WINDOW_MESSAGE], 1, 0xAU, 6U);
        FillWindowPixelBuffer(&mailViewerApp->windows[MAIL_VIEWER_WINDOW_MESSAGE], 0xFFU);
        temp_r0 = String_New(0x4CU, mailViewerApp->heapID);
        ReadMsgDataIntoString((MsgData *)mailViewerApp->loader, 2, temp_r0);
        AddTextPrinterParameterizedWithColor(&mailViewerApp->windows[MAIL_VIEWER_WINDOW_MESSAGE], FONT_MESSAGE, temp_r0, 0U, 0U, 0U, 0x1020FU, NULL);
        String_Delete(temp_r0);
        break;
    case 1:
        if (3 & gSystem.newKeys) {
            var_r4 = 1;
            mailViewerApp->unkA = 0;
        } else if (System_GetTouchNew() != 0) {
            var_r4 = 1;
            mailViewerApp->unkA = 1;
            ov56_021E5D08(mailViewerApp);
        }
        if (var_r4 == 0) {
            return 0;
        }
        ClearFrameAndWindow2(&mailViewerApp->windows[MAIL_VIEWER_WINDOW_MESSAGE], 1);
        ClearWindowTilemapAndCopyToVram(&mailViewerApp->windows[MAIL_VIEWER_WINDOW_MESSAGE]);
        mailViewerApp->subState = 0;
        mailViewerApp->mode = mailViewerApp->initialMode;
        return 0;
    }
    mailViewerApp->subState += 1;
    return 0;
}
