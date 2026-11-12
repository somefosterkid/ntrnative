// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/bmp_oam.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "constants/arc.h"
#include "gfl/arc.h"
#include "gfl/bmp.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "nitro/gx.h"
#include "nitro/os.h"
#include "system/bmp_oam.h"

// Bitmaps shown as OAM sprites. The bitmap is cut into cells of 32x16 pixels, one cell actor each, and its pixels are
// uploaded to the actors' characters. Our names; swan has none for this file

// The size of a cell's characters, 32x16 pixels at 4 bits
#define CELL_CHAR_SIZE 0x100
// The size of a cell's row of 8 pixels
#define CELL_ROW_SIZE 0x80

#define CELL_ANIM_NONE 0xffffffff

// The OBJ character mappings that the cells come in
enum {
    CELL_MAPPING_1D_128K,
    CELL_MAPPING_1D_64K,
    CELL_MAPPING_1D_32K,
    CELL_MAPPING_COUNT,
};

struct BmpOamSys {
    ClActUnit *unit;
    HeapID heapId;
    ArcTool *arc;
    u32 cellAnims[CELL_MAPPING_COUNT];
};

#define BMP_OAM_CELL_MAX 24

struct BmpOamActor {
    GFLBitmap *bitmap;
    ClActor *actors[BMP_OAM_CELL_MAX];
    u32 vramType;
    u32 chars[BMP_OAM_CELL_MAX];
    u16 surface;
    // In cells
    u8 width;
    u8 height;
};

typedef struct {
    u32 charSize;
    u32 cellFileId;
    u32 animFileId;
} BmpOamCellRes;

static u32 func_02025a9c(BmpOamSys *sys, u32 vramType, ArcTool *arc);
static void func_02025b18(BmpOamSys *sys);

// pokebw2's sCellRes
static const BmpOamCellRes data_0209f87c[CELL_MAPPING_COUNT] = {
    [CELL_MAPPING_1D_128K] = { 0x130, 0, 1 },
    [CELL_MAPPING_1D_64K] = { 0x130, 3, 4 },
    [CELL_MAPPING_1D_32K] = { 0x130, 6, 7 },
};

// pokebw2's BmpOam_Init
BmpOamSys *func_02025538(HeapID heapId, ClActUnit *unit) {
    int i;
    BmpOamSys *sys = GFL_HeapAllocate(heapId, sizeof(BmpOamSys), TRUE, "bmp_oam.c", 115);

    sys->heapId = heapId;
    sys->unit = unit;
    for (i = 0; i < CELL_MAPPING_COUNT; i++) {
        sys->cellAnims[i] = CELL_ANIM_NONE;
    }
    sys->arc = GFL_ArcSysCreateFileHandle(0x33, HEAPID_TAIL(heapId));
    return sys;
}

// pokebw2's BmpOam_Exit
void func_02025588(BmpOamSys *sys) {
    GFL_ArcToolFree(sys->arc);
    func_02025b18(sys);
    GFL_HeapFree(sys);
}

