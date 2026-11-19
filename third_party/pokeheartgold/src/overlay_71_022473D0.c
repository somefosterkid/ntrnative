#include "global.h"
#include "sys_task.h"
#include "g2d_renderer.h"
#include "constants/heap.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "options.h"
#include "player_data.h"
#include "overlay_70_gtsapplication_state.h"
#include "msgdata.h"
#include "sprite.h"
#include "overlay_71_trade_sequence_data.h"

struct SysTask;

struct SysTaskQueue;

int ov71_022473D0(TradeSequenceData *sequenceData);

s32 ov71_022473DC(void **arg0);
extern s32 _0224C040;
void ov71_022473E4(void);

int ov71_022473D0(TradeSequenceData *sequenceData) {
    return Options_GetFrame(sequenceData->animationTemplate->options);
}

s32 ov71_022473DC(void **arg0) {
    return *(s32 *)((u8 *)(*arg0) + 0x10);
}

void ov71_022473E4(void) {
    _0224C040 = 0;
}
