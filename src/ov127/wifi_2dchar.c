// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov203/wifi_2dchar.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/wf2dmap/wifi_2dchar.h"
#include "gfl/arc.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"

// The characters of the Wi-Fi Club's 2D map, named after the ROM's "wifi_2dchar.c"

// No resource
#define WF_2DC_RES_NONE 0xffffffff

// The files of ARCID_WIFI_2DCHAR: for each of the two player characters a cell and an animation, then a palette and
// characters, then the shadow's
#define WF_2DC_ARC_ANM(chara) (((chara) - WF_2DC_CHARA_HERO) * 2)
#define WF_2DC_ARC_PLTT(chara) (((chara) - WF_2DC_CHARA_HERO) * 2 + 4)
#define WF_2DC_ARC_CHAR(chara) (((chara) - WF_2DC_CHARA_HERO) * 2 + 5)
#define WF_2DC_ARC_SHADOW_ANM 8
#define WF_2DC_ARC_SHADOW_CELL 9
#define WF_2DC_ARC_SHADOW_PLTT 10
#define WF_2DC_ARC_SHADOW_CHAR 11

// The files of ARCID_WIFI_UNIONCHAR: the palettes all its characters share, their characters, then a cell and an
// animation for each
#define WF_2DC_ARC_UNIONPLTT 0
#define WF_2DC_ARC_UNIONCHAR(chara) ((chara) + 1)
#define WF_2DC_ARC_UNIONCELL(chara) ((chara) * 2 + 17)
#define WF_2DC_ARC_UNIONANM(chara) ((chara) * 2 + 18)

#define WF_2DC_UNIONPLTT_NUM 8

// Where the shadow is from its character
#define WF_2DC_SHADOW_OFS_X 8
#define WF_2DC_SHADOW_OFS_Y 14

// A pattern animation that never ends
#define WF_2DC_ANMFRAME_LOOP 0xff

// A character's graphics
typedef struct {
    // Whether it is a Union Room character, whose palette is the shared one
    BOOL unionChara;
    u32 chars;
    u32 palette;
    u32 vramType;
    u32 moveType;
} WF_2DCRES;

typedef struct {
    u32 unk0[4];
    u32 cellAnims;
} WF_2DCANMRES;

typedef struct {
    u32 chars;
    u32 palette;
    u32 cellAnims;
} WF_2DCSHRES;

struct WF_2DCWK {
    const WF_2DCRES *res;
    ClActor *actor;
    ClActor *shadow;
    u32 anmType;
    u32 way;
    s16 frame;
};

struct WF_2DCSYS {
    ClActUnit *unit;
    void *unk4;
    WF_2DCWK *objs;
    u32 objNum;
    WF_2DCRES res[WF_2DC_CHARA_NUM];
    WF_2DCANMRES anmRes[WF_2DC_CHARA_NUM];
    WF_2DCSHRES shadowRes;
    u32 unk2ec;
    u32 shadowPriority;
    u32 unionPltt;
    ArcTool *handles[2];
    u8 unk300[0x30];
    // The player character that is loaded, which nothing reads
    u32 shadowChara;
};

