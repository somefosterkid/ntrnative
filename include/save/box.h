// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/save/box.h, the lines in use so far.
#ifndef BWPORT_SAVE_BOX_H
#define BWPORT_SAVE_BOX_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/heap.h"
#include "pml/poke_party.h"

u32 howManyNormalPokesAreInAllBoxes(BoxSaveAccessor *boxes);
u32 BoxSaveAccessor_GetLastOpenedBox(BoxSaveAccessor *boxes);
// How many boxes are open: 8, 16 or 24
u8 BoxSaveAccessor_GetAvailableBoxCount(BoxSaveAccessor *boxes);
// Opens 8 more boxes, up to 24, and returns how many are open
u8 BoxSaveAccessor_UnlockMoreBoxes(BoxSaveAccessor *boxes);
void saveLastOpenedBoxIdx(BoxSaveAccessor *boxes, u32 box);
u32 howManyPokesInGeneralAreInBox(BoxSaveAccessor *boxes, u32 box);

// The day care, save block 0x32: its two Pokémon and whether an egg waits
#define DAY_CARE_SLOTS 2

typedef struct {
    BOOL used;
    PartyPkm pkm;
    u32 steps;
} DayCareSlot;

// Twinned code confirms the offsets of slots and eggSpawned in Black. Black's block ends after eggSpawned: 0x1cc bytes
// (getSizeofBreedingBlock), so Black 2's seed isn't in it
struct DayCareSave {
    DayCareSlot slots[DAY_CARE_SLOTS];
    BOOL eggSpawned;
};

void DayCareSave_SetSpawnFlag(DayCareSave *dayCare);
// A slot's Pokémon, or the first slot's when the slot is empty
PartyPkm *DayCareSave_GetPkm(DayCareSave *dayCare, int slot);
void DayCareSave_AddPkm(DayCareSave *dayCare, int slot, const PartyPkm *pkm);
void DayCareSave_DisablePkmStatus(DayCareSave *dayCare, int slot);
BOOL DayCareSave_GetPkmStatus(DayCareSave *dayCare, u32 slot);
u32 DayCareSave_GetPkmCount(DayCareSave *dayCare);
u32 DayCareSave_GetPkmStepCounter(DayCareSave *dayCare, u32 slot);
// Adds steps to the counters of the Pokémon in the day care
void DayCareSave_UpdateStepCount(DayCareSave *dayCare, u32 steps);
// pokebw2's DayCareSave_SwapSlots
// Swaps two slots
void func_0200bcec(DayCareSave *dayCare, int slot1, int slot2);

// pokebw2's BattleBoxSave_ClearFlags
void func_0200bbd8(BattleBoxSave *battleBox, u32 mask);

// pokebw2's BattleBoxSave_CheckFlags
// Whether any of the flags is set
BOOL func_0200bbfc(BattleBoxSave *battleBox, u16 flags);
// pokebw2's BattleBoxSave_SetFlags
// Sets the flags of the mask in the battle box, and clears them
void func_0200bbc8(BattleBoxSave *battleBox, u32 mask);

// Black 2's, not checked against Black
// The battle box, save block 0x31: one box of six Pokémon, its name and its flags
#define BATTLE_BOX_COUNT 1

#define BATTLE_BOX_SLOTS 6

// Black 2's, not checked against Black
#define BATTLE_BOX_NAME_LENGTH 20

// Black 2's; matching code confirms the offsets of pkm, name and flags, and its size in Black
struct BattleBoxSave {
    BoxPkm pkm[BATTLE_BOX_COUNT][BATTLE_BOX_SLOTS];
    u16 name[BATTLE_BOX_NAME_LENGTH];
    u16 flags;
    u8 padding35A[2];
};

BoxPkm *getBoxSlotAddress(BattleBoxSave *battleBox, u32 box, u32 slot);
// pokebw2's BattleBoxSave_GetSize
u32 func_0200ba78(void);
// pokebw2's BattleBoxSave_HasPkm
// Whether the battle box has a Pokémon
BOOL func_0200bba8(BattleBoxSave *battleBox);

