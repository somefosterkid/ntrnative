// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov036/field_menu.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The field menu on the touch screen: its items' icons and names, the cursor, the return button, and scrolling it in
// and out. The name is the ROM's own, from GFL_HeapAllocate's file argument
#include "types.h"
#include "constants/arc.h"
#include "constants/text_banks.h"
#include "field/field.h"
#include "field/field_menu.h"
#include "field/player_state.h"
#include "field/subscreen.h"
#include "field/zone.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/graphics.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/math_util.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/sound.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "nitro/gx.h"
#include "nnsys/g2d.h"
#include "save/event_work.h"
#include "save/player_info.h"
#include "system/app_menu_common.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "system/printsys.h"
#include "system/wipe.h"

// The item that leaves a slot empty
#define FIELD_MENU_ITEM_NONE 7
// The slot of the button at the bottom
#define FIELD_MENU_SLOT_BOTTOM 6

// An item of the menu
typedef struct {
    BOOL active;
    BmpWin *win;
    u32 itemId;
    u32 chars;
    u32 cellAnims;
    // Where the cursor goes on the item
    u8 cursorX;
    u8 cursorY;
    ClActor *icon;
} FieldMenuItem;

struct FieldMenu {
    HeapID heapId;
    HeapID tmpHeapId;
    Field *field;
    FieldSubscreen *subscreen;
    u32 state;
    void *msgBGSys;
    EventWork *eventWork;
    PlayerInfo *playerInfo;
    u16 zoneId;
    // The cursor's column and row; the item is x + y * 2, and row 3 is the bottom button
    u8 cursorX;
    u8 cursorY;
    u8 decideFrames;
    u8 menuType;
    BOOL cursorDirty;
    BOOL cancel;
    // Whether the keys move the cursor, rather than the touch screen
    BOOL keyMode;
    u32 selectedItemId;
    FieldMenuItem items[7];
    FieldMenuItem *curItem;
    s16 scrollY;
    BOOL scrollDirty;
    u32 menuPalette;
    u32 menuChars;
    u32 menuCellAnims;
    u32 commonPalette;
    u32 commonChars;
    u32 commonCellAnims;
    ClActUnit *actUnit;
    ClActRenderer *renderer;
    ClActor *cursor;
    ClActor *returnButton;
    PrintQueue *printQueue;
    TCB *vblankTcb;
    u32 cursorFadeFrame;
};

// What func_ov021_021a9154 makes an item from
typedef struct {
    u32 itemId;
    u8 x;
    u8 y;
    u8 winX;
    u8 winY;
    u8 iconX;
    u8 iconY;
    StrBuf *name;
    ArcTool *arc;
    u32 charFile;
    u32 cellFile;
    u32 animFile;
} FieldMenuItemSetup;

// The player's name when the save has none
typedef struct {
    u16 str[7];
} FieldMenuName;

void func_ov021_021a84a8(FieldMenu *menu);
static void func_ov021_021a8588(void);
static void func_ov021_021a87a8(FieldMenu *menu);
static void func_ov021_021a8874(FieldMenu *menu, ArcTool *arc, u8 menuType, void *msgBGSys);
static int func_ov021_021a8b84(u8 menuType);
static void LoadFieldMenuTexts(FieldMenu *menu, ArcTool *arc, u8 menuType);
static void func_ov021_021a8d40(TCB *tcb, void *data);
static void func_ov021_021a8dd8(FieldMenu *menu);
static int func_ov021_021a8e78(u8 x, u8 y);
static BOOL func_ov021_021a8e80(FieldMenu *menu, u32 dir);
static void func_ov021_021a8eec(FieldMenu *menu);
static void func_ov021_021a8ffc(FieldMenu *menu);
static void func_ov021_021a9080(FieldMenu *menu, u8 x, u8 y);
static void func_ov021_021a90a4(FieldMenu *menu, BOOL cancel);
static void func_ov021_021a90e0(FieldMenu *menu);
static void func_ov021_021a9154(FieldMenu *menu, FieldMenuItem *item, const FieldMenuItemSetup *setup);
static void func_ov021_021a926c(FieldMenu *menu, FieldMenuItem *item);
static void func_ov021_021a9294(FieldMenu *menu, FieldMenuItem *item);
static void func_ov021_021a92a4(HeapID heapId, u32 bg);

