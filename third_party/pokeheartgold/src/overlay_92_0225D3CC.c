#include "global.h"

#include "constants/heap.h"

#include "struct_defs/resdat_narc_structs.h"

#include "field/overlay_01_021FB878.h"

#include "bg_window.h"
#include "camera.h"
#include "easy_chat.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "gf_3d_loader.h"
#include "gf_3d_vramman.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "palette.h"
#include "pm_string.h"
#include "save.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "touch_hitbox_controller.h"
#include "touchscreen.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "yes_no_prompt.h"
#include "gf_gfx_planes.h"
#include "overlay_45_wifi_plaza_system.h"
#include "overlay_90_unk_struct_ov114_0225c76c.h"
#include "overlay_90_unk_struct_ov114_0225c9a8.h"
#include "overlay_90_unk_struct_ov114_0225e0f8.h"
#include "overlay_90_unk_struct_ov114_0225cef0.h"
#include "overlay_90_unk_struct_ov114_02260284.h"
#include "overlay_90_unk_struct_ov114_0225_d084.h"
#include "overlay_90_unk_struct_ov114_0225d338.h"
#include "overlay_90_unk_struct_ov114_0225cfcc.h"
#include "overlay_90_unk_struct_ov114_0225ccd0.h"
#include "overlay_90_unk_struct_ov114_0225cdb4.h"
#include "overlay_90_unk_struct_ov114_0225cbf0.h"
#include "overlay_90_unk_struct_ov114_0225f270.h"
#include "overlay_90_unk_struct_ov114_0225f578.h"
#include "overlay_90_unk_struct_ov114_0225dac0.h"
#include "overlay_90_unk_struct_ov114_0225dffc.h"
#include "overlay_91_unk_struct_ov66_02230dbc.h"
#include "overlay_90_unk_struct_ov114_0225_e854_t.h"

typedef struct BufferManager BufferManager;

typedef struct YesNoButtonAnimData YesNoButtonAnimData;

typedef struct YesNoButtonSprite YesNoButtonSprite;

typedef struct UnkStruct_ov114_0225D678_t UnkStruct_ov114_0225D678;

typedef struct UnkStruct_ov114_0225E854_t UnkStruct_ov114_0225E854;

typedef struct UnkStruct_ov114_02025CAD4_t UnkStruct_ov114_0225CAD4;

struct BufferManager {
    void *buffers[2];
    u8 writeBuffer;
    u8 mode;
    u8 padding[2];
};

struct YesNoButtonAnimData {
    const fx32 *frameThresholds;
    u32 frameCount;
    fx32 frameProgress;
};

struct YesNoButtonSprite {
    BgConfig *bgConfig;
    enum GFBgLayer bgLayer;
    void *nscrBuffers[4];
    NNSG2dScreenData *screenData[4];
    YesNoButtonAnimData animData;
    u8 tilemapLeft;
    u8 tilemapTop;
    u8 animFrame;
};

typedef struct {
    Window unk_00;
    u32 unk_10;
    String *unk_14;
    UnkStruct_ov114_0225CBF0 unk_18;
    s16 unk_2C;
    s16 unk_2E;
} UnkStruct_ov114_0225E1A4;

struct UnkStruct_ov114_0225D678_t {
    u8 unk_00;
    u8 unk_01;
    u16 heapID;
    u16 unk_04;
    u8 unk_06;
    u8 unk_07;
    UnkStruct_ov114_0225C76C unk_08;
    UnkStruct_ov114_0225E0F8 unk_18;
    UnkStruct_ov114_0225CEF0 unk_30;
    UnkStruct_ov114_0225D084 unk_3C;
    UnkStruct_ov114_0225D338 unk_4C;
    UnkStruct_ov114_0225CFCC unk_84;
    UnkStruct_ov114_0225CCD0 unk_214;
    UnkStruct_ov114_0225CDB4 *unk_22C;
    UnkStruct_ov114_0225E1A4 unk_230;
    void *unk_260;
    NNSG2dScreenData *unk_264;
    UnkStruct_ov114_0225DAC0 unk_268[4];
    UnkStruct_ov114_0225DFFC unk_348;
    SysTask *unk_650;
    SysTask *unk_654;
};

typedef struct {
    UnkStruct_ov66_02230DBC unk_00;
    SaveData *saveData;
    BOOL unk_38;
    u32 unk_3C;
} UnkStruct_ov66_02231134;

typedef struct {
    u8 unk_00;
    u8 padding_01[3];
    u32 unk_04;
} UnkStruct_ov116_0226048C;

