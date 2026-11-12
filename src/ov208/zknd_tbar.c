// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov298/zknd_tbar.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/zukan_detail.h"
#include "constants/sound.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "nnsys/g2d.h"
#include "system/app_menu_common.h"
#include "system/bmp_winframe.h"
#include "pml/poke_graphic.h"

ClActor *func_0204c040(ClActUnit *unit, u32 chars, u32 palette, u32 cellAnims, const ClActorSetup *setup, u16 surface,
                       HeapID heapId);

// The Pokédex's copy of the touch bar: a bar at the bottom of a screen with icons that are touched or pressed with
// their keys. An icon plays its pushed animation, or flips on or off, and the bar reports it once the animation ends

// The bar's steps
enum {
    TBAR_SEQ_WAIT,
    TBAR_SEQ_TOUCHED,
    TBAR_SEQ_ANIM,
    TBAR_SEQ_DONE,
};

// The icons' resources
enum {
    OBJRES_PALETTE,
    OBJRES_CHARS,
    OBJRES_CELLANIMS,
    OBJRES_COUNT,
};

// The resources of the bar's icons and BG
typedef struct {
    u32 objRes[OBJRES_COUNT];
    u32 bg;
    u32 bgChars;
} ZkndTbarResource;

typedef struct ZkndTbarItem ZkndTbarItem;
typedef void (*ZkndTbarItemCallback)(ZkndTbarItem *item);

struct ZkndTbarItem {
    ClActor *actor;
    BOOL active;
    u32 anim;
    ZkndTbarItemCallback callback;
    u32 type;
    ZkndTbarIcon icon;
    union {
        BOOL pushed;
        BOOL triggered;
    };
};

struct ZkndTbar {
    int trigger;
    u32 itemCount;
    int seq;
    BOOL active;
    // Set when an icon is touched, until the app unlocks the bar
    BOOL locked;
    ZkndTbarResource resource;
    ZkndTbarItem items[0];
};

// The animations, sound, key and type of the bar's own icons
typedef struct {
    u16 activeAnim;
    u16 inactiveAnim;
    u16 pushedAnim;
    u16 se;
    u32 key;
    u32 type;
} ZkndTbarIconData;

static void func_ov208_021bd814(ZkndTbarItem *item, BOOL active);
static void func_ov208_021bd844(ZkndTbarItem *item, u32 key);
static void func_ov208_021bd848(ZkndTbarItem *item, BOOL flip);
static BOOL func_ov208_021bd868(const ZkndTbarItem *item);
static void func_ov208_021bd878(ZkndTbarItem *item);
static void func_ov208_021bd880(ZkndTbarItem *item);
static void func_ov208_021bd89c(ZkndTbarItem *item);
void func_ov208_021bd538(ZkndTbarResource *resource);
void func_ov208_021bd444(ZkndTbarResource *gfx, u8 bg, u32 vramType, u32 bgScreenType, u32 mapping, u8 bgPalette,
                         u8 objPalette, BOOL noBg, HeapID heapId);
void func_ov208_021bd634(ZkndTbarItem *item, ClActUnit *unit, const ZkndTbarResource *resource, const ZkndTbarIcon *icon,
                         u32 surface, HeapID heapId);
BOOL func_ov208_021bd764(ZkndTbarItem *item);
ZkndTbarItem *func_ov208_021bd404(ZkndTbar *tbar, u32 type);
ZkndTbarItem *func_ov208_021bd43c(ZkndTbar *tbar, u32 type);
void func_ov208_021bd74c(ZkndTbarItem *item);
BOOL func_ov208_021bd7e4(ZkndTbarItem *item);
u32 func_ov208_021bd7f8(const ZkndTbarItem *item);
void func_ov208_021bd7fc(ZkndTbarItem *item, BOOL visible);
BOOL func_ov208_021bd808(ZkndTbarItem *item);

