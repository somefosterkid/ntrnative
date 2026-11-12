// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/btl_main.h: the prototypes of Black's
// named functions and the types they need.
#ifndef BWPORT_BATTLE_BTL_MAIN_H
#define BWPORT_BATTLE_BTL_MAIN_H

#include "types.h"
#include "struct_decls.h"
#include "battle/btl_calc.h"
#include "battle/btl_rec.h"
#include "gfl/heap.h"
#include "nitro/math.h"
#include "system/pms_data.h"

// Swan's names for these two take the main module, whose first field points to the BtlSetup
u32 BtlSetup_GetBattleStyle(BtlMainModule *mainModule);
u32 BtlSetup_GetBattleType(BtlMainModule *mainModule);
u32 GetValidPosMax(BtlMainModule *mainModule);
BtlFieldSituation *GetFieldEffectData(BtlMainModule *mainModule);
BOOL DoesClientExist(BtlMainModule *mainModule, u8 clientId);
u8 GetClientSide(BtlMainModule *mainModule, u8 clientId);
u8 BattlePosToClientID(BtlMainModule *mainModule, u8 pos);
u8 MonIDToClientID(u8 monId);
u8 MonIDToBattlePos(BtlMainModule *mainModule, BtlPokeCon *pokeCon, u8 monId);
u8 GetPlayerClientID(BtlMainModule *mainModule);
BOOL IsAllyClientID(u8 clientId1, u8 clientId2);
void ChangeFriendshipWhenFainted(BtlMainModule *mainModule, BattleMon *mon, BOOL reason);
void ChangeFriendship(BtlMainModule *mainModule, BattleMon *mon, u32 reason);
const BattleMon *GetPokeParamConst(const BtlPokeCon *pokeCon, u8 monId);
BOOL IsAllyMonID(u8 monId1, u8 monId2);
u8 GetSideFromMonID(u8 monId);
s32 FindPartyMon(const BattleParty *party, BattleMon *mon);
BattleMon *GetBattleMonFromParty(BattleParty *party, u8 index);
u8 GetNumMonsInParty(BattleParty *party);
u8 GetAlivePartyCount(BattleParty *party);
BattleMon *GetIllusionDisguise(BtlMainModule *mainModule, BtlPokeCon *pokeCon, BattleMon *mon);

// What IsAdjacentOpponent looks up for a position
struct AdjacentOpponentData {
    u16 count1;
    u16 count2;
    u8 list1[3];
    u8 list2[3];
};

// The battle positions, 0 to 5. Even positions are on one side and odd positions on the other
#define BTL_POS_MAX 6

// Black 2's; twinned code confirms the offsets of mons and count in Black
// Layout reconstructed from the party accessors in the game code.
struct BattleParty {
    BattleMon *mons[6];
    u8 count;
    u8 unk19[3];
};

// The parties of the four clients and every BattleMon by mon ID, 0xe8 bytes as the main module holds two
struct BtlPokeCon {
    BtlMainModule *mainModule;
    BattleParty parties[4];
    // The parties the clients' Pokémon come from
    PokeParty *srcParties[4];
    BattleMon *mons[24];
    u32 unkE4;
};

// Black 2's, not checked against Black
struct BtlMainUnk478 {
    u8 unk00[8];
    s8 unk08;
    u8 unk09[0xb];
};

// Black 2's, not checked against Black
// A step of the main module's setup or cleanup, run until it returns TRUE
typedef BOOL (*BtlMainSeqFunc)(u32 *state, BtlMainModule *mainModule);

// Black 2's, not checked against Black
struct BtlMainSeq {
    BtlMainSeqFunc func;
    BtlMainSeqFunc nextFunc;
    BtlMainModule *mainModule;
    u32 state;
};

// Black 2's, not checked against Black
// What the clients of a link battle exchange before it starts
struct BtlMainSyncData {
    MATHRandContext32 rand;
    u16 unk18;
    u16 unk1A;
    u16 unk1C;
    u8 unk1E;
    u8 unk1F_0 : 4;
    u8 unk1F_4 : 4;
};

// A client's trainer, 0x28 bytes
struct BtlTrainerData {
    PlayerInfo *playerInfo;
    StrBuf *name;
    u16 unk08;
    u16 unk0A;
    u32 unk0C;
    u16 unk10[4];
    PMSData unk18;
    PMSData unk20;
};

