// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov143/monolith_tool.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The Entralink monolith's helpers that its screens share: text in frame actors, the panels' palette effects, the
// return button, the menus and the bar of the White and Black levels. The name is the ROM's string, from
// GFL_HeapAllocate's assert

#include "types.h"
#include "app/monolith/monolith_tool.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "system/app_taskmenu.h"
#include "system/bmp_oam.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "system/palanm.h"
#include "system/printsys.h"
#include "system/wordset.h"
#include "app/monolith/monolith_main.h"
#include "field/intrude_work.h"
#include "struct_decls.h"

// The color of the task menus' text
#define MONOLITH_MENU_TEXT_COLOR 0x39e0

void func_ov071_021b6db4(MonolithScreenParam *param, u8 mode, int req);
u32 func_ov071_021b6e00(u32 req);
u32 func_ov071_021b6e0c(u32 req);
u32 func_ov071_021b6e18(u32 req);

static const ClActorSetup data_ov071_021ba80c = { 128, 0, 0, 11, 1 };

// The setup of func_ov071_021b6e1c's text. The ROM has it after data_ov071_021ba814; MWCC's sort of the objects by
// size puts it there only when it is declared first
static const BmpOamActorSetup data_ov071_021ba82c = { NULL, 0, 0, 0, 0, 200, 3, 0, CLACT_VRAM_MAIN };

static const BmpOamActorSetup data_ov071_021ba814 = { NULL, 0, 0, 0, 0, 10, 1, 0, CLACT_VRAM_MAIN };

// pokebw2's MonolithTool_DeleteText

void func_ov071_021b6a0c(MonolithWork *wk, MonolithTextActor *text, int sub, int width, int x, int y, u32 msgId,
                         WordSet *wordSet) {
    ClActorSetup frameSetup = data_ov071_021ba80c;
    BmpOamActorSetup textSetup = data_ov071_021ba814;
    int tiles;
    StrBuf *str;
    u32 vramType;

    switch (width) {
    case MONOLITH_TEXT_WIDE:
        tiles = 22;
        frameSetup.sequence = 0;
        textSetup.x = x - 88;
        break;
    case MONOLITH_TEXT_MEDIUM:
        tiles = 18;
        frameSetup.sequence = 1;
        textSetup.x = x - 72;
        break;
    case MONOLITH_TEXT_NARROW:
    default:
        tiles = 12;
        frameSetup.sequence = 8;
        textSetup.x = x - 48;
        break;
    }
    text->bitmap = GFL_BitmapCreate(tiles, 2, 0x20, HEAPID_MONOLITH);
    frameSetup.x = x;
    frameSetup.y = y;
    textSetup.y = y - 8;
    textSetup.bitmap = text->bitmap;
    textSetup.palette = wk->actorRes[sub].fontPalette;
    vramType = sub == 0 ? CLACT_VRAM_MAIN : CLACT_VRAM_SUB;
    textSetup.vramType = vramType;
    textSetup.surface = vramType;
    text->frame = func_0204b294(wk->clactUnit, wk->actorRes[sub].chars, wk->actorRes[sub].palette,
                                wk->actorRes[sub].cellAnims, &frameSetup, vramType, HEAPID_MONOLITH);
    text->text = func_020255a0(wk->bmpOam, &textSetup);
    str = GFL_MsgDataLoadStrbufNew(wk->msgMonolith, msgId);
    if (wordSet != NULL) {
        StrBuf *expanded = GFL_StrBufCreate(0x80, HEAPID_MONOLITH);

        GFL_WordSetFormatStrbuf(wordSet, expanded, str);
        func_0201c6fc(wk->printQueue, text->bitmap, 0, 0, expanded, wk->font);
        GFL_StrBufFree(expanded);
    } else {
        func_0201c6fc(wk->printQueue, text->bitmap, 0, 0, str, wk->font);
    }
    GFL_StrBufFree(str);
    text->printing = TRUE;
}