static void func_ov127_021c42fc(WF_2DCSYS *sys, u32 unused, u32 heapId);
static void func_ov127_021c4330(WF_2DCSYS *sys, u32 unused);
static u32 func_ov127_021c4354(u32 charaId);
static BOOL func_ov127_021c4358(u32 charaId, u32 moveType);
static void func_ov127_021c435c(WF_2DCSYS *sys, u32 charaId, u32 vramType, u32 moveType, u32 heapId, BOOL unionChara);
static void func_ov127_021c4380(WF_2DCSYS *sys, u32 charaId, u32 vramType, u32 moveType, u32 heapId);
static void func_ov127_021c43e4(WF_2DCSYS *sys, u32 charaId, u32 vramType, u32 moveType, u32 heapId);
static void func_ov127_021c4424(WF_2DCSYS *sys, u32 charaId);
static void func_ov127_021c4440(WF_2DCSYS *sys, u32 charaId);
static void func_ov127_021c4464(WF_2DCSYS *sys, u32 charaId);
static BOOL func_ov127_021c447c(const WF_2DCSYS *sys, u32 charaId);
static void func_ov127_021c4494(WF_2DCSYS *sys, u32 vramType, u32 heapId);
static void func_ov127_021c4514(WF_2DCSYS *sys);
static BOOL func_ov127_021c4550(const WF_2DCSHRES *res);
static void func_ov127_021c4564(WF_2DCSYS *sys, u32 vramType, u32 plttOffset, u32 heapId);
static void func_ov127_021c4594(WF_2DCSYS *sys);
static void func_ov127_021c45ac(WF_2DCSYS *sys, u32 heapId);
static void func_ov127_021c45e0(WF_2DCSYS *sys);
static WF_2DCWK *func_ov127_021c4604(WF_2DCSYS *sys);
static void func_ov127_021c4634(WF_2DCWK *wk);
static void func_ov127_021c464c(WF_2DCWK *wk, u32 way);
static void func_ov127_021c465c(WF_2DCWK *wk, u32 way);
static void func_ov127_021c466c(WF_2DCWK *wk, u32 way);
static void func_ov127_021c4680(WF_2DCWK *wk, u32 way);
static void func_ov127_021c4698(WF_2DCWK *wk, u32 way);
static void func_ov127_021c46ac(WF_2DCWK *wk, u32 way);
static void func_ov127_021c46c0(WF_2DCWK *wk, u32 way);
static void func_ov127_021c46d4(WF_2DCWK *wk, u32 way);
static void func_ov127_021c46e8(WF_2DCWK *wk, u32 way);
static void func_ov127_021c46fc(WF_2DCWK *wk);
static void func_ov127_021c470c(WF_2DCWK *wk);
static void func_ov127_021c471c(WF_2DCWK *wk);
static void func_ov127_021c4740(WF_2DCWK *wk);
static void func_ov127_021c4750(WF_2DCWK *wk);
static void func_ov127_021c4760(WF_2DCWK *wk);
static void func_ov127_021c4770(WF_2DCWK *wk);
static void func_ov127_021c4780(WF_2DCWK *wk);
static BOOL func_ov127_021c4790(const WF_2DCWK *wk);

// How many frames each animation lasts
// pokebw2's sWF_2DC_AnmFrames
static const u8 data_ov127_021c5fb0[WF_2DC_ANMNUM] = {
    1, WF_2DC_ANMFRAME_LOOP, 8, 2, 4, 16, 16, 2, 4, 0,
};

// pokebw2's sWF_2DC_AnmInitFuncs
static void (*const data_ov127_021c5fbc[WF_2DC_ANMNUM])(WF_2DCWK *wk, u32 way) = {
    func_ov127_021c464c,       func_ov127_021c465c,
    func_ov127_021c466c,      func_ov127_021c4680,
    func_ov127_021c4698,       func_ov127_021c46ac,
    func_ov127_021c46c0,  func_ov127_021c46d4,
    func_ov127_021c46e8, NULL,
};

// pokebw2's sWF_2DC_AnmMainFuncs
static void (*const data_ov127_021c5fe4[WF_2DC_ANMNUM])(WF_2DCWK *wk) = {
    NULL,
    func_ov127_021c46fc,
    func_ov127_021c470c,
    func_ov127_021c471c,
    func_ov127_021c4740,
    func_ov127_021c4750,
    func_ov127_021c4760,
    func_ov127_021c4770,
    func_ov127_021c4780,
    NULL,
};

// pokebw2's WF_2DC_SysInit
WF_2DCSYS *func_ov127_021c3ee0(ClActUnit *unit, void *unk1, u32 objNum, u32 heapId) {
    WF_2DCSYS *sys;
    int i;

    sys = GFL_HeapAllocate(heapId, sizeof(WF_2DCSYS), TRUE, "wifi_2dchar.c", 339);
    sys->unit = unit;
    sys->unk4 = unk1;
    sys->handles[0] = GFL_ArcSysCreateFileHandle(ARCID_WIFI_2DCHAR, heapId);
    sys->handles[1] = GFL_ArcSysCreateFileHandle(ARCID_WIFI_UNIONCHAR, heapId);

    for (i = 0; i < WF_2DC_CHARA_NUM; i++) {
        sys->res[i].chars = WF_2DC_RES_NONE;
    }
    sys->shadowRes.chars = WF_2DC_RES_NONE;
    sys->unionPltt = WF_2DC_RES_NONE;

    func_ov127_021c42fc(sys, 0, heapId);

    sys->objNum = objNum;
    sys->objs = GFL_HeapAllocate(heapId, sizeof(WF_2DCWK) * sys->objNum, TRUE, "wifi_2dchar.c", 379);
    return sys;
}

