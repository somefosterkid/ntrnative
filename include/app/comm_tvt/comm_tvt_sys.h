// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/comm_tvt/comm_tvt_sys.h: the lines in
// use so far.
#ifndef BWPORT_APP_COMM_TVT_COMM_TVT_SYS_H
#define BWPORT_APP_COMM_TVT_COMM_TVT_SYS_H

#include "types.h"
#include "gfl/heap.h"
#include "system/printsys.h"
#include "struct_decls.h"
#include "gfl/clact.h"

// pokebw2's CommTvt_IsCameraEnabled
BOOL func_ov172_021be100(void);

// Black 2's; twinned code confirms the values of COMM_TVT_MODE_DRAW in Black
// The modes of the Xtransceiver, each the work of one file
enum {
    COMM_TVT_MODE_NONE,
    COMM_TVT_MODE_TALK,
    COMM_TVT_MODE_CALL,
    COMM_TVT_MODE_DRAW,
    COMM_TVT_MODE_GAME,
    COMM_TVT_MODE_EXIT,
    COMM_TVT_MODE_EXIT_ERROR,
};

void func_ov172_021be34c(CommTvtWork *sys);
// pokebw2's CommTvt_GetPrintQueue
PrintQueue *func_ov172_021be044(CommTvtWork *sys);
void func_ov172_021be318(CommTvtWork *sys, BmpWin *window);
// pokebw2's CommTvt_GetFont
Font *func_ov172_021be034(CommTvtWork *sys);
// pokebw2's CommTvt_GetMsgData
MsgData *func_ov172_021be03c(CommTvtWork *sys);
// pokebw2's CommTvt_GetTaskMenuRes
AppTaskMenuRes *func_ov172_021be04c(CommTvtWork *sys);

void func_ov172_021be2e0(CommTvtWork *sys);
void func_ov172_021be1a4(CommTvtWork *sys);
// pokebw2's CommTvt_GetComm
CtvtComm *func_ov172_021bdfe4(CommTvtWork *sys);
// pokebw2's CommTvt_ToggleZoom
void func_ov172_021be08c(CommTvtWork *sys);
void func_ov172_021be0b8(CommTvtWork *sys);

// Black 2's; twinned code confirms the values of CTDM_DOUBLE and CTDM_QUAD in Black
// How the top screen is split between the members of the call. Game Freak's name for the first is in an assert; the
// others are guesses
enum {
    CTDM_SINGLE,
    CTDM_DOUBLE,
    CTDM_QUAD,
};

// pokebw2's CommTvt_GetCamera
CtvtCamera *func_ov172_021bdfdc(CommTvtWork *sys);
// pokebw2's CommTvt_GetMic
CtvtMic *func_ov172_021bdfec(CommTvtWork *sys);
// pokebw2's CommTvt_GetArc
ArcTool *func_ov172_021be010(CommTvtWork *sys);
// pokebw2's CommTvt_GetObjResource
u32 func_ov172_021be018(CommTvtWork *sys, int index);
// pokebw2's CommTvt_GetClActUnit
ClActUnit *func_ov172_021be024(CommTvtWork *sys);
// pokebw2's CommTvt_GetDisplayMode
int func_ov172_021be05c(CommTvtWork *sys);
// pokebw2's CommTvt_SendZoom
void func_ov172_021be07c(CommTvtWork *sys, BOOL zoomed);
// pokebw2's CommTvt_SetZoomed
void func_ov172_021be0a0(CommTvtWork *sys, BOOL zoomed);
// pokebw2's CommTvt_SetSelfIndex
void func_ov172_021be0a8(CommTvtWork *sys, u8 index);
// The yes/no menus and the cancel button, at a corner in tiles
AppTaskMenu *func_ov172_021be11c(CommTvtWork *sys);
void func_ov172_021be1e4(CommTvtWork *sys);
void func_ov172_021be210(CommTvtWork *sys, u8 talker);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's CommTvt_GetHeapId
HeapID func_ov172_021be00c(CommTvtWork *sys);
// pokebw2's CommTvt_GetParam
CommTvtParam *func_ov172_021be004(CommTvtWork *sys);
BOOL func_ov172_021be0b4(CommTvtWork *sys);
BOOL func_ov172_021be0c8(CommTvtWork *sys);
// pokebw2's CommTvt_GetMemberCount
u8 func_ov172_021be054(CommTvtWork *sys);
// pokebw2's CommTvt_GetDrawSystem
DrawSystem *func_ov172_021bdffc(CommTvtWork *sys);
// pokebw2's CommTvt_IsZoomed
BOOL func_ov172_021be078(CommTvtWork *sys);
BOOL func_ov172_021be0d0(CommTvtWork *sys);
BOOL func_ov172_021be02c(CommTvtWork *sys);
void func_ov172_021be030(CommTvtWork *sys, BOOL value);
// pokebw2's CommTvt_GetSelfIndex
u8 func_ov172_021be0a4(CommTvtWork *sys);
BOOL func_ov172_021be0ac(CommTvtWork *sys);
void func_ov172_021be0b0(CommTvtWork *sys, BOOL value);
void func_ov172_021be0cc(CommTvtWork *sys, BOOL value);
void func_ov172_021be0d4(CommTvtWork *sys, BOOL value);
BOOL func_ov172_021be0d8(CommTvtWork *sys);
// pokebw2's CommTvt_CanExchangePhotos
BOOL func_ov172_021be0ec(CommTvtWork *sys);
// pokebw2's CommTvt_ClearCanExchangePhotos
void func_ov172_021be0f0(CommTvtWork *sys);
BOOL func_ov172_021be0f8(CommTvtWork *sys);
void func_ov172_021be0fc(CommTvtWork *sys, BOOL value);
void func_ov172_021be2f0(CommTvtWork *sys, const CtvtCommMemberInfo *info);

#endif // BWPORT_APP_COMM_TVT_COMM_TVT_SYS_H
