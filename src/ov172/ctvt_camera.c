// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov257/ctvt_camera.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "app/comm_tvt/ctvt_camera.h"
#include "types.h"
#include "app/comm_tvt/camera_system.h"
#include "app/comm_tvt/comm_tvt_sys.h"
#include "app/comm_tvt/ctvt_comm.h"
#include "constants/sound.h"
#include "gfl/arc_util.h"
#include "gfl/heap.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_cp.h"
#include "nitro/gx.h"
#include "nitro/os.h"
#include "system/dsi.h"
#include "twl/camera.h"

// pokebw2's prototypes of what Black's headers don't declare, under pokebw2's names until tools/port_file.py
// gives them Black's from the relocations
CtvtCamera *CommTvt_GetCamera(CommTvtWork *sys);
BOOL CtvtComm_HasMemberCamera(CommTvtWork *sys, CtvtComm *comm, u8 member);
BOOL CtvtComm_IsMemberInfoReceived(CommTvtWork *sys, CtvtComm *comm, u8 member);
void DC_FlushRange(const void *addr, u32 size);
BOOL func_ov257_021aab10(CommTvtWork *sys);
BOOL func_ov257_021aab18(CommTvtWork *sys);
BOOL func_ov257_021aab5c(CommTvtWork *sys);
void func_ov257_021aab60(CommTvtWork *sys, BOOL value);

// The Xtransceiver's video. Each member of the call has a window on the top screen, BG 3 of the main engine, a
// direct-color bitmap. The own camera's frames are cropped into a buffer and sent to the others, and each member's
// picture is copied into its window once it changes. Without a camera, a picture from the archive stands in. The
// windows slide into place when the layout changes, and the screen opens from the top when the video starts

#define CTVT_CAMERA_HEAP_SIZE 0x80000
// The bitmap of the top screen, 256 x 192 direct colors
#define CTVT_SCREEN_SIZE 0x18000
// A copy of the middle of a camera frame, 128 x 192 direct colors
#define CTVT_FRAME_COPY_SIZE 0xc000

// The opening of the screen grows by this many lines each frame, up to the screen's height
#define CTVT_OPEN_SPEED 8
#define CTVT_OPEN_END 192

void func_ov172_021b75ac(void *frame, void *data);
static void func_ov172_021b7888(CtvtCameraWindow *window, u16 x, u16 y, u16 width, u16 height);
void func_ov172_021b78a0(CtvtCameraWindow *window, u16 x, u16 y, u16 width, u16 height);
static BOOL func_ov172_021b78b8(CtvtCamera *work, CtvtCameraWindow *window);
static BOOL func_ov172_021b792c(CtvtCamera *work, u16 *cur, u16 *target);

const u16 data_ov257_021b18c4[4] = { 0, 0x80, 0x40, 0xc0 };

static const u32 data_ov172_021c6e9c[] = {
    [CTDM_SINGLE] = 256,
    [CTDM_DOUBLE] = 128,
    [CTDM_QUAD] = 128,
};

static const u32 data_ov172_021c6ea8[] = {
    [CTDM_SINGLE] = 192,
    [CTDM_DOUBLE] = 192,
    [CTDM_QUAD] = 96,
};

static const u32 data_ov172_021c6eb4[] = {
    [CTDM_SINGLE] = 256 * 192 * 2,
    [CTDM_DOUBLE] = 128 * 192 * 2,
    [CTDM_QUAD] = 128 * 96 * 2,
};

