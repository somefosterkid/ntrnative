#include "global.h"
#include "constants/heap.h"
#include "sprite.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "save.h"
#include "pm_string.h"
#include "camera.h"
#include "sys_task.h"
#include "options.h"
#include "screen_fade.h"
#include "screen_fade_internal.h"
#include "gf_gfx_planes.h"
#include "palette.h"
#include "font.h"
#include "overlay_manager.h"
#include "unk_02013534.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"

struct OverlayManager;

struct BgConfig;

struct Camera;

struct CameraAngle;

struct CharTransferAllocation;

struct Field3dModel;

struct Field3dObject;

struct TextOBJ;

struct G3DPipelineBuffers;

struct ManagedSprite;

struct MsgData;

struct NARC;

struct Options;

struct PaletteData;

struct SaveData;

struct SpriteManager;

struct SpriteSystem;

struct String;

struct MessageFormat;

struct SysTask;

struct UnkStruct_ov113_0225DBCC_t;

struct UnkStruct_ov113_0225E6B8_t;

struct Window;

struct YesNoPrompt;

struct YesNoPromptTemplate;

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

void *OverlayManager_GetData(OverlayManager *appMan);

BOOL ov89_02259EC4(UnkStruct_ov113_0225DBCC *param0, const UnkStruct_ov113_02260818 *param1, s32 param2);

void FillWindowPixelBuffer(Window *window, u8 val);

BOOL ov45_0222A330(const WiFiPlazaSystem *param0);

void ov45_0222A4A8(WiFiPlazaSystem *param0);

BOOL ov45_0222D844(void);

void ov45_0222EEB8(void);

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

void ov89_0225A4F4(UnkStruct_ov113_0225EB20 *param0, Camera *camera, int param2, int param3);

void PaletteData_LoadPaletteSlotFromHardware(PaletteData *paletteData, enum PaletteBufferId bufferID, u16 start, u32 size);

void PaletteData_BlendPalette(PaletteData *paletteData, enum PaletteBufferId bufferID, u16 index, u16 size, u8 fraction, u16 target);

typedef struct CharTransferAllocation {
    u32 size;
    u32 offset;
    u16 vramType;
    u16 atEnd;
} CharTransferAllocation;

int ov89_0225C84C(UnkStruct_ov113_02260818 *param0, int param1);

void ov89_0225C8DC(UnkStruct_ov113_0225DBCC *param0);

BOOL ov89_0225C91C(const UnkStruct_ov113_02260818 *param0);

typedef struct {
    UnkStruct_ov113_02260818 unk_00;
} UnkStruct_ov113_0225CA04;

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

void DrawFrameAndWindow2(Window *window, s32 skipTransfer, u16 tile, u8 palette);

void ClearFrameAndWindow2(Window *window, u8 skipTransfer);

Options *Save_PlayerData_GetOptionsAddr(SaveData *saveData);

void BeginNormalPaletteFade(enum FadeMode mode, enum FadeType typeMain, enum FadeType typeSub, u16 color, int steps, int framesPerStep, enum HeapID heapID);

BOOL IsPaletteFadeFinished(void);

void sub_0200FB70(void);

BOOL PlaySE(u16 seqID);

typedef u32 TextColor;

u8 TextPrinterCheckActive(u8 printerID);

u8 AddTextPrinterParameterized(Window *window, u32 fontID, const String *string, u32 xOffset, u32 yOffset, u32 renderDelay, PrinterCallback_t callback);

typedef struct YesNoPrompt YesNoPrompt;

typedef struct YesNoPromptTemplate {
    BgConfig *bgConfig;
    enum GFBgLayer bgLayer;
    u32 baseTile;
    u32 palette;
    u8 tilemapLeft;
    u8 tilemapTop;
    u8 unk_12; /* HGSS layout */
    u8 unk_13;
} YesNoPromptTemplate;

void YesNoPrompt_InitFromTemplate(YesNoPrompt *menu, const YesNoPromptTemplate *params);

