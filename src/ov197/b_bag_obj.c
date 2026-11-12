// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov286/b_bag_obj.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "battle/b_bag_obj.h"
#include "battle/b_app_tool.h"
#include "battle/b_bag_item.h"
#include "battle/b_bag_main.h"
#include "battle/btlv_effect.h"
#include "battle/btlv_finger_cursor.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/clact.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "nitro/gx.h"
#include "pml/item.h"
#include "system/palanm.h"
#include "system/shooter_item.h"

// The battle bag's OBJ resources and cell actors: the item icons, the Wonder Launcher's costs and energy gauges,
// overlay 285's cursor and overlay 168's finger cursor of the catching demo. The file name is our guess, after the
// ROM's b_bag_main.c and the b_bag_* files of Diamond and Pearl.

// How func_ov197_0220c3b4 creates each of the bag's actors, by index into the work's resources
typedef struct {
    u32 chars;
    u32 palette;
    u32 cellAnims;
} BBagActorSetup;

static void func_ov197_0220c06c(BBagWork *wk);
static void func_ov197_0220c0b4(BBagWork *wk);
static void func_ov197_0220c180(BBagWork *wk, u16 item, u32 chars, u32 row);
static void func_ov197_0220c220(BBagWork *wk, u16 item, u32 row);
static void func_ov197_0220c27c(BBagWork *wk);
static void func_ov197_0220c304(BBagWork *wk);
static ClActor *func_ov197_0220c3b4(BBagWork *wk, const BBagActorSetup *data);
static void func_ov197_0220c40c(BBagWork *wk);
static void func_ov197_0220c4d4(ClActor *actor, const ClActorPos *pos);
static void func_ov197_0220c538(BBagWork *wk);
static void func_ov197_0220c574(BBagWork *wk);
static void func_ov197_0220c618(BBagWork *wk);
static void func_ov197_0220c68c(BBagWork *wk);
static void func_ov197_0220c6c0(BBagWork *wk);
static void func_ov197_0220c6d0(BBagWork *wk);
static void func_ov197_0220c6e8(BBagWork *wk);
static void func_ov197_0220c6f8(BBagWork *wk, s16 x, s16 y);
void func_ov197_0220c714(BBagWork *wk);
static void func_ov197_0220c724(BBagWork *wk, u16 actor, u8 cost, u8 spent, const ClActorPos *pos);

// Declared in the order that MWCC's sort by size lays out in address order (rodata_order.py)

// The last used item on page 0
static const ClActorPos data_ov197_0220db7c = { 0x24, 0xb4 };
// The item's cost on page 2
static const ClActorPos data_ov197_0220db84 = { 0xa8, 0x28 };
// The item on page 2
static const ClActorPos data_ov197_0220db80 = { 0x28, 0x2c };
// Page 1: the items, then their costs, then the energy
static const ClActorPos data_ov197_0220db88[13] = {
#include "asset/b_bag_obj_data_ov197_0220db88.inc"
};

static const BBagActorSetup data_ov197_0220dbbc[BBAG_ACTOR_MAX] = {
#include "asset/b_bag_obj_data_ov197_0220dbbc.inc"
};

// pokebw2's BBagObj_Init
void func_ov197_0220c034(BBagWork *wk) {
    func_ov197_0220c06c(wk);
    func_ov197_0220c0b4(wk);
    func_ov197_0220c27c(wk);
    func_ov197_0220c304(wk);
    func_ov197_0220c40c(wk);
    func_ov197_0220c68c(wk);
    func_ov197_0220c6d0(wk);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
}

// pokebw2's BBagObj_ClearResources
static void func_ov197_0220c06c(BBagWork *wk) {
    u32 i;

    for (i = 0; i < NELEMS(wk->charRes); i++) {
        wk->charRes[i] = -1;
    }
    for (i = 0; i < NELEMS(wk->plttRes); i++) {
        wk->plttRes[i] = -1;
    }
    for (i = 0; i < NELEMS(wk->cellRes); i++) {
        wk->cellRes[i] = -1;
    }
}

