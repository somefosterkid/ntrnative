#include "global.h"

#include "unk_02035900.h"
#include "overlay_83_battle_castle_opponent_app.h"

void ov83_02245248(BattleCastleOpponentApp *arg0);

void ov83_02245104(BattleCastleOpponentApp *arg0, u16 cmd);

void ov83_0224517C(void *arg0, u16 cmd, u16 slot);

void ov83_02245210(void *arg0, u16 cmd);

u32 ov83_022450A8(void *param_1, u16 param_2, u16 arg2);

u32 ov83_022450A8(void *param_1, u16 param_2, u16 arg2) {
    BOOL BVar1;
    s32 unaff_r4;

    switch (param_2) {
    case 0x14:
        unaff_r4 = 0x35;
        ov83_02245104(param_1, param_2);
        break;
    case 0x15:
        unaff_r4 = 0x36;
        ov83_0224517C(param_1, param_2, arg2);
        break;
    case 0x16:
        unaff_r4 = 0x37;
        ov83_02245210(param_1, param_2);
        break;
    case 0x17:
        unaff_r4 = 0x38;
        ov83_02245248(param_1);
    }
    BVar1 = sub_02037030(unaff_r4, (void *)((u8 *)param_1 + 0x564), 0x28);
    if (BVar1 != 1) {
        return 0;
    }
    return 1;
}
