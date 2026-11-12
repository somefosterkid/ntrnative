// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov139/touchbar.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// touchbar.c: the bar of icons at the bottom of the lower screen, such as the return button. The names are ours

#include "app/ui/touchbar.h"
#include "types.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/key.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "system/app_menu_common.h"
#include "system/bmp_winframe.h"

// The states of the bar
enum {
    TOUCHBAR_STATE_IDLE,
    TOUCHBAR_STATE_TOUCHED,
    TOUCHBAR_STATE_ANIMATING,
    TOUCHBAR_STATE_DONE,
};

// The kinds of icon: one that plays an animation when pressed, or one that toggles between two
enum {
    TOUCHBAR_KIND_PRESS,
    TOUCHBAR_KIND_TOGGLE,
};

// The index of the animations in an icon's item
enum {
    TOUCHBAR_ANIM_REST,
    TOUCHBAR_ANIM_INACTIVE,
    TOUCHBAR_ANIM_PRESSED,
    TOUCHBAR_ANIM_TOGGLED,
};

// The resources of TouchBarGfx::res
#define TOUCHBAR_RES_PALETTE 0
#define TOUCHBAR_RES_CHARS 1
#define TOUCHBAR_RES_CELL_ANIMS 2

// An icon of the bar's own, by icon number
typedef struct {
    u16 anims[4];
    u16 se;
    u32 key;
    u32 kind;
} TouchBarIconInfo;

// pokebw2's sIconInfo
static const TouchBarIconInfo data_ov067_021b942c[TOUCHBAR_ICON_CUSTOM] = {
    { { 0, 14, 8, 0 }, 0x556, PAD_BUTTON_X, TOUCHBAR_KIND_PRESS },
    { { 1, 15, 9, 0 }, 0x551, PAD_BUTTON_B, TOUCHBAR_KIND_PRESS },
    { { 2, 16, 10, 0 }, 0x54c, PAD_KEY_DOWN, TOUCHBAR_KIND_PRESS },
    { { 3, 17, 11, 0 }, 0x54c, PAD_KEY_UP, TOUCHBAR_KIND_PRESS },
    { { 4, 18, 12, 0 }, 0x54c, PAD_KEY_LEFT, TOUCHBAR_KIND_PRESS },
    { { 5, 19, 13, 0 }, 0x54c, PAD_KEY_RIGHT, TOUCHBAR_KIND_PRESS },
    { { 6, 21, 7, 22 }, 0x646, PAD_BUTTON_Y, TOUCHBAR_KIND_TOGGLE },
};

static TouchBarIcon *func_ov067_021b65c8(TouchBar *bar, u32 icon);
static TouchBarIcon *func_ov067_021b6600(TouchBar *bar, u32 icon);
static void func_ov067_021b6608(TouchBarGfx *gfx, u8 bg, u32 vramType, u32 bgScreenType, u32 mapping, u8 bgPalette,
                             u8 objPalette, BOOL noBg, HeapID heapId);
static void func_ov067_021b66fc(TouchBarGfx *gfx);
static u32 func_ov067_021b6730(const TouchBarGfx *gfx, u32 res);
static void func_ov067_021b6738(ArcTool *arc, u32 fileId, u32 bg, u32 charPos, u8 srcX, u8 srcY, u8 srcWidth, u8 srcHeight,
                          u8 x, u8 y, u8 width, u8 height, u8 palette, BOOL compressed, HeapID heapId);
static void func_ov067_021b67f8(TouchBarIcon *icon, ClActUnit *unit, TouchBarGfx *gfx,
                              const TouchBarItem *item, u32 surface, HeapID heapId);
static void func_ov067_021b691c(TouchBarIcon *icon);
static BOOL func_ov067_021b6934(TouchBarIcon *icon);
static BOOL func_ov067_021b69a4(TouchBarIcon *icon);
static u32 func_ov067_021b69b8(TouchBarIcon *icon);
static void func_ov067_021b69bc(TouchBarIcon *icon, BOOL visible);
static void func_ov067_021b69c8(TouchBarIcon *icon, BOOL active);
static void func_ov067_021b69d0(TouchBarIcon *icon, u32 se);
static void func_ov067_021b69d4(TouchBarIcon *icon, u32 key);
static void func_ov067_021b69d8(TouchBarIcon *icon, BOOL pressed);
static BOOL func_ov067_021b69f8(TouchBarIcon *icon);
static void func_ov067_021b6a10(TouchBarIcon *icon, u8 priority);
static void func_ov067_021b6a1c(TouchBarIcon *icon, u8 bgPriority);
static void func_ov067_021b6a28(TouchBarIcon *icon);
static void func_ov067_021b6a44(TouchBarIcon *icon);
static void func_ov067_021b6a68(TouchBarIcon *icon, BOOL active);
static void func_ov067_021b6a98(TouchBarIcon *icon, BOOL active);

