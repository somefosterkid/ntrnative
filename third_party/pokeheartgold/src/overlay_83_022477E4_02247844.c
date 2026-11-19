#include "global.h"

#include "yes_no_prompt.h"
#include "overlay_83_battle_castle_yes_no_prompt.h"

void ov83_02247844(BattleCastleYesNoPrompt *yesNo);

void ov83_02247844(BattleCastleYesNoPrompt *yesNo) {
    yesNo->prompt = YesNoPrompt_Create(HEAP_ID_107);
    yesNo->isActive = FALSE;
}