u32 YesNoPrompt_HandleInput(YesNoPrompt *menu);

void YesNoPrompt_Reset(YesNoPrompt *menu);

typedef struct {
    Field3dModel unk_00;
    Field3dObject unk_10;
} UnkStruct_ov113_0225DBCC_sub1;

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
    UnkStruct_ov113_0225DBCC_sub1 unk_D8;
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

void ov89_0225A468(UnkStruct_ov113_0225DBCC *param0);

void ov89_02259E28(UnkStruct_ov113_0225DBCC *param0, UnkStruct_ov113_0225CA04 *param1);

void ov89_0225A16C(UnkStruct_ov113_0225DBCC *param0, int param1);

void ov89_0225A21C(UnkStruct_ov113_0225DBCC *param0);

int ov89_02259F9C(UnkStruct_ov113_0225DBCC *param0);

void ov89_0225A398(UnkStruct_ov113_0225DBCC *param0);

BOOL ov89_0225A41C(UnkStruct_ov113_0225DBCC *param0, int param1);

void ov113_0225E15C(UnkStruct_ov113_0225DBCC *param0, UnkStruct_ov113_0225E250 *param1, const String *param2, enum Font param3, TextColor param4, int param5, int param6, int param7, int param8, int param9);

int ov89_02258B04(OverlayManager *appMan, int *param1, s32 arg2);

