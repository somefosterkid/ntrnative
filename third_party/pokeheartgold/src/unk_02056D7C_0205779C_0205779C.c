#include "global.h"

#include "comm_player_manager.h"
#include "unk_02056D7C.h"

s32 sub_02057184(u32);

void sub_0205779C(int arg0, int arg1, void *arg2, void *unused3) {
    s8 var_r0;
    CommPlayerPosition *temp_r1;
    u32 temp_r3;

    temp_r3 = *(u8 *)arg2 & 0xF;
    if (_021D41C4 != 0) {
        temp_r1 = &_021D41C4->positions[temp_r3];
        if (0x80 & *(u8 *)((u8 *)arg2 + 2)) {
            var_r0 = 1;
        } else {
            var_r0 = 0;
        }
        temp_r1->bumped = var_r0;
        temp_r1->x = 0U;
        temp_r1->z = 0U;
        temp_r1->x = (u16)(temp_r1->x + *(u8 *)((u8 *)arg2 + 1));
        temp_r1->x = (u16)(temp_r1->x + ((*(u8 *)((u8 *)arg2 + 2) << 8) & 0x100));
        temp_r1->z = (u16)(temp_r1->z + *(u8 *)((u8 *)arg2 + 3));
        temp_r1->z = (u16)(temp_r1->z + (0x100 & (*(u8 *)((u8 *)arg2 + 2) << 7)));
        temp_r1->facing = (s8)(((s32) * (u8 *)arg2 >> 4) & 3);
        temp_r1->speed = (s8)(3 & ((s32) * (u8 *)arg2 >> 6));
        sub_02057184((u8)temp_r3);
    }
}