// pokebw2's sZkndTbarIconData
static const ZkndTbarIconData data_ov208_021c8314[ZKND_TBAR_ICON_CUSTOM] = {
    { 0, 14, 8, SEQ_SE_CLOSE1, PAD_BUTTON_X, ZKND_TBAR_TYPE_PUSH },
    { 1, 15, 9, SEQ_SE_CANCEL1, PAD_BUTTON_B, ZKND_TBAR_TYPE_PUSH },
    { 2, 16, 10, SEQ_SE_SELECT1, PAD_KEY_DOWN, ZKND_TBAR_TYPE_PUSH },
    { 3, 17, 11, SEQ_SE_SELECT1, PAD_KEY_UP, ZKND_TBAR_TYPE_PUSH },
    { 4, 18, 12, SEQ_SE_SELECT1, PAD_KEY_LEFT, ZKND_TBAR_TYPE_PUSH },
    { 5, 19, 13, SEQ_SE_SELECT1, PAD_KEY_RIGHT, ZKND_TBAR_TYPE_PUSH },
    { 6, 20, 7, SEQ_SE_SYS_07, PAD_BUTTON_Y, ZKND_TBAR_TYPE_FLIP },
};

// pokebw2's ZkndTbar_Create
ZkndTbar *func_ov208_021bd148(ZkndTbarParam *param, HeapID heapId) {
    u32 size = sizeof(ZkndTbar) + param->iconCount * sizeof(ZkndTbarItem);
    ZkndTbar *tbar = GFL_HeapAllocate(heapId, size, FALSE, "zknd_tbar.c", 263);
    u32 engine;
    u32 palType;
    u32 i;

    MI_CpuFill8(tbar, 0, size);
    tbar->itemCount = param->iconCount;
    tbar->seq = TBAR_SEQ_WAIT;
    tbar->active = TRUE;
    tbar->locked = FALSE;

    palType = PALTYPE_MAIN_BG;
    engine = CLACT_VRAM_SUB;
    if (param->bg >= 4) {
        palType = PALTYPE_SUB_BG;
    } else {
        engine = CLACT_VRAM_MAIN;
    }
    func_ov208_021bd444(&tbar->resource, param->bg, engine, palType, param->mapping, param->bgPalette,
                          param->objPalette, param->noBG, heapId);
    for (i = 0; i < tbar->itemCount; i++) {
        func_ov208_021bd634(&tbar->items[i], param->unit, &tbar->resource, &param->icons[i], engine, heapId);
    }
    return tbar;
}

// pokebw2's ZkndTbar_Free
void func_ov208_021bd208(ZkndTbar *tbar) {
    u32 i;

    for (i = 0; i < tbar->itemCount; i++) {
        func_ov208_021bd74c(&tbar->items[i]);
    }
    func_ov208_021bd538(&tbar->resource);
    GFL_HeapFree(tbar);
}

// pokebw2's ZkndTbar_Main
void func_ov208_021bd23c(ZkndTbar *tbar) {
    u32 i;

    switch (tbar->seq) {
    case TBAR_SEQ_WAIT:
        tbar->trigger = -1;
        if (tbar->active && !tbar->locked) {
            for (i = 0; i < tbar->itemCount; i++) {
                if (func_ov208_021bd764(&tbar->items[i])) {
                    tbar->trigger = i;
                    tbar->seq = TBAR_SEQ_TOUCHED;
                    tbar->locked = TRUE;
                    break;
                }
            }
        }
        break;
    case TBAR_SEQ_TOUCHED:
        tbar->seq = TBAR_SEQ_ANIM;
        // fallthrough
    case TBAR_SEQ_ANIM:
        if (func_ov208_021bd7e4(&tbar->items[tbar->trigger])) {
            tbar->seq = TBAR_SEQ_DONE;
        }
        break;
    case TBAR_SEQ_DONE:
        tbar->seq = TBAR_SEQ_WAIT;
        break;
    }

    for (i = 0; i < tbar->itemCount; i++) {
        func_ov208_021bd878(&tbar->items[i]);
    }
}

// pokebw2's ZkndTbar_GetTrigger
int func_ov208_021bd2e0(ZkndTbar *tbar) {
    if (tbar->seq == TBAR_SEQ_DONE && tbar->trigger != -1) {
        return func_ov208_021bd7f8(&tbar->items[tbar->trigger]);
    }
    return -1;
}

// pokebw2's ZkndTbar_GetTouch
int func_ov208_021bd308(ZkndTbar *tbar) {
    if (tbar->seq == TBAR_SEQ_TOUCHED && tbar->trigger != -1) {
        return func_ov208_021bd7f8(&tbar->items[tbar->trigger]);
    }
    return -1;
}

