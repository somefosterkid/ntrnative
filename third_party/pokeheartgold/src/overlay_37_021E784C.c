#include "global.h"
#include "sprite_resources_header.h"

#include "unk_02095EAC.h"
#include "bg_window.h"
#include "options.h"
#include "pm_string.h"
#include "unk_0200A090.h"
#include "touchscreen.h"

typedef struct {
    int (*unk_00)(UnkStruct_02095EAC *, int);
    int unk_04;
} UnkStruct_ov58_021D3180;

extern UnkStruct_ov58_021D3180 ov37_021E7D20[24];

void ov37_021E784C(UnkStruct_02095EAC *param0, int param1);

struct YesNoPromptTemplate;
BOOL ov37_021E7880(UnkStruct_02095EAC *param0, YesNoPromptTemplate *param1);
void YesNoPrompt_Reset(YesNoPrompt *menu);
void ov37_021E78A4(UnkStruct_02095EAC *param0);

extern TouchscreenHitbox _021E7968;
s32 ov37_021E78C4(void);

void ov37_021E784C(UnkStruct_02095EAC *param0, int param1) {
    if (param0->unk_368 != param0->unk_36C) {
        if (param1 != 2) {
            param0->unk_368 = param0->unk_36C;
        } else {
            if (ov37_021E7D20[param0->unk_36C].unk_04) {
                param0->unk_368 = param0->unk_36C;
            }
        }
    }
}

BOOL ov37_021E7880(UnkStruct_02095EAC *param0, YesNoPromptTemplate *param1) {
    if (!param0->unk_945C) {
        YesNoPrompt_InitFromTemplate(param0->unk_9454, param1);
        param0->unk_945C = 1;
        return 1;
    } else {
        return 0;
    }
}

void ov37_021E78A4(UnkStruct_02095EAC *param0) {
    if (param0->unk_945C) {
        YesNoPrompt_Reset(param0->unk_9454);
        param0->unk_945C = 0;
    }
}

s32 ov37_021E78C4(void) {
    if (TouchscreenHitbox_FindRectAtTouchNew(&_021E7968) != -1) {
        return 1;
    }
    return 0;
}
