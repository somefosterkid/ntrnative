// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/save/save_control.h, the lines in use so
// far.
#ifndef BWPORT_SAVE_SAVE_CONTROL_H
#define BWPORT_SAVE_SAVE_CONTROL_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/heap.h"

// The area NPC data, which keeps the symbol encounters of the Entree Forest, encrypted
void decryptNpcData(AreaNPCSave *npcData);
void func_0200e06c(AreaNPCSave *npcData);
EncountSave *SaveControl_GetEncountSave(SaveControl *save);
PokeParty *SaveControl_GetPokePartySave(SaveControl *save);
// pokebw2's SaveControl_GetWorldTradeData
WorldTradeData *func_0200ad6c(SaveControl *save);
AdventureSave *getSaveAdventureDataBlk(SaveControl *save);
GameRecords *getTrainerCardInfoBlkAddress(SaveControl *save);
UnityTowerSurveySave *getUnityTower_SurveySaveBlkAddrress(SaveControl *save);
DreamWorldSave *getDreamWorldStuffAddress(SaveControl *save);
TrainerGameInfoSave *getTrainerGameInfoAddress(SaveControl *save);

// The musical save; the photo of the last musical is at its start
MusicalSave *getAddressOfMusicalDataInfo(SaveControl *save);
// pokebw2's func_0200ad5c
MusicalShot *func_0200a640(MusicalSave *musical);

// pokebw2's func_0200746c
BOOL func_020071f0(SaveControl *save);

AdventureTime *getSaveAdventureTimeBlock(SaveControl *save);

// Reads one of the two copies of a block from the card
BOOL func_02007248(SaveControl *save, u32 block, u32 copy, void *dest, u32 size);
// Puts a Pokémon from the Dream World in the Entree Forest: its species, a2, sex and form, a5 below 9, and size 2 for
// a big overworld sprite or 3
void func_0200e1a8(AreaNPCSave *npcData, u16 species, u16 a2, u8 sex, u8 form, u8 a5, u32 size);
void func_0200e27c(AreaNPCSave *npcData, u16 index);

void func_02007150(SaveControl *save);

u32 getBadgeCount(TrainerGameInfoSave *info);

u32 func_0200725c(SaveControl *save, u32 block, HeapID heapId);

void func_0200c5a4(void *work);

u32 func_0200b6e0(GameData *gameData, HeapID a1, u32 a2, u32 a3, u32 a4, u16 *a5, u16 *a6);

u32 func_0200e8b0(void);
void *func_0200e8d0(void *block);
BOOL func_0200e8d8(void *block);
void func_0200e8e0(void *block, u32 valid);

// Imports a downloaded musical, a step a frame until it returns TRUE
void *func_0200c5c8(GameData *gameData, void *data, u32 size, HeapID heapId);
BOOL func_0200c5f8(void *work);

u32 addCashToTotal(TrainerGameInfoSave *info, u32 amount);
// Stops at 0
u32 subCashFromTotal(TrainerGameInfoSave *info, u32 amount);

// Black 2's; twinned code confirms the values of SAVE_EXTRA_ID_HALL_OF_FAME in Black
// The extra saves, each of one block. Only the Hall of Fame has a backup copy
typedef enum {
    SAVE_EXTRA_ID_BATTLE_REC_0,
    SAVE_EXTRA_ID_BATTLE_REC_1,
    SAVE_EXTRA_ID_BATTLE_REC_2,
    SAVE_EXTRA_ID_BATTLE_REC_3,
    SAVE_EXTRA_ID_CGEAR_PICTURE,
    SAVE_EXTRA_ID_BATTLE_EXAMINATION,
    SAVE_EXTRA_ID_MUSICAL_DIST,
    SAVE_EXTRA_ID_POKEDEX_SKIN,
    SAVE_EXTRA_ID_HALL_OF_FAME,
    // Eight films
    SAVE_EXTRA_ID_POKEWOOD_0,
    // Three downloaded tournaments
    SAVE_EXTRA_ID_TOURNAMENT_0 = 17,
    SAVE_EXTRA_ID_TOURNAMENT_1,
    SAVE_EXTRA_ID_TOURNAMENT_2,
    // A block of 0xc8 bytes
    SAVE_EXTRA_ID_20,
    SAVE_EXTRA_ID_MAX
} SaveExtraID;

