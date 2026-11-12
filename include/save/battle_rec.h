// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/save/battle_rec.h: the prototypes of Black's
// named functions and the types they need.
#ifndef BWPORT_SAVE_BATTLE_REC_H
#define BWPORT_SAVE_BATTLE_REC_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/heap.h"
#include "battle/btl_setup.h"
#include "nitro/math.h"
#include "save/player_info.h"
#include "system/pms_data.h"
#include "stddef.h"

// Loads the saved video of a slot (0 is the player's own, 1 to 3 the downloaded ones). result is 1 if it loaded,
// 0 if there is none, 2 if its CRCs are wrong and 3 if the load failed
void func_0200b570(SaveControl *save, HeapID heapId, u32 *result, u32 slot);

// Black 2's; twinned code confirms the offsets of badEgg, species, isEgg, gender, form and nickname in Black
// A Pokémon as a recording keeps it: the fields of its blocks that the battle uses, unencrypted
typedef struct {
    u32 pid;
    u16 partyDecrypted : 1;
    u16 boxDecrypted : 1;
    u16 badEgg : 1;
    u16 nature : 8;
    u16 nPoke : 1;
    u16 species;
    u16 item;
    u32 id;
    u32 exp;
    u8 friendship;
    u8 ability;
    u8 hpEV;
    u8 atkEV;
    u8 defEV;
    u8 speEV;
    u8 spaEV;
    u8 spdEV;
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
    u8 fatefulEncounter : 1;
    u8 gender : 2;
    u8 form : 5;
    u16 nickname[11];
    u16 otName[8];
    u8 ball;
    u8 language;
    u32 statusCond;
    u8 level;
    u8 pokestarFame;
    u16 nowHP;
    u16 maxHP;
    u16 atk;
    u16 def;
    u16 spe;
    u16 spa;
    u16 spd;
} BattleRecPkm;

// A party as a recording keeps it
typedef struct {
    u16 capacity;
    u16 count;
    BattleRecPkm pkm[6];
} BattleRecParty;

// A BtlSetupTrainer as a recording keeps it, with the name as characters
typedef struct {
    u32 aiFlags;
    u16 trainerId;
    u16 trainerClass;
    u16 items[4];
    u16 name[16];
    PMSData unk30;
    PMSData unk38;
} BattleRecTrainer;

// Black 2's, not checked against Black
// A client of the battle: a player, whose info the setup has, or a trainer, BATTLE_REC_CLIENT_*
typedef struct {
    u16 type;
    u16 unk2;
    union {
        PlayerInfo player;
        BattleRecTrainer trainer;
    } info;
} BattleRecClient;

// Black 2's, not checked against Black
// The setup's parameters, which both forms keep at their start
typedef struct {
    MATHRandContext32 rand;
    BtlFieldEnv env;
    u8 config[4];
    u16 bgm;
    u16 unk2E;
    u16 unk30;
    u8 battleType : 5;
    u8 unk32_5 : 3;
    u8 battleStyle : 4;
    u8 unk33_4 : 3;
    u8 unk33_7 : 1;
} BattleRecSetup;

// The Pokémon that a header lists
#define BATTLE_REC_HEADER_PKM_MAX 12

// Black 2's; twinned code confirms the offsets of species, formGender, counter, mode, magic, unk2A, version, dataNumber
// and crc, and its size in Black
// The header of a battle video, which is what a list of videos shows
struct BattleRecHeader {
    u16 species[BATTLE_REC_HEADER_PKM_MAX];
    // The form in the low 7 bits and the gender in the top one
    u8 formGender[BATTLE_REC_HEADER_PKM_MAX];
    u16 counter;
    u16 mode;
    u16 magic;
    u8 unk2A;
    u8 version;
    u8 unk2C[0xc];
    // The video's number, which the server gives it and the CRC leaves out
    u64 dataNumber;
    // getCRC16 of everything before dataNumber
    u16 crc;
    u16 padding;
};

// Black 2's; twinned code confirms the offsets of parties, magic and crc in Black
// The body of a battle video, after its profile and header
typedef struct {
    BattleRecSetup setup;
    u32 dataSize;
    u8 data[0xc00];
    BattleRecParty parties[4];
    BattleRecClient clients[4];
    u16 magic;
    u16 padding;
    // getCRC16 of everything before it, and the key that the body is encrypted with
    u16 crc;
    u16 padding2;
} BattleRecBody;

typedef struct {
    u8 profile[0x80];
    BattleRecHeader header;
    BattleRecBody body;
} BattleRecVideo;

enum {
    BATTLE_REC_HEADER_SPECIES,
    BATTLE_REC_HEADER_FORM,
    BATTLE_REC_HEADER_COUNTER,
    BATTLE_REC_HEADER_MODE,
    BATTLE_REC_HEADER_DATA_NUMBER,
    BATTLE_REC_HEADER_UNK2A,
    BATTLE_REC_HEADER_GENDER,
    BATTLE_REC_HEADER_VERSION,
};

// Makes the loaded video of a received profile, header and body, and decrypts its body
void func_0200b938(const GdsProfile *profile, const BattleRecHeader *header, const BattleRecBody *body, SaveControl *save);
// A value from the header, BATTLE_REC_HEADER_*, such as the video's number
u64 func_0200b98c(BattleRecHeader *header, u32 id, u32 index);
BattleRecHeader *func_0200b928(void);

// The magic number of a video's header and body
#define BATTLE_REC_MAGIC 0xe281

// The size that the header's CRC covers
#define BATTLE_REC_HEADER_CRC_SIZE offsetof(BattleRecHeader, dataNumber)

// The size that the body's CRC covers, and that is encrypted
#define BATTLE_REC_BODY_CRC_SIZE offsetof(BattleRecBody, crc)

void clearVSPlayerBlk(BattleRecVideo *video);

BattleRecVideo *func_0200b5a4(SaveControl *save, HeapID heapId, u32 *result, u32 slot);
// The header of a video
BattleRecHeader *func_0200b934(BattleRecVideo *video);

// The version that a video's header carries
#define BATTLE_REC_VERSION 100

// The clients of a battle mode and the Pokémon of each
void func_0200b784(u16 mode, s32 *clientCount, s32 *pkmCount);
// Sets the CRC of a body
void func_0200ba44(BattleRecBody *body);
// The body and the header of the loaded video
BattleRecBody *func_0200b91c(void);

// The size of the extra save block that holds a video, which is larger than the video (Black's; Black 2's is 0x1a00)
#define BATTLE_REC_BLOCK_SIZE 0x2000

#define BATTLE_REC_MODE_MAX 41

u32 getSizeOfVSPlayer(void);

// Black 2's, not checked against Black
enum {
    BATTLE_REC_CLIENT_NONE,
    BATTLE_REC_CLIENT_PLAYER,
    BATTLE_REC_CLIENT_TRAINER,
};

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// The profile of the player who recorded the loaded video. The name is swan's
GdsProfile *getVSPlayerAllocation(void);
// decryptData, which encrypts as well
void decryptVSPlayer(void *data, u32 size, u32 seed);
// Saves the loaded video to a slot as it is, a step each call: 0 while it runs, then the save's result (2 or 3)
u32 func_0200b67c(GameData *gameData, BattleRecVideo *video, u32 block, u16 *seq, HeapID heapId);
void func_0200b438(BattleRecVideo *video);
// Decrypt and encrypt the loaded video's body
void func_0200b490(void);
void func_0200b4b8(void);

#endif // BWPORT_SAVE_BATTLE_REC_H
