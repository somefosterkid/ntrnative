// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov203/wf2dmap_judge.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/wf2dmap/wf2dmap_common.h"
#include "app/wf2dmap/wf2dmap_judge.h"
#include "app/wf2dmap/wf2dmap_map.h"
#include "app/wf2dmap/wf2dmap_obj.h"

// Decides whether an object of the Wi-Fi Club's 2D map can do what its player asks. The ROM has no name for this
// file: it is named after the older games' wf2dmap_judge.c

// How long an object has to stand before it walks against a wall
#define WF2DMAP_JUDGE_WALLWALK_FRAME 8

static BOOL func_ov127_021c539c(const WF2DMAP_OBJWK *wk, const WF2DMAP_MAPSYS *mapSys,
                                      const WF2DMAP_OBJSYS *objSys, const WF2DMAP_REQCMD *req, WF2DMAP_ACTCMD *act);
static BOOL func_ov127_021c53c8(const WF2DMAP_OBJWK *wk, const WF2DMAP_MAPSYS *mapSys,
                                      const WF2DMAP_OBJSYS *objSys, const WF2DMAP_REQCMD *req, WF2DMAP_ACTCMD *act);
static BOOL func_ov127_021c5404(const WF2DMAP_OBJWK *wk, const WF2DMAP_MAPSYS *mapSys,
                                      const WF2DMAP_OBJSYS *objSys, const WF2DMAP_REQCMD *req, WF2DMAP_ACTCMD *act);
static BOOL func_ov127_021c5478(const WF2DMAP_OBJWK *wk, const WF2DMAP_MAPSYS *mapSys,
                                     const WF2DMAP_OBJSYS *objSys, const WF2DMAP_REQCMD *req, WF2DMAP_ACTCMD *act);
static BOOL func_ov127_021c5488(const WF2DMAP_OBJWK *wk, const WF2DMAP_MAPSYS *mapSys,
                                      const WF2DMAP_OBJSYS *objSys, const WF2DMAP_REQCMD *req, WF2DMAP_ACTCMD *act);
static BOOL func_ov127_021c54b0(const WF2DMAP_OBJWK *wk, const WF2DMAP_MAPSYS *mapSys,
                                          const WF2DMAP_OBJSYS *objSys, const WF2DMAP_REQCMD *req, WF2DMAP_ACTCMD *act);
static BOOL func_ov127_021c54b4(const WF2DMAP_OBJWK *wk, const WF2DMAP_MAPSYS *mapSys,
                                          const WF2DMAP_OBJSYS *objSys, const WF2DMAP_REQCMD *req, WF2DMAP_ACTCMD *act);
static BOOL func_ov127_021c54c4(const WF2DMAP_OBJWK *wk, const WF2DMAP_MAPSYS *mapSys,
                                          const WF2DMAP_OBJSYS *objSys, const WF2DMAP_REQCMD *req, WF2DMAP_ACTCMD *act);
static BOOL func_ov127_021c54ec(const WF2DMAP_OBJWK *wk, const WF2DMAP_MAPSYS *mapSys, u32 way);
static BOOL func_ov127_021c5550(const WF2DMAP_OBJWK *wk, const WF2DMAP_OBJSYS *objSys, u32 way);
static void func_ov127_021c5564(WF2DMAP_ACTCMD *act, const WF2DMAP_OBJWK *wk, u32 cmd, u32 playId, u32 way);

// By the request's command, a WF2DMAP_OBJST_*
// pokebw2's sWF2DMAP_JUDGEFuncs
static BOOL (*const data_ov127_021c607c[WF2DMAP_OBJST_NUM])(const WF2DMAP_OBJWK *wk, const WF2DMAP_MAPSYS *mapSys,
                                                            const WF2DMAP_OBJSYS *objSys, const WF2DMAP_REQCMD *req,
                                                            WF2DMAP_ACTCMD *act) = {
    func_ov127_021c539c,     func_ov127_021c53c8,     func_ov127_021c5404,
    func_ov127_021c5478,      func_ov127_021c5488,     func_ov127_021c54b0,
    func_ov127_021c54b4, func_ov127_021c5404,     func_ov127_021c54c4,
    func_ov127_021c54c4, func_ov127_021c54c4, func_ov127_021c54c4,
};

BOOL func_ov127_021c5370(const WF2DMAP_MAPSYS *mapSys, const WF2DMAP_OBJSYS *objSys, const WF2DMAP_REQCMD *req,
                              WF2DMAP_ACTCMD *act) {
    const WF2DMAP_OBJWK *wk;

    wk = func_ov127_021c4994(objSys, req->playId);
    return data_ov127_021c607c[req->cmd](wk, mapSys, objSys, req, act);
}

static BOOL func_ov127_021c539c(const WF2DMAP_OBJWK *wk, const WF2DMAP_MAPSYS *mapSys,
                                      const WF2DMAP_OBJSYS *objSys, const WF2DMAP_REQCMD *req, WF2DMAP_ACTCMD *act) {
    s32 status;

    status = func_ov127_021c49a4(wk, WF2DMAP_OBJPM_ST);
    if (status == WF2DMAP_OBJST_NONE || status == WF2DMAP_OBJST_BUSY) {
        func_ov127_021c5564(act, wk, req->cmd, req->playId, req->way);
        return TRUE;
    }
    return FALSE;
}

