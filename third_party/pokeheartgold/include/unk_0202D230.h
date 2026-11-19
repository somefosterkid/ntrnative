#ifndef POKEHEARTGOLD_UNK_0202D230_H
#define POKEHEARTGOLD_UNK_0202D230_H

#include "constants/heap.h"

#include "frontier_data.h"
#include "frontier_pokemon.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "unk_02069528.h"
#include "wifi_battle_tower_match_candidate.h"

struct WifiBattleTowerDownloadData;
struct BattleTowerSave;

typedef struct WifiBattleTowerIndices {
    u8 rank;
    u8 opponentIdx;
} WifiBattleTowerIndices;

void sub_0202D234(struct BattleTowerSave *save);
void sub_0202D240(FrontierData *frontierData);
void sub_0202D254(MailMessage *messages);
void sub_0202D274(struct WifiBattleTowerDownloadData *downloadData);
u32 sub_0202D284(struct BattleTowerSave *save, int fieldId, void *outBuf);
void sub_0202D308(struct BattleTowerSave *save, int fieldId, const void *inBuf);
void sub_0202D3B0(struct BattleTowerSave *save, u8 param1, u16 param2, u16 param3);
BOOL sub_0202D3DC(struct BattleTowerSave *save);
void sub_0202D3E4(struct BattleTowerSave *save, BOOL didSave);
u8 sub_0202D450(FrontierData *frontierData, int op);
u8 sub_0202D488(FrontierData *frontierData, int op);
void sub_0202D4B8(FrontierData *frontierData, int teamIdx, FrontierPokemon *mons);
void sub_0202D4DC(FrontierData *frontierData, int teamIdx, FrontierPokemon *outBuf);
u16 sub_0202D4FC(FrontierData *frontierData, struct BattleTowerSave *save);
u16 sub_0202D564(FrontierData *frontierData);
u8 sub_0202D568(FrontierData *frontierData);
u16 sub_0202D57C(FrontierData *frontierData, u16 challengeMode, int op);
u16 sub_0202D5C4(FrontierData *frontierData, u16 challengeMode, u16 roomNum);
BOOL sub_0202D5DC(FrontierData *frontierData, u16 bitIdx, int op);
void sub_0202D638(FrontierData *frontierData, u32 rngState);
u32 sub_0202D63C(FrontierData *frontierData);
void sub_0202D640(SaveData *saveData, int a1, MailMessage *mailMessage);
BOOL sub_0202D7B0(struct WifiBattleTowerDownloadData *downloadData);
u32 sub_0202D7B8(struct WifiBattleTowerDownloadData *downloadData);
void sub_0202D8D0(struct WifiBattleTowerDownloadData *downloadData, WifiBattleTowerIndices *outIndices);
WifiBattleTowerMatchCandidate *sub_0202D8E4(struct WifiBattleTowerDownloadData *downloadData, enum HeapID heapID);
struct BattleTowerSave *sub_0202D908(SaveData *saveData);
struct WifiBattleTowerDownloadData *sub_0202D928(SaveData *saveData);
MailMessage *sub_0202D660(SaveData *saveData, u32 a1);

#endif // POKEHEARTGOLD_UNK_0202D230_H
