#include "global.h"
#include "field_system_def.h"
#include "simple3dmodel.h"
#include "overlay_108_021E8850.h"
#include "overlay_108_safari_decoration_app.h"
#include "constants/heap.h"
#include "gf_gfx_planes.h"
#include "unk_struct_0209_bdf8.h"
#include "save.h"
#include "options.h"
#include "pm_string.h"
#include "filesystem.h"
#include "palette.h"
#include "pokemon_types_def.h"
#include "sprite_system.h"
#include "player_data.h"
#include "game_stats.h"
#include "overlay_85_ov109_021D5140.h"
#include "camera.h"
#include "msgdata.h"
#include "simple3danimation.h"
#include "unk_struct_0209_c194.h"

struct BgConfig;

struct BgTemplate;

struct Camera;

struct CameraAngle;

struct GameStats;

struct GraphicsModes;

struct JournalEntry;

struct MsgData;

struct NARC;

struct Options;

struct PaletteData;

struct Party;

struct SaveData;

struct Simple3DAnimation;

struct Simple3DModel;

struct Simple3DRenderObj;

struct Simple3DRotationAngles;

struct SpriteManager;

struct SpriteSystem;

struct String;

struct MessageFormat;

struct SysTask;

struct SysTaskManager;

struct TouchPadDataBuffer;

struct PlayerProfile;

struct UnkStruct_0205C22C_t;

struct UnkStruct_ov109_021D0F70_t;

struct Window;

typedef struct {
    u16 unk_00;
    u16 unk_02;
    s16 unk_04;
    u16 unk_06;
} UnkStruct_0209C0F0;

typedef struct UnkStruct_ov109_021D0F70_t UnkStruct_ov109_021D0F70;

typedef struct {
    u16 unk_00;
    u16 unk_02;
} UnkStruct_ov109_021D1048;

typedef struct {
    u16 unk_00;
    u16 unk_02;
    u16 unk_04[5];
    u16 unk_0E;
} UnkStruct_ov109_021D17EC;

typedef struct UnkStruct_0205C22C_t UnkStruct_0205C22C;

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

typedef struct JournalEntry JournalEntry;

void SetBothScreensModesAndDisable(const GraphicsModes *graphicsModes);

BgConfig *BgConfig_Alloc(enum HeapID heapID);

void InitBgFromTemplate(BgConfig *bgConfig, u8 bgLayer, const BgTemplate *bgTemplate, u8 bgType);

void BG_ClearCharDataRange(u8 bgLayer, u32 size, u32 offset, enum HeapID heapID);

void BgClearTilemapBufferAndCommit(BgConfig *bgConfig, u8 bgLayer);

typedef struct Simple3DRotationAngles {
    u16 alpha;
    u16 beta;
    u16 gamma;
    u8 padding_06[2];
} Simple3DRotationAngles;

typedef struct TouchPadDataBuffer {
    u16 bufferSize;
    TPData buffer[8];
} TouchPadDataBuffer;

typedef struct {
    VecFx32 unk_00;
    VecFx32 unk_0C;
    VecFx32 unk_18;
    VecFx32 unk_24;
    VecFx32 unk_30;
    fx32 unk_3C;
    fx32 unk_40;
    fx32 unk_44;
    fx32 unk_48;
    Simple3DRotationAngles unk_4C;
    Simple3DModel unk_54;
    Simple3DRenderObj unk_68;
} UnkStruct_ov109_021D2AE4;

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    fx32 unk_1C;
    fx32 unk_20;
    Simple3DRotationAngles unk_24;
    VecFx32 unk_2C;
    VecFx32 unk_38;
    VecFx32 unk_44;
    VecFx32 unk_50;
    Simple3DRenderObj unk_5C;
} UnkStruct_ov109_021D2D78;

typedef struct {
    int unk_00;
    Simple3DRotationAngles unk_04;
    VecFx32 unk_0C;
    VecFx32 unk_18;
    Simple3DRenderObj unk_24;
    UnkStruct_ov109_021D2D78 *unk_78;
} UnkStruct_ov109_021D2EF4;

