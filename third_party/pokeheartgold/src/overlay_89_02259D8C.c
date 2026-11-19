#include "global.h"

#include "filesystem.h"
#include "pm_string.h"
#include "save.h"
#include "sprite.h"
#include "unk_02005D10.h"
#include "gf_3d_vramman.h"
#include "palette.h"
#include "sys_task.h"
#include "unk_02013534.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "yes_no_prompt.h"

struct BgConfig;
struct Camera;
struct TextOBJ;
struct G3DPipelineBuffers;
struct MsgData;
struct PaletteData;
struct Party;
struct Pokemon;
struct SpriteManager;
struct SpriteSystem;
struct MessageFormat;
struct SysTask;
struct SysTaskManager;
struct UnkStruct_ov113_0225DBCC_t;
struct UnkStruct_ov113_0225E6B8_t;
struct YesNoPrompt;
void GfGfx_EngineATogglePlanes(u8 layerMask, u8 enable);
typedef struct SysTaskManager SysTaskManager;
struct SysTaskManager {
    u16 maxTasks;
    u16 stackPointer;
    SysTask sentinelTask;
    SysTask **taskStack;
    SysTask *tasks;
    BOOL locked;
    SysTask *currentTask;
    SysTask *nextTask;
};
void GF_3DVramMan_Delete(GF3DVramMan *vramMan);
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
s32 ov45_0222E9E0(void);
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
typedef struct CharTransferAllocation {
    u32 size;
    u32 offset;
    u16 vramType;
    u16 atEnd;
} CharTransferAllocation;
typedef struct G3DPipelineBuffers G3DPipelineBuffers;
typedef struct {
    TextOBJ *unk_00;
    CharTransferAllocation unk_04;
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
    YesNoPrompt *unk_24;
    u8 unk_28;
    u8 unk_29;
    u8 pad_2A[2];
    MessageFormat *unk_2C;
    MsgData *unk_30;
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
typedef struct {
    UnkStruct_ov113_02260818 unk_00;
} UnkStruct_ov113_0225CA04;
int Party_GetCount(const Party *party);
Pokemon *Party_GetMonByIndex(const Party *party, int slot);
Party *SaveArray_Party_Get(SaveData *saveData);
u32 GetMonData(Pokemon *mon, int param, void *dest);
u16 ov89_0225C830(int param0, u32 param1);
u32 ov45_0222EC7C(s32 param0);
BOOL ov89_0225A49C(UnkStruct_ov113_0225DBCC *param0, UnkStruct_ov113_0225EB20 *param1, const UnkStruct_ov113_02260818 *param2, NARC *param3, NARC *param4, BOOL param5);
void PaletteData_FillPaletteInBuffer(PaletteData *paletteData, enum PaletteBufferId bufferID, enum PaletteSelector selector, u16 fillVal, u16 start, u16 end);
void ManagedSprite_SetDrawFlag(ManagedSprite *managedSprite, BOOL draw);
BOOL ov89_02259A3C(int param0, int param1, ManagedSprite *param2, NARC *param3, NARC *param4, NNS_G2D_VRAM_TYPE param5, BOOL param6);

void ov89_02259D8C(void);
void ov89_02259E10(GF3DVramMan *vramMan);
void ov89_02259E18(UnkStruct_ov113_0225DBCC *param0);
void ov89_02259E28(UnkStruct_ov113_0225DBCC *param0, UnkStruct_ov113_0225CA04 *param1);
UnkStruct_ov113_022607EC *ov89_02259E48(UnkStruct_ov113_0225DBCC *param0);
void ov89_02259E50(int param0, SaveData *saveData, UnkStruct_ov113_02260818 *param2);
BOOL ov89_02259EC4(UnkStruct_ov113_0225DBCC *param0, const UnkStruct_ov113_02260818 *param1, s32 param2);

void ov89_02259D8C(void) {
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, 1);

    G2_SetBG0Priority(1);
    G3X_SetShading(GX_SHADING_TOON);
    G3X_AntiAlias(1);
    G3X_AlphaTest(0, 0);
    G3X_AlphaBlend(1);
    G3X_EdgeMarking(0);
    G3X_SetFog(0, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x8000, 0);
    G3X_SetClearColor(((GXRgb)(((0) << (0)) | ((0) << (5)) | ((0) << (10)))), 0, 0x7fff, 63, 0);
    G3_ViewPort(0, 0, 255, 191);
}

void ov89_02259E10(GF3DVramMan *vramMan) {
    GF_3DVramMan_Delete(vramMan);
}

void ov89_02259E18(UnkStruct_ov113_0225DBCC *param0) {
    UnkStruct_ov113_022607EC *v0 = &param0->unk_168;
    v0->unk_04 = ov45_0222E9E0();
}

void ov89_02259E28(UnkStruct_ov113_0225DBCC *param0, UnkStruct_ov113_0225CA04 *param1) {
    UnkStruct_ov113_02260818 *v0;

    MI_CpuClear8(param1, sizeof(UnkStruct_ov113_0225CA04));

    v0 = &param1->unk_00;

    v0->unk_02 = 150;
    v0->unk_00 = 0x1f;
    v0->unk_04 = 0x101;
    v0->unk_08 = 0;
}

UnkStruct_ov113_022607EC *ov89_02259E48(UnkStruct_ov113_0225DBCC *param0) {
    return &param0->unk_168;
}

void ov89_02259E50(int param0, SaveData *saveData, UnkStruct_ov113_02260818 *param2) {
    Party *v0;
    int v1;
    Pokemon *v2;
    int v3;

    MI_CpuClear8(param2, sizeof(UnkStruct_ov113_02260818) * 6);

    v0 = SaveArray_Party_Get(saveData);
    v1 = Party_GetCount(v0);

    for (v3 = 0; v3 < v1; v3++) {
        v2 = Party_GetMonByIndex(v0, v3);
        param2[v3].unk_02 = GetMonData(v2, MON_DATA_SPECIES_OR_EGG, 0L);
        param2[v3].unk_04 = GetMonData(v2, MON_DATA_PERSONALITY, 0L);
        param2[v3].unk_08 = GetMonData(v2, MON_DATA_FORM, 0L);
        param2[v3].unk_00 = ov89_0225C830(param0, GetMonData(v2, MON_DATA_OT_ID, 0L));
    }
}

BOOL ov89_02259EC4(UnkStruct_ov113_0225DBCC *param0, const UnkStruct_ov113_02260818 *param1, s32 param2) {
    int v0;
    u32 v1;

    v0 = ov89_0225A49C(param0, &param0->unk_194, param1, param0->unk_160, param0->unk_164, param0->unk_19E0);

    if (v0 == 1) {
        v1 = ov45_0222EC7C(param2);

        if (v1 != 0xffffffff) {
            if ((param0->unk_99C[v1] != param1->unk_02) || (param0->unk_9AC[v1] != param1->unk_00)) {
                param0->unk_99C[v1] = param1->unk_02;
                param0->unk_9AC[v1] = param1->unk_00;
                ov89_02259A3C(param1->unk_02, param1->unk_08, param0->unk_970[v1], param0->unk_160, param0->unk_164, NNS_G2D_VRAM_TYPE_2DSUB, param0->unk_19E0);
                ManagedSprite_SetDrawFlag(param0->unk_970[v1], 1);
                PaletteData_FillPaletteInBuffer(param0->unk_0C, PLTTBUF_SUB_BG, PLTTSEL_BOTH, param1->unk_00, (2 * 16 + 1) + v1, (2 * 16 + 1) + v1 + 1);
            }
        }

        PlaySE(1515);
    }

    return v0;
}
