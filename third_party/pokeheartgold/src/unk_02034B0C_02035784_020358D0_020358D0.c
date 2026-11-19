#include "global.h"

#include "comm_server_client.h"

const void *sub_020358D0(int param0);

const void *sub_020358D0(int param0) {
    if (_021D4134.sCommServerClient && _021D4134.sCommServerClient->unk_D44[param0] != 0) {
        CommGameInfo *v0 = (CommGameInfo *)_021D4134.sCommServerClient->unk_114[param0].gameInfo.userGameInfo;
        return &v0->unk_08;
    }

    return 0L;
}
