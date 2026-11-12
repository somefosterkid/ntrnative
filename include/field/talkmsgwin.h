// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/talkmsgwin.h.
#ifndef BWPORT_FIELD_TALKMSGWIN_H
#define BWPORT_FIELD_TALKMSGWIN_H

#include "types.h"
#include "gfl/bmpwin.h"
#include "gfl/g3d.h"
#include "gfl/heap.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"

// The field's talk windows (talkmsgwin.c; in Black, part of overlay 21, which Black 2 has as overlay 36), whose frame
// other overlays draw around their own windows with overlay 21 loaded. Only the functions those overlays call are
// declared

typedef struct {
    HeapID heapId;
    G3DCamera *unk4;           // the camera the tails are projected with; NULL when none has one
    Font *font;
    u16 unkC;
    u32 bg;
    u8 unk14;
    u8 unk15;
} TalkMsgWinSetup;

TalkMsgWinSys *func_ov021_02194670(const TalkMsgWinSetup *setup);
void func_ov021_02194798(TalkMsgWinSys *sys);
// Draws the talk window's frame around a window, and clears it
void func_ov021_02194a24(TalkMsgWinSys *sys, BmpWin *window, u32 a2);
void func_ov021_02194a34(TalkMsgWinSys *sys, BmpWin *window);

void func_ov021_0219470c(TalkMsgWinSys *param_1);
void func_ov021_02194760(TalkMsgWinSys *param_1);
void ActorMsgWin_CreateAtTop(TalkMsgWinSys *sys, s32 index, const VecFx32 *target, StrBuf *strbuf, u8 unused, int frameType, u32 tail, u32 wait);
void ActorMsgWin_CreateAtBottom(TalkMsgWinSys *sys, s32 index, const VecFx32 *target, StrBuf *strbuf, u8 unused, int frameType, u32 tail, u32 wait);
void func_ov021_021948cc(TalkMsgWinSys *sys, s32 index, const VecFx32 *target, StrBuf *strbuf, u8 unused, int frameType, u32 wait);
void func_ov021_02194940(TalkMsgWinSys *param_1, s32 param_2);
void func_ov021_0219499c(TalkMsgWinSys *sys, s32 index, StrBuf *strbuf, s32 wait);
BmpWin *func_ov021_02194a00(TalkMsgWinSys *param_1, u8 param_2, u8 param_3, u8 param_4, u8 param_5, u32 param_6);
void func_ov021_021947e8(TalkMsgWinSys *sys, s32 index, StrBuf *strbuf, u8 x, u8 y, u8 width, u8 height, u32 unused,
                         u32 frameType, u32 wait, u8 textX, u8 textY);
void func_ov021_02195588(TalkMsgWinSys *sys);
void loadTexture_wrapper(TalkMsgWinSys *sys);

#endif // BWPORT_FIELD_TALKMSGWIN_H
