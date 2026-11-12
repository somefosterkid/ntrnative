// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov271/br_sidebar.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The Battle Recorder's sidebars: the twelve strips along the edges of the two screens, which slide in shaking,
// sway in place and slide back out between its screens. The name is the ROM's string, which GFL_HeapAllocate is
// given.

#include "types.h"
#include "app/battle_recorder/br_sidebar.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "gfl/math_util.h"
#include "gfl/std.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "app/battle_recorder/br_res.h"

ClActor *func_0204b294(ClActUnit *unit, u32 chars, u32 palette, u32 cellAnims, const ClActorSetup *setup, u16 surface, HeapID heapId);

#define BR_SIDEBAR_MAX 12
// The scale the sidebars shrink to, and the step they shrink by
#define BR_SIDEBAR_SCALE_END 0x19a

// Where a sidebar starts and how it moves
typedef struct {
    s16 x;
    s16 y;
    s16 scale;
    // The steps of a slide
    s16 sync;
    s16 end_x;
    s16 end_y;
    // The angle the sway starts at, and how far it turns each step, in degrees
    s16 theta;
    s16 theta_add;
    u32 display;
    // Which way it shakes and sways
    BOOL dir;
} BrSidebarData;

typedef struct BrSidebarWork BrSidebarWork;

typedef void (*BrSidebarMoveFunc)(BrSidebarWork *p_wk);

struct BrSidebarWork {
    BOOL is_use;
    ClActor *clwk;
    fx32 x;
    fx32 y;
    fx32 now_x;
    fx32 scale;
    BrSidebarMoveFunc move_func;
    u32 seq;
    s32 theta;
    s32 cnt;
    s32 wait;
    s32 dx;
    fx32 dscale;
    BrSidebarData data;
};

struct BrSidebar {
    BrSidebarWork wk[BR_SIDEBAR_MAX];
};

// How a sidebar moves
enum {
    BR_SIDEBAR_MOVE_NONE,
    BR_SIDEBAR_MOVE_OPEN,
    BR_SIDEBAR_MOVE_BOUND,
    BR_SIDEBAR_MOVE_CLOSE,
};

static void func_ov186_021feef4(BrSidebarWork *p_wk, ClActUnit *unit, BrRes *res, const BrSidebarData *cp_data,
                               HeapID heapId);
static void func_ov186_021ff004(BrSidebarWork *p_wk);
static void func_ov186_021ff01c(BrSidebarWork *p_wk);
static BOOL func_ov186_021ff028(const BrSidebarWork *cp_wk);
static void func_ov186_021ff02c(BrSidebarWork *p_wk, BOOL isVisible);
static void func_ov186_021ff038(BrSidebarWork *p_wk, u32 type);
static void func_ov186_021ff074(BrSidebarWork *p_wk);
static void func_ov186_021ff0a0(BrSidebarWork *p_wk);
static void func_ov186_021ff248(BrSidebarWork *p_wk);
static void func_ov186_021ff334(BrSidebarWork *p_wk);

// pokebw2's sc_sidebar_data
static const BrSidebarData data_ov186_02200b74[BR_SIDEBAR_MAX] = {
    { 82, 96, 0x1666, 6, 6, 96, 90, 4, CLACT_SURFACE_MAIN, FALSE },
    { 178, 96, 0x1ccd, 4, 5, 96, 135, -4, CLACT_SURFACE_MAIN, FALSE },
    { 118, 96, 0x2000, 14, 10, 96, 270, 4, CLACT_SURFACE_MAIN, FALSE },
    { 42, 96, 0x199a, 8, 246, 96, 90, 4, CLACT_SURFACE_MAIN, TRUE },
    { 150, 96, 0x1666, 4, 244, 96, 135, -4, CLACT_SURFACE_MAIN, TRUE },
    { 210, 96, 0x199a, 14, 249, 96, 270, 4, CLACT_SURFACE_MAIN, TRUE },
    { 82, 96, 0x1666, 6, 6, 96, 90, 4, CLACT_SURFACE_SUB, FALSE },
    { 178, 96, 0x1ccd, 4, 5, 96, 135, -4, CLACT_SURFACE_SUB, FALSE },
    { 118, 96, 0x2000, 14, 10, 96, 270, 4, CLACT_SURFACE_SUB, FALSE },
    { 42, 96, 0x199a, 8, 246, 96, 90, 4, CLACT_SURFACE_SUB, TRUE },
    { 150, 96, 0x1666, 4, 244, 96, 135, -4, CLACT_SURFACE_SUB, TRUE },
    { 210, 96, 0x199a, 14, 249, 96, 270, 4, CLACT_SURFACE_SUB, TRUE },
};