// Where each slot's name window is, in tiles
static const u8 data_ov021_021d8b18[7][2] = {
#include "asset/field_menu_data_ov021_021d8b18.inc"
};

// The items of each kind of menu, the last one at the bottom button
static const u32 data_ov021_021d8d24[6][7] = {
#include "asset/field_menu_data_ov021_021d8d24.inc"
};

// The cursor's column and row of each slot
static const u8 data_ov021_021d8b26[7][2] = {
#include "asset/field_menu_data_ov021_021d8b26.inc"
};

// The message of each item's name
static const u16 data_ov021_021d8b34[8] = {7, 2, 1, 3, 4, 5, 6, 9};





// The screen of each kind of menu
static const u32 data_ov021_021d8b44[6] = {0x10, 0x12, 0x12, 0x11, 0x12, 0x12};

// Where the cursor moves from each slot, up, down, left and right, for menus of four, five and six items. 0xfe
// and 0xff are the bottom button's rows, 6 none
static const u8 data_ov021_021d8c4c[3][8][4] = {
#include "asset/field_menu_data_ov021_021d8c4c.inc"
};

typedef struct FieldMenuCursorColor {
    s32 red;
    s32 blue;
    s32 green;
} FieldMenuCursorColor;

static inline GXRgb FieldMenu_InterpolateCursorColor(const FieldMenuCursorColor *start, const FieldMenuCursorColor *end,
                                                     fx32 phase) {
    s32 red = phase * (end->red - start->red) / FX32_CONST(2) + start->red;
    s32 blue = phase * (end->blue - start->blue) / FX32_CONST(2) + start->blue;
    s32 green = phase * (end->green - start->green) / FX32_CONST(2) + start->green;

    return (blue << GX_RGB_B_SHIFT) | (green << GX_RGB_G_SHIFT) | red;
}

// The cursor's colors 0 and 1, each pulsing between a start and an end endpoint
const FieldMenuCursorColor data_ov036_021cff00[2][2] = {
#include "asset/field_menu_data_ov036_021cff00.inc"
};

// The characters, cells and animations of each item's icon
static const u32 data_ov021_021d8cac[10][3] = {
#include "asset/field_menu_data_ov021_021d8cac.inc"
};


FieldMenu *func_ov021_021a836c(HeapID heapId, HeapID tmpHeapId, FieldSubscreen *subscreen, Field *field, BOOL open) {
    FieldMenu *menu = GFL_HeapAllocate(heapId, sizeof(FieldMenu), FALSE, "field_menu.c", 308);
    GameSystem *gsys = Field_GetGameSystem(field);
    GameData *gameData = GSYS_GetGameData(gsys);
    ArcTool *arc;
    u8 menuType;

    menu->heapId = heapId;
    menu->tmpHeapId = tmpHeapId;
    menu->field = field;
    menu->subscreen = subscreen;
    menu->state = 1;
    menu->zoneId = PlayerState_GetZoneID(GSYS_GetPlayerState(gsys));
    menu->eventWork = GameData_GetEventWork(gameData);
    menu->playerInfo = GetGameDataPlayerInfo(gameData);
    menu->msgBGSys = Field_GetMsgBGSys(field);
    menu->cursorX = 0;
    menu->cursorY = 0;
    menu->cursorDirty = menu->keyMode = func_020355c4() ^ TRUE;
    menu->curItem = NULL;
    menu->vblankTcb = GFL_VBlankTCBAdd(func_ov021_021a8d40, menu, 0);
    menu->printQueue = func_0201c440(menu->heapId);
    menu->scrollDirty = FALSE;
    menu->scrollY = 0;
    arc = GFL_ArcSysCreateFileHandle(0x47, menu->tmpHeapId);
    menuType = func_ov021_021a8b18(gameData, menu->eventWork, menu->zoneId);
    menu->menuType = menuType;
    func_ov021_021a8874(menu, arc, menuType, menu->msgBGSys);
    LoadFieldMenuTexts(menu, arc, menuType);
    GFL_ArcToolFree(arc);
    GFL_BGSysSetBGEnabled(1, TRUE);
    if (open == TRUE) {
        menu->cursorX = 0;
        menu->cursorY = 0;
        menu->cursorDirty = TRUE;
        func_ov021_021a90e0(menu);
        func_ov021_021a8dd8(menu);
    } else {
        func_ov021_021a8e20(menu, func_020129c4(gameData));
        menu->cursorDirty = TRUE;
        if (menu->keyMode) {
            func_0204b3dc(menu->cursor, TRUE);
        } else {
            func_0204b3dc(menu->cursor, FALSE);
        }
    }
    func_0203d34c(FALSE, menu->tmpHeapId);
    return menu;
}

