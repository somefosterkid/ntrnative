#include "global.h"

#include "unk_02035900.h"
#include "unk_020379A0.h"
#include "comm_tool.h"

struct UnkStruct_021D414C;


void sub_02037A24(int a0, int a1, u8 *a2);

void sub_02037A24(int a0, int a1, u8 *a2) {
    u8 local[3];
    int i;
    local[0] = a2[0];
    if (sub_0203769C() != 0) {
        return;
    }
    local[1] = a0;
    local[2] = local[0];
    sub_02037184(0x12, &local[1]);
    _021D414C->syncNo[a0] = local[0];
    for (i = 0; i < 8; i++) {
        if (sub_020373B4((u16)i) != 0) {
            if (local[0] != _021D414C->syncNo[i]) {
                return;
            }
        }
    }
    if (sub_02037184(0x11, local) != 0) {
        return;
    }
    _021D414C->syncState = 1;
}
