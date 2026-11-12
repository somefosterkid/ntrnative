// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov298/zukan_detail_touchbar.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/zukan_detail.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/clact.h"
#include "gfl/graphics.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/std.h"
#include "gfl/tcb.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"
#include "nnsys/g2d.h"
#include "nnsys/gfd.h"

// The detail screen's touch bar, on BG 1 of the main screen: the tabs of the pages with the buttons to close, return,
// check the Pokémon and move through the list, or the map's and the forms' own buttons. It slides in and out, and the
// tab of the page being shown glows

#define ITEM_MAX 9
#define ITEM_NONE ITEM_MAX

// How far the bar is hidden below the screen
#define HIDDEN_Y 24

// The bar's icons of each type, in the order of the bar's items
enum {
    GENERAL_RETURN,
    GENERAL_CLOSE,
    GENERAL_CHECK,
    GENERAL_FORM,
    GENERAL_VOICE,
    GENERAL_MAP,
    GENERAL_INFO,
    GENERAL_CUR_D,
    GENERAL_CUR_U,
};

enum {
    MAP_RETURN,
    MAP_PLACE,
};

enum {
    FORM_RETURN,
    FORM_CUR_R,
    FORM_CUR_L,
    FORM_BUTTON,
    FORM_CUR_D,
    FORM_CUR_U,
};

// The Pokédex's own icons
enum {
    ICON_INFO = ZKND_TBAR_ICON_CUSTOM,
    ICON_MAP,
    ICON_VOICE,
    ICON_FORM,
};

typedef struct {
    int icon;
    ClActorPos pos;
    u16 width;
    // The commands of the icon once its animation has played, and as it is touched
    int trigger;
    int touch;
} ZukanDetailTouchbarIcon;

typedef struct {
    const ZukanDetailTouchbarIcon *icon;
    ClActor *actor;
} ZukanDetailTouchbarItem;

// The resources of the Pokédex's own icons
typedef struct {
    u32 palette;
    u32 chars;
    u32 cellAnims;
} ZukanDetailTouchbarRes;

struct ZukanDetailTouchbar {
    HeapID heapId;
    BOOL showFormTab;
    int state;
    // The type before the last change, whose icons go then
    int prevType;
    int type;
    // The page shown before, the page shown, and the page whose tab glows
    int prevPage;
    int page;
    int selectedPage;
    u32 speed;
    u8 wait;
    ClActUnit *unit;
    TCB *vblankTcb;
    ZkndTbar *tbar;
    BOOL active;
    u8 bgPriority;
    BOOL bgPriorityChanged;
    BOOL showArrows;
    ZukanDetailTouchbarRes res[3];
    u8 itemCount;
    ZukanDetailTouchbarItem items[ITEM_MAX];
    s16 scrollY;
    // The selected tab's palette glows between two palettes
    u16 glowPalettes[2][16];
    u16 glowPalette[16];
    int glowPhase;
};

void func_ov208_021bdcdc(TCB *tcb, void *data);
static void func_ov208_021bdcf8(ZukanDetailTouchbar *touchbar);
static void func_ov208_021bdd24(ZukanDetailTouchbar *touchbar);
static void func_ov208_021bdd48(ZukanDetailTouchbar *touchbar, s16 offset);
void func_ov208_021bdd98(ZukanDetailTouchbar *touchbar);
static void func_ov208_021bdfa8(ZukanDetailTouchbar *touchbar);
void func_ov208_021bdfdc(ZukanDetailTouchbar *touchbar);
void func_ov208_021be128(ZukanDetailTouchbar *touchbar);
static void func_ov208_021be16c(ZukanDetailTouchbar *touchbar);
static void func_ov208_021be170(ZukanDetailTouchbar *touchbar);
static void func_ov208_021be1bc(ZukanDetailTouchbar *touchbar);
static void func_ov208_021be20c(ZukanDetailTouchbar *touchbar);
u8 func_ov208_021be300(int page);
u8 func_ov208_021be32c(int command);
u8 func_ov208_021be378(int command);
static void func_ov208_021be3c4(ZukanDetailTouchbar *touchbar);
void func_ov208_021be4dc(ZukanDetailTouchbar *touchbar);
void func_ov208_021be504(ZukanDetailTouchbar *touchbar);
void func_ov208_021be680(ZukanDetailTouchbar *touchbar);
static void func_ov208_021be6a8(ZukanDetailTouchbar *touchbar);

// How long the bar waits between moves, and how far it moves, at each speed
static const u8 data_ov208_021c8384[2] = { 0, 0 };
static const u32 data_ov208_021c8388[2] = { 3, 3 };

// Where the general icons go when the bar is in the other mode, and when the forms' tab is hidden
// pokebw2's sZukanDetailTouchbarNoFormPositions
static const ClActorPos data_ov208_021c83a4[ITEM_MAX] = {
    { 232, 168 }, { 208, 168 }, { 184, 172 }, { 168, 168 }, { 136, 168 },
    { 96, 168 },  { 56, 168 },  { 24, 168 },  { 0, 168 },
};

static const ClActorPos sZukanDetailTouchbarModePositions[ITEM_MAX] = {
    { 232, 168 }, { 208, 168 }, { 184, 172 }, { 136, 168 }, { 96, 168 },
    { 96, 168 },  { 56, 168 },  { 24, 168 },  { 0, 168 },
};

// pokebw2's sZukanDetailTouchbarMapIcons
static const ZukanDetailTouchbarIcon data_ov208_021c8390[] = {
    { ZKND_TBAR_ICON_RETURN, { 232, 168 }, 24, ZUKAN_DETAIL_CMD_MAP_RETURN, ZUKAN_DETAIL_CMD_MAP_RETURN_TOUCH },
};

