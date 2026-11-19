#include "global.h"

#include "constants/heap.h"

#include "struct_defs/resdat_narc_structs.h"

#include "field/overlay_01_021FB878.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "font.h"
#include "gf_3d_loader.h"
#include "gf_3d_vramman.h"
#include "message_format.h"
#include "msgdata.h"
#include "obj_char_transfer.h"
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
#include "gf_gfx_planes.h"

struct TextOBJ;

struct UnkStruct_02012744_t;

struct UnkStruct_ov113_0225DBCC_t;

struct UnkStruct_ov113_0225E6B8_t;

struct UnkStruct_ov66_0222DFF8_t;

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

typedef u32 TextColor;

void TextOBJ_SetSpritesDrawFlag(TextOBJ *param0, BOOL param1);

enum YesNoTouchButton {
    BUTTON_YES = 0,
    BUTTON_NO,
    BUTTON_COUNT,
    BUTTON_NONE,
};

typedef struct YesNoButtonAnimData {
    const fx32 *frameThresholds;
    u32 frameCount;
    fx32 frameProgress;
} YesNoButtonAnimData;

typedef struct YesNoButtonSprite {
    BgConfig *bgConfig;
    enum GFBgLayer bgLayer;
    void *nscrBuffers[4];
    NNSG2dScreenData *screenData[4];
    YesNoButtonAnimData animData;
    u8 tilemapLeft;
    u8 tilemapTop;
    u8 animFrame;
} YesNoButtonSprite;

typedef struct YesNoPrompt {
    TouchHitboxController *buttonAction;
    TouchscreenHitbox hitTable[BUTTON_COUNT];
    YesNoButtonSprite buttonSprites[BUTTON_COUNT];
    BgConfig *bgConfig;
    u32 bgLayer;
    enum HeapID heapID;
    fx32 frameThresholds[4];
    u8 tilemapLeft;
    u8 tilemapTop;
    u8 pressedButton;
    u8 paramsApplied : 4;
    u8 buttonState : 4;
} YesNoPrompt;

typedef struct {
    TextOBJ *unk_00;
    UnkStruct_02021AC8 unk_04;
    u16 unk_10;
} UnkStruct_ov113_0225E250;

struct UnkStruct_ov113_0225DBCC_t {
    UnkStruct_ov66_0223127C *unk_00;
    SaveData *saveData;
    BgConfig *unk_08;
    PaletteData *unk_0C;
    UnkStruct_02012744 *unk_10;
    GF3DVramMan *unk_14;
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

void ov89_0225A260(UnkStruct_ov113_0225DBCC *param0, UnkStruct_ov113_0225E250 *param1, const String *param2, enum Font param3, TextColor param4, int param5, int param6, int param7, int param8, int param9);

extern const ManagedSpriteTemplate ov89_0225CB08;

extern const ManagedSpriteTemplate ov89_0225CB3C;

extern const ManagedSpriteTemplate ov89_0225CB70;

void ov89_02259588(UnkStruct_ov113_0225DBCC *param0);

void ov89_02259588(UnkStruct_ov113_0225DBCC *param0) {
    int v0;
    ManagedSpriteTemplate v1;

    v1 = ov89_0225CB08;

    for (v0 = 0; v0 < 6; v0++) {
        v1.x = 16 + 32 * v0;
        v1.y = 176;

        param0->unk_924[v0] = SpriteSystem_NewSprite(param0->unk_1C, param0->unk_20, &v1);

        ManagedSprite_SetAnim(param0->unk_924[v0], v0);
        Sprite_TickFrame(param0->unk_924[v0]->sprite);
    }

    v1 = ov89_0225CB3C;

    for (v0 = 0; v0 < 6; v0++) {
        v1.x = 16 + 32 * v0;
        v1.y = 176;

        param0->unk_93C[v0] = SpriteSystem_NewSprite(param0->unk_1C, param0->unk_20, &v1);

        ManagedSprite_SetAnim(param0->unk_93C[v0], v0);
        Sprite_TickFrame(param0->unk_93C[v0]->sprite);
    }

    v1 = ov89_0225CB70;

    for (v0 = 0; v0 < 6; v0++) {
        v1.x = 16 + 32 * v0;
        v1.y = 176;
        v1.resIdList[0] = 10001 + v0;

        param0->unk_954[v0] = SpriteSystem_NewSprite(param0->unk_1C, param0->unk_20, &v1);
        Sprite_TickFrame(param0->unk_954[v0]->sprite);
    }

    {
        String *v2;

        v2 = NewString_ReadMsgData(param0->unk_30, 1);
        ov89_0225A260(param0, &param0->unk_19C0, v2, FONT_SUBSCREEN, ((TextColor)(((1 & 0xFF) << 16) | ((2 & 0xFF) << 8) | ((3 & 0xFF) << 0))), 0, 10003, 0x1c * 8, 176, 1);
        TextOBJ_SetSpritesDrawFlag(param0->unk_19C0.unk_00, 1);
        String_Delete(v2);
    }
}