void BoxSaveAccessor_SwapPkms(BoxSaveAccessor *boxes, u32 box1, u32 slot1, u32 box2, u32 slot2);
// A box's wallpaper
u32 getBoxNumFromIdx(BoxSaveAccessor *boxes, u32 box);
void getBoxNameFromStrbuf(BoxSaveAccessor *boxes, u32 box, StrBuf *name);

// pokebw2's BoxSaveAccessor_CountEmptySlots
// How many Pokémon the boxes have room for
u32 func_02007774(BoxSaveAccessor *boxes);

// The PC boxes, save blocks 0 (the main block) and 1 to 24 (the boxes). The names of the limits are the ones the
// assertions of box_savedata.c keep
#define BOX_MAX_TRAY 24

#define BOX_MAX_POS 30
#define BOX_NAME_LENGTH 20

// The box that is open, where a function takes a box
#define BOXDAT_TRAYNUM_CURRENT 0xffffffff

typedef struct BoxMainSave BoxMainSave;

u32 BoxMainSave_GetBlockSize(void);
// The size of a box
u32 getSizeofPokeBox(void);
// A box's block: its size, its init and its pointer
void initFullBox(void *tray);
// pokebw2's BoxSaveAccessor_SetWallpaper
void func_02007838(BoxSaveAccessor *boxes, u32 box, u32 wallpaper);
// pokebw2's BoxSaveAccessor_SetFlags
// Sets flags of the main block: the sets of extra wallpapers that are unlocked
void func_02007a68(BoxSaveAccessor *boxes, u32 flags);
// pokebw2's BoxSaveAccessor_CheckFlags
// Whether a set of extra wallpapers (1 or 2) is unlocked
BOOL func_02007a80(BoxSaveAccessor *boxes, u32 set);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
u32 getSizeofBox(void);
void *BoxSaveAccessor_GetBox(BoxSaveAccessor *boxes, u32 box);
u32 howManyTotalPokesAreInBoxes(BoxSaveAccessor *boxes);
BOOL BoxSaveAccessor_InsertPkm(BoxSaveAccessor *boxes, BoxPkm *pkm);
BOOL BoxSaveAccessor_InsertPkmCore(BoxSaveAccessor *boxes, u32 box, BoxPkm *pkm);
void BoxSaveAccessor_ClearPkm(BoxSaveAccessor *boxes, u32 box, u32 slot);
BOOL BoxSaveAccessor_GetNextFreeBoxSlot(BoxSaveAccessor *boxes, int *box, int *slot);
BoxPkm *BoxSaveAccessor_GetPkm(BoxSaveAccessor *boxes, u32 box, u32 slot);
// Copies a box's name into a string, and back
void loadBoxNameToStrbuf(BoxSaveAccessor *boxes, u32 box, StrBuf *str);
BOOL BoxSaveAccessor_SetPkm(BoxSaveAccessor *boxes, u32 box, u32 slot, BoxPkm *pkm);
// The battle box, save block 0x31
BattleBoxSave *getBattleBox(SaveControl *save);
// Allocates a party of the battle box's Pokémon
PokeParty *convertBoxedPokeSetToParty(BattleBoxSave *battleBox, HeapID heapId);
// Store the party in the Battle Box
void copySelectedPkmToBattleBlk(BattleBoxSave *battleBox, PokeParty *party);
// The day care, save block 0x32
DayCareSave *getDaycareBlockAddress(SaveControl *save);
// Draws a new seed and clears the egg
void DayCareSave_ResetSeed(DayCareSave *dayCare);
BOOL DayCareSave_CheckSpawnFlag(DayCareSave *dayCare);
u32 getSizeofBreedingBlock(void);
void setEggArea(DayCareSave *dayCare);
// pokebw2's BattleBoxSave_Init
void func_0200ba80(BattleBoxSave *battleBox);
u32 countEmptySlotsInBox(BoxSaveAccessor *boxes, u32 box);
BoxSaveAccessor *BoxSaveAccessor_Create(HeapID heapId, SaveControl *save);
void BoxSaveAccessor_Free(BoxSaveAccessor *boxes);

#endif // BWPORT_SAVE_BOX_H
