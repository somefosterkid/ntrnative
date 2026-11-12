// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov300/zknsearch_obj.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/ui/frame_list.h"
#include "app/zknsearch_main.h"
#include "app/zknsearch_obj.h"
#include "app/zukan_common.h"
#include "constants/arc.h"
#include "gfl/arc.h"
#include "gfl/clact.h"
#include "gfl/gx_layers.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/gx.h"
#include "nitro/math.h"
#include "system/app_menu_common.h"
#include "system/game_data.h"
#include "app/zukansearch.h"

// The actors of the Pokédex's search screen: the touch bar's buttons, the scroll bar, the marks of the chosen items
// and the shapes. The ROM doesn't name this file; zknsearch_obj.c is a guess after zknsearch_main.c

// The resources
enum {
    CHAR_RES_TOUCHBAR,
    CHAR_RES_LIST,
    CHAR_RES_LIST_SUB,
    CHAR_RES_SHAPE,
    CHAR_RES_SHAPE_SUB,
    CHAR_RES_LOADING,
};

enum {
    CELL_RES_TOUCHBAR,
    CELL_RES_LIST,
    CELL_RES_SHAPE,
    CELL_RES_LOADING,
};

// The window that an actor's shape cuts out of the blending, which lib/nitro's GX_OAM_MODE_* doesn't have yet
#define OAM_MODE_OBJWND 2

// Where the marks are: their x, the rows' heights and the height of the top screen's list
#define MARK_X 140
#define ROW_HEIGHT 24
#define SHAPE_X 204
#define SHAPE_ROW_HEIGHT 40
#define TOP_LIST_HEIGHT 192

static void func_ov210_021bee94(void);
void func_ov210_021beec4(ZknSearchWork *wk);
static void func_ov210_021bf080(ZknSearchWork *wk);
static void func_ov210_021bf0dc(ZknSearchWork *wk);
static void func_ov210_021bf358(ZknSearchWork *wk, u32 id);
static void func_ov210_021bf370(ZknSearchWork *wk);

// pokebw2's sZknSearchActorTemplates
static const ZukanActorTemplate data_ov210_021c09ac[] = {
#include "asset/zknsearch_obj_data_ov210_021c09ac.inc"
};

// pokebw2's ZknSearchObj_Init
void func_ov210_021bed2c(ZknSearchWork *wk) {
    func_ov210_021bee94();
    func_ov210_021beec4(wk);
    func_ov210_021bf0dc(wk);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
}

// pokebw2's ZknSearchObj_Exit
void func_ov210_021bed54(ZknSearchWork *wk) {
    func_ov210_021bf370(wk);
    func_ov210_021bf080(wk);
    func_0204a5ac();
}

// pokebw2's ZknSearchObj_Update
void func_ov210_021bed68(ZknSearchWork *wk) {
    u32 i;

    for (i = 0; i < ZKNSEARCH_OBJ_COUNT; i++) {
        if (wk->actors[i] != NULL && func_0204b944(wk->actors[i]) != TRUE) {
            func_0204b8d4(wk->actors[i], FX32_ONE);
        }
    }
    func_0204a600();
}

// pokebw2's ZknSearchObj_SetAnmSeq
void func_ov210_021bed98(ZknSearchWork *wk, u32 id, u32 anm) {
    func_0204b8c4(wk->actors[id], 0);
    func_0204b878(wk->actors[id], anm);
}

// pokebw2's ZknSearchObj_SetAnm
void func_ov210_021bedb8(ZknSearchWork *wk, u32 id, u32 anm) {
    func_ov210_021bed98(wk, id, anm);
    func_0204b92c(wk->actors[id], TRUE);
}

// pokebw2's ZknSearchObj_GetAnm
u32 func_ov210_021bedd0(ZknSearchWork *wk, u32 id) {
    return func_0204b898(wk->actors[id]);
}

// pokebw2's ZknSearchObj_IsAnmPlaying
BOOL func_ov210_021bede0(ZknSearchWork *wk, u32 id) {
    return func_0204b98c(wk->actors[id]);
}

// pokebw2's ZknSearchObj_SetVisible
void func_ov210_021bedf0(ZknSearchWork *wk, u32 id, BOOL visible) {
    if (wk->actors[id] != NULL) {
        func_0204b3dc(wk->actors[id], visible);
    }
}

// pokebw2's ZknSearchObj_IsVisible
BOOL func_ov210_021bee04(ZknSearchWork *wk, u32 id) {
    return func_0204b3f4(wk->actors[id]);
}

