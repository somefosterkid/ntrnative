#include "global.h"

#include "filesystem.h"
#include "font.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_89.h"
#include "overlay_manager.h"
#include "palette.h"
#include "pm_string.h"
#include "render_text.h"
#include "sprite_system.h"
#include "sys_task_api.h"
#include "system.h"
#include "unk_02013534.h"
#include "unk_020210A0.h"
#include "unk_0203A3B0.h"
#include "vram_transfer_manager.h"
#include "yes_no_prompt.h"
#include "field/overlay_01_021FB878.h"
#include "bg_window.h"
#include "camera.h"
#include "obj_char_transfer.h"
#include "save.h"
#include "sprite.h"
#include "sys_task.h"
#include "unk_02026E30.h"

s32 ov89_02259230(void *);

s32 ov89_022596DC(void *);

s32 ov89_022598A8(void *);

s32 ov89_02259BA0(void *);

s32 ov89_02259C00(void *);

s32 ov89_02259D50(void *);

SysTask *ov89_02259E10(void *);

s32 ov89_0225A47C(void *);

struct FontOAM;
struct G3DPipelineBuffers;
struct MessageLoader;
struct StringTemplate;
struct UnkStruct_02012744_t;
struct UnkStruct_ov113_0225DBCC_t;
struct UnkStruct_ov113_0225E6B8_t;
struct UnkStruct_ov66_0222DFF8_t;
struct YesNoTouchMenu;
typedef struct UnkStruct_ov113_0225DBCC_t UnkStruct_ov113_0225DBCC;
typedef struct {
    BOOL unk_00;
    s32 unk_04;
} UnkStruct_ov113_022607EC;
typedef struct {
    u16 unk_00;
    u16 unk_02;
    u32 unk_04;
    u8 unk_08;
    u8 unk_09;
    u8 unk_0A;
    u8 unk_0B;
} UnkStruct_ov113_02260818;
typedef struct FontOAM FontOAM;
typedef struct UnkStruct_02012744_t UnkStruct_02012744;
typedef struct UnkStruct_ov66_0222DFF8_t WiFiPlazaSystem;
typedef struct {
    WiFiPlazaSystem *unk_00;
    u8 unk_04;
    u8 padding_05[3];
} UnkStruct_ov66_0223127C;
typedef struct UnkStruct_ov113_0225E6B8_t UnkStruct_ov113_0225E6B8;
typedef struct {
    fx32 unk_00;
    s16 unk_04;
    u8 unk_06;
    u8 unk_07;
} UnkStruct_ov113_0225FE30;
typedef struct {
    VecFx32 unk_00;
    u8 unk_0C;
    u8 unk_0D;
    u8 unk_0E;
    u8 unk_0F;
} UnkStruct_ov113_0225FEE0;
typedef struct {
    CameraAngle cameraAngle;
    s32 unk_08;
    s16 unk_0C;
    u8 unk_0E;
    u8 unk_0F;
} UnkStruct_ov113_0225FF8C;
typedef struct {
    CameraAngle cameraAngle;
    s32 unk_08;
    s16 unk_0C;
    u8 unk_0E;
    u8 unk_0F;
} UnkStruct_ov113_02260064;
typedef struct {
    CameraAngle cameraAngle;
    s32 unk_08;
    s16 unk_0C;
    u8 unk_0E;
    u8 unk_0F;
} UnkStruct_ov113_0226013C;
typedef struct {
    CameraAngle cameraAngle;
    fx32 unk_08;
    u8 unk_0C;
    u8 unk_0D;
    u8 unk_0E[2];
} UnkStruct_ov113_02260218;
typedef struct {
    fx32 unk_00;
    s16 unk_04;
    u8 unk_06;
    u8 unk_07;
} UnkStruct_ov113_022602E4;
typedef struct {
    VecFx32 unk_00;
    VecFx32 unk_0C;
    fx32 unk_18;
    u8 unk_1C;
    u8 unk_1D;
    u8 unk_1E[2];
} UnkStruct_ov113_02260394;
typedef struct {
    VecFx32 unk_00;
    VecFx32 unk_0C;
    fx32 unk_18;
    u8 unk_1C;
    u8 unk_1D;
    u8 unk_1E[2];
} UnkStruct_ov113_0226046C;
typedef struct {
    CameraAngle cameraAngle;
    s32 unk_08;
    s16 unk_0C;
    u8 unk_0E;
    u8 unk_0F;
} UnkStruct_ov113_02260544;
typedef struct {
    UnkStruct_ov113_0225FE30 unk_00;
    UnkStruct_ov113_0225FEE0 unk_08;
    UnkStruct_ov113_0225FF8C unk_18;
    UnkStruct_ov113_02260064 unk_28;
    UnkStruct_ov113_0226013C unk_38;
    UnkStruct_ov113_02260218 unk_48;
    UnkStruct_ov113_022602E4 unk_58;
    UnkStruct_ov113_02260394 unk_60;
    UnkStruct_ov113_0226046C unk_80;
    UnkStruct_ov113_02260544 unk_A0;
} UnkStruct_ov113_0225EB20_sub1;
typedef struct {
    u8 unk_00;
    u8 unk_01;
    u16 unk_02;
} UnkStruct_ov113_0225EDCC;
typedef struct {
    u8 unk_00;
    u8 unk_01;
    s16 unk_02;
} UnkStruct_ov113_0225FD80;
typedef struct {
    UnkStruct_ov113_0225EB20_sub1 unk_00;
    UnkStruct_ov113_0225FD80 unk_B0;
    UnkStruct_ov113_0225E6B8 *unk_B4[128];
    UnkStruct_ov113_0225E6B8 *unk_2B4[128];
    NNSG3dResTex *unk_4B4[32];
    u32 unk_534[2];
    UnkStruct_ov113_0225EDCC unk_53C[128];
    u8 unk_73C;
    u8 unk_73D;
    u8 unk_73E;
    u8 unk_73F;
    u8 unk_740;
    s16 unk_742;
} UnkStruct_ov113_0225EB20;
void ov89_0225A5EC(UnkStruct_ov113_0225EB20 *param0);
typedef struct MessageLoader MessageLoader;
typedef struct StringTemplate StringTemplate;
enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};
typedef struct Easy3DObject {
    NNSG3dRenderObj renderObj;
    VecFx32 position;
    VecFx32 scale;
    BOOL visible;
    u16 rotation[ROTATION_AXIS_COUNT];
    u8 padding_76[2];
} Easy3DObject;
typedef struct G3DPipelineBuffers G3DPipelineBuffers;
typedef struct YesNoTouchMenu YesNoTouchMenu;
typedef struct {
    Field3dModel unk_00;
    Easy3DObject unk_10;
} UnkStruct_ov113_0225DBCC_sub1;
typedef struct {
    FontOAM *unk_00;
    UnkStruct_02021AC8 unk_04;
    u16 unk_10;
} UnkStruct_ov113_0225E250;
struct UnkStruct_ov113_0225DBCC_t {
    UnkStruct_ov66_0223127C *unk_00;
    SaveData *saveData;
    BgConfig *unk_08;
    PaletteData *unk_0C;
    UnkStruct_02012744 *unk_10;
    G3DPipelineBuffers *unk_14;
    SysTask *unk_18;
    SpriteSystem *unk_1C;
    SpriteManager *unk_20;
    YesNoTouchMenu *unk_24;
    u8 unk_28;
    u8 unk_29;
    u8 pad_2A[2];
    StringTemplate *unk_2C;
    MessageLoader *unk_30;
    Window unk_34[8];
    Window unk_B4;
    String *unk_C4;
    u8 unk_C8;
    u8 pad_C9[3];
    Camera *camera;
    fx32 unk_D0;
    fx32 unk_D4;
    struct { struct { void *data; struct NNSG3dResMdlSet_ *set; struct NNSG3dResMdl_ *model; struct NNSG3dResTex_ *texture; } unk_00; struct { NNSG3dRenderObj renderObj; struct { fx32 x; fx32 y; fx32 z; } position; struct { fx32 x; fx32 y; fx32 z; } scale; BOOL visible; u16 rotation[3]; u8 padding_76[2]; } unk_10; } unk_D8;
    NARC *unk_160;
    NARC *unk_164;
    UnkStruct_ov113_022607EC unk_168;
    s32 unk_170[8];
    int unk_190;
    UnkStruct_ov113_0225EB20 unk_194;
    UnkStruct_ov113_02260818 unk_8D8[6];
    u8 unk_920;
    u8 unk_921;
    u8 pad_922[2];
    ManagedSprite *unk_924[6];
    ManagedSprite *unk_93C[6];
    ManagedSprite *unk_954[6];
    ManagedSprite *unk_96C;
    ManagedSprite *unk_970[8];
    ManagedSprite *unk_990[3];
    u16 unk_99C[8];
    u16 unk_9AC[8];
    int unk_9BC;
    u16 unk_9C0[2048];
    UnkStruct_ov113_0225E250 unk_19C0;
    int unk_19D4;
    int unk_19D8;
    int unk_19DC;
    BOOL unk_19E0;
};
void ov89_02259C0C(struct UnkStruct_ov113_0225DBCC_t *param0);