// pokebw2's BrSidebar_Init
BrSidebar *func_ov186_021fed50(ClActUnit *unit, BrFade *fade, BrRes *res, HeapID heapId) {
    BrSidebar *p_wk;
    int i;

    p_wk = GFL_HeapAllocate(heapId, sizeof(BrSidebar), FALSE, "br_sidebar.c", 306);
    MI_CpuFill8(p_wk, 0, sizeof(BrSidebar));
    func_ov186_021fbe54(res, BR_RES_OBJ_SIDEBAR_M, heapId);
    func_ov186_021fbe54(res, BR_RES_OBJ_SIDEBAR_S, heapId);
    for (i = 0; i < BR_SIDEBAR_MAX; i++) {
        func_ov186_021feef4(&p_wk->wk[i], unit, res, &data_ov186_02200b74[i], heapId);
    }
    return p_wk;
}

// pokebw2's BrSidebar_Exit
void func_ov186_021fedc4(BrSidebar *p_wk, BrRes *res) {
    int i;

    for (i = 0; i < BR_SIDEBAR_MAX; i++) {
        if (func_ov186_021ff028(&p_wk->wk[i])) {
            func_ov186_021ff004(&p_wk->wk[i]);
        }
    }
    func_ov186_021fc0c8(res, BR_RES_OBJ_SIDEBAR_M);
    func_ov186_021fc0c8(res, BR_RES_OBJ_SIDEBAR_S);
    GFL_HeapFree(p_wk);
}

// pokebw2's BrSidebar_Main
void func_ov186_021fee00(BrSidebar *p_wk) {
    int i;

    for (i = 0; i < BR_SIDEBAR_MAX; i++) {
        if (func_ov186_021ff028(&p_wk->wk[i])) {
            func_ov186_021ff01c(&p_wk->wk[i]);
        }
    }
}

// pokebw2's BrSidebar_StartOpen
void func_ov186_021fee24(BrSidebar *p_wk) {
    int i;

    for (i = 0; i < BR_SIDEBAR_MAX; i++) {
        if (func_ov186_021ff028(&p_wk->wk[i])) {
            func_ov186_021ff038(&p_wk->wk[i], BR_SIDEBAR_MOVE_OPEN);
        }
    }
}

// pokebw2's BrSidebar_StartBound
void func_ov186_021fee4c(BrSidebar *p_wk) {
    int i;

    for (i = 0; i < BR_SIDEBAR_MAX; i++) {
        if (func_ov186_021ff028(&p_wk->wk[i])) {
            func_ov186_021ff038(&p_wk->wk[i], BR_SIDEBAR_MOVE_BOUND);
        }
    }
}

// pokebw2's BrSidebar_StartClose
void func_ov186_021fee74(BrSidebar *p_wk) {
    int i;

    for (i = 0; i < BR_SIDEBAR_MAX; i++) {
        if (func_ov186_021ff028(&p_wk->wk[i])) {
            func_ov186_021ff038(&p_wk->wk[i], BR_SIDEBAR_MOVE_CLOSE);
        }
    }
}

// pokebw2's BrSidebar_SetEndPos
void func_ov186_021fee9c(BrSidebar *p_wk) {
    int i;

    for (i = 0; i < BR_SIDEBAR_MAX; i++) {
        if (func_ov186_021ff028(&p_wk->wk[i])) {
            func_ov186_021ff074(&p_wk->wk[i]);
        }
    }
}

// pokebw2's BrSidebar_SetVisible
void func_ov186_021feec0(BrSidebar *p_wk, u32 display, BOOL isVisible) {
    int i;

    for (i = 0; i < BR_SIDEBAR_MAX; i++) {
        if (display == data_ov186_02200b74[i].display) {
            func_ov186_021ff02c(&p_wk->wk[i], isVisible);
        }
    }
}