CtvtCamera *func_ov172_021b6ee0(CommTvtWork *sys, HeapID heapId) {
    CtvtCamera *pCVar1;
    ArcTool *arc;
    BOOL BVar2;
    CameraSystem *pCVar3;
    void *frameBuffers;
    u16 *frameCopy;
    u8 *screen;
    u32 i;

    pCVar1 = GFL_HeapAllocate(heapId,sizeof(CtvtCamera),1,"ctvt_camera.c",0x67);
    arc = func_ov172_021be010(sys);
    BVar2 = func_ov172_021be100();
    if (BVar2 == 1) {
        pCVar1->heapId = 0x6e;
        GFL_HeapCreateChild(1, 0x6e, 0x80000);
        pCVar3 = func_ov172_021b6100(pCVar1->heapId);
        pCVar1->camera = pCVar3;
        func_ov172_021b6950(pCVar1->camera, CAMERA_SIZE_DS_LCD);
        func_ov172_021b69bc(pCVar1->camera, CAMERA_SELECT_IN);
        func_ov172_021b674c(pCVar1->camera, 2, pCVar1->heapId & 0x7fff | 0x8000);
        func_ov172_021b6744(pCVar1->camera, func_ov172_021b75ac, sys);
        func_ov172_021b6814(pCVar1->camera);
        frameBuffers = GFL_HeapAllocate(pCVar1->heapId & 0x7fff | 0x8000,0xc000,1,"ctvt_camera.c",0x76);
        pCVar1->frameBuffers[0] = frameBuffers;
        frameBuffers = GFL_HeapAllocate(pCVar1->heapId & 0x7fff | 0x8000,0xc000,1,"ctvt_camera.c",0x77);
        pCVar1->frameBuffers[1] = frameBuffers;
        frameCopy = GFL_HeapAllocate(pCVar1->heapId & 0x7fff | 0x8000,0xc000,1,"ctvt_camera.c",0x78);
        pCVar1->frameCopy = frameCopy;
        frameBuffers = GFL_ArcToolReadHeapNewLZ(arc, 0x19, 0, pCVar1->heapId);
        pCVar1->noCameraPicture = frameBuffers;
        frameBuffers = GFL_ArcToolReadHeapNewLZ(arc, 0x1a, 0, pCVar1->heapId);
    }
    else {
        frameBuffers = GFL_ArcToolReadHeapNewLZ(arc, 0x19, 0, heapId);
        pCVar1->noCameraPicture = frameBuffers;
        frameBuffers = GFL_ArcToolReadHeapNewLZ(arc, 0x1a, 0, heapId);
    }
    pCVar1->noCameraPictureZoomed = frameBuffers;
    i = 0;
    pCVar1->dirtyMask = 0;
    pCVar1->redrawFrames = 0;
    pCVar1->redrawAll = 0;
    pCVar1->openLines = 0xc0;
    pCVar1->startOpening = 0;
    pCVar1->updateDisplay = 0;
    pCVar1->zoomed = 0;
    pCVar1->opening = 0;
    pCVar1->recording = 0;
    pCVar1->recordingEnded = 0;
    screen = GFL_HeapAllocate(heapId & 0x7fff | 0x8000,0x18000,1,"ctvt_camera.c",0x8c);
    pCVar1->screen = screen;
    do {
        func_ov172_021b7888(pCVar1->windows + i, 0, 0, 0, 0);
        func_ov172_021b78a0(pCVar1->windows + i, 0, 0, 0, 0);
        pCVar1->windows[i].moving = 0;
        pCVar1->windows[i].slide = 1;
        pCVar1->windows[i].playSE = 1;
        i = i + 1 & 0xff;
    } while (i < 4);
    return pCVar1;
}

void func_ov172_021b70a8(CommTvtWork *sys, CtvtCamera *work) {
    MtxFx22 mtx;

    mtx._00 = FX_Inv(FX32_ONE);
    mtx._11 = FX_Inv(FX32_ONE);
    mtx._01 = 0;
    mtx._10 = 0;
    G2_SetBG3Affine(&mtx, 0, 0, 0, 0);
    GFL_HeapFree(work->screen);
    GFL_HeapFree(work->noCameraPictureZoomed);
    GFL_HeapFree(work->noCameraPicture);
    if (func_ov172_021be100() == TRUE) {
        GFL_HeapFree(work->frameCopy);
        GFL_HeapFree(work->frameBuffers[0]);
        GFL_HeapFree(work->frameBuffers[1]);
        func_ov172_021b63b8(work->camera);
        GFL_HeapDelete(0x6e);
    }
    GFL_HeapFree(work);
}


