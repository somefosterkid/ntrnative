#include "global.h"
#include "sprite_resources_header.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "struct_defs/resdat_narc_structs.h"
#include "font.h"
#include "msgdata.h"
#include "overlay_55.h"
#include "overlay_56_mail_viewer_app.h"
#include "overlay_41_sprite_resource_collection.h"
#include "list_menu_items.h"

struct BgConfig;

struct ListMenuCursor;

struct MailMessage;

struct G2dRenderer;

struct ManagedSprite;

struct ListMenu2D;

struct ListMenu2DTemplate;

struct MessageBank;

struct MessageBankEntry;

struct MsgData;

struct Options;

struct PaletteBuffer;

struct PaletteData;

struct SelectedPaletteData;

struct GF_2DGfxRawResObj;

struct GF_2DGfxRawResMan;

struct Sprite;

struct SpriteList;

struct SpriteManager;

struct SpriteResource;

struct SpriteResourceCollection;

struct GF_2DGfxResObjList;

struct GF_2DGfxResHeader;

struct SpriteResourcesHeader;

struct SpriteResourcesHeaderList;

struct SpriteSystem;

struct String;

struct ListMenuItem;

struct Window;

void String_Delete(String *string);

void String_SetEmpty(String *string);

String *MailMsg_GetExpandedString(const MailMessage *sentence, enum HeapID heapID);


typedef struct SpriteResourcesHeaderList {
    SpriteResourcesHeader *headers;
    int length;
} SpriteResourcesHeaderList;

void CopyWindowToVram(Window *window);

typedef u32 TextColor;

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

void ov56_021E6AA4(MailViewerApp *mailViewerApp);

void ov56_021E6AA4(MailViewerApp *mailViewerApp) {
    for (int i = 0; i < 3; i++) {
        if (!MailMsg_IsInit(&mailViewerApp->args->mailMessages[i])) {
            continue;
        }

        String *sentenceStr = MailMsg_GetExpandedString(&mailViewerApp->args->mailMessages[i], mailViewerApp->heapID);

        AddTextPrinterParameterizedWithColor(&mailViewerApp->windows[i], FONT_MESSAGE, sentenceStr, 0, 0, 0, ((TextColor)(((1 & 0xFF) << 16) | ((2 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
        String_Delete(sentenceStr);
        CopyWindowToVram(&mailViewerApp->windows[i]);
    }

    if (mailViewerApp->mode == MAIL_VIEWER_MODE_WRITE) {
        mailViewerApp->loader = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, (enum NarcId)27, 233, mailViewerApp->heapID);

        String *string = String_New(8 * 2, mailViewerApp->heapID);

        for (int i = 0; i < 2; i++) {
            String_SetEmpty(string);
            ReadMsgDataIntoString(mailViewerApp->loader, 0 + i, string);

            int xOffset = 8 * 8 - FontID_String_GetWidth(FONT_MESSAGE, string, 0);
            xOffset /= 2;

            AddTextPrinterParameterizedWithColor(&mailViewerApp->windows[MAIL_VIEWER_WINDOW_CONFIRM + i], FONT_MESSAGE, string, xOffset, 2, 0, ((TextColor)(((1 & 0xFF) << 16) | ((2 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
            CopyWindowToVram(&mailViewerApp->windows[MAIL_VIEWER_WINDOW_CONFIRM + i]);
        }

        String_Delete(string);
    } else {
        AddTextPrinterParameterizedWithColor(&mailViewerApp->windows[MAIL_VIEWER_WINDOW_CANCEL], FONT_MESSAGE, mailViewerApp->args->mailAuthorName, 0, 1, 0, ((TextColor)(((1 & 0xFF) << 16) | ((2 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
        CopyWindowToVram(&mailViewerApp->windows[MAIL_VIEWER_WINDOW_CANCEL]);
    }
}
