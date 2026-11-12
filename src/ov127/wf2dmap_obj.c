// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov203/wf2dmap_obj.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/wf2dmap/wf2dmap_common.h"
#include "app/wf2dmap/wf2dmap_obj.h"
#include "gfl/heap.h"
#include "gfl/std.h"

// The objects of the Wi-Fi Club's 2D map, named after the ROM's "wf2dmap_obj.c"

struct WF2DMAP_OBJWK {
    BOOL used;
    WF2DMAP_POS pos;
    WF2DMAP_POS lastPos;
    u16 playId;
    u16 charaId;
    u16 frame;
    u16 endFrame;
    u8 way;
    u8 status;
    u16 statusFrame;
};

struct WF2DMAP_OBJSYS {
    WF2DMAP_OBJWK *wk;
    u32 num;
};

typedef struct {
    s8 x;
    s8 y;
} WF2DMAP_OBJWAYPOS;

static WF2DMAP_OBJWK *func_ov127_021c4a98(WF2DMAP_OBJSYS *sys);
static void func_ov127_021c4c88(WF2DMAP_OBJWK *wk, const WF2DMAP_ACTCMD *act);
static void func_ov127_021c4cb4(WF2DMAP_OBJWK *wk);
static void func_ov127_021c4ce4(WF2DMAP_OBJWK *wk, WF2DMAP_POS pos, u32 way);
static void func_ov127_021c4d34(WF2DMAP_OBJWK *wk, WF2DMAP_POS pos, u32 way);
static void func_ov127_021c4d98(WF2DMAP_OBJWK *wk, WF2DMAP_POS pos, u32 way);
static void func_ov127_021c4e2c(WF2DMAP_OBJWK *wk, WF2DMAP_POS pos, u32 way);
static void func_ov127_021c4ec0(WF2DMAP_OBJWK *wk, WF2DMAP_POS pos, u32 way);
static void func_ov127_021c4f10(WF2DMAP_OBJWK *wk, WF2DMAP_POS pos, u32 way);
static void func_ov127_021c4f74(WF2DMAP_OBJWK *wk, WF2DMAP_POS pos, u32 way);
static void func_ov127_021c5008(WF2DMAP_OBJWK *wk, WF2DMAP_POS pos, u32 way);
static void func_ov127_021c509c(WF2DMAP_OBJWK *wk, WF2DMAP_POS pos, u32 way);
static void func_ov127_021c5100(WF2DMAP_OBJWK *wk, WF2DMAP_POS pos, u32 way);
static void func_ov127_021c5164(WF2DMAP_OBJWK *wk, WF2DMAP_POS pos, u32 way);
static void func_ov127_021c51c8(WF2DMAP_OBJWK *wk, WF2DMAP_POS pos, u32 way);
static BOOL func_ov127_021c522c(WF2DMAP_OBJWK *wk);
static BOOL func_ov127_021c5230(WF2DMAP_OBJWK *wk);
static BOOL func_ov127_021c5238(WF2DMAP_OBJWK *wk);
static BOOL func_ov127_021c5278(WF2DMAP_OBJWK *wk);
static BOOL func_ov127_021c52b8(WF2DMAP_OBJWK *wk);
static BOOL func_ov127_021c52bc(WF2DMAP_OBJWK *wk);
static BOOL func_ov127_021c52c4(WF2DMAP_OBJWK *wk);
static WF2DMAP_POS func_ov127_021c52cc(WF2DMAP_POS pos, u32 way);
static BOOL func_ov127_021c5300(WF2DMAP_OBJWK *wk);
static void func_ov127_021c5334(WF2DMAP_OBJWK *wk);

// pokebw2's sWF2DMAP_OBJRetWay
static const u8 data_ov127_021c6010[WF2DMAP_WAY_NUM] = {
    WF2DMAP_WAY_DOWN,
    WF2DMAP_WAY_UP,
    WF2DMAP_WAY_RIGHT,
    WF2DMAP_WAY_LEFT,
};