void func_ov071_021b6b38(MonolithTextActor *text) {
    func_0204b3b4(text->frame);
    func_0202570c(text->text);
    GFL_BitmapFree(text->bitmap);
    text->frame = NULL;
    text->text = NULL;
    text->bitmap = NULL;
}

// pokebw2's MonolithTool_SetTextVisible
void func_ov071_021b6b58(MonolithTextActor *text, BOOL visible) {
    func_0204b3dc(text->frame, visible);
    func_02025774(text->text, visible);
}

// pokebw2's MonolithTool_UpdateText
BOOL func_ov071_021b6b70(MonolithWork *wk, MonolithTextActor *text) {
    if (text->printing == TRUE && func_0201c6c4(wk->printQueue, text->bitmap) == FALSE) {
        func_020257d0(text->text);
        text->printing = FALSE;
        return TRUE;
    }
    return FALSE;
}

void func_ov071_021b6b98(MonolithScreenParam *param, MonolithTextActor *texts, int count, int cursor,
                                int panel) {
    int i;

    for (i = 0; i < count; i++) {
        if (i == cursor) {
            func_0204b6f4(texts[i].frame, 1, TRUE);
            func_ov071_021b6db4(param, PANEL_MODE_PULSE, panel);
        } else {
            func_0204b6f4(texts[i].frame, 0, TRUE);
        }
    }
    if (cursor == 0xff) {
        func_ov071_021b6db4(param, PANEL_MODE_NONE, panel);
    }
}

void func_ov071_021b6bec(MonolithScreenParam *param, MonolithTextActor *texts, int count, int cursor,
                                int panel) {
    int i;

    for (i = 0; i < count; i++) {
        if (i == cursor) {
            func_0204b6f4(texts[i].frame, 1, TRUE);
            func_ov071_021b6db4(param, PANEL_MODE_FLASH, panel);
        } else {
            func_0204b6f4(texts[i].frame, 0, TRUE);
        }
    }
}

// pokebw2's MonolithTool_SetPanelPulse
void func_ov071_021b6c30(MonolithScreenParam *param, BOOL pulse, int panel) {
    if (pulse == TRUE) {
        func_ov071_021b6db4(param, PANEL_MODE_PULSE, panel);
    } else {
        func_ov071_021b6db4(param, PANEL_MODE_NONE, panel);
    }
}

// pokebw2's MonolithTool_FlashPanel
void func_ov071_021b6c48(MonolithScreenParam *param, int panel) {
    func_ov071_021b6db4(param, PANEL_MODE_FLASH, panel);
}


// pokebw2's MonolithTool_ClearPanels
void func_ov071_021b6c54(MonolithScreenParam *param) {
    int i;
    for (i = 0; i < PANEL_CONTROL_MAX; i++) {
        MI_CpuFill8((u8 *)&param->unk_0C + i * 8, 0, 8);
    }
}

// pokebw2's MonolithTool_GetPanelMode
u8 func_ov071_021b6c74(MonolithScreenParam *param, int panel) {
    MonolithPanelControl *panels = (MonolithPanelControl *)&param->unk_0C;
    return panels[panel].mode;
}

