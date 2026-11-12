// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/pml/poke_party.h, the lines in use so far.
#ifndef BWPORT_PML_POKE_PARTY_H
#define BWPORT_PML_POKE_PARTY_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"
#include "pml/mail.h"
#include "stddef.h"

PokeParty *PokeParty_Create(HeapID heapId);
// The trainer ID and the PID are 64-bit so that they can hold these values beside any 32-bit one. The trainer ID is
// random, or one with which the PID isn't shiny
#define PKM_ID_RANDOM 0xffffffffffffffffULL
#define PKM_PID_RANDOM 0xffffffff00000000ULL
#define PKM_IVS_RANDOM -1
void PokeParty_CreatePkm(PartyPkm *pkm, u16 species, u16 level, u64 trainerId, s32 ivs, u64 pid);
// A field of a Pokémon, PKM_PARAM_*. The functions take it as an enum, swan's PkmField, and MWCC doesn't share a sum
// that makes one between two calls, as it would an integer
typedef enum {
    // Black's: two fewer than Black 2's 0xb4 (constants/pokemon.h)
    PKM_PARAM_COUNT = 0xb2,
} PkmField;

u32 PokeParty_GetParam(const PartyPkm *pkm, PkmField param, void *buffer);
// The size of a Pokémon's data
u32 PokeParty_GetPkmRawSize(void);
// A field that is not a number (a name as a StrBuf or a raw string, the mail) takes a pointer to its value, which the
// setter copies from: uintptr_t, Black's 32-bit u32 that holds a pointer whole on a 64-bit target too. pokebw2: u32
void PokeParty_SetParam(PartyPkm *pkm, PkmField param, uintptr_t value);
void PML_PkmSetParam(BoxPkm *pkm, u32 param, uintptr_t value);
// The size of a boxed Pokémon's data
u32 PML_GetPkmRawSize(void);
void PML_PkmInit(BoxPkm *pkm);
void PML_CreateTempPkm(BoxPkm *pkm, u16 species, u16 level, u64 id);
u32 PokeParty_GetSaveDataSize(void);
u32 PML_GenPID(u32 seed, u16 species, u16 form, u32 sex, u32 ability, u32 a5);
void PokeParty_SetHiddenAbil(PartyPkm *pkm, u32 species, u32 form);
void PokeParty_SetDefaultMoves(PartyPkm *pkm);
// Teaches a move, and returns 0xffff when all four slots are full
u16 PokeParty_LearnMove(PartyPkm *pkm, u16 move);
// Replaces the last move
void PokeParty_SetLastMove(PartyPkm *pkm, u16 move);
// pokebw2's func_0201d620
BoxPkm *func_020185f0(PartyPkm *pkm);
void PokeParty_InitCore(PokeParty *party, u32 capacity);
PartyPkm *PokeParty_GetPkm(PokeParty *party, u32 index);
int PokeParty_GetPkmCount(PokeParty *party);
// Whether the Pokemon in a slot can battle
BOOL func_0201ab34(PokeParty *party, u32 index);
int PokeParty_GetCapacity(PokeParty *party);
BOOL PokeParty_AddPkm(PokeParty *party, PartyPkm *pkm);
BOOL PokeParty_ChangeForme(PartyPkm *pkm, u16 forme);
void func_0201b7b4(PartyPkm *pkm, u16 item, u16 zoneId, HeapID heapId);
// Whether a move's PP is below its maximum, and restoring amount of it
u8 PokeParty_CheckPPNeedsReplenish(PartyPkm *pkm, u32 slot);
BOOL PokeParty_AddPP(PartyPkm *pkm, u32 slot, u32 amount);
u32 GetStatusCond(PartyPkm *pkm);
void PokeParty_RecalcStats(PartyPkm *pkm);
// Allocates a Pokémon that is not in a party, with random IVs and PID. trainerId is a 32-bit ID or one of the PKM_ID_
// values
PartyPkm *PokeParty_NewTempPkm(u16 species, u16 level, u64 trainerId, HeapID heapId);
int howManyPartyPokesAreNotEggs(PokeParty *party);

BOOL doesPkmHaveLevelMoveToLearn(const u16 *moves);

void TransformVsPokePartyBySeason(GameData *gameData, PokeParty *party, u8 season);

void FriendshipManagerCalc(PartyPkm *pkm, u32 reason, u16 zoneId, u16 heapId);
// pokebw2's FriendshipManagerCalcParty
void func_0201b818(PokeParty *party, u16 zoneId, HeapID heapId);

// Restores a Pokémon's HP and PP and cures its status
void PokeParty_Recover(PartyPkm *pkm);

BOOL CheckSpeciesInParty(PokeParty *party, u32 species);

