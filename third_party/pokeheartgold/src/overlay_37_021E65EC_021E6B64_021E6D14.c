#include "global.h"
#include "sprite_resources_header.h"

#include "unk_02095EAC.h"
#include "bg_window.h"
#include "options.h"
#include "pm_string.h"
#include "render_window.h"
#include "unk_0200A090.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
void sub_02038C1C(s8 maxNumConnection);
#else
void sub_02038C1C(int maxNumConnection);
#endif

PlayerProfile *sub_02034818(int netId);

BOOL sub_02037108(int cmd, const void *data, int size);

int sub_02037454(void);

u16 sub_0203769C(void);

void ov37_021E68AC(UnkStruct_02095EAC *param0);

int ov37_021E75C4(void);

void ov37_021E6848(Sprite **param0, BOOL param1);

void ov37_021E7844(UnkStruct_02095EAC *param0, int param1);

void ov37_021E78A4(UnkStruct_02095EAC *param0);

int ov37_021E6D14(UnkStruct_02095EAC *param0, int param1);

int ov37_021E6D14(UnkStruct_02095EAC *param0, int param1) {
    int v0 = YesNoPrompt_HandleInput(param0->unk_9454);

    if ((param0->unk_37C != ov37_021E75C4()) || (param0->unk_9418 != 0)) {
        ov37_021E68AC(param0);
        return param1;
    }

    switch (v0) {
    case 1:
        ov37_021E7844(param0, 15);
        sub_02037108(127, 0L, 0);
        BufferPlayersName(param0->unk_0C, 0, sub_02034818(0));
        param1 = 2;
        ov37_021E78A4(param0);
        break;
    case 2:
        ov37_021E7844(param0, 4);
        ov37_021E6848(param0->unk_2AC, 0);
        ClearFrameAndWindow2(&param0->unk_33C, 1);
        ov37_021E78A4(param0);

        if (sub_0203769C() == 0) {
            sub_02038C1C(sub_02037454() + 1);
            param0->unk_9458 = 1;
        }
        break;
    }

    ov37_021E68AC(param0);
    return param1;
}
