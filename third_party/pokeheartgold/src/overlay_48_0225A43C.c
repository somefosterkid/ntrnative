#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "pm_string.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "unk_0200A090.h"
#include "overlay_48_unk_struct_ov69_0225_d35_c.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"

struct G3DPipelineBuffers;
struct SpriteList;
struct SpriteResourceCollection;
typedef struct G3DPipelineBuffers G3DPipelineBuffers;
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
static void ov48_0225A4B4(UnkStruct_ov69_0225E084 *param0, u32 param1);
static BOOL ov48_0225A4C0(UnkStruct_ov69_0225E084 *param0, u32 param1, UnkStruct_ov69_0225D35C *param2);
void ov48_0225A668(UnkStruct_ov69_0225E478 *param0, s32 param1, s32 param2);
void ov48_0225AD38(UnkStruct_ov69_0225EB60 *param0);
static void ov48_0225A57C(UnkStruct_ov69_0225E084 *param0, u32 param1, UnkStruct_ov69_0225D35C *param2);
u8 *ov48_0225A834(UnkStruct_ov69_0225E084 *param0, u32 param1, u32 param2);
u8 *ov48_0225A858(UnkStruct_ov69_0225E084 *param0, u32 param1);
void ov48_0225A868(UnkStruct_ov69_0225E084 *param0, u32 param1, u32 param2, const u8 *param3);
void ov48_0225AA5C(UnkStruct_ov69_0225E7DC *param0, u32 param1, UnkStruct_ov69_0225D35C *param2);
void ov48_0225A650(UnkStruct_ov69_0225E478 *param0, s32 param1, s32 param2);
void ov48_0225A680(SysTask *param0, void *param1);

BOOL ov48_0225A43C(UnkStruct_ov69_0225E084 *param0, UnkStruct_ov69_0225D35C *param1, int param2);
static void ov48_0225A4B4(UnkStruct_ov69_0225E084 *param0, u32 param1);
static BOOL ov48_0225A4C0(UnkStruct_ov69_0225E084 *param0, u32 param1, UnkStruct_ov69_0225D35C *param2);
static void ov48_0225A57C(UnkStruct_ov69_0225E084 *param0, u32 param1, UnkStruct_ov69_0225D35C *param2);
void ov48_0225A5C4(UnkStruct_ov69_0225E478 *param0);

void ov48_0225A634(void *arg0);

BOOL ov48_0225A43C(UnkStruct_ov69_0225E084 *param0, UnkStruct_ov69_0225D35C *param1, int param2) {
    u32 v0;
    int v1;
    BOOL v2;
    BOOL v3 = 1;

    if ((param0->unk_3C % param2) == 0) {
        v0 = param0->unk_3C / param2;
        ov48_0225A4B4(param0, v0);
    }

    if ((param0->unk_3C + 1) < ((3 * 6) * param2)) {
        param0->unk_3C++;
    }

    for (v1 = 0; v1 < param0->unk_3E; v1++) {
        v2 = ov48_0225A4C0(param0, v1, param1);

        if (v2 == 0) {
            v3 = 0;
        }
    }

    if (v3 == 1) {
        ov48_0225AD38(&param0->unk_1B8);
        ov48_0225A668(&param0->unk_2E4, 0, 0);
    }

    return v3;
}

static void ov48_0225A4B4(UnkStruct_ov69_0225E084 *param0, u32 param1) {
    param0->unk_40[param1].unk_00 = 1;
}

static BOOL ov48_0225A4C0(UnkStruct_ov69_0225E084 *param0, u32 param1, UnkStruct_ov69_0225D35C *param2) {
    u32 v0;
    u32 v1;
    u32 v2;
    u32 v3;

    if (param0->unk_40[param1].unk_00 == 0) {
        return 0;
    }

    if (param0->unk_40[param1].unk_01 >= 2) {
        return 1;
    }

    v0 = ((3 * 6) - param1) - 1;
    v2 = v0 / 6;
    v1 = v0 % 6;
    v3 = param0->unk_88[v2];

    switch (param0->unk_40[param1].unk_01) {
    case 0: {
        u8 *v4;

        v4 = ov48_0225A858(param0, v1);
        ov48_0225A868(param0, v2, v1, v4);
    }

        {
            ov48_0225A57C(param0, v0, param2);
        }

        {
            ov48_0225A668(&param0->unk_2E4, (s16)(v0 * 8), (s16)((3 * 6) * 8));
        }
        break;
    case 1: {
        u8 *v5;

        v5 = ov48_0225A834(param0, v2, v1);
        ov48_0225A868(param0, v2, v1, v5);
    }
        {
            ov48_0225AA5C(&param0->unk_A0[v3], v0, param2);
        }
        break;
    default:
        break;
    }

    param0->unk_40[param1].unk_01++;
    return 0;
}

static void ov48_0225A57C(UnkStruct_ov69_0225E084 *param0, u32 param1, UnkStruct_ov69_0225D35C *param2) {
    CopyToBgTilemapRect(param2->unk_00, 6, 0, 0 + param1, 32, 1, param0->unk_98->rawData, 0, 0 + param1, param0->unk_98->screenWidth / 8, param0->unk_98->screenHeight / 8);
    ScheduleBgTilemapBufferTransfer(param2->unk_00, 6);
}

void ov48_0225A5C4(UnkStruct_ov69_0225E478 *param0) {
    GXS_SetVisibleWnd(GX_WNDMASK_W0 | GX_WNDMASK_W1);
    G2S_SetWndOutsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ, 0);
    G2S_SetWnd0InsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3, 0);
    G2S_SetWnd1InsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3, 0);

    ov48_0225A650(param0, 0, 0);
    ov48_0225A668(param0, 0, 0);

    param0->unk_0C = SysTask_CreateOnVWaitQueue(ov48_0225A680, param0, 0);
}

void ov48_0225A634(void *arg0) {
    SysTask_Destroy(*(SysTask **)((u8 *)arg0 + 0xC));
    reg_GXS_DB_DISPCNT &= 0xFFFF1FFF;
}
