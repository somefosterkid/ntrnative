// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov203/wf2dmap_scrdraw.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/wf2dmap/wf2dmap_scrdraw.h"
#include "app/wf2dmap/wf2dmap_scroll.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "nnsys/g2d.h"

// Draws the Wi-Fi Club's 2D map on a BG, named after the ROM's "wf2dmap_scrdraw.c"

// The part of the map that is written to the BG's screen, in characters: the display and one more row and column
#define WF2DMAP_SCRDRAW_WIDTH 33
#define WF2DMAP_SCRDRAW_HEIGHT 25

// A screen wider or taller than this is stored as blocks of this size
#define WF2DMAP_SCRDRAW_BLOCK_SIZE 32

// Which sides of the map's screen are larger than a block
enum {
    WF2DMAP_SCRDRAW_SCRN_SMALL,
    WF2DMAP_SCRDRAW_SCRN_WIDE,
    WF2DMAP_SCRDRAW_SCRN_TALL,
    WF2DMAP_SCRDRAW_SCRN_LARGE,
};

struct WF2DMAP_SCRDRAW {
    ClActRenderer *renderer;
    void *unk4;
    void *scrnBuff;
    NNSG2dScreenData *scrn;
    u32 surface;
    u32 bg;
    // The character of the map at the BG's top left corner
    u16 scrnX;
    u16 scrnY;
};

typedef struct {
    s16 width;
    s16 height;
} WF2DMAP_SCRDRAWSIZE;

static void func_ov127_021c5aac(void *unk0, u32 bg, const WF2DMAP_SCRDRAWINIT *init);
static void func_ov127_021c5b0c(WF2DMAP_SCRDRAW *sys, s16 x, s16 y);
static void func_ov127_021c5b98(void *unk0, u32 bg, const NNSG2dScreenData *scrn, s16 x, s16 y);
static void func_ov127_021c5c8c(void *unk0, u8 bg, u8 x, u8 y, u8 width, u8 height, const void *buff,
                                            u8 srcX, u8 srcY, u8 srcWidth, u8 srcHeight);
static u32 func_ov127_021c5e14(u32 width, u32 height);
static const void *func_ov127_021c5e2c(const void *buff, u8 blockX, u8 blockY, u32 type, u8 width, u8 height,
                                                  WF2DMAP_SCRDRAWSIZE *size);

WF2DMAP_SCRDRAW *func_ov127_021c59f4(ClActRenderer *renderer, void *unk1, const WF2DMAP_SCRDRAWINIT *init,
                                        u32 heapId) {
    WF2DMAP_SCRDRAW *sys;

    sys = GFL_HeapAllocate(heapId, sizeof(WF2DMAP_SCRDRAW), TRUE, "wf2dmap_scrdraw.c", 104);
    sys->renderer = renderer;
    sys->unk4 = unk1;
    sys->surface = init->surface;
    sys->bg = init->bg;
    func_ov127_021c5aac(unk1, init->bg, init);
    sys->scrnBuff = GFL_G2DIOReadNSCR(init->arcId, init->fileId, init->compressed, &sys->scrn, heapId);
    return sys;
}

// pokebw2's WF2DMAP_SCRDrawSysExit
void func_ov127_021c5a4c(WF2DMAP_SCRDRAW *sys) {
    GFL_HeapFree(sys->scrnBuff);
    GFL_HeapFree(sys);
}

// pokebw2's WF2DMAP_SCRDrawSysMain
void func_ov127_021c5a60(WF2DMAP_SCRDRAW *sys, const WF2DMAP_SCROLL *scroll) {
    s16 top;
    s16 left;
    ClActorPos pos;

    top = func_ov127_021c59e4(scroll);
    left = func_ov127_021c59ec(scroll);

    pos.x = left;
    pos.y = top;
    if (sys->surface == 0) {
        func_0204b09c(sys->renderer, 0, &pos);
    } else {
        pos.y += 192;
        func_0204b09c(sys->renderer, 1, &pos);
    }

    func_ov127_021c5b0c(sys, left, top);
}

// pokebw2's WF2DMAP_SCRDrawSysBGInit
static void func_ov127_021c5aac(void *unk0, u32 bg, const WF2DMAP_SCRDRAWINIT *init) {
    BGSetup setup = { 0, 0, 0x1000, 0, BGRES_512x256, 0, 0, 0, 0, 0, 0, 0, 0 };

    setup.colorMode = init->colorMode;
    setup.screenBase = init->screenBase;
    setup.charBase = init->charBase;
    setup.extPaletteSlot = init->extPaletteSlot;
    setup.priority = init->priority;
    setup.areaOverflow = 0;
    setup.mosaic = init->mosaic;

    GFL_BGSysReleaseBG(bg);
    GFL_BGSysCreateBG(bg, &setup, BGMODE_TEXT);
    GFL_BGSysSetBGEnabled(bg, TRUE);
}

