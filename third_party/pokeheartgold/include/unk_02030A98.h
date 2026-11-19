#ifndef POKEHEARTGOLD_UNK_02030A98_H
#define POKEHEARTGOLD_UNK_02030A98_H

#include "save.h"

// Only forward declarations: save_frontier.h (the full Frontier save block) is included by the files that read its fields.
struct BattleArcadeSave;
struct BattleArcadeStreakFlags;
struct BattleCastlePersistentSave;
struct BattleCastleSave;
struct BattleFactorySave;
struct BattleFactoryStreakFlags;
struct BattleHallSave;
struct BattleHallStreakFlags;
struct FrontierSave;

struct UnkStruct_02030A98 {
    u8 filler_000[0xBA0];
}; // size = 0xBA0

struct BattleFactorySave *sub_0203094C(SaveData *saveData);
void sub_02030AA4(struct BattleFactoryStreakFlags *flags, u8 field, u8 challengeType, u8 *value);
u32 sub_02030AD4(struct BattleFactoryStreakFlags *flags, u8 field, u8 challengeType, void *unused);
struct BattleFactoryStreakFlags *sub_02030AE8(SaveData *saveData);
struct BattleHallSave *sub_02030B04(SaveData *saveData);
struct BattleHallStreakFlags *sub_02030C5C(SaveData *saveData);
void sub_02030C6C(struct BattleHallStreakFlags *flags, u8 field, u8 challengeType, u8 unused, u8 *value);
u32 sub_02030CA0(struct BattleHallStreakFlags *flags, u8 field, u8 challengeType, u8 unused, void *unused2);
struct BattleCastleSave *sub_02030CC8(SaveData *saveData);
struct BattleCastlePersistentSave *sub_02030E08(SaveData *saveData);
void sub_02030E18(struct BattleCastlePersistentSave *save, u8 field, u8 challengeType, u8 unused, u8 *value);
u32 sub_02030E58(struct BattleCastlePersistentSave *save, u8 field, u8 challengeType, u8 unused, void *unused2);
struct BattleArcadeSave *sub_02030E88(SaveData *saveData);
struct BattleArcadeStreakFlags *sub_02030FA0(SaveData *saveData);
void sub_02030FB0(void *, u8, u8, u8, u8 *);
u32 sub_02030FE4(void *, u8, u8, u8, u32);
struct FrontierSave *Save_Frontier_GetStatic(SaveData *saveData);
void Save_Frontier_Load(SaveData *saveData);
u16 FrontierSave_GetStat(struct FrontierSave *, int, int);
void Save_Frontier_Commit(SaveData *saveData);
u16 sub_02031108(struct FrontierSave *, int, int, u16);
void sub_0203126C(struct FrontierSave *a0, u32 a1, u32 a2, u16 a3);
u32 *sub_020312C4(SaveData *saveData, u32 a1, u32 *out);
u16 sub_020312E0(SaveData *saveData, u32 *a1, u32 a2, u32 a3);
u32 sub_02030E98(u32 a0);
void sub_02031188(void);

#endif // POKEHEARTGOLD_UNK_02030A98_H