// pokebw2's WF_2DC_SysExit
void func_ov127_021c3f80(WF_2DCSYS *sys) {
    int i;

    for (i = 0; i < sys->objNum; i++) {
        if (func_ov127_021c4790(&sys->objs[i]) == FALSE) {
            func_ov127_021c41bc(&sys->objs[i]);
        }
    }
    func_ov127_021c4330(sys, 0);
    func_ov127_021c4010(sys);
    GFL_ArcToolFree(sys->handles[0]);
    GFL_ArcToolFree(sys->handles[1]);
    GFL_HeapFree(sys->objs);
    GFL_HeapFree(sys);
}

// pokebw2's WF_2DC_ResSet
void func_ov127_021c3fdc(WF_2DCSYS *sys, u32 charaId, u32 vramType, u32 moveType, u32 heapId) {
    u32 charNo;

    func_ov127_021c4358(charaId, moveType);
    charNo = func_ov127_021c4354(charaId);
    func_ov127_021c435c(sys, charNo, vramType, moveType, heapId, FALSE);
}

// pokebw2's WF_2DC_AllResDel
void func_ov127_021c4010(WF_2DCSYS *sys) {
    int i;

    if (sys->unionPltt != WF_2DC_RES_NONE) {
        func_ov127_021c4080(sys);
    }
    for (i = 0; i < WF_2DC_CHARA_NUM; i++) {
        if (func_ov127_021c447c(sys, i) == TRUE) {
            func_ov127_021c4424(sys, i);
        }
    }
}

// pokebw2's WF_2DC_UnionResSet
void func_ov127_021c4044(WF_2DCSYS *sys, u32 vramType, u32 moveType, u32 plttOffset, u32 heapId) {
    int i;

    func_ov127_021c4564(sys, vramType, plttOffset, heapId);
    func_ov127_021c45ac(sys, heapId);
    for (i = 0; i < WF_2DC_UNIONCHARA_NUM; i++) {
        func_ov127_021c435c(sys, i, vramType, moveType, heapId, TRUE);
    }
}

// pokebw2's WF_2DC_UnionResDel
void func_ov127_021c4080(WF_2DCSYS *sys) {
    int i;

    func_ov127_021c4594(sys);
    func_ov127_021c45e0(sys);
    for (i = 0; i < WF_2DC_UNIONCHARA_NUM; i++) {
        func_ov127_021c4424(sys, i);
    }
}

// pokebw2's WF_2DC_ShadowResSet
void func_ov127_021c40a0(WF_2DCSYS *sys, u32 vramType, u32 priority, u32 heapId) {
    func_ov127_021c4494(sys, vramType, heapId);
    sys->shadowPriority = priority;
}

// pokebw2's WF_2DC_ShadowResDel
void func_ov127_021c40b4(WF_2DCSYS *sys) {
    func_ov127_021c4514(sys);
}

// pokebw2's WF_2DC_WkAdd
WF_2DCWK *func_ov127_021c40bc(WF_2DCSYS *sys, const WF_2DC_WKDATA *data, u32 charaId, u32 heapId) {
    WF_2DCWK *wk;
    u32 charNo;
    ClActorSetup setup;

    wk = func_ov127_021c4604(sys);
    charNo = func_ov127_021c4354(charaId);

    setup.x = data->x;
    setup.y = data->y;
    setup.sequence = 0;
    setup.priority = data->priority;
    setup.bgPriority = data->bgPriority;
    wk->actor = func_0204b294(sys->unit, sys->res[charNo].chars, sys->res[charNo].palette,
                              sys->anmRes[charNo].cellAnims, &setup, 0, heapId);
    func_0204b84c(wk->actor, (u8)data->bgPriority);

    if (sys->res[charNo].moveType == WF_2DC_MOVENORMAL) {
        func_0204b908(wk->actor, 6);
    } else {
        func_0204b908(wk->actor, 1);
    }

    wk->anmType = WF_2DC_ANMNONE;
    wk->frame = 0;
    wk->way = 1;
    wk->res = &sys->res[charNo];

    if (func_ov127_021c4550(&sys->shadowRes)) {
        setup.x = data->x;
        setup.y = data->y;
        setup.sequence = 0;
        setup.priority = sys->shadowPriority;
        setup.bgPriority = 0;
        wk->shadow = func_0204b294(sys->unit, sys->shadowRes.chars, sys->shadowRes.palette, sys->shadowRes.cellAnims,
                                   &setup, 0, heapId);
        func_0204b84c(wk->shadow, (u8)data->bgPriority);
    } else {
        wk->shadow = NULL;
    }
    return wk;
}

