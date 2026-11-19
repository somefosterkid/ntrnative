#include "global.h"

#include "error_handling.h"
#include "party.h"
#include "pokemon.h"
#include "overlay_80_022340E8.h"
#include "overlay_80_battle_arcade.h"

struct BattleArcade;

void ov80_02235264(BattleArcade *arcade, Party *party, u8 partySize);

void ov80_022352BC(void *arg0);
void ov80_022352E8(void *arg0);
void ov80_022352F4(void *arg0);
void ov80_02235300(void *arg0);

void ov80_02235264(BattleArcade *arcade, Party *party, u8 partySize) {
    for (int i = 0; i < partySize; i++) {
        Pokemon *mon = Party_GetMonByIndex(party, i);
        u32 level = GetMonData(mon, MON_DATA_LEVEL, NULL);

        level += 3;
        if (level > 100) {
            GF_ASSERT(FALSE);
            level = 100;
        }

        u32 exp = GetMonExpBySpeciesAndLevel(GetMonData(mon, MON_DATA_SPECIES, NULL), level);

        SetMonData(mon, MON_DATA_EXPERIENCE, &exp);
        CalcMonLevelAndStats(mon);
    }
}

void ov80_022352BC(void *arg0) {
    *(s32 *)((u8 *)arg0 + 0x14) = 0x3E9;
}

void ov80_022352C8(void *arg0) {
    *(s32 *)((u8 *)arg0 + 0x14) = 1;
}

void ov80_022352D0(void *arg0) {
    *(s32 *)((u8 *)arg0 + 0x14) = 7;
}

void ov80_022352D8(void *arg0) {
    *(s32 *)((u8 *)arg0 + 0x14) = 4;
}

void ov80_022352E0(void *arg0) {
    *(s32 *)((u8 *)arg0 + 0x14) = 9;
}

void ov80_022352E8(void *arg0) {
    *(s32 *)((u8 *)arg0 + 0x14) = 0x3EA;
}

void ov80_022352F4(void *arg0) {
    u8 v0;

    v0 = *(u8 *)((u8 *)arg0 + 0x1C);
    if ((u32)v0 < 7U) {
        *(u8 *)((u8 *)arg0 + 0x1C) = (u8)(v0 + 1);
    }
}

void ov80_02235300(void *arg0) {
    u8 v0;

    v0 = *(u8 *)((u8 *)arg0 + 0x1C);
    if (v0 != 0) {
        *(u8 *)((u8 *)arg0 + 0x1C) = (u8)(v0 - 1);
    }
}

void ov80_0223530C(void *arg0) {
    *(s8 *)((u8 *)arg0 + 0x12) = 1;
}

void ov80_02235314(void) {
}

void ov80_02235318(void) {
}

void ov80_0223531C(void) {
}

void ov80_02235320(void) {
}
