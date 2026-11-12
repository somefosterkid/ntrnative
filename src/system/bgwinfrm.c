// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/bgwinfrm.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "system/bgwinfrm.h"
#include "types.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmpwin.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "nitro/mi.h"



// Frames of BG screen data that are put on a BG, cleared and slid across it. The file name is the ROM's (its heap
// allocations' assert text); names are ours

// The default area a frame shows in: the whole screen, in tiles
#define SCREEN_WIDTH_TILES 32
#define SCREEN_HEIGHT_TILES 24

typedef struct {
    u16 *screen;
    // In tiles
    u16 width : 6;
    u16 height : 6;
    // Not on the BG: set up, or cleared
    u16 hidden : 1;
    u16 moving : 3;
    s8 x;
    s8 y;
    s8 moveX;
    s8 moveY;
    u8 bg;
    u8 moveCount;
    // The area of the BG it shows in
    s8 left;
    s8 right;
    s8 top;
    s8 bottom;
} BGWinFrameEntry;

struct BGWinFrame {
    BGWinFrameEntry *entries;
    u16 count;
    u16 transferMode : 15;
    u16 moving : 1;
    u32 heapId;
};

static void func_0202d018(u8 bg);

// pokebw2's sBGWinFrameTransferFuncs
static void (*const data_020a0058[])(u8 bg) = {
    [BGWINFRAME_TRANSFER_NONE] = func_0202d018,
    [BGWINFRAME_TRANSFER_NOW] = GFL_BGSysLoadScr,
    [BGWINFRAME_TRANSFER_VBLANK] = GFL_BGSysQueueScrLoad,
};

// pokebw2's BGWinFrame_Create
BGWinFrame *func_0202c9b4(u32 transferMode, u32 count, HeapID heapId) {
    BGWinFrame *frames = GFL_HeapAllocate(heapId, sizeof(BGWinFrame), FALSE, "bgwinfrm.c", 86);

    frames->count = count;
    frames->heapId = heapId;
    frames->transferMode = transferMode;
    frames->moving = FALSE;
    frames->entries = GFL_HeapAllocate(heapId, count * sizeof(BGWinFrameEntry), TRUE, "bgwinfrm.c", 93);
    return frames;
}

// pokebw2's BGWinFrame_Delete
void func_0202ca0c(BGWinFrame *frames) {
    u32 i;

    for (i = 0; i < frames->count; i++) {
        if (frames->entries[i].screen != NULL) {
            GFL_HeapFree(frames->entries[i].screen);
        }
    }
    GFL_HeapFree(frames->entries);
    GFL_HeapFree(frames);
}

// pokebw2's BGWinFrame_InitFrame
void func_0202ca3c(BGWinFrame *frames, u32 index, u32 bg, u32 width, u32 height) {
    BGWinFrameEntry *frame = &frames->entries[index];

    frame->screen = GFL_HeapAllocate(frames->heapId, width * height * sizeof(u16), TRUE, "bgwinfrm.c", 138);
    frame->width = width;
    frame->height = height;
    frame->bg = bg;
    frame->x = 0;
    frame->y = 0;
    frame->moving = 0;
    frame->hidden = TRUE;
    frame->left = 0;
    frame->right = SCREEN_WIDTH_TILES;
    frame->top = 0;
    frame->bottom = SCREEN_HEIGHT_TILES;
}

// pokebw2's BGWinFrame_SetScreen
void func_0202cac4(BGWinFrame *frames, u32 index, u16 *screen) {
    BGWinFrameEntry *frame = &frames->entries[index];

    MIi_CpuCopy16(screen, frame->screen, frame->width * frame->height * sizeof(u16));
}

// pokebw2's BGWinFrame_LoadScreen
void func_0202cae4(BGWinFrame *frames, u32 index, u32 arcId, u32 fileId, BOOL compressed) {
    NNSG2dScreenData *screen;
    void *file = GFL_G2DIOReadNSCR(arcId, fileId, compressed, &screen, frames->heapId);

    func_0202cac4(frames, index, (u16 *)screen->rawData);
    GFL_HeapFree(file);
}

