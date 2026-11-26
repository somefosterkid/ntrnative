#include "global.h"

#include "constants/heap.h"

#include "field/overlay_01_021FB878.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "options.h"
#include "pm_string.h"
#include "save.h"
#include "sys_task.h"
#include "unk_0200A090.h"
#include "gf_gfx_planes.h"
#include "vram_transfer_manager.h"
#include "unk_0200B150.h"
#include "overlay_48_unk_struct_ov69_0225_d35_c.h"
#include "overlay_48_unk_struct_ov69_0225ddc8.h"

struct G3DPipelineBuffers;

struct MessageLoader;

struct SpriteList;

struct SpriteResourceCollection;

struct StringTemplate;

struct UnkStruct_ov66_0222DFF8_t;

struct WiFiHistory;

struct YesNoTouchMenu;

typedef struct WiFiHistory WiFiHistory;

typedef struct UnkStruct_ov66_0222DFF8_t WiFiPlazaSystem;

typedef struct {
    u8 pad_0[4];
    u8 unk_4;
    u8 unk_5;
    u16 unk_6;
    u32 unk_8;
} UnkStruct_ov66_02231428;

typedef struct {
    u16 unk_00;
    u8 unk_02;
    u8 unk_03_0 : 4;
    u8 unk_03_4 : 4;
} UnkStruct_ov66_02231560_sub1;

typedef struct {
    UnkStruct_ov66_02231560_sub1 unk_00[50];
} UnkStruct_ov66_02231560;

typedef struct {
    const UnkStruct_ov66_02231560 *unk_00;
    SaveData *saveData;
    UnkStruct_ov66_02231428 unk_08;
    WiFiPlazaSystem *unk_0C;
} UnkStruct_ov66_02230F50;

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

typedef struct MessageLoader MessageLoader;

typedef struct SpriteList SpriteList;

typedef struct Sprite {
    VecFx32 position;
    VecFx32 affineTranslation;
    VecFx32 affineScale;
    u16 affineZRotation;
    u8 affineOverwriteMode;
    u8 flip;
    u8 overwriteFlags;
    u8 explicitPalette;
    u8 explicitPaletteOffset;
    BOOL explicitMosaic;
    GXOamMode explicitOamMode;
    u8 draw;
    u8 animate;
    fx32 animSpeed;
    SpriteList *list;

    u32 animData[29];
    NNSG2dImageProxy imageProxy;
    NNSG2dImagePaletteProxy paletteProxy;
    u32 type;
    u16 activeAnimID;
    u8 explicitPriority;
    u16 priority;
    enum NNS_G2D_VRAM_TYPE vramType;
    struct Sprite *prev;
    struct Sprite *next;
} Sprite;

struct SpriteList {
    Sprite *sprites;
    int capacity;
    Sprite **freeSprites;
    int freeSpriteHead;
    Sprite sentinelData;
    NNSG2dRendererInstance *renderer;
    void *rawAnimData;
    NNSG2dCellAnimBankData *defaultAnimBank;
    BOOL active;
};

typedef struct SpriteResourceCollection SpriteResourceCollection;

typedef struct StringTemplate StringTemplate;

typedef struct YesNoTouchMenu YesNoTouchMenu;

typedef struct {
    s16 unk_00;
    s16 unk_02;
    MtxFx33 unk_04;
    u16 unk_28;
    u16 unk_2A;
    u16 unk_2C;
} UnkStruct_ov69_0225CA7C_sub1_sub1;

typedef struct {
    u32 unk_00;
    UnkStruct_ov69_0225CA7C_sub1_sub1 unk_04[1024];
} UnkStruct_ov69_0225CA7C_sub1;

typedef struct {
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    s16 unk_06;
    s16 unk_08;
    s16 unk_0A;
    s16 unk_0C;
    u16 unk_0E;
} UnkStruct_ov69_0225ECF0;

typedef struct {
    u32 unk_00_0 : 1;
    u32 unk_00_1 : 1;
    u32 : 30;
    u32 unk_04_0 : 31;
    u32 : 1;
} UnkStruct_ov69_0225DA74;

typedef struct {
    VecFx32 unk_00;
    VecFx32 unk_0C;
    VecFx32 unk_18;
    Easy3DObject unk_24;
    Field3dModel unk_9C;
} UnkStruct_ov69_0225D71C;

typedef struct {
    UnkStruct_ov69_0225CA7C_sub1 unk_00;
    VecFx32 unk_B804;
    Easy3DObject unk_B810[3];
    Field3dModel unk_B978[3];
} UnkStruct_ov69_0225CA7C;

typedef struct {
    Camera *camera;
    CameraAngle unk_04;
    fx32 unk_0C;
    u16 unk_10;
    u16 unk_12;
} UnkStruct_ov69_0225DAEC;

