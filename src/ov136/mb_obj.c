// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov215/mb_obj.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/mailbox_local.h"
#include "constants/arc.h"
#include "gfl/arc.h"
#include "gfl/clact.h"
#include "gfl/gx_layers.h"
#include "nitro/gx.h"
#include "pml/poke_party.h"
#include "system/app_menu_common.h"

// The mailbox screen's cell actors. The ROM doesn't name this file; the name is a guess, after box2_obj.c

// Black 2's; twinned code confirms the offsets of x, y, sequence, surface, charIndex, paletteIndex, cellAnimIndex and
// bgPriority in Black
typedef struct {
    s16 x;
    s16 y;
    s16 unk4;
    u16 sequence;
    u32 unk8;
    u32 unkC;
    u32 surface;
    u32 charIndex;
    u32 paletteIndex;
    u32 cellAnimIndex;
    u32 unk20;
    u32 unk24;
    u32 unk28;
    u32 bgPriority;
    u32 unk30;
} MailBoxActorTemplate;

static void func_ov136_021c65a8(MailBoxView *view);
static void func_ov136_021c65e4(MailBoxView *view);
static ClActor *func_ov136_021c660c(MailBoxView *view, const MailBoxActorTemplate *template);
static void func_ov136_021c666c(MailBoxView *view, u32 index);
static void func_ov136_021c6688(MailBoxView *view);
static void func_ov136_021c6738(MailBoxView *view);
static void func_ov136_021c674c(MailBoxView *view);
static void func_ov136_021c675c(MailBoxView *view);
static void func_ov136_021c67d4(MailBoxView *view);

// pokebw2's sClActSetup
static const ClActSysSetup data_ov136_021c7ba0 = { 0, 0, 0, 512, 4, 64, 4, 64, 0, 6, 4, 4, 32, 16, 16 };

// pokebw2's sActorTemplates
static const MailBoxActorTemplate data_ov136_021c7bbc[MAILBOX_ACTOR_COUNT] = {
#include "asset/mb_obj_data_ov136_021c7bbc.inc"
};

// pokebw2's MailBox_CreateOBJs
void func_ov136_021c64a8(MailBoxWork *work) {
    u32 i;

    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
    for (i = 0; i < 8; i++) {
        work->view->chars[i] = 0;
        work->view->palettes[i] = 0;
        work->view->cellAnims[i] = 0;
    }
    func_ov136_021c6738(work->view);
    func_ov136_021c65a8(work->view);
    func_ov136_021c67d4(work->view);
    func_ov136_021c675c(work->view);
    for (i = 0; i < 8; i++) {
        work->view->actors[i] = func_ov136_021c660c(work->view, &data_ov136_021c7bbc[i]);
        func_0204b92c(work->view->actors[i], TRUE);
    }
    if (work->view->mailCount <= MAILBOX_PAGE_SIZE) {
        func_ov136_021c66b8(work->view, MAILBOX_ACTOR_PREV_PAGE, FALSE);
        func_ov136_021c66b8(work->view, MAILBOX_ACTOR_NEXT_PAGE, FALSE);
    } else if (work->page == 0) {
        func_ov136_021c66b8(work->view, MAILBOX_ACTOR_PREV_PAGE, FALSE);
    } else {
        func_ov136_021c66b8(work->view, MAILBOX_ACTOR_NEXT_PAGE, FALSE);
    }
    func_ov136_021c66b8(work->view, 4, FALSE);
    func_ov136_021c66b8(work->view, 5, FALSE);
    func_ov136_021c66b8(work->view, 6, FALSE);
}

// pokebw2's MailBox_FreeOBJs
void func_ov136_021c657c(MailBoxWork *work) {
    func_ov136_021c6688(work->view);
    func_ov136_021c65e4(work->view);
    func_ov136_021c674c(work->view);
}

// pokebw2's MailBox_UpdateClActUnit
void func_ov136_021c6594(MailBoxView *view) {
    if (view->clActUnit != NULL) {
        func_0204a600();
    }
}

// pokebw2's MailBox_InitClAct
static void func_ov136_021c65a8(MailBoxView *view) {
    ClActSys_Create(&data_ov136_021c7ba0, &data_ov136_021c78e0, HEAPID_MAILBOX_VIEW);
    view->clActUnit = func_0204b100(8, 0, HEAPID_MAILBOX_VIEW);
    func_0204b270(view->clActUnit);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
}

// pokebw2's MailBox_FreeClAct
static void func_ov136_021c65e4(MailBoxView *view) {
    // BUG: the palette and the cell animations are freed with each other's function. Both are resource 0, so it
    // works
#ifdef BUGFIX
    func_0204afd8(view->cellAnims[0]);
    func_0204ada4(view->palettes[0]);
#else
    func_0204afd8(view->palettes[0]);
    func_0204ada4(view->cellAnims[0]);
#endif
    func_0204b1cc(view->clActUnit);
    func_0204a5ac();
}