// pokebw2's sZukanDetailTouchbarFormIcons
static const ZukanDetailTouchbarIcon data_ov208_021c83c8[] = {
    { ZKND_TBAR_ICON_RETURN, { 232, 168 }, 24, ZUKAN_DETAIL_CMD_FORM_RETURN, ZUKAN_DETAIL_CMD_FORM_RETURN_TOUCH },
    { ZKND_TBAR_ICON_CUR_R, { 192, 168 }, 24, ZUKAN_DETAIL_CMD_FORM_CUR_R, ZUKAN_DETAIL_CMD_FORM_CUR_R_TOUCH },
    { ZKND_TBAR_ICON_CUR_L, { 168, 168 }, 24, ZUKAN_DETAIL_CMD_FORM_CUR_L, ZUKAN_DETAIL_CMD_FORM_CUR_L_TOUCH },
    { ICON_INFO, { 104, 168 }, 48, ZUKAN_DETAIL_CMD_FORM_BUTTON, ZUKAN_DETAIL_CMD_FORM_BUTTON_TOUCH },
    { ZKND_TBAR_ICON_CUR_D, { 24, 168 }, 24, ZUKAN_DETAIL_CMD_FORM_CUR_D, ZUKAN_DETAIL_CMD_FORM_CUR_D_TOUCH },
    { ZKND_TBAR_ICON_CUR_U, { 0, 168 }, 24, ZUKAN_DETAIL_CMD_FORM_CUR_U, ZUKAN_DETAIL_CMD_FORM_CUR_U_TOUCH },
};

// pokebw2's sZukanDetailTouchbarGeneralIcons
static const ZukanDetailTouchbarIcon data_ov208_021c8440[] = {
    { ZKND_TBAR_ICON_RETURN, { 232, 168 }, 24, ZUKAN_DETAIL_CMD_RETURN, ZUKAN_DETAIL_CMD_RETURN_TOUCH },
    { ZKND_TBAR_ICON_CLOSE, { 208, 168 }, 24, ZUKAN_DETAIL_CMD_CLOSE, ZUKAN_DETAIL_CMD_CLOSE_TOUCH },
    { ZKND_TBAR_ICON_CHECK, { 184, 172 }, 24, ZUKAN_DETAIL_CMD_CHECK, ZUKAN_DETAIL_CMD_CHECK_TOUCH },
    { ICON_FORM, { 144, 168 }, 32, ZUKAN_DETAIL_CMD_FORM, ZUKAN_DETAIL_CMD_FORM_TOUCH },
    { ICON_VOICE, { 112, 168 }, 32, ZUKAN_DETAIL_CMD_VOICE, ZUKAN_DETAIL_CMD_VOICE_TOUCH },
    { ICON_MAP, { 80, 168 }, 32, ZUKAN_DETAIL_CMD_MAP, ZUKAN_DETAIL_CMD_MAP_TOUCH },
    { ICON_INFO, { 48, 168 }, 32, ZUKAN_DETAIL_CMD_INFO, ZUKAN_DETAIL_CMD_INFO_TOUCH },
    { ZKND_TBAR_ICON_CUR_D, { 24, 168 }, 24, ZUKAN_DETAIL_CMD_CUR_D, ZUKAN_DETAIL_CMD_CUR_D_TOUCH },
    { ZKND_TBAR_ICON_CUR_U, { 0, 168 }, 24, ZUKAN_DETAIL_CMD_CUR_U, ZUKAN_DETAIL_CMD_CUR_U_TOUCH },
};

// pokebw2's ZukanDetailTouchbar_Free

ZukanDetailTouchbar *func_ov208_021bd8c0(HeapID heapId, BOOL showFormTab) {
    ZukanDetailTouchbar *touchbar = GFL_HeapAllocate(heapId, sizeof(ZukanDetailTouchbar), TRUE, "zukan_detail_touchbar.c", 328);

    touchbar->heapId = heapId;
    touchbar->showFormTab = showFormTab;
    touchbar->state = 0;
    touchbar->prevType = 0;
    touchbar->type = 0;
    touchbar->prevPage = ZUKAN_DETAIL_PAGE_INFO;
    touchbar->page = 0;
    touchbar->selectedPage = 0;
    touchbar->speed = 0;
    GFL_BGSysMoveBG(1, BG_MOVE_SET_Y, -HIDDEN_Y);
    GFL_BGSysSetBGPriority(1, 0);
    touchbar->bgPriority = 0;
    touchbar->bgPriorityChanged = FALSE;
    touchbar->unit = func_0204b100(16, 0, touchbar->heapId);
    func_0204b270(touchbar->unit);
    touchbar->tbar = NULL;
    func_ov208_021bdcf8(touchbar);
    func_ov208_021bd358(touchbar->tbar, FALSE);
    touchbar->scrollY = HIDDEN_Y;
    func_ov208_021bdd48(touchbar, touchbar->scrollY);
    touchbar->vblankTcb = GFL_VBlankTCBAdd(func_ov208_021bdcdc, touchbar, 1);
    return touchbar;
}

void func_ov208_021bd954(ZukanDetailTouchbar *touchbar) {
    GFL_TCBRemove(touchbar->vblankTcb);
    func_ov208_021bdd24(touchbar);
    func_0204b1cc(touchbar->unit);
    GFL_HeapFree(touchbar);
}