// Black 2's; twinned code confirms the offsets of setup, viewCore, server, unk0C, clients, trainers, unkC0, unkC4,
// pokeCons, unk298, unk2A8, unk2B8, unk2C0, unk2C4, recReader, unk3E0, rand, posClientIds, prizeMoney, money, unk43C,
// unk440, result, unk448, unk460, mainFunc, heapId, clientCount, playerClientId, unk46D, unk46E, unk470, unk471,
// unk472, unk473_0, unk473_1, unk473_2, unk473_3, unk473_5 and unk473_6 in Black
struct BtlMainModule {
    BtlSetup *setup;
    BtlvCore *viewCore;
    BtlServer *server;
    BtlServer *unk0C;
    BtlClient *clients[4];
    BtlTrainerData trainers[4];
    PlayerInfo *unkC0;
    u8 unkC4[4];
    BtlPokeCon pokeCons[2];
    PokeParty *unk298[4];
    PokeParty *unk2A8[4];
    PokeParty *unk2B8;
    BtlField *field;
    PartyPkm *unk2C0;
    void *unk2C4;
    BtlRecReader recReader;
    void *unk3E0[4];
    MATHRandContext32 rand;
    // What the clients of a link battle agree on before it starts
    BtlMainSyncData syncData;
    u8 posClientIds[6];
    u8 unk42e[2];
    // The prize money, which func_ov167_0219ca78 doubles and finalizes
    u32 prizeMoney;
    u32 money;
    u32 unk438;
    u32 unk43C;
    u16 unk440;
    u16 unk442;
    u32 result;
    BtlClientIDList unk448;
    BtlMainSeq seq;
    s32 unk460;
    BOOL (*mainFunc)(BtlMainModule *mainModule);
    u16 heapId;
    u8 unk46A;
    u8 clientCount;
    u8 playerClientId;
    u8 unk46D;
    u8 unk46E;
    u8 unk46F;
    u8 unk470;
    u8 unk471;
    u8 unk472;
    u8 unk473_0 : 1;
    u8 unk473_1 : 1;
    u8 unk473_2 : 1;
    u8 unk473_3 : 1;
    u8 unk473_4 : 1;
    u8 unk473_5 : 1;
    u8 unk473_6 : 1;
    u8 unk473_7 : 1;
};

// The side of the battle a mon ID is on: IDs from 12 are the second side's
typedef enum { BTL_SIDE_1ST, BTL_SIDE_2ND } BtlSide;