// pokebw2's ZknSearchObj_SetBlend
void func_ov210_021bee14(ZknSearchWork *wk, u32 id, BOOL blend) {
    if (blend == TRUE) {
        func_0204b688(wk->actors[id], GX_OAM_MODE_XLU);
    } else {
        func_0204b688(wk->actors[id], GX_OAM_MODE_NORMAL);
    }
}

// pokebw2's ZknSearchObj_SetPos
void func_ov210_021bee38(ZknSearchWork *wk, u32 id, s16 x, s16 y, u16 surface) {
    ClActorPos pos;

    pos.x = x;
    pos.y = y;
    func_0204b404(wk->actors[id], &pos, surface);
}

// pokebw2's ZknSearchObj_GetPos
void func_ov210_021bee58(ZknSearchWork *wk, u32 id, s16 *x, s16 *y, u16 surface) {
    ClActorPos pos;

    func_0204b454(wk->actors[id], &pos, surface);
    *x = pos.x;
    *y = pos.y;
}

// pokebw2's ZknSearchObj_SetBGPriority
void func_ov210_021bee80(ZknSearchWork *wk, u32 id, u32 priority) {
    func_0204b84c(wk->actors[id], (u8)priority);
}

// pokebw2's ZknSearchObj_InitSystem
static void func_ov210_021bee94(void) {
    ClActSysSetup setup = { 0,
                            0,
                            0,
                            512,
                            4,
                            124,
                            4,
                            124,
                            0,
                            ZKNSEARCH_CHAR_RES_COUNT,
                            ZKNSEARCH_PALETTE_RES_COUNT,
                            ZKNSEARCH_CELL_RES_COUNT,
                            0,
                            16,
                            16 };

    ClActSys_Create(&setup, func_ov210_021bc3e8(), HEAPID_ZUKAN_SEARCH);
}

// pokebw2's ZknSearchObj_LoadResources
void func_ov210_021beec4(ZknSearchWork *wk) {
    ArcTool *arc;
    u32 i;

    for (i = 0; i < ZKNSEARCH_CHAR_RES_COUNT; i++) {
        wk->charRes[i] = -1;
    }
    for (i = 0; i < ZKNSEARCH_PALETTE_RES_COUNT; i++) {
        wk->paletteRes[i] = -1;
    }
    for (i = 0; i < ZKNSEARCH_CELL_RES_COUNT; i++) {
        wk->cellRes[i] = -1;
    }

    arc = GFL_ArcSysCreateFileHandle(ARCID_ZUKAN_GRA, HEAPID_ZUKAN_SEARCH | HEAPID_TAIL_BIT);
    wk->charRes[CHAR_RES_LIST] = func_0204a6c8(arc, 78, FALSE, CLACT_VRAM_MAIN, HEAPID_ZUKAN_SEARCH);
    wk->paletteRes[CHAR_RES_LIST] = func_0204abf0(arc, 75, CLACT_VRAM_MAIN, 0 * 0x20, HEAPID_ZUKAN_SEARCH);
    wk->cellRes[CELL_RES_LIST] = func_0204af28(arc, 84, 100, HEAPID_ZUKAN_SEARCH);
    wk->charRes[CHAR_RES_LIST_SUB] = func_0204a6c8(arc, 78, FALSE, CLACT_VRAM_SUB, HEAPID_ZUKAN_SEARCH);
    wk->paletteRes[CHAR_RES_LIST_SUB] = func_0204abf0(arc, 75, CLACT_VRAM_SUB, 0 * 0x20, HEAPID_ZUKAN_SEARCH);
    wk->charRes[CHAR_RES_LOADING] = func_0204a6c8(arc, 77, FALSE, CLACT_VRAM_MAIN, HEAPID_ZUKAN_SEARCH);
    wk->paletteRes[CHAR_RES_LOADING] = func_0204abf0(arc, 72, CLACT_VRAM_MAIN, 3 * 0x20, HEAPID_ZUKAN_SEARCH);
    wk->cellRes[CELL_RES_LOADING] = func_0204af28(arc, 83, 99, HEAPID_ZUKAN_SEARCH);
    wk->charRes[CHAR_RES_SHAPE] = func_0204a6c8(arc, 82, FALSE, CLACT_VRAM_MAIN, HEAPID_ZUKAN_SEARCH);
    wk->paletteRes[CHAR_RES_SHAPE] = func_0204abf0(arc, 76, CLACT_VRAM_MAIN, 4 * 0x20, HEAPID_ZUKAN_SEARCH);
    wk->cellRes[CELL_RES_SHAPE] = func_0204af28(arc, 85, 101, HEAPID_ZUKAN_SEARCH);
    wk->charRes[CHAR_RES_SHAPE_SUB] = func_0204a6c8(arc, 82, FALSE, CLACT_VRAM_SUB, HEAPID_ZUKAN_SEARCH);
    wk->paletteRes[CHAR_RES_SHAPE_SUB] = func_0204abf0(arc, 76, CLACT_VRAM_SUB, 3 * 0x20, HEAPID_ZUKAN_SEARCH);
    GFL_ArcToolFree(arc);

    arc = GFL_ArcSysCreateFileHandle(getUINarcIdx(), HEAPID_ZUKAN_SEARCH | HEAPID_TAIL_BIT);
    wk->charRes[CHAR_RES_TOUCHBAR] = func_0204a6c8(arc, func_02027618(), FALSE, CLACT_VRAM_MAIN, HEAPID_ZUKAN_SEARCH);
    wk->paletteRes[CHAR_RES_TOUCHBAR] =
        func_0204abf0(arc, func_02027614(), CLACT_VRAM_MAIN, 5 * 0x20, HEAPID_ZUKAN_SEARCH);
    wk->cellRes[CELL_RES_TOUCHBAR] = func_0204af28(arc, func_0202761c(2), func_02027620(2), HEAPID_ZUKAN_SEARCH);
    GFL_ArcToolFree(arc);
}

