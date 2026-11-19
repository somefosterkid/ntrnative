#include "global.h"
#include "sprite_resources_header.h"

#include "unk_02005D10.h"
#include "overlay_55.h"
#include "overlay_56_mail_viewer_app.h"
#include "struct_defs/resdat_narc_structs.h"
#include "overlay_41_sprite_resource_collection.h"
#include "bg_window.h"
#include "list_menu_items.h"
#include "screen_fade_internal.h"

struct ListMenuCursor;

struct ListMenu2D;

struct ListMenu2DTemplate;

struct MessageBank;

struct MessageBankEntry;

struct SelectedPaletteData;

struct SpriteResourcesHeader;

struct SpriteResourcesHeaderList;

struct String;

struct ListMenuItem;

struct SysTask;

struct SysTaskQueue;

struct Window;

enum MailViewerResult {
    MAIL_VIEWER_RESULT_EDIT_SENTENCE_1 = 0,
    MAIL_VIEWER_RESULT_EDIT_SENTENCE_2,
    MAIL_VIEWER_RESULT_EDIT_SENTENCE_3,
    MAIL_VIEWER_RESULT_CONFIRM,
    MAIL_VIEWER_RESULT_CANCEL = 0xFFFF,
};

typedef struct SpriteResourcesHeaderList {
    SpriteResourcesHeader *headers;
    int length;
} SpriteResourcesHeaderList;

typedef u32 TextColor;

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

enum VerticalSelectionIndex {
    SELECTION_INDEX_SENTENCE_1 = 0,
    SELECTION_INDEX_SENTENCE_2,
    SELECTION_INDEX_SENTENCE_3,
    SELECTION_INDEX_CONFIRM_CANCEL,
    VERTICAL_SELECTION_INDEX_COUNT,
};

BOOL ov56_021E5CE0(MailViewerApp *mailViewerApp);

BOOL ov56_021E5DDC(MailViewerApp *mailViewerApp);

BOOL ov56_021E5DDC(MailViewerApp *mailViewerApp) {
    BOOL selectionChanged = 0;

    if (gSystem.newKeys & (0x0001)) {
        if (mailViewerApp->verticalSelectionIndex == SELECTION_INDEX_CONFIRM_CANCEL) {
            if (mailViewerApp->horizontalSelectionIndex == 0) {
                if (ov56_021E5CE0(mailViewerApp)) {
                    PlaySE(1501);
                    mailViewerApp->mode = MAIL_VIEWER_MODE_CONFIRM_EMPTY;
                    return 0;
                } else {
                    mailViewerApp->args->result = MAIL_VIEWER_RESULT_CONFIRM;
                    PlaySE(1507);
                    mailViewerApp->args->mailMessageIdx = 0;
                    mailViewerApp->args->horizontalSelectionIndex = 0;
                    return 1;
                }
            } else {
                PlaySE(1501);
                mailViewerApp->mode = MAIL_VIEWER_MODE_CANCEL;
                return 0;
            }
        } else {
            mailViewerApp->args->result = mailViewerApp->args->mailMessageIdx = mailViewerApp->verticalSelectionIndex;
            mailViewerApp->args->horizontalSelectionIndex = mailViewerApp->horizontalSelectionIndex;
            PlaySE(1501);
        }

        return 1;
    } else if (gSystem.newKeys & (0x0002)) {
        PlaySE(1501);
        mailViewerApp->mode = MAIL_VIEWER_MODE_CANCEL;
        return 0;
    }

    if (gSystem.newKeys & (0x0008)) {
        mailViewerApp->verticalSelectionIndex = SELECTION_INDEX_CONFIRM_CANCEL;
        mailViewerApp->horizontalSelectionIndex = 0;
        selectionChanged = 1;
    } else if (gSystem.newKeys & (0x0080)) {
        mailViewerApp->verticalSelectionIndex = (mailViewerApp->verticalSelectionIndex + 1) % VERTICAL_SELECTION_INDEX_COUNT;
        selectionChanged = 1;
    } else if (gSystem.newKeys & (0x0040)) {
        mailViewerApp->verticalSelectionIndex = (mailViewerApp->verticalSelectionIndex + VERTICAL_SELECTION_INDEX_COUNT - 1) % VERTICAL_SELECTION_INDEX_COUNT;
        selectionChanged = 1;
    } else if (gSystem.newKeys & (0x0010 | 0x0020)) {
        if (mailViewerApp->verticalSelectionIndex == SELECTION_INDEX_CONFIRM_CANCEL) {
            mailViewerApp->horizontalSelectionIndex ^= 1;
            selectionChanged = 1;
        }
    } else {
        return 0;
    }

    if (!selectionChanged) {
        return 0;
    }

    PlaySE(1500);

    if (mailViewerApp->verticalSelectionIndex == SELECTION_INDEX_CONFIRM_CANCEL) {
        mailViewerApp->blendIndex = mailViewerApp->verticalSelectionIndex + mailViewerApp->horizontalSelectionIndex;
    } else {
        mailViewerApp->blendIndex = mailViewerApp->verticalSelectionIndex;
    }

    return 0;
}