// pokebw2's sWF2DMAP_OBJWayPos
static const WF2DMAP_OBJWAYPOS data_ov127_021c6014[WF2DMAP_WAY_NUM] = {
    { 0, -WF2DMAP_GRID_SIZE },
    { 0, WF2DMAP_GRID_SIZE },
    { -WF2DMAP_GRID_SIZE, 0 },
    { WF2DMAP_GRID_SIZE, 0 },
};

// pokebw2's sWF2DMAP_OBJStInitFuncs
static void (*const data_ov127_021c601c[WF2DMAP_OBJST_NUM])(WF2DMAP_OBJWK *wk, WF2DMAP_POS pos, u32 way) = {
    func_ov127_021c4ce4,      func_ov127_021c4d34,      func_ov127_021c4d98,
    func_ov127_021c4e2c,       func_ov127_021c4ec0,      func_ov127_021c4f10,
    func_ov127_021c4f74,  func_ov127_021c5008,     func_ov127_021c509c,
    func_ov127_021c5100, func_ov127_021c5164, func_ov127_021c51c8,
};

// Each returns TRUE when the state is over
// pokebw2's sWF2DMAP_OBJStMainFuncs
static BOOL (*const data_ov127_021c604c[WF2DMAP_OBJST_NUM])(WF2DMAP_OBJWK *wk) = {
    func_ov127_021c522c,     func_ov127_021c5230,     func_ov127_021c5238,
    func_ov127_021c5278,      func_ov127_021c52b8,     func_ov127_021c52bc,
    func_ov127_021c5238,     func_ov127_021c5238,     func_ov127_021c52c4,
    func_ov127_021c52c4, func_ov127_021c52c4, func_ov127_021c52c4,
};

// pokebw2's WF2DMAP_OBJSysInit
WF2DMAP_OBJSYS *func_ov127_021c4880(u32 objNum, u32 heapId) {
    WF2DMAP_OBJSYS *sys;

    sys = GFL_HeapAllocate(heapId, sizeof(WF2DMAP_OBJSYS), TRUE, "wf2dmap_obj.c", 128);
    sys->num = objNum;
    sys->wk = GFL_HeapAllocate(heapId, sizeof(WF2DMAP_OBJWK) * sys->num, TRUE, "wf2dmap_obj.c", 133);
    return sys;
}

// pokebw2's WF2DMAP_OBJSysExit
void func_ov127_021c48c0(WF2DMAP_OBJSYS *sys) {
    GFL_HeapFree(sys->wk);
    GFL_HeapFree(sys);
}

// pokebw2's WF2DMAP_OBJSysCmdSet
void func_ov127_021c48d4(WF2DMAP_OBJSYS *sys, const WF2DMAP_ACTCMD *act) {
    WF2DMAP_OBJWK *wk;

    wk = func_ov127_021c4954(sys, act->playId);
    func_ov127_021c4c88(wk, act);
}

// pokebw2's WF2DMAP_OBJSysMain
void func_ov127_021c48e8(WF2DMAP_OBJSYS *sys) {
    u32 i;
    u32 num;
    WF2DMAP_OBJWK *wk;

    num = func_ov127_021c4914(sys);
    for (i = 0; i < num; i++) {
        wk = func_ov127_021c4980(sys, i);
        if (wk != NULL) {
            func_ov127_021c4cb4(wk);
        }
    }
}

// pokebw2's WF2DMAP_OBJSysObjNumGet
u32 func_ov127_021c4914(const WF2DMAP_OBJSYS *sys) {
    return sys->num;
}

// pokebw2's WF2DMAP_OBJWkNew
WF2DMAP_OBJWK *func_ov127_021c4918(WF2DMAP_OBJSYS *sys, const WF2DMAP_OBJDATA *data) {
    WF2DMAP_OBJWK *wk;

    wk = func_ov127_021c4a98(sys);
    wk->pos.x = data->x;
    wk->pos.y = data->y;
    wk->lastPos.x = data->x;
    wk->lastPos.y = data->y;
    wk->playId = data->playId;
    wk->status = data->status;
    wk->way = data->way;
    wk->charaId = data->charaId;
    wk->used = TRUE;
    return wk;
}

// pokebw2's WF2DMAP_OBJWkDel
void func_ov127_021c4948(WF2DMAP_OBJWK *wk) {
    MI_CpuFill8(wk, 0, sizeof(WF2DMAP_OBJWK));
}