// pokebw2's TouchBar_Create
TouchBar *func_ov067_021b62e0(TouchBarSetup *setup, HeapID heapId) {
    TouchBar *bar;
    u32 bgScreenType;
    u32 i;
    u32 size;
    u32 sub;

    size = sizeof(TouchBar) + setup->count * sizeof(TouchBarIcon);
    bar = GFL_HeapAllocate(heapId, size, FALSE, "touchbar.c", 291);
    sub = 0;
    MI_CpuFill8(bar, 0, size);
    bar->count = setup->count;
    bar->state = sub;
    if (setup->bg >= 4) {
        sub = 1;
        bgScreenType = 4;
    } else {
        bgScreenType = 0;
    }
    func_ov067_021b6608(&bar->gfx, setup->bg, sub, bgScreenType, setup->vramType, setup->bgPalette, setup->objPalette,
                     setup->unk1C, heapId);
    for (i = 0; i < bar->count; i++) {
        func_ov067_021b67f8(&bar->icons[i], setup->unit, &bar->gfx, &setup->items[i], sub, heapId);
    }
    return bar;
}

// pokebw2's TouchBar_Free
void func_ov067_021b639c(TouchBar *bar) {
    u32 i;

    for (i = 0; i < bar->count; i++) {
        func_ov067_021b691c(&bar->icons[i]);
    }
    func_ov067_021b66fc(&bar->gfx);
    GFL_HeapFree(bar);
}

// pokebw2's TouchBar_Main
void func_ov067_021b63d0(TouchBar *bar) {
    u32 i;

    switch (bar->state) {
    case TOUCHBAR_STATE_IDLE:
        bar->selected = -1;
        for (i = 0; i < bar->count; i++) {
            if (func_ov067_021b6934(&bar->icons[i])) {
                bar->selected = i;
                bar->state = TOUCHBAR_STATE_TOUCHED;
                return;
            }
        }
        break;
    case TOUCHBAR_STATE_TOUCHED:
        bar->state = TOUCHBAR_STATE_ANIMATING;
        // fallthrough
    case TOUCHBAR_STATE_ANIMATING:
        if (func_ov067_021b69a4(&bar->icons[bar->selected])) {
            bar->state = TOUCHBAR_STATE_DONE;
        }
        break;
    case TOUCHBAR_STATE_DONE:
        bar->state = TOUCHBAR_STATE_IDLE;
        break;
    }
}

// pokebw2's TouchBar_GetDecided
int func_ov067_021b6448(TouchBar *bar) {
    if (bar->state == TOUCHBAR_STATE_DONE && bar->selected != -1) {
        return func_ov067_021b69b8(&bar->icons[bar->selected]);
    }
    return -1;
}

// pokebw2's TouchBar_GetTouched
u32 func_ov067_021b6470(TouchBar *bar) {
    if (bar->state == TOUCHBAR_STATE_TOUCHED && bar->selected != -1) {
        return func_ov067_021b69b8(&bar->icons[bar->selected]);
    }
    return -1;
}