// Whether the personality is shiny for the trainer ID
BOOL PML_UtilPIDIsRare(u32 id, u32 pid);

// pokebw2's PML_PkmGetSpriteForme
// The form a sprite shows: 0 for the four species whose forms share one sprite
u8 func_02019d58(u16 species, u8 form);

// Black 2's, not checked against Black
// A Pokémon's data, encrypted unless decrypted for a series of reads and writes. Names from swan
typedef struct {
    u8 rawData[32];
} PkmBufferChunk;

// Black 2's, not checked against Black
typedef struct {
    PkmBufferChunk chunks[4];
} PkmBuffer;

// Black 2's; twinned code confirms the offsets of pid, partyDecrypted, boxDecrypted, badEgg, checksum and
// contentBuffer, and its size in Black
struct BoxPkm {
    u32 pid;
    u16 partyDecrypted : 1;
    u16 boxDecrypted : 1;
    u16 badEgg : 1;
    u16 unk4_3 : 13;
    u16 checksum;
    PkmBuffer contentBuffer;
};

// Black 2's; twinned code confirms the offsets of base, statusCond, level, unk8D, nowHP, maxHP, atk, def, spe, spa, spd
// and mail, and its size in Black
struct PartyPkm {
    BoxPkm base;
    u32 statusCond;
    u8 level;
    u8 unk8D;
    u16 nowHP;
    u16 maxHP;
    u16 atk;
    u16 def;
    u16 spe;
    u16 spa;
    u16 spd;
    MailData mail;
    u32 unkD4;
    u32 unkD8;
};

// Whether the gender ratio leaves no choice of sex
BOOL isGenderlessOrSetGender(u32 genderRatio);

// Black 2's; twinned code confirms the offsets of species and ribbons in Black
// The four blocks of a Pokémon's data, which PML_PkmGetParamBlockCore finds
typedef struct {
    u16 species;
    u16 item;
    u32 id;
    u32 exp;
    u8 friendship;
    u8 ability;
    u8 markings;
    u8 language;
    u8 hpEV;
    u8 atkEV;
    u8 defEV;
    u8 speEV;
    u8 spaEV;
    u8 spdEV;
    u8 contest[6];
    u32 ribbons;
} PkmBlockA;

// Black 2's; twinned code confirms the offsets of ribbons in Black
typedef struct {
    u16 moves[4];
    u8 pp[4];
    u8 ppUps[4];
    u32 hpIV : 5;
    u32 atkIV : 5;
    u32 defIV : 5;
    u32 speIV : 5;
    u32 spaIV : 5;
    u32 spdIV : 5;
    u32 isEgg : 1;
    u32 isNicknamed : 1;
    u32 ribbons;
    u8 fatefulEncounter : 1;
    u8 gender : 2;
    u8 form : 5;
    u8 nature;
    u16 hiddenAbility : 1;
    // Black 2's nPoke : 1 (one of N's Pokémon) is the low bit of this: Black's poke_tool.c has no field there
    u16 unk1A_1 : 15;
    u32 unk1C;
} PkmBlockB;

// Black 2's; twinned code confirms the offsets of nickname and ribbons in Black
typedef struct {
    u16 nickname[11];
    u8 unk16;
    u8 originGame;
    u64 ribbons;
} PkmBlockC;

// Black 2's, not checked against Black
typedef struct {
    u16 otName[8];
    u8 eggDate[3];
    u8 metDate[3];
    u16 eggLocation;
    u16 metLocation;
    u8 pokerus;
    u8 ball;
    u8 metLevel : 7;
    u8 otGender : 1;
    u8 unk1D;
    // Black's: a u16 (PML_PkmGetParamCore reads it with ldrh); Black 2 has u8 unk1E and u8 pokestarFame here
    u16 unk1E;
} PkmBlockD;

#define PKM_ID_NOT_SHINY 0xffffffff00000000ULL
#define PKM_PID_FROM_ID 0xffffffff00000001ULL
#define LEARN_MOVE_KNOWN 0xfffe

// Black 2's, not checked against Black
#define LEARN_MOVE_NO_SLOT 0x8000

