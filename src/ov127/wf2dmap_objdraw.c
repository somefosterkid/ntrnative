// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov203/wf2dmap_objdraw.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/wf2dmap/wf2dmap_common.h"
#include "app/wf2dmap/wf2dmap_obj.h"
#include "app/wf2dmap/wf2dmap_objdraw.h"
#include "app/wf2dmap/wifi_2dchar.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "gfl/std.h"

// Draws the objects of the Wi-Fi Club's 2D map, named after the ROM's "wf2dmap_objdraw.c"

#define WF2DMAP_OBJDRAW_BG_PRI 2
#define WF2DMAP_OBJDRAW_SHADOW_PRI 0xff

// The priority of an object at the top of the map. The player's own character is in front of the others on its row
#define WF2DMAP_OBJDRAW_PRI_HERO 250
#define WF2DMAP_OBJDRAW_PRI_OTHER 251

struct WF2DMAP_OBJDRAWWK {
    const WF2DMAP_OBJWK *objWk;
    WF_2DCWK *drawWk;
    // What the object did when its animation was last set
    u16 status;
    u16 way;
    u16 playId;
    // The frame the last animation that keeps its frame was left at, and that animation
    u16 lastAnmFrame;
    u16 lastAnm;
    u8 hero;
    u8 updata;
};

struct WF2DMAP_OBJDRAWSYS {
    WF_2DCSYS *charSys;
    WF2DMAP_OBJDRAWWK *wk;
    u16 num;
    u8 bgPriority;
    u8 vramType;
};

static s32 func_ov127_021c5878(s16 y, u32 hero);
static BOOL func_ov127_021c58b8(const WF2DMAP_OBJDRAWWK *wk);
static WF2DMAP_OBJDRAWWK *func_ov127_021c58c8(WF2DMAP_OBJDRAWSYS *sys);
static u32 func_ov127_021c58f8(const WF_2DCWK *drawWk, u32 status);
static void func_ov127_021c5914(const WF2DMAP_OBJDRAWWK *wk, s16 *x, s16 *y);
static void func_ov127_021c5944(WF2DMAP_OBJDRAWWK *wk, u32 anmType, u32 way);

// Whether an animation goes on from the frame it was left at when it starts again
// pokebw2's sWF2DMAP_OBJDrawAnmKeepFrame
static const u8 data_ov127_021c60ac[WF_2DC_ANMNUM] = {
    FALSE, FALSE, TRUE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE,
};

// The animation of each WF2DMAP_OBJST_*
// pokebw2's sWF2DMAP_OBJDrawStAnm
static const u8 data_ov127_021c60b6[WF2DMAP_OBJST_NUM] = {
    WF_2DC_ANMWAY,       WF_2DC_ANMTURN,      WF_2DC_ANMWALK,     WF_2DC_ANMRUN,
    WF_2DC_ANMWAY,       WF_2DC_ANMWALLWALK,  WF_2DC_ANMSLOWWALK, WF_2DC_ANMHIGHWALK4,
    WF_2DC_ANMHIGHWALK2, WF_2DC_ANMHIGHWALK4, WF_2DC_ANMWALK,     WF_2DC_ANMSLOWWALK,
};

WF2DMAP_OBJDRAWSYS *func_ov127_021c5590(ClActUnit *unit, void *unk1, u32 objNum, u32 charaId, u32 moveType,
                                           u32 vramType, u32 heapId) {
    WF2DMAP_OBJDRAWSYS *sys;

    sys = GFL_HeapAllocate(heapId, sizeof(WF2DMAP_OBJDRAWSYS), TRUE, "wf2dmap_objdraw.c", 161);
    sys->charSys = func_ov127_021c3ee0(unit, unk1, objNum, heapId);
    sys->num = objNum;
    sys->wk = GFL_HeapAllocate(heapId, sizeof(WF2DMAP_OBJDRAWWK) * sys->num, TRUE, "wf2dmap_objdraw.c", 168);
    sys->bgPriority = WF2DMAP_OBJDRAW_BG_PRI;
    sys->vramType = vramType;
    func_ov127_021c3fdc(sys->charSys, charaId, sys->vramType, moveType, heapId);
    func_ov127_021c40a0(sys->charSys, sys->vramType, WF2DMAP_OBJDRAW_SHADOW_PRI, heapId);
    return sys;
}

// pokebw2's WF2DMAP_OBJDrawSysExit
void func_ov127_021c5608(WF2DMAP_OBJDRAWSYS *sys) {
    int i;

    for (i = 0; i < sys->num; i++) {
        if (func_ov127_021c58b8(&sys->wk[i]) == FALSE) {
            func_ov127_021c5704(&sys->wk[i]);
        }
    }
    func_ov127_021c40b4(sys->charSys);
    func_ov127_021c5658(sys);
    func_ov127_021c3f80(sys->charSys);
    GFL_HeapFree(sys->wk);
    GFL_HeapFree(sys);
}

