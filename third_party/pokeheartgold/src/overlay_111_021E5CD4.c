#include "global.h"
#include "error_handling.h"
#include "constants/heap.h"
#include "screen_fade_internal.h"
#include "overlay_111.h"

typedef struct G3DPipelineBuffers {
    enum HeapID heapID;
    void *paletteBuffer;
    void *textureBuffer;
} G3DPipelineBuffers;
enum VramManagerType {
    VRAM_MANAGER_TYPE_LINKED_LIST = 0,
    VRAM_MANAGER_TYPE_FRAME,
};
enum TextureVramSize {
    TEXTURE_VRAM_SIZE_128K = 1,
    TEXTURE_VRAM_SIZE_256K,
    TEXTURE_VRAM_SIZE_384K,
    TEXTURE_VRAM_SIZE_512K,
};
enum PaletteVramSize {
    PALETTE_VRAM_SIZE_16K = 1,
    PALETTE_VRAM_SIZE_32K,
    PALETTE_VRAM_SIZE_48K,
    PALETTE_VRAM_SIZE_64K,
    PALETTE_VRAM_SIZE_80K,
    PALETTE_VRAM_SIZE_96K,
};
typedef void (*G3DPipelineSetupCallback)(void);
G3DPipelineBuffers *GF_3DVramMan_Create(enum HeapID heapID, enum VramManagerType texVramManagerType, enum TextureVramSize texVramSize, enum VramManagerType plttVramManagerType, enum PaletteVramSize plttVramSize, G3DPipelineSetupCallback cb);

void ov111_021E5D08(BugContestSwapMon *arg0, s32 arg1);

static inline G3DPipelineBuffers *G3DPipeline_Init(enum HeapID heapID, enum TextureVramSize texVramSize, enum PaletteVramSize plttVramSize, G3DPipelineSetupCallback cb) {
    return GF_3DVramMan_Create(heapID, VRAM_MANAGER_TYPE_LINKED_LIST, texVramSize, VRAM_MANAGER_TYPE_LINKED_LIST, plttVramSize, cb);
}
void GfGfx_DisableEngineAPlanes(void);
void GfGfx_DisableEngineBPlanes(void);
void Main_SetVBlankIntrCB(Callback cb, void *data);
void HBlankInterruptDisable(void);

void ov111_021E5CD4(void);

void ov111_021E5CD4(void) {
    Main_SetVBlankIntrCB(0L, 0L);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();

    GX_SetVisiblePlane(0);
    GXS_SetVisiblePlane(0);
}

void ov111_021E5D08(BugContestSwapMon *arg0, s32 arg1) {
    BugContestSwapMonArgs *v0;
    BugContestSwapMonArgs *v1;

    switch (arg1) {
    case 1:
        v0 = arg0->args;
        v0->unk10 = v0->newlyCaughtMon;
        return;
    case 2:
        v1 = arg0->args;
        v1->unk10 = v1->currentMon;
        return;
    default:
        GF_AssertFail();
        return;
    }
}