// pokebw2's WF_2DC_WkDel
void func_ov127_021c41bc(WF_2DCWK *wk) {
    if (wk->shadow != NULL) {
        func_0204b3b4(wk->shadow);
    }
    func_0204b3b4(wk->actor);
    MI_CpuFill8(wk, 0, sizeof(WF_2DCWK));
}

// pokebw2's WF_2DC_WkMatrixSet
void func_ov127_021c41dc(WF_2DCWK *wk, s16 x, s16 y) {
    ClActorPos pos;

    pos.x = x;
    pos.y = y;
    func_0204b528(wk->actor, &pos);
    if (wk->shadow != NULL) {
        pos.x += WF_2DC_SHADOW_OFS_X;
        pos.y += WF_2DC_SHADOW_OFS_Y;
        func_0204b528(wk->shadow, &pos);
    }
}

// pokebw2's WF_2DC_WkDrawPriSet
void func_ov127_021c4214(WF_2DCWK *wk, u16 priority) {
    func_0204b800(wk->actor, priority);
}

// pokebw2's WF_2DC_WkDrawPriGet
u32 func_ov127_021c4224(const WF_2DCWK *wk) {
    return func_0204b838(wk->actor);
}

// pokebw2's WF_2DC_WkDrawFlagSet
void func_ov127_021c4230(WF_2DCWK *wk, BOOL flag) {
    func_0204b3dc(wk->actor, flag);
    if (wk->shadow != NULL) {
        func_0204b3dc(wk->shadow, flag);
    }
}

// pokebw2's WF_2DC_WkShadowDrawFlagSet
void func_ov127_021c424c(WF_2DCWK *wk, BOOL flag) {
    if (wk->shadow != NULL) {
        func_0204b3dc(wk->shadow, flag);
    }
}

// pokebw2's WF_2DC_WkDrawFlagGet
BOOL func_ov127_021c425c(const WF_2DCWK *wk) {
    return func_0204b3f4(wk->actor);
}

// pokebw2's WF_2DC_WkMoveTypeGet
u32 func_ov127_021c4268(const WF_2DCWK *wk) {
    return wk->res->moveType;
}

// pokebw2's WF_2DC_WkAnmTypeGet
u32 func_ov127_021c4270(const WF_2DCWK *wk) {
    return wk->anmType;
}

// pokebw2's WF_2DC_WkAnmFrameSet
void func_ov127_021c4274(WF_2DCWK *wk, u16 frame) {
    func_0204b908(wk->actor, frame);
}

// pokebw2's WF_2DC_WkAnmFrameGet
u16 func_ov127_021c4280(const WF_2DCWK *wk) {
    return func_0204b918(wk->actor);
}

// pokebw2's WF_2DC_WkShadowMatrixSet
void func_ov127_021c428c(WF_2DCWK *wk, s16 x, s16 y) {
    ClActorPos pos;

    if (wk->shadow != NULL) {
        pos.x = x + WF_2DC_SHADOW_OFS_X;
        pos.y = y + WF_2DC_SHADOW_OFS_Y;
        func_0204b528(wk->shadow, &pos);
    }
}

// pokebw2's WF_2DC_WkPatAnmStart
void func_ov127_021c42a8(WF_2DCWK *wk, u32 anmType, u32 way) {
    wk->frame = 0;
    wk->anmType = anmType;
    if (data_ov127_021c5fbc[anmType] != NULL) {
        data_ov127_021c5fbc[anmType](wk, way);
    }
}

