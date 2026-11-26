#include "global.h"

#include "constants/heap.h"

#include "error_handling.h"
#include "game_stats.h"
#include "gf_rtc.h"
#include "overlay_manager.h"
#include "save.h"
#include "screen_fade.h"
#include "touchscreen.h"
#include "unk_02037C94.h"
#include "yes_no_prompt.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "overlay_45_wifi_plaza_system.h"

s32 ov45_0222A5C0(u32);

void ov45_0222B244(void *);

void ov45_0222B270(void *);

u32 ov45_0222D7CC(u32, u32);

s32 ov45_0222D7FC(u32, u32);

s32 ov45_0222E7CC(void);

s32 ov45_0222E7FC(s32 error);

void ov46_02259374(u32 *, u32);

void ov46_022593F8(u32 *);

void ov46_02259450(u32 *);

void ov46_02259474(u32 *);

void ov46_02259534(u32 *, u32);

u32 ov46_02259550(u32, u32);

u32 ov46_0225892C(OverlayManager *param_1, s32 *param_2);

u32 ov46_0225892C(OverlayManager *param_1, s32 *param_2) {
    u8 uVar1;
    u32 *puVar2;
    void *pvVar3;
    s32 arg1;
    GameStats *gameStats;
    u32 *puVar4;
    s32 iVar5;
    u32 uVar6;
    YesNoResponse YVar7;
    BOOL BVar8;
    u32 uVar9;
    s64 sVar10;
    u32 uStack_1c;

    puVar2 = OverlayManager_GetData(param_1);
    pvVar3 = OverlayManager_GetArgs(param_1);
    switch (*param_2) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0xffff, 6, 1, HEAP_ID_119);
        *param_2 = *param_2 + 1;
        break;
    case 1:
        BVar8 = IsPaletteFadeFinished();
        if (BVar8 != 0) {
            *param_2 = *param_2 + 1;
        }
        break;
    case 2:
        if (*(s32 *)((u8 *)pvVar3 + 8) == 0) {
            ov46_02259374(puVar2 + 0x1c, 0x11);
            uVar6 = ov46_02259550(puVar2[3], 0x230);
            puVar2[0x34] = uVar6;
            *param_2 = *param_2 + 1;
        } else {
            *param_2 = 4;
        }
        break;
    case 3:
        YVar7 = YesNoPrompt_HandleInput((YesNoPrompt *)puVar2[0x34]);
        if (YVar7 == YESNORESPONSE_YES) {
            YesNoPrompt_Destroy((YesNoPrompt *)puVar2[0x34]);
            *param_2 = 4;
        } else if (YVar7 == YESNORESPONSE_NO) {
            YesNoPrompt_Destroy((YesNoPrompt *)puVar2[0x34]);
            ov46_022593F8(puVar2 + 0x10);
            *param_2 = 7;
        }
        break;
    case 4:
        arg1 = ov45_0222A5C0(*(u32 *)((u8 *)pvVar3 + 4));
        sub_0203976C((SaveData *)*puVar2, arg1);
        ov46_02259374(puVar2 + 0x1c, 0x17);
        ov46_02259450(puVar2 + 0x1c);
        *param_2 = *param_2 + 1;
        break;
    case 5:
        BVar8 = sub_020393C8();
        if ((BVar8 != 0) || (BVar8 = sub_020397FC(), BVar8 != 0)) {
            ov46_02259474(puVar2 + 0x1c);
            *param_2 = 9;
        }
        uVar1 = sub_020397E4();
        if (uVar1 == 1) {
            ov45_0222B244(*(void **)((u8 *)pvVar3 + 4));
            *param_2 = *param_2 + 1;
        }
        break;
    case 6:
        BVar8 = sub_020393C8();
        if ((BVar8 != 0) || (BVar8 = sub_020397FC(), BVar8 != 0)) {
            ov46_02259474(puVar2 + 0x1c);
            *param_2 = 9;
        }
        BVar8 = sub_02039274();
        if (BVar8 != 0) {
            ov46_02259474(puVar2 + 0x1c);
            gameStats = Save_GameStats_Get((SaveData *)*puVar2);
            GameStats_AddScore(gameStats, 0x21);
            sVar10 = GF_RTC_DateTimeToSec();
            **(s64 **)((u8 *)pvVar3 + 0xc) = sVar10;
            *(u32 *)((u8 *)pvVar3 + 0x10) = 1;
            *param_2 = *param_2 + 1;
        }
        break;
    case 9:
        BVar8 = sub_020393C8();
        if (BVar8 != 0) {
            puVar4 = sub_020392D8();
            uStack_1c = ov45_0222D7CC(*puVar4, puVar4[1]);
            uVar9 = *puVar4;
        } else {
            uVar9 = ov45_0222E7FC(ov45_0222E7CC());
            uStack_1c = 0x20;
        }
        ov45_0222B270(*(void **)((u8 *)pvVar3 + 4));
        ov46_022593F8(puVar2 + 0x10);
        ov46_022593F8(puVar2 + 0x1c);
        ov46_02259534(puVar2 + 0x28, uVar9);
        ov46_02259374(puVar2 + 0x28, uStack_1c);
        *param_2 = *param_2 + 1;
        break;
    case 10:
        if (((((u32 *)&gSystem)[0x12] & 3) != 0) || (BVar8 = System_GetTouchNew(), BVar8 == 1)) {
            BVar8 = sub_020393C8();
            if (BVar8 != 0) {
                puVar4 = sub_020392D8();
                iVar5 = ov45_0222D7FC(*puVar4, puVar4[1]);
                if (iVar5 == 0) {
                    *param_2 = 0xb;
                } else {
                    *param_2 = 0xe;
                }
            } else {
                *param_2 = 0xb;
            }
        }
        break;
    case 0xb:
        ov46_022593F8(puVar2 + 0x28);
        ov46_02259374(puVar2 + 0x10, 0x58);
        uVar6 = ov46_02259550(puVar2[3], 0x230);
        puVar2[0x34] = uVar6;
        *param_2 = *param_2 + 1;
        break;
    case 0xc:
        YVar7 = YesNoPrompt_HandleInput((YesNoPrompt *)puVar2[0x34]);
        if (YVar7 == YESNORESPONSE_YES) {
            YesNoPrompt_Destroy((YesNoPrompt *)puVar2[0x34]);
            sub_020397C8();
            *param_2 = 0xd;
        } else if (YVar7 == YESNORESPONSE_NO) {
            YesNoPrompt_Destroy((YesNoPrompt *)puVar2[0x34]);
            *param_2 = 0xe;
        }
        break;
    case 0xd:
        BVar8 = sub_02037D78();
        if (BVar8 == 0) {
            *param_2 = 4;
        }
        break;
    case 0xe:
        ov46_022593F8(puVar2 + 0x10);
        ov46_022593F8(puVar2 + 0x1c);
        ov46_022593F8(puVar2 + 0x28);
        sub_020397C8();
        *param_2 = *param_2 + 1;
        break;
    case 0xf:
        BVar8 = sub_02037D78();
        if (BVar8 == 0) {
            ov46_022593F8(puVar2 + 0x10);
            ov46_022593F8(puVar2 + 0x1c);
            *param_2 = 7;
        }
        break;
    case 7:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0, 6, 1, HEAP_ID_119);
        *param_2 = *param_2 + 1;
        break;
    case 8:
        BVar8 = IsPaletteFadeFinished();
        if (BVar8 != 0) {
            return 1;
        }
        break;
    default:
        GF_AssertFail();
    }
    return 0;
}
