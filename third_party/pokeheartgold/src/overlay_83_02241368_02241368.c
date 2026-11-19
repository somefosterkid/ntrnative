#include "global.h"

#include "unk_02035900.h"
#include "overlay_83_battle_castle_self_app.h"

void ov83_02241504(void *arg0);

void ov83_022413C4(BattleCastleSelfApp *arg0, u16 cmd);

void ov83_0224143C(BattleCastleSelfApp *arg0, u16 cmd, u16 slot);

void ov83_022414CC(BattleCastleSelfApp *arg0, u16 cmd);

u32 ov83_02241368(BattleCastleSelfApp *param_1, u16 param_2, u16 arg2);

u32 ov83_02241368(BattleCastleSelfApp *param_1, u16 param_2, u16 arg2) {
    BOOL BVar1;
    s32 unaff_r4;

    switch (param_2) {
    case 10:
        unaff_r4 = 0x31;
        ov83_022413C4(param_1, param_2);
        break;
    case 0xb:
        unaff_r4 = 0x32;
        ov83_0224143C(param_1, param_2, arg2);
        break;
    case 0xc:
        unaff_r4 = 0x33;
        ov83_022414CC(param_1, param_2);
        break;
    case 0xd:
        unaff_r4 = 0x34;
        ov83_02241504(param_1);
    }
    BVar1 = sub_02037030(unaff_r4, param_1->commPayload, 0x28);
    if (BVar1 != 1) {
        return 0;
    }
    return 1;
}