void func_ov021_021a84a8(FieldMenu *menu) {
    if (menu->cancel == TRUE) {
        func_ov021_021a0fb4(menu->subscreen, menu->heapId);
    }
}

// pokebw2's FieldMenu_Free
void func_ov021_021a84bc(FieldMenu *menu) {
    u8 i;

    for (i = 0; i < 7; i++) {
        func_ov021_021a926c(menu, &menu->items[i]);
    }
    func_ov021_021a84a8(menu);
    GFL_TCBRemove(menu->vblankTcb);
    func_0204b3b4(menu->returnButton);
    func_0204b3b4(menu->cursor);
    func_0204b084(menu->renderer);
    func_0204b1cc(menu->actUnit);
    func_0204afd8(menu->menuCellAnims);
    func_0204a8d4(menu->menuChars);
    func_0204ada4(menu->menuPalette);
    func_0204afd8(menu->commonCellAnims);
    func_0204a8d4(menu->commonChars);
    func_0204ada4(menu->commonPalette);
    GFL_BGSysMoveBG(6, BG_MOVE_SET_Y, 0);
    GFL_BGSysReleaseBG(7);
    GFL_BGSysReleaseBG(5);
    GFL_BGSysReleaseBG(6);
    func_0201c6ec(menu->printQueue);
    func_0201c4c0(menu->printQueue);
    GFL_HeapFree(menu);
}

static void func_ov021_021a8588(void) {
    GFL_BGSysLoadScr(6);
    GFL_BGSysSetBGEnabled(4, TRUE);
    GFL_BGSysSetBGEnabled(5, TRUE);
    GFL_BGSysSetBGEnabled(6, TRUE);
    GFL_BGSysSetBGEnabled(7, TRUE);
}

// pokebw2's FieldMenu_Update
void func_ov021_021a85b4(FieldMenu *menu) {
    switch (menu->state) {
    case 1:
        if (func_0201c6b4(menu->printQueue) == TRUE) {
            u8 i;

            func_ov021_021a12d4(menu->subscreen, menu->tmpHeapId);
            for (i = 0; i < 7; i++) {
                func_ov021_021a9294(menu, &menu->items[i]);
            }
            if (menu->scrollY != 0) {
                GFL_SndSEPlay(0x554);
                menu->state = 0;
            } else {
                menu->state = 2;
                func_ov021_021a8588();
            }
        }
        break;
    case 0:
        if (menu->scrollY < 0x40) {
            menu->scrollY = 0;
            menu->state = 3;
            func_ov021_021a8588();
        } else {
            menu->scrollY -= 0x40;
            func_ov021_021a87a8(menu);
        }
        menu->scrollDirty = TRUE;
        break;
    case 2:
        if (GFL_WipeIsFinished() == TRUE) {
            func_ov021_021a12d4(menu->subscreen, menu->tmpHeapId);
            menu->state = 3;
        }
        break;
    case 3:
        func_ov021_021a8eec(menu);
        func_ov021_021a8ffc(menu);
        func_ov021_021a90e0(menu);
        break;
    case 4:
        if (!func_0204b98c(menu->returnButton)) {
            menu->state = 5;
            GFL_BGSysSetBGEnabled(5, TRUE);
            GFL_BGSysSetBGEnabled(6, TRUE);
            GFL_BGSysSetBGEnabled(7, TRUE);
            GFL_BGSysSetBGEnabled(4, TRUE);
        }
        break;
    case 5:
        if (menu->cancel == TRUE || menu->cursorY == 3) {
            menu->state = 9;
            GFL_SndSEPlay(0x556);
            GFL_BGSysSetBGEnabled(4, FALSE);
            GFL_BGSysSetBGEnabled(5, TRUE);
            GFL_BGSysSetBGEnabled(6, TRUE);
            GFL_BGSysSetBGEnabled(7, TRUE);
            func_0204b3dc(menu->returnButton, FALSE);
        } else if (menu->curItem->icon != NULL) {
            menu->decideFrames = 0;
            func_ov021_021a9080(menu, menu->curItem->cursorX, menu->curItem->cursorY);
            func_0204b878(menu->cursor, 1);
            func_0204b3dc(menu->cursor, TRUE);
            menu->state = 6;
            GFL_SndSEPlay(0x54c);
        } else {
            menu->state = 8;
        }
        break;
    case 6:
        menu->decideFrames++;
        if (!func_0204b98c(menu->cursor)) {
            menu->state = 8;
        }
        break;
    case 7:
        FieldSubscreen_SaveReturnSubscreen(menu->subscreen, 4);
        menu->state = 10;
        break;
    case 8:
        FieldSubscreen_SaveReturnSubscreen(menu->subscreen, 3);
        menu->state = 10;
        break;
    case 9:
        if (menu->scrollY + 0x40 > 0xc0) {
            menu->scrollY = 0xc0;
            menu->state = 7;
        } else {
            menu->scrollY += 0x40;
            func_ov021_021a87a8(menu);
        }
        menu->scrollDirty = TRUE;
        break;
    }
    func_0201c4e4(menu->printQueue);
}