// pokebw2's ZukanDetailTouchbar_Update
void func_ov208_021bd974(ZukanDetailTouchbar *touchbar) {
    switch (touchbar->state) {
    case ZUKAN_DETAIL_TOUCHBAR_HIDDEN:
    case ZUKAN_DETAIL_TOUCHBAR_SHOWN:
        break;
    case ZUKAN_DETAIL_TOUCHBAR_APPEARING:
        if (touchbar->wait == 0) {
            if (GFL_BGSysGetBGOffsetY(1) >= 0) {
                GFL_BGSysMoveBGReq(1, BG_MOVE_SET_Y, 0);
                touchbar->scrollY = 0;
                func_ov208_021bdd48(touchbar, touchbar->scrollY);
                touchbar->state = ZUKAN_DETAIL_TOUCHBAR_SHOWN;
                if (touchbar->active) {
                    func_ov208_021bd358(touchbar->tbar, TRUE);
                }
            } else {
                GFL_BGSysMoveBGReq(1, BG_MOVE_DOWN, data_ov208_021c8388[touchbar->speed]);
                touchbar->scrollY = touchbar->scrollY - (s16)data_ov208_021c8388[touchbar->speed];
                func_ov208_021bdd48(touchbar, touchbar->scrollY);
                touchbar->wait = data_ov208_021c8384[touchbar->speed];
            }
        } else {
            touchbar->wait--;
        }
        break;
    case ZUKAN_DETAIL_TOUCHBAR_DISAPPEARING:
        if (touchbar->wait == 0) {
            if (GFL_BGSysGetBGOffsetY(1) <= -HIDDEN_Y) {
                GFL_BGSysMoveBGReq(1, BG_MOVE_SET_Y, -HIDDEN_Y);
                touchbar->scrollY = HIDDEN_Y;
                func_ov208_021bdd48(touchbar, touchbar->scrollY);
                touchbar->state = ZUKAN_DETAIL_TOUCHBAR_HIDDEN;
            } else {
                GFL_BGSysMoveBGReq(1, BG_MOVE_UP, data_ov208_021c8388[touchbar->speed]);
                touchbar->scrollY = touchbar->scrollY + (s16)data_ov208_021c8388[touchbar->speed];
                func_ov208_021bdd48(touchbar, touchbar->scrollY);
                touchbar->wait = data_ov208_021c8384[touchbar->speed];
            }
        } else {
            touchbar->wait--;
        }
        break;
    }

    func_ov208_021bd23c(touchbar->tbar);
    if (touchbar->type == ZUKAN_DETAIL_TOUCHBAR_GENERAL) {
        func_ov208_021bdfdc(touchbar);
        func_ov208_021be20c(touchbar);
    } else if (touchbar->type == ZUKAN_DETAIL_TOUCHBAR_FORM) {
        func_ov208_021be6a8(touchbar);
    }
}

// pokebw2's ZukanDetailTouchbar_SetType
void func_ov208_021bdaac(ZukanDetailTouchbar *touchbar, int type, int page, BOOL showArrows) {
    touchbar->prevType = touchbar->type;
    touchbar->showArrows = showArrows;
    touchbar->type = type;
    touchbar->prevPage = touchbar->page;
    touchbar->page = page;
    func_ov208_021bdd24(touchbar);
    touchbar->selectedPage = touchbar->page;
    func_ov208_021bdcf8(touchbar);
}

// pokebw2's ZukanDetailTouchbar_GetState
int func_ov208_021bdad0(ZukanDetailTouchbar *touchbar) {
    return touchbar->state;
}

// pokebw2's ZukanDetailTouchbar_Appear
void func_ov208_021bdad4(ZukanDetailTouchbar *touchbar, u32 speed) {
    touchbar->speed = speed;
    if (touchbar->state != ZUKAN_DETAIL_TOUCHBAR_SHOWN) {
        touchbar->state = ZUKAN_DETAIL_TOUCHBAR_APPEARING;
        touchbar->wait = 0;
    }
}

// pokebw2's ZukanDetailTouchbar_Disappear
void func_ov208_021bdae8(ZukanDetailTouchbar *touchbar, u32 speed) {
    touchbar->speed = speed;
    if (touchbar->state != ZUKAN_DETAIL_TOUCHBAR_HIDDEN) {
        touchbar->state = ZUKAN_DETAIL_TOUCHBAR_DISAPPEARING;
        touchbar->wait = 0;
        func_ov208_021bd358(touchbar->tbar, FALSE);
    }
}

// pokebw2's ZukanDetailTouchbar_GetTrigger
int func_ov208_021bdb08(ZukanDetailTouchbar *touchbar) {
    int command;
    int icon;
    u8 i;

    icon = func_ov208_021bd2e0(touchbar->tbar);
    command = ZUKAN_DETAIL_CMD_NONE;
    if (icon != -1) {
        switch (touchbar->type) {
        case ZUKAN_DETAIL_TOUCHBAR_GENERAL:
        case ZUKAN_DETAIL_TOUCHBAR_MAP:
        case ZUKAN_DETAIL_TOUCHBAR_FORM:
            for (i = 0; i < touchbar->itemCount; i++) {
                if (icon == touchbar->items[i].icon->icon) {
                    command = touchbar->items[i].icon->trigger;
                    break;
                }
            }
            break;
        }
    }
    return command;
}

// pokebw2's ZukanDetailTouchbar_GetTouch
int func_ov208_021bdb54(ZukanDetailTouchbar *touchbar) {
    int command;
    int icon;
    u8 i;

    icon = func_ov208_021bd308(touchbar->tbar);
    command = ZUKAN_DETAIL_CMD_NONE;
    if (icon != -1) {
        switch (touchbar->type) {
        case ZUKAN_DETAIL_TOUCHBAR_GENERAL:
        case ZUKAN_DETAIL_TOUCHBAR_MAP:
        case ZUKAN_DETAIL_TOUCHBAR_FORM:
            for (i = 0; i < touchbar->itemCount; i++) {
                if (icon == touchbar->items[i].icon->icon) {
                    command = touchbar->items[i].icon->touch;
                    break;
                }
            }
            break;
        }
    }
    return command;
}

// pokebw2's ZukanDetailTouchbar_Unlock
void func_ov208_021bdba0(ZukanDetailTouchbar *touchbar) {
    func_ov208_021bd360(touchbar->tbar);
}

// pokebw2's ZukanDetailTouchbar_SetVisibleAll
void func_ov208_021bdbac(ZukanDetailTouchbar *touchbar, BOOL visible) {
    func_ov208_021bd330(touchbar->tbar, visible);
}

// pokebw2's ZukanDetailTouchbar_SetPage
void func_ov208_021bdbb8(ZukanDetailTouchbar *touchbar, int page) {
    if (touchbar->type == ZUKAN_DETAIL_TOUCHBAR_GENERAL) {
        BOOL restore = FALSE;
        int selectedPage;

        touchbar->prevPage = touchbar->page;
        touchbar->page = page;
        if (page == touchbar->selectedPage) {
            selectedPage = touchbar->selectedPage;
            restore = TRUE;
            touchbar->selectedPage = ZUKAN_DETAIL_PAGE_FORM;
        }
        func_ov208_021be1bc(touchbar);
        func_ov208_021be170(touchbar);
        if (restore) {
            touchbar->selectedPage = selectedPage;
        }
    }
}