// pokebw2's ZkndTbar_SetVisibleAll
void func_ov208_021bd330(ZkndTbar *tbar, BOOL visible) {
    u32 i;

    for (i = 0; i < tbar->itemCount; i++) {
        func_ov208_021bd7fc(&tbar->items[i], visible);
    }
}

// pokebw2's ZkndTbar_SetActiveAll
void func_ov208_021bd358(ZkndTbar *tbar, BOOL active) {
    tbar->active = active;
}

// pokebw2's ZkndTbar_GetActiveAll
BOOL func_ov208_021bd35c(ZkndTbar *tbar) {
    return tbar->active;
}

// pokebw2's ZkndTbar_Unlock
void func_ov208_021bd360(ZkndTbar *tbar) {
    tbar->locked = FALSE;
}

// pokebw2's ZkndTbar_SetActive
void func_ov208_021bd368(ZkndTbar *tbar, int icon, BOOL active) {
    func_ov208_021bd814(func_ov208_021bd404(tbar, icon), active);
}

// pokebw2's ZkndTbar_SetVisible
void func_ov208_021bd378(ZkndTbar *tbar, int icon, BOOL visible) {
    func_ov208_021bd7fc(func_ov208_021bd404(tbar, icon), visible);
}

// pokebw2's ZkndTbar_GetVisible
BOOL func_ov208_021bd388(ZkndTbar *tbar, int icon) {
    return func_ov208_021bd808(func_ov208_021bd43c(tbar, icon));
}

// pokebw2's ZkndTbar_SetKey
void func_ov208_021bd394(ZkndTbar *tbar, int icon, u32 key) {
    func_ov208_021bd844(func_ov208_021bd404(tbar, icon), key);
}

// pokebw2's ZkndTbar_SetFlip
void func_ov208_021bd3a4(ZkndTbar *tbar, int icon, BOOL flip) {
    func_ov208_021bd848(func_ov208_021bd404(tbar, icon), flip);
}

// pokebw2's ZkndTbar_GetFlip
BOOL func_ov208_021bd3b4(ZkndTbar *tbar, int icon) {
    return func_ov208_021bd868(func_ov208_021bd43c(tbar, icon));
}

// pokebw2's ZkndTbar_GetActor
ClActor *func_ov208_021bd3c0(ZkndTbar *tbar, int icon) {
    return func_ov208_021bd404(tbar, icon)->actor;
}

// pokebw2's ZkndTbar_SetPos
void func_ov208_021bd3cc(ZkndTbar *tbar, int icon, const ClActorPos *pos) {
    ZkndTbarItem *item = func_ov208_021bd404(tbar, icon);
    u32 surface;

    item->icon.pos = *pos;
    surface = CLACT_VRAM_SUB;
    if (tbar->resource.bg < 4) {
        surface = CLACT_VRAM_MAIN;
    }
    // The original truncates the surface to 16 bits
    func_0204b404(item->actor, pos, surface);
}

// pokebw2's ZkndTbar_Push
void func_ov208_021bd3f8(ZkndTbar *tbar, int icon) {
    func_ov208_021bd404(tbar, icon)->pushed = TRUE;
}

ZkndTbarItem *func_ov208_021bd404(ZkndTbar *tbar, u32 type) {
    u32 i;

    for (i = 0; i < tbar->itemCount; i++) {
        if (type == func_ov208_021bd7f8(&tbar->items[i])) {
            return &tbar->items[i];
        }
    }
    return NULL;
}

ZkndTbarItem *func_ov208_021bd43c(ZkndTbar *tbar, u32 type) {
    return func_ov208_021bd404(tbar, type);
}