// pokebw2's CtvtCamera_Update
void func_ov172_021b712c(CommTvtWork *sys, CtvtCamera *work) {
    u8 i;

    if (work->redrawAll == FALSE) {
        for (i = 0; i < CTVT_CAMERA_MEMBERS; i++) {
            if (func_ov172_021b78b8(work, &work->windows[i]) == TRUE) {
                work->dirtyMask |= 1 << i;
                break;
            }
        }
    }
    if (func_ov172_021be100() == TRUE) {
        func_ov172_021b64ac(work->camera);
    }
    if (work->openLines < CTVT_OPEN_END && work->opening == FALSE) {
        work->openLines += CTVT_OPEN_SPEED;
        G2_SetWnd0Position(0, 0, 255, work->openLines);
        G2_SetWnd1Position(128, 0, 256, work->openLines);
    }
}

// pokebw2's CtvtCamera_Draw
void func_ov172_021b71b8(CommTvtWork *sys, CtvtCamera *work) {
    MtxFx22 mtx;
    int displayMode = func_ov172_021be05c(sys);
    BOOL zoomed = func_ov172_021be078(sys);
    CtvtComm *comm = func_ov172_021bdfe4(sys);
    u8 i;
    u8 count;
    fx32 scale;
    int pos;
    int width;
    u32 size;
    int row;
    uintptr_t src;
    u16 height;

    if (func_ov172_021be054(sys) < 2) {
        return;
    }

    if (displayMode == CTDM_SINGLE) {
        if (work->dirtyMask != 0) {
            DC_FlushRange(work->screen, CTVT_SCREEN_SIZE);
            GX_LoadBG3Scr(work->screen, 0, CTVT_SCREEN_SIZE);
            work->dirtyMask = 0;
        }
    } else {
        if (work->redrawAll == TRUE) {
            count = 0;
            for (i = 0; i < CTVT_CAMERA_MEMBERS; i++) {
                if (work->dirtyMask & (1 << i)) {
                    count++;
                }
            }
            if (count != func_ov172_021be054(sys)) {
                return;
            }
            if (work->redrawFrames < 2) {
                work->dirtyMask = 0;
                work->redrawFrames++;
                for (i = 0; i < CTVT_CAMERA_MEMBERS; i++) {
                    if (func_ov172_021b8b8c(sys, comm, i) == TRUE &&
                        func_ov172_021b8bac(sys, comm, i) == FALSE) {
                        work->dirtyMask |= 1 << i;
                    }
                }
                return;
            }
            if (work->openLines < CTVT_OPEN_END) {
                return;
            }
            if (work->startOpening == TRUE) {
                work->openLines = 0;
                work->opening = TRUE;
                work->startOpening = FALSE;
                G2_SetWnd0Position(0, 0, 255, 0);
                G2_SetWnd1Position(128, 0, 256, 0);
            }
            if (work->updateDisplay == TRUE) {
                work->updateDisplay = FALSE;
                func_ov172_021be1e4(sys);
            }
            MIi_CpuClear32(0, G2_GetBG3ScrPtr(), CTVT_SCREEN_SIZE);
            work->redrawAll = FALSE;
            if (work->zoomed != func_ov172_021be078(sys)) {
                work->zoomed = func_ov172_021be078(sys);
                if (work->zoomed == TRUE) {
                    scale = FX32_ONE * 2;
                    mtx._00 = FX_Inv(scale);
                    mtx._11 = FX_Inv(scale);
                } else {
                    scale = FX32_ONE;
                    mtx._00 = FX_Inv(scale);
                    mtx._11 = FX_Inv(scale);
                }
                mtx._01 = 0;
                mtx._10 = 0;
                G2_SetBG3Affine(&mtx, 0, 0, 0, 0);
                for (i = 0; i < CTVT_CAMERA_MEMBERS; i++) {
                    if (work->zoomed == TRUE) {
                        work->windows[i].target.x /= 2;
                        work->windows[i].target.y /= 2;
                        work->windows[i].target.width /= 2;
                        work->windows[i].target.height /= 2;
                        work->windows[i].cur.x /= 2;
                        work->windows[i].cur.y /= 2;
                        work->windows[i].cur.width /= 2;
                        work->windows[i].cur.height /= 2;
                    } else {
                        work->windows[i].target.x *= 2;
                        work->windows[i].target.y *= 2;
                        work->windows[i].target.width *= 2;
                        work->windows[i].target.height *= 2;
                        work->windows[i].cur.x *= 2;
                        work->windows[i].cur.y *= 2;
                        work->windows[i].cur.width *= 2;
                        work->windows[i].cur.height *= 2;
                    }
                }
            }
            work->opening = FALSE;
        }

        for (i = 0; i < CTVT_CAMERA_MEMBERS; i++) {
            if (!(work->dirtyMask & (1 << i))) {
                continue;
            }
            if (func_ov172_021b8b8c(sys, comm, i) == TRUE) {
                pos = i;
                if (displayMode == CTDM_DOUBLE && i != 0) {
                    pos = 1;
                }
                if (zoomed == FALSE) {
                    width = 128;
                    height = displayMode == CTDM_DOUBLE ? 192 : 96;
                } else {
                    width = 64;
                    height = displayMode == CTDM_DOUBLE ? 96 : 48;
                }
                if (func_ov172_021b8bac(sys, comm, i) == TRUE && !canPlayerExchangePhotos()) {
                    size = width * height * 2;
                    src = (uintptr_t)work->screen + pos * size;
                    DC_FlushRange((void *)src, size);
                } else if (zoomed == FALSE) {
                    if (displayMode == CTDM_DOUBLE) {
                        src = (uintptr_t)work->noCameraPicture;
                        DC_FlushRange((void *)src, 0xc000);
                    } else {
                        src = (uintptr_t)work->noCameraPicture + 0x3000;
                        DC_FlushRange((void *)src, 0x6000);
                    }
                } else {
                    if (displayMode == CTDM_DOUBLE) {
                        src = (uintptr_t)work->noCameraPictureZoomed;
                        DC_FlushRange((void *)src, 0x3000);
                    } else {
                        src = (uintptr_t)work->noCameraPictureZoomed + 0xc00;
                        DC_FlushRange((void *)src, 0x1800);
                    }
                }
                for (row = 0; row < work->windows[i].cur.height; row++) {
                    int y = height - work->windows[i].cur.height + row;

                    GX_LoadBG3Scr((void *)(src + y * width * 2 + (width - work->windows[i].cur.width) * 2),
                                        work->windows[i].cur.x * 2 + (work->windows[i].cur.y + row) * 512,
                                        work->windows[i].cur.width * 2);
                }
            }
            work->dirtyMask -= 1 << i;
        }
    }
}