void PokeParty_CreateTempPkm(PartyPkm *pkm, u16 species, u16 level, u64 id);
u32 makeSpecialPID(u32 id, u16 species, u16 form, u8 sex, u8 a4, BOOL a5);
u16 PML_CryptoGenKey(void *data, u32 size);
// Block 0 to 3 of a Pokémon's data, PkmBlockA to PkmBlockD, wherever its PID puts it
void *PML_PkmGetParamBlockCore(BoxPkm *pkm, u32 pid, u32 block);
// Whether the Pokémon's original trainer is someone other than the player
BOOL PML_UtilCheckForeignOT(BoxPkm *pkm, PlayerInfo *playerInfo);
u32 PML_PkmGetNature(BoxPkm *pkm);
u8 PML_PkmGetSex(BoxPkm *pkm);
void PML_PkmPPRecover(BoxPkm *pkm);
// The level a Pokémon of the species has with the experience
u32 CalcLevelByExp(u16 species, u16 form, u32 exp);
// Hatches an egg, recording where and by whom
void hatchEgg(PartyPkm *pkm, PlayerInfo *playerInfo, u16 placeName, HeapID heapId);
void PokeParty_SetNature(PartyPkm *pkm, u32 nature);
// The form, or 0 if the species has fewer forms. Species 650 counts as having two
u8 PML_PkmSanitizeForme(u16 species, u8 form);
PartyPkm *PokeParty_NewPkm(u16 species, u16 level, u64 trainerId, s32 ivs, u64 pid, HeapID heapId);
// pokebw2's PML_ItemGetGenesectDriveForme
u32 func_02019d00(u16 item);

// The party: its Pokémon, and a flag of each slot
#define PARTY_SIZE 6

// pokebw2's PokeIcon_GetFileOfPkm
// A Pokémon's icon in ARCID_POKEICON: its characters' file and its palette
u32 func_0201ba68(BoxPkm *pkm);
// pokebw2's PokeIcon_GetPaletteOfPkm
u32 func_0201bbe8(BoxPkm *pkm);
// pokebw2's getOBJTileMapping_MainEng
u32 func_0201bc40(void);
// pokebw2's getOBJTileMapping_SubEng
u32 func_0201bc78(void);

// pokebw2's getOBJTileMapping_MainEng
// pokebw2's getOBJTileMapping_SubEng