// pokebw2's ZknSearchObj_FreeResources
static void func_ov210_021bf080(ZknSearchWork *wk) {
    u32 i;

    for (i = 0; i < ZKNSEARCH_CHAR_RES_COUNT; i++) {
        if (wk->charRes[i] != -1) {
            func_0204a8d4(wk->charRes[i]);
        }
    }
    for (i = 0; i < ZKNSEARCH_PALETTE_RES_COUNT; i++) {
        if (wk->paletteRes[i] != -1) {
            func_0204ada4(wk->paletteRes[i]);
        }
    }
    for (i = 0; i < ZKNSEARCH_CELL_RES_COUNT; i++) {
        if (wk->cellRes[i] != -1) {
            func_0204afd8(wk->cellRes[i]);
        }
    }
}

// pokebw2's ZknSearchObj_CreateActors
static void func_ov210_021bf0dc(ZknSearchWork *wk) {
    u32 i;

    wk->actorUnit = func_0204b100(ZKNSEARCH_OBJ_COUNT, 0, HEAPID_ZUKAN_SEARCH);
    for (i = 0; i < ZKNSEARCH_OBJ_COUNT; i++) {
        wk->actors[i] = NULL;
    }
    for (i = ZKNSEARCH_OBJ_SCROLL_BAR; i <= ZKNSEARCH_OBJ_MARK_WINDOW + 1; i++) {
        ZukanActorTemplate template = data_ov210_021c09ac[i];

        template.chars = wk->charRes[template.chars];
        template.palette = wk->paletteRes[template.palette];
        template.cells = wk->cellRes[template.cells];
        wk->actors[i] = func_ov212_021c940c(wk->actorUnit, &template, HEAPID_ZUKAN_SEARCH);
    }
    if (GameData_IsShortcutRegistered(wk->param->gameData, SHORTCUT_POKEDEX_SEARCH) == TRUE) {
        func_ov210_021bedb8(wk, ZKNSEARCH_OBJ_SHORTCUT, 7);
    }
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_PAGE_DOWN, FALSE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_PAGE_UP, FALSE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_SCROLL_BAR, FALSE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_SCROLL_RAIL, FALSE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_SHAPE, FALSE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_LOADING_BAR, FALSE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_MARK, FALSE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_MARK + 1, FALSE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_MARK_SUB, FALSE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_MARK_SUB + 1, FALSE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_MARK_WINDOW, FALSE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_MARK_WINDOW + 1, FALSE);
    func_0204b688(wk->actors[ZKNSEARCH_OBJ_MARK_WINDOW], OAM_MODE_OBJWND);
    func_0204b688(wk->actors[ZKNSEARCH_OBJ_MARK_WINDOW + 1], OAM_MODE_OBJWND);
    func_ov210_021bee14(wk, ZKNSEARCH_OBJ_MARK_SUB, TRUE);
    func_ov210_021bee14(wk, ZKNSEARCH_OBJ_MARK_SUB + 1, TRUE);

    {
        ZukanActorTemplate template;
        ZukanActorTemplate shape = {
#include "asset/zknsearch_obj_shape_setup.inc"
            CHAR_RES_SHAPE, CHAR_RES_SHAPE, CELL_RES_SHAPE, 0, CLACT_SURFACE_MAIN
        };
        ZukanActorTemplate shapeSub = {
#include "asset/zknsearch_obj_shapeSub_setup.inc"
            CHAR_RES_SHAPE_SUB, CHAR_RES_SHAPE_SUB, CELL_RES_SHAPE, 0, CLACT_SURFACE_SUB
        };

        for (i = 0; i < ZKNSEARCH_SHAPE_COUNT; i++) {
            template = shape;
            template.chars = wk->charRes[template.chars];
            template.palette = wk->paletteRes[template.palette];
            template.cells = wk->cellRes[template.cells];
            template.setup.sequence = i;
            wk->actors[ZKNSEARCH_OBJ_SHAPE_ICON + i] =
                func_ov212_021c940c(wk->actorUnit, &template, HEAPID_ZUKAN_SEARCH);
            func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_SHAPE_ICON + i, FALSE);

            template = shapeSub;
            template.chars = wk->charRes[template.chars];
            template.palette = wk->paletteRes[template.palette];
            template.cells = wk->cellRes[template.cells];
            template.setup.sequence = i;
            wk->actors[ZKNSEARCH_OBJ_SHAPE_ICON_SUB + i] =
                func_ov212_021c940c(wk->actorUnit, &template, HEAPID_ZUKAN_SEARCH);
            func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_SHAPE_ICON_SUB + i, FALSE);
            func_ov210_021bee14(wk, ZKNSEARCH_OBJ_SHAPE_ICON_SUB + i, TRUE);

            wk->actors[ZKNSEARCH_OBJ_SHAPE_ICON_WINDOW + i] =
                func_ov212_021c940c(wk->actorUnit, &template, HEAPID_ZUKAN_SEARCH);
            func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_SHAPE_ICON_WINDOW + i, FALSE);
            func_0204b688(wk->actors[ZKNSEARCH_OBJ_SHAPE_ICON_WINDOW + i], OAM_MODE_OBJWND);
        }
    }
}