// pokebw2's CtvtCamera_StopCamera
void func_ov172_021b7578(CommTvtWork *sys, CtvtCamera *work) {
    if (func_ov172_021be100() == TRUE) {
        func_ov172_021b6868(work->camera);
    }
}

// pokebw2's CtvtCamera_IsSoundDone
BOOL func_ov172_021b7590(CommTvtWork *sys, CtvtCamera *work) {
    if (func_ov172_021be100() == TRUE) {
        return func_ov172_021b688c(work->camera);
    }
    return TRUE;
}

// pokebw2's CtvtCamera_OnFrame
void func_ov172_021b75ac(void *frame, void *data) {
    CommTvtWork *sys = data;
    CtvtCamera *work = func_ov172_021bdfdc(sys);
    int displayMode = func_ov172_021be05c(sys);
    BOOL zoomed = func_ov172_021be078(sys);
    int left;
    u8 self = func_ov172_021be0a4(sys);
    int y;
    u8 pos;
    u16 top;
    int width;
    u16 height;

    if (func_ov172_021be0ac(sys) == TRUE || func_ov172_021be054(sys) < 2) {
        return;
    }
    if (work->dirtyMask & (1 << self)) {
        return;
    }

    if (func_ov172_021be0b4(sys) == FALSE) {
        for (y = 0; y < 192; y++) {
            MIi_CpuCopy32((u16 *)frame + y * 256 + 64, work->frameCopy + y * 128, 256);
        }
    }
    if (displayMode == CTDM_SINGLE) {
        GFL_ASSERT(FALSE);
        return;
    }
    pos = self;
    if (displayMode == CTDM_DOUBLE && self > 1) {
        pos = 1;
    }
    if (zoomed == FALSE) {
        left = 0;
        top = displayMode == CTDM_DOUBLE ? 0 : 48;
        width = 128;
        height = displayMode == CTDM_DOUBLE ? 192 : 96;
    } else {
        left = 32;
        top = displayMode == CTDM_DOUBLE ? 48 : 80;
        width = 64;
        height = displayMode == CTDM_DOUBLE ? 96 : 48;
    }

    if (func_ov172_021be0f8(sys) == FALSE) {
        func_ov172_021be0fc(sys, TRUE);
        for (y = 0; y < height; y++) {
            MIi_CpuCopy32((void *)((uintptr_t)work->frameCopy + (top + y) * 256 + left * 2),
                          (void *)((uintptr_t)work->screen + pos * (width * height * 2) + width * y * 2), width * 2);
        }
        func_ov172_021be0fc(sys, FALSE);
        work->dirtyMask |= 1 << self;
    }
}