// pokebw2's WF2DMAP_OBJWkGet
WF2DMAP_OBJWK *func_ov127_021c4954(WF2DMAP_OBJSYS *sys, u16 playId) {
    int i;

    for (i = 0; i < sys->num; i++) {
        if (sys->wk[i].used == TRUE) {
            if (sys->wk[i].playId == playId) {
                return &sys->wk[i];
            }
        }
    }
    return NULL;
}

// pokebw2's WF2DMAP_OBJWkIdxGet
WF2DMAP_OBJWK *func_ov127_021c4980(WF2DMAP_OBJSYS *sys, u16 index) {
    if (sys->wk[index].used == TRUE) {
        return &sys->wk[index];
    }
    return NULL;
}

// pokebw2's WF2DMAP_OBJWkConstGet
const WF2DMAP_OBJWK *func_ov127_021c4994(const WF2DMAP_OBJSYS *sys, u16 playId) {
    return func_ov127_021c4954((WF2DMAP_OBJSYS *)sys, playId);
}

// pokebw2's WF2DMAP_OBJWkConstIdxGet
const WF2DMAP_OBJWK *func_ov127_021c499c(const WF2DMAP_OBJSYS *sys, u16 index) {
    return func_ov127_021c4980((WF2DMAP_OBJSYS *)sys, index);
}

// pokebw2's WF2DMAP_OBJWkDataGet
s32 func_ov127_021c49a4(const WF2DMAP_OBJWK *wk, u32 id) {
    s32 data;

    switch (id) {
    case WF2DMAP_OBJPM_X:
        data = wk->pos.x;
        break;
    case WF2DMAP_OBJPM_Y:
        data = wk->pos.y;
        break;
    case WF2DMAP_OBJPM_LX:
        data = wk->lastPos.x;
        break;
    case WF2DMAP_OBJPM_LY:
        data = wk->lastPos.y;
        break;
    case WF2DMAP_OBJPM_PLID:
        data = wk->playId;
        break;
    case WF2DMAP_OBJPM_ST:
        data = wk->status;
        break;
    case WF2DMAP_OBJPM_WAY:
        data = wk->way;
        break;
    case WF2DMAP_OBJPM_CHARA:
        data = wk->charaId;
        break;
    case WF2DMAP_OBJPM_FRAME:
        data = wk->frame;
        break;
    case WF2DMAP_OBJPM_ENDFRAME:
        data = wk->endFrame;
        break;
    case WF2DMAP_OBJPM_STFRAME:
        data = wk->statusFrame;
        break;
    }
    return data;
}

// pokebw2's WF2DMAP_OBJWkDataSet
void func_ov127_021c4a00(WF2DMAP_OBJWK *wk, u32 id, s32 data) {
    switch (id) {
    case WF2DMAP_OBJPM_X:
        wk->pos.x = data;
        break;
    case WF2DMAP_OBJPM_Y:
        wk->pos.y = data;
        break;
    case WF2DMAP_OBJPM_LX:
        wk->lastPos.x = data;
        break;
    case WF2DMAP_OBJPM_LY:
        wk->lastPos.y = data;
        break;
    case WF2DMAP_OBJPM_PLID:
        wk->playId = data;
        break;
    case WF2DMAP_OBJPM_ST:
        wk->status = data;
        break;
    case WF2DMAP_OBJPM_WAY:
        wk->way = data;
        break;
    case WF2DMAP_OBJPM_CHARA:
        wk->charaId = data;
        break;
    case WF2DMAP_OBJPM_FRAME:
        wk->frame = data;
        break;
    case WF2DMAP_OBJPM_ENDFRAME:
        wk->endFrame = data;
        break;
    case WF2DMAP_OBJPM_STFRAME:
        wk->statusFrame = data;
        break;
    }
}

// pokebw2's WF2DMAP_OBJToolWayPosGet
WF2DMAP_POS func_ov127_021c4a54(WF2DMAP_POS pos, u32 way) {
    pos.x += data_ov127_021c6014[way].x;
    pos.y += data_ov127_021c6014[way].y;
    return pos;
}