s32 GetPartyPkmnEligibleForBattle(PokeParty *party);
void AddBattleMonToParty(BattleParty *party, BattleMon *mon);
BOOL func_ov093_021b64cc(BtlMainModule *mainModule);
void func_ov093_021b66d4(BtlMainSeq *seq, BtlMainModule *mainModule, BtlSetup *setup);
void func_ov093_021b6804(BtlMainSeq *seq, BtlMainModule *mainModule, BtlSetup *setup);
u8 func_ov093_021b6818(BtlSetup *setup);
void func_ov093_021b68d8(BtlMainModule *mainModule, BtlSetup *setup);
void func_ov093_021b6954(BtlMainModule *mainModule, u8 clientId);
// Underlying battle-type-dependent count used by GetClientBattlerCount.
u8 func_ov093_021b6994(BtlMainModule *mainModule, u8 clientId);
void func_ov093_021b69fc(BtlMainModule *mainModule, BtlSetup *setup);
void func_ov093_021b6a70(BtlMainModule *mainModule, BtlSetup *setup, u32 arg2);
BOOL func_ov093_021b6aac(u32 *state, BtlMainModule *mainModule);
BOOL func_ov093_021b6c08(u32 *state, BtlMainModule *mainModule);
BOOL func_ov093_021b6c5c(u32 *state, BtlMainModule *mainModule);
BOOL func_ov093_021b7050(u32 *state, BtlMainModule *mainModule);
BOOL func_ov093_021b71d4(u32 *state, BtlMainModule *mainModule);
BOOL func_ov093_021b734c(u32 *state, BtlMainModule *mainModule);
BOOL func_ov093_021b73c4(u32 *state, BtlMainModule *mainModule);
BOOL func_ov093_021b7518(u32 *state, BtlMainModule *mainModule);
BOOL func_ov093_021b7758(BtlMainModule *mainModule, s32 *state);
BOOL func_ov093_021b7cb4(BtlMainModule *mainModule, s32 *state);
BOOL func_ov093_021b8070(BtlMainModule *mainModule, s32 *state);
BOOL func_ov093_021b81dc(BtlMainModule *mainModule, s32 *state);
BOOL func_ov093_021b835c(BtlMainModule *mainModule, s32 *state);
BOOL func_ov093_021b83f4(BtlMainModule *mainModule);
BOOL func_ov093_021b8434(BtlMainModule *mainModule);
BOOL func_ov093_021b84d8(BtlMainModule *mainModule);
void func_ov093_021b8548(BtlMainModule *mainModule);
BOOL func_ov093_021b8564(BtlMainModule *mainModule);
u32 func_ov093_021b8590(BtlMainModule *mainModule);
s32 func_ov093_021b85dc(BtlMainModule *mainModule);
u32 func_ov093_021b8670(BtlMainModule *mainModule);
BOOL func_ov093_021b86a0(BtlMainModule *mainModule, u32 pos);
u8 func_ov093_021b86c0(BtlMainModule *mainModule);
BOOL func_ov093_021b86c8(BtlMainModule *mainModule);
u16 func_ov093_021b86dc(BtlMainModule *mainModule);
PlayerInfo *func_ov093_021b871c(BtlMainModule *mainModule);
u8 func_ov093_021b8778(BtlMainModule *mainModule, u16 posFlags, u8 *out);
u8 func_ov093_021b87e8(BtlMainModule *mainModule, u8 type, u8 pos, u8 *out);
u8 func_ov093_021b8834(BtlMainModule *mainModule, u8 type, u8 pos, u8 *out);
u8 func_ov093_021b88ec(BtlMainModule *mainModule, u8 type, u8 pos, u8 *out);
u32 func_ov093_021b8b78(BtlMainModule *mainModule, u8 clientId);
BOOL func_ov093_021b8bd0(BtlMainModule *mainModule, u8 side);
u8 func_ov093_021b8bec(BtlMainModule *mainModule, u8 clientId, u8 slot);
u8 func_ov093_021b8c20(u32 battleStyle, u8 pos, u8 index);
// pokebw2's GetPosOnSameSide
// The position at index on the same side as pos
u8 func_ov093_021b8c38(u8 pos, u8 index);
u8 func_ov093_021b8c50(BtlMainModule *mainModule, u8 pos, u8 index);
u8 func_ov093_021b8c5c(u32 battleStyle, u8 pos);
u8 func_ov093_021b8cb0(BtlMainModule *mainModule, u8 pos);
// pokebw2's AreClientsOnOppositeSides
BOOL func_ov093_021b8ce8(BtlMainModule *mainModule, u8 clientId1, u8 clientId2);
u8 func_ov093_021b8d38(BtlMainModule *mainModule, BtlPokeCon *pokeCon, u16 posFlags, u8 *monIds);
u8 func_ov093_021b8dc0(BtlMainModule *mainModule, BtlPokeCon *pokeCon, u8 monId);
u8 func_ov093_021b8dec(BtlMainModule *mainModule, u8 pos);
void func_ov093_021b8e28(BtlMainModule *mainModule, u8 pos, u8 *clientId, u8 *index);
u8 func_ov093_021b8e70(BtlMainModule *mainModule, u8 pos);
u8 func_ov093_021b8ed8(BtlMainModule *mainModule, u8 index);
BOOL func_ov093_021b8f38(BtlMainModule *mainModule);
PokeParty *func_ov093_021b8f5c(BtlMainModule *mainModule, u8 clientId);
PokeParty *func_ov093_021b8f7c(BtlMainModule *mainModule, u8 clientId);
u8 func_ov093_021b8fc0(BtlMainModule *mainModule, u8 clientId);
u8 func_ov093_021b8fe4(BtlMainModule *mainModule);
u8 func_ov093_021b9010(BtlMainModule *mainModule, u8 clientId);
u8 func_ov093_021b904c(BtlMainModule *mainModule, u8 other);
u32 func_ov093_021b9064(BtlMainModule *mainModule, u8 clientId, u8 other);
u8 func_ov093_021b9114(BtlMainModule *mainModule);
void func_ov093_021b916c(BtlMainModule *mainModule, u32 result);
u32 func_ov093_021b919c(BtlMainModule *mainModule);
void func_ov093_021b91e4(BtlMainModule *mainModule, u32 money);
u32 func_ov093_021b9210(BtlMainModule *mainModule);
void func_ov093_021b9234(BtlMainModule *mainModule);
void func_ov093_021b92a0(BtlMainModule *mainModule);
void func_ov093_021b9358(BtlMainModule *mainModule, u8 monId);
void func_ov093_021b93e0(BtlPokeCon *pokeCon, BtlMainModule *mainModule, u32 unkE4);
void func_ov093_021b9424(BtlPokeCon *pokeCon);
void func_ov093_021b9674(BtlPokeCon *pokeCon);
void func_ov093_021b969c(BtlPokeCon *pokeCon, BtlMainModule *mainModule, u8 clientId, BOOL keepBaseForm);
PokeParty *func_ov093_021b985c(BtlPokeCon *pokeCon, u8 clientId);
s32 func_ov093_021b9864(BtlPokeCon *pokeCon, u8 clientId, u8 monId);
BattleMon *func_ov093_021b990c(BtlPokeCon *pokeCon, u8 clientId, u8 index);
BOOL func_ov093_021b994c(BtlMainModule *mainModule, u32 index, void **out);
u8 func_ov093_021b997c(BtlMainModule *mainModule, u8 clientId);
u8 func_ov093_021b99c0(BtlMainModule *mainModule, u8 clientId);
// Returns the opponent-position lookup entry for a battle position.
const AdjacentOpponentData *func_ov093_021b99e0(u8 pos);
BOOL func_ov093_021b99f0(u8 pos);
BOOL func_ov093_021b9a00(u8 pos, u8 *out);
u8 func_ov093_021b9a5c(u8 side);
// pokebw2's IsAdjacentOpponent
// Whether pos2 is an opponent next to pos1 in a triple battle
BOOL func_ov093_021b9a6c(u8 pos1, u8 pos2);
u8 func_ov093_021b9ab0(u32 pos);
u32 func_ov093_021b9ac8(u32 pos);
u8 func_ov093_021b9ae0(u8 pos);
void func_ov093_021b9b10(BtlMainModule *mainModule, u8 clientId, u8 value);
void func_ov093_021b9b40(BattleParty *party);
void func_ov093_021b9b60(BattleParty *party);
u8 func_ov093_021b9bc4(BattleParty *party, s32 index);
void func_ov093_021b9c10(BattleParty *party, u32 first, u32 second);
void func_ov093_021b9c24(BattleParty *party, u8 index);
void func_ov093_021b9c50(BattleParty *party, u32 pos, BattleMon **oldOut, BattleMon **newOut);
s32 func_ov093_021b9cbc(const BattleParty *party, u32 monId);
BattleMon *func_ov093_021b9ce8(const BattleParty *party);
void func_ov093_021b9d10(BtlMainModule *mainModule, BattleParty *party, u8 clientId);
void func_ov093_021b9db8(BtlMainModule *mainModule);
void func_ov093_021b9de8(BtlMainModule *mainModule);
void func_ov093_021b9e7c(BtlTrainerData *trainer, const BtlSetupTrainer *src);
void func_ov093_021b9ef0(BtlTrainerData *trainer, const BtlCommTrainerData *src);
BOOL func_ov093_021b9f70(BtlMainModule *mainModule, u8 clientId);
u16 func_ov093_021b9f84(BtlMainModule *mainModule, u8 clientId, u8 index);
StrBuf *func_ov093_021b9fac(BtlMainModule *mainModule, u8 clientId, u32 *trainerClass);
u32 func_ov093_021b9fbc(BtlMainModule *mainModule, u8 clientId);
u16 func_ov093_021ba004(BtlMainModule *mainModule, u8 clientId);
u32 func_ov093_021ba020(BtlMainModule *mainModule, u8 clientId);
PMSData *func_ov093_021ba02c(BtlMainModule *mainModule, u8 clientId, u32 which);
PlayerInfo *func_ov093_021ba064(BtlMainModule *mainModule, u8 clientId);
BtlField *func_ov093_021ba08c(BtlMainModule *mainModule);
void func_ov093_021ba094(BtlMainModule *mainModule);
void func_ov093_021ba0cc(BtlMainModule *mainModule);
void func_ov093_021ba128(BtlMainModule *mainModule, u8 clientId, const PokeParty *party);
PokeParty *func_ov093_021ba178(BtlMainModule *mainModule, u8 clientId, u32 useSecond);
void func_ov093_021ba238(BtlMainModule *mainModule, BattleMon *mon);
void func_ov093_021ba2d4(BtlMainModule *mainModule, BattleMon *mon);
BOOL func_ov093_021ba534(BtlMainModule *mainModule);
u16 func_ov093_021ba5c4(BtlMainModule *mainModule);
u16 func_ov093_021ba5d0(BtlMainModule *mainModule);
BOOL func_ov093_021ba5d8(BtlMainModule *mainModule);
BOOL func_ov093_021ba618(BtlMainModule *mainModule);
void func_ov093_021ba648(BtlMainModule *mainModule, u8 clientId, const PokeParty *party);
void func_ov093_021ba670(BtlMainModule *mainModule, u8 clientId, const PlayerInfo *src);
void func_ov093_021ba698(BtlMainModule *mainModule, u8 clientId);
u8 func_ov093_021ba710(u8 playerClientId, u8 clientId);
void func_ov093_021ba73c(BtlMainModule *mainModule, u32 arg1);
void func_ov093_021ba7f8(BtlMainModule *mainModule);