// pokebw2's WF2DMAP_OBJDrawSysAllResDel
void func_ov127_021c5658(WF2DMAP_OBJDRAWSYS *sys) {
    func_ov127_021c4010(sys->charSys);
}

// pokebw2's WF2DMAP_OBJDrawSysUnionResSet
void func_ov127_021c5664(WF2DMAP_OBJDRAWSYS *sys, u32 moveType, u32 heapId) {
    func_ov127_021c4044(sys->charSys, sys->vramType, moveType, 0, heapId);
}

// pokebw2's WF2DMAP_OBJDrawWkNew
WF2DMAP_OBJDRAWWK *func_ov127_021c5680(WF2DMAP_OBJDRAWSYS *sys, const WF2DMAP_OBJWK *objWk, BOOL hero, u32 heapId) {
    WF2DMAP_OBJDRAWWK *wk;
    WF_2DC_WKDATA data;

    wk = func_ov127_021c58c8(sys);
    wk->status = func_ov127_021c49a4(objWk, WF2DMAP_OBJPM_ST);
    wk->way = func_ov127_021c49a4(objWk, WF2DMAP_OBJPM_WAY);
    wk->playId = func_ov127_021c49a4(objWk, WF2DMAP_OBJPM_PLID);
    wk->objWk = objWk;
    wk->hero = hero;
    wk->updata = TRUE;
    wk->lastAnmFrame = 0;
    wk->lastAnm = 0;

    data.x = func_ov127_021c49a4(objWk, WF2DMAP_OBJPM_X);
    data.y = func_ov127_021c49a4(objWk, WF2DMAP_OBJPM_Y);
    data.priority = func_ov127_021c5878(data.y, wk->hero);
    data.bgPriority = sys->bgPriority;
    wk->drawWk = func_ov127_021c40bc(sys->charSys, &data, func_ov127_021c49a4(objWk, WF2DMAP_OBJPM_CHARA), heapId);

    func_ov127_021c574c(wk);
    return wk;
}

// pokebw2's WF2DMAP_OBJDrawWkDel
void func_ov127_021c5704(WF2DMAP_OBJDRAWWK *wk) {
    func_ov127_021c41bc(wk->drawWk);
    MI_CpuFill8(wk, 0, sizeof(WF2DMAP_OBJDRAWWK));
}

// pokebw2's WF2DMAP_OBJDrawSysUpdata
void func_ov127_021c571c(WF2DMAP_OBJDRAWSYS *sys) {
    int i;

    for (i = 0; i < sys->num; i++) {
        if (func_ov127_021c58b8(&sys->wk[i]) == FALSE) {
            func_ov127_021c574c(&sys->wk[i]);
        }
    }
}

// pokebw2's WF2DMAP_OBJDrawWkUpdata
void func_ov127_021c574c(WF2DMAP_OBJDRAWWK *wk) {
    s32 status;
    s32 way;
    s16 frame;
    s16 x;
    s16 y;
    u16 priority;

    if (wk->updata) {
        status = func_ov127_021c49a4(wk->objWk, WF2DMAP_OBJPM_ST);
        way = func_ov127_021c49a4(wk->objWk, WF2DMAP_OBJPM_WAY);
        frame = func_ov127_021c49a4(wk->objWk, WF2DMAP_OBJPM_FRAME);

        if (wk->status != status || wk->way != way || frame == 0) {
            wk->status = status;
            wk->way = way;
            func_ov127_021c5944(wk, func_ov127_021c58f8(wk->drawWk, status), way);
        }

        func_ov127_021c5914(wk, &x, &y);
        func_ov127_021c41dc(wk->drawWk, x, y);
        priority = func_ov127_021c5878(y, wk->hero);
        func_ov127_021c4214(wk->drawWk, priority);
        func_ov127_021c42c4(wk->drawWk);
    }
}

// pokebw2's WF2DMAP_OBJDrawWkUpdataFlagSet
void func_ov127_021c57d8(WF2DMAP_OBJDRAWWK *wk, BOOL flag) {
    wk->updata = flag;
}

// pokebw2's WF2DMAP_OBJDrawWkKuruAnimeStart
void func_ov127_021c57dc(WF2DMAP_OBJDRAWWK *wk) {
    func_ov127_021c5944(wk, WF_2DC_ANMROTA, wk->way);
}

// pokebw2's WF2DMAP_OBJDrawWkKuruAnimeMain
void func_ov127_021c57e8(WF2DMAP_OBJDRAWWK *wk) {
    func_ov127_021c42c4(wk->drawWk);
}