// pokebw2's CtvtCamera_SetDirty
void func_ov172_021b7714(CommTvtWork *sys, CtvtCamera *work, u8 member) {
    work->dirtyMask |= 1 << member;
}

// pokebw2's CtvtCamera_ClearDirty
void func_ov172_021b7724(CommTvtWork *sys, CtvtCamera *work, u8 member) {
    if (work->redrawAll == FALSE) {
        u32 bit = 1 << member;

        if (work->dirtyMask & bit) {
            work->dirtyMask -= bit;
        }
    }
}

// pokebw2's CtvtCamera_Redraw
void func_ov172_021b7740(CommTvtWork *sys, CtvtCamera *work, BOOL startOpening, BOOL updateDisplay) {
    CtvtComm *comm = func_ov172_021bdfe4(sys);
    u8 i;

    work->redrawAll = TRUE;
    if (work->startOpening == FALSE) {
        work->startOpening = startOpening;
    }
    if (work->updateDisplay == FALSE) {
        work->updateDisplay = updateDisplay;
    }
    work->dirtyMask = 0;
    work->redrawFrames = 0;
    if (func_ov172_021be100() == FALSE) {
        work->dirtyMask |= 1 << func_ov172_021be0a4(sys);
    }
    for (i = 0; i < CTVT_CAMERA_MEMBERS; i++) {
        if (func_ov172_021b8ba4(sys, comm, i) == TRUE) {
            func_ov172_021b796c(sys, work, i);
        }
    }
}

// pokebw2's CtvtCamera_IsRedrawing
BOOL func_ov172_021b77b4(CommTvtWork *sys, CtvtCamera *work) {
    return work->redrawAll;
}

// pokebw2's CtvtCamera_GetWidth
u16 func_ov172_021b77b8(CommTvtWork *sys, CtvtCamera *work) {
    int displayMode = func_ov172_021be05c(sys);

    if (func_ov172_021be078(sys) == FALSE) {
        return data_ov172_021c6e9c[displayMode];
    }
    return data_ov172_021c6e9c[displayMode] / 2;
}