void func_ov208_021bd574(ArcTool *arc, u32 fileId, u32 bg, u32 charPos, u8 srcX, u8 srcY, u8 srcWidth, u8 srcHeight, u8 x, u8 y, u8 width, u8 height, u8 palette, BOOL compressed, HeapID heapId);
void func_ov208_021bd444(ZkndTbarResource *gfx, u8 bg, u32 vramType, u32 bgScreenType, u32 mapping,
                                u8 bgPalette, u8 objPalette, BOOL noBg, HeapID heapId) {
    ArcTool *arc;
    BOOL flag;

    flag = FALSE;
    MI_CpuFill8(gfx, 0, sizeof(ZkndTbarResource));
    gfx->bg = bg;
    arc = GFL_ArcSysCreateFileHandle(getUINarcIdx(), heapId);
    if (!noBg) {
        GFL_G2DIOLoadArcNCLRDefault(arc, func_02027624(), bgScreenType, bgPalette * 32, 0x20, heapId);
        gfx->bgChars = GFL_BGSysLoadArcNCGRDynamic(arc, func_02027628(), gfx->bg, 0x20 << 6, flag, heapId);
        func_ov208_021bd574(arc, func_0202762c(), gfx->bg, CHAR_POS(gfx->bgChars), flag, 0x15, 0x20, 0x18,
                      flag, 0x15, 0x20, 3, bgPalette, flag, heapId);
    }
    gfx->objRes[OBJRES_PALETTE] =
        func_0204ac18(arc, func_02027614(), vramType, objPalette * 32, flag, 3, heapId);
    gfx->objRes[OBJRES_CHARS] = func_0204a6c8(arc, func_02027618(), FALSE, vramType, heapId);
    gfx->objRes[OBJRES_CELLANIMS] = func_0204af28(arc, func_0202761c(mapping), func_02027620(mapping), heapId);
    GFL_ArcToolFree(arc);
}

void func_ov208_021bd538(ZkndTbarResource *resource) {
    func_0204afd8(resource->objRes[OBJRES_CELLANIMS]);
    func_0204a8d4(resource->objRes[OBJRES_CHARS]);
    func_0204ada4(resource->objRes[OBJRES_PALETTE]);
    GFL_BGSysFreeCharMemory(resource->bg, CHAR_POS(resource->bgChars), CHAR_SIZE(resource->bgChars));
    MI_CpuFill8(resource, 0, sizeof(ZkndTbarResource));
}

u32 func_ov208_021bd56c(const ZkndTbarResource *resource, u32 index)
{
    return resource->objRes[index];
}

void func_ov208_021bd574(ArcTool *arc, u32 fileId, u32 bg, u32 charPos, u8 srcX, u8 srcY, u8 srcWidth, u8 srcHeight,
                          u8 x, u8 y, u8 width, u8 height, u8 palette, BOOL compressed, HeapID heapId) {
    NNSG2dScreenData *screen;
    void *file = GFL_ArcToolReadHeapNewLZ(arc, fileId, compressed, HEAPID_TAIL(heapId));
    int i;

    NNS_G2dGetUnpackedScreenData(file, &screen);
    if (charPos != 0 && GFL_BGSysGetBGColorPaletteMode(bg) == 0) {
        u16 *data = (u16 *)screen->rawData;

        for (i = 0; i < srcWidth * srcHeight; i++) {
            data[i] += (u16)charPos;
        }
    }
    if (GFL_BGSysIsScrHeapExists(bg)) {
        GFL_BGSysLoadScrArea(bg, x, y, width, height, screen->rawData, srcX, srcY, srcWidth, srcHeight);
        GFL_BGSysSetScrPaletteNo(bg, x, y, width, height, palette);
        GFL_BGSysLoadScr(bg);
    }
    GFL_HeapFree(file);
}

void func_ov208_021bd634(ZkndTbarItem *item, ClActUnit *unit, const ZkndTbarResource *resource, const ZkndTbarIcon *icon,
                         u32 surface, HeapID heapId) {
    ClActorSetup setup;
    u32 chars;
    u32 palette;
    u32 cellAnims;

    MI_CpuFill8(item, 0, sizeof(ZkndTbarItem));
    item->icon = *icon;
    item->active = TRUE;
    item->pushed = FALSE;

    MI_CpuFill8(&setup, 0, sizeof(setup));
    setup.bgPriority = GFL_BGSysGetBGPriority(resource->bg);
    setup.x = icon->pos.x;
    setup.y = icon->pos.y;
    if (icon->icon >= 7) {
        chars = icon->chars;
        palette = icon->palette;
        cellAnims = icon->cellAnims;
        item->type = 0;
    } else {
        chars = func_ov208_021bd56c(resource, 1);
        palette = func_ov208_021bd56c(resource, 0);
        cellAnims = func_ov208_021bd56c(resource, 2);
        item->icon.activeAnim = data_ov208_021c8314[icon->icon].activeAnim;
        item->icon.inactiveAnim = data_ov208_021c8314[icon->icon].inactiveAnim;
        item->icon.pushedAnim = data_ov208_021c8314[icon->icon].pushedAnim;
        item->icon.key = data_ov208_021c8314[icon->icon].key;
        item->icon.se = data_ov208_021c8314[icon->icon].se;
        item->type = data_ov208_021c8314[icon->icon].type;
    }
    item->anim = item->icon.activeAnim;
    setup.sequence = item->icon.activeAnim;

    item->actor = func_0204b294(unit, chars, palette, cellAnims, &setup, surface, heapId);
    func_0204b92c(item->actor, TRUE);
    switch (item->type) {
    case 0:
        item->callback = func_ov208_021bd880;
        break;
    case 1:
        item->callback = func_ov208_021bd89c;
        break;
    }
}