// pokebw2's WF2DMAP_OBJToolRetWayGet
u32 func_ov127_021c4a8c(u32 way) {
    return data_ov127_021c6010[way];
}

// pokebw2's WF2DMAP_OBJSysCleanWkGet
static WF2DMAP_OBJWK *func_ov127_021c4a98(WF2DMAP_OBJSYS *sys) {
    int i;

    for (i = 0; i < sys->num; i++) {
        if (sys->wk[i].used == FALSE) {
            return &sys->wk[i];
        }
    }
    return NULL;
}

// pokebw2's WF2DMAP_OBJWkMatrixGet
WF2DMAP_POS func_ov127_021c4ac4(const WF2DMAP_OBJWK *wk) {
    return wk->pos;
}

// pokebw2's WF2DMAP_OBJWkLastMatrixGet
WF2DMAP_POS func_ov127_021c4ad0(const WF2DMAP_OBJWK *wk) {
    return wk->lastPos;
}

// pokebw2's WF2DMAP_OBJWkFrameMatrixGet
WF2DMAP_POS func_ov127_021c4adc(const WF2DMAP_OBJWK *wk) {
    WF2DMAP_POS pos;
    WF2DMAP_POS lastPos;
    s32 frame;
    s32 endFrame;

    frame = func_ov127_021c49a4(wk, WF2DMAP_OBJPM_FRAME);
    endFrame = func_ov127_021c49a4(wk, WF2DMAP_OBJPM_ENDFRAME);
    pos = func_ov127_021c4ac4(wk);
    lastPos = func_ov127_021c4ad0(wk);

    pos.x -= lastPos.x;
    pos.y -= lastPos.y;
    if (frame > 0) {
        pos.x = (frame * pos.x) / endFrame;
        pos.y = (frame * pos.y) / endFrame;
    } else {
        pos.x = 0;
        pos.y = 0;
    }
    pos.x += lastPos.x;
    pos.y += lastPos.y;
    return pos;
}

// pokebw2's WF2DMAP_OBJWkMatrixSet
void func_ov127_021c4b7c(WF2DMAP_OBJWK *wk, WF2DMAP_POS pos) {
    wk->pos = pos;
}

// pokebw2's WF2DMAP_OBJWkLastMatrixSet
void func_ov127_021c4b90(WF2DMAP_OBJWK *wk, WF2DMAP_POS pos) {
    wk->lastPos = pos;
}

// pokebw2's WF2DMAP_OBJSysHitCheck
const WF2DMAP_OBJWK *func_ov127_021c4ba4(const WF2DMAP_OBJWK *wk, const WF2DMAP_OBJSYS *sys, u32 way) {
    WF2DMAP_POS pos;
    WF2DMAP_POS objPos;
    const WF2DMAP_OBJWK *obj;
    u32 num;
    s32 playId;
    s32 objPlayId;
    int i;

    pos = func_ov127_021c4ac4(wk);
    pos = func_ov127_021c4a54(pos, way);
    playId = func_ov127_021c49a4(wk, WF2DMAP_OBJPM_PLID);
    num = func_ov127_021c4914(sys);

    for (i = 0; i < num; i++) {
        obj = func_ov127_021c499c(sys, i);
        if (obj != NULL) {
            objPlayId = func_ov127_021c49a4(obj, WF2DMAP_OBJPM_PLID);
            if (objPlayId != playId) {
                objPos = func_ov127_021c4ac4(obj);
                if (objPos.x == pos.x && objPos.y == pos.y) {
                    return obj;
                }
                objPos = func_ov127_021c4ad0(obj);
                if (objPos.x == pos.x && objPos.y == pos.y) {
                    return obj;
                }
            }
        }
    }
    return NULL;
}

// pokebw2's WF2DMAP_OBJWkCmdSet
static void func_ov127_021c4c88(WF2DMAP_OBJWK *wk, const WF2DMAP_ACTCMD *act) {
    data_ov127_021c601c[act->cmd](wk, act->pos, act->way);
    wk->statusFrame = 0;
}

