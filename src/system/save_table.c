// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/save_table.c, with Black's tables: the
// blocks of the main save, and the one block of each extra save. The file has no code and no name in the ROM;
// save_table.c is pokebw2's guess. That it is a file apart from save_control.c shows in the order of its tables,
// which MWCC only gives when the file defines nothing else.
#include "types.h"
#include "field/field_actor.h"
#include "field/unity_tower.h"
#include "gfl/backup_system.h"
#include "gfl/savedata.h"
#include "pml/mail.h"
#include "pml/poke_party.h"
#include "save/adventure.h"
#include "save/battle_examination.h"
#include "save/bag.h"
#include "save/battle_rec.h"
#include "save/box.h"
#include "save/bsubway_save.h"
#include "save/cgear_picture_save.h"
#include "save/chatter.h"
#include "save/dream_world.h"
#include "save/encounter.h"
#include "save/event_work.h"
#include "save/field_actor_savedata.h"
#include "save/gimmick_savedata.h"
#include "save/hall_of_fame.h"
#include "save/high_link.h"
#include "save/misc_savedata.h"
#include "save/musical_dist_save.h"
#include "save/musical_savedata.h"
#include "save/mystery_gift.h"
#include "save/player_savedata.h"
#include "save/pokedex.h"
#include "save/pokedex_skin_save.h"
#include "save/records.h"
#include "save/save_control.h"
#include "save/save_table.h"
#include "save/survey_savedata.h"
#include "save/symbol_save.h"
#include "save/timesig_savedata.h"
#include "save/tournament_block.h"
#include "save/traded_pokemon.h"
#include "save/trainer_card.h"
#include "save/trainer_data_savedata.h"
#include "save/trainer_game_info.h"
#include "save/trainer_log.h"
#include "save/trial_house.h"
#include "save/wifi_list.h"
#include "save/worldtrade_data.h"
#include "system/pms.h"
#include "system/pms_word.h"

// The blocks' functions that no header declares yet
void func_02007454(BoxMainSave *main);
u32 func_02007cb0(void);
void func_02007cb8(BagSave *bag);
u32 func_020097bc(void);
u32 func_020090b0(void);
void func_020090b4(void *block);
u32 func_02009930(void);
u32 func_0200e9dc(void);
void func_0200e9e0(void *block);
u32 func_0200e6b0(void);
u32 func_0200e874(void);
u32 func_0200e8e8(void);
void func_0200e8ec(void *block);

// Where the main save is on the card, and the magic number in every save's footer
#define SAVE_MAIN_ADDR 0
#define SAVE_BACKUP_ADDR 0x24000
#define SAVE_SIZE 0x24000
#define SAVE_MAGIC 0x31053527

typedef void (*SaveBlockInit)(void *block);

// The main save's blocks, by ID
static const SaveBlockDef data_0209da28[] = {
    { 0, BoxMainSave_GetBlockSize, (SaveBlockInit)func_02007454 },
    { 1, getSizeofBox, initFullBox },
    { 2, getSizeofBox, initFullBox },
    { 3, getSizeofBox, initFullBox },
    { 4, getSizeofBox, initFullBox },
    { 5, getSizeofBox, initFullBox },
    { 6, getSizeofBox, initFullBox },
    { 7, getSizeofBox, initFullBox },
    { 8, getSizeofBox, initFullBox },
    { 9, getSizeofBox, initFullBox },
    { 10, getSizeofBox, initFullBox },
    { 11, getSizeofBox, initFullBox },
    { 12, getSizeofBox, initFullBox },
    { 13, getSizeofBox, initFullBox },
    { 14, getSizeofBox, initFullBox },
    { 15, getSizeofBox, initFullBox },
    { 16, getSizeofBox, initFullBox },
    { 17, getSizeofBox, initFullBox },
    { 18, getSizeofBox, initFullBox },
    { 19, getSizeofBox, initFullBox },
    { 20, getSizeofBox, initFullBox },
    { 21, getSizeofBox, initFullBox },
    { 22, getSizeofBox, initFullBox },
    { 23, getSizeofBox, initFullBox },
    { 24, getSizeofBox, initFullBox },
    { 25, func_02007cb0, (SaveBlockInit)func_02007cb8 },
    { 26, PokeParty_GetSaveDataSize, (SaveBlockInit)PokeParty_Init },
    { 27, getSizeofTrainerData, (SaveBlockInit)func_02008704 },
    { 28, PlayerSave_GetBlockSize, (SaveBlockInit)PlayerSave_Init },
    { 29, func_02009454, (SaveBlockInit)func_0200945c },
    { 30, func_020097bc, (SaveBlockInit)initWifi },
    { 31, PalPadFriendList_GetSize, (SaveBlockInit)PalPadFriendList_Init },
    { 32, func_020090b0, func_020090b4 },
    { 33, func_020089a8, (SaveBlockInit)func_020089b0 },
    { 34, func_02009ed4, (SaveBlockInit)func_02009edc },
    { 35, func_02009930, (SaveBlockInit)func_02009230 },
    { 36, func_02007b00, (SaveBlockInit)initChatotChatter },
    { 37, getSizeofAdventureDataBlk, (SaveBlockInit)initAdventureDataBlk },
    { 38, func_02008c20, (SaveBlockInit)initTrainerCardInfoBlk },
    { 39, func_02024934, (SaveBlockInit)func_02024938 },
    { 40, func_02008ffc, (SaveBlockInit)func_02009004 },
    { 41, ActorSave_GetBlockSize, (SaveBlockInit)ActorSave_Init },
    { 42, func_0200a53c, (SaveBlockInit)func_0200a544 },
    { 43, func_0200a840, (SaveBlockInit)func_0200a848 },
    { 44, func_0200ab24, (SaveBlockInit)func_0200ab2c },
    { 45, EventWork_GetBlockSize, (SaveBlockInit)EventWork_Init },
    { 46, func_0200ad78, (SaveBlockInit)func_0200ad7c },
    { 47, getSizeofTourneyBlock, clearTournamentBlock },
    { 48, GimmickSave_GetBlockSize, (SaveBlockInit)GimmickSave_Init },
    { 49, func_0200ba78, (SaveBlockInit)func_0200ba80 },
    { 50, getSizeofBreedingBlock, (SaveBlockInit)setEggArea },
    // The only block whose functions are an overlay's, the field's (overlay 10)
    { 51, func_ov010_0216e474, (SaveBlockInit)func_ov010_0216e47c },
    { 52, func_0200c054, (SaveBlockInit)func_0200c058 },
    { 53, getSizeofProbabilityTable, (SaveBlockInit)probability },
    { 54, func_0200c3f4, func_0200c3f8 },
    { 55, getSizeofPokedexData, (SaveBlockInit)func_0200c9f4 },
    { 56, EncountSave_GetBlockSize, EncountSave_Init },
    { 57, func_0200d858, (SaveBlockInit)func_0200d85c },
    { 58, func_0200da64, (SaveBlockInit)func_0200da6c },
    { 59, func_0200dd10, (SaveBlockInit)func_0200dd18 },
    { 60, func_0200e9dc, func_0200e9e0 },
    { 61, getSizeofNpcData, (SaveBlockInit)initNpcData },
    { 62, func_0200e6b0, (SaveBlockInit)func_0200e6b4 },
    { 63, func_0200e368, (SaveBlockInit)func_0200e370 },
    { 64, func_0200e874, (SaveBlockInit)UnityTowerSave_Init },
    { 65, func_0200e8e8, func_0200e8ec },
    { 66, func_0200ea14, (SaveBlockInit)func_0200ea2c },
    { 67, func_0200e730, (SaveBlockInit)func_0200e734 },
    { 68, getSizeofHollow_RivalData, (SaveBlockInit)clearHollowRivalData },
};