static BOOL func_ov127_021c53c8(const WF2DMAP_OBJWK *wk, const WF2DMAP_MAPSYS *mapSys,
                                      const WF2DMAP_OBJSYS *objSys, const WF2DMAP_REQCMD *req, WF2DMAP_ACTCMD *act) {
    s32 status;
    s32 way;

    status = func_ov127_021c49a4(wk, WF2DMAP_OBJPM_ST);
    way = func_ov127_021c49a4(wk, WF2DMAP_OBJPM_WAY);
    if (status == WF2DMAP_OBJST_NONE && way != req->way) {
        func_ov127_021c5564(act, wk, req->cmd, req->playId, req->way);
        return TRUE;
    }
    return FALSE;
}

// An object that has stood facing a wall for a while walks against it instead
static BOOL func_ov127_021c5404(const WF2DMAP_OBJWK *wk, const WF2DMAP_MAPSYS *mapSys,
                                      const WF2DMAP_OBJSYS *objSys, const WF2DMAP_REQCMD *req, WF2DMAP_ACTCMD *act) {
    s32 status;

    status = func_ov127_021c49a4(wk, WF2DMAP_OBJPM_ST);
    if (status == WF2DMAP_OBJST_NONE) {
        if (func_ov127_021c54ec(wk, mapSys, req->way) != TRUE) {
            if (func_ov127_021c5550(wk, objSys, req->way) != TRUE) {
                func_ov127_021c5564(act, wk, req->cmd, req->playId, req->way);
                return TRUE;
            }
        }
    }

    if (status == WF2DMAP_OBJST_NONE) {
        if (func_ov127_021c49a4(wk, WF2DMAP_OBJPM_STFRAME) >= WF2DMAP_JUDGE_WALLWALK_FRAME) {
            func_ov127_021c5564(act, wk, WF2DMAP_OBJST_WALLWALK, req->playId, req->way);
            return TRUE;
        }
    }
    return FALSE;
}

static BOOL func_ov127_021c5478(const WF2DMAP_OBJWK *wk, const WF2DMAP_MAPSYS *mapSys,
                                     const WF2DMAP_OBJSYS *objSys, const WF2DMAP_REQCMD *req, WF2DMAP_ACTCMD *act) {
    return func_ov127_021c5404(wk, mapSys, objSys, req, act);
}

static BOOL func_ov127_021c5488(const WF2DMAP_OBJWK *wk, const WF2DMAP_MAPSYS *mapSys,
                                      const WF2DMAP_OBJSYS *objSys, const WF2DMAP_REQCMD *req, WF2DMAP_ACTCMD *act) {
    s32 status;

    status = func_ov127_021c49a4(wk, WF2DMAP_OBJPM_ST);
    if (status == WF2DMAP_OBJST_NONE) {
        func_ov127_021c5564(act, wk, req->cmd, req->playId, req->way);
        return TRUE;
    }
    return FALSE;
}

static BOOL func_ov127_021c54b0(const WF2DMAP_OBJWK *wk, const WF2DMAP_MAPSYS *mapSys,
                                          const WF2DMAP_OBJSYS *objSys, const WF2DMAP_REQCMD *req,
                                          WF2DMAP_ACTCMD *act) {
    return FALSE;
}

static BOOL func_ov127_021c54b4(const WF2DMAP_OBJWK *wk, const WF2DMAP_MAPSYS *mapSys,
                                          const WF2DMAP_OBJSYS *objSys, const WF2DMAP_REQCMD *req,
                                          WF2DMAP_ACTCMD *act) {
    return func_ov127_021c5404(wk, mapSys, objSys, req, act);
}

static BOOL func_ov127_021c54c4(const WF2DMAP_OBJWK *wk, const WF2DMAP_MAPSYS *mapSys,
                                          const WF2DMAP_OBJSYS *objSys, const WF2DMAP_REQCMD *req,
                                          WF2DMAP_ACTCMD *act) {
    s32 status;

    status = func_ov127_021c49a4(wk, WF2DMAP_OBJPM_ST);
    if (status == WF2DMAP_OBJST_NONE) {
        func_ov127_021c5564(act, wk, req->cmd, req->playId, req->way);
        return TRUE;
    }
    return FALSE;
}

// pokebw2's WF2DMAP_JUDGESysMapHitCheck
static BOOL func_ov127_021c54ec(const WF2DMAP_OBJWK *wk, const WF2DMAP_MAPSYS *mapSys, u32 way) {
    WF2DMAP_POS pos;

    pos = func_ov127_021c4ac4(wk);
    pos = func_ov127_021c4a54(pos, way);
    return func_ov127_021c4844(mapSys, pos.x / WF2DMAP_GRID_SIZE, pos.y / WF2DMAP_GRID_SIZE);
}

// pokebw2's WF2DMAP_JUDGESysObjHitCheck
static BOOL func_ov127_021c5550(const WF2DMAP_OBJWK *wk, const WF2DMAP_OBJSYS *objSys, u32 way) {
    if (func_ov127_021c4ba4(wk, objSys, way) != NULL) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's WF2DMAP_JUDGESysActCmdSet
static void func_ov127_021c5564(WF2DMAP_ACTCMD *act, const WF2DMAP_OBJWK *wk, u32 cmd, u32 playId, u32 way) {
    act->pos = func_ov127_021c4ac4(wk);
    act->cmd = cmd;
    act->playId = playId;
    act->way = way;
}
