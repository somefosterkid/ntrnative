// Black's talkmsgwin.c (the name its heap calls pass; pokebw2's ctalkmsgwin.c): ov021 0x02194670-0x02196030.

#include "types.h"
#include "field/talkmsgwin.h"
#include "gfl/arc_util.h"
#include "gfl/bmp.h"
#include "system/printsys.h"
#include "gfl/tcbl.h"
#include "gfl/sound.h"
#include "nitro/fx/fx_cp.h"
#include "nitro/fx/fx_vec.h"

#define TALKMSG_WIN_COUNT 32

// How a window is made: placed as asked (0), across the top (1) or the bottom (2) of the screen; the tail's side
// (0: the side the target is on), the place and size in characters, the window's color, the frame's style and the
// print wait. pokebw2's TalkMsgWinParam
typedef struct {
    u32 shape;
    u32 tail;
    u8 x;
    u8 y;
    u8 width;
    u8 height;
    u16 color;
    u16 frameType;
    u32 wait;
} TalkMsgWinParam;

// The tail's triangle (vtx[0..2]) and the window's quad (vtx[3..6]), relative to origin and divided by size; tail is
// the side of the frame the tail leaves, 0 for none. pokebw2's TalkMsgWinPolygon
typedef struct {
    VecFx32 origin;
    fx32 size;
    VecFx16 vtx[7];
    u32 tail;
} TalkMsgWinPolygon;

// One of the 32 windows, 0x64 bytes. pokebw2's TalkMsgWin
typedef struct {
    u16 state;
    u8 skipAnim;
    u8 frameType;
    u32 shape;
    PrintStream *stream;
    BmpWin *bmpWin;
    const VecFx32 *target;
    StrBuf *strbuf;
    u16 color;
    u16 anchorWin;
    TalkMsgWinPolygon poly;
    u8 textX;
    u8 textY;
    u16 frame;
    u32 wait;
} TalkMsgWin;

// 0xcd0 bytes (func_ov021_02194670's allocation), the layout pokebw2's has
struct TalkMsgWinSys {
    TalkMsgWinSetup setup;
    TalkMsgWin windows[TALKMSG_WIN_COUNT];
    TCBExManager *tcbMgr;
    u16 frameChars;
    VecFx32 camPos;
    VecFx32 camUp;
    VecFx32 camTarget;
    fx32 camZNear;
    NNSGfdTexKey texKey;
    NNSGfdPlttKey plttKey;
};

BmpWin *func_ov021_02194a94(TalkMsgWinSys *sys, u8 x, u8 y, u8 width, u8 height);
void func_ov021_02195e54(TalkMsgWinSys *sys, BmpWin *bmpWin, s32 frameType, u32 tail);
void func_ov021_02195e90(TalkMsgWinSys *sys, BmpWin *bmpWin);
u32 func_ov021_021954cc(TalkMsgWinSys *sys, TalkMsgWinSetup *setup);
static u32 TALKMSG_WIN_TEX[8] = { 0x44444444, 0x33333334, 0x22222234, 0x11111234, 0x11111234, 0x11111234, 0x11111234, 0xff111234 };
static u16 TALKMSG_WIN_PLT[16] = { 0x0000, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x6318 };
void func_ov021_02195530(TalkMsgWinSys *sys);
void func_ov021_02194b70(TalkMsgWin *win);
void func_ov021_02194b78(TalkMsgWin *win);
BOOL func_ov021_02194a84(TalkMsgWin *win);
void func_ov021_02194b3c(TalkMsgWinSys *sys, TalkMsgWin *win);
void func_ov021_021953d4(TalkMsgWinSys *sys, s32 draw);
void func_ov021_02194ce8(TalkMsgWinSys *sys, TalkMsgWin *win);
void func_ov021_0219545c(TalkMsgWinSys *sys);
void func_ov021_02194b84(TalkMsgWinSys *sys, TalkMsgWin *win);
void func_ov021_02194c60(TalkMsgWinSys *sys, TalkMsgWin *win);
void func_ov021_02195490(TalkMsgWinSys *sys);
void func_ov021_02194a58(TalkMsgWin *win);
void ActorMsgWin_CreateCore(TalkMsgWinSys *sys, TalkMsgWin *win, const VecFx32 *target, StrBuf *strbuf, TalkMsgWinParam *param);
u32 func_ov021_02195338(TalkMsgWinSys *sys, TalkMsgWin *win);
void func_ov021_02195e80(TalkMsgWinSys *sys, TalkMsgWin *win);
void func_ov021_02195e70(TalkMsgWinSys *sys, TalkMsgWin *win);
void func_ov021_02194d1c(TalkMsgWinSys *sys, TalkMsgWin *win, s32 shown);
u32 func_ov021_02194f84(TalkMsgWin *win, s32 px, s32 py, VecFx32 *near1, VecFx32 *near2);
void func_ov021_0219518c(TalkMsgWinSys *sys, TalkMsgWin *win, const VecFx32 *target, const VecFx32 *start, const VecFx32 *end, const VecFx32 *topLeft, const VecFx32 *topRight, const VecFx32 *bottomLeft, const VecFx32 *bottomRight);
void func_ov021_021955ec(TalkMsgWinSys *sys, BmpWin *bmpWin, u32 tail);
void func_ov021_02195938(TalkMsgWinSys *sys, BmpWin *bmpWin, u32 tail);

TalkMsgWinSys *func_ov021_02194670(const TalkMsgWinSetup *setup) {
    s32 i;
    TalkMsgWinSys *sys;

    sys = GFL_HeapAllocate(setup->heapId, sizeof(TalkMsgWinSys), TRUE, "talkmsgwin.c", 0xec);
    sys->setup = *setup;
    sys->tcbMgr = GFL_TCBExMgrCreate(setup->heapId, setup->heapId, TALKMSG_WIN_COUNT, 0x20);
    for (i = 0; i < TALKMSG_WIN_COUNT; i++) {
        func_ov021_02194a58(&sys->windows[i]);
    }
    sys->texKey = NNS_GfdDefaultFuncAllocTexVram(sizeof(TALKMSG_WIN_TEX), 0, 0);
    sys->plttKey = NNS_GfdDefaultFuncAllocPlttVram(sizeof(TALKMSG_WIN_PLT), 0, 0);
    sys->frameChars = func_ov021_021954cc(sys, &sys->setup) >> 5;
    return sys;
}

