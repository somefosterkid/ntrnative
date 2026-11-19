#include "global.h"
struct BattleCastleOpponentApp;

#include "unk_02035900.h"
#include "overlay_83_battle_castle_opponent_app.h"

void ov83_022451B8(s32 netID, s32 unused, void *data, void *context);

void ov83_02245210(void *arg0, u16 arg1);

void ov83_02244D0C(void *, u32, u32);
void ov83_02245220(u32 param_1, u32 param_2, void *param_3, struct BattleCastleOpponentApp *param_4);

void ov83_02245248(struct BattleCastleOpponentApp *arg0);

void ov83_022451B8(s32 netID, s32 unused, void *data, void *context) {
    *(u8 *)((u8 *)context + 0x17) = (u8)(*(u8 *)((u8 *)context + 0x17) + 1);
    if (netID != sub_0203769C()) {
        *(u8 *)((u8 *)context + 0x5B5) = (u8) * (u16 *)((u8 *)data + 2);
        if (sub_0203769C() == 0) {
            if (*(u8 *)((u8 *)context + 0x11) != 0xFF) {
                *(u8 *)((u8 *)context + 0x5B5) = 0U;
                return;
            }
            *(u8 *)((u8 *)context + 0x11) = (u8)(*(u8 *)((u8 *)context + 0x5B5) + *(u8 *)((u8 *)context + 0x15));
            *(s8 *)((u8 *)context + 0x12) = (s8) * (u16 *)((u8 *)data + 8);
            *(s8 *)((u8 *)context + 0x13) = (s8) * (u16 *)((u8 *)data + 0xA);
            return;
        }
        *(u8 *)((u8 *)context + 0x11) = (u8) * (u16 *)((u8 *)data + 4);
        *(s8 *)((u8 *)context + 0x12) = (s8) * (u16 *)((u8 *)data + 8);
        *(s8 *)((u8 *)context + 0x13) = (s8) * (u16 *)((u8 *)data + 0xA);
    }
}

void ov83_02245210(void *arg0, u16 arg1) {
    *(s16 *)((u8 *)arg0 + 0x564) = arg1;
    *(s16 *)((u8 *)arg0 + 0x566) = (s16) * (u8 *)((u8 *)arg0 + 0xD);
}

void ov83_02245220(u32 param_1, u32 param_2, void *param_3, BattleCastleOpponentApp *param_4) {
    u16 uVar1;

    uVar1 = sub_0203769C();
    if (param_1 != uVar1) {
        *((s8 *)&param_4->partnersSelectedSlot) = (s8) * (u16 *)(param_3 + 2);
        ov83_02244D0C(param_4, param_4->partnersSelectedSlot, 1);
    }
    return;
}

void ov83_02245248(BattleCastleOpponentApp *arg0) {
    *((s16 *)&arg0->commPayload[0]) = 1;
}