typedef struct {
    int unk_00;
    VecFx32 unk_04;
    Simple3DRenderObj unk_10;
    UnkStruct_ov109_021D2D78 *unk_64;
} UnkStruct_ov109_021D2FA4;

typedef struct {
    Simple3DModel unk_00;
    Simple3DModel unk_14;
    Simple3DModel unk_28[5];
    Simple3DAnimation unk_8C[5];
    UnkStruct_ov109_021D2D78 unk_140[5];
    UnkStruct_ov109_021D2EF4 unk_4B0[5];
    UnkStruct_ov109_021D2FA4 unk_71C[5];
} UnkStruct_ov109_021D1334;

typedef struct {
    int unk_00;
    MsgData *unk_04;
    MessageFormat *unk_08;
    Window unk_0C[1];
    Window unk_1C[5];
    void *unk_6C;
} UnkStruct_ov109_021D24F8;

typedef struct {
    fx32 unk_00;
    u32 unk_04;
    VecFx32 unk_08;
    CameraAngle cameraAngle;
    Camera *camera;
} UnkStruct_ov109_021D28C4;

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    TouchPadDataBuffer unk_10;
} UnkStruct_ov109_021D294C;

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    fx32 unk_10;
    fx32 unk_14;
    fx32 unk_18;
    UnkStruct_ov109_021D2D78 *unk_1C;
} UnkStruct_ov109_021D3110;

typedef struct {
    UnkStruct_ov109_021D3110 unk_00[5];
} UnkStruct_ov109_021D31A8;

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    fx32 unk_0C;
    UnkStruct_ov109_021D2D78 *unk_10;
} UnkStruct_ov109_021D3268;

typedef struct {
    UnkStruct_ov109_021D3268 unk_00[5];
} UnkStruct_ov109_021D32DC;

typedef struct {
    int unk_00;
    int unk_04;
    fx32 unk_08;
    fx32 unk_0C;
    fx32 unk_10;
    UnkStruct_ov109_021D2D78 *unk_14;
} UnkStruct_ov109_021D3370;

typedef struct {
    UnkStruct_ov109_021D3370 unk_00[5];
} UnkStruct_ov109_021D3414;

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    fx32 unk_10;
} UnkStruct_ov109_021D34A8;

typedef struct {
    u16 unk_00;
    u16 unk_02;
    int unk_04;
} UnkStruct_ov109_021D3700;

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    UnkStruct_ov109_021D2D78 *unk_0C;
} UnkStruct_ov109_021D3818;

typedef struct {
    UnkStruct_ov109_021D3818 unk_00[5];
} UnkStruct_ov109_021D3884;

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18[5];
    UnkStruct_0209C0F0 unk_2C;
    UnkStruct_ov109_021D17EC unk_34;
    UnkStruct_ov109_021D1048 unk_44[5];
    PlayerProfile *unk_58[5];
    String *unk_6C[5];
} UnkStruct_ov109_021D0F70_sub1;

struct UnkStruct_ov109_021D0F70_t {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    int unk_1C;
    u32 unk_20;
    Party *unk_24;
    UnkStruct_ov109_021D2D78 *unk_28;
    UnkStruct_ov109_021D0F70_sub1 unk_2C;
    u8 unk_AC[32];
    UnkStruct_0209C194 *unk_CC;
    UnkStruct_0209BDF8 *unk_D0;
    UnkStruct_ov109_021D2AE4 unk_D4;
    UnkStruct_ov109_021D1334 unk_190;
    UnkStruct_ov109_021D31A8 unk_AB4;
    UnkStruct_ov109_021D32DC unk_B54;
    UnkStruct_ov109_021D3414 unk_BB8;
    UnkStruct_ov109_021D34A8 unk_C30;
    UnkStruct_ov109_021D3700 unk_C44;
    UnkStruct_ov109_021D3884 unk_C4C;
    UnkStruct_ov109_021D24F8 unk_C9C;
    UnkStruct_ov109_021D28C4 unk_D0C;
    UnkStruct_ov109_021D294C unk_D2C;
    NARC *unk_D80;
    BgConfig *unk_D84;
    NNSG2dScreenData *unk_D88;
    NNSG2dCharacterData *unk_D8C;
    NNSG2dPaletteData *unk_D90;
    SpriteSystem *unk_D94;
    SpriteManager *unk_D98;
    PaletteData *unk_D9C;
    GXRgb unk_DA0[8];
    SysTask *unk_DB0;
    SysTask *unk_DB4;
    SysTask *unk_DB8;
    SysTask *unk_DBC;
    SysTask *unk_DC0;
    SysTask *unk_DC4;
    void *unk_DC8;
};