// Scrolls the BG within a character, and writes its screen again when the map moves by a character
// pokebw2's WF2DMAP_SCRDrawSysBGScroll
static void func_ov127_021c5b0c(WF2DMAP_SCRDRAW *sys, s16 x, s16 y) {
    s16 scrnX;
    s16 scrnY;
    s16 ofsX;
    s16 ofsY;

    scrnX = x / 8;
    scrnY = y / 8;
    ofsX = x % 8;
    ofsY = y % 8;

    if (scrnX != sys->scrnX || scrnY != sys->scrnY) {
        sys->scrnX = scrnX;
        sys->scrnY = scrnY;
        func_ov127_021c5b98(sys->unk4, sys->bg, sys->scrn, -scrnX, -scrnY);
        GFL_BGSysQueueScrLoad(sys->bg);
    }

    GFL_BGSysMoveBGReq(sys->bg, BG_MOVE_SET_X, ofsX);
    GFL_BGSysMoveBGReq(sys->bg, BG_MOVE_SET_Y, ofsY);
}

// Writes the map with its top left corner at character (x, y) of the BG, which is negative when the map is scrolled
// pokebw2's WF2DMAP_SCRDrawSysScrnWrite
static void func_ov127_021c5b98(void *unk0, u32 bg, const NNSG2dScreenData *scrn, s16 x, s16 y) {
    s16 scrnWidth;
    s16 scrnHeight;
    s16 srcX;
    s16 srcY;
    s16 width;
    s16 height;

    scrnWidth = scrn->screenWidth / 8;
    scrnHeight = scrn->screenHeight / 8;

    if (x < 0) {
        srcX = -x;
    } else {
        srcX = 0;
    }
    if (y < 0) {
        srcY = -y;
    } else {
        srcY = 0;
    }
    if (x <= 0) {
        x = 0;
    }
    if (y <= 0) {
        y = 0;
    }

    width = WF2DMAP_SCRDRAW_WIDTH - x;
    height = WF2DMAP_SCRDRAW_HEIGHT - y;
    if (scrnWidth < srcX + width) {
        width -= (s16)(srcX + width - scrnWidth);
    }
    if (scrnHeight < srcY + height) {
        height -= (s16)(srcY + height - scrnHeight);
    }

    GFL_BGSysFillScrArea(bg, 0, 0, 0, WF2DMAP_SCRDRAW_WIDTH, WF2DMAP_SCRDRAW_HEIGHT, 17);
    func_ov127_021c5c8c(unk0, bg, x, y, width, height, scrn->rawData, srcX, srcY, scrnWidth, scrnHeight);
}

// GFL_BGSysLoadScrArea for a source of any size: a source stored as blocks is written block by block
static void func_ov127_021c5c8c(void *unk0, u8 bg, u8 x, u8 y, u8 width, u8 height, const void *buff,
                                            u8 srcX, u8 srcY, u8 srcWidth, u8 srcHeight) {
    s8 restHeight;
    s8 writeHeight;
    u32 type;
    s8 restWidth;
    s8 writeWidth;
    s8 blockX;
    s8 blockY;
    s8 readX;
    s8 writeX;
    s32 ofsX;
    s32 ofsY;
    const void *blockBuff;
    WF2DMAP_SCRDRAWSIZE size;

    type = func_ov127_021c5e14(srcWidth, srcHeight);
    if (type == WF2DMAP_SCRDRAW_SCRN_SMALL) {
        GFL_BGSysLoadScrArea(bg, x, y, width, height, buff, srcX, srcY, srcWidth, srcHeight);
        return;
    }

    restHeight = height;
    while (restHeight > 0) {
        ofsY = srcY % WF2DMAP_SCRDRAW_BLOCK_SIZE;
        if (ofsY + restHeight <= WF2DMAP_SCRDRAW_BLOCK_SIZE) {
            writeHeight = restHeight;
            restHeight = 0;
        } else {
            writeHeight = WF2DMAP_SCRDRAW_BLOCK_SIZE - ofsY;
            restHeight -= writeHeight;
        }
        blockY = srcY / WF2DMAP_SCRDRAW_BLOCK_SIZE;

        restWidth = width;
        readX = srcX;
        writeX = x;
        while (restWidth > 0) {
            ofsX = readX % WF2DMAP_SCRDRAW_BLOCK_SIZE;
            if (ofsX + restWidth <= WF2DMAP_SCRDRAW_BLOCK_SIZE) {
                writeWidth = restWidth;
                restWidth = 0;
            } else {
                writeWidth = WF2DMAP_SCRDRAW_BLOCK_SIZE - ofsX;
                restWidth -= writeWidth;
            }
            blockX = readX / WF2DMAP_SCRDRAW_BLOCK_SIZE;

            blockBuff = func_ov127_021c5e2c(buff, blockX, blockY, type, srcWidth, srcHeight, &size);
            GFL_BGSysLoadScrArea(bg, writeX, y, writeWidth, writeHeight, blockBuff, ofsX, ofsY, size.width,
                                 size.height);

            readX += writeWidth;
            writeX += writeWidth;
        }

        srcY += (u8)writeHeight;
        y += (u8)writeHeight;
    }
}

