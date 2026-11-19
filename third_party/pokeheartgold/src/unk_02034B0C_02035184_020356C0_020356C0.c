#include "global.h"

#include "comm_server_client.h"

void sub_020356C0(BOOL param0);

void sub_020356C0(BOOL param0) {
    if (_021D4134.sCommServerClient) {
        _021D4134.sCommServerClient->unk_D95_2 = param0;
    }
}