void ov89_0225A760(UnkStruct_ov113_0225EB20 *param0, int param1);

s32 ov89_02258F00(OverlayManager *man) {
    void *v0;

    v0 = OverlayManager_GetData(man);
    SysTask_Destroy(*(SysTask **)((u8 *)v0 + 0x18));
    ov89_022596DC(v0);
    ov89_022598A8(v0);
    YesNoPrompt_Destroy(*(YesNoPrompt **)((u8 *)v0 + 0x24));
    ov89_0225A47C(v0 + 0x194);
    String_Delete(*(String **)((u8 *)v0 + 0xC4));
    FontID_Release(FONT_SUBSCREEN);
    sub_020135AC(*(UnkStruct_02013534 **)((u8 *)v0 + 0x10));
    DestroyMsgData(*(MsgData **)((u8 *)v0 + 0x30));
    MessageFormat_Delete(*(MessageFormat **)((u8 *)v0 + 0x2C));
    ov89_02259D50(v0);
    ov89_02259230(*(void **)((u8 *)v0 + 8));
    Heap_Free(*(void **)((u8 *)v0 + 8));
    SpriteSystem_FreeResourcesAndManager(*(SpriteSystem **)((u8 *)v0 + 0x1C), *(SpriteManager **)((u8 *)v0 + 0x20));
    SpriteSystem_Free(*(SpriteSystem **)((u8 *)v0 + 0x1C));
    PaletteData_FreeBuffers(*(PaletteData **)((u8 *)v0 + 0xC), PLTTBUF_MAIN_BG);
    PaletteData_FreeBuffers(*(PaletteData **)((u8 *)v0 + 0xC), PLTTBUF_SUB_BG);
    PaletteData_FreeBuffers(*(PaletteData **)((u8 *)v0 + 0xC), PLTTBUF_MAIN_OBJ);
    PaletteData_FreeBuffers(*(PaletteData **)((u8 *)v0 + 0xC), PLTTBUF_SUB_OBJ);
    PaletteData_Free(*(PaletteData **)((u8 *)v0 + 0xC));
    ov89_02259C00(v0);
    ov89_02259BA0(v0);
    ov89_02259E10(*(void **)((u8 *)v0 + 0x14));
    NARC_Delete(*(NARC **)((u8 *)v0 + 0x160));
    NARC_Delete(*(NARC **)((u8 *)v0 + 0x164));
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GF_DestroyVramTransferManager();
    DisableTouchPad();
    TextFlags_SetCanABSpeedUpPrint(0);
    TextFlags_SetAutoScrollParam(0);
    TextFlags_SetCanTouchSpeedUpPrint(0);
    sub_0203A914();
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_125);
    return 1;
}

void ov89_02258FF4(SysTask *param0, void *param1) {
    UnkStruct_ov113_0225DBCC *v0 = param1;

    ov89_0225A5EC(&v0->unk_194);
    ov89_02259C0C(v0);

    SpriteSystem_DrawSprites(v0->unk_20);
    SpriteSystem_UpdateTransfer();
    RequestSwap3DBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
}

void ov89_0225901C(void *param0) {
    UnkStruct_ov113_0225DBCC *v0 = param0;

    ov89_0225A760(&v0->unk_194, v0->unk_9BC);

    GF_RunVramTransferTasks();
    SpriteSystem_TransferOam();
    PaletteData_PushTransparentBuffers(v0->unk_0C);
    DoScheduledBgGpuUpdates(v0->unk_08);

    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}