typedef struct {
    u32 unk_00;
    u8 padding_04[4];
} UnkStruct_ov116_02260494;

typedef struct {
    UnkStruct_ov116_02260494 unk_00[4];
} UnkStruct_ov116_02260498;

typedef struct {
    u8 unk_00;
    u8 padding_01[3];
} UnkStruct_ov116_022649E4_sub1;

typedef struct {
    NARC *unk_00;
    NARC *unk_04;
    SpriteSystem *unk_08;
    SpriteManager *unk_0C;
    BgConfig *unk_10;
    PaletteData *unk_14;
    GF3DVramMan *unk_18;
    Camera *camera;
    u8 padding_20[4];
    NNSFndAllocator unk_24;
    UnkStruct_ov116_022649E4_sub1 unk_2C;
} UnkStruct_ov116_022649E4;

typedef struct {
    u8 padding_00[4];
    int unk_04;
    s16 unk_08;
    u8 padding_0A;
    UnkStruct_ov116_022649E4 *unk_0C;
} UnkStruct_ov116_02262A44;

typedef struct {
    u32 unk_00;
} UnkStruct_ov116_02260490;

typedef struct {
    u32 unk_00;
    u32 unk_04;
    u8 padding_08[4];
} UnkStruct_ov116_02262A8C_sub1;

typedef struct {
    fx32 unk_00;
    fx32 unk_04;
    fx32 unk_08;
    fx32 unk_0C;
    int unk_10;
    int unk_14;
} UnkStruct_ov116_0226469C;

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    s16 unk_1C;
    s16 unk_1E;
    s16 unk_20[4];
    int unk_28[4];
    int unk_38;
    s16 unk_3C;
    s16 unk_3E;
    s16 unk_40;
    s16 unk_42;
    UnkStruct_ov116_0226469C unk_44;
    UnkStruct_ov116_0226469C unk_5C;
    UnkStruct_ov116_0226469C unk_74;
    UnkStruct_ov116_0226469C unk_8C;
    BOOL unk_A4;
    BOOL unk_A8;
    BOOL unk_AC;
    BOOL unk_B0;
    BOOL unk_B4;
    s16 unk_B8;
    s16 unk_BA;
} UnkStruct_ov116_02262A8C_sub2;

typedef union {
    struct {
        f32 val1_00;
        f32 val1_04;
        f32 val1_08;
        f32 val1_0C;
    };
    f32 val2[4];
} UnkUnion_ov116_02266FEC;

typedef union {
    struct {
        f32 val1_00;
        f32 val1_04;
        f32 val1_08;
        f32 val1_0C;
        f32 val1_10;
        f32 val1_14;
        f32 val1_18;
        f32 val1_1C;
        f32 val1_20;
        f32 val1_24;
        f32 val1_28;
        f32 val1_2C;
        f32 val1_30;
        f32 val1_34;
        f32 val1_38;
        f32 val1_3C;
    };
    f32 val2[4][4];
    f32 val3[16];
} UnkUnion_ov116_022671EC;

typedef struct {
    Field3dObject unk_00;
    Field3dModel unk_78;
    Field3DModelAnimation unk_88[10];
    UnkUnion_ov116_022671EC unk_150;
    UnkUnion_ov116_02266FEC unk_190;
    UnkUnion_ov116_02266FEC unk_1A0;
    MtxFx43 unk_1B0;
    VecFx32 unk_1E0;
    BOOL unk_1EC;
    BOOL unk_1F0;
    int unk_1F4;
    int unk_1F8;
    f32 unk_1FC;
    f32 unk_200;
    f32 unk_204;
    f32 unk_208;
} UnkStruct_ov116_0226501C;

typedef struct {
    int unk_00;
    int unk_04;
    u8 padding_08[4];
    ManagedSprite *unk_0C;
    UnkStruct_ov116_0226469C unk_10;
    int unk_28;
    int *unk_2C;
    BOOL unk_30;
    UnkStruct_ov116_0226501C *unk_34;
    UnkStruct_ov116_0226501C *unk_38;
    UnkStruct_ov116_022649E4 *unk_3C;
    int unk_40;
} UnkStruct_ov116_02262D08;

typedef struct {
    BOOL unk_00;
    int unk_04;
    ManagedSprite *unk_08[3];
    UnkStruct_ov116_0226469C unk_14[3][2];
    UnkStruct_ov116_022649E4 *unk_A4;
} UnkStruct_ov116_022647BC;

typedef struct {
    int unk_00;
    BOOL unk_04;
    void *unk_08[8];
    int unk_28[10];
    BOOL unk_50[10];
} UnkStruct_ov116_02265470;