// pokebw2's ZukanDetailTouchbar_SetFormArrowsVisible
void func_ov208_021bdbec(ZukanDetailTouchbar *touchbar, BOOL visible) {
    if (touchbar->type == ZUKAN_DETAIL_TOUCHBAR_FORM) {
        func_ov208_021bd378(touchbar->tbar, touchbar->items[FORM_CUR_R].icon->icon, visible);
        func_ov208_021bd378(touchbar->tbar, touchbar->items[FORM_CUR_L].icon->icon, visible);
    }
}

// pokebw2's ZukanDetailTouchbar_SetCheck
void func_ov208_021bdc14(ZukanDetailTouchbar *touchbar, BOOL check) {
    if (touchbar->type == ZUKAN_DETAIL_TOUCHBAR_GENERAL) {
        func_ov208_021bd3a4(touchbar->tbar, touchbar->items[GENERAL_CHECK].icon->icon, check);
    }
}

// pokebw2's ZukanDetailTouchbar_GetCheck
BOOL func_ov208_021bdc2c(ZukanDetailTouchbar *touchbar) {
    if (touchbar->type == ZUKAN_DETAIL_TOUCHBAR_GENERAL) {
        return func_ov208_021bd3b4(touchbar->tbar, touchbar->items[GENERAL_CHECK].icon->icon);
    }
    return FALSE;
}

// pokebw2's ZukanDetailTouchbar_SetActive
void func_ov208_021bdc48(ZukanDetailTouchbar *touchbar, BOOL active) {
    touchbar->active = active;
    if (active) {
        if (!func_ov208_021bd35c(touchbar->tbar) && touchbar->state == ZUKAN_DETAIL_TOUCHBAR_SHOWN) {
            func_ov208_021bd358(touchbar->tbar, TRUE);
        }
    } else {
        if (func_ov208_021bd35c(touchbar->tbar)) {
            func_ov208_021bd358(touchbar->tbar, FALSE);
        }
    }
}

// The palette of the Pokédex's own icons
// pokebw2's ZukanDetailTouchbar_GetIconPalette
u32 func_ov208_021bdc80(ZukanDetailTouchbar *touchbar) {
    switch (touchbar->type) {
    case ZUKAN_DETAIL_TOUCHBAR_GENERAL:
        return touchbar->res[ZUKAN_DETAIL_TOUCHBAR_GENERAL].palette;
    case ZUKAN_DETAIL_TOUCHBAR_MAP:
        return touchbar->res[ZUKAN_DETAIL_TOUCHBAR_MAP].palette;
    case ZUKAN_DETAIL_TOUCHBAR_FORM:
        return touchbar->res[ZUKAN_DETAIL_TOUCHBAR_FORM].palette;
    }
    return -1;
}

// Sets the priority of the bar's BG at the next VBlank, and the icons' with it
// pokebw2's ZukanDetailTouchbar_SetBGPriority
void func_ov208_021bdca4(ZukanDetailTouchbar *touchbar, u8 priority) {
    u8 i;

    touchbar->bgPriority = priority;
    touchbar->bgPriorityChanged = TRUE;
    for (i = 0; i < touchbar->itemCount; i++) {
        func_0204b84c(touchbar->items[i].actor, priority);
    }
}

// pokebw2's ZukanDetailTouchbar_VBlank
void func_ov208_021bdcdc(TCB *tcb, void *data) {
    ZukanDetailTouchbar *touchbar = data;

    if (touchbar->bgPriorityChanged) {
        GFL_BGSysSetBGPriority(1, touchbar->bgPriority);
        touchbar->bgPriorityChanged = FALSE;
    }
}

// pokebw2's ZukanDetailTouchbar_CreateType
static void func_ov208_021bdcf8(ZukanDetailTouchbar *touchbar) {
    switch (touchbar->type) {
    case ZUKAN_DETAIL_TOUCHBAR_GENERAL:
        func_ov208_021bdd98(touchbar);
        break;
    case ZUKAN_DETAIL_TOUCHBAR_MAP:
        func_ov208_021be3c4(touchbar);
        break;
    case ZUKAN_DETAIL_TOUCHBAR_FORM:
        func_ov208_021be504(touchbar);
        break;
    }
    touchbar->active = TRUE;
}

// pokebw2's ZukanDetailTouchbar_FreeType
static void func_ov208_021bdd24(ZukanDetailTouchbar *touchbar) {
    switch (touchbar->prevType) {
    case ZUKAN_DETAIL_TOUCHBAR_GENERAL:
        func_ov208_021bdfa8(touchbar);
        break;
    case ZUKAN_DETAIL_TOUCHBAR_MAP:
        func_ov208_021be4dc(touchbar);
        break;
    case ZUKAN_DETAIL_TOUCHBAR_FORM:
        func_ov208_021be680(touchbar);
        break;
    }
}

// Moves the icons with the bar's BG
// pokebw2's ZukanDetailTouchbar_SetItemsY
static void func_ov208_021bdd48(ZukanDetailTouchbar *touchbar, s16 offset) {
    u8 i;
    ClActorPos pos;

    for (i = 0; i < touchbar->itemCount; i++) {
        func_0204b53c(touchbar->items[i].actor, &pos);
        pos.y = touchbar->items[i].icon->pos.y;
        pos.y += offset;
        func_0204b528(touchbar->items[i].actor, &pos);
    }
}