// pokebw2's CtvtCamera_GetHeight
u16 func_ov172_021b77e8(CommTvtWork *sys, CtvtCamera *work) {
    int displayMode = func_ov172_021be05c(sys);

    if (func_ov172_021be078(sys) == FALSE) {
        return data_ov172_021c6ea8[displayMode];
    }
    return data_ov172_021c6ea8[displayMode] / 2;
}

// pokebw2's CtvtCamera_GetOwnPicture
void *func_ov172_021b7818(CommTvtWork *sys, CtvtCamera *work) {
    return func_ov172_021b7830(sys, work, func_ov172_021be0a4(sys));
}

// pokebw2's CtvtCamera_GetPicture
void *func_ov172_021b7830(CommTvtWork *sys, CtvtCamera *work, u8 member) {
    u32 size = func_ov172_021b785c(sys, work);

    if (func_ov172_021be05c(sys) == CTDM_DOUBLE && member != 0) {
        return work->screen + size;
    }
    return work->screen + size * member;
}

// pokebw2's CtvtCamera_GetPictureSize
u32 func_ov172_021b785c(CommTvtWork *sys, CtvtCamera *work) {
    int displayMode = func_ov172_021be05c(sys);

    if (func_ov172_021be078(sys) == FALSE) {
        return data_ov172_021c6eb4[displayMode];
    }
    return data_ov172_021c6eb4[displayMode] / 4;
}

// pokebw2's CtvtCamera_SetWindowRect
static void func_ov172_021b7888(CtvtCameraWindow *window, u16 x, u16 y, u16 width, u16 height) {
    window->moving = TRUE;
    window->cur.x = x;
    window->cur.y = y;
    window->cur.width = width;
    window->cur.height = height;
}

// pokebw2's CtvtCamera_SetWindowTarget
void func_ov172_021b78a0(CtvtCameraWindow *window, u16 x, u16 y, u16 width, u16 height) {
    window->moving = TRUE;
    window->target.x = x;
    window->target.y = y;
    window->target.width = width;
    window->target.height = height;
}

// pokebw2's CtvtCamera_MoveWindow
static BOOL func_ov172_021b78b8(CtvtCamera *work, CtvtCameraWindow *window) {
    u32 moved;

    if (window->moving == TRUE) {
        moved = 0;
        moved |= func_ov172_021b792c(work, &window->cur.x, &window->target.x);
        moved |= func_ov172_021b792c(work, &window->cur.y, &window->target.y);
        moved |= func_ov172_021b792c(work, &window->cur.width, &window->target.width);
        moved |= func_ov172_021b792c(work, &window->cur.height, &window->target.height);
        if (!moved) {
            window->moving = FALSE;
        }
        if (window->playSE == TRUE) {
            window->playSE = FALSE;
            GFL_SndSEPlay(SEQ_SE_SYS_47);
        }
        window->slide = FALSE;
        return TRUE;
    }
    return FALSE;
}

// pokebw2's CtvtCamera_MoveCoord
static BOOL func_ov172_021b792c(CtvtCamera *work, u16 *cur, u16 *target) {
    u8 speed = work->zoomed == TRUE ? 4 : 8;

    if (*cur < *target) {
        if (*cur + speed < *target) {
            *cur += speed;
        } else {
            *cur = *target;
        }
        return TRUE;
    }
    if (*cur > *target) {
        if (*cur - speed > *target) {
            *cur -= speed;
        } else {
            *cur = *target;
        }
        return TRUE;
    }
    return FALSE;
}

