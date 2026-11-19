#include "global.h"

#include "comm_server_client.h"

void sub_020356EC(BOOL param0);

void sub_020356EC(BOOL param0) {
    if (_021D4134.sCommServerClient) {
        _021D4134.sCommServerClient->unk_D95_1 = param0;
        _021D4134.sCommServerClient->unk_D8E = 0xffff;
    }
}