typedef struct {
    u32 unk_00;
    u32 unk_04;
    u32 unk_08;
} UnkStruct_ov69_0225E0C0;

typedef struct {
    u16 unk_00;
    u16 unk_02;
    u8 unk_04[4];
    void *unk_08[2];
    NNSG2dScreenData *unk_10[2];
    u16 unk_18;
    u16 unk_1A;
    void *unk_1C;
    NNSG2dPaletteData *unk_20;
    u8 unk_24;
    u8 unk_25;
    u8 unk_26;
    u8 unk_27;
} UnkStruct_ov69_0225E7DC;

typedef struct {
    u8 unk_00;
    s8 unk_01;
    u8 unk_02[2];
} UnkStruct_ov69_0225E084_sub1;

typedef struct {
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    s16 unk_06;
    u16 unk_08;
    u16 unk_0A;
    const UnkStruct_ov69_0225ECF0 *unk_0C;
} UnkStruct_ov69_0225ED90;

typedef struct {
    UnkStruct_ov69_0225ED90 unk_00[3];
    s16 unk_30;
    s16 unk_32;
    u8 unk_34[4];
    u8 unk_38[4];
    Sprite *unk_3C[12];
    SpriteResource *unk_6C[12][4];
} UnkStruct_ov69_0225EB60;

typedef struct {
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    s16 unk_06;
    s16 unk_08;
    s16 unk_0A;
    SysTask *unk_0C;
} UnkStruct_ov69_0225E478;

typedef struct {
    u32 unk_00;
    UnkStruct_ov69_0225E0C0 unk_04[4];
    u16 unk_34;
    u16 unk_36;
    u32 unk_38;
    u16 unk_3C;
    u16 unk_3E;
    UnkStruct_ov69_0225E084_sub1 unk_40[18];
    u32 unk_88[3];
    void *unk_94;
    NNSG2dScreenData *unk_98;
    u16 unk_9C;
    u16 unk_9E;
    UnkStruct_ov69_0225E7DC unk_A0[5];
    Window unk_168[3];
    Window unk_198;
    Window unk_1A8;
    UnkStruct_ov69_0225EB60 unk_1B8;
    UnkStruct_ov69_0225E478 unk_2E4;
} UnkStruct_ov69_0225E084;

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    Window unk_1C;
} UnkStruct_ov69_0225DC48;

typedef struct {
    MessageLoader *unk_00;
    StringTemplate *unk_04;
    String *unk_08;
    String *unk_0C;
} UnkStruct_ov69_0225EF54;

typedef struct {
    u16 unk_00;
    u16 unk_02;
    UnkStruct_ov69_0225DA74 unk_04;
    WiFiHistory *wiFiHistory;
    Options *options;
    int unk_14;
    int unk_18;
    UnkStruct_ov66_02231428 unk_1C;
    UnkStruct_ov69_0225D35C unk_20;
    UnkStruct_ov69_0225EF54 unk_1CC;
    UnkStruct_ov69_0225D71C unk_1DC;
    UnkStruct_ov69_0225CA7C unk_288;
    UnkStruct_ov69_0225DAEC unk_BC30;
    UnkStruct_ov69_0225DC48 unk_BC44;
    UnkStruct_ov69_0225E084 unk_BC70;
    UnkStruct_ov69_0225DDC8 unk_BF64;
} UnkStruct_ov69_0225CE64;

void ov48_022593F4(UnkStruct_ov69_0225CE64 *param0, const UnkStruct_ov66_02230F50 *param1);

s32 ov48_022594F0(void *, Options *, enum HeapID);
s32 ov48_02259688(void *, enum HeapID);
s32 ov48_02259750(void *, enum HeapID);
extern GraphicsBanks ov48_0225B210;
void ov48_02259464(UnkStruct_ov69_0225D35C *arg0, void *arg1, enum HeapID arg2);
s32 ov48_02259650(void *);
s32 ov48_02259724(void *);
s32 ov48_02259788(void *);
void ov48_022594A8(UnkStruct_ov69_0225D35C *arg0);

