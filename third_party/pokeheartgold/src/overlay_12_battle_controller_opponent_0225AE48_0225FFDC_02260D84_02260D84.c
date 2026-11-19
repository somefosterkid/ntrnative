#include "global.h"

#include "battle/battle.h"
#include "battle/battle_controller.h"
#include "battle/battle_system.h"

#include "bg_window.h"
#include "heap.h"
#include "palette.h"
#include "pokepic.h"
#include "sound.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "unk_020163E0.h"

typedef struct UnkStruct_ov12_02260D84_arg1 {
    BattleSystem *unk_0;
    u8 unk_4;
    u8 unk_5;
    u8 unk_6;
    u8 unk_7;
    u8 unk_8;
} UnkStruct_ov12_02260D84_arg1;

void ov12_02260D84(SysTask *arg0, void *param);

void ov12_02260D84(SysTask *arg0, void *param) {
    UnkStruct_ov12_02260D84_arg1 *arg1 = param;
    PaletteData *temp_r4;
    PokepicManager *temp_r7;
    u8 temp_r0;
    u8 temp_r0_2;
    u8 temp_r1;
    temp_r4 = BattleSystem_GetPaletteData(arg1->unk_0);
    temp_r7 = BattleSystem_GetPokepicManager(arg1->unk_0);
    temp_r0 = arg1->unk_6;
    switch (temp_r0) {
    case 0:
        if ((arg1->unk_7) == 0) {
            sub_0201649C(BattleSystem_GetMessageIcon(arg1->unk_0), 1);
            Pokepic_StartPaletteFadeAll(temp_r7, 0, 0x10, 0, 0xFFFF);
        } else {
            GF_SndStartFadeOutBGM(0U, 0x10U);
        }
        arg1->unk_6 = (u8)((arg1->unk_6) + 1);

    case 1:
        temp_r0_2 = arg1->unk_8;
        if ((arg1->unk_7) == 0) {
            PaletteData_BlendPalette(temp_r4, PLTTBUF_MAIN_BG, 0U, 0xA0U, (u8)((s32)(arg1->unk_8)), 0xFFFFU);
            PaletteData_BlendPalette(temp_r4, PLTTBUF_MAIN_BG, 0xC0U, 0x40U, (u8)((s32)(arg1->unk_8)), 0xFFFFU);
            PaletteData_BlendPalette(temp_r4, PLTTBUF_MAIN_OBJ, 0U, 0xE0U, (u8)((s32)(arg1->unk_8)), 0xFFFFU);
        } else {
            PaletteData_BlendPalette(temp_r4, PLTTBUF_MAIN_BG, 0xA0U, 0x20U, (u8)((s32)(arg1->unk_8)), 0xFFFFU);
            PaletteData_BlendPalette(temp_r4, PLTTBUF_SUB_BG, 0U, 0x100U, (u8)((s32)(arg1->unk_8)), 0xFFFFU);
            PaletteData_BlendPalette(temp_r4, PLTTBUF_SUB_OBJ, 0U, 0x100U, (u8)((s32)(arg1->unk_8)), 0xFFFFU);
        }
        temp_r1 = arg1->unk_8;
        arg1->unk_8 = (u8)(temp_r1 + 1);
        if (temp_r1 == 0x10) {
            if ((arg1->unk_7) == 0) {
                ToggleBgLayer(0U, 0U);
            }
            arg1->unk_6 = (u8)((arg1->unk_6) + 1);
            return;
        }
        return;

    case 2:
        BattleController_EmitClearCommand(arg1->unk_0, (s32)(arg1->unk_5), (s32)(arg1->unk_4));
        Heap_Free(arg1);
        SysTask_Destroy(arg0);
        break;
    }
}