void func_ov208_021bdd98(ZukanDetailTouchbar *touchbar) {
    ArcTool *arc;
    u8 i;
    ZkndTbarParam param;
    ZkndTbarIcon icons[NELEMS(data_ov208_021c8440)];

    arc = GFL_ArcSysCreateFileHandle(ARCID_ZUKAN_GRA, touchbar->heapId);
    touchbar->res[ZUKAN_DETAIL_TOUCHBAR_GENERAL].palette = func_0204ac18(arc, 3, 0, 0x160, 0, 3, touchbar->heapId);
    touchbar->res[ZUKAN_DETAIL_TOUCHBAR_GENERAL].chars = func_0204a6c8(arc, 13, FALSE, 0, touchbar->heapId);
    touchbar->res[ZUKAN_DETAIL_TOUCHBAR_GENERAL].cellAnims = func_0204af28(arc, 27, 44, touchbar->heapId);
    GFL_ArcToolFree(arc);

    touchbar->itemCount = NELEMS(data_ov208_021c8440);
    for (i = 0; i < touchbar->itemCount; i++) {
        touchbar->items[i].icon = &data_ov208_021c8440[i];
    }
    for (i = 0; i < touchbar->itemCount; i++) {
        icons[i].icon = touchbar->items[i].icon->icon;
        icons[i].pos = touchbar->items[i].icon->pos;
        icons[i].width = touchbar->items[i].icon->width;
    }

    icons[GENERAL_FORM].chars = touchbar->res[ZUKAN_DETAIL_TOUCHBAR_GENERAL].chars;
    icons[GENERAL_FORM].palette = touchbar->res[ZUKAN_DETAIL_TOUCHBAR_GENERAL].palette;
    icons[GENERAL_FORM].cellAnims = touchbar->res[ZUKAN_DETAIL_TOUCHBAR_GENERAL].cellAnims;
    icons[GENERAL_FORM].activeAnim = 3;
    icons[GENERAL_FORM].inactiveAnim = 3;
    icons[GENERAL_FORM].pushedAnim = 7;
    icons[GENERAL_FORM].key = 0;
    icons[GENERAL_FORM].se = SEQ_SE_SELECT3;
    icons[GENERAL_VOICE].chars = touchbar->res[ZUKAN_DETAIL_TOUCHBAR_GENERAL].chars;
    icons[GENERAL_VOICE].palette = touchbar->res[ZUKAN_DETAIL_TOUCHBAR_GENERAL].palette;
    icons[GENERAL_VOICE].cellAnims = touchbar->res[ZUKAN_DETAIL_TOUCHBAR_GENERAL].cellAnims;
    icons[GENERAL_VOICE].activeAnim = 2;
    icons[GENERAL_VOICE].inactiveAnim = 2;
    icons[GENERAL_VOICE].pushedAnim = 6;
    icons[GENERAL_VOICE].key = 0;
    icons[GENERAL_VOICE].se = SEQ_SE_SELECT3;
    icons[GENERAL_MAP].chars = touchbar->res[ZUKAN_DETAIL_TOUCHBAR_GENERAL].chars;
    icons[GENERAL_MAP].palette = touchbar->res[ZUKAN_DETAIL_TOUCHBAR_GENERAL].palette;
    icons[GENERAL_MAP].cellAnims = touchbar->res[ZUKAN_DETAIL_TOUCHBAR_GENERAL].cellAnims;
    icons[GENERAL_MAP].activeAnim = 1;
    icons[GENERAL_MAP].inactiveAnim = 1;
    icons[GENERAL_MAP].pushedAnim = 5;
    icons[GENERAL_MAP].key = 0;
    icons[GENERAL_MAP].se = SEQ_SE_SELECT3;
    icons[GENERAL_INFO].chars = touchbar->res[ZUKAN_DETAIL_TOUCHBAR_GENERAL].chars;
    icons[GENERAL_INFO].palette = touchbar->res[ZUKAN_DETAIL_TOUCHBAR_GENERAL].palette;
    icons[GENERAL_INFO].cellAnims = touchbar->res[ZUKAN_DETAIL_TOUCHBAR_GENERAL].cellAnims;
    icons[GENERAL_INFO].activeAnim = 0;
    icons[GENERAL_INFO].inactiveAnim = 0;
    icons[GENERAL_INFO].pushedAnim = 4;
    icons[GENERAL_INFO].key = 0;
    icons[GENERAL_INFO].se = SEQ_SE_SELECT3;

    MI_CpuFill8(&param, 0, sizeof(ZkndTbarParam));
    param.icons = icons;
    param.iconCount = NELEMS(data_ov208_021c8440);
    param.unit = touchbar->unit;
    param.bg = 1;
    param.bgPalette = 13;
    param.objPalette = 8;
    param.mapping = 2;
    touchbar->tbar = func_ov208_021bd148(&param, touchbar->heapId);
    for (i = 0; i < touchbar->itemCount; i++) {
        touchbar->items[i].actor = func_ov208_021bd3c0(touchbar->tbar, touchbar->items[i].icon->icon);
    }

    if (!touchbar->showFormTab) {
        for (i = 0; i < ITEM_MAX; i++) {
            func_ov208_021bd3cc(touchbar->tbar, touchbar->items[i].icon->icon, &data_ov208_021c83a4[i]);
        }
        func_ov208_021bd378(touchbar->tbar, touchbar->items[GENERAL_FORM].icon->icon, FALSE);
    }

    i = func_ov208_021be300(touchbar->page);
    if (i != ITEM_NONE) {
        func_ov208_021bd368(touchbar->tbar, touchbar->items[i].icon->icon, FALSE);
    }
    func_ov208_021be128(touchbar);
    func_ov208_021be170(touchbar);
    if (!touchbar->showArrows) {
        func_ov208_021bd378(touchbar->tbar, touchbar->items[GENERAL_CUR_U].icon->icon, FALSE);
        func_ov208_021bd378(touchbar->tbar, touchbar->items[GENERAL_CUR_D].icon->icon, FALSE);
    }
}


// pokebw2's ZukanDetailTouchbar_FreeGeneral
static void func_ov208_021bdfa8(ZukanDetailTouchbar *touchbar) {
    if (touchbar->tbar != NULL) {
        func_ov208_021be1bc(touchbar);
        func_ov208_021be16c(touchbar);
        func_ov208_021bd208(touchbar->tbar);
        func_0204ada4(touchbar->res[ZUKAN_DETAIL_TOUCHBAR_GENERAL].palette);
        func_0204a8d4(touchbar->res[ZUKAN_DETAIL_TOUCHBAR_GENERAL].chars);
        func_0204afd8(touchbar->res[ZUKAN_DETAIL_TOUCHBAR_GENERAL].cellAnims);
    }
    touchbar->tbar = NULL;
}