// Black 2's, not checked against Black
// A choice the player is offered in a Pokestar Studios scene
struct BtlStudioChoice {
    // -1 for none
    s16 msgId;
    s16 msgId2;
    s16 unk04;
    s16 unk06;
    s16 points;
    s16 unk0A;
};

// Black 2's, not checked against Black
// One entry of a Pokestar Studios movie's script
struct BtlStudioScene {
    s16 unk00;
    s16 unk02;
    s16 unk04;
    s16 unk06;
    s16 unk08;
    s16 unk0A;
    // The messages of two kinds, by step, which the cut-ins' message command picks between
    s16 messages[2][10];
    // The script event each step waits for, -1 for none
    s16 events[10];
    BtlStudioChoice choices[4];
};

// Black 2's, not checked against Black
struct BtlStudioTurnEvent {
    s16 turn;
    s16 value;
};

// Black 2's, not checked against Black
// The rules of a scripted battle, which seem to be the Pokestar Studios movies
struct BtlScriptedRules {
    s16 movie;
    u8 unk02[2];
    s16 unk04;
    s16 unk06;
    // Which of the studio's message files the movie uses
    s16 unk08;
    s16 turnLimit;
    s16 unk0C;
    s16 unk0E;
    s16 rule;
    s16 species1;
    s16 species2;
    s16 unk16;
    u8 unk18[0xc];
    // btlv_effect.c passes it to the field
    s16 unk24;
    u8 unk26[0xa];
    BtlStudioTurnEvent turnEvents[3];
    u8 unk3C[0x6c];
    // Messages by turn
    s16 unkA8[20];
    s16 unkD0[20];
    BtlStudioScene scenes[];
};