void func_ov071_021b6c7c(MonolithScreenParam *param, int req) {
    MonolithPanelControl *panels = (MonolithPanelControl *)&param->unk_0C;
    MonolithPanelControl *panel = &panels[req];
    u32 colorPos;
    u32 targetPos;
    int count;
    int i;
    int fraction;

    GFL_ASSERT(req < PANEL_CONTROL_MAX);
    colorPos = func_ov071_021b6e00(req);
    targetPos = func_ov071_021b6e0c(req);
    count = func_ov071_021b6e18(req);
    switch (panel->mode) {
    case PANEL_MODE_PULSE:
        fraction = panel->fraction;
        if (panel->falling == FALSE) {
            fraction += 0x80;
            if (fraction >= 0x1100) {
                fraction = 0x1000;
                panel->falling ^= 1;
            }
        } else {
            fraction -= 0x80;
            if (fraction <= 0) {
                fraction = 0x100;
                panel->falling ^= 1;
            }
        }
        panel->fraction = fraction;
        for (i = 0; i < count; i++) {
            func_02021fd8(param->work->fade, req, colorPos + i, 1, fraction >> 8,
                          func_02021f3c(param->work->fade, req, 1, targetPos + i));
        }
        break;
    case PANEL_MODE_FLASH:
        if (panel->wait == 0) {
            panel->wait = 3;
            if (panel->fraction == 0) {
                panel->fraction = 0x1000;
            } else {
                panel->fraction = 0;
                panel->flashes++;
                if (panel->flashes > 1) {
                    func_ov071_021b6db4(param, PANEL_MODE_NONE, req);
                }
            }
            for (i = 0; i < count; i++) {
                func_02021fd8(param->work->fade, req, colorPos + i, 1, panel->fraction >> 8,
                              func_02021f3c(param->work->fade, req, 1, targetPos + i));
            }
        } else {
            panel->wait--;
        }
        break;
    }
}


void func_ov071_021b6db4(MonolithScreenParam *param, u8 mode, int req) {
    MonolithPanelControl *panel = (MonolithPanelControl *)&param->unk_0C + req;

    MI_CpuFill8(panel, 0, sizeof(MonolithPanelControl));
    panel->mode = mode;
    func_02021fd8(param->work->fade, req, func_ov071_021b6e00(req),
                            func_ov071_021b6e18(req), 0, 0);
}




// The panels of PaletteFade buffers 0 and 1 (the BGs) blend colors 0x32 to 0x34 toward 0x42 to 0x44, those of 2 and 3
// (the actors) 0x12 to 0x14 toward 0x22 to 0x24
// pokebw2's MonolithTool_GetPanelColorPos
u32 func_ov071_021b6e00(u32 req) {
    if (req <= 1) {
        return 0x32;
    }
    return 0x12;
}

// pokebw2's MonolithTool_GetPanelTargetPos
u32 func_ov071_021b6e0c(u32 req) {
    if (req <= 1) {
        return 0x42;
    }
    return 0x22;
}

// pokebw2's MonolithTool_GetPanelColorCount
u32 func_ov071_021b6e18(u32 req) {
    return 3;
}

void func_ov071_021b6e1c(MonolithWork *wk, MonolithOamText *text, int sub, int x, int y, int width, int height,
                         u32 msgId, WordSet *wordSet) {
    BmpOamActorSetup setup = data_ov071_021ba82c;
    StrBuf *str;
    u32 vramType;

    text->bitmap = GFL_BitmapCreate(width, height, 0x20, HEAPID_MONOLITH);
    setup.bitmap = text->bitmap;
    setup.x = x - (width / 2) * 8;
    setup.y = y - (height / 2) * 8;
    setup.palette = wk->actorRes[sub].fontPalette;
    vramType = sub != 0 ? CLACT_VRAM_SUB : CLACT_VRAM_MAIN;
    setup.vramType = vramType;
    setup.surface = vramType;
    text->actor = func_020255a0(wk->bmpOam, &setup);
    str = GFL_MsgDataLoadStrbufNew(wk->msgMonolith, msgId);
    if (wordSet != NULL) {
        StrBuf *expanded = GFL_StrBufCreate(0x80, HEAPID_MONOLITH);

        GFL_WordSetFormatStrbuf(wordSet, expanded, str);
        func_0201c6fc(wk->printQueue, text->bitmap, 0, 0, expanded, wk->font);
        GFL_StrBufFree(expanded);
    } else {
        func_0201c6fc(wk->printQueue, text->bitmap, 0, 0, str, wk->font);
    }
    GFL_StrBufFree(str);
    text->printing = TRUE;
}