// pokebw2's TouchBar_IsTouching
BOOL func_ov067_021b6498(TouchBar *bar) {
    if (bar->state != TOUCHBAR_STATE_IDLE) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's TouchBar_SetActive
void func_ov067_021b64a8(TouchBar *bar, BOOL active) {
    u32 i;

    for (i = 0; i < bar->count; i++) {
        func_ov067_021b69c8(&bar->icons[i], active);
    }
}

// pokebw2's TouchBar_SetVisible
void func_ov067_021b64d0(TouchBar *bar, BOOL visible) {
    u32 i;

    for (i = 0; i < bar->count; i++) {
        func_ov067_021b69bc(&bar->icons[i], visible);
    }
}

// pokebw2's TouchBar_SetBgPriority
void func_ov067_021b64f8(TouchBar *bar, u8 bgPriority) {
    u32 i;

    for (i = 0; i < bar->count; i++) {
        func_ov067_021b6a1c(&bar->icons[i], bgPriority);
    }
}

// pokebw2's TouchBar_SetPriority
void func_ov067_021b6520(TouchBar *bar, u8 priority) {
    u32 i;

    for (i = 0; i < bar->count; i++) {
        func_ov067_021b6a10(&bar->icons[i], priority);
    }
}

// pokebw2's TouchBar_SetIconActive
void func_ov067_021b6548(TouchBar *bar, u32 icon, BOOL active) {
    func_ov067_021b69c8(func_ov067_021b65c8(bar, icon), active);
}

// pokebw2's TouchBar_SetIconVisible
void func_ov067_021b6558(TouchBar *bar, u32 icon, BOOL visible) {
    func_ov067_021b69bc(func_ov067_021b65c8(bar, icon), visible);
}

// pokebw2's TouchBar_SetIconSe
void func_ov067_021b6568(TouchBar *bar, u32 icon, u32 se) {
    func_ov067_021b69d0(func_ov067_021b65c8(bar, icon), se);
}

// pokebw2's TouchBar_SetIconKey
void func_ov067_021b6578(TouchBar *bar, u32 icon, u32 key) {
    func_ov067_021b69d4(func_ov067_021b65c8(bar, icon), key);
}

// pokebw2's TouchBar_SetIconBgPriority
void func_ov067_021b6588(TouchBar *bar, u32 icon, u8 bgPriority) {
    func_ov067_021b6a1c(func_ov067_021b65c8(bar, icon), bgPriority);
}

// pokebw2's TouchBar_SetIconPressed
void func_ov067_021b6598(TouchBar *bar, u32 icon, BOOL pressed) {
    func_ov067_021b69d8(func_ov067_021b65c8(bar, icon), pressed);
}

// pokebw2's TouchBar_IsIconPressed
BOOL func_ov067_021b65a8(TouchBar *bar, u32 icon) {
    return func_ov067_021b69f8(func_ov067_021b6600(bar, icon));
}

// Makes the bar idle again, then sets whether an icon is active
// pokebw2's TouchBar_Reset
void func_ov067_021b65b4(TouchBar *bar, u32 icon) {
    bar->state = TOUCHBAR_STATE_IDLE;
    bar->selected = -1;
    func_ov067_021b6548(bar, icon, TRUE);
}

// pokebw2's TouchBar_FindIcon
static TouchBarIcon *func_ov067_021b65c8(TouchBar *bar, u32 icon) {
    u32 i;

    for (i = 0; i < bar->count; i++) {
        if (icon == func_ov067_021b69b8(&bar->icons[i])) {
            return (TouchBarIcon *)(bar + 1) + i;
        }
    }
    return NULL;
}

// pokebw2's TouchBar_GetIcon
static TouchBarIcon *func_ov067_021b6600(TouchBar *bar, u32 icon) {
    return func_ov067_021b65c8(bar, icon);
}

static void func_ov067_021b6608(TouchBarGfx *gfx, u8 bg, u32 vramType, u32 bgScreenType, u32 mapping, u8 bgPalette,
                             u8 objPalette, BOOL noBg, HeapID heapId) {
    ArcTool *arc;

    MI_CpuFill8(gfx, 0, sizeof(TouchBarGfx));
    gfx->bg = bg;
    arc = GFL_ArcSysCreateFileHandle(getUINarcIdx(), heapId);
    if (!noBg) {
        GFL_G2DIOLoadArcNCLRDefault(arc, func_02027624(), bgScreenType, bgPalette * 32, 0x20, heapId);
        gfx->bgChars = GFL_BGSysLoadArcNCGRDynamic(arc, func_02027628(), gfx->bg, 0x20 << 6, FALSE, heapId);
        func_ov067_021b6738(arc, func_0202762c(), gfx->bg, CHAR_POS(gfx->bgChars), 0, 21, 32, 24, 0, 21, 32, 3, bgPalette,
                      FALSE, heapId);
    }
    gfx->res[TOUCHBAR_RES_PALETTE] =
        func_0204ac18(arc, func_02027614(), vramType, objPalette * 32, 0, 3, heapId);
    gfx->res[TOUCHBAR_RES_CHARS] = func_0204a6c8(arc, func_02027618(), FALSE, vramType, heapId);
    gfx->res[TOUCHBAR_RES_CELL_ANIMS] = func_0204af28(arc, func_0202761c(mapping), func_02027620(mapping), heapId);
    GFL_ArcToolFree(arc);
}

// pokebw2's TouchBarGfx_Unload
static void func_ov067_021b66fc(TouchBarGfx *gfx) {
    func_0204afd8(gfx->res[TOUCHBAR_RES_CELL_ANIMS]);
    func_0204a8d4(gfx->res[TOUCHBAR_RES_CHARS]);
    func_0204ada4(gfx->res[TOUCHBAR_RES_PALETTE]);
    GFL_BGSysFreeCharMemory(gfx->bg, CHAR_POS(gfx->bgChars), CHAR_SIZE(gfx->bgChars));
    MI_CpuFill8(gfx, 0, sizeof(TouchBarGfx));
}

// pokebw2's TouchBarGfx_GetResource
static u32 func_ov067_021b6730(const TouchBarGfx *gfx, u32 res) {
    return gfx->res[res];
}

// Loads a screen's rectangle into a BG, adding charPos to its characters when the BG has 16 colors palettes
static void func_ov067_021b6738(ArcTool *arc, u32 fileId, u32 bg, u32 charPos, u8 srcX, u8 srcY, u8 srcWidth, u8 srcHeight,
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

static void func_ov067_021b67f8(TouchBarIcon *icon, ClActUnit *unit, TouchBarGfx *gfx,
                              const TouchBarItem *item, u32 surface, HeapID heapId) {
    ClActorSetup setup;
    u32 chars;
    u32 palette;
    u32 cellAnims;

    MI_CpuFill8(icon, 0, sizeof(TouchBarIcon));
    icon->item = *item;
    icon->active = TRUE;
    MI_CpuFill8(&setup, 0, sizeof(setup));
    setup.x = item->pos.x;
    setup.y = item->pos.y;
    if (item->icon >= TOUCHBAR_ICON_CUSTOM) {
        chars = item->charRes;
        palette = item->plttRes;
        cellAnims = item->cellRes;
        icon->kind = TOUCHBAR_KIND_PRESS;
    } else {
        chars = func_ov067_021b6730(gfx, TOUCHBAR_RES_CHARS);
        palette = func_ov067_021b6730(gfx, TOUCHBAR_RES_PALETTE);
        cellAnims = func_ov067_021b6730(gfx, TOUCHBAR_RES_CELL_ANIMS);
        icon->item.anims[0] = data_ov067_021b942c[item->icon].anims[0];
        icon->item.anims[1] = data_ov067_021b942c[item->icon].anims[1];
        icon->item.anims[2] = data_ov067_021b942c[item->icon].anims[2];
        icon->item.anims[3] = data_ov067_021b942c[item->icon].anims[3];
        icon->item.key = data_ov067_021b942c[item->icon].key;
        icon->item.se = data_ov067_021b942c[item->icon].se;
        icon->kind = data_ov067_021b942c[item->icon].kind;
    }
    icon->anim = icon->item.anims[0];
    setup.sequence = icon->item.anims[0];
    icon->actor = func_0204b294(unit, chars, palette, cellAnims, &setup, surface, heapId);
    func_0204b92c(icon->actor, TRUE);
    switch (icon->kind) {
    case TOUCHBAR_KIND_PRESS:
        icon->onTouch = func_ov067_021b6a28;
        icon->setActive = func_ov067_021b6a68;
        break;
    case TOUCHBAR_KIND_TOGGLE:
        icon->onTouch = func_ov067_021b6a44;
        icon->setActive = func_ov067_021b6a98;
        break;
    }
}

// pokebw2's TouchBarIcon_Delete
static void func_ov067_021b691c(TouchBarIcon *icon) {
    func_0204b3b4(icon->actor);
    MI_CpuFill8(icon, 0, sizeof(TouchBarIcon));
}

// Whether the icon was touched or its key pressed, in which case its touch callback has run
// pokebw2's TouchBarIcon_CheckTouch
static BOOL func_ov067_021b6934(TouchBarIcon *icon) {
    BOOL touched = FALSE;

    if (func_0204b3f4(icon->actor) & icon->active) {
        u32 x, y;

        if (func_02035d20(&x, &y)) {
            if (x - icon->item.pos.x <= 24 && y - icon->item.pos.y <= 24) {
                touched = TRUE;
                func_020355d8(TRUE);
            }
        }
        if (icon->item.key != 0 && (GCTX_HIDGetPressedKeys() & icon->item.key)) {
            func_020355d8(FALSE);
            touched = TRUE;
        }
        if (touched) {
            icon->onTouch(icon);
            return TRUE;
        }
    }
    return FALSE;
}

// pokebw2's TouchBarIcon_IsAnimEnded
static BOOL func_ov067_021b69a4(TouchBarIcon *icon) {
    if (func_0204b98c(icon->actor) == 0) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's TouchBarIcon_GetId
static u32 func_ov067_021b69b8(TouchBarIcon *icon) {
    return icon->item.icon;
}

// pokebw2's TouchBarIcon_SetVisible
static void func_ov067_021b69bc(TouchBarIcon *icon, BOOL visible) {
    func_0204b3dc(icon->actor, visible);
}

// pokebw2's TouchBarIcon_SetActive
static void func_ov067_021b69c8(TouchBarIcon *icon, BOOL active) {
    icon->setActive(icon, active);
}

// pokebw2's TouchBarIcon_SetSe
static void func_ov067_021b69d0(TouchBarIcon *icon, u32 se) {
    icon->item.se = se;
}

// pokebw2's TouchBarIcon_SetKey
static void func_ov067_021b69d4(TouchBarIcon *icon, u32 key) {
    icon->item.key = key;
}

// pokebw2's TouchBarIcon_SetPressed
static void func_ov067_021b69d8(TouchBarIcon *icon, BOOL pressed) {
    if (pressed) {
        icon->anim = icon->item.anims[TOUCHBAR_ANIM_PRESSED];
    } else {
        icon->anim = icon->item.anims[TOUCHBAR_ANIM_REST];
    }
    func_0204b878(icon->actor, icon->anim);
}

// pokebw2's TouchBarIcon_IsPressed
static BOOL func_ov067_021b69f8(TouchBarIcon *icon) {
    BOOL pressed = TRUE;

    if (icon->anim != icon->item.anims[TOUCHBAR_ANIM_PRESSED] && icon->anim != icon->item.anims[TOUCHBAR_ANIM_TOGGLED]) {
        pressed = FALSE;
    }
    return pressed;
}

// pokebw2's TouchBarIcon_SetPriority
static void func_ov067_021b6a10(TouchBarIcon *icon, u8 priority) {
    func_0204b800(icon->actor, priority);
}

// pokebw2's TouchBarIcon_SetBgPriority
static void func_ov067_021b6a1c(TouchBarIcon *icon, u8 bgPriority) {
    func_0204b84c(icon->actor, bgPriority);
}

// pokebw2's TouchBarIcon_OnTouchPress
static void func_ov067_021b6a28(TouchBarIcon *icon) {
    icon->anim = icon->item.anims[TOUCHBAR_ANIM_PRESSED];
    func_0204b878(icon->actor, icon->anim);
    if (icon->item.se != 0) {
        GFL_SndSEPlay(icon->item.se);
    }
}

// pokebw2's TouchBarIcon_OnTouchToggle
static void func_ov067_021b6a44(TouchBarIcon *icon) {
    func_ov067_021b69d8(icon, !func_ov067_021b69f8(icon));
    if (icon->item.se != 0) {
        GFL_SndSEPlay(icon->item.se);
    }
}

// pokebw2's TouchBarIcon_SetActivePress
static void func_ov067_021b6a68(TouchBarIcon *icon, BOOL active) {
    icon->active = active;
    if (active) {
        if (icon->anim == icon->item.anims[TOUCHBAR_ANIM_PRESSED]) {
            icon->anim = icon->item.anims[TOUCHBAR_ANIM_REST];
        }
        func_0204b878(icon->actor, icon->anim);
    } else {
        func_0204b878(icon->actor, icon->item.anims[TOUCHBAR_ANIM_INACTIVE]);
    }
}

// pokebw2's TouchBarIcon_SetActiveToggle
static void func_ov067_021b6a98(TouchBarIcon *icon, BOOL active) {
    icon->active = active;
    if (active) {
        if (icon->anim == icon->item.anims[TOUCHBAR_ANIM_PRESSED]) {
            icon->anim = icon->item.anims[TOUCHBAR_ANIM_TOGGLED];
        } else if (icon->anim == icon->item.anims[TOUCHBAR_ANIM_INACTIVE]) {
            icon->anim = icon->item.anims[TOUCHBAR_ANIM_REST];
        }
        func_0204b878(icon->actor, icon->anim);
    } else {
        if (icon->anim == icon->item.anims[TOUCHBAR_ANIM_TOGGLED]) {
            icon->anim = icon->item.anims[TOUCHBAR_ANIM_PRESSED];
        } else if (icon->anim == icon->item.anims[TOUCHBAR_ANIM_REST]) {
            icon->anim = icon->item.anims[TOUCHBAR_ANIM_INACTIVE];
        }
        func_0204b878(icon->actor, icon->item.anims[TOUCHBAR_ANIM_INACTIVE]);
    }
}