// pokebw2's WF2DMAP_OBJWkMain
static void func_ov127_021c4cb4(WF2DMAP_OBJWK *wk) {
    s32 status;
    BOOL end;

    status = func_ov127_021c49a4(wk, WF2DMAP_OBJPM_ST);
    end = data_ov127_021c604c[status](wk);
    wk->statusFrame++;
    if (end == TRUE) {
        func_ov127_021c5334(wk);
        wk->statusFrame = 0;
    }
}

// pokebw2's WF2DMAP_OBJWkStInit_None
static void func_ov127_021c4ce4(WF2DMAP_OBJWK *wk, WF2DMAP_POS pos, u32 way) {
    func_ov127_021c4b7c(wk, pos);
    func_ov127_021c4b90(wk, pos);
    func_ov127_021c4a00(wk, WF2DMAP_OBJPM_WAY, way);
    func_ov127_021c4a00(wk, WF2DMAP_OBJPM_ST, WF2DMAP_OBJST_NONE);
}

// pokebw2's WF2DMAP_OBJWkStInit_Turn
static void func_ov127_021c4d34(WF2DMAP_OBJWK *wk, WF2DMAP_POS pos, u32 way) {
    func_ov127_021c4b7c(wk, pos);
    func_ov127_021c4b90(wk, pos);
    func_ov127_021c4a00(wk, WF2DMAP_OBJPM_WAY, way);
    func_ov127_021c4a00(wk, WF2DMAP_OBJPM_ST, WF2DMAP_OBJST_TURN);
    func_ov127_021c4a00(wk, WF2DMAP_OBJPM_FRAME, 0);
    func_ov127_021c4a00(wk, WF2DMAP_OBJPM_ENDFRAME, 2);
}

// pokebw2's WF2DMAP_OBJWkStInit_Walk
static void func_ov127_021c4d98(WF2DMAP_OBJWK *wk, WF2DMAP_POS pos, u32 way) {
    WF2DMAP_POS wayPos;

    wayPos = func_ov127_021c52cc(pos, way);
    func_ov127_021c4b90(wk, pos);
    func_ov127_021c4b7c(wk, wayPos);
    func_ov127_021c4a00(wk, WF2DMAP_OBJPM_WAY, way);
    func_ov127_021c4a00(wk, WF2DMAP_OBJPM_ST, WF2DMAP_OBJST_WALK);
    func_ov127_021c4a00(wk, WF2DMAP_OBJPM_FRAME, 0);
    func_ov127_021c4a00(wk, WF2DMAP_OBJPM_ENDFRAME, 8);
}

// pokebw2's WF2DMAP_OBJWkStInit_Run
static void func_ov127_021c4e2c(WF2DMAP_OBJWK *wk, WF2DMAP_POS pos, u32 way) {
    WF2DMAP_POS wayPos;

    wayPos = func_ov127_021c52cc(pos, way);
    func_ov127_021c4b90(wk, pos);
    func_ov127_021c4b7c(wk, wayPos);
    func_ov127_021c4a00(wk, WF2DMAP_OBJPM_WAY, way);
    func_ov127_021c4a00(wk, WF2DMAP_OBJPM_ST, WF2DMAP_OBJST_RUN);
    func_ov127_021c4a00(wk, WF2DMAP_OBJPM_FRAME, 0);
    func_ov127_021c4a00(wk, WF2DMAP_OBJPM_ENDFRAME, 4);
}

// pokebw2's WF2DMAP_OBJWkStInit_Busy
static void func_ov127_021c4ec0(WF2DMAP_OBJWK *wk, WF2DMAP_POS pos, u32 way) {
    func_ov127_021c4b7c(wk, pos);
    func_ov127_021c4b90(wk, pos);
    func_ov127_021c4a00(wk, WF2DMAP_OBJPM_WAY, way);
    func_ov127_021c4a00(wk, WF2DMAP_OBJPM_ST, WF2DMAP_OBJST_BUSY);
}