// pokebw2's BmpOam_ActorAdd
BmpOamActor *func_020255a0(BmpOamSys *sys, const BmpOamActorSetup *setup) {
    ClActorSetup clSetup = { 0 };
    BmpOamActor *actor = GFL_HeapAllocate(sys->heapId, sizeof(BmpOamActor), TRUE, "bmp_oam.c", 163);
    u8 width, height;
    u8 x, y;
    ArcTool *arc;
    u32 mapping;

    actor->bitmap = setup->bitmap;
    width = GFL_BitmapGetWidth(setup->bitmap) / 32;
    height = GFL_BitmapGetHeight(setup->bitmap) / 16;
    if (GFL_BitmapGetWidth(setup->bitmap) % 32 != 0) {
        width++;
    }
    if (GFL_BitmapGetHeight(setup->bitmap) % 16 != 0) {
        height++;
    }
    actor->width = width;
    actor->height = height;
    actor->surface = setup->surface;
    actor->vramType = setup->vramType;

    arc = sys->arc;
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            mapping = func_02025a9c(sys, setup->vramType, arc);
            actor->chars[y * width + x] = func_0204a7a0(data_0209f87c[mapping].charSize, setup->vramType, sys->heapId);
            clSetup.x = setup->x + x * 32;
            clSetup.y = setup->y + y * 16;
            clSetup.priority = setup->priority;
            clSetup.bgPriority = setup->bgPriority;
            actor->actors[y * width + x] =
                func_0204b294(sys->unit, actor->chars[y * width + x], setup->palette, sys->cellAnims[mapping], &clSetup,
                              setup->surface, sys->heapId);
            func_0204b6f4(actor->actors[y * width + x], (u8)setup->paletteOffset, 0);
            func_0204ba20(actor->actors[y * width + x], FALSE);
        }
    }
    return actor;
}

// pokebw2's BmpOam_ActorDel
void func_0202570c(BmpOamActor *actor) {
    u8 x, y;

    for (y = 0; y < actor->height; y++) {
        for (x = 0; x < actor->width; x++) {
            func_0204a8d4(actor->chars[y * actor->width + x]);
            func_0204b3b4(actor->actors[y * actor->width + x]);
        }
    }
    GFL_HeapFree(actor);
}

// pokebw2's BmpOam_ActorSetDrawEnable
void func_02025774(BmpOamActor *actor, BOOL enable) {
    u8 x, y;

    for (y = 0; y < actor->height; y++) {
        for (x = 0; x < actor->width; x++) {
            func_0204b3dc(actor->actors[y * actor->width + x], enable);
        }
    }
}

// pokebw2's BmpOam_ActorGetDrawEnable
BOOL func_020257c4(BmpOamActor *actor) {
    return func_0204b3f4(actor->actors[0]);
}

// pokebw2's BmpOam_ActorBmpTrans
void func_020257d0(BmpOamActor *actor) {
    u8 *pixels = GFL_BitmapGetPixelData(actor->bitmap);
    void (*upload)(const void *src, u32 offset, u32 size);
    int x;
    u32 tilesHigh;
    u32 vramOffset;
    int y;
    // Where the cell's upper and lower rows of 8 pixels are in the bitmap
    int offset;
    int lowerOffset;
    u32 tilesWide;
    u32 size;
    // The size of a row of the last cell of a row, which may be narrower
    u32 lastSize;

    DC_FlushRange(pixels, GFL_BitmapCalcPixelDataSize(actor->bitmap));
    upload = actor->vramType == CLACT_VRAM_MAIN ? GX_LoadOBJ : GXS_LoadOBJ;
    tilesWide = GFL_BitmapGetWidth(actor->bitmap) / 8;
    tilesHigh = GFL_BitmapGetHeight(actor->bitmap) / 8;
    lastSize = GFL_BitmapGetWidth(actor->bitmap) % 32 / 8 * 32;
    if (lastSize == 0) {
        lastSize = CELL_ROW_SIZE;
    }

    offset = 0;
    lowerOffset = tilesWide * 32;
    for (y = 0; y < actor->height; y++) {
        for (x = 0; x < actor->width; x++) {
            if (x == actor->width - 1) {
                size = lastSize;
            } else {
                size = CELL_ROW_SIZE;
            }
            vramOffset = func_0204abbc(actor->chars[y * actor->width + x], actor->vramType);
            upload(pixels + offset, vramOffset, size);
            // Not when the bitmap ends halfway through the last row of cells
            if (y != actor->height - 1 || (tilesHigh & 1) == 0) {
                vramOffset += CELL_ROW_SIZE;
                upload(pixels + lowerOffset, vramOffset, size);
            }
            offset += size;
            lowerOffset += size;
        }
        // Skip the row of tiles that the lower rows came from
        offset += tilesWide * 32;
        lowerOffset += tilesWide * 32;
    }
}

