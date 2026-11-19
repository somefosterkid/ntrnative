#include "global.h"
#include "constants/heap.h"
#include "screen_fade_internal.h"

extern void *ov112_021FF244;

extern u8 ov112_021FF2CC;

void ov112_021F0D48(void *arg0);

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
void GfGfx_DisableEngineAPlanes(void);
void GfGfx_DisableEngineBPlanes(void);
void Main_SetVBlankIntrCB(Callback cb, void *data);
void HBlankInterruptDisable(void);
void ov112_021F0DC0(void);

void ov112_021F0D48(void *arg0) {
    void *v2;
    s32 v1;
    u8 *v0;
    s32 v3;
    u16 v14;
    s32 v6;
    u32 v9;
    u8 v4;
    u32 v5;
    u8 v7;
    u8 v8;
    u8 v13;
    void **v11;
    void *v10;
    void *v12;

    v12 = arg0;
    v0 = &ov112_021FF2CC;
    v14 = 0;
    v2 = v12;
    v11 = &ov112_021FF244;
    v1 = 0;
    v2 = v12 + 0x40;
    do {
        *(u16 *)v12 = v14;
        v5 = *v0;
        v13 = 0;
        v9 = 0;
        *(u8 *)((u8 *)v12 + 2) = v5;
        v10 = *v11;
        v6 = v5 >> 1;
        if (v6 != 0) {
            do {
                v9 += 1;
                v8 = v13 + *(u8 *)v10;
                v7 = *(u8 *)((u8 *)v10 + 1);
                v10 += 2;
                v13 = v8 + v7;
            } while (v9 < v6);
        }
        *(u8 *)((u8 *)v12 + 3) = v13;
        MI_CpuCopy8(*v11, v2 + v14, (u32) * (u8 *)((u8 *)v12 + 2));
        v4 = *(u8 *)((u8 *)v12 + 2);
        v12 += 4;
        v11 += 1;
        v14 += v4;
        v0 += 1;
        v3 = v1 + 1;
        v1 = v3;
    } while (v3 < 0x10);
}

static inline G3DPipelineBuffers *G3DPipeline_Init(enum HeapID heapID, enum TextureVramSize texVramSize, enum PaletteVramSize plttVramSize, G3DPipelineSetupCallback cb) {
    return GF_3DVramMan_Create(heapID, VRAM_MANAGER_TYPE_LINKED_LIST, texVramSize, VRAM_MANAGER_TYPE_LINKED_LIST, plttVramSize, cb);
}

void ov112_021F0DC0(void) {
    Main_SetVBlankIntrCB(0L, 0L);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();

    GX_SetVisiblePlane(0);
    GXS_SetVisiblePlane(0);
}
