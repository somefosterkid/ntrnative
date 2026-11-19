#include "global.h"
#include "sprite_resources_header.h"
#include "overlay_70_gtsapplication_state.h"
#include "message_format.h"
#include "msgdata.h"
#include "msgdata/msg.naix"
#include "unk_0202C034.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "pokemon_summary_app.h"
#include "evolution_data.h"
#include "list_menu.h"
#include "screen_fade_internal.h"
#include "sav_chatot.h"
#include "overlay_73_wifi_battle_tower_app_state.h"
#include "sound.h"
#include "filesystem_files_def.h"

struct AffineSpriteListTemplate;

struct OverlayManager;

struct BgConfig;

struct ListMenuCursor;

struct G3DPipelineBuffers;

struct GTSApplicationState;

struct GTSBoxPokemonCriteria;

struct GTSPlayerData;

struct GraphicsModes;

struct ListMenu2D;

struct NARC;

struct SpriteResourcesHeader;

struct SysTask;

BOOL Heap_Create(enum HeapID parent, enum HeapID child, u32 size);

void Heap_Free(void *ptr);

void BlendPalette(const u16 *src, u16 *dest, u16 size, u8 fraction, u16 target);

typedef struct AffineSpriteListTemplate AffineSpriteListTemplate;

typedef struct GTSPlayerData GTSPlayerData;

void SetBothScreensModesAndDisable(const GraphicsModes *graphicsModes);

BgConfig *BgConfig_Alloc(enum HeapID heapID);

void *GfGfxLoader_GetPlttDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, NNSG2dPaletteData **outPaletteData, enum HeapID heapID);

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

void *OverlayManager_CreateAndGetData(OverlayManager *appMan, u32 size, enum HeapID heapID);

typedef struct ListMenuCursor ListMenuCursor;

typedef struct ListMenu2D ListMenu2D;

enum GTSApplicationLoopState {
    GTS_LOOP_STATE_WAIT_FOR_WIRELESS_DRIVER = 0,
    GTS_LOOP_STATE_INIT,
    GTS_LOOP_STATE_WAIT_FADE,
    GTS_LOOP_STATE_MAIN,
    GTS_LOOP_STATE_FINISH,
    GTS_LOOP_STATE_EXIT
};

typedef struct GTSBoxPokemonCriteria GTSBoxPokemonCriteria;

enum ScreenArgument {
    SCREEN_ARGUMENT_NONE = 0,
    SCREEN_ARGUMENT_RESUME_LISTING = 3,
    SCREEN_ARGUMENT_CHOOSE_OFFER_MON = 5,
    SCREEN_ARGUMENT_CHOOSE_LISTED_MON,
    SCREEN_ARGUMENT_DEPOSIT_POKEMON,
    SCREEN_ARGUMENT_TAKE_BACK_POKEMON,
    SCREEN_ARGUMENT_EXCHANGE_POKEMON,
    SCREEN_ARGUMENT_EXCHANGE_POKEMON_ALT,
    SCREEN_ARGUMENT_CHECK_SERVER,
    SCREEN_ARGUMENT_SAVE_AFTER_EVOLVE,
};

void GfGfx_DisableEngineAPlanes(void);

void GfGfx_DisableEngineBPlanes(void);

SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0);

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1);

static inline void inline_ov61_0222C3B0(UnkStruct_ov61_0222C3B0 *param0, NARC *param1, int param2, enum HeapID heapID) {
    NNSG2dPaletteData *v0;
    void *v1;

    MI_CpuClear8(param0, sizeof(UnkStruct_ov61_0222C3B0));

    v1 = GfGfxLoader_GetPlttDataFromOpenNarc(param1, param2, &v0, heapID);

    MI_CpuCopy16(&((u16 *)(v0->pRawData))[0 * 16], param0->unk_08, 4 * 0x20);
    MI_CpuCopy16(&((u16 *)(v0->pRawData))[0 * 16], param0->unk_88, 4 * 0x20);
    Heap_Free(v1);
    inline_ov61_0222C3B0_sub_1(param0);

    param0->unk_04 = 1;
    param0->unk_00 = SysTask_ExecuteOnVBlank(inline_ov61_0222C3B0_sub, param0, 20);
}

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0) {
    int v0, v1, v2, v3, v4, v5;
    v1 = 0;

    for (v0 = 0; v0 < 4 - 1; v0++) {
        v2 = v0 + 1;
        v3 = 0;
        v5 = 0;
        do {
            ((v1 < (((16 << 8) / 0x300 + 2) * (4 - 1))) ? (void)0 : GF_AssertFail());
            for (v4 = 1; v4 < 1 + 15; v4++) {
                BlendPalette(&param0->unk_08[v0][v4], &param0->unk_88[v1][v4], 1, v3 >> 8, param0->unk_08[v2][v4]);
            }
            v1++;
            if (v5 == 1) {
                break;
            }
            v3 += 0x300;
            if (v3 >= (16 << 8)) {
                v3 = (16 << 8);
                v5 = 1;
            }
        } while (1);
    }
    DC_FlushRange(param0->unk_88, (((16 << 8) / 0x300 + 2) * (4 - 1)) * 16 * sizeof(u16));
}

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1) {
    UnkStruct_ov61_0222C3B0 *v0 = param1;

    if (v0->unk_04 == 0) {
        return;
    }

    v0->unk_32B ^= 1;

    if (v0->unk_32B & 1) {
        return;
    }

    GX_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);
    GXS_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);

    if (v0->unk_32A == 0) {
        v0->unk_328++;
        if (v0->unk_328 >= (((16 << 8) / 0x300 + 2) * (4 - 1))) {
            v0->unk_328 = (((16 << 8) / 0x300 + 2) * (4 - 1)) - 2;
            v0->unk_32A ^= 1;
        }
    } else {
        v0->unk_328--;
        if (v0->unk_328 < 0) {
            v0->unk_328 = 1;
            v0->unk_32A ^= 1;
        }
    }
}