static void func_ov186_021feef4(BrSidebarWork *p_wk, ClActUnit *unit, BrRes *res, const BrSidebarData *cp_data,
                               HeapID heapId) {
    ClActorSetup setup;
    BrResObjData obj;
    BOOL ret;
    u32 objID;

    MI_CpuFill8(p_wk, 0, sizeof(BrSidebarWork));
    p_wk->data = *cp_data;
    objID = BR_RES_OBJ_SIDEBAR_M;
    if (cp_data->display) {
        objID = BR_RES_OBJ_SIDEBAR_S;
    }
    ret = func_ov186_021fc124(res, objID, &obj);
    GFL_ASSERT(ret);

    MI_CpuFill8(&setup, 0, sizeof(ClActorSetup));
    setup.x = cp_data->x;
    setup.y = cp_data->y;
    setup.bgPriority = 1;
    p_wk->x = FX32_CONST(setup.x);
    p_wk->y = FX32_CONST(setup.y);
    p_wk->scale = cp_data->scale;
    p_wk->clwk = func_0204b294(unit, obj.chr, obj.plt, obj.cell, &setup, p_wk->data.display, heapId);
    func_0204b570(p_wk->clwk, 2);
    func_0204b5d4(p_wk->clwk, p_wk->scale, 0);
    func_0204b688(p_wk->clwk, 0);
    func_0204b3dc(p_wk->clwk, FALSE);
    p_wk->is_use = TRUE;
}

// pokebw2's BrSidebarWork_Exit
static void func_ov186_021ff004(BrSidebarWork *p_wk) {
    func_0204b3b4(p_wk->clwk);
    MI_CpuFill8(p_wk, 0, sizeof(BrSidebarWork));
}

// pokebw2's BrSidebarWork_Main
static void func_ov186_021ff01c(BrSidebarWork *p_wk) {
    if (p_wk->move_func != NULL) {
        p_wk->move_func(p_wk);
    }
}

// pokebw2's BrSidebarWork_IsUse
static BOOL func_ov186_021ff028(const BrSidebarWork *cp_wk) {
    return cp_wk->is_use;
}

// pokebw2's BrSidebarWork_SetVisible
static void func_ov186_021ff02c(BrSidebarWork *p_wk, BOOL isVisible) {
    func_0204b3dc(p_wk->clwk, isVisible);
}

// pokebw2's BrSidebarWork_StartMove
static void func_ov186_021ff038(BrSidebarWork *p_wk, u32 type) {
    switch (type) {
    case BR_SIDEBAR_MOVE_NONE:
        p_wk->move_func = NULL;
        break;
    case BR_SIDEBAR_MOVE_OPEN:
        p_wk->move_func = func_ov186_021ff0a0;
        break;
    case BR_SIDEBAR_MOVE_BOUND:
        p_wk->move_func = func_ov186_021ff248;
        break;
    case BR_SIDEBAR_MOVE_CLOSE:
        p_wk->move_func = func_ov186_021ff334;
        break;
    }
    p_wk->seq = 0;
}

// pokebw2's BrSidebarWork_SetEndPos
static void func_ov186_021ff074(BrSidebarWork *p_wk) {
    p_wk->scale = BR_SIDEBAR_SCALE_END;
    func_0204b49c(p_wk->clwk, p_wk->data.end_x, p_wk->data.display, 0);
    func_0204b5d4(p_wk->clwk, p_wk->scale, 0);
}

// The sign of a sidebar's sway: 1 when its data's dir is set, otherwise -1
static inline int BrSidebar_GetDir(BOOL dir) {
    int ret = -1;

    if (dir) {
        ret = 1;
    }
    return ret;
}