// Scrolls the actors with the BGs, hiding the icons that leave the screen
static void func_ov021_021a87a8(FieldMenu *menu) {
    ClActorPos scroll;
    ClActorPos iconPos;
    ClActorPos cursorPos;
    u8 i;

    scroll.x = 0;
    scroll.y = -menu->scrollY;
    func_0204b09c(menu->renderer, 0, &scroll);
    for (i = 0; i < 7; i++) {
        if (menu->items[i].icon != NULL && menu->items[i].itemId != FIELD_MENU_ITEM_NONE) {
            func_0204b454(menu->items[i].icon, &iconPos, 0);
            if (iconPos.y < 0xd0) {
                func_0204b3dc(menu->items[i].icon, TRUE);
            } else {
                func_0204b3dc(menu->items[i].icon, FALSE);
            }
        }
    }
    func_0204b454(menu->cursor, &cursorPos, 0);
    if (menu->keyMode && cursorPos.y < 0xd6) {
        func_0204b3dc(menu->cursor, TRUE);
    } else {
        func_0204b3dc(menu->cursor, FALSE);
    }
}

// pokebw2's FieldMenu_UpdateScroll
void func_ov021_021a8840(FieldMenu *menu) {
    if (menu->scrollDirty == TRUE) {
        GFL_BGSysMoveBGReq(5, BG_MOVE_SET_Y, -menu->scrollY);
        GFL_BGSysMoveBGReq(6, BG_MOVE_SET_Y, -5 - menu->scrollY);
        menu->scrollDirty = FALSE;
    }
}