void func_ov208_021bd74c(ZkndTbarItem *item) {
    func_0204b3b4(item->actor);
    MI_CpuFill8(item, 0, sizeof(ZkndTbarItem));
}

BOOL func_ov208_021bd764(ZkndTbarItem *item) {
    BOOL touched = FALSE;
    u32 x;
    u32 y;

    if (func_0204b3f4(item->actor) & item->active) {
        if (func_02035d20(&x, &y)) {
            if ((u32)(x - item->icon.pos.x) <= item->icon.width && (u32)(y - item->icon.pos.y) <= 0x18) {
                touched = TRUE;
                func_020355d8(TRUE);
            }
        }
        if (item->icon.key != 0 && (GCTX_HIDGetPressedKeys() & item->icon.key)) {
            func_020355d8(FALSE);
            touched = TRUE;
        }
        if (item->pushed) {
            func_020355d8(FALSE);
            touched = TRUE;
        }
        if (touched) {
            item->callback(item);
            return TRUE;
        }
    }
    return FALSE;
}


BOOL func_ov208_021bd7e4(ZkndTbarItem *item) {
    if (func_0204b98c(item->actor) == 0) {
        return TRUE;
    }
    return FALSE;
}

u32 func_ov208_021bd7f8(const ZkndTbarItem *item) {
    return item->icon.icon;
}

void func_ov208_021bd7fc(ZkndTbarItem *item, BOOL visible) {
    func_0204b3dc(item->actor, visible);
}



BOOL func_ov208_021bd808(ZkndTbarItem *item) {
    return func_0204b3f4(item->actor);
}






















// pokebw2's ZkndTbarItem_SetActive
static void func_ov208_021bd814(ZkndTbarItem *item, BOOL active) {
    item->active = active;
    if (active) {
        if (item->anim == item->icon.pushedAnim) {
            item->anim = item->icon.activeAnim;
        }
        func_0204b878(item->actor, item->anim);
    } else {
        func_0204b878(item->actor, item->icon.inactiveAnim);
    }
}

// pokebw2's ZkndTbarItem_SetKey
static void func_ov208_021bd844(ZkndTbarItem *item, u32 key) {
    item->icon.key = key;
}

// pokebw2's ZkndTbarItem_SetFlip
static void func_ov208_021bd848(ZkndTbarItem *item, BOOL flip) {
    if (flip) {
        item->anim = item->icon.pushedAnim;
    } else {
        item->anim = item->icon.activeAnim;
    }
    func_0204b878(item->actor, item->anim);
}

// pokebw2's ZkndTbarItem_GetFlip
static BOOL func_ov208_021bd868(const ZkndTbarItem *item) {
    if (item->anim == item->icon.pushedAnim) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's ZkndTbarItem_ClearPush
static void func_ov208_021bd878(ZkndTbarItem *item) {
    item->pushed = FALSE;
}

// pokebw2's ZkndTbarItem_PushCallback
static void func_ov208_021bd880(ZkndTbarItem *item) {
    item->anim = item->icon.pushedAnim;
    func_0204b878(item->actor, item->icon.pushedAnim);
    if (item->icon.se != 0) {
        GFL_SndSEPlay(item->icon.se);
    }
}

// pokebw2's ZkndTbarItem_FlipCallback
static void func_ov208_021bd89c(ZkndTbarItem *item) {
    func_ov208_021bd848(item, func_ov208_021bd868(item) ? FALSE : TRUE);
    if (item->icon.se != 0) {
        GFL_SndSEPlay(item->icon.se);
    }
}
