#include "global.h"

#include "comm_player_manager.h"
#include "error_handling.h"
#include "field_types_def.h"
#include "unk_02054648.h"

#include "unk_02056D7C.h"

void sub_020576C0(int arg0, int arg1, void *arg2, void *unused3) {
    s32 var_r5;
    CommPlayerPosition *temp_r2;

    if (_021D41C4 != NULL) {
        temp_r2 = &_021D41C4->hostPositions[arg0];
        if (0x80 & *(u8 *)((u8 *)arg2 + 4)) {
            _021D41C4->sendAllPositions = 1;
            return;
        }
        if (temp_r2->facing == -1) {
            var_r5 = 0;
            do {
                _021D41C4->positionChanged[var_r5] = 1;
                var_r5 += 1;
            } while (var_r5 < 8);
        }
        temp_r2->x = 0U;
        temp_r2->z = 0U;
        temp_r2->x = (u16)(temp_r2->x + *(u8 *)arg2);
        temp_r2->x = (u16)(temp_r2->x + ((*(u8 *)((u8 *)arg2 + 1) << 8) & 0xFF00));
        temp_r2->z = (u16)(temp_r2->z + *(u8 *)((u8 *)arg2 + 2));
        temp_r2->z = (u16)(temp_r2->z + (0xFF00 & (*(u8 *)((u8 *)arg2 + 3) << 8)));
        temp_r2->facing = (s8)(0xF & *(u8 *)((u8 *)arg2 + 4));
        _021D41C4->active[arg0] = 1;
        _021D41C4->positionChanged[arg0] = 1;
        if (sub_020548C0(_021D41C4->fieldSystem, (s32)temp_r2->x, (s32)temp_r2->z) != 0) {
            GF_AssertFail();
        }
    }
}