struct PokeParty {
    s32 capacity;
    s32 count;
    PartyPkm pkms[PARTY_SIZE];
    u8 slotExists;
};
// Gives a Pokémon of the party Pokérus at random
void pokerusHandler(PokeParty *party);
// Counts down the Pokérus of the party's Pokémon by days
void pokerusDecay(PokeParty *party, s32 days);
// Spreads Pokérus from the party's Pokémon to their neighbours at random
void pokerusSpread(PokeParty *party);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void PokeParty_SetStatusCond(PartyPkm *pkm, u32 status);
u8 PokeParty_GetSex(PartyPkm *pkm);
BOOL PokeParty_CheckAnyRibbon(PartyPkm *pkm);
BOOL PokeParty_IsRare(PartyPkm *pkm);
// Decrypt a Pokémon for a series of reads and writes, and return whether it was encrypted, which is what the
// encryption afterwards takes
BOOL PokeParty_DecryptPkm(PartyPkm *pkm);
BOOL PokeParty_EncryptPkm(PartyPkm *pkm, BOOL wasEncrypted);
u32 PML_PkmGetParam(BoxPkm *pkm, u32 param, void *buffer);
BOOL PML_PkmDecrypt(BoxPkm *pkm);
u32 PML_PkmGetLevel(BoxPkm *pkm);
BOOL PML_PkmChangeForme(BoxPkm *pkm, u32 forme);
BOOL PML_PkmReEncrypt(BoxPkm *pkm, BOOL wasEncrypted);
BOOL PML_PkmIsRare(BoxPkm *pkm);
// Whether the Pokémon has Pokérus that hasn't run its course
BOOL doesPokerusHaveDuration(BoxPkm *pkm);
// Whether the Pokémon has had Pokérus
BOOL doesPokeHavePokerus(BoxPkm *pkm);
// The same two for a Pokémon of the party
BOOL pokerusDuration(PartyPkm *pkm);
BOOL pokeHasPkrs(PartyPkm *pkm);
// Puts a move in a slot with its full PP and no PP Ups
void PML_PkmSetMove(BoxPkm *pkm, u16 move, u8 slot);
// How a nature changes a stat, attack to special defense from 1: 1 raised, -1 lowered, 0 neither
s8 statAffectedByNature(u8 nature, u32 stat);
void copyPartyPkm(const PartyPkm *src, PartyPkm *dest);
u8 PML_UtilDerivePkmSex(u16 species, u16 form, u32 pid);
void copyPkmIntoPartyBlk(PokeParty *party, u32 index, const PartyPkm *pkm);
// Changes a Pokémon into another species, as evolution does
// Black's: u16 (ov010 narrows the species before the call)
void setChangedPkmSpecies(PartyPkm *pkm, u16 species);
void PokeParty_Init(PokeParty *party);
void PokeParty_Copy(const PokeParty *src, PokeParty *dest);
void PokeParty_ClearPkm(PartyPkm *pkm);
// The experience the Pokémon's level starts at
u32 getExpForPkm_Wrapper(PartyPkm *pkm);
void setLevel(PartyPkm *pkm, u32 level);
u32 getOBJTileMapping_MainEng(void);
// A Pokémon icon's character file in its archive, and its palette
u32 PokeParty_GetIconIndex(u32 species, u32 form, u32 sex, BOOL egg);
// A flag of each slot, kept in the party
BOOL PokeParty_GetSlotExists(PokeParty *party, u32 slot);
void PokeParty_SetSlotExists(PokeParty *party, u32 slot, BOOL exists);
// A new party Pokémon made from a box Pokémon, with its stats calculated
PartyPkm *boxPkmRegenToPartyPkm(BoxPkm *pkm, HeapID heapId);
u32 PokeParty_GetFirstBattleReady(PokeParty *party);
u32 isEggInParty(PokeParty *party);
int howManyPokesAreAbleToFight(PokeParty *party);
int countAllEggsInParty(PokeParty *party);
int countSanityEggsInParty(PokeParty *party);
BOOL PokeParty_SwapPkms(PokeParty *party, u32 indexA, u32 indexB, HeapID heapId);
BOOL PokeParty_RemovePkm(PokeParty *party, u32 index);
void PokeParty_RecoverAll(PokeParty *party);
u32 getOBJTileMapping_SubEng(void);
// The form of Arceus for a plate, and of Genesect for a drive
u16 _getTypeForPlate(u16 item);
// Black's order and return: the assembly indexes the form moves with r1 and passes r2 to PokeParty_SetMove as the slot
BOOL PML_PkmChangeRotomForme(PartyPkm *pkm, u32 forme, u32 moveSlot);
void PokeParty_SetMove(PartyPkm *pkm, u16 move, u8 slot);
u16 *PokeParty_GetRememberableMoves(PartyPkm *pkm, HeapID heapId);
// The number of Pokémon in the party that can battle: not fainted and not eggs
int countActivePkms(PokeParty *party);
// Whether a Pokémon's nature raises (1) or lowers (-1) a stat
s8 doesNatureAffectStat(PartyPkm *pkm, u32 stat);
u8 getHiddenPowerType(PartyPkm *pkm);
u32 getHiddenPowerBasePwr(PartyPkm *pkm);
// pokebw2's PokeIcon_GetPaletteFile
// A species with its form and sex in one u16
// The icons' palette and cell files, for the OBJ mapping in use
u32 func_0201bc3c(void);
// pokebw2's PokeIcon_GetCellFile_Main
// pokebw2's PokeIcon_GetPalette
u32 func_0201bb5c(u32 species, u32 form, u32 sex, BOOL egg);
u32 PokeParty_GetNature(PartyPkm *pkm);
// Opens the personal archives and empties the cache of the species data read last
void PML_PersonalInit(HeapID heapId);
// Restore the PP of the Pokémon's moves
void PokeParty_PPRecover(PartyPkm *pkm);
// Whether the Pokémon can learn the move at moveIndex of the list of the tutors that teach for free
BOOL PokeParty_CheckMoveTutorNormal(PartyPkm *pkm, u32 moveIndex);
// Resets the nickname to the species' name
void setNicknameToNick(PartyPkm *pkm);
u32 PokeParty_GetLevel(PartyPkm *pkm);
// Whether a Pokémon can learn the TM or HM of the number PML_ItemGetTMBitMask gives
BOOL canPkmLearnTM_Wrapper(PartyPkm *pkm, u8 tm);
BOOL IsTrainerOT(PartyPkm *pkm, PlayerInfo *player);
// pokebw2's PokeParty_LearnMoveTutor
// Learn a move, and set a move in a slot, as the move tutors teach them
u16 func_02018238(PartyPkm *pkm, u16 move);
// pokebw2's PokeParty_SetMoveKeepPP
void func_020182a0(PartyPkm *pkm, u16 move, u8 slot);
// pokebw2's PokeParty_LearnNextLevelMove
// The next move that a Pokémon learns at its level, going on from *index: 0 once there are none left, 0xfffe for one
// it already knows, and the move with 0x8000 set when it has no free slot for it
u16 func_02018328(PartyPkm *pkm, u32 *index, HeapID heapId);
BoxPkm *func_0201d624(PartyPkm *pkm);
// Whether the species is a legendary Pokémon of the national Pokédex
BOOL PML_PkmIsLegendNational(u16 species);
// pokebw2's PokeIcon_GetPalette
// Encrypts or decrypts size bytes of data with key: a Pokémon's blocks with their checksum, its party fields with its
// PID
void PML_CryptoRun(void *data, u32 size, u32 key);

#endif // BWPORT_PML_POKE_PARTY_H