// pokebw2's ZknSearchObj_DeleteActor
static void func_ov210_021bf358(ZknSearchWork *wk, u32 id) {
    if (wk->actors[id] != NULL) {
        func_0204b3b4(wk->actors[id]);
        wk->actors[id] = NULL;
    }
}

// pokebw2's ZknSearchObj_DeleteActors
static void func_ov210_021bf370(ZknSearchWork *wk) {
    u32 i;

    for (i = 0; i < ZKNSEARCH_OBJ_COUNT; i++) {
        func_ov210_021bf358(wk, i);
    }
    func_0204b1cc(wk->actorUnit);
}

// pokebw2's ZknSearchObj_HideList
void func_ov210_021bf38c(ZknSearchWork *wk) {
    u32 i;

    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_SCROLL_BAR, FALSE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_SCROLL_RAIL, FALSE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_MARK, FALSE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_MARK + 1, FALSE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_MARK_SUB, FALSE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_MARK_SUB + 1, FALSE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_MARK_WINDOW, FALSE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_MARK_WINDOW + 1, FALSE);
    for (i = 0; i < ZKNSEARCH_SHAPE_COUNT; i++) {
        func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_SHAPE_ICON + i, FALSE);
        func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_SHAPE_ICON_SUB + i, FALSE);
        func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_SHAPE_ICON_WINDOW + i, FALSE);
    }
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_SHAPE, FALSE);
}

// pokebw2's ZknSearchObj_SetMain
void func_ov210_021bf418(ZknSearchWork *wk) {
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_PAGE_UP, FALSE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_PAGE_DOWN, FALSE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_RETURN, TRUE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_EXIT, TRUE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_SHORTCUT, TRUE);
    if (wk->param->criteria->shape != ZKNSEARCH_NONE) {
        func_ov210_021bed98(wk, ZKNSEARCH_OBJ_SHAPE, wk->param->criteria->shape);
        func_ov210_021bee38(wk, ZKNSEARCH_OBJ_SHAPE, 192, 116, CLACT_SURFACE_MAIN);
        func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_SHAPE, TRUE);
    }
}

// pokebw2's ZknSearchObj_SetOrder
void func_ov210_021bf480(ZknSearchWork *wk) {
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_EXIT, FALSE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_SHORTCUT, FALSE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_RETURN, TRUE);
}

// pokebw2's ZknSearchObj_SetInitial
void func_ov210_021bf4a4(ZknSearchWork *wk) {
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_EXIT, FALSE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_SHORTCUT, FALSE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_RETURN, TRUE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_PAGE_DOWN, TRUE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_PAGE_UP, TRUE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_SCROLL_BAR, TRUE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_SCROLL_RAIL, TRUE);
    func_ov210_021bee38(wk, ZKNSEARCH_OBJ_SCROLL_BAR, 252, 12, CLACT_SURFACE_MAIN);
}

