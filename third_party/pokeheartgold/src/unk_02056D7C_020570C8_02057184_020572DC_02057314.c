#include "global.h"

#include "comm_player_manager.h"
#include "unk_02035900.h"
#include "unk_02056D7C.h"

void sub_02057818(int netId);

void sub_02057314(struct FieldSystem *unused) {
    u8 sp0;
    s32 var_r5;
    s32 var_r7;

    var_r5 = 0;
    var_r7 = var_r5;
    do {
        if (sub_020373B4(var_r5) != 0) {
            sub_02057818(var_r5);
        } else {
            if (_021D41C4->active[var_r5] != 0) {
                sp0 = (u8)var_r5;
                sub_0205776C(var_r7, 1, &sp0, _021D41C4->fieldSystem);
            }
        }
        var_r5 += 1;
    } while (var_r5 < 8);
}