int ov89_02258B04(OverlayManager *appMan, int *param1, s32 arg2) {
    UnkStruct_ov113_0225DBCC *v0 = OverlayManager_GetData(appMan);

    ov89_0225A4F4(&v0->unk_194, v0->camera, v0->unk_9BC, v0->unk_00->unk_04);

    switch (*param1) {
    case 0:
        v0->unk_168.unk_00 = 1;
        ov89_0225C8DC(v0);

        {
            UnkStruct_ov113_0225CA04 v1;
            ov89_02259E28(v0, &v1);
        }
        (*param1)++;
        break;
    case 1:
        if (IsPaletteFadeFinished() == 1) {
            v0->unk_9BC = 1;
            (*param1)++;
        }
        break;
    case 2:
        if (gSystem.touchNew && (gSystem.touchY < 160)) {
            if (ov89_0225A41C(v0, (32 << 8) / 6) == 1) {
                v0->unk_8D8[v0->unk_920].unk_09 = gSystem.touchX;
                v0->unk_8D8[v0->unk_920].unk_0A = gSystem.touchY;

                ov89_02259EC4(v0, &v0->unk_8D8[v0->unk_920], v0->unk_168.unk_04);
                ov89_0225C91C(&v0->unk_8D8[v0->unk_920]);
            } else {
                (void)0;
            }
        }

        {
            int v2;
            v2 = ov89_0225C84C(v0->unk_8D8, v0->unk_920);

            if (v2 < 6) {
                v0->unk_920 = v2;
                ov89_0225A16C(v0, v2);
            } else if (((*param1) == 2) && (v2 == 0xfe)) {
                PlaySE(1509);
                PaletteData_BlendPalette(v0->unk_0C, PLTTBUF_MAIN_BG, 0 * 16 + 9, 1, 8, 0x0);
                PaletteData_BlendPalette(v0->unk_0C, PLTTBUF_MAIN_OBJ, v0->unk_921 * 16, 16, 8, 0x0);
                *param1 = 3;
            }
        }
        break;
    case 3:
        FillWindowPixelBuffer(&v0->unk_B4, 0xf);
        DrawFrameAndWindow2(&v0->unk_B4, 0, 1, 14);
        ReadMsgDataIntoString(v0->unk_30, 2, v0->unk_C4);
        v0->unk_C8 = AddTextPrinterParameterized(&v0->unk_B4, FONT_MESSAGE, v0->unk_C4, 0, 0, Options_GetTextFrameDelay(Save_PlayerData_GetOptionsAddr(v0->saveData)), 0L);
        (*param1)++;
        break;
    case 4:
        if (TextPrinterCheckActive(v0->unk_C8) == 0) {
            YesNoPromptTemplate v3;

            v3.bgConfig = v0->unk_08;
            v3.bgLayer = GF_BG_LYR_MAIN_1;
            v3.baseTile = (1 + (18 + 12));
            v3.palette = 11;
            v3.tilemapLeft = 25;
            v3.tilemapTop = 6;
            v3.unk_13 = 0;

            YesNoPrompt_InitFromTemplate(v0->unk_24, &v3);

            v0->unk_28 = 1;
            (*param1)++;
        }

        break;
    case 5:
        PaletteData_LoadPaletteSlotFromHardware(v0->unk_0C, PLTTBUF_MAIN_BG, ((11) * (16)), ((16) * sizeof(u16)) * 2);
        {
            u32 v4 = YesNoPrompt_HandleInput(v0->unk_24);

            switch (v4) {
            case 1:
                YesNoPrompt_Reset(v0->unk_24);
                v0->unk_28 = 0;
                ClearFrameAndWindow2(&v0->unk_B4, 0);
                v0->unk_168.unk_00 = 0;
                v0->unk_9BC = 3;
                *param1 = 9;
                break;
            case 2:
                YesNoPrompt_Reset(v0->unk_24);
                v0->unk_28 = 0;
                ClearFrameAndWindow2(&v0->unk_B4, 0);
                PaletteData_BlendPalette(v0->unk_0C, PLTTBUF_MAIN_BG, 0 * 16 + 9, 1, 0, 0x0);
                PaletteData_BlendPalette(v0->unk_0C, PLTTBUF_MAIN_OBJ, v0->unk_921 * 16, 16, 0, 0x0);
                *param1 = 2;
                break;
            }
        }
        break;
    case 6:
        FillWindowPixelBuffer(&v0->unk_B4, 0xf);
        DrawFrameAndWindow2(&v0->unk_B4, 0, 1, 14);
        ReadMsgDataIntoString(v0->unk_30, 3, v0->unk_C4);
        v0->unk_C8 = AddTextPrinterParameterized(&v0->unk_B4, FONT_MESSAGE, v0->unk_C4, 0, 0, Options_GetTextFrameDelay(Save_PlayerData_GetOptionsAddr(v0->saveData)), 0L);
        (*param1)++;
        break;
    case 7:
        if (TextPrinterCheckActive(v0->unk_C8) == 0) {
            (*param1)++;
        }
        break;
    case 8:
        v0->unk_29++;

        if (v0->unk_29 > 90) {
            *param1 = 9;
        }
        break;
    case 9:
        if (IsPaletteFadeFinished() == 0) {
            sub_0200FB70();
        }

        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, (((0) << 10) | ((0) << 5) | (0)), 6, 1, (enum HeapID)125);
        (*param1)++;
        break;
    case 10:
        if (IsPaletteFadeFinished() == 1) {
            (*param1)++;
        }
        break;
    default:
        ov45_0222EEB8();
        return 1;
    }

    ov89_0225A21C(v0);
    ov89_0225A398(v0);

    if (v0->unk_9BC == 1) {
        v0->unk_190++;

        if (v0->unk_190 > 30) {
            v0->unk_190 = 0;
            ov89_02259F9C(v0);
        }

        if (((ov45_0222D844() == 1) || (ov45_0222A330(v0->unk_00->unk_00) == 1)) && ((*param1) != 4)) {
            if (v0->unk_28 == 1) {
                YesNoPrompt_Reset(v0->unk_24);
            }

            v0->unk_168.unk_00 = 0;
            v0->unk_9BC = 3;

            if (ov45_0222D844() == 1) {
                (*param1) = 9;
            } else {
                PlaySE(1521);
                ov45_0222A4A8(v0->unk_00->unk_00);
                (*param1) = 6;
            }
        }
    }

    ov89_0225A468(v0);

    return 0;
}
