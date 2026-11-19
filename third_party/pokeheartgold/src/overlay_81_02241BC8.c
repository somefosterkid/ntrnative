#include "global.h"

#include "sys_task.h"
#include "yes_no_prompt.h"

void YesNoPrompt_Destroy(YesNoPrompt *yesno);

void ov81_02241BC8(YesNoPrompt *yesno);

void ov81_02241BC8(YesNoPrompt *yesno) {
    YesNoPrompt_Destroy(yesno);
}
