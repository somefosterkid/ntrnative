// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/wf2dmap/wf2dmap_objdraw.h: the lines in
// use so far.
#ifndef BWPORT_APP_WF2DMAP_WF2DMAP_OBJDRAW_H
#define BWPORT_APP_WF2DMAP_WF2DMAP_OBJDRAW_H

#include "types.h"
#include "app/wf2dmap/wf2dmap_common.h"
#include "app/wf2dmap/wf2dmap_obj.h"
#include "gfl/clact.h"

typedef struct WF2DMAP_OBJDRAWSYS WF2DMAP_OBJDRAWSYS;
typedef struct WF2DMAP_OBJDRAWWK WF2DMAP_OBJDRAWWK;

// pokebw2's WF2DMAP_OBJDrawSysUpdata
void func_ov127_021c571c(WF2DMAP_OBJDRAWSYS *sys);
// pokebw2's WF2DMAP_OBJDrawWkUpdataFlagSet
// Whether WF2DMAP_OBJDrawWkUpdata does anything
void func_ov127_021c57d8(WF2DMAP_OBJDRAWWK *wk, BOOL flag);
// pokebw2's WF2DMAP_OBJDrawWkKuruAnimeStart
// Plays the spinning animation
void func_ov127_021c57dc(WF2DMAP_OBJDRAWWK *wk);
// pokebw2's WF2DMAP_OBJDrawWkDel
void func_ov127_021c5704(WF2DMAP_OBJDRAWWK *wk);
// pokebw2's WF2DMAP_OBJDrawWkWaySet
// Stands it facing way
void func_ov127_021c5864(WF2DMAP_OBJDRAWWK *wk, u32 way);
// pokebw2's WF2DMAP_OBJDrawWkMatrixSet
void func_ov127_021c5814(WF2DMAP_OBJDRAWWK *wk, WF2DMAP_POS pos);
// pokebw2's WF2DMAP_OBJDrawSysInit
// Loads the player's character (charaId, a WF_2DC_CHARA_*) and the shadow. unk1 is never used
WF2DMAP_OBJDRAWSYS *func_ov127_021c5590(ClActUnit *unit, void *unk1, u32 objNum, u32 charaId, u32 moveType,
                                           u32 vramType, u32 heapId);
// pokebw2's WF2DMAP_OBJDrawSysUnionResSet
void func_ov127_021c5664(WF2DMAP_OBJDRAWSYS *sys, u32 moveType, u32 heapId);
// pokebw2's WF2DMAP_OBJDrawSysExit
void func_ov127_021c5608(WF2DMAP_OBJDRAWSYS *sys);
// pokebw2's WF2DMAP_OBJDrawWkNew
WF2DMAP_OBJDRAWWK *func_ov127_021c5680(WF2DMAP_OBJDRAWSYS *sys, const WF2DMAP_OBJWK *objWk, BOOL hero, u32 heapId);
// pokebw2's WF2DMAP_OBJDrawWkKuruAnimeEnd
void func_ov127_021c57f4(WF2DMAP_OBJDRAWWK *wk);

// pokebw2's WF2DMAP_OBJDrawWkUpdata
// Follows its object: animation, position and priority
void func_ov127_021c574c(WF2DMAP_OBJDRAWWK *wk);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's WF2DMAP_OBJDrawWkDrawPriGet
u32 func_ov127_021c5858(const WF2DMAP_OBJDRAWWK *wk);
// pokebw2's WF2DMAP_OBJDrawWkShadowDrawFlagSet
void func_ov127_021c584c(WF2DMAP_OBJDRAWWK *wk, BOOL flag);
// pokebw2's WF2DMAP_OBJDrawWkKuruAnimeMain
void func_ov127_021c57e8(WF2DMAP_OBJDRAWWK *wk);
// pokebw2's WF2DMAP_OBJDrawWkDrawFlagGet
BOOL func_ov127_021c5834(const WF2DMAP_OBJDRAWWK *wk);
// pokebw2's WF2DMAP_OBJDrawWkShadowMatrixSet
void func_ov127_021c5898(WF2DMAP_OBJDRAWWK *wk, WF2DMAP_POS pos);
// pokebw2's WF2DMAP_OBJDrawSysAllResDel
void func_ov127_021c5658(WF2DMAP_OBJDRAWSYS *sys);
// pokebw2's WF2DMAP_OBJDrawWkDrawFlagSet
void func_ov127_021c5840(WF2DMAP_OBJDRAWWK *wk, BOOL flag);

#endif // BWPORT_APP_WF2DMAP_WF2DMAP_OBJDRAW_H