typedef struct {
    int unk_00;
    int *unk_04;
    UnkStruct_ov116_02265470 unk_08[5];
    SysTask *unk_80;
    UnkStruct_ov116_022649E4 *unk_84;
} UnkStruct_ov116_0226534C;

typedef struct {
    ManagedSprite *unk_00;
    int unk_04;
    s16 unk_08;
    u8 padding_0A[6];
    int unk_10;
    s16 unk_14;
    s16 unk_16;
    int unk_18;
    int unk_1C;
    UnkStruct_ov116_0226469C unk_20;
    UnkStruct_ov116_0226469C unk_38;
} UnkStruct_ov116_02265D84;

typedef struct {
    UnkStruct_ov116_02265D84 unk_00[2];
    UnkStruct_ov116_02265D84 unk_A0[2];
    int unk_140;
    int unk_144;
    int unk_148;
    int unk_14C;
} UnkStruct_ov116_02265AA4;

typedef struct {
    UnkStruct_ov116_02265AA4 unk_00;
    u8 padding_04[4];
    SysTask *unk_08;
    int *unk_0C;
    int *unk_10;
    int *unk_14;
    int unk_18;
    int unk_1C;
    u8 padding_20[4];
    int unk_24;
    BOOL unk_28;
    BOOL unk_2C;
    UnkStruct_ov116_022649E4 *unk_30;
    UnkStruct_ov116_0226469C unk_34[5];
} UnkStruct_ov116_022660A8;

typedef struct {
    u32 unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    u8 padding_10[4];
    UnkStruct_ov116_022649E4 *unk_14;
    u8 padding_18[4];
    ManagedSprite *unk_1C[3];
    ManagedSprite *unk_20[3];
    ManagedSprite *unk_24[3];
    ManagedSprite *unk_28[36];
    ManagedSprite *unk_B8[4];
    ManagedSprite *unk_C8[2];
    ManagedSprite *unk_D0[2];
    ManagedSprite *unk_D8;
    u16 unk_DC[4];
    u8 padding_E4[12];
    int unk_F0;
    int unk_F4;
    int unk_F8;
    UnkStruct_ov116_0226501C unk_FC;
    UnkStruct_ov116_0226501C unk_308[3];
    UnkStruct_ov116_0226501C unk_92C;
    UnkStruct_ov116_0226501C unk_B38[8];
    UnkStruct_ov116_0226501C unk_1B98;
    UnkStruct_ov116_0226501C unk_1DA4;
    VecFx32 unk_1FB0;
    UnkStruct_ov116_02262A8C_sub1 unk_1FBC;
    Window unk_1FC8;
    Window unk_1FD8[4];
    UnkStruct_ov116_022660A8 unk_2018;
    UnkStruct_ov116_0226534C unk_20C4;
    UnkStruct_ov116_022647BC unk_214C[8];
    UnkStruct_ov116_02262D08 unk_268C[4];
    UnkStruct_ov116_02262A8C_sub2 unk_279C;
    UnkStruct_ov116_02260490 unk_2858[4];
    u8 padding_2868[4];
    u32 unk_286C;
    int unk_2870;
    BOOL unk_2874;
} UnkStruct_ov116_02262A8C;

typedef struct {
    UnkStruct_ov116_02262A44 *unk_00;
    UnkStruct_ov116_02262A8C *unk_04;
    BOOL unk_08;
    BOOL unk_0C;
    BOOL unk_10;
    UnkStruct_ov116_0226048C unk_14;
    UnkStruct_ov116_02260494 unk_1C;
    UnkStruct_ov116_02260498 unk_24;
    u32 unk_44;
    UnkStruct_ov116_022649E4 unk_48;
    u32 unk_78;
    UnkStruct_ov114_0225CAD4 *unk_7C;
    UnkStruct_ov66_02231134 *unk_80;
    UnkStruct_ov114_0225C76C unk_84;
    UnkStruct_ov114_0225D678 *unk_94;
    UnkStruct_ov114_0225C9A8 unk_98;
    UnkStruct_ov114_0225E854 *unk_B0;
    int unk_B4[4];
    BOOL unk_C4;
} UnkStruct_ov116_0226139C;

void ov92_0225D594(BgConfig *param0);

void ov92_0225D7CC(UnkStruct_ov116_0226139C *param0);

void ov92_0225DD88(UnkStruct_ov116_0226139C *param0);

extern u16 ov92_022638B0;

void ov92_0225D3CC(UnkStruct_ov116_0226139C *param0);

