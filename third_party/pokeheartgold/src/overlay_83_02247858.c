#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "pm_string.h"
#include "sprite.h"
#include "sys_task.h"
#include "unk_0200A090.h"
#include "yes_no_prompt.h"
#include "overlay_83_battle_castle_yes_no_prompt.h"


void ov83_02247858(BattleCastleYesNoPrompt *yesNo);

void ov83_02247858(BattleCastleYesNoPrompt *yesNo) {
    YesNoPrompt_Destroy(yesNo->prompt);
}