// pokebw2's WF2DMAP_SCRDrawSysScrnTypeGet
static u32 func_ov127_021c5e14(u32 width, u32 height) {
    u32 type;

    if (width <= WF2DMAP_SCRDRAW_BLOCK_SIZE) {
        type = WF2DMAP_SCRDRAW_SCRN_SMALL;
        if (height > WF2DMAP_SCRDRAW_BLOCK_SIZE) {
            type = WF2DMAP_SCRDRAW_SCRN_TALL;
        }
    } else {
        type = WF2DMAP_SCRDRAW_SCRN_WIDE;
        if (height > WF2DMAP_SCRDRAW_BLOCK_SIZE) {
            type = WF2DMAP_SCRDRAW_SCRN_LARGE;
        }
    }
    return type;
}

// The screen of a block of the source, and the block's size in size
static const void *func_ov127_021c5e2c(const void *buff, u8 blockX, u8 blockY, u32 type, u8 width, u8 height,
                                                  WF2DMAP_SCRDRAWSIZE *size) {
    u16 offset;

    switch (type) {
    case WF2DMAP_SCRDRAW_SCRN_SMALL:
        size->width = width;
        size->height = height;
        return buff;

    case WF2DMAP_SCRDRAW_SCRN_WIDE:
        if ((blockX + 1) * WF2DMAP_SCRDRAW_BLOCK_SIZE <= width) {
            size->width = WF2DMAP_SCRDRAW_BLOCK_SIZE;
        } else {
            size->width = width - blockX * WF2DMAP_SCRDRAW_BLOCK_SIZE;
        }
        size->height = height;
        return (const u8 *)buff + (s16)(height * WF2DMAP_SCRDRAW_BLOCK_SIZE * 2) * blockX;

    case WF2DMAP_SCRDRAW_SCRN_TALL:
        size->width = width;
        if ((blockY + 1) * WF2DMAP_SCRDRAW_BLOCK_SIZE <= height) {
            size->height = WF2DMAP_SCRDRAW_BLOCK_SIZE;
        } else {
            size->height = height - blockY * WF2DMAP_SCRDRAW_BLOCK_SIZE;
        }
        return (const u8 *)buff + (s16)(width * WF2DMAP_SCRDRAW_BLOCK_SIZE * 2) * blockY;

    case WF2DMAP_SCRDRAW_SCRN_LARGE:
        if ((blockX + 1) * WF2DMAP_SCRDRAW_BLOCK_SIZE <= width) {
            size->width = WF2DMAP_SCRDRAW_BLOCK_SIZE;
        } else {
            size->width = width - blockX * WF2DMAP_SCRDRAW_BLOCK_SIZE;
        }
        if ((blockY + 1) * WF2DMAP_SCRDRAW_BLOCK_SIZE <= height) {
            size->height = WF2DMAP_SCRDRAW_BLOCK_SIZE;
        } else {
            size->height = height - blockY * WF2DMAP_SCRDRAW_BLOCK_SIZE;
        }
        if (blockY == 0) {
            offset = 0;
            if (blockX != 0) {
                offset += WF2DMAP_SCRDRAW_BLOCK_SIZE * WF2DMAP_SCRDRAW_BLOCK_SIZE * 2;
            }
        } else {
            offset = width * WF2DMAP_SCRDRAW_BLOCK_SIZE * 2;
            if (blockX != 0) {
                offset += height * WF2DMAP_SCRDRAW_BLOCK_SIZE * 2;
            }
        }
        return (const u8 *)buff + offset;
    }
    return NULL;
}
