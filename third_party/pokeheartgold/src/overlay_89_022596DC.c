#include "global.h"

#include "filesystem.h"
#include "pm_string.h"
#include "save.h"
#include "sprite.h"
#include "palette.h"

struct BgConfig;
struct Camera;
struct TextOBJ;
struct G3DPipelineBuffers;
struct MsgData;
struct PaletteData;
struct SpriteManager;
struct SpriteSystem;
struct MessageFormat;
struct SysTask;
struct SysTaskManager;
struct UnkStruct_02012744_t;
struct UnkStruct_ov113_0225DBCC_t;
struct UnkStruct_ov113_0225E6B8_t;
struct UnkStruct_ov66_0222DFF8_t;
struct YesNoPrompt;
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
typedef struct TextOBJ TextOBJ;
typedef struct UnkStruct_02012744_t UnkStruct_02012744;
typedef struct BgConfig BgConfig;
typedef struct Window {
    BgConfig *bgConfig;
    u8 bgLayer;
    u8 tilemapLeft;
    u8 tilemapTop;
    u8 width;
    u8 height;
    u8 palette;
    u16 baseTile : 15;
    u16 colorMode : 1;
    void *pixels;
} Window;
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
typedef struct CameraAngle {
    u16 x;
    u16 y;
    u16 z;
    u16 padding_06;
} CameraAngle;
typedef struct Camera Camera;
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
typedef struct MsgData MsgData;
typedef struct SysTask SysTask;
typedef struct SysTaskManager SysTaskManager;
typedef void (*SysTaskFunc)(SysTask *, void *);
enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};
struct SysTask {
    SysTaskManager *manager;
    SysTask *prevTask;
    SysTask *nextTask;
    u32 priority;
    void *param;
    SysTaskFunc callback;
    enum TaskState state;
};
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
typedef struct CharTransferAllocation {
    u32 size;
    u32 offset;
    u16 vramType;
    u16 atEnd;
} CharTransferAllocation;
typedef struct SpriteSystem SpriteSystem;
typedef struct SpriteManager SpriteManager;
void Sprite_DeleteAndFreeResources(ManagedSprite *managedSprite);
typedef struct MessageFormat MessageFormat;
typedef struct G3DPipelineBuffers G3DPipelineBuffers;
typedef struct YesNoPrompt YesNoPrompt;
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
void ov89_0225A354(UnkStruct_ov113_0225E250 *param0);
BOOL SpriteSystem_LoadCharResObjFromOpenNarc(SpriteSystem *spriteSys, SpriteManager *spriteMan, NARC *narc, int memberIdx, BOOL compressed, enum NNS_G2D_VRAM_TYPE vramType, int resourceID);
u8 SpriteSystem_LoadPaletteBufferFromOpenNarc(PaletteData *paletteData, enum PaletteBufferId bufferID, SpriteSystem *spriteSys, SpriteManager *spriteMan, NARC *narc, int memberIdx, BOOL compressed, int paletteIdx, enum NNS_G2D_VRAM_TYPE vramType, int resourceID);
BOOL SpriteSystem_LoadCellResObjFromOpenNarc(SpriteSystem *spriteSys, SpriteManager *spriteMan, NARC *narc, int memberIdx, BOOL compressed, int resourceID);
BOOL SpriteSystem_LoadAnimResObjFromOpenNarc(SpriteSystem *spriteSys, SpriteManager *spriteMan, NARC *narc, int memberIdx, BOOL compressed, int resourceID);

void ov89_022596DC(UnkStruct_ov113_0225DBCC *param0);
void ov89_02259734(UnkStruct_ov113_0225DBCC *param0, NARC *param1);

void ov89_022596DC(UnkStruct_ov113_0225DBCC *param0) {
    int v0;

    for (v0 = 0; v0 < 6; v0++) {
        Sprite_DeleteAndFreeResources(param0->unk_924[v0]);
    }

    for (v0 = 0; v0 < 6; v0++) {
        Sprite_DeleteAndFreeResources(param0->unk_93C[v0]);
    }

    for (v0 = 0; v0 < 6; v0++) {
        Sprite_DeleteAndFreeResources(param0->unk_954[v0]);
    }

    ov89_0225A354(&param0->unk_19C0);
}

void ov89_02259734(UnkStruct_ov113_0225DBCC *param0, NARC *param1) {
    int v0;

    SpriteSystem_LoadPaletteBufferFromOpenNarc(param0->unk_0C, PLTTBUF_SUB_OBJ, param0->unk_1C, param0->unk_20, param1, 10, 0, 1, NNS_G2D_VRAM_TYPE_2DSUB, 10004);
    SpriteSystem_LoadCharResObjFromOpenNarc(param0->unk_1C, param0->unk_20, param1, 9, 0, NNS_G2D_VRAM_TYPE_2DSUB, 10009);
    SpriteSystem_LoadCellResObjFromOpenNarc(param0->unk_1C, param0->unk_20, param1, 8, 0, 10004);
    SpriteSystem_LoadAnimResObjFromOpenNarc(param0->unk_1C, param0->unk_20, param1, 7, 0, 10004);

    for (v0 = 0; v0 < 8; v0++) {
        SpriteSystem_LoadCharResObjFromOpenNarc(param0->unk_1C, param0->unk_20, param1, 13, 0, NNS_G2D_VRAM_TYPE_2DSUB, 10010 + v0);
    }

    SpriteSystem_LoadCellResObjFromOpenNarc(param0->unk_1C, param0->unk_20, param1, 12, 0, 10005);
    SpriteSystem_LoadAnimResObjFromOpenNarc(param0->unk_1C, param0->unk_20, param1, 11, 0, 10005);
}