// pokebw2's WF2DMAP_OBJDrawWkKuruAnimeEnd
void func_ov127_021c57f4(WF2DMAP_OBJDRAWWK *wk) {
    func_ov127_021c5944(wk, func_ov127_021c58f8(wk->drawWk, wk->status), wk->way);
    func_ov127_021c574c(wk);
}

// pokebw2's WF2DMAP_OBJDrawWkMatrixSet
void func_ov127_021c5814(WF2DMAP_OBJDRAWWK *wk, WF2DMAP_POS pos) {
    func_ov127_021c41dc(wk->drawWk, pos.x, pos.y);
}

// pokebw2's WF2DMAP_OBJDrawWkDrawFlagGet
BOOL func_ov127_021c5834(const WF2DMAP_OBJDRAWWK *wk) {
    return func_ov127_021c425c(wk->drawWk);
}

// pokebw2's WF2DMAP_OBJDrawWkDrawFlagSet
void func_ov127_021c5840(WF2DMAP_OBJDRAWWK *wk, BOOL flag) {
    func_ov127_021c4230(wk->drawWk, flag);
}

// pokebw2's WF2DMAP_OBJDrawWkShadowDrawFlagSet
void func_ov127_021c584c(WF2DMAP_OBJDRAWWK *wk, BOOL flag) {
    func_ov127_021c424c(wk->drawWk, flag);
}

// pokebw2's WF2DMAP_OBJDrawWkDrawPriGet
u32 func_ov127_021c5858(const WF2DMAP_OBJDRAWWK *wk) {
    return func_ov127_021c4224(wk->drawWk);
}

// pokebw2's WF2DMAP_OBJDrawWkWaySet
void func_ov127_021c5864(WF2DMAP_OBJDRAWWK *wk, u32 way) {
    func_ov127_021c5944(wk, WF_2DC_ANMWAY, way);
    func_ov127_021c42c4(wk->drawWk);
}

// Objects lower on the map are in front
// pokebw2's WF2DMAP_OBJDrawWkDrawPriCalc
static s32 func_ov127_021c5878(s16 y, u32 hero) {
    if (hero == TRUE) {
        return WF2DMAP_OBJDRAW_PRI_HERO - y / 4;
    }
    return WF2DMAP_OBJDRAW_PRI_OTHER - y / 4;
}

// pokebw2's WF2DMAP_OBJDrawWkShadowMatrixSet
void func_ov127_021c5898(WF2DMAP_OBJDRAWWK *wk, WF2DMAP_POS pos) {
    func_ov127_021c428c(wk->drawWk, pos.x, pos.y);
}

// pokebw2's WF2DMAP_OBJDrawWkCleanCheck
static BOOL func_ov127_021c58b8(const WF2DMAP_OBJDRAWWK *wk) {
    if (wk->drawWk == NULL) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's WF2DMAP_OBJDrawSysCleanWkGet
static WF2DMAP_OBJDRAWWK *func_ov127_021c58c8(WF2DMAP_OBJDRAWSYS *sys) {
    int i;

    for (i = 0; i < sys->num; i++) {
        if (func_ov127_021c58b8(&sys->wk[i]) == TRUE) {
            return &sys->wk[i];
        }
    }
    return NULL;
}

// pokebw2's WF2DMAP_OBJDrawWkAnmTypeGet
static u32 func_ov127_021c58f8(const WF_2DCWK *drawWk, u32 status) {
    if (func_ov127_021c4268(drawWk) == WF_2DC_MOVETURN) {
        return WF_2DC_ANMWAY;
    }
    return data_ov127_021c60b6[status];
}

// pokebw2's WF2DMAP_OBJDrawWkFrameMatrixGet
static void func_ov127_021c5914(const WF2DMAP_OBJDRAWWK *wk, s16 *x, s16 *y) {
    WF2DMAP_POS pos;

    pos = func_ov127_021c4adc(wk->objWk);
    *x = pos.x;
    *y = pos.y;
}

// pokebw2's WF2DMAP_OBJDrawWkAnmSet
static void func_ov127_021c5944(WF2DMAP_OBJDRAWWK *wk, u32 anmType, u32 way) {
    u16 frame;
    u16 lastAnm;

    frame = func_ov127_021c4280(wk->drawWk);
    lastAnm = func_ov127_021c4270(wk->drawWk);
    func_ov127_021c42a8(wk->drawWk, anmType, way);

    if (lastAnm != anmType) {
        if (data_ov127_021c60ac[anmType] == TRUE) {
            if (wk->lastAnm == anmType) {
                func_ov127_021c4274(wk->drawWk, wk->lastAnmFrame);
            }
        }
        if (data_ov127_021c60ac[lastAnm] == TRUE) {
            wk->lastAnm = lastAnm;
            wk->lastAnmFrame = frame;
        }
    }
}