// pokebw2's ZknSearchObj_SetType
void func_ov210_021bf500(ZknSearchWork *wk) {
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_EXIT, FALSE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_SHORTCUT, FALSE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_RETURN, TRUE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_PAGE_DOWN, TRUE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_PAGE_UP, TRUE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_SCROLL_BAR, TRUE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_SCROLL_RAIL, TRUE);
    func_ov210_021bee38(wk, ZKNSEARCH_OBJ_SCROLL_BAR, 252, 12, CLACT_SURFACE_MAIN);
}

// pokebw2's ZknSearchObj_SetColor
void func_ov210_021bf55c(ZknSearchWork *wk) {
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_EXIT, FALSE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_SHORTCUT, FALSE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_RETURN, TRUE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_PAGE_DOWN, TRUE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_PAGE_UP, TRUE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_SCROLL_BAR, TRUE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_SCROLL_RAIL, TRUE);
    func_ov210_021bee38(wk, ZKNSEARCH_OBJ_SCROLL_BAR, 252, 12, CLACT_SURFACE_MAIN);
}

// pokebw2's ZknSearchObj_SetShape
void func_ov210_021bf5b8(ZknSearchWork *wk) {
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_EXIT, FALSE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_SHORTCUT, FALSE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_RETURN, TRUE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_PAGE_DOWN, TRUE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_PAGE_UP, TRUE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_SCROLL_BAR, TRUE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_SCROLL_RAIL, TRUE);
    func_ov210_021bee38(wk, ZKNSEARCH_OBJ_SCROLL_BAR, 252, 12, CLACT_SURFACE_MAIN);
    func_ov210_021bf618(wk);
}

// pokebw2's ZknSearchObj_PutShape
void func_ov210_021bf618(ZknSearchWork *wk) {
    if (wk->param->criteria->shape != ZKNSEARCH_NONE) {
        func_ov210_021bed98(wk, ZKNSEARCH_OBJ_SHAPE, wk->param->criteria->shape);
        func_ov210_021bee38(wk, ZKNSEARCH_OBJ_SHAPE, 64, 104, CLACT_SURFACE_MAIN);
        func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_SHAPE, TRUE);
    } else {
        func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_SHAPE, FALSE);
    }
}

// pokebw2's ZknSearchObj_PutShapeIcons
void func_ov210_021bf658(ZknSearchWork *wk) {
    s16 scroll = func_ov067_021b93b8(wk->frameList);
    s16 i;

    for (i = 0; i < ZKNSEARCH_SHAPE_COUNT; i++) {
        func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_SHAPE_ICON + i, FALSE);
        func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_SHAPE_ICON_SUB + i, FALSE);
        func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_SHAPE_ICON_WINDOW + i, FALSE);
    }
    for (i = 0; i < 5; i++) {
        func_ov210_021bfb8c(wk, scroll + i, i * SHAPE_ROW_HEIGHT, TRUE);
        if (scroll - (i + 1) >= 0) {
            func_ov210_021bfb8c(wk, scroll - (i + 1), TOP_LIST_HEIGHT - (i + 1) * SHAPE_ROW_HEIGHT, FALSE);
        }
    }
}

// pokebw2's ZknSearchObj_UpdateScrollBar
void func_ov210_021bf6e0(ZknSearchWork *wk) {
    s16 x, y;
    u32 pos;

    func_ov210_021bee58(wk, ZKNSEARCH_OBJ_SCROLL_BAR, &x, &y, CLACT_SURFACE_MAIN);
    pos = func_ov067_021b8ae4(wk->frameList, y);
    if (pos < 12) {
        pos = 12;
    } else if (pos > 156) {
        pos = 156;
    }
    func_ov210_021bee38(wk, ZKNSEARCH_OBJ_SCROLL_BAR, 252, pos, CLACT_SURFACE_MAIN);
}

// pokebw2's ZknSearchObj_SetPageButtons
void func_ov210_021bf728(ZknSearchWork *wk, BOOL keep) {
    s16 cursor = func_ov067_021b93b0(wk->frameList);

    if (func_ov210_021bedd0(wk, ZKNSEARCH_OBJ_PAGE_UP) != 12 || keep == FALSE) {
        if (func_ov067_021b93b8(wk->frameList) == 0 && cursor == 0) {
            func_ov210_021bedb8(wk, ZKNSEARCH_OBJ_PAGE_UP, 18);
        } else {
            func_ov210_021bedb8(wk, ZKNSEARCH_OBJ_PAGE_UP, 4);
        }
    }
    if (func_ov210_021bedd0(wk, ZKNSEARCH_OBJ_PAGE_DOWN) != 13 || keep == FALSE) {
        if (func_ov067_021b93c0(wk->frameList) == FALSE && cursor == wk->listRows - 1) {
            func_ov210_021bedb8(wk, ZKNSEARCH_OBJ_PAGE_DOWN, 19);
        } else {
            func_ov210_021bedb8(wk, ZKNSEARCH_OBJ_PAGE_DOWN, 5);
        }
    }
}