void func_ov021_021a8874(FieldMenu *menu, ArcTool *arc, u8 menuType, void *msgBGSys) {
    static const BGSetup data_ov021_021d8b5c = {0, 0, 0x1000, 0, 2, 0, 0xc, 0, 0x5800, 0, 1, 0, 0};
    static const BGSetup data_ov021_021d8b7c = {0, 0, 0x800, 0, 1, 0, 0xa, 0, 0x5800, 0, 3, 0, 0};
    static const BGSetup data_ov036_021cfec0 = {0, 0, 0x800, 0, 1, 0, 2, 1, 0x8000, 0, 0, 0, 0};
    static const BGSetup data_ov021_021d8bbc = {0, 0, 0x1000, 0, 2, 0, 0xe, 0, 0x5800, 0, 3, 0, 0};
    ArcTool *common;
    ClActorSetup setup;

    GXS_SetMasterBrightness(-16);
    GFL_BGSysCreateBG(5, &data_ov021_021d8bbc, 0);
    GFL_BGSysSetBGEnabled(5, TRUE);
    GFL_BGSysClearScr(5);
    GFL_BGSysCreateBG(6, &data_ov021_021d8b5c, 0);
    GFL_BGSysSetBGEnabled(6, TRUE);
    GFL_BGSysClearScr(6);
    GFL_BGSysCreateBG(7, &data_ov021_021d8b7c, 0);
    GFL_BGSysSetBGEnabled(7, TRUE);
    GFL_BGSysClearScr(7);
    GFL_BGSysMoveBG(6, BG_MOVE_SET_Y, -5);
    GFL_G2DIOLoadArcNCLRDefault(arc, 0, 4, 0, 0, menu->tmpHeapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 2, 5, 0, 0, FALSE, menu->tmpHeapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, data_ov021_021d8b44[menuType], 5, 0, 0, FALSE, menu->tmpHeapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 0xe, 7, 0, 0, FALSE, menu->tmpHeapId);
    menu->menuPalette = func_0204ac18(arc, 1, CLACT_VRAM_SUB, 0, 0, 0xb, menu->heapId);
    menu->menuChars = func_0204a6c8(arc, 0xd, FALSE, CLACT_VRAM_SUB, menu->heapId);
    menu->menuCellAnims = func_0204af28(arc, 0x1c, 0x26, menu->heapId);
    common = GFL_ArcSysCreateFileHandle(getUINarcIdx(), menu->tmpHeapId);
    menu->commonPalette = func_0204ac18(common, func_02027614(), CLACT_VRAM_SUB, 0x160, 0, 2, menu->heapId);
    menu->commonChars = func_0204a6c8(common, func_02027618(), FALSE, CLACT_VRAM_SUB, menu->heapId);
    menu->commonCellAnims = func_0204af28(common, func_0202761c(0), func_02027620(0), menu->heapId);
    GFL_ArcToolFree(common);
    GFL_BGSysLoadScr(5);
    menu->actUnit = func_0204b100(8, 0, menu->heapId);
    {
        ClActSurfaceSetup surface = {0, 0, 0x100, 0xbf, 1, 0};

        menu->renderer = func_0204b034(&surface, 1, menu->heapId);
    }
    func_0204b258(menu->actUnit, menu->renderer);
    setup.x = 0x40;
    setup.y = 0x2c;
    setup.sequence = 0;
    setup.priority = 0;
    setup.bgPriority = 0;
    menu->cursor = func_0204b294(menu->actUnit, menu->menuChars, menu->menuPalette, menu->menuCellAnims, &setup, 0, menu->heapId);
    func_0204b954(menu->cursor, FX32_ONE);
    func_0204b92c(menu->cursor, TRUE);
    func_0204b84c(menu->cursor, 2);
    setup.x = 0xe0;
    setup.y = 0xa8;
    menu->returnButton = func_0204b294(menu->actUnit, menu->commonChars, menu->commonPalette, menu->commonCellAnims, &setup, 0, menu->heapId);
    func_0204b954(menu->returnButton, FX32_ONE);
    func_0204b92c(menu->returnButton, TRUE);
    func_0204b3dc(menu->returnButton, TRUE);
    func_0204b954(menu->returnButton, FX32_CONST(2));
}

// pokebw2's FieldMenu_LoadBar
void func_ov021_021a8afc(HeapID heapId, BOOL hideBg4) {
    func_ov021_021a92a4(heapId, 4);
    if (hideBg4) {
        GFL_BGSysSetBGEnabled(4, FALSE);
    }
}

u8 func_ov021_021a8b18(GameData *gameData, EventWork *eventWork, u32 zoneId) {
    u8 type = 0;

    if (GetZoneIsUnionRoom(zoneId) || IsZone150Or151(zoneId)) {
        type = 1;
    } else if (IsZoneRoyalUnova(zoneId)) {
        type = 4;
    } else if (GameData_IsForceSeasonSync(gameData)) {
        type = 5;
    } else if (!EventWork_FlagGet(eventWork, 0x961)) {
        type = 3;
    } else if (!EventWork_FlagGet(eventWork, 0x962)) {
        type = 2;
    }
    return type;
}


// How many items the kind of menu has
static int func_ov021_021a8b84(u8 menuType) {
    int i = 0;
    int count = 0;

    for (; data_ov021_021d8d24[menuType][i] != 0; i++) {
        if (data_ov021_021d8d24[menuType][i] != FIELD_MENU_ITEM_NONE) {
            count++;
        }
    }
    return count;
}

