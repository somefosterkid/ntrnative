// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/comm_player_support.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "system/comm_player_support.h"
#include "types.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "save/player_info.h"

// The support that another player gives over communication, used once by the next battle. Our names

struct CommPlayerSupport {
    u32 type;
    // The support the battle used, and the support that was set when the last battle ended
    u32 usedType;
    u32 lastType;
    PlayerInfo supporter;
    // The player whose support the battle used
    PlayerInfo usedSupporter;
};

// pokebw2's CommPlayerSupport_Create
CommPlayerSupport *func_0202e770(HeapID heapId) {
    CommPlayerSupport *support = GFL_HeapAllocate(heapId, sizeof(CommPlayerSupport), TRUE, "comm_player_support.c", 44);

    func_0202e79c(support);
    return support;
}

// pokebw2's CommPlayerSupport_Free
void func_0202e794(CommPlayerSupport *support) {
    GFL_HeapFree(support);
}

// pokebw2's CommPlayerSupport_Init
void func_0202e79c(CommPlayerSupport *support) {
    MI_CpuFill8(support, 0, sizeof(CommPlayerSupport));
    support->type = COMM_PLAYER_SUPPORT_NONE;
    support->usedType = COMM_PLAYER_SUPPORT_NONE;
    support->lastType = COMM_PLAYER_SUPPORT_NONE;
    func_020084a0(&support->supporter);
    func_020084a0(&support->usedSupporter);
}

void func_0202e7c4(CommPlayerSupport *support, u32 type, PlayerInfo *info) {
    support->type = type;
    support->supporter = *info;
}

PlayerInfo *func_0202e7dc(CommPlayerSupport *support) {
    return &support->supporter;
}

u32 func_0202e7e0(CommPlayerSupport *support) {
    return support->type;
}




// pokebw2's CommPlayerSupport_SetUsed
void func_0202e7e4(CommPlayerSupport *support) {
    support->usedType = support->type;
    support->usedSupporter = support->supporter;
    support->type = COMM_PLAYER_SUPPORT_USED;
}

// pokebw2's CommPlayerSupport_EndBattle
void func_0202e80c(CommPlayerSupport *support) {
    support->lastType = support->type;
    support->type = COMM_PLAYER_SUPPORT_NONE;
}
