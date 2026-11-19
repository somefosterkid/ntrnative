#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "overlay_71_bg_slide_state.h"
#include "overlay_71_trade_pokemon_reveal_phase.h"

void ov71_0224AF58(SysTask *arg0, void *param);

void ov71_0224AF08(TradePokemonRevealPhase *phase, SysTask **task);

void ov71_0224AF08(TradePokemonRevealPhase *phase, SysTask **task) {
    BgSlideState *data;
    SysTask *pSVar1;

    data = Heap_Alloc(HEAP_ID_57, sizeof(BgSlideState));
    if (data != NULL) {
        data->phase = phase;
        data->bgConfig = phase->bgConfig;
        data->mainBgOffset = -0x180000;
        data->subBgOffset = -0xc0000;
        data->offsetStep = 0x9999;
        data->framesRemaining = 0x28;
        data->taskHandle = task;
        pSVar1 = SysTask_CreateOnVBlankQueue(ov71_0224AF58, data, 0);
        *task = pSVar1;
        if (pSVar1 == NULL) {
            Heap_Free(data);
        }
    }
    return;
}