// pokebw2's BGWinFrame_LoadScreenArc
void func_0202cb18(BGWinFrame *frames, u32 index, ArcTool *arc, u32 fileId, BOOL compressed) {
    NNSG2dScreenData *screen;
    void *file = GFL_G2DIOReadNSCRArc(arc, fileId, compressed, &screen, frames->heapId);

    func_0202cac4(frames, index, (u16 *)screen->rawData);
    GFL_HeapFree(file);
}

// pokebw2's BGWinFrame_Put
void func_0202cb4c(BGWinFrame *frames, u32 index, s8 x, s8 y) {
    BGWinFrameEntry *frame = &frames->entries[index];
    u8 putX, putY, sizeX, sizeY, srcX, srcY;

    frame->x = x;
    frame->y = y;
    if (x >= frame->right || y >= frame->bottom || x + frame->width < frame->left || y + frame->height < frame->top) {
        return;
    }

    putX = x;
    sizeX = frame->width;
    srcX = 0;
    if (x < frame->left) {
        putX = frame->left;
        srcX = frame->left - x;
        sizeX -= srcX;
    }
    if (x + frame->width >= frame->right) {
        sizeX -= x + frame->width - frame->right;
    }

    putY = y;
    sizeY = frame->height;
    srcY = 0;
    if (y < frame->top) {
        putY = frame->top;
        srcY = frame->top - y;
        sizeY -= srcY;
    }
    if (y + frame->height >= frame->bottom) {
        sizeY -= y + frame->height - frame->bottom;
    }

    GFL_BGSysLoadScrArea(frame->bg, putX, putY, sizeX, sizeY, frame->screen, srcX, srcY, frame->width, frame->height);
    data_020a0058[frames->transferMode](frame->bg);
    frame->hidden = FALSE;
}

// pokebw2's BGWinFrame_Show
void func_0202cc58(BGWinFrame *frames, u32 index) {
    func_0202cb4c(frames, index, frames->entries[index].x, frames->entries[index].y);
}

// pokebw2's BGWinFrame_Hide
void func_0202cc70(BGWinFrame *frames, u32 index) {
    BGWinFrameEntry *frame = &frames->entries[index];
    s8 x, y, sizeX, sizeY;

    frame->hidden = TRUE;
    if (frame->x >= frame->right || frame->y >= frame->bottom || frame->x + frame->width < frame->left ||
        frame->y + frame->height < frame->top) {
        return;
    }

    x = frame->x;
    sizeX = frame->width;
    if (frame->x < frame->left) {
        x = frame->left;
        sizeX -= frame->left - frame->x;
    }
    if (frame->x + frame->width >= frame->right) {
        sizeX -= frame->x + frame->width - frame->right;
    }

    y = frame->y;
    sizeY = frame->height;
    if (frame->y < frame->top) {
        y = frame->top;
        sizeY -= frame->top - frame->y;
    }
    if (frame->y + frame->height >= frame->bottom) {
        sizeY -= frame->y + frame->height - frame->bottom;
    }

    GFL_BGSysFillScrArea(frame->bg, 0, x, y, sizeX, sizeY, BGSYS_FILL_KEEP_PALETTE);
    data_020a0058[frames->transferMode](frame->bg);
}

// pokebw2's BGWinFrame_StartMove
void func_0202cd64(BGWinFrame *frames, u32 index, s8 moveX, s8 moveY, u8 count) {
    BGWinFrameEntry *frame = &frames->entries[index];

    frame->moveX = moveX;
    frame->moveY = moveY;
    frame->moveCount = count;
    frame->moving = 1;
    frames->moving = TRUE;
}