void func_ov208_021bdfdc(ZukanDetailTouchbar *touchbar) {
    u8 item = ITEM_NONE;
    u32 pressed = GCTX_HIDGetPressedKeys();
    u32 typed = GCTX_HIDGetTypedKeys();
    int trigger;
    int touch;

    if (typed & PAD_KEY_LEFT) {
        switch (touchbar->page) {
        case ZUKAN_DETAIL_PAGE_INFO - 1:
            if (func_ov208_021bd388(touchbar->tbar, touchbar->items[GENERAL_FORM].icon->icon)) {
                item = GENERAL_FORM;
            } else {
                item = GENERAL_VOICE;
            }
            break;
        case ZUKAN_DETAIL_PAGE_MAP - 1:
            item = GENERAL_INFO;
            break;
        case ZUKAN_DETAIL_PAGE_VOICE - 1:
            item = GENERAL_MAP;
            break;
        case ZUKAN_DETAIL_PAGE_FORM - 1:
            item = GENERAL_VOICE;
            break;
        }
    } else if (typed & PAD_KEY_RIGHT) {
        switch (touchbar->page) {
        case ZUKAN_DETAIL_PAGE_INFO - 1:
            item = GENERAL_MAP;
            break;
        case ZUKAN_DETAIL_PAGE_MAP - 1:
            item = GENERAL_VOICE;
            break;
        case ZUKAN_DETAIL_PAGE_VOICE - 1:
            if (func_ov208_021bd388(touchbar->tbar, touchbar->items[GENERAL_FORM].icon->icon)) {
                item = GENERAL_FORM;
            } else {
                item = GENERAL_INFO;
            }
            break;
        case ZUKAN_DETAIL_PAGE_FORM - 1:
            item = GENERAL_INFO;
            break;
        }
    }
    if (item != ITEM_NONE) {
        func_ov208_021bd3f8(touchbar->tbar, touchbar->items[item].icon->icon);
    }

    if (!(pressed & (PAD_KEY_UP | PAD_KEY_DOWN))) {
        if (typed & PAD_KEY_UP) {
            func_ov208_021bd3f8(touchbar->tbar, touchbar->items[GENERAL_CUR_U].icon->icon);
        } else if (typed & PAD_KEY_DOWN) {
            func_ov208_021bd3f8(touchbar->tbar, touchbar->items[GENERAL_CUR_D].icon->icon);
        }
    }

    trigger = func_ov208_021bdb08(touchbar);
    touch = func_ov208_021bdb54(touchbar);
    if (trigger != ZUKAN_DETAIL_CMD_NONE) {
        u8 pageItem = func_ov208_021be32c(trigger);
        u8 page = func_ov208_021be378(trigger);

        if (pageItem != ITEM_NONE && page != ZUKAN_DETAIL_PAGE_FORM) {
            for (item = GENERAL_FORM; item <= GENERAL_INFO; item++) {
                func_ov208_021bd368(touchbar->tbar, touchbar->items[item].icon->icon, item == pageItem ? FALSE : TRUE);
            }
            touchbar->selectedPage = page;
            func_ov208_021be170(touchbar);
        }
    }
    if (touch != ZUKAN_DETAIL_CMD_NONE) {
        switch (touch) {
        case ZUKAN_DETAIL_CMD_INFO_TOUCH:
        case ZUKAN_DETAIL_CMD_MAP_TOUCH:
        case ZUKAN_DETAIL_CMD_VOICE_TOUCH:
        case ZUKAN_DETAIL_CMD_FORM_TOUCH:
            func_ov208_021be1bc(touchbar);
            break;
        }
    }
}


// Reads the two palettes that the selected tab glows between
// pokebw2's ZukanDetailTouchbar_InitGlow
void func_ov208_021be128(ZukanDetailTouchbar *touchbar) {
    NNSG2dPaletteData *palette;
    void *file = GFL_G2DIOReadNCLR(ARCID_ZUKAN_GRA, 3, &palette, touchbar->heapId);
    u16 *data = palette->pRawData;

    MI_CpuCopy8(data + 16, touchbar->glowPalettes[0], 32);
    MI_CpuCopy8(data + 48, touchbar->glowPalettes[1], 32);
    GFL_HeapFree(file);
    touchbar->glowPhase = 0;
}

// pokebw2's ZukanDetailTouchbar_FreeGlow
static void func_ov208_021be16c(ZukanDetailTouchbar *touchbar) {
}

// pokebw2's ZukanDetailTouchbar_StartGlow
static void func_ov208_021be170(ZukanDetailTouchbar *touchbar) {
    u8 item = ITEM_NONE;

    switch (touchbar->selectedPage) {
    case ZUKAN_DETAIL_PAGE_INFO - 1:
        item = GENERAL_INFO;
        break;
    case ZUKAN_DETAIL_PAGE_MAP - 1:
        item = GENERAL_MAP;
        break;
    case ZUKAN_DETAIL_PAGE_VOICE - 1:
        item = GENERAL_VOICE;
        break;
    case ZUKAN_DETAIL_PAGE_FORM - 1:
        item = GENERAL_FORM;
        break;
    }
    if (item != ITEM_NONE) {
        func_0204b6f4(func_ov208_021bd3c0(touchbar->tbar, touchbar->items[item].icon->icon), 2, 0);
    }
}

// pokebw2's ZukanDetailTouchbar_StopGlow
static void func_ov208_021be1bc(ZukanDetailTouchbar *touchbar) {
    u8 item = ITEM_NONE;

    switch (touchbar->selectedPage) {
    case ZUKAN_DETAIL_PAGE_INFO - 1:
        item = GENERAL_INFO;
        break;
    case ZUKAN_DETAIL_PAGE_MAP - 1:
        item = GENERAL_MAP;
        break;
    case ZUKAN_DETAIL_PAGE_VOICE - 1:
        item = GENERAL_VOICE;
        break;
    case ZUKAN_DETAIL_PAGE_FORM - 1:
        item = GENERAL_FORM;
        break;
    }
    if (item != ITEM_NONE) {
        func_0204b6f4(func_ov208_021bd3c0(touchbar->tbar, touchbar->items[item].icon->icon), 0, 0);
        touchbar->selectedPage = ZUKAN_DETAIL_PAGE_FORM;
    }
}

