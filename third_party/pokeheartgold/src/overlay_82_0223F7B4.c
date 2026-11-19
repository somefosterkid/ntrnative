#include "global.h"

#include "overlay_82.h"
#include "unk_02035900.h"
#include "overlay_82_battle_hall_app.h"


void ov82_0223F7B4(s32 netID, s32 unused, void *data, void *context) {
    *(u8 *)((u8 *)context + 0x16) = (u8)(*(u8 *)((u8 *)context + 0x16) + 1);
    if (netID != sub_0203769C()) {
        *(u8 *)((u8 *)context + 0x27C) = (u8) * (u16 *)((u8 *)data + 2);
        if (sub_0203769C() == 0) {
            if (*(u8 *)((u8 *)context + 0x18) != 0xFF) {
                *(u8 *)((u8 *)context + 0x27C) = 0U;
            } else {
                *(u8 *)((u8 *)context + 0x18) = (u8)(*(u8 *)((u8 *)context + 0x27C) + 0x14);
            }
        } else {
            *(u8 *)((u8 *)context + 0x18) = (u8) * (u16 *)((u8 *)data + 4);
        }
        **(u16 **)((u8 *)context + 0x21C) = *(u16 *)((u8 *)data + 6);
    }
}

void ov82_0223F808(void *arg0, s16 arg1, s16 arg2) {
    *((s16 *)&((BattleHallApp *)arg0)->commPayload[0]) = arg1;
    *((s16 *)&((BattleHallApp *)arg0)->commPayload[1]) = arg2;
}
