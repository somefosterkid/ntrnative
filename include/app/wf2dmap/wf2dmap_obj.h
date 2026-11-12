// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/wf2dmap/wf2dmap_obj.h: the lines in use
// so far.
#ifndef BWPORT_APP_WF2DMAP_WF2DMAP_OBJ_H
#define BWPORT_APP_WF2DMAP_WF2DMAP_OBJ_H

#include "types.h"
#include "app/wf2dmap/wf2dmap_common.h"

typedef struct WF2DMAP_OBJWK WF2DMAP_OBJWK;

// pokebw2's WF2DMAP_OBJWkFrameMatrixGet
// Where it is between its last position and its position at this frame of its move
WF2DMAP_POS func_ov127_021c4adc(const WF2DMAP_OBJWK *wk);

// Black 2's; twinned code confirms the values of WF2DMAP_OBJPM_X, WF2DMAP_OBJPM_Y, WF2DMAP_OBJPM_LX, WF2DMAP_OBJPM_LY,
// WF2DMAP_OBJPM_PLID, WF2DMAP_OBJPM_ST, WF2DMAP_OBJPM_WAY, WF2DMAP_OBJPM_CHARA, WF2DMAP_OBJPM_FRAME,
// WF2DMAP_OBJPM_ENDFRAME and WF2DMAP_OBJPM_STFRAME in Black
// The values of an object that WF2DMAP_OBJWkDataGet and WF2DMAP_OBJWkDataSet reach
enum {
    WF2DMAP_OBJPM_X,
    WF2DMAP_OBJPM_Y,
    // Where it was before its last move
    WF2DMAP_OBJPM_LX,
    WF2DMAP_OBJPM_LY,
    WF2DMAP_OBJPM_PLID,
    // WF2DMAP_OBJST_*
    WF2DMAP_OBJPM_ST,
    WF2DMAP_OBJPM_WAY,
    WF2DMAP_OBJPM_CHARA,
    // The frame of its state and the frame the state ends at
    WF2DMAP_OBJPM_FRAME,
    WF2DMAP_OBJPM_ENDFRAME,
    // How many frames it has been in its state
    WF2DMAP_OBJPM_STFRAME,
    WF2DMAP_OBJPM_NUM,
};

typedef struct {
    s16 x;
    s16 y;
    u16 playId;
    u16 status;
    u16 way;
    u16 charaId;
} WF2DMAP_OBJDATA;

typedef struct WF2DMAP_OBJSYS WF2DMAP_OBJSYS;

// pokebw2's WF2DMAP_OBJSysInit
WF2DMAP_OBJSYS *func_ov127_021c4880(u32 objNum, u32 heapId);
// pokebw2's WF2DMAP_OBJSysMain
void func_ov127_021c48e8(WF2DMAP_OBJSYS *sys);
// pokebw2's WF2DMAP_OBJWkDataGet
s32 func_ov127_021c49a4(const WF2DMAP_OBJWK *wk, u32 id);
// pokebw2's WF2DMAP_OBJToolWayPosGet
// The position one square from pos
WF2DMAP_POS func_ov127_021c4a54(WF2DMAP_POS pos, u32 way);
// pokebw2's WF2DMAP_OBJSysCmdSet
void func_ov127_021c48d4(WF2DMAP_OBJSYS *sys, const WF2DMAP_ACTCMD *act);
// pokebw2's WF2DMAP_OBJSysHitCheck
// Another object on, or leaving, the square in front of wk, or NULL
const WF2DMAP_OBJWK *func_ov127_021c4ba4(const WF2DMAP_OBJWK *wk, const WF2DMAP_OBJSYS *sys, u32 way);
// pokebw2's WF2DMAP_OBJWkNew
WF2DMAP_OBJWK *func_ov127_021c4918(WF2DMAP_OBJSYS *sys, const WF2DMAP_OBJDATA *data);

// pokebw2's WF2DMAP_OBJWkGet
// The object of a player, or NULL
WF2DMAP_OBJWK *func_ov127_021c4954(WF2DMAP_OBJSYS *sys, u16 playId);
// pokebw2's WF2DMAP_OBJWkIdxGet
// The object in a slot, or NULL when the slot is free
WF2DMAP_OBJWK *func_ov127_021c4980(WF2DMAP_OBJSYS *sys, u16 index);
// pokebw2's WF2DMAP_OBJWkDataSet
void func_ov127_021c4a00(WF2DMAP_OBJWK *wk, u32 id, s32 data);
// pokebw2's WF2DMAP_OBJWkMatrixGet
WF2DMAP_POS func_ov127_021c4ac4(const WF2DMAP_OBJWK *wk);
// pokebw2's WF2DMAP_OBJWkLastMatrixGet
WF2DMAP_POS func_ov127_021c4ad0(const WF2DMAP_OBJWK *wk);
// pokebw2's WF2DMAP_OBJWkMatrixSet
void func_ov127_021c4b7c(WF2DMAP_OBJWK *wk, WF2DMAP_POS pos);
// pokebw2's WF2DMAP_OBJWkLastMatrixSet
void func_ov127_021c4b90(WF2DMAP_OBJWK *wk, WF2DMAP_POS pos);
// pokebw2's WF2DMAP_OBJWkMatrixGet

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's WF2DMAP_OBJSysExit
void func_ov127_021c48c0(WF2DMAP_OBJSYS *sys);
// pokebw2's WF2DMAP_OBJWkDel
void func_ov127_021c4948(WF2DMAP_OBJWK *wk);
// pokebw2's WF2DMAP_OBJToolRetWayGet
// The opposite way
u32 func_ov127_021c4a8c(u32 way);
// pokebw2's WF2DMAP_OBJSysObjNumGet
u32 func_ov127_021c4914(const WF2DMAP_OBJSYS *sys);
// pokebw2's WF2DMAP_OBJWkConstGet
const WF2DMAP_OBJWK *func_ov127_021c4994(const WF2DMAP_OBJSYS *sys, u16 playId);
// pokebw2's WF2DMAP_OBJWkConstIdxGet
const WF2DMAP_OBJWK *func_ov127_021c499c(const WF2DMAP_OBJSYS *sys, u16 index);
// pokebw2's WF2DMAP_OBJWkConstGet

#endif // BWPORT_APP_WF2DMAP_WF2DMAP_OBJ_H
