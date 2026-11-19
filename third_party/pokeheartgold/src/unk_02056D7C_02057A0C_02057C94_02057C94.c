#include "global.h"
#include "unk_02037C94.h"

u16 sub_0203769C(void);

extern int sub_0203774C(int param0);

extern void sub_02037894(int param0, int param1);

extern u16 sub_02057A34(int netId);

extern u16 sub_02057A88(int netId);

typedef struct {
    u16 unk_00;
    u16 unk_02;
} UnkStruct_020590C4;


BOOL sub_02057C94(void);

typedef struct {
    UnkStruct_020590C4 a[2];
} _020FC7A0_Array;

extern const _020FC7A0_Array _020FC7A0;

typedef struct {
    UnkStruct_020590C4 a[4];
} _020FC7A8_Array;

extern const _020FC7A8_Array _020FC7A8;

BOOL sub_02057C94(void) {
    _020FC7A0_Array batleGrid1v1 = _020FC7A0;
    _020FC7A8_Array battleGrid2v2 = _020FC7A8;
    int connectedPlayers = sub_0203774C(sub_0203993C());
    int netId, netJd, playerCnt = 0, v6[4];
    int v8 = 0;
    UnkStruct_020590C4 *v9;

    if (connectedPlayers == 2) {
        v9 = batleGrid1v1.a;
    } else {
        v9 = battleGrid2v2.a;
    }

    for (netId = 0; netId < connectedPlayers; netId++) {
        for (netJd = 0; netJd < connectedPlayers; netJd++) {
            if ((sub_02057A34(netJd) == v9[netId].unk_00) && (sub_02057A88(netJd) == v9[netId].unk_02)) {
                playerCnt++;
                v6[netJd] = netId;

                if (netJd == sub_0203769C()) {
                    v8 = 1;
                }
                break;
            }
        }
    }

    if (playerCnt == connectedPlayers) {
        for (netId = 0; netId < connectedPlayers; netId++) {
            sub_02037894(v6[netId], netId);
        }
    }

    return v8;
}