// pokebw2's CtvtCamera_SetWindow
void func_ov172_021b796c(CommTvtWork *sys, CtvtCamera *work, u8 member) {
    int displayMode = func_ov172_021be05c(sys);
    int pos = member;

    if (displayMode == CTDM_DOUBLE && member != 0) {
        pos = 1;
    }
    if (work->zoomed == FALSE) {
        work->windows[member].target.x = (pos == 0 || pos == 2) ? 0 : 128;
        work->windows[member].target.y = (pos == 0 || pos == 1) ? 0 : 96;
        work->windows[member].target.width = 128;
        work->windows[member].target.height = displayMode == CTDM_DOUBLE ? 192 : 96;
    } else {
        work->windows[member].target.x = (pos == 0 || pos == 2) ? 0 : 64;
        work->windows[member].target.y = (pos == 0 || pos == 1) ? 0 : 48;
        work->windows[member].target.width = 64;
        work->windows[member].target.height = displayMode == CTDM_DOUBLE ? 96 : 48;
    }

    if (work->windows[member].slide == FALSE) {
        work->windows[member].cur.x = work->windows[member].target.x;
        work->windows[member].cur.y = work->windows[member].target.y;
        work->windows[member].cur.width = work->windows[member].target.width;
        work->windows[member].cur.height = work->windows[member].target.height;
    } else {
        // The window grows from the screen's corner or edge nearest to it
        work->windows[member].playSE = TRUE;
        switch (pos) {
        case 0:
            if (work->zoomed == FALSE) {
                func_ov172_021b7888(&work->windows[member], 0, 0, 128, 0);
            } else {
                func_ov172_021b7888(&work->windows[member], 0, 0, 64, 0);
            }
            break;
        case 1:
            if (work->zoomed == FALSE) {
                func_ov172_021b7888(&work->windows[member], 128, 0, 0, displayMode == CTDM_DOUBLE ? 192 : 96);
            } else {
                func_ov172_021b7888(&work->windows[member], 64, 0, 0, displayMode == CTDM_DOUBLE ? 96 : 48);
            }
            break;
        case 2:
            if (work->zoomed == FALSE) {
                func_ov172_021b7888(&work->windows[member], 0, 96, 128, 0);
            } else {
                func_ov172_021b7888(&work->windows[member], 0, 48, 64, 0);
            }
            break;
        case 3:
            if (work->zoomed == FALSE) {
                func_ov172_021b7888(&work->windows[member], 128, 96, 0, 96);
            } else {
                func_ov172_021b7888(&work->windows[member], 64, 48, 0, 48);
            }
            break;
        }
    }
    work->windows[member].moving = TRUE;
}

// pokebw2's CtvtCamera_MarkWindow
void func_ov172_021b7b20(CommTvtWork *sys, CtvtCamera *work, u8 member) {
    work->windows[member].slide = TRUE;
    work->windows[member].playSE = TRUE;
}

// pokebw2's CtvtCamera_IsWindowMoving
BOOL func_ov172_021b7b30(CommTvtWork *sys, CtvtCamera *work, u8 member) {
    return work->windows[member].moving;
}

void func_ov172_021b7b3c(CommTvtWork *sys, CtvtCamera *work, CAMERASize size) {
    BOOL BVar1;

    BVar1 = func_ov172_021be100();
    if ((BVar1 == 1) && (BVar1 = OS_IsRunOnTwl(), BVar1 == 1)) {
        func_02701248(3, 3, size);
    }
}

// pokebw2's CtvtCamera_StartRecording
void func_ov172_021b7b64(CommTvtWork *sys, CtvtCamera *work) {
    if (func_ov172_021be100() == TRUE) {
        func_ov172_021b69d0(work->camera);
        work->recording = TRUE;
    }
}

// pokebw2's CtvtCamera_EndRecording
void func_ov172_021b7b80(CommTvtWork *sys, CtvtCamera *work) {
    if (func_ov172_021be100() == TRUE && work->recording == TRUE && work->recordingEnded == FALSE) {
        func_ov172_021b69d8(work->camera);
        work->recordingEnded = TRUE;
    }
}