void func_02007324(SaveControl *save, u32 block, u32 heapId, void *buffer, u32 size);
void freeIntermediateSaveExtraBlksAfterLoad2(SaveControl *save, u32 block);
u32 func_0200ba38(u8 a0);
// The sizes of a downloaded C-Gear skin and Pokédex skin, and a Pokédex skin block's valid flag
u32 func_0200c6e4(void);

// Black 2's, not checked against Black
#define SAVE_CONTROL_ERROR_PARTIAL 0x1

// Black 2's, not checked against Black
#define SAVE_CONTROL_ERROR_BROKEN 0x2
#define SAVE_CONTROL_ERROR_EXTRA(extraId) (1 << ((extraId) + 2))
#define SAVE_CONTROL_ERROR_GIFTS_BROKEN 0x800
#define SAVE_CONTROL_ERROR_KEYS_BROKEN 0x1000000

// Black 2's; twinned code confirms the offsets of newGame, unk2, unk3, sv and sv_extra in Black
struct SaveControl {
    // No save was loaded, so the game starts anew
    u8 newGame;
    // The card holds a save
    u8 dataExists;
    // Cleared when both copies of the save loaded right
    u8 unk2;
    // func_02008df4 at the start of a save, which canceling restores
    u8 unk3;
    // What SaveOutside told of the data outside the save
    u8 giftsLoaded;
    u8 giftsBroken;
    u8 keysLoaded;
    u8 keysBroken;
    // The next save calls func_020115f0, which marks Dream Radar's data on the card as read
    u8 unk8;
    u32 errors;
    SaveData *sv;
    // Black's nine extra saves, the ones before Black 2's films (sv_extra_blk_data's): Black allocates 0x38 bytes, and
    // has no Memory Link pointer after them
    SaveData *sv_extra[SAVE_EXTRA_ID_POKEWOOD_0];
}; // size: 0x38

