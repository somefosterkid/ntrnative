#include "global.h"

#include "bg_window.h"
#include "pm_string.h"
#include "sprite.h"
#include "sys_task.h"
#include "unk_0200B150.h"
#include "overlay_70_gtsapplication_state.h"
#include "overlay_71_trade_sequence_data.h"
#include "msgdata.h"

struct MsgData;

struct MessageFormat;

struct TradeAnimationTemplate;

void ov71_02246C48(SysTask *unused, void *param);

void ov71_02246C48(SysTask *unused, void *param) {
    TradeSequenceData *sequenceData = param;

    SpriteList_RenderAndAnimateSprites(sequenceData->spriteList);
    OamManager_ApplyAndResetBuffers();

    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}