// Shakes in place, then slides to its end and shrinks
// pokebw2's BrSidebarWork_Move_Open
static void func_ov186_021ff0a0(BrSidebarWork *p_wk) {
    p_wk->wait = (p_wk->wait + 1) % 2;
    if (p_wk->wait != 0) {
        return;
    }

    switch (p_wk->seq) {
    case 0:
        func_0204b3dc(p_wk->clwk, TRUE);
        p_wk->cnt = 0;
        p_wk->scale = p_wk->data.scale;
        p_wk->seq = 1;
        break;
    case 1:
        if (p_wk->cnt == 3) {
            p_wk->seq = 2;
            p_wk->now_x = FX32_CONST(func_0204b4ec(p_wk->clwk, p_wk->data.display, 0));
            p_wk->cnt = 0;
        } else {
            p_wk->cnt++;
        }
        break;
    case 2: {
        s8 dir = BrSidebar_GetDir(p_wk->data.dir);

        if (p_wk->cnt == 4) {
            p_wk->seq = 3;
            p_wk->cnt = 0;
        } else {
            u16 rot = 0xffff * p_wk->theta / 360;

            p_wk->now_x += -dir * func_0203f684(rot);
            p_wk->theta += 32;
            p_wk->theta %= 360;
            func_0204b49c(p_wk->clwk, p_wk->now_x >> FX32_SHIFT, p_wk->data.display, 0);
            p_wk->cnt++;
        }
        break;
    }
    case 3: {
        s16 x = func_0204b4ec(p_wk->clwk, p_wk->data.display, 0);

        if (p_wk->cnt == 0) {
            p_wk->dx = p_wk->data.end_x - x;
            p_wk->dx /= p_wk->data.sync;
            p_wk->cnt++;
        } else if (p_wk->cnt == p_wk->data.sync + 1) {
            x = p_wk->data.end_x;
            p_wk->seq = 4;
        } else {
            x += (s16)p_wk->dx;
            p_wk->cnt++;
        }
        func_0204b49c(p_wk->clwk, x, p_wk->data.display, 0);
        break;
    }
    case 4:
        func_ov186_021ff038(p_wk, BR_SIDEBAR_MOVE_BOUND);
        break;
    }

    if (p_wk->seq == 3 && p_wk->scale > BR_SIDEBAR_SCALE_END) {
        p_wk->scale -= BR_SIDEBAR_SCALE_END;
        func_0204b5d4(p_wk->clwk, p_wk->scale, 0);
    }
}

// Sways about its end
// pokebw2's BrSidebarWork_Move_Bound
static void func_ov186_021ff248(BrSidebarWork *p_wk) {
    p_wk->wait = (p_wk->wait + 1) % 2;
    if (p_wk->wait != 0) {
        return;
    }

    switch (p_wk->seq) {
    case 0:
        func_0204b3dc(p_wk->clwk, TRUE);
        func_0204b49c(p_wk->clwk, p_wk->data.end_x, p_wk->data.display, 0);
        p_wk->now_x = FX32_CONST(p_wk->data.end_x);
        p_wk->theta = p_wk->data.theta;
        p_wk->seq = 1;
        break;
    case 1: {
        s8 dir = BrSidebar_GetDir(p_wk->data.dir);
        u16 rot;
        fx32 x;

        rot = 0xffff * p_wk->theta / 360;
        x = p_wk->now_x + -dir * (func_0203f684(rot) * 3);
        p_wk->theta += p_wk->data.theta_add;
        p_wk->theta %= 360;
        func_0204b49c(p_wk->clwk, x >> FX32_SHIFT, p_wk->data.display, 0);
        break;
    }
    }

    if (p_wk->scale > BR_SIDEBAR_SCALE_END) {
        p_wk->scale -= BR_SIDEBAR_SCALE_END;
        func_0204b5d4(p_wk->clwk, p_wk->scale, 0);
    }
}

// Slides back to its start, growing back
// pokebw2's BrSidebarWork_Move_Close
static void func_ov186_021ff334(BrSidebarWork *p_wk) {
    p_wk->wait = (p_wk->wait + 1) % 2;
    if (p_wk->wait != 0) {
        return;
    }

    switch (p_wk->seq) {
    case 0:
        func_0204b3dc(p_wk->clwk, TRUE);
        p_wk->cnt = 0;
        p_wk->seq = 1;
        break;
    case 1: {
        s16 x = func_0204b4ec(p_wk->clwk, p_wk->data.display, 0);
        fx32 scale = func_0204b5e4(p_wk->clwk, 0);

        if (p_wk->cnt == 0) {
            p_wk->dx = p_wk->data.x - x;
            p_wk->dx /= p_wk->data.sync;
            p_wk->cnt++;
            p_wk->dscale = p_wk->data.scale - scale;
            p_wk->dscale /= p_wk->data.sync;
        } else if (p_wk->cnt == p_wk->data.sync + 1) {
            x = p_wk->data.x;
            scale = p_wk->data.scale;
            p_wk->seq = 2;
        } else {
            x += (s16)p_wk->dx;
            scale += p_wk->dscale;
            p_wk->cnt++;
        }
        func_0204b49c(p_wk->clwk, x, p_wk->data.display, 0);
        func_0204b5d4(p_wk->clwk, scale, 0);
        break;
    }
    case 2:
        func_ov186_021ff038(p_wk, BR_SIDEBAR_MOVE_NONE);
        break;
    }
}