// pokebw2's BBagObj_LoadItemIconRes
static void func_ov197_0220c0b4(BBagWork *wk) {
    ArcTool *arc;
    u16 item;
    u32 charType;
    u32 plttType;
    u16 i;

    if (wk->param->mode == 1) {
        item = func_0202f138(0);
        charType = ITEM_FILE_LIST_ICON_CHAR;
        plttType = ITEM_FILE_LIST_ICON_PLTT;
    } else {
        charType = ITEM_FILE_ICON_CHAR;
        item = 1;
        plttType = ITEM_FILE_ICON_PLTT;
    }
    arc = GFL_ArcSysCreateFileHandle(func_02020f08(), HEAPID_TAIL(wk->param->heapId));
    for (i = 0; i < 7; i++) {
        wk->charRes[i] =
            func_0204a6c8(arc, GetItemGraphicsDatID(item, charType), FALSE, CLACT_VRAM_SUB, wk->param->heapId);
        wk->plttRes[i] =
            func_0204abf0(arc, GetItemGraphicsDatID(item, plttType), CLACT_VRAM_SUB, i * 0x20, wk->param->heapId);
    }
    wk->cellRes[0] = func_0204af28(arc, func_02020f0c(), func_02020f10(), wk->param->heapId);
    GFL_ArcToolFree(arc);
}

// pokebw2's BBagObj_LoadItemIcon
static void func_ov197_0220c180(BBagWork *wk, u16 item, u32 chars, u32 row) {
    NNSG2dCharacterData *charData;
    BBagParam *param = wk->param;
    ArcTool *arc;
    void *file;
    u32 charType;
    u32 plttType;

    if (param->mode == 1) {
        charType = ITEM_FILE_LIST_ICON_CHAR;
        plttType = ITEM_FILE_LIST_ICON_PLTT;
    } else {
        charType = ITEM_FILE_ICON_CHAR;
        plttType = ITEM_FILE_ICON_PLTT;
    }
    arc = GFL_ArcSysCreateFileHandle(func_02020f08(), HEAPID_TAIL(param->heapId));
    file = GFL_G2DIOReadOBJNCGRArc(arc, GetItemGraphicsDatID(item, charType), FALSE, &charData, wk->param->heapId);
    func_0204a9d0(wk->charRes[chars], charData);
    GFL_HeapFree(file);
    func_020216b4(wk->paletteFade, arc, GetItemGraphicsDatID(item, plttType), wk->param->heapId,
                            PALFADE_BUFFER_SUB_OBJ, 0x20, row * 16);
    GFL_ArcToolFree(arc);
}

// pokebw2's BBagObj_LoadItemIconPalette
static void func_ov197_0220c220(BBagWork *wk, u16 item, u32 row) {
    BBagParam *param = wk->param;

    if (param->mode == 1) {
        GFL_BGSysLoadNCLRDefault(func_02020f08(), GetItemGraphicsDatID(item, ITEM_FILE_LIST_ICON_PLTT),
                                 PALTYPE_SUB_OBJ, row * 0x20, 0x20, param->heapId);
    } else {
        GFL_BGSysLoadNCLRDefault(func_02020f08(), GetItemGraphicsDatID(item, ITEM_FILE_ICON_PLTT),
                                 PALTYPE_SUB_OBJ, row * 0x20, 0x20, param->heapId);
    }
}

// pokebw2's BBagObj_LoadGaugeRes
static void func_ov197_0220c27c(BBagWork *wk) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(99, HEAPID_TAIL(wk->param->heapId));

    wk->charRes[7] = func_0204a6c8(arc, 13, TRUE, CLACT_VRAM_SUB, wk->param->heapId);
    wk->plttRes[7] = func_0204abf0(arc, 14, CLACT_VRAM_SUB, 0xe0, wk->param->heapId);
    wk->cellRes[1] = func_0204af28(arc, 15, 16, wk->param->heapId);
    func_020216b4(wk->paletteFade, arc, 14, wk->param->heapId, PALFADE_BUFFER_SUB_OBJ, 0x20, 0x70);
    GFL_ArcToolFree(arc);
}