// The one block of each extra save
static const SaveBlockDef data_0209d8cc[] = {
    { 0, getSizeOfVSPlayer, (SaveBlockInit)clearVSPlayerBlk },
};

static const SaveBlockDef data_0209d92c[] = {
    { 0, getSizeOfVSPlayer, (SaveBlockInit)clearVSPlayerBlk },
};

static const SaveBlockDef data_0209d920[] = {
    { 0, getSizeOfVSPlayer, (SaveBlockInit)clearVSPlayerBlk },
};

static const SaveBlockDef data_0209d914[] = {
    { 0, getSizeOfVSPlayer, (SaveBlockInit)clearVSPlayerBlk },
};

static const SaveBlockDef data_0209d908[] = {
    { 0, func_0200c6e4, (SaveBlockInit)func_0200c6ec },
};

static const SaveBlockDef data_0209d8d8[] = {
    { 0, func_0200e588, (SaveBlockInit)func_0200e590 },
};

static const SaveBlockDef data_0209d8f0[] = {
    { 0, func_0200c520, func_0200c528 },
};

static const SaveBlockDef data_0209d8e4[] = {
    { 0, func_0200e8b0, func_0200e8b8 },
};

static const SaveBlockDef data_0209d8fc[] = {
    { 0, func_0200ea0c, (SaveBlockInit)func_0200ea1c },
};

const SaveDataParams data_0209d938 = {
    data_0209da28, NELEMS(data_0209da28), SAVE_MAIN_ADDR, SAVE_BACKUP_ADDR, SAVE_SIZE, SAVE_MAGIC,
};

// The extra saves follow the main save's two copies. Black has no Black 2's films, tournaments and block 20: its
// last extra save is the Hall of Fame, which has a backup copy, as the first battle recording has
const SaveDataParams sv_extra_blk_data[SAVE_EXTRA_ID_HALL_OF_FAME + 1] = {
    { data_0209d8cc, 1, 0x48000, 0x4a000, 0x2000, SAVE_MAGIC },
    { data_0209d92c, 1, 0x4c000, 0x4c000, 0x2000, SAVE_MAGIC },
    { data_0209d920, 1, 0x4e000, 0x4e000, 0x2000, SAVE_MAGIC },
    { data_0209d914, 1, 0x50000, 0x50000, 0x2000, SAVE_MAGIC },
    { data_0209d908, 1, 0x52000, 0x52000, 0x3000, SAVE_MAGIC },
    { data_0209d8d8, 1, 0x55000, 0x55000, 0x1000, SAVE_MAGIC },
    { data_0209d8f0, 1, 0x56000, 0x56000, 0x20000, SAVE_MAGIC },
    { data_0209d8e4, 1, 0x76000, 0x76000, 0x6800, SAVE_MAGIC },
    { data_0209d8fc, 1, 0x7c800, 0x7e000, 0x1800, SAVE_MAGIC },
};
