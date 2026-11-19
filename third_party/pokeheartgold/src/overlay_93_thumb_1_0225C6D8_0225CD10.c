#include "global.h"

#include "bg_window.h"
#include "camera.h"
#include "palette.h"
#include "pm_string.h"
#include "render_text.h"
#include "save.h"
#include "sprite_system.h"
#include "sys_task_api.h"
#include "unk_020210A0.h"
#include "vram_transfer_manager.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "gf_gfx_planes.h"

struct OverlayManager;

struct UnkStruct_ov117_02261280_t;

void ov90_02258C38(UnkStruct_ov114_0225CAD4 *param0);

void ov93_0225E860(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_022626B0 *param1);

void ov93_022602E4(UnkStruct_ov117_02261280_t *param0);

void ov93_02260608(UnkStruct_ov117_02261280_t *param0);

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

static inline GF3DVramMan *G3DPipeline_Init(enum HeapID heapID, enum TextureVramSize texVramSize, enum PaletteVramSize plttVramSize, G3DPipelineSetupCallback cb) {
    return GF_3DVramMan_Create(heapID, VRAM_MANAGER_TYPE_LINKED_LIST, texVramSize, VRAM_MANAGER_TYPE_LINKED_LIST, plttVramSize, cb);
}

void sub_0203A914(void);

void sub_020135AC(UnkStruct_02012744 *param0);

void ov93_0225CFB8(GF3DVramMan *param0);

void ov93_0225D6E0(UnkStruct_ov117_02261280_t *param0);

void ov93_0225D9E8(UnkStruct_ov117_02261280_t *param0);

void ov93_0225DAF8(UnkStruct_ov117_02261280_t *param0);

void ov93_0225DBC4(UnkStruct_ov117_02261280_t *param0);

void ov93_0225DD28(UnkStruct_ov117_02261280_t *param0);

void ov93_0225DED0(UnkStruct_ov117_02261280_t *param0);

void ov93_0225D064(UnkStruct_ov117_02261280_t *param0);

int ov93_0225CD10(OverlayManager *appMan, int *param1);

int ov93_0225CD10(OverlayManager *appMan, int *param1) {
    UnkStruct_ov117_02261280_t *v0 = OverlayManager_GetData(appMan);
    int v1;

    v0->unk_00->unk_10.unk_14 = v0->unk_2FD0;

    ov90_02258C38(v0->unk_1C);
    ov93_022602E4(v0);
    ov93_02260608(v0);

    Main_SetVBlankIntrCB(0L, 0L);

    ov93_0225D6E0(v0);
    ov93_0225D9E8(v0);
    ov93_0225DAF8(v0);
    ov93_0225DBC4(v0);
    ov93_0225DD28(v0);
    ov93_0225DED0(v0);
    ov93_0225E860(v0, v0->unk_D4);

    for (v1 = 0; v1 < 5; v1++) {
        RemoveWindow(&v0->unk_30[v1]);
    }

    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, 0);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, 0);
    FreeBgTilemapBuffer(v0->unk_2C, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(v0->unk_2C, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(v0->unk_2C, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(v0->unk_2C, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(v0->unk_2C, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(v0->unk_2C, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(v0->unk_2C, GF_BG_LYR_SUB_3);
    sub_020135AC(v0->unk_90);
    SpriteSystem_FreeResourcesAndManager(v0->unk_24, v0->unk_28);
    SpriteSystem_Free(v0->unk_24);
    GF_DestroyVramTransferManager();
    PaletteData_FreeBuffers(v0->unk_8C, PLTTBUF_MAIN_BG);
    PaletteData_FreeBuffers(v0->unk_8C, PLTTBUF_SUB_BG);
    PaletteData_FreeBuffers(v0->unk_8C, PLTTBUF_MAIN_OBJ);
    PaletteData_FreeBuffers(v0->unk_8C, PLTTBUF_SUB_OBJ);
    PaletteData_Free(v0->unk_8C);
    String_Delete(v0->unk_88);
    MessageFormat_Delete(v0->unk_84);
    DestroyMsgData(v0->unk_80);
    Heap_Free(v0->unk_2C);
    ov93_0225D064(v0);
    SysTask_Destroy(v0->unk_94);

    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);

    ov93_0225CFB8(v0->unk_98);
    DisableTouchPad();
    OverlayManager_FreeData(appMan);
    TextFlags_SetCanABSpeedUpPrint(0);
    TextFlags_SetAutoScrollParam(0);
    TextFlags_SetCanTouchSpeedUpPrint(0);
    sub_0203A914();

    return 1;
}

void ov93_0225EA50(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_022626B0 *param1);

void ov93_0225CEA0(void *param0);

void ov93_0225CEA0(void *param0) {
    UnkStruct_ov117_02261280_t *v0 = param0;

    ov93_0225EA50(v0, v0->unk_D4);

    GF_RunVramTransferTasks();
    SpriteSystem_TransferOam();
    PaletteData_PushTransparentBuffers(v0->unk_8C);

    if (v0->unk_1428.unk_2C == 1) {
        ToggleBgLayer(GF_BG_LYR_SUB_3, 1);
        v0->unk_1428.unk_2C = 0;
    } else if (v0->unk_1428.unk_2D == 1) {
        ToggleBgLayer(GF_BG_LYR_SUB_3, 0);
        v0->unk_1428.unk_2D = 0;
    }

    DoScheduledBgGpuUpdates(v0->unk_2C);
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}