// Blends the glowing palette between the two palettes with a cosine, and loads it into OBJ palette 13
// pokebw2's ZukanDetailTouchbar_UpdateGlow
static void func_ov208_021be20c(ZukanDetailTouchbar *touchbar) {
    s16 ratio;
    u8 i;

    if (touchbar->glowPhase + 0x400 >= 0x10000) {
        touchbar->glowPhase = touchbar->glowPhase + 0x400 - 0x10000;
    } else {
        touchbar->glowPhase += 0x400;
    }
    ratio = (FX_CosIdx(touchbar->glowPhase) + FX32_ONE) / 2;

    for (i = 0; i < 16; i++) {
        u16 from = touchbar->glowPalettes[0][i];
        u16 to = touchbar->glowPalettes[1][i];
        u8 r0 = from & 0x1f;
        u8 g0 = (from & 0x3e0) >> 5;
        u8 b0 = (from & 0x7c00) >> 10;
        u8 r1 = to & 0x1f;
        u8 g1 = (to & 0x3e0) >> 5;
        u8 b1 = (to & 0x7c00) >> 10;
        u8 r = r0 + (((r1 - r0) * ratio) >> FX32_SHIFT);
        u8 g = g0 + (((g1 - g0) * ratio) >> FX32_SHIFT);
        u8 b = b0 + (((b1 - b0) * ratio) >> FX32_SHIFT);

        touchbar->glowPalette[i] = r | (g << 5) | (b << 10);
    }
    NNS_GfdRegisterNewVramTransferTask(14, 13 * 32, touchbar->glowPalette, 32);
}

// pokebw2's ZukanDetailTouchbar_GetPageItem
u8 func_ov208_021be300(int page) {
    switch (page) {
    case ZUKAN_DETAIL_PAGE_INFO - 1:
        return GENERAL_INFO;
    case ZUKAN_DETAIL_PAGE_MAP - 1:
        return GENERAL_MAP;
    case ZUKAN_DETAIL_PAGE_VOICE - 1:
        return GENERAL_VOICE;
    case ZUKAN_DETAIL_PAGE_FORM - 1:
        return GENERAL_FORM;
    }
    return ITEM_NONE;
}

// pokebw2's ZukanDetailTouchbar_GetCommandItem
u8 func_ov208_021be32c(int command) {
    switch (command) {
    case ZUKAN_DETAIL_CMD_INFO:
    case ZUKAN_DETAIL_CMD_INFO_TOUCH:
        return GENERAL_INFO;
    case ZUKAN_DETAIL_CMD_MAP:
    case ZUKAN_DETAIL_CMD_MAP_TOUCH:
        return GENERAL_MAP;
    case ZUKAN_DETAIL_CMD_VOICE:
    case ZUKAN_DETAIL_CMD_VOICE_TOUCH:
        return GENERAL_VOICE;
    case ZUKAN_DETAIL_CMD_FORM:
    case ZUKAN_DETAIL_CMD_FORM_TOUCH:
        return GENERAL_FORM;
    }
    return ITEM_NONE;
}

// pokebw2's ZukanDetailTouchbar_GetCommandPage
u8 func_ov208_021be378(int command) {
    switch (command) {
    case ZUKAN_DETAIL_CMD_INFO:
    case ZUKAN_DETAIL_CMD_INFO_TOUCH:
        return ZUKAN_DETAIL_PAGE_INFO - 1;
    case ZUKAN_DETAIL_CMD_MAP:
    case ZUKAN_DETAIL_CMD_MAP_TOUCH:
        return ZUKAN_DETAIL_PAGE_MAP - 1;
    case ZUKAN_DETAIL_CMD_VOICE:
    case ZUKAN_DETAIL_CMD_VOICE_TOUCH:
        return ZUKAN_DETAIL_PAGE_VOICE - 1;
    case ZUKAN_DETAIL_CMD_FORM:
    case ZUKAN_DETAIL_CMD_FORM_TOUCH:
        return ZUKAN_DETAIL_PAGE_FORM - 1;
    }
    return ZUKAN_DETAIL_PAGE_FORM;
}

static void func_ov208_021be3c4(ZukanDetailTouchbar *touchbar) {
    ArcTool *arc;
    u8 i;
    ZkndTbarParam param;
    ZkndTbarIcon icons[NELEMS(data_ov208_021c8390)];

    arc = GFL_ArcSysCreateFileHandle(ARCID_TOWNMAP_GRA, touchbar->heapId);
    touchbar->res[ZUKAN_DETAIL_TOUCHBAR_MAP].palette = func_0204ac18(arc, 2, 0, 0x160, 2, 3, touchbar->heapId);
    touchbar->res[ZUKAN_DETAIL_TOUCHBAR_MAP].chars = func_0204a6c8(arc, 8, FALSE, 0, touchbar->heapId);
    touchbar->res[ZUKAN_DETAIL_TOUCHBAR_MAP].cellAnims = func_0204af28(arc, 12, 19, touchbar->heapId);
    GFL_ArcToolFree(arc);

    touchbar->itemCount = NELEMS(data_ov208_021c8390);
    for (i = 0; i < touchbar->itemCount; i++) {
        touchbar->items[i].icon = &data_ov208_021c8390[i];
    }
    for (i = 0; i < touchbar->itemCount; i++) {
        icons[i].icon = touchbar->items[i].icon->icon;
        icons[i].pos = touchbar->items[i].icon->pos;
        icons[i].width = touchbar->items[i].icon->width;
    }

    MI_CpuFill8(&param, 0, sizeof(ZkndTbarParam));
    param.icons = icons;
    param.iconCount = NELEMS(data_ov208_021c8390);
    param.unit = touchbar->unit;
    param.bg = 1;
    param.bgPalette = 13;
    param.objPalette = 8;
    param.mapping = 2;
    touchbar->tbar = func_ov208_021bd148(&param, touchbar->heapId);

    for (i = 0; i < touchbar->itemCount; i++) {
        touchbar->items[i].actor = func_ov208_021bd3c0(touchbar->tbar, touchbar->items[i].icon->icon);
    }
}


