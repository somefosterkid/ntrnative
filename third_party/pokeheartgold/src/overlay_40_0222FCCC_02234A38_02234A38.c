#include "global.h"
#include "sys_task.h"
#include "overlay_39_unk_struct_ov62_022349_a8.h"
#include "overlay_40_unk_struct_ov62_02233310.h"
#include "overlay_40_unk_struct_ov62_0223118c.h"
#include "overlay_40_unk_struct_0208_c06_c.h"
#include "overlay_40_unk_struct_ov62_022312b0.h"
#include "constants/heap.h"
#include "palette.h"

struct BgConfig;

struct DressUpPhoto;

struct TextOBJ;

struct G3DPipelineBuffers;

struct ManagedSprite;

struct MsgData;

struct PaletteData;

struct PokepicManager;

struct SaveData;

struct SpriteManager;

struct SpriteSystem;

struct String;

struct MessageFormat;

struct SysTask;

struct SysTaskManager;

struct TouchHitboxController;

struct UnkStruct_020F3DCC_t;

struct UnkStruct_ov22_0225B1BC_t;

typedef struct String String;

typedef struct MailMessage {
    u16 type;
    u16 id;
    u16 words[2];
} MailMessage;

typedef struct DressUpPhoto DressUpPhoto;

typedef struct {
    u16 unk_00;
    u16 unk_02;
} UnkStruct_0202F298_sub1;

typedef struct UnkStruct_02030A80_t {
    u16 unk_00[8];
    u32 id;
    u8 gender;
    u8 month;
    u8 appearance;
    u8 country;
    u8 region;
    u8 version;
    u8 language;
    u8 isEgg : 1;
    u8 form : 7;
    u16 species;
    u8 unk_1E;
    u8 unk_1F;
    union {
        MailMessage unk_20_val1;
        u16 unk_20_val2[40];
    };
    u8 unk_70[12];
    UnkStruct_0202F298_sub1 unk_7C;
} UnkStruct_02030A80;

void PaletteData_BlendPalettes(PaletteData *paletteData, enum PaletteBufferId bufferID, u16 toBlend, u8 fraction, u16 target);

typedef struct ManagedSprite ManagedSprite;

typedef struct SpriteSystem SpriteSystem;

typedef struct SpriteManager SpriteManager;

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

typedef struct UnkStruct_020F3DCC_t UnkStruct_020F3DCC;

struct UnkStruct_020F3DCC_t {
    BOOL unk_00;
    BOOL unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    int unk_1C;
    const UnkStruct_020F3DCC *unk_20;
};

typedef struct TextOBJ TextOBJ;

enum TouchScreenButtonState {
    TOUCH_BUTTON_NOT_PRESSED = -1,
    TOUCH_BUTTON_PRESSED,
    TOUCH_BUTTON_RELEASED,
    TOUCH_BUTTON_HELD,
    TOUCH_BUTTON_HELD_OUT_OF_BOUNDS
};

typedef struct {
    u32 personality;
    u32 otID;
    u16 species;
    u16 trainerName[7 + 1];
    s8 priority;
    u8 xPos;
    u8 yPos;
    u8 form;
} UnkStruct_ov61_0222AE80_sub1;

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    s8 unk_03;
} UnkStruct_ov61_0222AE80_sub2;

typedef struct {
    u16 unk_00;
    u16 unk_02;
} UnkStruct_ov62_02239DA4_sub1;

typedef struct {
    u32 integrity;
    UnkStruct_ov61_0222AE80_sub1 unk_04;
    u32 unk_24;
    MailMessage title;
    UnkStruct_ov61_0222AE80_sub2 unk_30[10];
    u8 unk_58;
    u8 language;
    u8 unk_5A[2];
    UnkStruct_ov62_02239DA4_sub1 unk_5C;
} UnkStruct_ov61_0222AE80;

typedef struct {
    u16 unk_00[8];
    u32 unk_10;
    u8 unk_14;
    u8 unk_15;
    u8 unk_16;
    u8 unk_17;
    u8 unk_18;
    u8 unk_19;
    u8 unk_1A;
    u8 unk_1B_0 : 1;
    u8 unk_1B_1 : 7;
    u16 unk_1C;
    u8 unk_1E;
    u8 unk_1F;
    union {
        MailMessage unk_20_val1;
        u16 unk_20_val2[40];
    };
    u8 unk_70[12];
    UnkStruct_ov62_02239DA4_sub1 unk_7C;
} UnkStruct_ov61_0222AFC0;

typedef struct SaveData SaveData;

typedef struct MsgData MsgData;

typedef struct PokepicManager PokepicManager;

typedef struct MessageFormat MessageFormat;

typedef struct {
    int unk_00;
    int unk_04;
    u64 unk_08;
} UnkStruct_ov62_02249380;

typedef struct {
    const UnkStruct_ov62_02249380 *unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    int unk_1C;
    int unk_20;
    u32 unk_24;
    const TouchscreenHitbox *unk_28;
} UnkStruct_ov62_02248CDC;

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

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

typedef void (*TouchScreenHitTableAction)(u32, enum TouchScreenButtonState, void *);

typedef struct TouchHitboxController TouchHitboxController;

TouchHitboxController *TouchHitboxController_Create(const TouchscreenHitbox *hitTable, u32 hitTableLength, TouchScreenHitTableAction action, void *context, u32 heapID);

void TouchHitboxController_Destroy(TouchHitboxController *buttonAction);

void TouchHitboxController_IsTriggered(TouchHitboxController *buttonAction);