static void LoadFieldMenuTexts(FieldMenu *menu, ArcTool *arc, u8 menuType) {
    FieldMenuItemSetup setup;
    FieldMenuName name;
    MsgData *msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_LOAD_FIELD_MENU_TEXTS, menu->tmpHeapId);
    const u32 *items;
    u8 i;

    for (i = 0; i < 7; i++) {
        menu->items[i].active = FALSE;
    }
    items = data_ov021_021d8d24[menuType];
    // The counter is set before the template's copy, which the block's initializer makes here
    i = 0;
    {
        FieldMenuName noName = {{'N', 'o', 'N', 'a', 'm', 'e', 0xffff}};

        for (; i < 7; i++) {
            u32 itemId = items[i];

            if (itemId == 4) {
                setup.name = GFL_StrBufCreate(16, menu->tmpHeapId);
                if (!func_020084bc(menu->playerInfo)) {
                    GFL_StrBufLoadString(setup.name, GetPlayerName(menu->playerInfo));
                } else {
                    name = noName;
                    GFL_StrBufLoadString(setup.name, name.str);
                }
            } else {
                setup.name = GFL_MsgDataLoadStrbufNew(msgData, data_ov021_021d8b34[itemId]);
            }
            setup.itemId = itemId;
            if (i != FIELD_MENU_SLOT_BOTTOM) {
                setup.x = (i % 2) * 0x80 + 0x40;
                setup.y = (i / 2) * 0x30 + 0x2c;
                setup.iconX = setup.x - 0x20;
                setup.iconY = setup.y;
                setup.arc = arc;
                setup.charFile = data_ov021_021d8cac[itemId][0];
                setup.cellFile = data_ov021_021d8cac[itemId][1];
                setup.animFile = data_ov021_021d8cac[itemId][2];
                if (itemId == 3 && getTrainerGender(menu->playerInfo) == 1) {
                    setup.charFile = 7;
                    setup.cellFile = 0x16;
                    setup.animFile = 0x20;
                }
                if (itemId == 2 && getTrainerGender(menu->playerInfo) == 1) {
                    setup.charFile = 0xc;
                    setup.cellFile = 0x1b;
                    setup.animFile = 0x25;
                }
            } else {
                setup.x = 0x80;
                setup.y = 0xb4;
                setup.iconX = 0;
                setup.iconY = 0;
                setup.arc = NULL;
                setup.charFile = 0;
                setup.cellFile = 0;
                setup.animFile = 0;
            }
            setup.winX = data_ov021_021d8b18[i][0];
            setup.winY = data_ov021_021d8b18[i][1];
            func_ov021_021a9154(menu, &menu->items[i], &setup);
            GFL_StrBufFree(setup.name);
        }
    }
    GFL_MsgDataFree(msgData);
}

// Pulses the cursor's colors
static void func_ov021_021a8d40(TCB *tcb, void *data) {
    FieldMenu *menu = data;
    fx32 t = func_0203f684((u16)menu->cursorFadeFrame) + FX32_ONE;
    GXRgb colors[2];

    colors[1] = FieldMenu_InterpolateCursorColor(&data_ov036_021cff00[0][0], &data_ov036_021cff00[0][1], t);
    GXS_LoadOBJPltt(&colors[1], 0x142, sizeof(GXRgb));
    colors[0] = FieldMenu_InterpolateCursorColor(&data_ov036_021cff00[1][0], &data_ov036_021cff00[1][1], t);
    GXS_LoadOBJPltt(&colors[0], 0x15e, sizeof(GXRgb));
    menu->cursorFadeFrame += 6;
}

// Shows the menu open, without scrolling it in
void func_ov021_021a8dd8(FieldMenu *menu) {
    ClActorPos pos;

    menu->scrollY = 0xc0;
    GFL_BGSysMoveBG(5, BG_MOVE_SET_Y, -0xc0);
    GFL_BGSysMoveBG(6, BG_MOVE_SET_Y, -0xc5);
    pos.x = 0;
    pos.y = 0xc0;
    func_0204b09c(menu->renderer, 0, &pos);
}

// pokebw2's FieldMenu_GetSelectedItem
u32 func_ov021_021a8e10(FieldMenu *menu) {
    if (menu->curItem == NULL) {
        return 0;
    }
    return menu->curItem->itemId;
}

// pokebw2's FieldMenu_SetCursorItem
void func_ov021_021a8e20(FieldMenu *menu, u32 itemId) {
    u8 i = 0;
    FieldMenuItem *items = menu->items;

    for (; i < 7; i++) {
        if (menu->items[i].active == TRUE && itemId == menu->items[i].itemId) {
            FieldMenuItem *item;

            menu->cursorX = i % 2;
            menu->cursorY = i / 2;
            menu->cursorDirty = TRUE;
            item = &items[menu->cursorX + menu->cursorY * 2];
            func_ov021_021a9080(menu, item->cursorX, item->cursorY);
        }
    }
}

static int func_ov021_021a8e78(u8 x, u8 y) {
    return x + y * 2;
}

