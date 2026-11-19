#include "global.h"
struct GTSApplicationState;

#include "yes_no_prompt.h"
#include "overlay_70_gtsapplication_state.h"

s32 ov70_02238FE0(void);

enum YesNoResponse ov70_02238C8C(struct GTSApplicationState *arg0);

enum YesNoResponse ov70_02238C8C(GTSApplicationState *arg0) {
    enum YesNoResponse v0;

    v0 = YesNoPrompt_HandleInput(arg0->unk_11C8);
    if ((u32)(v0 - 1) <= 1U) {
        ov70_02238FE0();
    }
    return v0;
}