struct UnkStruct_ov63_0222BE18_t;
struct UnkStruct_ov63_0222BEC0_t;
struct UnkStruct_ov63_0222D848_t;
struct UnkStruct_ov66_02234548_t;
struct UnkStruct_ov66_02234798_t;
struct UnkStruct_ov70_0225C894_t;
struct UnkStruct_ov70_0225CA20_t;
struct UnkStruct_ov70_0225CC54_t;
typedef struct UnkStruct_ov70_0225C894_t UnkStruct_ov70_0225C894;
typedef struct UnkStruct_ov70_0225CA20_t UnkStruct_ov70_0225CA20;
typedef struct UnkStruct_ov70_0225CC54_t UnkStruct_ov70_0225CC54;
typedef struct UnkStruct_ov63_0222BE18_t UnkStruct_ov63_0222BE18;
typedef struct UnkStruct_ov63_0222BEC0_t UnkStruct_ov63_0222BEC0;
typedef struct UnkStruct_ov63_0222D848_t UnkStruct_ov63_0222D848;
typedef struct UnkStruct_ov66_02234548_t UnkStruct_ov66_02234548;
typedef struct UnkStruct_ov66_02234798_t UnkStruct_ov66_02234798;
void SpriteList_RenderAndAnimateSprites(UnkStruct_ov66_02234548 *param0);
typedef struct {
    fx32 unk_00;
    fx32 unk_04;
    fx32 unk_08;
    u32 unk_0C;
} UnkStruct_ov70_0225D220;
typedef struct {
    u8 unk_00;
    u8 unk_01;
    s8 unk_02;
    s8 unk_03;
    s16 unk_04;
    s16 unk_06;
} UnkStruct_ov70_0225D93C;
typedef union {
    struct {
        s16 unk_00;
        s16 unk_02;
    } val1;
    struct {
        UnkStruct_ov70_0225D220 unk_00;
        u32 unk_10;
    } val2;
    struct {
        UnkStruct_ov70_0225D220 unk_00;
        u32 unk_10;
        fx32 unk_14;
    } val3;
    struct {
        s32 unk_00;
        VecFx32 unk_04;
    } val4;
} UnkStruct_ov70_0225CC54_sub1;
struct UnkStruct_ov70_0225CC54_t {
    UnkStruct_ov63_0222BEC0 *unk_00;
    UnkStruct_ov66_02234798 *unk_04;
    u16 unk_08;
    u8 unk_0A;
    u8 unk_0B;
    UnkStruct_ov70_0225CC54_sub1 unk_0C;
    void (*unk_24)(struct UnkStruct_ov70_0225CC54_t *, UnkStruct_ov70_0225CA20 *);
};
struct UnkStruct_ov70_0225CA20_t {
    UnkStruct_ov63_0222BE18 *unk_00;
    UnkStruct_ov66_02234548 *unk_04;
    UnkStruct_ov70_0225CC54 *unk_08;
    u16 unk_0C;
    u16 unk_0E;
    UnkStruct_ov63_0222D848 *unk_10;
    const UnkStruct_ov70_0225C894 *unk_14;
    UnkStruct_ov70_0225D93C unk_18;
    UnkStruct_ov70_0225CC54 *unk_20;
    UnkStruct_ov70_0225CC54 *unk_24;
};
void ov48_022594D0(UnkStruct_ov70_0225CA20 *param0);
void ov48_022594DC(UnkStruct_ov69_0225D35C *param0);

void ov48_022593F4(UnkStruct_ov69_0225CE64 *param0, const UnkStruct_ov66_02230F50 *param1) {
    int v0;

    param0->unk_04.unk_00_1 = 0;
    param0->unk_04.unk_00_0 = 0;

    if (gGameLanguage == LANGUAGE_JAPANESE) {
        param0->unk_04.unk_00_1 = 1;
    } else {
        param0->unk_04.unk_00_1 = 0;
    }

    if (param0->unk_04.unk_00_1 == 1) {
        for (v0 = 0; v0 < 50; v0++) {
            if (param1->unk_00->unk_00[v0].unk_03_4 == 1) {
                if (param1->unk_00->unk_00[v0].unk_00 != 103) {
                    param0->unk_04.unk_00_0 = 1;
                }
            }
        }
    } else {
        param0->unk_04.unk_00_0 = 1;
    }
}

void ov48_02259464(UnkStruct_ov69_0225D35C *arg0, void *arg1, enum HeapID heapID) {
    arg0->unk_1A8 = NARC_New(NARC_a_1_9_2, heapID);
    GF_CreateVramTransferManager(0x40U, heapID);
    GfGfx_SetBanks(&ov48_0225B210);
    ov48_022594F0(arg0, arg1, heapID);
    ov48_02259688(arg0, heapID);
    ov48_02259750(arg0, heapID);
}

void ov48_022594A8(UnkStruct_ov69_0225D35C *arg0) {
    NARC_Delete(arg0->unk_1A8);
    GF_DestroyVramTransferManager();
    ov48_02259650(arg0);
    ov48_02259724(arg0);
    ov48_02259788(arg0);
}

void ov48_022594D0(UnkStruct_ov70_0225CA20 *param0) {
    SpriteList_RenderAndAnimateSprites(param0->unk_04);
}

void ov48_022594DC(UnkStruct_ov69_0225D35C *param0) {
    DoScheduledBgGpuUpdates(param0->unk_00);
    OamManager_ApplyAndResetBuffers();
    GF_RunVramTransferTasks();
}
