// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/wf2dmap/wifi_2dchar.h: the lines in use
// so far.
#ifndef BWPORT_APP_WF2DMAP_WIFI_2DCHAR_H
#define BWPORT_APP_WF2DMAP_WIFI_2DCHAR_H

#include "types.h"
#include "gfl/clact.h"
#include "struct_decls.h"

// Black 2's, not checked against Black
// Not from swan: the archives of the two player characters with the shadow, and of the Union Room characters. They
// belong in constants/arc.h
#define ARCID_WIFI_2DCHAR 31

// Black 2's, not checked against Black
#define ARCID_WIFI_UNIONCHAR 32

// The characters. The Union Room ones are 0 to 15; that the other two are the male and female player is a guess
#define WF_2DC_UNIONCHARA_NUM 16

#define WF_2DC_CHARA_HERO 16
#define WF_2DC_CHARA_HEROINE 17
#define WF_2DC_CHARA_NUM 18

// Black 2's; twinned code confirms the values of WF_2DC_MOVENORMAL and WF_2DC_MOVETURN in Black
// How a character is animated. Names from the older games; the code shows that _NORMAL starts on another frame and
// that _TURN only ever stands
enum {
    WF_2DC_MOVERUN,
    WF_2DC_MOVENORMAL,
    WF_2DC_MOVETURN,
};

// Black 2's; twinned code confirms the values of WF_2DC_ANMWAY and WF_2DC_ANMNONE in Black
// The animations. The number in a name is how many frames it lasts
enum {
    // Stand
    WF_2DC_ANMWAY,
    // Spin until another animation starts
    WF_2DC_ANMROTA,
    WF_2DC_ANMWALK,
    WF_2DC_ANMTURN,
    WF_2DC_ANMRUN,
    WF_2DC_ANMWALLWALK,
    WF_2DC_ANMSLOWWALK,
    WF_2DC_ANMHIGHWALK2,
    WF_2DC_ANMHIGHWALK4,
    WF_2DC_ANMNONE,
    WF_2DC_ANMNUM,
};

struct WF_2DC_WKDATA {
    s16 x;
    s16 y;
    s16 priority;
    s16 bgPriority;
};

typedef struct WF_2DCSYS WF_2DCSYS;
typedef struct WF_2DCWK WF_2DCWK;

// pokebw2's WF_2DC_SysExit
void func_ov127_021c3f80(WF_2DCSYS *sys);
// pokebw2's WF_2DC_ResSet
void func_ov127_021c3fdc(WF_2DCSYS *sys, u32 charaId, u32 vramType, u32 moveType, u32 heapId);
// pokebw2's WF_2DC_AllResDel
void func_ov127_021c4010(WF_2DCSYS *sys);
// pokebw2's WF_2DC_UnionResSet
// Loads the Union Room characters, whose palettes go to plttOffset
void func_ov127_021c4044(WF_2DCSYS *sys, u32 vramType, u32 moveType, u32 plttOffset, u32 heapId);
// pokebw2's WF_2DC_UnionResDel
void func_ov127_021c4080(WF_2DCSYS *sys);
// pokebw2's WF_2DC_ShadowResSet
void func_ov127_021c40a0(WF_2DCSYS *sys, u32 vramType, u32 priority, u32 heapId);
// pokebw2's WF_2DC_WkAdd
WF_2DCWK *func_ov127_021c40bc(WF_2DCSYS *sys, const WF_2DC_WKDATA *data, u32 charaId, u32 heapId);
// pokebw2's WF_2DC_WkDel
void func_ov127_021c41bc(WF_2DCWK *wk);
// pokebw2's WF_2DC_WkMatrixSet
void func_ov127_021c41dc(WF_2DCWK *wk, s16 x, s16 y);
// pokebw2's WF_2DC_WkDrawPriSet
void func_ov127_021c4214(WF_2DCWK *wk, u16 priority);
// pokebw2's WF_2DC_WkDrawFlagSet
void func_ov127_021c4230(WF_2DCWK *wk, BOOL flag);
// pokebw2's WF_2DC_WkMoveTypeGet
// WF_2DC_MOVE*
u32 func_ov127_021c4268(const WF_2DCWK *wk);
// pokebw2's WF_2DC_WkShadowMatrixSet
// x and y are the character's: the shadow goes under its feet
void func_ov127_021c428c(WF_2DCWK *wk, s16 x, s16 y);
// pokebw2's WF_2DC_WkPatAnmStart
void func_ov127_021c42a8(WF_2DCWK *wk, u32 anmType, u32 way);
// pokebw2's WF_2DC_WkPatAnmMain
void func_ov127_021c42c4(WF_2DCWK *wk);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's WF_2DC_SysInit
// unk1 is never used
WF_2DCSYS *func_ov127_021c3ee0(ClActUnit *unit, void *unk1, u32 objNum, u32 heapId);
// pokebw2's WF_2DC_ShadowResDel
void func_ov127_021c40b4(WF_2DCSYS *sys);
// pokebw2's WF_2DC_WkDrawPriGet
u32 func_ov127_021c4224(const WF_2DCWK *wk);
// pokebw2's WF_2DC_WkShadowDrawFlagSet
void func_ov127_021c424c(WF_2DCWK *wk, BOOL flag);
// pokebw2's WF_2DC_WkDrawFlagGet
BOOL func_ov127_021c425c(const WF_2DCWK *wk);
// pokebw2's WF_2DC_WkAnmTypeGet
// WF_2DC_ANM*
u32 func_ov127_021c4270(const WF_2DCWK *wk);
// pokebw2's WF_2DC_WkAnmFrameSet
void func_ov127_021c4274(WF_2DCWK *wk, u16 frame);
// pokebw2's WF_2DC_WkAnmFrameGet
u16 func_ov127_021c4280(const WF_2DCWK *wk);

#endif // BWPORT_APP_WF2DMAP_WIFI_2DCHAR_H
