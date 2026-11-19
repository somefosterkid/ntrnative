#ifndef POKEHEARTGOLD_OVERLAY_80_0222AEF8_H
#define POKEHEARTGOLD_OVERLAY_80_0222AEF8_H

#include "global.h"

void ov80_0222AEF8(int dummy1, int sLotoId, void *dummy2, void *battleHall);
void ov80_0222AF3C(int dummy1, int sLotoId, void *dummy2, void *battleHall);

s32 ov80_0222AF54(void *arg0);

void ov80_0222AF84(int netID, int unused, void *trainerIDs, void *battleHall);
void ov80_0222AFEC(int netID, int unused, void *monIndices, void *battleHall);

void ov80_0222B048(int netID, int unused, void *param2, void *battleHall);

void ov80_0222B0B8(int netID, int sLotoId, void *partnersMon, void *battleHall);

s32 ov80_0222B0E8(s32 arg0, s32 arg1, s32 arg2);
struct BattleFactory;

s32 ov80_0222B108(struct BattleFactory *factory);

void ov80_0222B140(int netID, int sLotoId, void *param2, void *battleFactory);
void ov80_0222B1A4(int netID, int unused, void *trainerIDs, void *battleFactory);

void ov80_0222B24C(s32 arg0, s32 arg1, u16 *arg2, void *arg3);

void ov80_0222B334(int netID, int unused, void *param2, void *battleFactory);
void ov80_0222B3D4(int netID, int unused, void *param2, void *battleFactory);
void ov80_0222B420(int netID, int unused, void *param2, void *battleFactory);

s32 ov80_0222B6C8(void *arg0);

void ov80_0222B860(int netID, int unused, void *monData, void *battleCastle);
void ov80_0222B8F8(int netID, int unused, void *param2, void *battleCastle);
void ov80_0222B940(int netID, int unused, void *param2, void *battleCastle);
void ov80_0222B9CC(int netID, int unused, void *param2, void *battleCastle);
u8 *ov80_0222BA5C(int index, void *battleCastle, int size);
void ov80_0222BAB0(int netID, int sLotoId, void *param2, void *battleArcade);
void ov80_0222BB18(int netID, int unused, void *trainerIDs, void *battleArcade);
void ov80_0222BBD0(int netID, int unused, void *param2, void *battleArcade);
void ov80_0222BC6C(int netID, int unused, void *param2, void *battleArcade);
void ov80_0222BCB8(int netID, int unused, void *param2, void *battleArcade);
void ov80_0222BD44(int netID, int unused, void *partnersParty, void *battleArcade);
u8 *ov80_0222BDD4(int netID, void *battleArcade, int size);

#endif // POKEHEARTGOLD_OVERLAY_80_0222AEF8_H