void HBlankInterruptDisable(void);

void SetKeyRepeatTimers(int rate, int delay);

BOOL Sound_SetSceneAndPlayBGM(u8 scene, u16 bgmID, int unused);

void sub_02034D8C(void);

void LoadDwcOverlay(void);

void Overlay_LoadHttpOverlay(void);

void GTSApplication_InitPlayerData(GTSApplicationState *appState, OverlayManager *appMan);

void GTSApplication_InitRenderingSystem(GTSApplicationState *appState);

extern struct {
    NNSFndHeapHandle heapHandle;
    GTSApplicationState *appState; /* pooled bss: ov70_02246944 + 4 */
} ov70_02246944;

void GTSApplication_InitAffineTemplate(AffineSpriteListTemplate *template, GTSApplicationState *appState, SpriteResourcesHeader *spriteResourceHeader, enum NNS_G2D_VRAM_TYPE vramType);

void GTSApplication_SetNextScreenWithArgument(GTSApplicationState *appState, int nextScreen, enum ScreenArgument screenArgument);

BOOL ov70_02238430(OverlayManager *appMan, int *loopState);

void TextFlags_SetCanTouchSpeedUpPrint(BOOL enable);

void LoadOVY38(void);

s32 sub_02039FD8(enum HeapID heapID);

void ov70_02238818(GTSApplicationState *appState, OverlayManager *appMan);

void ov70_02238E70(GTSApplicationState *appState);

void FontID_Alloc(int, enum HeapID heapID);

void ov70_02238880(void);

extern const GraphicsModes ov70_022451F8;

BOOL ov70_02238430(OverlayManager *appMan, int *loopState) {
    switch (*loopState) {
    case GTS_LOOP_STATE_WAIT_FOR_WIRELESS_DRIVER:
        Main_SetVBlankIntrCB(NULL, NULL);
        HBlankInterruptDisable();
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();

        GX_SetVisiblePlane(0);
        GXS_SetVisiblePlane(0);
        ov70_02238880();

        Heap_Create(HEAP_ID_3, HEAP_ID_61, 0x70000);
        LoadDwcOverlay();
        LoadOVY38();
        sub_02039FD8(HEAP_ID_61);

        GTSApplicationState *appState = OverlayManager_CreateAndGetData(appMan, sizeof(GTSApplicationState), HEAP_ID_61);
        memset(appState, 0, sizeof(GTSApplicationState));
        appState->bgConfig = BgConfig_Alloc(HEAP_ID_61);
        ov70_02246944.appState = appState;

        GraphicsModes graphicsModes = ov70_022451F8;

        SetBothScreensModesAndDisable(&graphicsModes);
        FontID_Alloc(4, HEAP_ID_61);
        appState->stringTemplate = MessageFormat_New_Custom(11, 64, HEAP_ID_61);

        appState->gtsMessageLoader = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, msg_0775, HEAP_ID_61);
        appState->connProgressMessageLoader = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, msg_0778, HEAP_ID_61);
        appState->disconnStatusMessageLoader = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, msg_0800, HEAP_ID_61);
        appState->speciesMessageLoader = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, msg_0237, HEAP_ID_61);
        appState->countryMessageLoader = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, msg_0798, HEAP_ID_61);

        SetKeyRepeatTimers(4, 8);

        ov70_02238818(appState, appMan);
        ov70_02238E70(appState);

        Sound_SetSceneAndPlayBGM((enum SoundScene)11, 0x47D, 1);

        appState->dwcHeapPointer = Heap_Alloc(HEAP_ID_61, 0x20000 + 32);
        appState->dwcHeapHandle = NNS_FndCreateExpHeapEx((void *)(((u32)appState->dwcHeapPointer + 31) / 32 * 32), 0x20000, 0);
        *loopState = GTS_LOOP_STATE_INIT;
        break;
    case GTS_LOOP_STATE_INIT:
        sub_02034D8C();
        TextFlags_SetCanTouchSpeedUpPrint(1);
        (*loopState) = GTS_LOOP_STATE_WAIT_FOR_WIRELESS_DRIVER;
        return 1;
    }

    return 0;
}
