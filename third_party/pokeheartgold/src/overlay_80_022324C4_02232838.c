#include "global.h"

#include "frontier/overlay_80_022340E8.h"

#include "party.h"
#include "pokemon.h"
#include "overlay_80_battle_castle.h"
#include "overlay_80_02237A70.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov80_0222A6B8(u8, u16, u16, void *, void *, void *, void *, u8);
#else
s32 ov80_0222A6B8(s32, u16, u16, void *, void *, void *, void *, s32);
#endif


u8 ov80_02237B58(u8, s32);

u16 ov80_02237ED8(BattleCastle *);

void ov80_02232838(struct BattleCastle *castle);

void ov80_02232838(struct BattleCastle *castle) {
    s32 v0;
    Pokemon *mon;
    s32 v3;
    s32 slot;
    u8 v1;
    u8 v2;

    v1 = (u8)Party_GetCount(castle->playersParty);
    slot = 0;
    v0 = (s32)v1;
    if ((s32)v1 > 0) {
        do {
            mon = Party_GetMonByIndex(castle->playersParty, slot);
            castle->startingPP[slot][0] = (s16)GetMonData(mon, 0x3A, NULL);
            castle->startingPP[slot][1] = (s16)GetMonData(mon, 0x3B, NULL);
            castle->startingPP[slot][2] = (s16)GetMonData(mon, 0x3C, NULL);
            castle->startingPP[slot][3] = (s16)GetMonData(mon, 0x3D, NULL);
            slot += 1;
        } while (slot < v0);
    }
    ov80_02237ADC(castle->challengeType, ov80_02237ED8(castle), castle->trainerIDs, 0xE);
    v2 = castle->currentBattle;
    v3 = ov80_02237B58(castle->challengeType, 1);
    ov80_0222A6B8(v3, castle->trainerIDs[v2], castle->trainerIDs[v2 + 7], castle->monSetIDs, castle->opponentMons, castle->opponentMonIVs, castle->opponentMonPersonalities, (s32)(u8)ov80_02237D8C(castle->challengeType));
}