// pokebw2's ZknSearchObj_PutMark
void func_ov210_021bf7b0(ZknSearchWork *wk, int index, s16 y, BOOL touchScreen) {
    if (touchScreen == TRUE) {
        func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_MARK + index, TRUE);
        func_ov210_021bee38(wk, ZKNSEARCH_OBJ_MARK + index, MARK_X, y + ROW_HEIGHT / 2, CLACT_SURFACE_MAIN);
    } else {
        func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_MARK_SUB + index, TRUE);
        func_ov210_021bee38(wk, ZKNSEARCH_OBJ_MARK_SUB + index, MARK_X, y + ROW_HEIGHT / 2, CLACT_SURFACE_SUB);
        func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_MARK_WINDOW + index, TRUE);
        func_ov210_021bee38(wk, ZKNSEARCH_OBJ_MARK_WINDOW + index, MARK_X, y + ROW_HEIGHT / 2, CLACT_SURFACE_SUB);
    }
}

// pokebw2's ZknSearchObj_HideMarks
void func_ov210_021bf81c(ZknSearchWork *wk) {
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_MARK, FALSE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_MARK + 1, FALSE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_MARK_SUB, FALSE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_MARK_SUB + 1, FALSE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_MARK_WINDOW, FALSE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_MARK_WINDOW + 1, FALSE);
}

// pokebw2's ZknSearchObj_PutRowMark
void func_ov210_021bf85c(ZknSearchWork *wk, u16 row, BOOL visible) {
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_MARK, visible);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_MARK_SUB, FALSE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_MARK_WINDOW, FALSE);
    if (visible == TRUE) {
        func_ov210_021bee38(wk, ZKNSEARCH_OBJ_MARK, MARK_X, row * ROW_HEIGHT + ROW_HEIGHT / 2, CLACT_SURFACE_MAIN);
    }
}

// The marks of both types: on the top screen's rows above the list's scroll, or on the touch screen's
// pokebw2's ZknSearchObj_PutTypeMarks
void func_ov210_021bf89c(ZknSearchWork *wk, u8 type1, u8 type2) {
    u8 scroll = func_ov067_021b93b8(wk->frameList);
    u8 diff;

    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_MARK, FALSE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_MARK + 1, FALSE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_MARK_SUB, FALSE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_MARK_SUB + 1, FALSE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_MARK_WINDOW, FALSE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_MARK_WINDOW + 1, FALSE);
    if (type1 != ZKNSEARCH_NONE) {
        type1 = func_ov210_021bcef0(wk, type1);
        diff = MATH_ABS(type1 - scroll);
        if (type1 < scroll) {
            if (diff <= 8) {
                func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_MARK_SUB, TRUE);
                func_ov210_021bee38(wk, ZKNSEARCH_OBJ_MARK_SUB, MARK_X, (8 - diff) * ROW_HEIGHT + ROW_HEIGHT / 2,
                                    CLACT_SURFACE_SUB);
                func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_MARK_WINDOW, TRUE);
                func_ov210_021bee38(wk, ZKNSEARCH_OBJ_MARK_WINDOW, MARK_X, (8 - diff) * ROW_HEIGHT + ROW_HEIGHT / 2,
                                    CLACT_SURFACE_SUB);
            }
        } else if (diff <= 7) {
            func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_MARK, TRUE);
            func_ov210_021bee38(wk, ZKNSEARCH_OBJ_MARK, MARK_X, diff * ROW_HEIGHT + ROW_HEIGHT / 2, CLACT_SURFACE_MAIN);
        }
    }
    if (type2 != ZKNSEARCH_NONE) {
        type2 = func_ov210_021bcef0(wk, type2);
        diff = MATH_ABS(type2 - scroll);
        if (type2 < scroll) {
            if (diff <= 8) {
                func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_MARK_SUB + 1, TRUE);
                func_ov210_021bee38(wk, ZKNSEARCH_OBJ_MARK_SUB + 1, MARK_X, (8 - diff) * ROW_HEIGHT + ROW_HEIGHT / 2,
                                    CLACT_SURFACE_SUB);
                func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_MARK_WINDOW + 1, TRUE);
                func_ov210_021bee38(wk, ZKNSEARCH_OBJ_MARK_WINDOW + 1, MARK_X, (8 - diff) * ROW_HEIGHT + ROW_HEIGHT / 2,
                                    CLACT_SURFACE_SUB);
            }
        } else if (diff <= 7) {
            func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_MARK + 1, TRUE);
            func_ov210_021bee38(wk, ZKNSEARCH_OBJ_MARK + 1, MARK_X, diff * ROW_HEIGHT + ROW_HEIGHT / 2,
                                CLACT_SURFACE_MAIN);
        }
    }
}

