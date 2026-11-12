// Black's code at ov074 0x021f54e0: the field cut-in table and its getter (pokebw2's battle_cutin_db.c, ov147 there).

#include "types.h"

typedef struct BattleCutinParam {
    u32 cutin;
    u32 unk_04;
    u32 unk_08;
    u32 unk_0c;
    u32 flag;
} BattleCutinParam;

static const BattleCutinParam sBattleCutinDb[] = {
#include "asset/battle_cutin_db_sBattleCutinDb.inc"
};

const BattleCutinParam *func_ov074_021f54e0(u32 index) {
    return &sBattleCutinDb[index];
}