extern const GraphicsModes ov108_021EAAC0;

extern const BgTemplate ov108_021EAB54;

extern const BgTemplate ov108_021EAB1C;

extern const BgTemplate ov108_021EAB38;

extern const BgTemplate ov108_021EAB8C;

extern const BgTemplate ov108_021EAAE4;

extern const BgTemplate ov108_021EAB00;

extern const BgTemplate ov108_021EAB70;

void ov108_021E9850(SafariDecorationApp *param0) {
    ov108_021E9830();
    GX_SetDispSelect(GX_DISP_SELECT_MAIN_SUB);
    param0->bgConfig = BgConfig_Alloc(param0->heapID);

    {
        GraphicsModes v0 = ov108_021EAAC0;

        SetBothScreensModesAndDisable(&v0);
    }

    {
        BgTemplate v1 = ov108_021EAB54;

        InitBgFromTemplate(param0->bgConfig, GF_BG_LYR_MAIN_1, &v1, 0);
        BgClearTilemapBufferAndCommit(param0->bgConfig, GF_BG_LYR_MAIN_1);
    }

    {
        BgTemplate v2 = ov108_021EAB1C;

        InitBgFromTemplate(param0->bgConfig, GF_BG_LYR_MAIN_2, &v2, 0);
        BgClearTilemapBufferAndCommit(param0->bgConfig, GF_BG_LYR_MAIN_2);
    }

    {
        BgTemplate v3 = ov108_021EAB38;

        InitBgFromTemplate(param0->bgConfig, GF_BG_LYR_MAIN_3, &v3, 0);
        BgClearTilemapBufferAndCommit(param0->bgConfig, GF_BG_LYR_MAIN_3);
    }

    {
        BgTemplate v6 = ov108_021EAB8C;

        InitBgFromTemplate(param0->bgConfig, GF_BG_LYR_SUB_0, &v6, 0);
        BgClearTilemapBufferAndCommit(param0->bgConfig, GF_BG_LYR_SUB_0);
    }

    {
        BgTemplate v7 = ov108_021EAAE4;

        InitBgFromTemplate(param0->bgConfig, GF_BG_LYR_SUB_1, &v7, 0);
        BgClearTilemapBufferAndCommit(param0->bgConfig, GF_BG_LYR_SUB_1);
    }

    {
        BgTemplate v4 = ov108_021EAB00;

        InitBgFromTemplate(param0->bgConfig, GF_BG_LYR_SUB_2, &v4, 0);
        BgClearTilemapBufferAndCommit(param0->bgConfig, GF_BG_LYR_SUB_2);
    }

    {
        BgTemplate v5 = ov108_021EAB70;

        InitBgFromTemplate(param0->bgConfig, GF_BG_LYR_SUB_3, &v5, 0);
        BgClearTilemapBufferAndCommit(param0->bgConfig, GF_BG_LYR_SUB_3);
    }

    BG_ClearCharDataRange(GF_BG_LYR_MAIN_3, 32, 0, param0->heapID);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_0, 32, 0, param0->heapID);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_3, 32, 0, param0->heapID);
}
