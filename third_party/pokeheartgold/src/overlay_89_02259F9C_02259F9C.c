#include "global.h"

#include "constants/heap.h"

#include "struct_defs/resdat_narc_structs.h"

#include "field/overlay_01_021FB878.h"

#include "bg_window.h"
#include "unk_struct_ov66_02232B20.h"
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
#include "unk_02005D10.h"
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

void ov45_0222EC3C(UnkStruct_ov66_02232B20 *param0);

typedef struct {
    WiFiPlazaSystem *unk_00;
    u8 unk_04;
    u8 padding_05[3];
} UnkStruct_ov66_0223127C;

extern void ov89_0225C724(MsgData *param0, MessageFormat *param1, Window param2[], WiFiPlazaSystem *param3, s32 param4);

extern void ov89_0225C818(Window param0[], u32 param1);

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
    struct { struct { struct { fx32 unk_00; s16 unk_04; u8 unk_06; u8 unk_07; } unk_00; struct { struct { fx32 x; fx32 y; fx32 z; } unk_00; u8 unk_0C; u8 unk_0D; u8 unk_0E; u8 unk_0F; } unk_08; struct { struct { u16 x; u16 y; u16 z; u16 padding_06; } cameraAngle; s32 unk_08; s16 unk_0C; u8 unk_0E; u8 unk_0F; } unk_18; struct { struct { u16 x; u16 y; u16 z; u16 padding_06; } cameraAngle; s32 unk_08; s16 unk_0C; u8 unk_0E; u8 unk_0F; } unk_28; struct { struct { u16 x; u16 y; u16 z; u16 padding_06; } cameraAngle; s32 unk_08; s16 unk_0C; u8 unk_0E; u8 unk_0F; } unk_38; struct { struct { u16 x; u16 y; u16 z; u16 padding_06; } cameraAngle; fx32 unk_08; u8 unk_0C; u8 unk_0D; u8 unk_0E[2]; } unk_48; struct { fx32 unk_00; s16 unk_04; u8 unk_06; u8 unk_07; } unk_58; struct { struct { fx32 x; fx32 y; fx32 z; } unk_00; struct { fx32 x; fx32 y; fx32 z; } unk_0C; fx32 unk_18; u8 unk_1C; u8 unk_1D; u8 unk_1E[2]; } unk_60; struct { struct { fx32 x; fx32 y; fx32 z; } unk_00; struct { fx32 x; fx32 y; fx32 z; } unk_0C; fx32 unk_18; u8 unk_1C; u8 unk_1D; u8 unk_1E[2]; } unk_80; struct { struct { u16 x; u16 y; u16 z; u16 padding_06; } cameraAngle; s32 unk_08; s16 unk_0C; u8 unk_0E; u8 unk_0F; } unk_A0; } unk_00; struct { u8 unk_00; u8 unk_01; s16 unk_02; } unk_B0; struct UnkStruct_ov113_0225E6B8_t *unk_B4[128]; struct UnkStruct_ov113_0225E6B8_t *unk_2B4[128]; struct NNSG3dResTex_ *unk_4B4[32]; u32 unk_534[2]; struct { u8 unk_00; u8 unk_01; u16 unk_02; } unk_53C[128]; u8 unk_73C; u8 unk_73D; u8 unk_73E; u8 unk_73F; u8 unk_740; u8 unk_742_pad[1]; s16 unk_742; } unk_194;
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
    struct { struct TextOBJ *unk_00; struct { u32 size; u32 offset; u16 vramType; u16 atEnd; } unk_04; u16 unk_10; u8 unk_12_tail[2]; } unk_19C0;
    int unk_19D4;
    int unk_19D8;
    int unk_19DC;
    BOOL unk_19E0;
};

extern __attribute__((aligned(4))) const s16 ov89_0225CA18[8][2];

extern __attribute__((aligned(4))) const u16 ov89_0225CBD8[8][4];

int ov89_02259F9C(UnkStruct_ov113_0225DBCC *param0);

int ov89_02259F9C(UnkStruct_ov113_0225DBCC *param0) {
    UnkStruct_ov66_02232B20 v0;
    int v1, v2, v3, v4;

    v2 = 0;
    v3 = 0;

    ov45_0222EC3C(&v0);

    for (v1 = 0; v1 < 8; v1++) {
        if (param0->unk_170[v1] != v0.unk_04[v1]) {
            if (v0.unk_04[v1] != -1) {
                ov89_0225C724(param0->unk_30, param0->unk_2C, param0->unk_34, param0->unk_00->unk_00, v0.unk_04[v1]);

                {
                    u16 *v5;
                    int v7;

                    v5 = GetBgTilemapBuffer(param0->unk_08, 6);

                    for (v7 = ov89_0225CBD8[v1][1]; v7 < ov89_0225CBD8[v1][1] + ov89_0225CBD8[v1][3]; v7++) {
                        MI_CpuCopy16(&param0->unk_9C0[v7 * 32 + ov89_0225CBD8[v1][0]], &v5[v7 * 32 + ov89_0225CBD8[v1][0]], ov89_0225CBD8[v1][2] * 2);
                    }

                    ScheduleBgTilemapBufferTransfer(param0->unk_08, 6);
                }

                v2++;
            } else {
                ov89_0225C818(param0->unk_34, v1);

                param0->unk_99C[v1] = 0;
                param0->unk_9AC[v1] = 0;

                ManagedSprite_SetDrawFlag(param0->unk_970[v1], 0);
                FillBgTilemapRect(param0->unk_08, 6, 0, ov89_0225CBD8[v1][0], ov89_0225CBD8[v1][1], ov89_0225CBD8[v1][2], ov89_0225CBD8[v1][3], 16);
                ScheduleBgTilemapBufferTransfer(param0->unk_08, 6);
                v3++;
            }

            if (v0.unk_04[v1] == param0->unk_168.unk_04) {
                ManagedSprite_SetPositionXYWithSubscreenOffset(param0->unk_96C, ov89_0225CA18[v1][0], ov89_0225CA18[v1][1], (192 + 80) << 12);
                ManagedSprite_SetDrawFlag(param0->unk_96C, 1);
            }
        }

        param0->unk_170[v1] = v0.unk_04[v1];
    }

    v4 = 0;

    for (v1 = 0; v1 < 8; v1++) {
        if (param0->unk_170[v1] != -1) {
            v4++;
        }
    }

    param0->unk_194.unk_73F = v4;

    if (v2 > 0) {
        PlaySE(1508);
        return 1;
    } else if (v3 > 0) {
        PlaySE(1508);
        return 2;
    }

    return 0;
}