typedef struct {
    BgConfig *unk_00;
    int unk_04;
    int unk_08;
    enum HeapID heapID;
} UnkStruct_ov22_0225AF8C;

typedef struct UnkStruct_ov22_0225B1BC_t UnkStruct_ov22_0225B1BC;

typedef struct {
    UnkStruct_ov61_0222AFC0 unk_00;
    UnkStruct_ov61_0222AE80 unk_80;
} UnkStruct_ov61_0222BC90;

extern const TouchscreenHitbox ov40_02245708[9];

void ov40_0222BF80(UnkStruct_0208C06C *param0, int param1);

BOOL ov40_0222DA00(int *param0, int *param1, int param2, int param3);

BOOL ov40_0222DA84(int *param0, int param1);

typedef struct {
    UnkStruct_02030A80 *unk_00;
    ManagedSprite *unk_04;
    ManagedSprite *unk_08;
    Window unk_0C[8];
} UnkStruct_ov62_022323B8;

void ov40_0222DED0(UnkStruct_0208C06C *param0, int param1);

void ov40_0222DFB0(UnkStruct_0208C06C *param0);

void ov40_022307DC(UnkStruct_0208C06C *param0, int param1, int param2);

void ov40_02230944(UnkStruct_0208C06C *param0);

typedef struct {
    int unk_00;
    int unk_04;
    u16 *unk_08;
    MsgData *unk_0C;
    BOOL unk_10;
} UnkStruct_ov62_0223E01C_sub1;

void GfGfx_EngineATogglePlanes(u8 layerMask, u8 enable);

void GfGfx_EngineBTogglePlanes(u8 layerMask, u8 enable);

typedef struct {
    int unk_00;
    int unk_04;
    DressUpPhoto *photo;
    UnkStruct_ov61_0222BC90 *unk_0C[5];
} UnkStruct_ov62_02237D24_sub1;

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    Window unk_10[12];
    Window unk_D0[12];
    int unk_190;
    int unk_194;
    UnkStruct_ov62_022312B0 unk_198[2];
    TouchHitboxController *unk_1D0[1];
    UnkStruct_ov62_0223E01C_sub1 unk_1D4;
    UnkStruct_ov62_02249380 *unk_1E8;
    UnkStruct_ov62_02248CDC unk_1EC;
    UnkStruct_ov22_0225AF8C unk_218;
    UnkStruct_ov22_0225B1BC *unk_228;
    DressUpPhoto *photo;
    UnkStruct_ov62_02237D24_sub1 unk_230;
    UnkStruct_ov62_022323B8 unk_250;
    UnkStruct_02030A80 *unk_2DC;
    int unk_2E0;
    int unk_2E4;
} UnkStruct_ov62_02237D24;

void ov40_02235FD0(u32 param0, enum TouchScreenButtonState param1, void *param2);

void ov40_02235FFC(UnkStruct_0208C06C *param0);

void ov40_0223610C(UnkStruct_0208C06C *param0);

extern const TouchscreenHitbox ov40_022451C4[1];

BOOL ov40_02234A38(UnkStruct_0208C06C *param0);

BOOL ov40_02234A38(UnkStruct_0208C06C *param0) {
    UnkStruct_ov62_02237D24 *v0 = param0->unk_860;

    switch (param0->unk_08) {
    case 0:
        ov40_022307DC(param0, 60, 7);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, 1);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, 1);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, 1);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, 1);
        v0->unk_1D0[0] = TouchHitboxController_Create(ov40_02245708, NELEMS(ov40_02245708), ov40_02235FD0, param0, HEAP_ID_109);
        param0->unk_08++;
        break;
    case 1:
        ov40_0222DA84(&v0->unk_08, 0);

        if (ov40_0222DA00(&v0->unk_00, &v0->unk_04, 0, 0)) {
            ov40_0222DED0(param0, 114);
            ov40_02235FFC(param0);
            param0->unk_08++;
        }

        PaletteData_BlendPalettes(param0->unk_14.unk_14, PLTTBUF_SUB_OBJ, 0xC, v0->unk_08, param0->unk_14.unk_44);
        break;
    case 2:
        TouchHitboxController_IsTriggered(v0->unk_1D0[0]);

        if (TouchscreenHitbox_TouchNewIsIn(&ov40_022451C4[0])
            || (v0->unk_1D4.unk_10 == 1)) {
            ov40_02230944(param0);
            param0->unk_08++;
        }
        break;
    case 3:
        ov40_0223610C(param0);
        TouchHitboxController_Destroy(v0->unk_1D0[0]);
        ov40_0222DFB0(param0);
        param0->unk_08++;
    case 4:
        if (v0->unk_1D4.unk_10 == 1) {
            if (ov40_0222DA00(&v0->unk_00, &v0->unk_04, 1, 2)) {
                param0->unk_08++;
            }
        } else {
            ov40_0222DA84(&v0->unk_08, 1);

            if (ov40_0222DA00(&v0->unk_00, &v0->unk_04, 1, 2)) {
                param0->unk_08++;
            }

            PaletteData_BlendPalettes(param0->unk_14.unk_14, PLTTBUF_SUB_OBJ, 0xC, v0->unk_08, param0->unk_14.unk_44);
        }
        break;
    default:
        if (v0->unk_1D4.unk_10 == 1) {
            ov40_0222BF80(param0, 4);
        } else {
            ov40_0222BF80(param0, 5);
        }

        break;
    }

    return 0;
}
