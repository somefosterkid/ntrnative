#include "global.h"
#include "overlay_12_field_battle_dto.h"
#include "constants/heap.h"
#include "save_palpad.h"
#include "overlay_manager.h"
#include "bg_window.h"
#include "palette.h"
#include "gf_gfx_planes.h"

struct OverlayManager;
struct BgConfig;
struct FieldBattleDTO;
struct SavePalPad;
struct PaletteData;
struct Window;
void Heap_Free(void *ptr);
void PaletteData_Free(PaletteData *paletteData);
void PaletteData_FreeBuffers(PaletteData *paletteData, enum PaletteBufferId bufferID);
void *OverlayManager_GetData(OverlayManager *appMan);
void FreeBgTilemapBuffer(BgConfig *bgConfig, u8 bgLayer);
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
static inline G3DPipelineBuffers *G3DPipeline_Init(enum HeapID heapID, enum TextureVramSize texVramSize, enum PaletteVramSize plttVramSize, G3DPipelineSetupCallback cb) {
    return GF_3DVramMan_Create(heapID, VRAM_MANAGER_TYPE_LINKED_LIST, texVramSize, VRAM_MANAGER_TYPE_LINKED_LIST, plttVramSize, cb);
}
typedef struct LinkBattleCommState {
    FieldBattleDTO *dto;
    BgConfig *bgConfig;
    Window *window;
    PaletteData *paletteData;
    SavePalPad *palPad[4];
    u8 sendBuffer[0x1000];
    u8 recvCount;
    u8 handshakeStep;
    u16 syncTimer;
    void *waitDial;
} LinkBattleCommState;
void GfGfx_EngineATogglePlanes(u8 layerMask, u8 enable);
typedef void (*Callback)(void *);
void Main_SetVBlankIntrCB(Callback cb, void *data);
void sub_0200FBF4(enum GFScreen screen, u16 color);

void ov12_0223A088(OverlayManager *appMan);

void ov12_0223A088(OverlayManager *appMan) {
    LinkBattleCommState *linkBattleCommState = OverlayManager_GetData(appMan);

    Main_SetVBlankIntrCB(0L, 0L);
    sub_0200FBF4(SCREEN_MAIN, (((0) << 10) | ((0) << 5) | (0)));
    PaletteData_FreeBuffers(linkBattleCommState->paletteData, PLTTBUF_MAIN_BG);
    PaletteData_Free(linkBattleCommState->paletteData);
    WindowArray_Delete(linkBattleCommState->window, 1);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, 0);
    FreeBgTilemapBuffer(linkBattleCommState->bgConfig, GF_BG_LYR_MAIN_1);
    Heap_Free(linkBattleCommState->bgConfig);
    Heap_Free(linkBattleCommState);
}
