#include "global.h"

#include "comm_server_client.h"

s32 sub_02034BE4(void);

BOOL sub_02034BF8(const u8 *param0, const u8 *param1, int param2);

s32 sub_02034BE4(void) {
    if (_021D4134.sCommServerClient != NULL) {
        return 1;
    }
    return 0;
}

BOOL sub_02034BF8(const u8 *param0, const u8 *param1, int param2) {
    const u8 *v1 = param0;
    const u8 *v2 = param1;

    for (int i = 0; i < param2; i++) {
        if (*v1 != *v2) {
            return 0;
        }

        v1++;
        v2++;
    }

    return 1;
}