// pokebw2's BGWinFrame_UpdateMoves
void func_0202cd94(BGWinFrame *frames) {
    u32 i;

    if (frames->moving == FALSE) {
        return;
    }
    frames->moving = FALSE;
    for (i = 0; i < frames->count; i++) {
        if (func_0202cdd4(frames, i) == TRUE) {
            frames->moving = TRUE;
        }
    }
}

// pokebw2's BGWinFrame_MoveStep
BOOL func_0202cdd4(BGWinFrame *frames, u32 index) {
    if (frames->entries[index].moving == 0) {
        return FALSE;
    }
    func_0202cc70(frames, index);
    frames->entries[index].x += frames->entries[index].moveX;
    frames->entries[index].y += frames->entries[index].moveY;
    func_0202cc58(frames, index);
    frames->entries[index].moveCount--;
    if (frames->entries[index].moveCount == 0) {
        frames->entries[index].moving = 0;
        return FALSE;
    }
    return TRUE;
}

// pokebw2's BGWinFrame_IsMoving
BOOL func_0202ce40(BGWinFrame *frames, u32 index) {
    return frames->entries[index].moving;
}

// pokebw2's BGWinFrame_SetPalette
void func_0202ce50(BGWinFrame *frames, u32 index, u8 x, u8 y, u8 width, u8 height, u8 palette) {
    u16 *screen = frames->entries[index].screen;
    u32 frameWidth = frames->entries[index].width;
    u16 i, j;

    for (i = y; i < y + height; i++) {
        for (j = x; j < x + width; j++) {
            screen[i * frameWidth + j] = (screen[i * frameWidth + j] & 0xfff) | (palette << 12);
        }
    }
}

// pokebw2's BGWinFrame_WriteBmpWin
void func_0202cebc(BGWinFrame *frames, u32 index, BmpWin *window) {
    u16 *screen = frames->entries[index].screen;
    u32 frameWidth = frames->entries[index].width;
    u16 charPos = func_02045768(window);
    u16 palette = func_02045778(window) << 12;
    u8 sizeX = func_02045738(window);
    u8 sizeY = func_02045740(window);
    u8 posX = BmpWin_GetPosX(window);
    u8 posY = BmpWin_GetPosY(window);
    u16 i, j;

    for (i = 0; i < sizeY; i++) {
        if (posY + i >= frames->entries[index].height) {
            break;
        }
        for (j = 0; j < sizeX; j++) {
            if (posX + j >= frames->entries[index].width) {
                break;
            }
            screen[(posY + i) * frameWidth + posX + j] = palette + (charPos + j);
        }
        charPos += sizeX;
    }
}

// pokebw2's BGWinFrame_GetScreen
u16 *func_0202cf84(BGWinFrame *frames, u32 index) {
    return frames->entries[index].screen;
}

// pokebw2's BGWinFrame_GetBG
u8 func_0202cf8c(BGWinFrame *frames, u32 index) {
    return frames->entries[index].bg;
}

// pokebw2's BGWinFrame_GetPos
void func_0202cf98(BGWinFrame *frames, u32 index, s8 *x, s8 *y) {
    if (x != NULL) {
        *x = frames->entries[index].x;
    }
    if (y != NULL) {
        *y = frames->entries[index].y;
    }
}

// pokebw2's BGWinFrame_GetSize
void func_0202cfc0(BGWinFrame *frames, u32 index, u16 *width, u16 *height) {
    if (width != NULL) {
        *width = frames->entries[index].width;
    }
    if (height != NULL) {
        *height = frames->entries[index].height;
    }
}

// pokebw2's BGWinFrame_SetArea
void func_0202cfec(BGWinFrame *frames, u32 index, s8 left, s8 right, s8 top, s8 bottom) {
    frames->entries[index].left = left;
    frames->entries[index].right = right;
    frames->entries[index].top = top;
    frames->entries[index].bottom = bottom;
}

// pokebw2's BGWinFrame_NoTransfer
static void func_0202d018(u8 bg) {
}