// pokebw2's WF2DMAP_OBJWkStInit_WallWalk
static void func_ov127_021c4f10(WF2DMAP_OBJWK *wk, WF2DMAP_POS pos, u32 way) {
    func_ov127_021c4b7c(wk, pos);
    func_ov127_021c4b90(wk, pos);
    func_ov127_021c4a00(wk, WF2DMAP_OBJPM_WAY, way);
    func_ov127_021c4a00(wk, WF2DMAP_OBJPM_ST, WF2DMAP_OBJST_WALLWALK);
    func_ov127_021c4a00(wk, WF2DMAP_OBJPM_FRAME, 0);
    func_ov127_021c4a00(wk, WF2DMAP_OBJPM_ENDFRAME, 16);
}

// pokebw2's WF2DMAP_OBJWkStInit_SlowWalk
static void func_ov127_021c4f74(WF2DMAP_OBJWK *wk, WF2DMAP_POS pos, u32 way) {
    WF2DMAP_POS wayPos;

    wayPos = func_ov127_021c52cc(pos, way);
    func_ov127_021c4b90(wk, pos);
    func_ov127_021c4b7c(wk, wayPos);
    func_ov127_021c4a00(wk, WF2DMAP_OBJPM_WAY, way);
    func_ov127_021c4a00(wk, WF2DMAP_OBJPM_ST, WF2DMAP_OBJST_SLOWWALK);
    func_ov127_021c4a00(wk, WF2DMAP_OBJPM_FRAME, 0);
    func_ov127_021c4a00(wk, WF2DMAP_OBJPM_ENDFRAME, 16);
}

// pokebw2's WF2DMAP_OBJWkStInit_Walk4
static void func_ov127_021c5008(WF2DMAP_OBJWK *wk, WF2DMAP_POS pos, u32 way) {
    WF2DMAP_POS wayPos;

    wayPos = func_ov127_021c52cc(pos, way);
    func_ov127_021c4b90(wk, pos);
    func_ov127_021c4b7c(wk, wayPos);
    func_ov127_021c4a00(wk, WF2DMAP_OBJPM_WAY, way);
    func_ov127_021c4a00(wk, WF2DMAP_OBJPM_ST, WF2DMAP_OBJST_WALK4);
    func_ov127_021c4a00(wk, WF2DMAP_OBJPM_FRAME, 0);
    func_ov127_021c4a00(wk, WF2DMAP_OBJPM_ENDFRAME, 4);
}

// pokebw2's WF2DMAP_OBJWkStInit_StayWalk2
static void func_ov127_021c509c(WF2DMAP_OBJWK *wk, WF2DMAP_POS pos, u32 way) {
    func_ov127_021c4b90(wk, pos);
    func_ov127_021c4b7c(wk, pos);
    func_ov127_021c4a00(wk, WF2DMAP_OBJPM_WAY, way);
    func_ov127_021c4a00(wk, WF2DMAP_OBJPM_ST, WF2DMAP_OBJST_STAYWALK2);
    func_ov127_021c4a00(wk, WF2DMAP_OBJPM_FRAME, 0);
    func_ov127_021c4a00(wk, WF2DMAP_OBJPM_ENDFRAME, 2);
}

// pokebw2's WF2DMAP_OBJWkStInit_StayWalk4
static void func_ov127_021c5100(WF2DMAP_OBJWK *wk, WF2DMAP_POS pos, u32 way) {
    func_ov127_021c4b90(wk, pos);
    func_ov127_021c4b7c(wk, pos);
    func_ov127_021c4a00(wk, WF2DMAP_OBJPM_WAY, way);
    func_ov127_021c4a00(wk, WF2DMAP_OBJPM_ST, WF2DMAP_OBJST_STAYWALK4);
    func_ov127_021c4a00(wk, WF2DMAP_OBJPM_FRAME, 0);
    func_ov127_021c4a00(wk, WF2DMAP_OBJPM_ENDFRAME, 4);
}

// pokebw2's WF2DMAP_OBJWkStInit_StayWalk8
static void func_ov127_021c5164(WF2DMAP_OBJWK *wk, WF2DMAP_POS pos, u32 way) {
    func_ov127_021c4b90(wk, pos);
    func_ov127_021c4b7c(wk, pos);
    func_ov127_021c4a00(wk, WF2DMAP_OBJPM_WAY, way);
    func_ov127_021c4a00(wk, WF2DMAP_OBJPM_ST, WF2DMAP_OBJST_STAYWALK8);
    func_ov127_021c4a00(wk, WF2DMAP_OBJPM_FRAME, 0);
    func_ov127_021c4a00(wk, WF2DMAP_OBJPM_ENDFRAME, 8);
}

