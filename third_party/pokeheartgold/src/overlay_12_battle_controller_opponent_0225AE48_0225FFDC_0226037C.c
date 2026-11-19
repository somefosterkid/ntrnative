#include "global.h"

#include "battle/battle_controller.h"
#include "battle/battle_system.h"

#include "heap.h"
#include "palette.h"
#include "pokepic.h"
#include "sound.h"
#include "sys_task_api.h"
#include "unk_020163E0.h"
#include "overlay_12_fade_out_data.h"

void ov12_0226037C(SysTask *arg0, void *arg1);

void ov12_0226037C(SysTask *task, void *ptr) {
    PaletteData *data;
    PokepicManager *pokepicManager;
    u8 v0;

    data = BattleSystem_GetPaletteData(*(BattleSystem **)ptr);
    pokepicManager = BattleSystem_GetPokepicManager(*(BattleSystem **)ptr);
    v0 = ((FadeOutData *)ptr)->state;
    switch (v0) {
    case 0:
        sub_0201649C(BattleSystem_GetMessageIcon(*(BattleSystem **)ptr), 1);
        PaletteData_BeginPaletteFade(data, 0xFU, 0xFFFFU, 1, 0U, 0x10U, 0U);
        Pokepic_StartPaletteFadeAll(pokepicManager, 0, 0x10, 0, 0);
        GF_SndStartFadeOutBGM(0U, 0x10U);
        ((FadeOutData *)ptr)->state = (u8)(((FadeOutData *)ptr)->state + 1);
    case 1:
        if (PaletteData_GetSelectedBuffersBitmask(data) == 0) {
            ((FadeOutData *)ptr)->state = (u8)(((FadeOutData *)ptr)->state + 1);
            return;
        }
        return;
    case 2:
        BattleController_EmitClearCommand(*(BattleSystem **)ptr, (s32) ((FadeOutData *)ptr)->battler, (s32) ((FadeOutData *)ptr)->command);
        Heap_Free(ptr);
        SysTask_Destroy(task);
        break;
    }
}