// pokebw2's WF_2DC_WkPatAnmMain
void func_ov127_021c42c4(WF_2DCWK *wk) {
    BOOL update = FALSE;

    if (data_ov127_021c5fb0[wk->anmType] == WF_2DC_ANMFRAME_LOOP) {
        update = TRUE;
    } else if (data_ov127_021c5fb0[wk->anmType] >= wk->frame + 1) {
        update = TRUE;
    }

    if (update == TRUE) {
        func_ov127_021c4634(wk);
        wk->frame++;
    }
}

// pokebw2's WF_2DC_AnmResLoad
static void func_ov127_021c42fc(WF_2DCSYS *sys, u32 unused, u32 heapId) {
    int i;

    for (i = WF_2DC_CHARA_HERO; i < WF_2DC_CHARA_NUM; i++) {
        sys->anmRes[i].cellAnims = func_0204af28(sys->handles[0], WF_2DC_ARC_ANM(i), WF_2DC_ARC_ANM(i) + 1, heapId);
    }
}

// pokebw2's WF_2DC_AnmResDel
static void func_ov127_021c4330(WF_2DCSYS *sys, u32 unused) {
    int i;

    for (i = WF_2DC_CHARA_HERO; i < WF_2DC_CHARA_NUM; i++) {
        func_0204afd8(sys->anmRes[i].cellAnims);
        sys->anmRes[i].cellAnims = 0;
    }
}

// pokebw2's WF_2DC_CharNoGet
static u32 func_ov127_021c4354(u32 charaId) {
    return charaId;
}

// pokebw2's WF_2DC_CharNoCheck
static BOOL func_ov127_021c4358(u32 charaId, u32 moveType) {
    return TRUE;
}

// pokebw2's WF_2DC_CharResLoad
static void func_ov127_021c435c(WF_2DCSYS *sys, u32 charaId, u32 vramType, u32 moveType, u32 heapId, BOOL unionChara) {
    if (unionChara == FALSE) {
        func_ov127_021c4380(sys, charaId, vramType, moveType, heapId);
    } else {
        func_ov127_021c43e4(sys, charaId, vramType, moveType, heapId);
    }
}

// pokebw2's WF_2DC_CharResLoadNormal
static void func_ov127_021c4380(WF_2DCSYS *sys, u32 charaId, u32 vramType, u32 moveType, u32 heapId) {
    sys->res[charaId].chars = func_0204a6c8(sys->handles[0], WF_2DC_ARC_CHAR(charaId), FALSE, vramType, heapId);
    sys->res[charaId].palette = func_0204ac18(sys->handles[0], WF_2DC_ARC_PLTT(charaId), vramType, 0x100, 0, 2, heapId);
    sys->res[charaId].vramType = vramType;
    sys->res[charaId].moveType = moveType;
    sys->res[charaId].unionChara = FALSE;
}

// pokebw2's WF_2DC_CharResLoadUnion
static void func_ov127_021c43e4(WF_2DCSYS *sys, u32 charaId, u32 vramType, u32 moveType, u32 heapId) {
    sys->res[charaId].chars = func_0204a6c8(sys->handles[1], WF_2DC_ARC_UNIONCHAR(charaId), FALSE, vramType, heapId);
    sys->res[charaId].palette = sys->unionPltt;
    sys->res[charaId].vramType = vramType;
    sys->res[charaId].moveType = moveType;
    sys->res[charaId].unionChara = TRUE;
}

// pokebw2's WF_2DC_CharResDel
static void func_ov127_021c4424(WF_2DCSYS *sys, u32 charaId) {
    if (sys->res[charaId].unionChara == FALSE) {
        func_ov127_021c4440(sys, charaId);
    } else {
        func_ov127_021c4464(sys, charaId);
    }
}

// pokebw2's WF_2DC_CharResDelNormal
static void func_ov127_021c4440(WF_2DCSYS *sys, u32 charaId) {
    func_0204a8d4(sys->res[charaId].chars);
    func_0204ada4(sys->res[charaId].palette);
    sys->res[charaId].chars = WF_2DC_RES_NONE;
    sys->res[charaId].palette = WF_2DC_RES_NONE;
}

// pokebw2's WF_2DC_CharResDelUnion
static void func_ov127_021c4464(WF_2DCSYS *sys, u32 charaId) {
    func_0204a8d4(sys->res[charaId].chars);
    sys->res[charaId].chars = WF_2DC_RES_NONE;
}

