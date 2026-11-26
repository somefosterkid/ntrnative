#include "global.h"

#include "comm_cmd_manager.h"
#include "unk_02035900.h"



BOOL sub_02037108(int cmd, const void *data, int size);

void sub_020342CC(int param0, int param1, void *param2, void *param3);

void sub_020342CC(int param0, int param1, void *param2, void *param3) {
    u8 *v0 = param2;
    int v1;

    if (sub_0203769C() != 0) {
        return;
    }

    _021D412C->unk_0C[param0] = 1;

    for (v1 = 0; v1 < (7 + 1); v1++) {
        if (!sub_020373B4(v1)) {
            continue;
        }

        if (!_021D412C->unk_0C[v1]) {
            return;
        }
    }

    sub_02037108(14, NULL, 0);
}