void func_ov071_021b6eec(MonolithOamText *text) {
    func_0202570c(text->actor);
    GFL_BitmapFree(text->bitmap);
    text->actor = (BmpOamActor *)0x0;
    text->bitmap = (GFLBitmap *)0x0;
}

BOOL func_ov071_021b6f04(MonolithWork *wk, MonolithOamText *text) {
    BOOL BVar1;

    if ((text->printing == 1) && (BVar1 = func_0201c6c4(wk->printQueue, text->bitmap), BVar1 == 0)) {
        func_020257d0(text->actor);
        text->printing = 0;
        return 1;
    }
    return 0;
}

void *func_ov071_021b6f2c(MonolithScreenParam *screen) {
    GameData *gameData;
    PlayerInfo *pPVar1;
    MonolithParam *pMVar2;

    pMVar2 = screen->param;
    gameData = GSYS_GetGameData(pMVar2->gsys);
    pPVar1 = func_ov010_021583ec(gameData, (u32)pMVar2->netId);
    return pPVar1;
}

void *func_ov071_021b6f40(MonolithScreenParam *screen) {
    GameData *gameData;
    PassPowerLevel *pPVar1;
    MonolithParam *pMVar2;

    pMVar2 = screen->param;
    gameData = GSYS_GetGameData(pMVar2->gsys);
    pPVar1 = func_ov010_021583cc(gameData, (u32)pMVar2->netId);
    return pPVar1;
}

ClActor *func_ov071_021b6f54(MonolithWork *wk, PassPowerLevel *levels, u32 index, const s32 *pos, u32 res) {
    ClActorSetup setup = { 0, 0, 4, 10, 2 };
    ClActor *actor;
    u16 surface = res == 0 ? CLACT_VRAM_MAIN : CLACT_VRAM_SUB;

    setup.x = pos[0];
    setup.y = pos[1];
    if (levels->unk06[index] == 1) {
        setup.sequence = 5;
    }
    actor = func_0204b294(wk->clactUnit, wk->actorRes[res].chars, wk->actorRes[res].palette,
                          wk->actorRes[res].cellAnims, &setup, surface, HEAPID_MONOLITH);
    if (levels->unk06[index] == 0) {
        func_0204b3dc(actor, FALSE);
    }
    func_0204b92c(actor, TRUE);
    return actor;
}


void func_ov071_021b6fd8(ClActor *actor) {
    func_0204b3b4(actor);
}

void func_ov071_021b6fe0(ClActor *actor, u8 *table, long index) {
    u32 sequence;
    u8 mode = table[index + 6];
    if (mode == 1) {
        sequence = 5;
    } else if (mode == 2) {
        sequence = 4;
    } else {
        func_0204b3dc(actor, FALSE);
        return;
    }
    func_0204b3dc(actor, TRUE);
    func_0204b8a0(actor, sequence);
}

ClActor *func_ov071_021b7014(MonolithWork *wk, const s32 *pos, u32 index) {
    ClActorSetup setup = { 0, 0, 11, 0, 2 };
    BOOL flag = FALSE;
    if (index != 0) {
        flag = TRUE;
    }
    setup.x = pos[0];
    setup.y = pos[1];
    return func_0204b294(wk->clactUnit, wk->actorRes[index].chars, wk->actorRes[index].palette,
                         wk->actorRes[index].cellAnims, &setup, flag, HEAPID_MONOLITH);
}



void func_ov071_021b7068(ClActor *actor) {
    func_0204b3b4(actor);
}

void func_ov071_021b7070(MonolithWork *wk, MonolithReturnButton *button) {
    ClActorSetup setup = { 240, 180, 6, 0, 2 };
    ClActor *actor = func_0204b294(wk->clactUnit, wk->actorRes[1].chars, wk->actorRes[1].palette,
                                   wk->actorRes[1].cellAnims, &setup, CLACT_VRAM_SUB, HEAPID_MONOLITH);

    func_0204b92c(actor, TRUE);
    button->actor = actor;
}