// pokebw2's ZknSearchObj_PutShapeMark
void func_ov210_021bfa14(ZknSearchWork *wk, s16 y, BOOL touchScreen) {
    if (touchScreen == TRUE) {
        func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_MARK, TRUE);
        func_ov210_021bee38(wk, ZKNSEARCH_OBJ_MARK, MARK_X, y + SHAPE_ROW_HEIGHT / 2, CLACT_SURFACE_MAIN);
    } else {
        func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_MARK_SUB, TRUE);
        func_ov210_021bee38(wk, ZKNSEARCH_OBJ_MARK_SUB, MARK_X, y + SHAPE_ROW_HEIGHT / 2, CLACT_SURFACE_SUB);
        func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_MARK_WINDOW, TRUE);
        func_ov210_021bee38(wk, ZKNSEARCH_OBJ_MARK_WINDOW, MARK_X, y + SHAPE_ROW_HEIGHT / 2, CLACT_SURFACE_SUB);
    }
}

// pokebw2's ZknSearchObj_PutShapeRowMark
void func_ov210_021bfa7c(ZknSearchWork *wk, u16 row, BOOL visible) {
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_MARK, visible);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_MARK_SUB, FALSE);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_MARK_WINDOW, FALSE);
    if (visible == TRUE) {
        func_ov210_021bee38(wk, ZKNSEARCH_OBJ_MARK, MARK_X, row * SHAPE_ROW_HEIGHT + SHAPE_ROW_HEIGHT / 2,
                            CLACT_SURFACE_MAIN);
    }
}

// pokebw2's ZknSearchObj_PutItemMark
void func_ov210_021bfabc(ZknSearchWork *wk, u32 index, u16 item) {
    u32 scroll = func_ov067_021b93b8(wk->frameList);
    s16 top = scroll - 7;
    s16 bottom = scroll + 7;

    if (top < 0) {
        top = 0;
    }
    if (item >= top && item < scroll) {
        func_ov210_021bf7b0(wk, index, TOP_LIST_HEIGHT - (scroll - item) * ROW_HEIGHT, FALSE);
    } else if (item >= scroll && item < bottom) {
        func_ov210_021bf7b0(wk, index, (item - scroll) * ROW_HEIGHT, TRUE);
    }
}

// pokebw2's ZknSearchObj_PutShapeItemMark
void func_ov210_021bfb24(ZknSearchWork *wk) {
    u32 scroll = func_ov067_021b93b8(wk->frameList);
    u8 shape = wk->param->criteria->shape;
    s16 top = shape - 4;
    s16 bottom = shape + 4;

    if (top < 0) {
        top = 0;
    }
    if (shape >= top && shape < scroll) {
        func_ov210_021bfa14(wk, TOP_LIST_HEIGHT - (scroll - shape) * SHAPE_ROW_HEIGHT, FALSE);
    } else if (shape >= scroll && shape < bottom) {
        func_ov210_021bfa14(wk, (shape - scroll) * SHAPE_ROW_HEIGHT, TRUE);
    }
}

// pokebw2's ZknSearchObj_PutShapeIcon
void func_ov210_021bfb8c(ZknSearchWork *wk, u16 shape, s16 y, BOOL touchScreen) {
    if (touchScreen == TRUE) {
        func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_SHAPE_ICON + shape, TRUE);
        func_ov210_021bee38(wk, ZKNSEARCH_OBJ_SHAPE_ICON + shape, SHAPE_X, y + SHAPE_ROW_HEIGHT / 2,
                            CLACT_SURFACE_MAIN);
    } else {
        func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_SHAPE_ICON_SUB + shape, TRUE);
        func_ov210_021bee38(wk, ZKNSEARCH_OBJ_SHAPE_ICON_SUB + shape, SHAPE_X, y + SHAPE_ROW_HEIGHT / 2,
                            CLACT_SURFACE_SUB);
        func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_SHAPE_ICON_WINDOW + shape, TRUE);
        func_ov210_021bee38(wk, ZKNSEARCH_OBJ_SHAPE_ICON_WINDOW + shape, SHAPE_X, y + SHAPE_ROW_HEIGHT / 2,
                            CLACT_SURFACE_SUB);
    }
}