// Moves the cursor in the direction, and returns whether it moved
static BOOL func_ov021_021a8e80(FieldMenu *menu, u32 dir) {
    int slot = func_ov021_021a8e78(menu->cursorX, menu->cursorY);
    u8 next = data_ov021_021d8c4c[func_ov021_021a8b84(menu->menuType) - 4][slot][dir];

    if (slot != next) {
        if (next == 0xff) {
            menu->cursorY = 2;
            return TRUE;
        }
        if (next == 0xfe) {
            menu->cursorY = 1;
            return TRUE;
        }
        if (next != FIELD_MENU_SLOT_BOTTOM) {
            menu->cursorX = data_ov021_021d8b26[next][0];
        }
        menu->cursorY = data_ov021_021d8b26[next][1];
        return TRUE;
    }
    return FALSE;
}

static void func_ov021_021a8eec(FieldMenu *menu) {
    u32 trg = GCTX_HIDGetPressedKeys();
    u32 repeat = GCTX_HIDGetTypedKeys();

    if (menu->keyMode == FALSE && (trg | repeat) != 0) {
        if ((trg & PAD_BUTTON_B) || (trg & PAD_BUTTON_X)) {
            func_ov021_021a90a4(menu, TRUE);
            return;
        }
        GFL_SndSEPlay(0x548);
        menu->keyMode = TRUE;
        menu->cursorDirty = TRUE;
        menu->curItem = NULL;
        func_0204b3dc(menu->cursor, TRUE);
        return;
    }
    if (repeat & PAD_KEY_UP) {
        if (func_ov021_021a8e80(menu, 0)) {
            menu->cursorDirty = TRUE;
            GFL_SndSEPlay(0x548);
        }
        return;
    }
    if (repeat & PAD_KEY_DOWN) {
        if (func_ov021_021a8e80(menu, 1)) {
            menu->cursorDirty = TRUE;
            GFL_SndSEPlay(0x548);
        }
        return;
    }
    if (repeat & PAD_KEY_LEFT) {
        if (func_ov021_021a8e80(menu, 2)) {
            menu->cursorDirty = TRUE;
            GFL_SndSEPlay(0x548);
        }
        return;
    }
    if (repeat & PAD_KEY_RIGHT) {
        if (func_ov021_021a8e80(menu, 3)) {
            menu->cursorDirty = TRUE;
            GFL_SndSEPlay(0x548);
        }
        return;
    }
    if ((trg & PAD_BUTTON_B) || (trg & PAD_BUTTON_X)) {
        func_ov021_021a90a4(menu, TRUE);
        return;
    }
    if (trg & PAD_BUTTON_A) {
        if (menu->items[func_ov021_021a8e78(menu->cursorX, menu->cursorY)].itemId != FIELD_MENU_ITEM_NONE) {
            func_ov021_021a90a4(menu, FALSE);
            func_020355d8(FALSE);
        }
    }
}

static void func_ov021_021a8ffc(FieldMenu *menu) {
    TouchRect rects[8] = {
#include "asset/field_menu_rects.inc"
};
    int slot = func_02035c08(rects);

    if (slot != TOUCH_RECT_NONE && menu->items[slot].itemId != FIELD_MENU_ITEM_NONE) {
        menu->cursorX = slot % 2;
        menu->cursorY = slot / 2;
        menu->keyMode = FALSE;
        func_0204b3dc(menu->cursor, FALSE);
        if (slot == FIELD_MENU_SLOT_BOTTOM) {
            func_ov021_021a90a4(menu, TRUE);
            return;
        }
        menu->cursorDirty = TRUE;
        func_ov021_021a90a4(menu, FALSE);
        func_020355d8(TRUE);
    }
}

static void func_ov021_021a9080(FieldMenu *menu, u8 x, u8 y) {
    ClActorPos pos;

    pos.x = x;
    pos.y = y;
    func_0204b404(menu->cursor, &pos, 0);
    menu->cursorFadeFrame = 0;
}

// Chooses the item under the cursor, or cancels
static void func_ov021_021a90a4(FieldMenu *menu, BOOL cancel) {
    if (cancel == TRUE) {
        menu->cancel = TRUE;
        menu->state = 4;
        func_0204b878(menu->returnButton, 8);
    } else {
        menu->cancel = FALSE;
        menu->state = 5;
        menu->selectedItemId = menu->items[func_ov021_021a8e78(menu->cursorX, menu->cursorY)].itemId;
    }
}

