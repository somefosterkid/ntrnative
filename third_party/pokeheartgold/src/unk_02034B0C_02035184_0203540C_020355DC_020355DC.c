#include "global.h"

#include "comm_server_client.h"
#include "unk_02032844.h"

int sub_02033298(void);

BOOL sub_020355DC(u16 param0);

BOOL sub_020355DC(u16 param0) {
    if (!_021D4134.sCommServerClient) {
        return 0;
    }

    if (sub_02033298() != 4) {
        return 0;
    }

    {
        u16 v0 = sub_02033250();

        if (v0 & (1 << param0)) {
            return 1;
        }
    }
    return 0;
}
