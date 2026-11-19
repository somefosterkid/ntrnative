#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "mail_message.h"
#include "options.h"
#include "palette.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_55.h"
#include "overlay_56.h"
#include "overlay_56_mail_viewer_app.h"
#include "msgdata.h"
#include "list_menu_items.h"

typedef struct MessageBankEntry MessageBankEntry;

typedef struct MessageBank MessageBank;

typedef struct ColoredArrow ColoredArrow;

typedef struct MenuTemplate MenuTemplate;

typedef struct Menu Menu;

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

BOOL ov56_021E5CE0(MailViewerApp *mailViewerApp);


BOOL ov56_021E5CE0(MailViewerApp *mailViewerApp) {
    BOOL BVar1;
    s32 iVar2;
    s32 iVar3;

    iVar2 = 0;
    iVar3 = 0;
    do {
        BVar1 = MailMsg_IsInit((MailMessage *)((u8 *)mailViewerApp->args->mailMessages + iVar3));
        if (BVar1 != 0) {
            return 0;
        }
        iVar2 = iVar2 + 1;
        iVar3 = iVar3 + 8;
    } while (iVar2 < 3);
    return 1;
}

void ov56_021E5D08(MailViewerApp *arg0) {
    PaletteData_BlendPalette(arg0->paletteData, PLTTBUF_MAIN_BG, (u16)(arg0->prevBlendIndex + 0x22), 1U, 0U, 0x7FFFU);
    *((s8 *)&arg0->blendDirection) = 0;
    *((s8 *)&arg0->blendFraction) = 0;
}
