#include "global.h"

#include "assert.h"
#include "comm_server_client.h"
#include "heap.h"
#include "mail_message.h"
#include "save_link_ruleset.h"
#include "system.h"
#include "unk_02034B0C.h"

s32 sub_02033990(void);

s32 sub_02032E24(void);

BOOL sub_0203393C(void);

void sub_02033858(void);

BOOL sub_02033920(void);

BOOL sub_02033958(void);

int sub_02034FE8(void) {
    if (!_021D4134.sCommServerClient) {
        return 1;
    }

    switch (_021D4134.sCommServerClient->unk_D94) {
    case 0:
        if (sub_02033990()) {
            sub_02032E24();
            _021D4134.sCommServerClient->unk_D94 = 1;
            break;
        }
        if (!sub_0203393C()) {
            sub_02033858();
            _021D4134.sCommServerClient->unk_D94 = 2;
        }
        break;
    case 1:
        if (!sub_0203393C()) {
            sub_02033858();
            _021D4134.sCommServerClient->unk_D94 = 2;
        }
        break;
    case 2:
        if (sub_02033920()) {
            return 1;
        }

        if (sub_02033958()) {
            _021D4134.sCommServerClient->unk_D94 = 1;
        }
        break;
    }

    return 0;
}