// pokebw2's BmpOam_ActorGetPos
void func_020258e8(BmpOamActor *actor, s16 *x, s16 *y) {
    ClActorPos pos;

    func_0204b454(actor->actors[0], &pos, actor->surface);
    *x = pos.x;
    *y = pos.y;
}

// pokebw2's BmpOam_ActorSetPos
void func_0202590c(BmpOamActor *actor, s16 x, s16 y) {
    ClActorPos pos;
    int i, j;

    for (i = 0; i < actor->height; i++) {
        for (j = 0; j < actor->width; j++) {
            pos.x = x + j * 32;
            pos.y = y + i * 16;
            func_0204b404(actor->actors[i * actor->width + j], &pos, actor->surface);
        }
    }
}

// pokebw2's BmpOam_ActorSetObjMode
void func_0202597c(BmpOamActor *actor, u32 mode) {
    int i, j;

    for (i = 0; i < actor->height; i++) {
        for (j = 0; j < actor->width; j++) {
            func_0204b688(actor->actors[i * actor->width + j], mode);
        }
    }
}

// pokebw2's BmpOam_ActorSetPriority
void func_020259c4(BmpOamActor *actor, u8 priority) {
    int i, j;

    for (i = 0; i < actor->height; i++) {
        for (j = 0; j < actor->width; j++) {
            func_0204b800(actor->actors[i * actor->width + j], priority);
        }
    }
}

// pokebw2's BmpOam_ActorSetBgPriority
void func_02025a0c(BmpOamActor *actor, u32 bgPriority) {
    int i, j;

    for (i = 0; i < actor->height; i++) {
        for (j = 0; j < actor->width; j++) {
            func_0204b84c(actor->actors[i * actor->width + j], bgPriority);
        }
    }
}

// pokebw2's BmpOam_ActorSetPaletteOffset
void func_02025a54(BmpOamActor *actor, u32 offset) {
    int i, j;

    for (i = 0; i < actor->height; i++) {
        for (j = 0; j < actor->width; j++) {
            func_0204b6f4(actor->actors[i * actor->width + j], offset, 0);
        }
    }
}

// Loads the cells and animations for the OBJ character mapping of the actor's screen, once
// pokebw2's BmpOam_LoadCellAnim
static u32 func_02025a9c(BmpOamSys *sys, u32 vramType, ArcTool *arc) {
    u32 mapping;
    GXOBJVRamModeChar mode;

    if (vramType == CLACT_VRAM_MAIN) {
        mode = GX_GetOBJVRamModeChar();
    } else {
        mode = GXS_GetOBJVRamModeChar();
    }
    switch (mode) {
    case GX_OBJVRAMMODE_CHAR_1D_128K:
        mapping = CELL_MAPPING_1D_128K;
        break;
    case GX_OBJVRAMMODE_CHAR_1D_64K:
        mapping = CELL_MAPPING_1D_64K;
        break;
    case GX_OBJVRAMMODE_CHAR_1D_32K:
        mapping = CELL_MAPPING_1D_32K;
        break;
    default:
        mapping = CELL_MAPPING_1D_128K;
        break;
    }
    if (sys->cellAnims[mapping] != CELL_ANIM_NONE) {
        return mapping;
    }
    sys->cellAnims[mapping] =
        func_0204af28(arc, data_0209f87c[mapping].cellFileId, data_0209f87c[mapping].animFileId, sys->heapId);
    return mapping;
}

// pokebw2's BmpOam_FreeCellAnims
static void func_02025b18(BmpOamSys *sys) {
    int i;

    for (i = 0; i < CELL_MAPPING_COUNT; i++) {
        if (sys->cellAnims[i] != CELL_ANIM_NONE) {
            func_0204afd8(sys->cellAnims[i]);
            sys->cellAnims[i] = CELL_ANIM_NONE;
        }
    }
}