static void func_ov021_021a90e0(FieldMenu *menu) {
    FieldMenuItem *prev;

    if (menu->cursorDirty == TRUE) {
        prev = menu->curItem;
        menu->curItem = &menu->items[menu->cursorX + menu->cursorY * 2];
        func_0204b878(menu->cursor, 0);
        func_ov021_021a9080(menu, menu->curItem->cursorX, menu->curItem->cursorY);
        if (menu->curItem->icon != NULL && menu->keyMode == TRUE) {
            func_0204b878(menu->curItem->icon, 1);
        }
        if (prev != NULL && prev->icon != NULL) {
            func_0204b878(prev->icon, 0);
        }
        menu->cursorDirty = FALSE;
    }
}

static void func_ov021_021a9154(FieldMenu *menu, FieldMenuItem *item, const FieldMenuItemSetup *setup) {
    Font *font = func_ov021_021904ac(Field_GetMsgBGSys(menu->field));
    ClActorSetupEx actorSetup;

    item->active = TRUE;
    item->itemId = setup->itemId;
    item->cursorX = setup->x;
    item->cursorY = setup->y;
    item->win = BmpWin_CreateDynamic(6, setup->winX, setup->winY, 8, 2, 13, TRUE);
    BmpWin_FlushMap(item->win);
    GFL_BitmapFill(BmpWin_GetBitmap(item->win), 0);
    func_0201c724(menu->printQueue, BmpWin_GetBitmap(item->win), 2, 1, setup->name, font, 0x440);
    if (setup->arc != NULL) {
        item->chars = func_0204a6c8(setup->arc, setup->charFile, FALSE, CLACT_VRAM_SUB, menu->heapId);
        item->cellAnims = func_0204af28(setup->arc, setup->cellFile, setup->animFile, menu->heapId);
        actorSetup.base.x = setup->iconX;
        actorSetup.base.y = setup->iconY;
        actorSetup.base.sequence = 0;
        actorSetup.base.priority = 0;
        actorSetup.base.bgPriority = 0;
        actorSetup.affineCenter.x = 0;
        actorSetup.affineCenter.y = 0;
        actorSetup.scaleX = FX32_ONE;
        actorSetup.scaleY = FX32_ONE;
        actorSetup.rotation = 0;
        actorSetup.affineMode = 2;
        item->icon = func_0204b324(menu->actUnit, item->chars, menu->menuPalette, item->cellAnims, &actorSetup, 0,
                                   menu->heapId);
        func_0204b954(item->icon, FX32_ONE);
        func_0204b92c(item->icon, TRUE);
        func_0204b878(item->icon, 0);
        if (item->itemId == FIELD_MENU_ITEM_NONE) {
            func_0204b3dc(item->icon, FALSE);
        } else {
            func_0204b3dc(item->icon, TRUE);
        }
    } else {
        item->icon = NULL;
    }
}

static void func_ov021_021a926c(FieldMenu *menu, FieldMenuItem *item) {
    if (item->active == TRUE) {
        BmpWin_Free(item->win);
        if (item->icon != NULL) {
            func_0204b3b4(item->icon);
            func_0204afd8(item->cellAnims);
            func_0204a8d4(item->chars);
        }
    }
}

static void func_ov021_021a9294(FieldMenu *menu, FieldMenuItem *item) {
    if (item->active == TRUE) {
        BmpWin_FlushChar(item->win);
    }
}

// Loads the bar at the bottom of the touch screen into the BG
static void func_ov021_021a92a4(HeapID heapId, u32 bg) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(getUINarcIdx(), heapId);
    NNSG2dScreenData *screen;
    void *data;
    u16 *raw;
    int i;

    GFL_G2DIOLoadArcNCLRDefault(arc, func_02027624(), 4, 0x60, 0x20, heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, func_02027628(), bg, 0x80, 0, FALSE, heapId);
    data = GFL_G2DIOReadNSCRArc(arc, func_0202762c(), FALSE, &screen, heapId);
    GFL_ArcToolFree(arc);
    raw = (u16 *)screen->rawData;
    for (i = 0; i < 0x300; i++) {
        raw[i] = (raw[i] + 0x80) | 0x3000;
    }
    GFL_BGSysLoadScrArea(bg, 0, 0x15, 0x20, 3, screen->rawData, 0, 0x15, 0x20, 0x18);
    GFL_BGSysQueueScrLoad(bg);
    GFL_HeapFree(data);
}