// pokebw2's WF2DMAP_OBJWkStInit_StayWalk16
static void func_ov127_021c51c8(WF2DMAP_OBJWK *wk, WF2DMAP_POS pos, u32 way) {
    func_ov127_021c4b90(wk, pos);
    func_ov127_021c4b7c(wk, pos);
    func_ov127_021c4a00(wk, WF2DMAP_OBJPM_WAY, way);
    func_ov127_021c4a00(wk, WF2DMAP_OBJPM_ST, WF2DMAP_OBJST_STAYWALK16);
    func_ov127_021c4a00(wk, WF2DMAP_OBJPM_FRAME, 0);
    func_ov127_021c4a00(wk, WF2DMAP_OBJPM_ENDFRAME, 16);
}

// pokebw2's WF2DMAP_OBJWkStMain_None
static BOOL func_ov127_021c522c(WF2DMAP_OBJWK *wk) {
    return FALSE;
}

// pokebw2's WF2DMAP_OBJWkStMain_Turn
static BOOL func_ov127_021c5230(WF2DMAP_OBJWK *wk) {
    return func_ov127_021c5300(wk);
}

// Arrives when the frames are over
// pokebw2's WF2DMAP_OBJWkStMain_Walk
static BOOL func_ov127_021c5238(WF2DMAP_OBJWK *wk) {
    BOOL end;
    WF2DMAP_POS pos;

    end = func_ov127_021c5300(wk);
    if (end == TRUE) {
        pos = func_ov127_021c4ac4(wk);
        func_ov127_021c4b90(wk, pos);
    }
    return end;
}

// pokebw2's WF2DMAP_OBJWkStMain_Run
static BOOL func_ov127_021c5278(WF2DMAP_OBJWK *wk) {
    BOOL end;
    WF2DMAP_POS pos;

    end = func_ov127_021c5300(wk);
    if (end == TRUE) {
        pos = func_ov127_021c4ac4(wk);
        func_ov127_021c4b90(wk, pos);
    }
    return end;
}

// pokebw2's WF2DMAP_OBJWkStMain_Busy
static BOOL func_ov127_021c52b8(WF2DMAP_OBJWK *wk) {
    return FALSE;
}

// pokebw2's WF2DMAP_OBJWkStMain_WallWalk
static BOOL func_ov127_021c52bc(WF2DMAP_OBJWK *wk) {
    return func_ov127_021c5300(wk);
}

// pokebw2's WF2DMAP_OBJWkStMain_StayWalk
static BOOL func_ov127_021c52c4(WF2DMAP_OBJWK *wk) {
    return func_ov127_021c5300(wk);
}

// pokebw2's WF2DMAP_OBJWayPosGet
static WF2DMAP_POS func_ov127_021c52cc(WF2DMAP_POS pos, u32 way) {
    return func_ov127_021c4a54(pos, way);
}

// Returns TRUE at the state's last frame
// pokebw2's WF2DMAP_OBJWkFrameCount
static BOOL func_ov127_021c5300(WF2DMAP_OBJWK *wk) {
    s32 frame;
    s32 endFrame;

    frame = func_ov127_021c49a4(wk, WF2DMAP_OBJPM_FRAME);
    endFrame = func_ov127_021c49a4(wk, WF2DMAP_OBJPM_ENDFRAME);
    if (frame < endFrame) {
        func_ov127_021c4a00(wk, WF2DMAP_OBJPM_FRAME, frame + 1);
        if (frame + 1 < endFrame) {
            return FALSE;
        }
    }
    return TRUE;
}

// Stands where it is
// pokebw2's WF2DMAP_OBJWkStEnd
static void func_ov127_021c5334(WF2DMAP_OBJWK *wk) {
    WF2DMAP_POS pos;
    s32 way;

    pos = func_ov127_021c4ac4(wk);
    way = func_ov127_021c49a4(wk, WF2DMAP_OBJPM_WAY);
    func_ov127_021c4ce4(wk, pos, way);
}