// pokebw2's WF_2DC_CharResCheck
static BOOL func_ov127_021c447c(const WF_2DCSYS *sys, u32 charaId) {
    if (sys->res[charaId].chars != WF_2DC_RES_NONE) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's WF_2DC_ShResLoad
static void func_ov127_021c4494(WF_2DCSYS *sys, u32 vramType, u32 heapId) {
    if (func_ov127_021c447c(sys, WF_2DC_CHARA_HERO) == TRUE) {
        sys->shadowChara = WF_2DC_CHARA_HERO;
    } else if (func_ov127_021c447c(sys, WF_2DC_CHARA_HEROINE) == TRUE) {
        sys->shadowChara = WF_2DC_CHARA_HEROINE;
    }

    sys->shadowRes.cellAnims = func_0204af28(sys->handles[0], WF_2DC_ARC_SHADOW_CELL, WF_2DC_ARC_SHADOW_ANM, heapId);
    sys->shadowRes.chars = func_0204a6c8(sys->handles[0], WF_2DC_ARC_SHADOW_CHAR, FALSE, vramType, heapId);
    sys->shadowRes.palette = func_0204ac18(sys->handles[0], WF_2DC_ARC_SHADOW_PLTT, vramType, 0x140, 0, 1, heapId);
}

// pokebw2's WF_2DC_ShResDel
static void func_ov127_021c4514(WF_2DCSYS *sys) {
    if (func_ov127_021c4550(&sys->shadowRes) == TRUE) {
        func_0204a8d4(sys->shadowRes.chars);
        sys->shadowRes.chars = WF_2DC_RES_NONE;
        func_0204ada4(sys->shadowRes.palette);
        sys->shadowRes.palette = WF_2DC_RES_NONE;
        func_0204afd8(sys->shadowRes.cellAnims);
        sys->shadowRes.cellAnims = WF_2DC_RES_NONE;
    }
}

// pokebw2's WF_2DC_ShResCheck
static BOOL func_ov127_021c4550(const WF_2DCSHRES *res) {
    if (res->chars != WF_2DC_RES_NONE) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's WF_2DC_UnionPlttLoad
static void func_ov127_021c4564(WF_2DCSYS *sys, u32 vramType, u32 plttOffset, u32 heapId) {
    sys->unionPltt =
        func_0204ac18(sys->handles[1], WF_2DC_ARC_UNIONPLTT, vramType, plttOffset, 0, WF_2DC_UNIONPLTT_NUM, heapId);
}

// pokebw2's WF_2DC_UnionPlttDel
static void func_ov127_021c4594(WF_2DCSYS *sys) {
    func_0204ada4(sys->unionPltt);
    sys->unionPltt = WF_2DC_RES_NONE;
}

// pokebw2's WF_2DC_UnionAnmResLoad
static void func_ov127_021c45ac(WF_2DCSYS *sys, u32 heapId) {
    int i;

    for (i = 0; i < WF_2DC_UNIONCHARA_NUM; i++) {
        sys->anmRes[i].cellAnims =
            func_0204af28(sys->handles[1], WF_2DC_ARC_UNIONCELL(i), WF_2DC_ARC_UNIONANM(i), heapId);
    }
}

// pokebw2's WF_2DC_UnionAnmResDel
static void func_ov127_021c45e0(WF_2DCSYS *sys) {
    int i;

    for (i = 0; i < WF_2DC_UNIONCHARA_NUM; i++) {
        func_0204afd8(sys->anmRes[i].cellAnims);
        sys->anmRes[i].cellAnims = WF_2DC_RES_NONE;
    }
}

// pokebw2's WF_2DC_WkCleanGet
static WF_2DCWK *func_ov127_021c4604(WF_2DCSYS *sys) {
    int i;

    for (i = 0; i < sys->objNum; i++) {
        if (func_ov127_021c4790(&sys->objs[i]) == TRUE) {
            return &sys->objs[i];
        }
    }
    return NULL;
}

// pokebw2's WF_2DC_WkPatAnmUpdate
static void func_ov127_021c4634(WF_2DCWK *wk) {
    if (data_ov127_021c5fe4[wk->anmType] != NULL) {
        data_ov127_021c5fe4[wk->anmType](wk);
    }
}

// pokebw2's WF_2DC_WkPatAnmWayInit
static void func_ov127_021c464c(WF_2DCWK *wk, u32 way) {
    wk->way = way;
    func_0204b8a0(wk->actor, way);
}

// pokebw2's WF_2DC_WkPatAnmRotaInit
static void func_ov127_021c465c(WF_2DCWK *wk, u32 way) {
    wk->way = way;
    func_0204b8a0(wk->actor, 4);
}

// pokebw2's WF_2DC_WkPatAnmWalkInit
static void func_ov127_021c466c(WF_2DCWK *wk, u32 way) {
    wk->way = way;
    func_0204b8a0(wk->actor, way + 5);
}

// Finishes in the way the character faced
// pokebw2's WF_2DC_WkPatAnmTurnInit
static void func_ov127_021c4680(WF_2DCWK *wk, u32 way) {
    func_0204b8a0(wk->actor, wk->way + 5);
    wk->way = way;
}

// pokebw2's WF_2DC_WkPatAnmRunInit
static void func_ov127_021c4698(WF_2DCWK *wk, u32 way) {
    wk->way = way;
    func_0204b8a0(wk->actor, way + 9);
}

// pokebw2's WF_2DC_WkPatAnmWallWalkInit
static void func_ov127_021c46ac(WF_2DCWK *wk, u32 way) {
    wk->way = way;
    func_0204b8a0(wk->actor, way + 5);
}

// pokebw2's WF_2DC_WkPatAnmSlowWalkInit
static void func_ov127_021c46c0(WF_2DCWK *wk, u32 way) {
    wk->way = way;
    func_0204b8a0(wk->actor, way + 5);
}

// pokebw2's WF_2DC_WkPatAnmHighWalk2Init
static void func_ov127_021c46d4(WF_2DCWK *wk, u32 way) {
    wk->way = way;
    func_0204b8a0(wk->actor, way + 5);
}

// pokebw2's WF_2DC_WkPatAnmHighWalk4Init
static void func_ov127_021c46e8(WF_2DCWK *wk, u32 way) {
    wk->way = way;
    func_0204b8a0(wk->actor, way + 5);
}

// pokebw2's WF_2DC_WkPatAnmRotaMain
static void func_ov127_021c46fc(WF_2DCWK *wk) {
    func_0204b8d4(wk->actor, 2 * FX32_ONE);
}

// pokebw2's WF_2DC_WkPatAnmWalkMain
static void func_ov127_021c470c(WF_2DCWK *wk) {
    func_0204b8d4(wk->actor, 2 * FX32_ONE);
}

// Turns to the new way after the first frame
// pokebw2's WF_2DC_WkPatAnmTurnMain
static void func_ov127_021c471c(WF_2DCWK *wk) {
    if (wk->frame == 0) {
        func_0204b8d4(wk->actor, 1);
    } else {
        func_0204b878(wk->actor, wk->way + 5);
    }
}

// pokebw2's WF_2DC_WkPatAnmRunMain
static void func_ov127_021c4740(WF_2DCWK *wk) {
    func_0204b8d4(wk->actor, 2 * FX32_ONE);
}

// pokebw2's WF_2DC_WkPatAnmWallWalkMain
static void func_ov127_021c4750(WF_2DCWK *wk) {
    func_0204b8d4(wk->actor, FX32_ONE);
}

// pokebw2's WF_2DC_WkPatAnmSlowWalkMain
static void func_ov127_021c4760(WF_2DCWK *wk) {
    func_0204b8d4(wk->actor, FX32_ONE);
}

// pokebw2's WF_2DC_WkPatAnmHighWalk2Main
static void func_ov127_021c4770(WF_2DCWK *wk) {
    func_0204b8d4(wk->actor, 8 * FX32_ONE);
}

// pokebw2's WF_2DC_WkPatAnmHighWalk4Main
static void func_ov127_021c4780(WF_2DCWK *wk) {
    func_0204b8d4(wk->actor, 6 * FX32_ONE);
}

// pokebw2's WF_2DC_WkCleanCheck
static BOOL func_ov127_021c4790(const WF_2DCWK *wk) {
    if (wk->actor == NULL) {
        return TRUE;
    }
    return FALSE;
}