// pokebw2's BBagObj_LoadCursorRes
static void func_ov197_0220c304(BBagWork *wk) {
    NNSG2dPaletteData *palette;
    ArcTool *arc = GFL_ArcSysCreateFileHandle(11, HEAPID_TAIL(wk->param->heapId));
    void *file;

    wk->charRes[8] = func_0204a6c8(arc, 0x77, FALSE, CLACT_VRAM_SUB, wk->param->heapId);
    wk->plttRes[8] = func_0204ac18(arc, 0x74, CLACT_VRAM_SUB, 0x100, 4, 1, wk->param->heapId);
    wk->cellRes[2] = func_0204af28(arc, 0x78, 0x79, wk->param->heapId);
    file = GFL_G2DIOReadNCLRArc(arc, 0x74, &palette, HEAPID_TAIL(wk->param->heapId));
    // The file's palette 4, faded as the sub screen's OBJ palette 8
    func_020215bc(wk->paletteFade, (GXRgb *)palette->pRawData + 4 * 16, PALFADE_BUFFER_SUB_OBJ, 0x80, 0x20);
    GFL_HeapFree(file);
    GFL_ArcToolFree(arc);
}

// pokebw2's BBagObj_CreateActor
static ClActor *func_ov197_0220c3b4(BBagWork *wk, const BBagActorSetup *data) {
    ClActorSetup setup;

    setup.x = 0;
    setup.y = 0;
    setup.sequence = 0;
    setup.priority = 1;
    setup.bgPriority = 1;
    return func_0204b294(wk->actorUnit, wk->charRes[data->chars], wk->plttRes[data->palette],
                         wk->cellRes[data->cellAnims], &setup, CLACT_SURFACE_SUB, wk->param->heapId);
}

// pokebw2's BBagObj_CreateActors
static void func_ov197_0220c40c(BBagWork *wk) {
    u32 i;

    wk->actorUnit = func_0204b100(BBAG_ACTOR_MAX + 4, 0, wk->param->heapId);
    for (i = 0; i < NELEMS(wk->actors); i++) {
        wk->actors[i] = func_ov197_0220c3b4(wk, &data_ov197_0220dbbc[i]);
    }
}

// pokebw2's BBagObj_Exit
void func_ov197_0220c448(BBagWork *wk) {
    u32 i;

    func_ov197_0220c6e8(wk);
    for (i = 0; i < NELEMS(wk->actors); i++) {
        func_0204b3b4(wk->actors[i]);
    }
    func_ov197_0220c6c0(wk);
    for (i = 0; i < NELEMS(wk->charRes); i++) {
        if (wk->charRes[i] != -1) {
            func_0204a8d4(wk->charRes[i]);
        }
    }
    for (i = 0; i < NELEMS(wk->plttRes); i++) {
        if (wk->plttRes[i] != -1) {
            func_0204ada4(wk->plttRes[i]);
        }
    }
    for (i = 0; i < NELEMS(wk->cellRes); i++) {
        if (wk->cellRes[i] != -1) {
            func_0204afd8(wk->cellRes[i]);
        }
    }
    func_0204b1cc(wk->actorUnit);
}

// pokebw2's BBagObj_ShowAt
static void func_ov197_0220c4d4(ClActor *actor, const ClActorPos *pos) {
    func_0204b3dc(actor, TRUE);
    func_0204b404(actor, pos, CLACT_SURFACE_SUB);
}

// pokebw2's BBagObj_SetPage
void func_ov197_0220c4ec(BBagWork *wk, u8 page) {
    u32 i;

    for (i = 0; i < NELEMS(wk->actors); i++) {
        func_0204b3dc(wk->actors[i], FALSE);
    }
    func_ov197_0220c714(wk);
    switch (page) {
    case 0:
        func_ov197_0220c538(wk);
        break;
    case 1:
        func_ov197_0220c574(wk);
        break;
    case 2:
        func_ov197_0220c618(wk);
        break;
    }
}

// pokebw2's BBagObj_ShowPockets
static void func_ov197_0220c538(BBagWork *wk) {
    if (wk->lastItem != 0) {
        func_ov197_0220c180(wk, wk->lastItem, 6, 6);
        func_ov197_0220c4d4(wk->actors[6], &data_ov197_0220db7c);
    }
    if (wk->param->mode == 2) {
        func_ov197_0220c6f8(wk, 192, 24);
    }
}