void loadTexture_wrapper(TalkMsgWinSys *sys) {
    func_ov021_02195530(sys);
}

void func_ov021_0219470c(TalkMsgWinSys *sys) {
    s32 i;

    GFL_TCBExMgrUpdate(sys->tcbMgr);
    func_ov021_0219545c(sys);
    func_ov021_021953d4(sys, 0);
    for (i = 0; i < TALKMSG_WIN_COUNT; i++) {
        if (sys->windows[i].skipAnim == 0) {
            func_ov021_02194b84(sys, &sys->windows[i]);
        } else {
            func_ov021_02194c60(sys, &sys->windows[i]);
        }
    }
    func_ov021_02195490(sys);
}

void func_ov021_02194760(TalkMsgWinSys *sys) {
}

void func_ov021_02194764(TalkMsgWinSys *sys) {
    s32 i;

    NNS_G3dGeFlushBuffer();
    func_ov021_021953d4(sys, 1);
    for (i = 0; i < TALKMSG_WIN_COUNT; i++) {
        if (sys->windows[i].skipAnim == 0) {
            func_ov021_02194ce8(sys, &sys->windows[i]);
        }
    }
}

void func_ov021_02194798(TalkMsgWinSys *sys) {
    s32 i;

    for (i = 0; i < TALKMSG_WIN_COUNT; i++) {
        func_ov021_02194b3c(sys, &sys->windows[i]);
    }
    NNS_GfdDefaultFuncFreePlttVram(sys->plttKey);
    NNS_GfdDefaultFuncFreeTexVram(sys->texKey);
    GFL_TCBExMgrFree(sys->tcbMgr);
    GFL_HeapFree(sys);
}

void func_ov021_021947e8(TalkMsgWinSys *sys, s32 index, StrBuf *strbuf, u8 x, u8 y, u8 width, u8 height, u32 unused, u32 frameType, u32 wait, u8 textX, u8 textY) {
    TalkMsgWinParam param;
    TalkMsgWin *win;

    param.shape = 0;
    param.tail = 0;
    param.x = x;
    param.y = y;
    param.width = width;
    param.height = height;
    param.color = 0x7fff;
    param.frameType = frameType;
    param.wait = wait;
    win = &sys->windows[index];
    ActorMsgWin_CreateCore(sys, win, NULL, strbuf, &param);
    win->textX = textX;
    win->textY = textY;
    win->skipAnim = 1;
}

void ActorMsgWin_CreateAtTop(TalkMsgWinSys *sys, s32 index, const VecFx32 *target, StrBuf *strbuf, u8 unused, int frameType, u32 tail, u32 wait) {
    TalkMsgWinParam param;

    param.shape = 1;
    param.tail = tail;
    param.x = 1;
    param.y = 1;
    param.width = 0x1e;
    param.height = 4;
    param.color = 0x7fff;
    param.frameType = frameType;
    param.wait = wait;
    ActorMsgWin_CreateCore(sys, &sys->windows[index], target, strbuf, &param);
}

void ActorMsgWin_CreateAtBottom(TalkMsgWinSys *sys, s32 index, const VecFx32 *target, StrBuf *strbuf, u8 unused, int frameType, u32 tail, u32 wait) {
    TalkMsgWinParam param;

    param.shape = 2;
    param.tail = tail;
    param.x = 1;
    param.y = 0x13;
    param.width = 0x1e;
    param.height = 4;
    param.color = 0x7fff;
    param.frameType = frameType;
    param.wait = wait;
    ActorMsgWin_CreateCore(sys, &sys->windows[index], target, strbuf, &param);
}

void func_ov021_021948cc(TalkMsgWinSys *sys, s32 index, const VecFx32 *target, StrBuf *strbuf, u8 unused, int frameType, u32 wait) {
    s32 x;
    s32 y;

    NNS_G3dWorldPosToScrPos(target, &x, &y);
    if (y < 0x60) {
        ActorMsgWin_CreateAtBottom(sys, index, target, strbuf, unused, frameType, 0, wait);
    } else {
        ActorMsgWin_CreateAtTop(sys, index, target, strbuf, unused, frameType, 0, wait);
    }
}

void func_ov021_0219492c(TalkMsgWinSys *sys, s32 index) {
    func_ov021_02194b3c(sys, &sys->windows[index]);
}

void func_ov021_02194940(TalkMsgWinSys *sys, s32 index) {
    func_ov021_02194b70(&sys->windows[index]);
}

void func_ov021_02194950(TalkMsgWinSys *sys, s32 index) {
    func_ov021_02194b78(&sys->windows[index]);
}