// pokebw2's MonolithTool_DeleteReturnButton
void func_ov071_021b70b8(MonolithReturnButton *button) {
    func_0204b3b4(button->actor);
}

// pokebw2's MonolithTool_UpdateReturnButton
void func_ov071_021b70c4(MonolithReturnButton *button) {
    if (button->blinking == TRUE) {
        if (button->timer % 5 == 0) {
            func_0204b6f4(button->actor, button->lit, TRUE);
            button->lit ^= 1;
            if (button->timer / 5 == 3) {
                button->blinking = FALSE;
            }
        }
        button->timer++;
    }
}

// pokebw2's MonolithTool_PressReturnButton
void func_ov071_021b7108(MonolithReturnButton *button) {
    button->blinking = TRUE;
    button->timer = 0;
    button->lit = TRUE;
}

// pokebw2's MonolithTool_IsReturnButtonBlinking
BOOL func_ov071_021b7114(MonolithReturnButton *button) {
    return button->blinking;
}

ClActor *func_ov071_021b7118(MonolithWork *wk, s16 x, s16 y, u16 sequence) {
    ClActorSetup setup = { x, y, sequence, 0, 1 };
    ClActor *actor = func_0204b294(wk->clactUnit, wk->actorRes[1].chars, wk->actorRes[1].palette,
                                   wk->actorRes[1].cellAnims, &setup, CLACT_VRAM_SUB, HEAPID_MONOLITH);

    func_0204b92c(actor, TRUE);
    return actor;
}


// pokebw2's MonolithTool_DeleteActor
void func_ov071_021b7168(ClActor *actor) {
    func_0204b3b4(actor);
}

void func_ov071_021b7170(void *ptr) {
}

// pokebw2's MonolithTool_CreateMenuRes
AppTaskMenuRes *func_ov071_021b7174(MonolithWork *wk, u32 bg, HeapID heapId) {
    return func_02027f60(bg, 14, wk->font, wk->printQueue, heapId);
}

// pokebw2's MonolithTool_FreeMenuRes
void func_ov071_021b718c(AppTaskMenuRes *res) {
    func_02027fd8(res);
}

// pokebw2's MonolithTool_CreateYesNoMenu
AppTaskMenu *func_ov071_021b7194(MonolithWork *wk, AppTaskMenuRes *res, HeapID heapId) {
    AppTaskMenuItem items[2];
    AppTaskMenuInit init;
    AppTaskMenu *menu;

    items[0].str = GFL_MsgDataLoadStrbufNew(wk->msgMonolith, 30);
    items[1].str = GFL_MsgDataLoadStrbufNew(wk->msgMonolith, 31);
    items[0].color = MONOLITH_MENU_TEXT_COLOR;
    items[1].color = MONOLITH_MENU_TEXT_COLOR;
    items[0].type = 0;
    items[1].type = 0;
    init.heapId = heapId;
    init.itemCount = 2;
    init.items = items;
    init.posType = APP_TASKMENU_POS_TOP_LEFT;
    init.x = 24;
    init.y = 12;
    init.width = 8;
    init.height = 3;
    menu = func_02027778(&init, res);
    GFL_StrBufFree(items[0].str);
    GFL_StrBufFree(items[1].str);
    return menu;
}

// pokebw2's MonolithTool_FreeYesNoMenu
void func_ov071_021b71fc(AppTaskMenu *menu) {
    func_02027858(menu);
}

// pokebw2's MonolithTool_UpdateYesNoMenu
BOOL func_ov071_021b7204(MonolithWork *wk, int unused, AppTaskMenu *menu, BOOL *yes) {
    func_02027974(menu);
    if (func_020279e8(menu) == TRUE) {
        *yes = func_02027a04(menu) == 0 ? TRUE : FALSE;
        return TRUE;
    }
    return FALSE;
}