// pokebw2's BBagObj_ShowItemList
static void func_ov197_0220c574(BBagWork *wk) {
    u32 i;

    for (i = 0; i < 6; i++) {
        u16 item = func_ov197_0220bb98(wk, i);
        if (item != 0) {
            func_ov197_0220c180(wk, item, i, i);
            func_ov197_0220c220(wk, item, i);
            func_ov197_0220c4d4(wk->actors[BBAG_ACTOR_ITEM + i], &data_ov197_0220db88[i]);
            if (wk->param->mode == 1) {
                func_ov197_0220c724(wk, BBAG_ACTOR_COST + i * 8, func_ov197_0220bbd8(item), 0,
                                 &data_ov197_0220db88[i + 6]);
            }
        }
    }
    if (wk->param->mode == 1) {
        func_ov197_0220c724(wk, BBAG_ACTOR_ENERGY, wk->param->shooterEnergy, wk->param->shooterSpent,
                         &data_ov197_0220db88[i + 6]);
    }
    if (wk->param->mode == 2) {
        func_ov197_0220c6f8(wk, 64, 16);
    }
}

// pokebw2's BBagObj_ShowItem
static void func_ov197_0220c618(BBagWork *wk) {
    u16 item = func_ov197_0220bb98(wk, wk->param->rows[wk->pocket]);

    func_ov197_0220c180(wk, item, 6, 6);
    func_ov197_0220c4d4(wk->actors[6], &data_ov197_0220db80);
    if (wk->param->mode == 1) {
        func_ov197_0220c724(wk, BBAG_ACTOR_COST, func_ov197_0220bbd8(item), 0, &data_ov197_0220db84);
    }
    if (wk->param->mode == 2) {
        func_ov197_0220c6f8(wk, 104, 152);
    }
}

// pokebw2's BBagObj_InitCursor
static void func_ov197_0220c68c(BBagWork *wk) {
    func_ov196_0220af6c(wk->cursor, wk->actorUnit, wk->charRes[8], wk->plttRes[8], wk->cellRes[2]);
    func_ov196_0220afdc(wk->cursor, FALSE);
}

// pokebw2's BBagObj_ExitCursor
static void func_ov197_0220c6c0(BBagWork *wk) {
    func_ov196_0220afc4(wk->cursor);
}

// pokebw2's BBagObj_CreateFingerCursor
static void func_ov197_0220c6d0(BBagWork *wk) {
    wk->fingerCursor = BtlvFingerCursor_Create(wk->paletteFade, 9, wk->param->heapId);
}

// pokebw2's BBagObj_DeleteFingerCursor
static void func_ov197_0220c6e8(BBagWork *wk) {
    func_ov094_02209b30(wk->fingerCursor);
}

// pokebw2's BBagObj_ShowFingerCursor
static void func_ov197_0220c6f8(BBagWork *wk, s16 x, s16 y) {
    func_ov094_02209b60(wk->fingerCursor, x, y, 2, 6, 20);
}

void func_ov197_0220c714(BBagWork *wk) {
    func_ov094_02209c18(wk->fingerCursor);
}


// pokebw2's BBagObj_ShowCost
static void func_ov197_0220c724(BBagWork *wk, u16 actor, u8 cost, u8 spent, const ClActorPos *pos) {
    ClActorPos p;
    s8 c;
    s8 s;
    u16 i;

    func_ov197_0220c4d4(wk->actors[actor], pos);
    func_0204b878(wk->actors[actor], cost);
    actor++;
    c = cost;
    s = spent;
    p = *pos;
    p.x += 12;
    for (i = 0; i < 7; i++) {
        if (c <= 0) {
            if (s <= 0) {
                func_0204b878(wk->actors[actor], 15);
            } else if (s == 1) {
                func_0204b878(wk->actors[actor], 19);
                s -= 1;
            } else {
                func_0204b878(wk->actors[actor], 20);
                s -= 2;
            }
        } else if (c == 1) {
            if (s == 0) {
                func_0204b878(wk->actors[actor], 16);
            } else if (s > 0) {
                func_0204b878(wk->actors[actor], 18);
                s -= 1;
            }
            c -= 1;
        } else {
            func_0204b878(wk->actors[actor], 17);
            c -= 2;
        }
        func_ov197_0220c4d4(wk->actors[actor], &p);
        p.x += 8;
        actor++;
    }
}