void func_02007230(SaveControl *save);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// Frees the save data and its control
void SaveControl_Free(void);
u32 SaveControl_GetStatus(SaveControl *save);
u32 getCash(TrainerGameInfoSave *info);
// Returns a pointer to one of the save's blocks
void *SaveControl_GetBlockPtr(SaveControl *save, u32 block);
// Returns the save block 0x20
void *func_02009210(SaveControl *save);
// Returns the save block 0x20 of the game data's save
void *func_0200921c(GameData *gameData);
// pokebw2's CGearSettings_IsOn
BOOL func_020090e0(CGearSettings *settings);
BOOL SaveControl_IsDataAlreadyPresent(SaveControl *save);
HighLinkSave *getHighLinkBlockAddress(SaveControl *save);
RecordSave *getRecordBlkAddress(SaveControl *save);
TrainerCardSave *getTrainerCardDataBlkAddress(GameData *gameData);
void func_020071a8(SaveControl *save);
EventWork *getConstDataBlock(SaveControl *save);
u16 func_0200c32c(TrainerGameInfoSave *info);
SaveControl *SaveControl_GetInstance(void);
// Save block 0x42, which keeps the rival's name
RivalDataSave *getHollow_RivalData(SaveControl *save);
// pokebw2's Misc_AddMetTrainer
// The C-Gear's save block (getCGearDataBlkAddress)
// Copies the C-Gear's record at index, 4 u16
void func_0200ed54(RivalDataSave *rivalData, u32 id);
// The area NPC data, which keeps the symbol encounters of the Entree Forest, encrypted
AreaNPCSave *getAreaNPCData(SaveControl *save);
u32 func_0200e164(AreaNPCSave *npcData, u32 kind);
u32 func_0200e184(u32 index);
void func_0200e18c(AreaNPCSave *npcData, u32 index);
// Whether the Entree Forest Pokémon, packed as EntreeForestPokemon is, is valid, checking its form when check is set
BOOL func_0200e2bc(SaveControl *save, u32 *pokemon, HeapID heapId, BOOL check);
void *getAddressOfExtraSaveBlk(SaveControl *save, u32 block, u32 arg2);
void freeIntermediateSaveExtraBlksAfterLoad(SaveControl *save, u32 block);
u32 func_0200b5cc(SaveControl *save, HeapID heapId, u32 *result, u32 block);
// A Pokédex skin block's characters and palette, and whether it is valid
void *func_0200e8cc(void *block);
void *func_0200c6d8(void *work);
u32 func_0200c6dc(void *work);
void func_0200c2f0(TrainerGameInfoSave *info, u32 item, u32 bits);
u8 func_0200c308(TrainerGameInfoSave *info, u32 item);
u32 func_0200c31c(TrainerGameInfoSave *info);
void func_0200c324(TrainerGameInfoSave *info, u32 value);
void func_0200c334(TrainerGameInfoSave *info, u8 question);
u8 func_0200c33c(TrainerGameInfoSave *info, u8 index);
// A Pokédex skin block's characters and palette, and whether it is valid
u8 func_0200c224(TrainerGameInfoSave *info);
void func_0200ba58(void);
void func_0200ba68(void);
u32 func_020072cc(SaveControl *save, u32 block, u16 heapId, void *buffer, u32 size);
// Reads the downloaded musical's save block (6) into a new work, its archive and the archive's size, and frees it
void *func_0200c52c(GameData *gameData, HeapID heapId);
// pokebw2's TrainerGameInfo_GetGreeting
const u16 *func_0200c1f4(TrainerGameInfoSave *info);
// The save data Game Sync uploads, and its size
void *func_020071d8(SaveControl *save, u32 *size);
u32 func_020071e8(SaveControl *save);
// pokebw2's TrainerGameInfo_SetPalParkResult
void func_0200c1b0(TrainerCardSave *trainerCard, u32 result);
// pokebw2's TrainerGameInfo_GetPalParkHighScore
// Poké Transfer's high score, 28 bits
u32 func_0200c184(TrainerCardSave *trainerCard);
// pokebw2's TrainerGameInfo_GetPalParkResult
// How the last Poké Transfer session ended, 4 bits, which the Poké Transfer Lab's scripts react to
u8 func_0200c1a4(TrainerCardSave *trainerCard);
// pokebw2's TrainerGameInfo_SetPalParkHighScore
void func_0200c18c(TrainerCardSave *trainerCard, u32 highScore);
// pokebw2's SaveControl_Create
// Loads the save, its extra saves' state and the data outside the save
SaveControl *func_02006f20(HeapID heapId);
// Loads the save again, returning a SAVE_LOAD_RESULT_*
u32 func_020070c8(SaveControl *save);
u32 func_020071c0(SaveControl *save);
void func_02007214(SaveControl *save, u32 value);
u8 func_02007218(SaveControl *save);
void func_0200721c(SaveControl *save, u32 value);
void func_0200723c(SaveControl *save, u32 *a1, u32 *a2);
void func_02007374(SaveControl *save, u32 block);
u32 func_020073b0(SaveControl *save, u32 extra_id);
BOOL func_02007400(SaveControl *save, u32 extra_id, u32 heapId);

const u16 *func_0200c20c(TrainerGameInfoSave *info);

void func_02007220(SaveControl *save);
u8 func_02007210(SaveControl *save);

#endif // BWPORT_SAVE_SAVE_CONTROL_H