// pokebw2's ZknSearchObj_ScrollMarks
void func_ov210_021bfbf8(ZknSearchWork *wk, s8 delta) {
    u32 i;
    s16 x, y;

    for (i = ZKNSEARCH_OBJ_MARK; i <= ZKNSEARCH_OBJ_MARK + 1; i++) {
        if (func_ov210_021bee04(wk, i)) {
            func_ov210_021bee58(wk, i, &x, &y, CLACT_SURFACE_MAIN);
            func_ov210_021bee38(wk, i, x, y + delta, CLACT_SURFACE_MAIN);
            if (y + delta <= -ROW_HEIGHT / 2 || y + delta >= TOP_LIST_HEIGHT + ROW_HEIGHT / 2) {
                func_ov210_021bedf0(wk, i, FALSE);
            }
        }
    }
    for (i = ZKNSEARCH_OBJ_MARK_SUB; i <= ZKNSEARCH_OBJ_MARK_SUB + 1; i++) {
        if (func_ov210_021bee04(wk, i)) {
            func_ov210_021bee58(wk, i, &x, &y, CLACT_SURFACE_SUB);
            func_ov210_021bee38(wk, i, x, y + delta, CLACT_SURFACE_SUB);
            func_ov210_021bee38(wk, i + 2, x, y + delta, CLACT_SURFACE_SUB);
            if (y + delta <= -ROW_HEIGHT / 2 || y + delta >= TOP_LIST_HEIGHT + ROW_HEIGHT / 2) {
                func_ov210_021bedf0(wk, i, FALSE);
                func_ov210_021bedf0(wk, i + 2, FALSE);
            }
        }
    }
}

// pokebw2's ZknSearchObj_ScrollShapeIcons
void func_ov210_021bfce4(ZknSearchWork *wk, s8 delta) {
    u32 i;
    s16 x, y;

    for (i = ZKNSEARCH_OBJ_SHAPE_ICON; i < ZKNSEARCH_OBJ_SHAPE_ICON_SUB; i++) {
        if (func_ov210_021bee04(wk, i)) {
            func_ov210_021bee58(wk, i, &x, &y, CLACT_SURFACE_MAIN);
            func_ov210_021bee38(wk, i, x, y + delta, CLACT_SURFACE_MAIN);
            if (y + delta <= -SHAPE_ROW_HEIGHT / 2 || y + delta >= 220) {
                func_ov210_021bedf0(wk, i, FALSE);
            }
        }
    }
    func_ov210_021bee58(wk, ZKNSEARCH_OBJ_MARK, &x, &y, CLACT_SURFACE_MAIN);
    func_ov210_021bee38(wk, ZKNSEARCH_OBJ_MARK, x, y + delta, CLACT_SURFACE_MAIN);
    if (y + delta <= -SHAPE_ROW_HEIGHT / 2 || y + delta >= 220) {
        func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_MARK, FALSE);
    }

    for (i = ZKNSEARCH_OBJ_SHAPE_ICON_SUB; i < ZKNSEARCH_OBJ_SHAPE_ICON_WINDOW; i++) {
        if (func_ov210_021bee04(wk, i)) {
            func_ov210_021bee58(wk, i, &x, &y, CLACT_SURFACE_SUB);
            func_ov210_021bee38(wk, i, x, y + delta, CLACT_SURFACE_SUB);
            func_ov210_021bee38(wk, i + ZKNSEARCH_SHAPE_COUNT, x, y + delta, CLACT_SURFACE_SUB);
            if (y + delta <= -28 || y + delta >= 212) {
                func_ov210_021bedf0(wk, i, FALSE);
                func_ov210_021bedf0(wk, i + ZKNSEARCH_SHAPE_COUNT, FALSE);
            }
        }
    }
    func_ov210_021bee58(wk, ZKNSEARCH_OBJ_MARK_SUB, &x, &y, CLACT_SURFACE_SUB);
    func_ov210_021bee38(wk, ZKNSEARCH_OBJ_MARK_SUB, x, y + delta, CLACT_SURFACE_SUB);
    func_ov210_021bee38(wk, ZKNSEARCH_OBJ_MARK_WINDOW, x, y + delta, CLACT_SURFACE_SUB);
    if (y + delta <= -28 || y + delta >= 212) {
        func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_MARK_SUB, FALSE);
        func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_MARK_WINDOW, FALSE);
    }
}

// pokebw2's ZknSearchObj_MoveLoadingBar
void func_ov210_021bfe84(ZknSearchWork *wk, u32 count) {
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_LOADING_BAR, TRUE);
    func_ov210_021bee38(wk, ZKNSEARCH_OBJ_LOADING_BAR, count * 2 + 8, 168, CLACT_SURFACE_MAIN);
}