void ov92_0225D49C(UnkStruct_ov116_0226139C *arg0);

void ov92_0225D3CC(UnkStruct_ov116_0226139C *param0) {
    param0->unk_48.unk_00 = NARC_New(NARC_a_1_9_3, HEAP_ID_113);
    param0->unk_48.unk_04 = NARC_New(NARC_a_1_8_9, HEAP_ID_113);
    param0->unk_48.unk_10 = BgConfig_Alloc(HEAP_ID_113);
    param0->unk_48.unk_14 = PaletteData_Init(HEAP_ID_113);
    param0->unk_48.unk_18 = (GF3DVramMan *)GF_3DVramMan_Create(HEAP_ID_113, 0, 1, 0, 4, NULL);
    param0->unk_48.camera = Camera_New(HEAP_ID_113);
    ov92_0225DD88(param0);
    PaletteData_SetAutoTransparent(param0->unk_48.unk_14, 1);
    PaletteData_AllocBuffers(param0->unk_48.unk_14, PLTTBUF_MAIN_BG, 0x200U, HEAP_ID_113);
    PaletteData_AllocBuffers(param0->unk_48.unk_14, PLTTBUF_SUB_BG, 0x200U, HEAP_ID_113);
    PaletteData_AllocBuffers(param0->unk_48.unk_14, PLTTBUF_MAIN_OBJ, 0x200U, HEAP_ID_113);
    PaletteData_AllocBuffers(param0->unk_48.unk_14, PLTTBUF_SUB_OBJ, 0x200U, HEAP_ID_113);
    ov92_0225D594(param0->unk_48.unk_10);
    ov92_0225D7CC(param0);
    reg_G3X_DISP3DCNT = 8 | (reg_G3X_DISP3DCNT & 0xFFFFCFFF);
    reg_G3X_DISP3DCNT = 0x20 | (reg_G3X_DISP3DCNT & 0xFFFFCFFF);
    G3X_SetEdgeColorTable(&ov92_022638B0);
    HeapExp_FndInitAllocator(&param0->unk_48.unk_24, HEAP_ID_113, 0x20);
}

void ov92_0225D49C(UnkStruct_ov116_0226139C *arg0) {
    GfGfx_EngineATogglePlanes(1U, 0U);
    GfGfx_EngineATogglePlanes(2U, 0U);
    GfGfx_EngineATogglePlanes(4U, 0U);
    GfGfx_EngineATogglePlanes(8U, 0U);
    GfGfx_EngineBTogglePlanes(1U, 0U);
    GfGfx_EngineBTogglePlanes(2U, 0U);
    GfGfx_EngineBTogglePlanes(4U, 0U);
    GfGfx_EngineBTogglePlanes(8U, 0U);
    FreeBgTilemapBuffer(arg0->unk_48.unk_10, 0U);
    FreeBgTilemapBuffer(arg0->unk_48.unk_10, 1U);
    FreeBgTilemapBuffer(arg0->unk_48.unk_10, 2U);
    FreeBgTilemapBuffer(arg0->unk_48.unk_10, 3U);
    FreeBgTilemapBuffer(arg0->unk_48.unk_10, 4U);
    FreeBgTilemapBuffer(arg0->unk_48.unk_10, 5U);
    FreeBgTilemapBuffer(arg0->unk_48.unk_10, 6U);
    FreeBgTilemapBuffer(arg0->unk_48.unk_10, 7U);
    Heap_Free(arg0->unk_48.unk_10);
    PaletteData_FreeBuffers(arg0->unk_48.unk_14, PLTTBUF_MAIN_BG);
    PaletteData_FreeBuffers(arg0->unk_48.unk_14, PLTTBUF_SUB_BG);
    PaletteData_FreeBuffers(arg0->unk_48.unk_14, PLTTBUF_MAIN_OBJ);
    PaletteData_FreeBuffers(arg0->unk_48.unk_14, PLTTBUF_SUB_OBJ);
    PaletteData_Free(arg0->unk_48.unk_14);
    NARC_Delete(arg0->unk_48.unk_00);
    NARC_Delete(arg0->unk_48.unk_04);
    GF_3DVramMan_Delete(arg0->unk_48.unk_18);
    Camera_Delete(arg0->unk_48.camera);
    SpriteSystem_FreeResourcesAndManager(arg0->unk_48.unk_08, arg0->unk_48.unk_0C);
    SpriteSystem_Free(arg0->unk_48.unk_08);
    gSystem.screensFlipped = 0;
    GfGfx_SwapDisplay();
    reg_G3X_DISP3DCNT &= 0xCFF7;
}
