#include "global.h"

#include "comm_cmd_manager.h"
#include "error_handling.h"
#include "unk_02033AE0.h"

void sub_02037974(void);

extern const CommCmdTable _020F6980[22];

void sub_02034170(int param0, int cmd, int param2, void *param3);

s32 sub_020342C8(void);

void sub_02034170(int param0, int cmd, int param2, void *param3) {
    CommCmdRecvFunc v0;

    if (cmd < 22) {
        v0 = _020F6980[cmd].unk_00;
    } else {
        GF_ASSERT(_021D412C);

        if (cmd > (_021D412C->unk_04 + 22)) {
            sub_02037974();
            return;
        }

        v0 = _021D412C->cmdTable[cmd - 22].unk_00;
    }

    if (v0 != NULL) {
        if (_021D412C) {
            v0(param0, param2, param3, _021D412C->unk_08);
        } else {
            v0(param0, param2, param3, NULL);
        }
    }
}

int sub_020341DC(int cmd);

int sub_020341DC(int cmd) {
    int v0 = 0;
    CommCmdSizeFunc v1;

    if (cmd < 22) {
        v1 = _020F6980[cmd].unk_04;
    } else {
        GF_ASSERT(_021D412C);

        if (_021D412C == NULL) {
            sub_02037974();
            return v0;
        }

        if (cmd > (_021D412C->unk_04 + 22)) {
            GF_ASSERT(FALSE);
            sub_02037974();
            return v0;
        }

        v1 = _021D412C->cmdTable[cmd - 22].unk_04;
    }

    if (v1 != NULL) {
        v0 = v1();
    }

    return v0;
}

BOOL sub_02034244(int cmd);

BOOL sub_02034244(int cmd) {
    if (cmd < 22) {
        return _020F6980[cmd].unk_08 != NULL;
    }

    return _021D412C->cmdTable[cmd - 22].unk_08 != NULL;
}

void *sub_02034280(int cmd, int netId, int param2);

void *sub_02034280(int cmd, int netId, int param2) {
    CommCmdAllocFunc v0;

    if (cmd < 22) {
        v0 = _020F6980[cmd].unk_08;
        return v0(netId, NULL, param2);
    } else {
        v0 = _021D412C->cmdTable[cmd - 22].unk_08;
        return v0(netId, _021D412C->unk_08, param2);
    }

    return NULL;
}

u32 sub_020342B8(void) {
    return 0xFFFFU;
}

u32 sub_020342C0(void) {
    return 0U;
}

u32 sub_020342C4(void) {
    return 1U;
}

s32 sub_020342C8(void) {
    return 2;
}