// pokebw2's MailBox_CreateActor
static ClActor *func_ov136_021c660c(MailBoxView *view, const MailBoxActorTemplate *template) {
    ClActorSetup setup;

    setup.x = template->x;
    setup.y = template->y;
    setup.sequence = template->sequence;
    setup.priority = 0;
    setup.bgPriority = template->bgPriority;
    return func_0204b294(view->clActUnit, view->chars[template->charIndex], view->palettes[template->paletteIndex],
                         view->cellAnims[template->cellAnimIndex], &setup, template->surface,
                         HEAPID_MAILBOX_VIEW);
}

// pokebw2's MailBox_FreeActor
static void func_ov136_021c666c(MailBoxView *view, u32 index) {
    if (view->actors[index] != NULL) {
        func_0204b3b4(view->actors[index]);
        view->actors[index] = NULL;
    }
}

// pokebw2's MailBox_FreeActors
static void func_ov136_021c6688(MailBoxView *view) {
    u32 i;

    for (i = 0; i < MAILBOX_ACTOR_COUNT; i++) {
        func_ov136_021c666c(view, i);
    }
}

// pokebw2's MailBox_SetActorAnim
void func_ov136_021c66a0(MailBoxView *view, u32 index, u32 sequence) {
    func_0204b878(view->actors[index], sequence);
}

// pokebw2's MailBox_SetActorVisible
void func_ov136_021c66b8(MailBoxView *view, u32 index, BOOL visible) {
    func_0204b3dc(view->actors[index], visible);
}

// pokebw2's MailBox_SetActorPos
void func_ov136_021c66cc(MailBoxView *view, u32 index, int x, int y) {
    ClActorPos pos;

    pos.x = x;
    pos.y = y;
    if (y < 256) {
        func_0204b404(view->actors[index], &pos, 0);
    } else {
        func_0204b404(view->actors[index], &pos, 1);
    }
    func_0204b8c4(view->actors[index], 0);
}

// pokebw2's MailBox_SetActorBlend
void func_ov136_021c670c(MailBoxView *view, u32 index, BOOL blend) {
    if (blend == TRUE) {
        func_0204b688(view->actors[index], GX_OAM_MODE_XLU);
    } else {
        func_0204b688(view->actors[index], GX_OAM_MODE_NORMAL);
    }
}

// pokebw2's MailBox_OpenIconArc
static void func_ov136_021c6738(MailBoxView *view) {
    view->iconArc = GFL_ArcSysCreateFileHandle(ARCID_POKEICON, HEAPID_MAILBOX_VIEW);
}

// pokebw2's MailBox_CloseIconArc
static void func_ov136_021c674c(MailBoxView *view) {
    GFL_ArcToolFree(view->iconArc);
}

// pokebw2's MailBox_LoadActorResources
static void func_ov136_021c675c(MailBoxView *view) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(getUINarcIdx(), HEAPID_MAILBOX_VIEW);
    u32 i;

    for (i = 0; i <= 2; i++) {
        view->chars[i] = func_0204a6c8(arc, func_02027698(), FALSE, TRUE, HEAPID_MAILBOX_VIEW);
    }
    view->palettes[0] = func_0204abf0(arc, func_0201bc3c(), 1, 0, HEAPID_MAILBOX_VIEW);
    view->cellAnims[0] = func_0204af28(arc, func_0202769c(0), func_020276a0(0), HEAPID_MAILBOX_VIEW);
    GFL_ArcToolFree(arc);
}

// pokebw2's MailBox_LoadButtonResources
static void func_ov136_021c67d4(MailBoxView *view) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_MAILBOX, HEAPID_MAILBOX_VIEW);

    view->chars[3] = func_0204a6c8(arc, 44, TRUE, 0, HEAPID_MAILBOX_VIEW);
    view->palettes[1] = func_0204abf0(arc, 47, 0, 0, HEAPID_MAILBOX_VIEW);
    view->cellAnims[1] = func_0204af28(arc, 45, 46, HEAPID_MAILBOX_VIEW);
    GFL_ArcToolFree(arc);
    arc = GFL_ArcSysCreateFileHandle(getUINarcIdx(), HEAPID_MAILBOX_VIEW);
    view->chars[4] = func_0204a6c8(arc, func_02027618(), FALSE, 0, HEAPID_MAILBOX_VIEW);
    view->palettes[2] = func_0204abf0(arc, func_02027614(), 0, 0x80, HEAPID_MAILBOX_VIEW);
    view->cellAnims[2] = func_0204af28(arc, func_0202761c(0), func_02027620(0), HEAPID_MAILBOX_VIEW);
    GFL_ArcToolFree(arc);
}

// pokebw2's MailBox_ShowReadCursor
void func_ov136_021c6884(MailBoxView *view) {
    func_ov136_021c66cc(view, MAILBOX_ACTOR_CURSOR, 224, 176);
    func_ov136_021c66a0(view, MAILBOX_ACTOR_CURSOR, 4);
}
