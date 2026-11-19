#include "global.h"
#include "constants/heap.h"
#include "gf_gfx_planes.h"
#include "bg_window.h"
#include "touchscreen.h"
#include "unk_02020A0C.h"
#include "unk_02019BA4.h"

struct BgConfig;

struct BgTemplate;

struct DpadMenuBox;

struct GridCallbacks;

struct NumberEntryRodata;

struct WindowTemplate;

void InitBgFromTemplate(BgConfig *bgConfig, u8 bgId, const BgTemplate *template, u8 bgType);

void BG_ClearCharDataRange(u8 bgId, u32 size, u32 offset, enum HeapID heapID);

void BgClearTilemapBufferAndCommit(BgConfig *bgConfig, u8 bgId);

void ov29_0225DC44(void *data, int newTarget, int prevTarget);

void ov29_0225DC48(void *data, int newTarget, int prevTarget);

typedef struct NumberEntryRodata {
    u8 xOffsets1[1];
    TouchscreenHitbox hitboxes1[2];
    TouchscreenHitbox hitboxes2[3];
    TouchscreenHitbox hitboxes3[4];
    GridCallbacks gridCallbacks;
    TouchscreenHitbox hitboxes4[5];
    TouchscreenHitbox hitboxes5[6];
    WindowTemplate windowTemplates[3];
    TouchscreenHitbox hitboxes6[7];
    const TouchscreenHitbox *hitboxes[7];
    BgTemplate bgTemplate4;
    const u8 *xOffsets[7];
    BgTemplate bgTemplate5;
    u8 spriteSpecs[11][4];
    TouchscreenHitbox padHitboxes[14];
    DpadMenuBox padDpadBoxes[13];
    u8 xOffsets2[4];
    u8 xOffsets3[4];
    u8 xOffsets4[4];
    u8 xOffsets5[8];
    u8 xOffsets6[8];
} NumberEntryRodata;

extern const NumberEntryRodata sRodata;

void ov29_0225D648(BgConfig *bgConfig);

extern const BgTemplate ov29_0225E1C8;

extern const BgTemplate ov29_0225E200;

void ov29_0225D648(BgConfig *bgConfig) {
    GXS_SetGraphicsMode(GX_BGMODE_0);
    {
        BgTemplate template = ov29_0225E1C8;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &template, GF_BG_TYPE_TEXT);
        BG_ClearCharDataRange(GF_BG_LYR_SUB_0, 0x20, 0, HEAP_ID_8);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);
    }
    {
        BgTemplate template = ov29_0225E200;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &template, GF_BG_TYPE_TEXT);
    }
}
