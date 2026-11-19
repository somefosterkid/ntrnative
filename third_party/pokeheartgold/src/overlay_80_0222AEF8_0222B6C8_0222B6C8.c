#include "global.h"

#include "overlay_80_0222AEF8.h"
#include "unk_02035900.h"
#include "overlay_80_battle_castle.h"

s32 ov80_0222B6C8(void *arg0) {
    u8 v0;
    u8 v1;
    u8 v2;

    *((s16 *)&((BattleCastle *)arg0)->commBuffer[0]) = (s16) ((BattleCastle *)arg0)->unk_A18;
    if (sub_0203769C() == 0) {
        v0 = ((BattleCastle *)arg0)->unk_A1B;
        if (v0 == 0) {
            ((BattleCastle *)arg0)->unk_A1B = (u8) ((BattleCastle *)arg0)->unk_A18;
        } else if ((v0 - 6) == 4) {
            v1 = ((BattleCastle *)arg0)->unk_A18;
            if (v1 != 4) {
                ((BattleCastle *)arg0)->unk_A1B = v1;
            }
        }
    } else if (((BattleCastle *)arg0)->unk_A1B == 4) {
        v2 = ((BattleCastle *)arg0)->unk_A18;
        if (v2 != 4) {
            ((BattleCastle *)arg0)->unk_A1B = (u8)(v2 + 6);
        }
    }
    *((s16 *)&((BattleCastle *)arg0)->commBuffer[1]) = (s16) ((BattleCastle *)arg0)->unk_A1B;
    if (sub_02037030(0x2C, arg0 + 0x3C0, 0x28) == 1) {
        return 1;
    }
    return 0;
}