BOOL func_ov021_02194960(TalkMsgWinSys *sys, s32 index) {
    if (sys->windows[index].state == 0) {
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov021_02194974(TalkMsgWinSys *sys, s32 index) {
    return func_ov021_02194a84(&sys->windows[index]);
}

PrintStream *func_ov021_02194984(TalkMsgWinSys *sys, s32 index) {
    return sys->windows[index].stream;
}

BmpWin *func_ov021_02194990(TalkMsgWinSys *sys, s32 index) {
    return sys->windows[index].bmpWin;
}

void func_ov021_0219499c(TalkMsgWinSys *sys, s32 index, StrBuf *strbuf, s32 wait) {
    TalkMsgWin *win;

    win = &sys->windows[index];
    if (win->stream != NULL) {
        func_0201ce48(win->stream);
        win->stream = NULL;
    }
    if (strbuf != NULL) {
        win->wait = wait;
        win->strbuf = strbuf;
        win->stream = func_0201cce4(win->bmpWin, win->textX, win->textY, win->strbuf, sys->setup.font, wait, sys->tcbMgr, 0, sys->setup.heapId, 0xf);
        win->state = 4;
    }
}

BmpWin *func_ov021_02194a00(TalkMsgWinSys *sys, u8 x, u8 y, u8 width, u8 height, u32 frameType) {
    BmpWin *bmpWin;

    bmpWin = func_ov021_02194a94(sys, x, y, width, height);
    func_ov021_02195e54(sys, bmpWin, frameType & 0xff, 0);
    return bmpWin;
}

void func_ov021_02194a24(TalkMsgWinSys *sys, BmpWin *window, u32 a2) {
    func_ov021_02195e54(sys, window, a2 & 0xff, 0);
}

void func_ov021_02194a34(TalkMsgWinSys *sys, BmpWin *window) {
    func_ov021_02195e90(sys, window);
    GFL_BGSysLoadScr(sys->setup.bg);
}

void func_ov021_02194a48(TalkMsgWinSys *sys, BmpWin *bmpWin) {
    func_ov021_02195e90(sys, bmpWin);
    BmpWin_Free(bmpWin);
}

void func_ov021_02194a58(TalkMsgWin *win) {
    win->state = 0;
    win->stream = NULL;
    VEC_Fx16Set(&win->poly.vtx[0], 0, 0, 0);
    VEC_Fx16Set(&win->poly.vtx[1], 0, 0, 0);
    VEC_Fx16Set(&win->poly.vtx[2], 0, 0, 0);
}

BOOL func_ov021_02194a74(TalkMsgWin *win) {
    if (win->state == 0) {
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov021_02194a84(TalkMsgWin *win) {
    if (win->state == 4) {
        return TRUE;
    }
    return FALSE;
}

BmpWin *func_ov021_02194a94(TalkMsgWinSys *sys, u8 x, u8 y, u8 width, u8 height) {
    BmpWin *window;
    GFLBitmap *bitmap;

    window = BmpWin_CreateDynamic(sys->setup.bg, x, y, width, height, sys->setup.unk15, 1);
    bitmap = BmpWin_GetBitmap(window);
    GFL_BitmapFill(bitmap, 0xf);
    BmpWin_FlushChar(window);
    return window;
}

void ActorMsgWin_CreateCore(TalkMsgWinSys *sys, TalkMsgWin *win, const VecFx32 *target, StrBuf *strbuf, TalkMsgWinParam *param) {
    u8 x;
    u8 y;
    u8 width;
    u8 height;

    func_ov021_02194b3c(sys, win);
    win->target = target;
    win->strbuf = strbuf;
    win->color = param->color;
    win->anchorWin = 0;
    win->shape = param->shape;
    win->frameType = param->frameType;
    win->wait = param->wait;
    width = param->width;
    x = param->x;
    x = (x + width > 0x20) ? 0x20 - width : x;
    height = param->height;
    y = param->y;
    y = (y + height > 0x18) ? 0x18 - height : y;
    win->bmpWin = func_ov021_02194a94(sys, x, y, width, height);
    win->poly.tail = param->tail;
    win->textX = 8;
    win->textY = 0;
    win->state = 1;
}

void func_ov021_02194b3c(TalkMsgWinSys *sys, TalkMsgWin *win) {
    if (func_ov021_02194a74(win) == FALSE) {
        if (win->stream != NULL) {
            func_0201ce48(win->stream);
            win->stream = NULL;
        }
        func_ov021_02195e90(sys, win->bmpWin);
        BmpWin_Free(win->bmpWin);
        win->skipAnim = 0;
        win->state = 0;
    }
}

void func_ov021_02194b70(TalkMsgWin *win) {
    win->state = 2;
}

void func_ov021_02194b78(TalkMsgWin *win) {
    win->state = 5;
    win->frame = 4;
}

void func_ov021_02194b84(TalkMsgWinSys *sys, TalkMsgWin *win) {
    u32 wait;

    switch (win->state) {
    case 2:
        func_ov021_02195338(sys, win);
        win->frame = 0;
        win->state = 3;
        break;
    case 3:
        func_ov021_02195338(sys, win);
        if (win->frame < 8) {
            win->frame++;
            return;
        }
        win->state = 4;
        wait = win->wait;
        func_ov021_02195e80(sys, win);
        win->stream = func_0201cce4(win->bmpWin, win->textX, win->textY, win->strbuf, sys->setup.font, wait, sys->tcbMgr, 0, sys->setup.heapId, 0xf);
        break;
    case 4:
        if (func_ov021_02195338(sys, win) == 1) {
            func_ov021_02195e80(sys, win);
        }
        break;
    case 5:
        if (win->frame != 0) {
            win->frame--;
            return;
        }
        win->state = 6;
        break;
    case 6:
        func_ov021_02194b3c(sys, win);
        break;
    }
}

void func_ov021_02194c60(TalkMsgWinSys *sys, TalkMsgWin *win) {
    u32 wait;

    switch (win->state) {
    case 2:
        win->frame = 0;
        win->state = 3;
    case 3:
        win->state = 4;
        wait = win->wait;
        func_ov021_02195e70(sys, win);
        win->stream = func_0201cce4(win->bmpWin, win->textX, win->textY, win->strbuf, sys->setup.font, wait, sys->tcbMgr, 0, sys->setup.heapId, 0xf);
        break;
    case 5:
        win->state = 6;
        break;
    case 6:
        func_ov021_02194b3c(sys, win);
        break;
    }
}

void func_ov021_02194ce8(TalkMsgWinSys *sys, TalkMsgWin *win) {
    switch (win->state) {
    case 0:
    case 1:
    case 2:
        break;
    case 3:
        func_ov021_02194d1c(sys, win, 0);
        break;
    case 4:
        func_ov021_02194d1c(sys, win, 1);
        break;
    case 5:
    case 6:
        break;
    }
}

void func_ov021_02194d1c(TalkMsgWinSys *sys, TalkMsgWin *win, s32 shown) {
    TalkMsgWinPolygon *poly;
    s32 scale;

    G3_PushMtx();
    poly = &win->poly;
    G3_Translate(poly->origin.x, poly->origin.y, poly->origin.z);
    scale = poly->size * win->frame / 8;
    G3_Scale(scale, scale, scale);
    G3_TexImageParam(GX_TEXFMT_PLTT16, GX_TEXGEN_NONE, GX_TEXSIZE_S8, GX_TEXSIZE_T8, GX_TEXREPEAT_ST, GX_TEXFLIP_NONE, GX_TEXPLTTCOLOR0_USE, NNS_GfdGetTexKeyAddr(sys->texKey));
    G3_TexPlttBase(NNS_GfdGetPlttKeyAddr(sys->plttKey), GX_TEXFMT_PLTT16);
    G3_MaterialColorDiffAmb(0x7fff, 0x4210, TRUE);
    G3_MaterialColorSpecEmi(0x4210, 0, FALSE);
    G3_PolygonAttr(GX_LIGHTMASK_NONE, GX_POLYGONMODE_MODULATE, GX_CULL_NONE, 63, 31, 0);
    if (poly->tail != 0) {
        G3_Begin(0);
        G3_Color(win->color);
        G3_TexCoord(0, 0x8000);
        G3_Vtx(poly->vtx[2].x, poly->vtx[2].y, poly->vtx[2].z);
        G3_TexCoord(0, 0);
        G3_Vtx(poly->vtx[0].x, poly->vtx[0].y, poly->vtx[0].z);
        G3_TexCoord(0x8000, 0);
        G3_Vtx(poly->vtx[1].x, poly->vtx[1].y, poly->vtx[1].z);
        G3_End();
    }
    if (shown == 0) {
        G3_Begin(1);
        G3_Color(win->color);
        G3_TexCoord(0, 0x8000);
        G3_Vtx(poly->vtx[5].x, poly->vtx[5].y, poly->vtx[5].z);
        G3_TexCoord(0, 0);
        G3_Vtx(poly->vtx[3].x, poly->vtx[3].y, poly->vtx[3].z);
        G3_TexCoord(0x8000, 0);
        G3_Vtx(poly->vtx[4].x, poly->vtx[4].y, poly->vtx[4].z);
        G3_TexCoord(0x8000, 0x8000);
        G3_Vtx(poly->vtx[6].x, poly->vtx[6].y, poly->vtx[6].z);
        G3_End();
    }
    G3_PopMtx(1);
}

void func_ov021_02194e74(VecFx32 *world, s32 *x, s32 *y, VecFx32 *out) {
    NNS_G3dWorldPosToScrPos(world, x, y);
    NNS_G3dScrPosToWorldLine(*x, *y, out, NULL);
}

void func_ov021_02194e90(TalkMsgWin *win, TalkMsgWin *other, s32 *param_3, s32 *param_4, VecFx32 *param_5) {
    s32 x1;
    s32 y1;
    s32 wid1;
    s32 hgt1;
    s32 x2;
    s32 y2;
    s32 wid2;
    s32 hgt2;

    x1 = (BmpWin_GetPosX(win->bmpWin) - 1) * 8;
    y1 = (BmpWin_GetPosY(win->bmpWin) - 1) * 8;
    wid1 = (BmpWin_GetWidth1(win->bmpWin) + 2) * 8;
    hgt1 = (BmpWin_GetHeight2(win->bmpWin) + 2) * 8;
    x2 = (BmpWin_GetPosX(other->bmpWin) - 1) * 8;
    y2 = (BmpWin_GetPosY(other->bmpWin) - 1) * 8;
    wid2 = (BmpWin_GetWidth1(other->bmpWin) + 2) * 8;
    hgt2 = (BmpWin_GetHeight2(other->bmpWin) + 2) * 8;

    if (x1 + wid1 < x2) {
        *param_3 = x2;
    } else if (x1 > x2 + wid2) {
        *param_3 = x2 + wid2;
    } else {
        *param_3 = x2 + wid2 / 2;
    }
    if (y1 + hgt1 < y2) {
        *param_4 = y2;
    } else if (y1 > y2 + hgt2) {
        *param_4 = y2 + hgt2;
    } else {
        *param_4 = y2 + hgt2 / 2;
    }
    NNS_G3dScrPosToWorldLine(*param_3, *param_4, param_5, NULL);
}

void func_ov021_02194f5c(s32 param_1, s32 param_2, s32 param_3, u16 *param_4, u16 *param_5) {
    if (param_3 == 0) {
        *param_4 = param_1 + 0x40;
        *param_5 = *param_4 + 0x10;
    } else {
        *param_5 = param_1 + param_2 - 0x40;
        *param_4 = *param_5 - 0x10;
    }
}

u32 func_ov021_02194f84(TalkMsgWin *win, s32 px, s32 py, VecFx32 *near1, VecFx32 *near2) {
    s32 x = (BmpWin_GetPosX(win->bmpWin) - 1) * 8;
    s32 y = (BmpWin_GetPosY(win->bmpWin) - 1) * 8;
    s32 width = (BmpWin_GetWidth1(win->bmpWin) + 2) * 8;
    s32 height = (BmpWin_GetHeight2(win->bmpWin) + 2) * 8;
    u16 top;
    u16 bottom;
    u16 left;
    u16 right;
    u32 result;
    s32 flag;

    y += GFL_BGSysGetBGOffsetY(BmpWin_GetBGIndex(win->bmpWin));
    switch (win->shape) {
    default:
    case 0: {
        s32 hh;

        if (width >= 0x10 && height >= 0x10) {
            hh = 0x10;
        } else {
            hh = height;
        }
        if (py < y) {
            bottom = top = y + 3;
            left = x + width / 2 - hh / 2;
            right = x + width / 2 + hh / 2;
            result = 1;
        } else if (py > y + height) {
            bottom = top = y + height - 3;
            left = x + width / 2 - hh / 2;
            right = x + width / 2 + hh / 2;
            result = 2;
        } else {
            top = y + height / 2 - hh / 2;
            bottom = y + height / 2 + hh / 2;
            if (px < x) {
                left = x + 1;
                right = x + 1;
                result = 3;
            } else if (px > x + width) {
                left = x + width - 1;
                right = x + width - 1;
                result = 4;
            } else {
                left = x + width / 2;
                right = x + width / 2;
                result = 0;
            }
        }
        break;
    }
    case 1:
        bottom = top = y + height - 4;
        if (win->poly.tail == 0) {
            if (px < 0x80) {
                flag = 0;
                result = 7;
            } else {
                flag = 1;
                result = 8;
            }
        } else if (win->poly.tail == 7) {
            flag = 0;
            result = 7;
        } else {
            flag = 1;
            result = 8;
        }
        func_ov021_02194f5c(x, width, flag, &left, &right);
        break;
    case 2:
        bottom = top = y + 4;
        if (win->poly.tail == 0) {
            if (px < 0x80) {
                flag = 0;
                result = 5;
            } else {
                flag = 1;
                result = 6;
            }
        } else if (win->poly.tail == 5) {
            flag = 0;
            result = 5;
        } else {
            flag = 1;
            result = 6;
        }
        func_ov021_02194f5c(x, width, flag, &left, &right);
        break;
    }
    NNS_G3dScrPosToWorldLine(left, top, near1, NULL);
    NNS_G3dScrPosToWorldLine(right, bottom, near2, NULL);
    return result;
}

void func_ov021_02195128(TalkMsgWin *win, VecFx32 *param_2, VecFx32 *param_3, VecFx32 *param_4, VecFx32 *param_5) {
    s32 x;
    s32 y;
    s32 w;
    s32 h;

    x = BmpWin_GetPosX(win->bmpWin) * 8;
    y = (BmpWin_GetPosY(win->bmpWin) - 1) * 8;
    w = BmpWin_GetWidth1(win->bmpWin) * 8;
    h = (BmpWin_GetHeight2(win->bmpWin) + 2) * 8;
    NNS_G3dScrPosToWorldLine(x, y, param_2, NULL);
    NNS_G3dScrPosToWorldLine(x + w, y, param_3, NULL);
    NNS_G3dScrPosToWorldLine(x, y + h, param_4, NULL);
    NNS_G3dScrPosToWorldLine(x + w, y + h, param_5, NULL);
}

void func_ov021_0219518c(TalkMsgWinSys *sys, TalkMsgWin *win,const VecFx32 *target, const VecFx32 *start, const VecFx32 *end, const VecFx32 *topLeft, const VecFx32 *topRight, const VecFx32 *bottomLeft, const VecFx32 *bottomRight) {
    VecFx32 toStart;
    VecFx32 toEnd;
    VecFx32 toTopLeft;
    VecFx32 toTopRight;
    VecFx32 toBottomLeft;
    VecFx32 toBottomRight;
    fx32 lenStart;
    fx32 lenEnd;
    fx32 lenTopLeft;
    fx32 lenTopRight;
    fx32 lenBottomLeft;
    fx32 lenBottomRight;
    fx32 size;

    VEC_Subtract(start, target, &toStart);
    VEC_Subtract(end, target, &toEnd);
    VEC_Subtract(topLeft, target, &toTopLeft);
    VEC_Subtract(topRight, target, &toTopRight);
    VEC_Subtract(bottomLeft, target, &toBottomLeft);
    VEC_Subtract(bottomRight, target, &toBottomRight);
    lenStart = VEC_Mag(&toStart);
    lenEnd = VEC_Mag(&toEnd);
    lenTopLeft = VEC_Mag(&toTopLeft);
    lenTopRight = VEC_Mag(&toTopRight);
    lenBottomLeft = VEC_Mag(&toBottomLeft);
    lenBottomRight = VEC_Mag(&toBottomRight);
    size = lenStart;
    if (size < lenEnd) {
        size = lenEnd;
    }
    if (size < lenTopLeft) {
        size = lenTopLeft;
    }
    if (size < lenTopRight) {
        size = lenTopRight;
    }
    if (size < lenBottomLeft) {
        size = lenBottomLeft;
    }
    if (size < lenBottomRight) {
        size = lenBottomRight;
    }
    VEC_Set(&win->poly.origin, target->x, target->y, target->z);
    win->poly.size = size;
    VEC_Fx16Set(&win->poly.vtx[0], 0, 0, 0);
    VEC_Fx16Set(&win->poly.vtx[1], FX_Div(toStart.x, size), FX_Div(toStart.y, size), FX_Div(toStart.z, size));
    VEC_Fx16Set(&win->poly.vtx[2], FX_Div(toEnd.x, size), FX_Div(toEnd.y, size), FX_Div(toEnd.z, size));
    VEC_Fx16Set(&win->poly.vtx[3], FX_Div(toTopLeft.x, size), FX_Div(toTopLeft.y, size), FX_Div(toTopLeft.z, size));
    VEC_Fx16Set(&win->poly.vtx[4], FX_Div(toTopRight.x, size), FX_Div(toTopRight.y, size), FX_Div(toTopRight.z, size));
    VEC_Fx16Set(&win->poly.vtx[5], FX_Div(toBottomLeft.x, size), FX_Div(toBottomLeft.y, size), FX_Div(toBottomLeft.z, size));
    VEC_Fx16Set(&win->poly.vtx[6], FX_Div(toBottomRight.x, size), FX_Div(toBottomRight.y, size), FX_Div(toBottomRight.z, size));
}

u32 func_ov021_02195338(TalkMsgWinSys *sys, TalkMsgWin *win) {
    VecFx32 VStack_20;
    VecFx32 VStack_2c;
    VecFx32 VStack_38;
    VecFx32 VStack_44;
    VecFx32 VStack_50;
    VecFx32 VStack_5c;
    VecFx32 VStack_68;
    VecFx32 VStack_74;
    s32 x;
    s32 y;
    s32 result;

    if (win->target != NULL) {
        VEC_Subtract(win->target, &sys->camTarget, &VStack_74);
        func_ov021_02194e74(&VStack_74, &x, &y, &VStack_20);
    } else {
        func_ov021_02194e90(win, &sys->windows[win->anchorWin], &x, &y, &VStack_20);
    }
    result = func_ov021_02194f84(win, x, y, &VStack_2c, &VStack_38);
    func_ov021_02195128(win, &VStack_44, &VStack_50, &VStack_5c, &VStack_68);
    func_ov021_0219518c(sys, win, &VStack_20, &VStack_2c, &VStack_38, &VStack_44, &VStack_50, &VStack_5c, &VStack_68);
    if (win->poly.tail != result) {
        win->poly.tail = result;
        return 1;
    }
    return 0;
}

void func_ov021_021953d4(TalkMsgWinSys *sys, s32 draw) {
    VecFx32 VStack_1c;
    VecFx32 VStack_28;
    VecFx32 VStack_34;
    VecFx32 VStack_40;
    fx32 fStack_44;

    VEC_Subtract(&sys->camPos, &sys->camTarget, &VStack_1c);
    VStack_28.x = VStack_1c.x;
    VStack_28.y = VStack_1c.y;
    VStack_28.z = VStack_1c.z;
    VStack_34.x = 0;
    VStack_34.y = 0x1000;
    VStack_34.z = 0;
    VStack_40.x = 0;
    VStack_40.y = 0;
    VStack_40.z = 0;
    if (draw == 0) {
        fStack_44 = 0x40000;
        if (sys->camZNear >= 0x40000) {
            fStack_44 = sys->camZNear + 0x1000;
        }
        GFL_G3DCameraSetLookatPos((G3DCamera *)sys->setup.unk4, &VStack_28);
        GFL_G3DCameraSetLookatUpVector((G3DCamera *)sys->setup.unk4, &VStack_34);
        GFL_G3DCameraSetLookatTarget((G3DCamera *)sys->setup.unk4, &VStack_40);
        GFL_G3DCameraSetProjectionZNear((G3DCamera *)sys->setup.unk4, &fStack_44);
        GFL_G3DCameraFlush((G3DCamera *)sys->setup.unk4);
    } else {
        G3i_LookAt_(&VStack_28, &VStack_34, &VStack_40, TRUE, NULL);
    }
}

void func_ov021_0219545c(TalkMsgWinSys *sys) {
    GFL_G3DCameraGetLookatPos((G3DCamera *)sys->setup.unk4, &sys->camPos);
    GFL_G3DCameraGetLookatUpVector((G3DCamera *)sys->setup.unk4, &sys->camUp);
    GFL_G3DCameraGetLookatTarget((G3DCamera *)sys->setup.unk4, &sys->camTarget);
    GFL_G3DCameraGetProjectionZNear((G3DCamera *)sys->setup.unk4, &sys->camZNear);
}

void func_ov021_02195490(TalkMsgWinSys *sys) {
    GFL_G3DCameraSetLookatPos((G3DCamera *)sys->setup.unk4, &sys->camPos);
    GFL_G3DCameraSetLookatUpVector((G3DCamera *)sys->setup.unk4, &sys->camUp);
    GFL_G3DCameraSetLookatTarget((G3DCamera *)sys->setup.unk4, &sys->camTarget);
    GFL_G3DCameraSetProjectionZNear((G3DCamera *)sys->setup.unk4, &sys->camZNear);
    GFL_G3DCameraFlush((G3DCamera *)sys->setup.unk4);
}

u32 func_ov021_021954cc(TalkMsgWinSys *sys, TalkMsgWinSetup *setup) {
    u32 type;

    GFL_BGSysClearScr(setup->bg);
    type = 0;
    switch (setup->bg) {
    case 0:
    case 1:
    case 2:
    case 3:
        break;
    case 4:
    case 5:
    case 6:
    case 7:
        type = 4;
        break;
    }
    GFL_BGSysLoadNCLRDefault(5, 0, type, setup->unk14 << 5, 0x20, setup->heapId);
    return GFL_BGSysLoadNCGRStatic(5, 5, setup->bg, setup->unkC, 0, 0, setup->heapId);
}

void func_ov021_02195530(TalkMsgWinSys *sys) {
    GX_BeginLoadTex();
    DC_FlushRange(TALKMSG_WIN_TEX, sizeof(TALKMSG_WIN_TEX));
    GX_LoadTex(TALKMSG_WIN_TEX, NNS_GfdGetTexKeyAddr(sys->texKey), sizeof(TALKMSG_WIN_TEX));
    GX_EndLoadTex();
    GX_BeginLoadTexPltt();
    DC_FlushRange(TALKMSG_WIN_PLT, sizeof(TALKMSG_WIN_PLT));
    GX_LoadTexPltt(TALKMSG_WIN_PLT, NNS_GfdGetPlttKeyAddr(sys->plttKey), sizeof(TALKMSG_WIN_PLT));
    GX_EndLoadTexPltt();
}

void func_ov021_02195588(TalkMsgWinSys *sys) {
    u32 type;

    GFL_BGSysClearScr(sys->setup.bg);
    type = 0;
    switch (sys->setup.bg) {
    case 0:
    case 1:
    case 2:
    case 3:
        break;
    case 4:
    case 5:
    case 6:
    case 7:
        type = 4;
        break;
    }
    GFL_BGSysLoadNCLRDefault(5, 0, type, sys->setup.unk14 << 5, 0x20, sys->setup.heapId);
    GFL_BGSysLoadNCGRStatic(5, 5, sys->setup.bg, sys->setup.unkC, 0, 0, sys->setup.heapId);
}

void func_ov021_021955ec(TalkMsgWinSys *sys, BmpWin *bmpWin, u32 tail) {
    int x = BmpWin_GetPosX(bmpWin);
    int y = BmpWin_GetPosY(bmpWin);
    u16 width = BmpWin_GetWidth1(bmpWin);
    u16 height = BmpWin_GetHeight2(bmpWin);
    int atTop = 0;
    int atBottom = 0;
    int atLeft = 0;
    int atRight = 0;
    u8 bg = sys->setup.bg;
    u32 pal = (sys->setup.unk14 & 0xf) << 12;
    u16 base = sys->setup.unkC;
    u16 tileTail;
    int above;
    int right;
    int bottom;

    if (x == 0) {
        atLeft = 1;
    }
    right = x + width;
    if (right == 32) {
        atRight = 1;
    }
    if (y == 0) {
        atTop = 1;
    }
    bottom = y + height;
    if (bottom == 24) {
        atBottom = 1;
    }
    BmpWin_FlushMap(bmpWin);
    if (atTop == 0) {
        if (atLeft == 0) {
            GFL_BGSysFillScrArea(bg, (base + 1) | pal, x - 1, y - 1, 1, 1, BGSYS_FILL_TILE_PALETTE);
        }
        above = y - 1;
        GFL_BGSysFillScrArea(bg, (base + 3) | pal, x, above, width, 1, BGSYS_FILL_TILE_PALETTE);
        if (atRight == 0) {
            GFL_BGSysFillScrArea(bg, (base + 0x401) | pal, right, above, 1, 1, BGSYS_FILL_TILE_PALETTE);
        }
    }
    if (atLeft == 0) {
        GFL_BGSysFillScrArea(bg, (base + 4) | pal, x - 1, y, 1, height, BGSYS_FILL_TILE_PALETTE);
    }
    if (atRight == 0) {
        GFL_BGSysFillScrArea(bg, (base + 0x404) | pal, right, y, 1, height, BGSYS_FILL_TILE_PALETTE);
    }
    if (atBottom == 0) {
        if (atLeft == 0) {
            GFL_BGSysFillScrArea(bg, (base + 0x801) | pal, x - 1, bottom, 1, 1, BGSYS_FILL_TILE_PALETTE);
        }
        GFL_BGSysFillScrArea(bg, (base + 0x803) | pal, x, bottom, width, 1, BGSYS_FILL_TILE_PALETTE);
        if (atRight == 0) {
            GFL_BGSysFillScrArea(bg, (base + 0xc01) | pal, right, bottom, 1, 1, BGSYS_FILL_TILE_PALETTE);
        }
    }
    tileTail = (base + 2) | pal;
    switch (tail) {
    case 0:
        break;
    case 1:
        if (atTop == 0) {
            GFL_BGSysFillScrArea(bg, tileTail, x + width / 2 - 1, y - 1, 2, 1, BGSYS_FILL_TILE_PALETTE);
        }
        break;
    case 2:
        if (atBottom == 0) {
            GFL_BGSysFillScrArea(bg, tileTail | 0x800, x + width / 2 - 1, bottom, 2, 1, BGSYS_FILL_TILE_PALETTE);
        }
        break;
    case 3:
        if (atLeft == 0) {
            GFL_BGSysFillScrArea(bg, (base + 5) | pal, x - 1, y + height / 2 - 1, 1, 2, BGSYS_FILL_TILE_PALETTE);
        }
        break;
    case 4:
        if (atRight == 0) {
            GFL_BGSysFillScrArea(bg, (base + 5) | pal, right, y + height / 2 - 1, 1, 2, BGSYS_FILL_TILE_PALETTE);
        }
        break;
    case 5:
        if (atTop == 0) {
            GFL_BGSysFillScrArea(bg, tileTail, x + 7, y - 1, 2, 1, BGSYS_FILL_TILE_PALETTE);
        }
        break;
    case 7:
        if (atTop == 0) {
            GFL_BGSysFillScrArea(bg, tileTail | 0x800, x + 7, bottom, 2, 1, BGSYS_FILL_TILE_PALETTE);
        }
        break;
    case 6:
        if (atBottom == 0) {
            right -= 9;
            GFL_BGSysFillScrArea(bg, tileTail, right, y - 1, 2, 1, BGSYS_FILL_TILE_PALETTE);
        }
        break;
    case 8:
        if (atBottom == 0) {
            right -= 9;
            GFL_BGSysFillScrArea(bg, tileTail | 0x800, right, bottom, 2, 1, BGSYS_FILL_TILE_PALETTE);
        }
        break;
    }
    GFL_BGSysLoadScr(bg);
}

void func_ov021_02195938(TalkMsgWinSys *sys, BmpWin *bmpWin, u32 tail) {
    int atTop;
    int atBottom;
    int atLeft;
    int atRight;
    u8 bg;
    int topLeftX;
    int rightEdge;
    int topRightY;
    int bottomEdge;
    int bottomLeftX;
    int bottomRightY;
    u32 x;
    u32 width;
    u32 height;
    int y;
    u32 pal;
    u16 base;
    int topRightX;
    int bottomLeftY;
    int bottomRightX;
    int topLeftY;
    u16 tileTail;

    x = BmpWin_GetPosX(bmpWin);
    y = BmpWin_GetPosY(bmpWin);
    width = BmpWin_GetWidth1(bmpWin);
    height = BmpWin_GetHeight2(bmpWin);
    atTop = 0;
    atBottom = 0;
    atLeft = 0;
    atRight = 0;
    bg = sys->setup.bg;
    pal = (sys->setup.unk14 & 0xf) << 12;
    base = sys->setup.unkC;

    if (x == 0) {
        atLeft = 1;
    }
    rightEdge = (int)x + (int)width;
    if (rightEdge == 32) {
        atRight = 1;
    }
    if (y == 0) {
        atTop = 1;
    }
    bottomEdge = y + (int)height;
    if (bottomEdge == 24) {
        atBottom = 1;
    }
    BmpWin_FlushMap(bmpWin);
    if (atTop == 0) {
        if (atLeft == 0) {
            topLeftX = x - 1;
            topLeftY = y - 1;
            GFL_BGSysFillScrArea(bg, (base + 9) | pal, topLeftX, topLeftY, 1, 1, BGSYS_FILL_TILE_PALETTE);
            GFL_BGSysFillScrArea(bg, (base + 0xa) | pal, x, topLeftY, 1, 1, BGSYS_FILL_TILE_PALETTE);
            GFL_BGSysFillScrArea(bg, (base + 0xb) | pal, topLeftX, y, 1, 1, BGSYS_FILL_TILE_PALETTE);
        }
        GFL_BGSysFillScrArea(bg, (base + 6) | pal, x + 1, y - 1, width - 1, 1, BGSYS_FILL_TILE_PALETTE);
        if (atRight == 0) {
            topRightY = y - 1;
            topRightX = x + width;
            GFL_BGSysFillScrArea(bg, (base + 0x409) | pal, topRightX, topRightY, 1, 1, BGSYS_FILL_TILE_PALETTE);
            GFL_BGSysFillScrArea(bg, (base + 0x40a) | pal, topRightX - 1, topRightY, 1, 1, BGSYS_FILL_TILE_PALETTE);
            GFL_BGSysFillScrArea(bg, (base + 0x40b) | pal, topRightX, y, 1, 1, BGSYS_FILL_TILE_PALETTE);
        }
    }
    if (atLeft == 0) {
        if (atTop == 0) {
            GFL_BGSysFillScrArea(bg, (base + 7) | pal, x - 1, y + 1, 1, height - 1, BGSYS_FILL_TILE_PALETTE);
        } else {
            GFL_BGSysFillScrArea(bg, (base + 7) | pal, x - 1, y, 1, height, BGSYS_FILL_TILE_PALETTE);
        }
    }
    if (atRight == 0) {
        if (atTop == 0) {
            GFL_BGSysFillScrArea(bg, (base + 0x407) | pal, rightEdge, y + 1, 1, height - 1, BGSYS_FILL_TILE_PALETTE);
        } else {
            GFL_BGSysFillScrArea(bg, (base + 0x407) | pal, rightEdge, y, 1, height, BGSYS_FILL_TILE_PALETTE);
        }
    }
    if (atBottom == 0) {
        if (atLeft == 0) {
            bottomLeftY = y + height;
            bottomLeftX = x - 1;
            GFL_BGSysFillScrArea(bg, (base + 0x809) | pal, bottomLeftX, bottomLeftY, 1, 1, BGSYS_FILL_TILE_PALETTE);
            GFL_BGSysFillScrArea(bg, (base + 0x80a) | pal, x, bottomLeftY, 1, 1, BGSYS_FILL_TILE_PALETTE);
            GFL_BGSysFillScrArea(bg, (base + 0x80b) | pal, bottomLeftX, bottomLeftY - 1, 1, 1, BGSYS_FILL_TILE_PALETTE);
        }
        GFL_BGSysFillScrArea(bg, (base + 0x806) | pal, x + 1, bottomEdge, width - 1, 1, BGSYS_FILL_TILE_PALETTE);
        if (atRight == 0) {
            bottomRightY = y + height;
            bottomRightX = x + width;
            GFL_BGSysFillScrArea(bg, (base + 0xc09) | pal, bottomRightX, bottomRightY, 1, 1, BGSYS_FILL_TILE_PALETTE);
            GFL_BGSysFillScrArea(bg, (base + 0xc0a) | pal, bottomRightX - 1, bottomRightY, 1, 1, BGSYS_FILL_TILE_PALETTE);
            GFL_BGSysFillScrArea(bg, (base + 0xc0b) | pal, bottomRightX, bottomRightY - 1, 1, 1, BGSYS_FILL_TILE_PALETTE);
        }
    }
    tileTail = (base + 8) | pal;
    switch (tail) {
    case 0:
        break;
    case 1:
        if (atTop == 0) {
            GFL_BGSysFillScrArea(bg, tileTail, x + width / 2 - 1, y - 1, 2, 1, BGSYS_FILL_TILE_PALETTE);
        }
        break;
    case 2:
        if (atBottom == 0) {
            GFL_BGSysFillScrArea(bg, tileTail | 0x800, x + width / 2 - 1, bottomEdge, 2, 1, BGSYS_FILL_TILE_PALETTE);
        }
        break;
    case 3:
        if (atLeft == 0) {
            GFL_BGSysFillScrArea(bg, (base + 5) | pal, x - 1, y + height / 2 - 1, 1, 2, BGSYS_FILL_TILE_PALETTE);
        }
        break;
    case 4:
        if (atRight == 0) {
            GFL_BGSysFillScrArea(bg, (base + 5) | pal, rightEdge, y + height / 2 - 1, 1, 2, BGSYS_FILL_TILE_PALETTE);
        }
        break;
    case 5:
        if (atTop == 0) {
            GFL_BGSysFillScrArea(bg, tileTail, x + 7, y - 1, 2, 1, BGSYS_FILL_TILE_PALETTE);
        }
        break;
    case 7:
        if (atTop == 0) {
            GFL_BGSysFillScrArea(bg, tileTail | 0x800, x + 7, bottomEdge, 2, 1, BGSYS_FILL_TILE_PALETTE);
        }
        break;
    case 6:
        if (atBottom == 0) {
            rightEdge -= 9;
            GFL_BGSysFillScrArea(bg, tileTail, rightEdge, y - 1, 2, 1, BGSYS_FILL_TILE_PALETTE);
        }
        break;
    case 8:
        if (atBottom == 0) {
            rightEdge -= 9;
            GFL_BGSysFillScrArea(bg, tileTail | 0x800, rightEdge, bottomEdge, 2, 1, BGSYS_FILL_TILE_PALETTE);
        }
        break;
    }
    GFL_BGSysLoadScr(bg);
}

void func_ov021_02195e54(TalkMsgWinSys *sys, BmpWin *bmpWin, s32 frameType, u32 tail) {
    switch (frameType) {
    case 0:
        func_ov021_021955ec(sys, bmpWin, tail);
        break;
    case 1:
        func_ov021_02195938(sys, bmpWin, tail);
        break;
    }
}

void func_ov021_02195e70(TalkMsgWinSys *sys, TalkMsgWin *win) {
    func_ov021_02195e54(sys, win->bmpWin, win->frameType, 0);
}

void func_ov021_02195e80(TalkMsgWinSys *sys, TalkMsgWin *win) {
    func_ov021_02195e54(sys, win->bmpWin, win->frameType, win->poly.tail);
}

#define TALKMSG_WIN_SCREEN_WIDTH 32

#define TALKMSG_WIN_SCREEN_HEIGHT 24
void func_ov021_02195e90(TalkMsgWinSys *sys, BmpWin *param_2) {
    int posY;
    int posX;
    int width;
    int height;
    int atTop;
    int atBottom;
    int hasLeft;
    int above;
    int right;
    int bottom;
    int hasRight;
    u8 bg;

    posX = BmpWin_GetPosX(param_2);
    posY = BmpWin_GetPosY(param_2);
    width = BmpWin_GetWidth1(param_2);
    height = BmpWin_GetHeight2(param_2);
    atTop = 0;
    atBottom = 0;
    hasLeft = 0;
    hasRight = 0;
    bg = sys->setup.bg;
    func_020456f8(param_2);
    if (posX == 0) {
        hasLeft = 1;
    }
    right = posX + width;
    if (right == TALKMSG_WIN_SCREEN_WIDTH) {
        hasRight = 1;
    }
    if (posY == 0) {
        atTop = 1;
    }
    bottom = posY + height;
    if (bottom == TALKMSG_WIN_SCREEN_HEIGHT) {
        atBottom = 1;
    }
    if (atTop == 0) {
        if (hasLeft == 0) {
            GFL_BGSysFillScrArea(bg, 0, posX - 1, posY - 1, 1, 1, 0x11);
        }
        above = posY - 1;
        GFL_BGSysFillScrArea(bg, 0, posX, above, width, 1, BGSYS_FILL_TILE_PALETTE);
        if (hasRight == 0) {
            GFL_BGSysFillScrArea(bg, 0, right, above, 1, 1, BGSYS_FILL_TILE_PALETTE);
        }
    }
    if (hasLeft == 0) {
        GFL_BGSysFillScrArea(bg, 0, posX - 1, posY, 1, height, 0x11);
    }
    if (hasRight == 0) {
        GFL_BGSysFillScrArea(bg, 0, right, posY, 1, height, 0x11);
    }
    if (atBottom == 0) {
        if (hasLeft == 0) {
            GFL_BGSysFillScrArea(bg, 0, posX - 1, bottom, 1, 1, 0x11);
        }
        GFL_BGSysFillScrArea(bg, 0, posX, bottom, width, 1, 0x11);
        if (hasRight == 0) {
            GFL_BGSysFillScrArea(bg, 0, right, bottom, 1, 1, 0x11);
        }
    }
    GFL_BGSysLoadScr(bg);
}