// pokebw2's GetClientParty
BattleParty *func_ov093_021b9af8(BtlPokeCon *pokeCon, u32 clientId);
u8 func_ov093_021b9b04(BtlPokeCon *pokeCon, u32 clientId);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
u32 BtlSetup_IsBattleType(BtlMainModule *mainModule, u32 flag);
BOOL IsSwitchMode(BtlMainModule *mainModule);
u32 GetRunMode(BtlMainModule *mainModule);
BattleMon *GetClientMonData(BtlPokeCon *pokeCon, u8 clientId, u8 monId);
BattleMon *GetPokeParam(BtlPokeCon *pokeCon, u8 monId);
u8 GetSideFromOpposingMonID(u8 monId);
BattleParty *GetPartyData(BtlPokeCon *pokeCon, u32 clientId);
u32 ReturnZero(BtlMainModule *mainModule, u32 flag);
u8 func_ov093_021b8ddc(u8 monId);
void BattleClient_SubItem(BtlMainModule *mainModule, u8 clientId, u16 item);
void BattleClient_AddItem(BtlMainModule *mainModule, u8 clientId, u16 item);
BOOL func_ov093_021b6100(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov093_021b6434(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov093_021b6500(GameProc *proc, u32 *state, void *param, void *work);
void func_ov093_021b6848(BtlMainModule *mainModule, BtlSetup *setup);
void func_ov093_021b6a3c(BtlMainModule *mainModule, BtlSetup *setup);
BOOL func_ov093_021b6dd0(u32 *state, BtlMainModule *mainModule);
BOOL func_ov093_021b7488(u32 *state, BtlMainModule *mainModule);
BOOL func_ov093_021b75a8(BtlMainModule *mainModule, s32 *state);
BOOL func_ov093_021b7968(BtlMainModule *mainModule, s32 *state);
BOOL func_ov093_021b7bb0(BtlMainModule *mainModule, s32 *state);
BOOL func_ov093_021b7e18(BtlMainModule *mainModule, s32 *state);
u8 func_ov093_021b85a0(BtlMainModule *mainModule);
BOOL func_ov093_021b8724(BtlMainModule *mainModule, BattleMon *mon);
u32 func_ov093_021b873c(BtlMainModule *mainModule);
GameData *func_ov093_021b8748(BtlMainModule *mainModule);
void func_ov093_021b8750(BtlMainModule *mainModule, u8 clientId, BattleMon *mon);
u8 func_ov093_021b8de4(BtlMainModule *mainModule, u8 pos);
u8 func_ov093_021b9000(BtlMainModule *mainModule);
BagSave *func_ov093_021b90e8(BtlMainModule *mainModule);
void *func_ov093_021b90f0(BtlMainModule *mainModule);
BOOL func_ov093_021b90f8(BtlMainModule *mainModule);
BtlServerFlow *func_ov093_021ba820(BtlMainModule *mainModule);
u32 func_ov093_021b917c(BtlMainModule *mainModule);
void func_ov093_021b9184(BtlMainModule *mainModule);
u32 func_ov093_021b9244(BtlMainModule *mainModule);
void func_ov093_021b9460(BtlPokeCon *pokeCon, BtlMainModule *mainModule, u8 clientId);
void func_ov093_021b97a0(BtlPokeCon *pokeCon, BtlMainModule *mainModule, u8 clientId);
BattleMon *func_ov093_021b98a4(BtlPokeCon *pokeCon, u8 pos);
BattleMon *func_ov093_021b98ac(const BtlPokeCon *pokeCon, u8 pos);
// pokebw2's GetClientBattlerCount
// How many of the client's Pokemon are in battle at once. They come first in the party, so this is also the index of
// the first Pokemon waiting to switch in
u8 func_ov093_021b99b8(BtlMainModule *mainModule, u8 clientId);
BattleMon *func_ov093_021b9bf0(BattleParty *party, u8 index);
void func_ov093_021b9e38(BtlTrainerData *trainer, HeapID heapId, PlayerInfo *src);
PlayerInfo *func_ov093_021ba080(BtlMainModule *mainModule);
void func_ov093_021ba1b4(BtlMainModule *mainModule, u32 record);
void func_ov093_021ba1cc(BtlMainModule *mainModule, u32 record, u32 value);
void *func_ov093_021ba1e4(BtlMainModule *mainModule);
BOOL func_ov093_021ba1ec(BtlMainModule *mainModule);
BOOL func_ov093_021ba20c(BtlMainModule *mainModule);
void func_ov093_021ba250(BtlMainModule *mainModule, BattleMon *mon, u16 item);
void func_ov093_021ba2e4(BtlMainModule *mainModule);
u32 func_ov093_021ba54c(BtlMainModule *mainModule);
void func_ov093_021ba6c0(BtlMainModule *mainModule);
void *func_ov093_021b8630(BtlMainModule *mainModule);
u32 func_ov093_021b85ec(BtlMainModule *mainModule);
void func_ov093_021b9120(BtlMainModule *mainModule, u8 pos);

#endif // BWPORT_BATTLE_BTL_MAIN_H