void func_ov208_021be4dc(ZukanDetailTouchbar *touchbar) {
    if (touchbar->tbar != NULL) {
        func_ov208_021bd208(touchbar->tbar);
        func_0204ada4(touchbar->res[ZUKAN_DETAIL_TOUCHBAR_MAP].palette);
        func_0204a8d4(touchbar->res[ZUKAN_DETAIL_TOUCHBAR_MAP].chars);
        func_0204afd8(touchbar->res[ZUKAN_DETAIL_TOUCHBAR_MAP].cellAnims);
    }
    touchbar->tbar = NULL;
}

void func_ov208_021be504(ZukanDetailTouchbar *touchbar) {
    ArcTool *arc;
    u8 i;
    ZkndTbarParam param;
    ZkndTbarIcon icons[NELEMS(data_ov208_021c83c8)];
    ZkndTbarIcon *button;

    arc = GFL_ArcSysCreateFileHandle(ARCID_ZUKAN_GRA, touchbar->heapId);
    touchbar->res[ZUKAN_DETAIL_TOUCHBAR_FORM].palette = func_0204ac18(arc, 3, 0, 0x160, 0, 3, touchbar->heapId);
    touchbar->res[ZUKAN_DETAIL_TOUCHBAR_FORM].chars = func_0204a6c8(arc, 13, FALSE, 0, touchbar->heapId);
    touchbar->res[ZUKAN_DETAIL_TOUCHBAR_FORM].cellAnims = func_0204af28(arc, 27, 44, touchbar->heapId);
    GFL_ArcToolFree(arc);

    touchbar->itemCount = NELEMS(data_ov208_021c83c8);
    for (i = 0; i < touchbar->itemCount; i++) {
        touchbar->items[i].icon = &data_ov208_021c83c8[i];
    }
    for (i = 0; i < touchbar->itemCount; i++) {
        icons[i].icon = touchbar->items[i].icon->icon;
        icons[i].pos = touchbar->items[i].icon->pos;
        icons[i].width = touchbar->items[i].icon->width;
    }
    // The forms' button is the Pokédex's own icon
    button = &icons[FORM_BUTTON];
    button->chars = touchbar->res[ZUKAN_DETAIL_TOUCHBAR_FORM].chars;
    button->palette = touchbar->res[ZUKAN_DETAIL_TOUCHBAR_FORM].palette;
    button->cellAnims = touchbar->res[ZUKAN_DETAIL_TOUCHBAR_FORM].cellAnims;
    button->activeAnim = 14;
    button->inactiveAnim = 14;
    button->pushedAnim = 20;
    button->key = PAD_BUTTON_A;
    button->se = SEQ_SE_DECIDE1;

    MI_CpuFill8(&param, 0, sizeof(ZkndTbarParam));
    param.icons = icons;
    param.iconCount = NELEMS(data_ov208_021c83c8);
    param.unit = touchbar->unit;
    param.bg = 1;
    param.bgPalette = 13;
    param.objPalette = 8;
    param.mapping = 2;
    touchbar->tbar = func_ov208_021bd148(&param, touchbar->heapId);

    for (i = 0; i < touchbar->itemCount; i++) {
        touchbar->items[i].actor = func_ov208_021bd3c0(touchbar->tbar, touchbar->items[i].icon->icon);
    }

    func_ov208_021bd394(touchbar->tbar, touchbar->items[FORM_CUR_R].icon->icon, PAD_KEY_RIGHT | PAD_BUTTON_R);
    func_ov208_021bd394(touchbar->tbar, touchbar->items[FORM_CUR_L].icon->icon, PAD_KEY_LEFT | PAD_BUTTON_L);
    if (!touchbar->showArrows) {
        func_ov208_021bd378(touchbar->tbar, touchbar->items[FORM_CUR_U].icon->icon, FALSE);
        func_ov208_021bd378(touchbar->tbar, touchbar->items[FORM_CUR_D].icon->icon, FALSE);
    }
}


void func_ov208_021be680(ZukanDetailTouchbar *touchbar) {
    if (touchbar->tbar != NULL) {
        func_ov208_021bd208(touchbar->tbar);
        func_0204ada4(touchbar->res[ZUKAN_DETAIL_TOUCHBAR_FORM].palette);
        func_0204a8d4(touchbar->res[ZUKAN_DETAIL_TOUCHBAR_FORM].chars);
        func_0204afd8(touchbar->res[ZUKAN_DETAIL_TOUCHBAR_FORM].cellAnims);
    }
    touchbar->tbar = NULL;
}

// pokebw2's ZukanDetailTouchbar_UpdateForm
void func_ov208_021be6a8(ZukanDetailTouchbar *touchbar) {
    u32 pressed = GCTX_HIDGetPressedKeys();
    u32 typed = GCTX_HIDGetTypedKeys();

    if ((pressed & (0x3f << 4)) == 0) {
        if (typed & 0x40) {
            func_ov208_021bd3f8(touchbar->tbar, touchbar->items[FORM_CUR_U].icon->icon);
            return;
        }
        if (typed & 0x80) {
            func_ov208_021bd3f8(touchbar->tbar, touchbar->items[FORM_CUR_D].icon->icon);
            return;
        }
        if (typed & (0x22 << 4)) {
            func_ov208_021bd3f8(touchbar->tbar, touchbar->items[FORM_CUR_L].icon->icon);
            return;
        }
        if (typed & (0x11 << 4)) {
            func_ov208_021bd3f8(touchbar->tbar, touchbar->items[FORM_CUR_R].icon->icon);
        }
    }
}




