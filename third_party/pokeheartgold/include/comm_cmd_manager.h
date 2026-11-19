#ifndef POKEHEARTGOLD_COMM_CMD_MANAGER_H
#define POKEHEARTGOLD_COMM_CMD_MANAGER_H

#include "global.h"

struct CommCmdTable;

// CommCmdManager: the comm command dispatcher's state (_021D412C, unk_02033AE0_0340C4_* / _02034170), merged from 3 local
// copies and the raw-offset users. 0x1C on the DS (the table pointer is 4 bytes there, 8 on the host).
typedef struct CommCmdManager {
    const struct CommCmdTable *cmdTable; // 0x0
    int unk_04; // 0x4
    void *unk_08; // 0x8
    u8 unk_0C[8]; // 0xC
    u8 unk_14; // 0x14
} CommCmdManager;

extern CommCmdManager *_021D412C;

#endif // POKEHEARTGOLD_COMM_CMD_MANAGER_H
