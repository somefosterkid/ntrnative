// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/bsubway_scr.h, the lines in use so
// far.
#ifndef BWPORT_FIELD_BSUBWAY_SCR_H
#define BWPORT_FIELD_BSUBWAY_SCR_H

#include "types.h"
#include "struct_decls.h"
#include "system/pms_data.h"
#include "app/ircbattlematch.h"
#include "app/ov306.h"
#include "save/player_info.h"

// Black 2's; twinned code confirms the offsets of species, form, item, moves, id, personality, ivs, evs, ppUps, region,
// ability, happiness and nickname in Black
struct BSubwayPokemon {
    u16 species : 11;
    u16 form : 5;
    u16 item;
    u16 moves[4];
    u32 id;
    u32 personality;
    union {
        u32 all;
        struct {
            u32 hp : 5;
            u32 attack : 5;
            u32 defense : 5;
            u32 speed : 5;
            u32 spAttack : 5;
            u32 spDefense : 5;
            u32 unk30 : 2;
        } stat;
    } ivs;
    u8 evs[6];
    // Two bits per move
    u8 ppUps;
    u8 region;
    u8 ability;
    u8 happiness;
    u16 nickname[11];
    u8 nature;
    u8 unk39[3];
};

struct BSubwayTrainer {
    // The number of the trainer's file plus 1
    u32 unk00;
    u16 trainerId;
    u8 unk06[2];
    u16 name[8];
    // What the Trainer says before the battle, or a sentence type of 0xffff and a message of file 0xb8
    PMSData message;
    // What the Trainer says on winning and on losing
    u16 winWords[4];
    u16 loseWords[4];
    BSubwayPokemon pokemon[4];
};

// Black 2's, not checked against Black
// The first two Pokémon picked for a trainer, to make them again: the ID they were made with, their files of the
// Pokémon arc, personalities and natures
struct BSubwayTeamConfig {
    u32 id;
    u16 files[2];
    u32 pids[2];
    u8 natures[2];
};

// The commands the multi-battle partners send each other, from GFL net command 0x2c00
enum {
    BSUBWAY_COMM_MEMBERS = 0x2c00,
    BSUBWAY_COMM_TRAINERS,
    BSUBWAY_COMM_RETIRE,
    BSUBWAY_COMM_PLAYER_INFO,
    BSUBWAY_COMM_PLAY_MODE,
    BSUBWAY_COMM_5,
    BSUBWAY_COMM_6,
    BSUBWAY_COMM_7,
    BSUBWAY_COMM_MAX,
};

// Black 2's, not checked against Black
// The size of a command's data, in u16s
#define BSUBWAY_COMM_BUF_LEN 35

// The beacon of a Battle Subway multi battle
struct BSubwayBeacon {
    PlayerInfo playerInfo;
    u8 mac[6];
    // 0x3a0b
    u16 gameId;
};

// Black 2's; twinned code confirms the offsets of heapId, memberCount, playMode, partnerGender, unkC_0, unkC_1, unkC_3,
// unkC_5, unkC_9, unkE, sendCommand, unk18, partnerSpecies, memberSlots, memberSpecies, memberItems, unk32, gameData,
// unk70, unk74, unk78, memberChoices, unk82, unk84, trainers, unk2C8, teamConfigs, unk664, sendBuf, beacon, recvCount,
// recvMode, unk727, recvResult, unk72A and partner in Black
struct BSubwayScrWork {
    // 0x12345678
    u32 magic;
    u32 heapId;
    u8 memberCount;
    u8 playMode;
    u8 gender;
    u8 partnerGender;
    u16 unkC_0 : 1;
    u16 unkC_1 : 2;
    // Whether the partner retired
    u16 unkC_3 : 1;
    u16 unkC_4 : 1;
    u16 unkC_5 : 3;
    u16 unkC_8 : 1;
    u16 unkC_9 : 1;
    u16 unkC_10 : 2;
    u16 unkC_12 : 1;
    u16 unkC_13 : 3;
    u16 unkE;
    u16 unk10;
    // The BSUBWAY_COMM_* command to send
    u16 sendCommand;
    u8 unk14[4];
    u16 unk18;
    u16 partnerSpecies[2];
    // The party slots and species and items of the members entered
    u8 memberSlots[4];
    u16 memberSpecies[4];
    u16 memberItems[4];
    u16 unk32[0x1d];
    GameData *gameData;
    BSubwayPlayData *unk70;
    BSubwayScoreData *unk74;
    void *unk78;
    // Each member's chosen party slot plus 1
    u8 memberChoices[6];
    u16 unk82;
    u16 unk84;
    u8 unk86[2];
    BSubwayTrainer trainers[2];
    BSubwayTrainer unk2C8[3];
    BSubwayTeamConfig teamConfigs[3];
    u8 unk664[4];
    // The data of the command to send, and of the one received. Some commands write a PlayerInfo or a u32 over it,
    // which the array's 2-byte alignment and odd length rule out as a union member
    u16 sendBuf[BSUBWAY_COMM_BUF_LEN];
    u16 recvBuf[BSUBWAY_COMM_BUF_LEN];
    BSubwayBeacon beacon;
    void *unk71C;
    u16 *resultVar;
    // How many of the commands sent have arrived, counting this machine's own
    u8 recvCount;
    // 1 when only this machine's command is awaited
    u8 recvMode;
    u8 unk726;
    u8 unk727;
    // What the partner's command answered
    u16 recvResult;
    u16 unk72A;
    PlayerInfo partner;
    u8 unk74C[0x58];
    IrcBattleMatchParam ircMatchParam;
    void *allocatedBuffer;
    BtlSetup *btlSetup;
    Ov306Param ov306Param;
    u16 unk7EC;
    u16 unk7EE;
};

u16 func_ov021_021ca2cc(s32 value);
void func_ov010_02167ab0(BSubwayScrWork *bsw);
void func_ov010_02167b00(BSubwayScrWork *bsw);
void func_ov010_02167b18(BSubwayScrWork *bsw);
// Fills the send buffer for a command, mode being the command's number
void func_ov010_02167bfc(BSubwayScrWork *bsw, u16 mode, u16 value);
BOOL func_ov010_02167cb4(BSubwayScrWork *bsw);
void func_ov010_02167cf4(BSubwayScrWork *bsw, u8 mode);
BOOL func_ov010_02167d00(BSubwayScrWork *bsw, u16 *var);

BtlSetup *func_ov021_021cbd08(BSubwayScrWork *bsw, GameSystem *gsys);
GameEvent *func_ov010_0216c860(GameSystem *gsys);

// Black 2's, not checked against Black
// The range of trainer IDs a train's trainers come from, by level
struct BSubwayTrainerRange {
    u16 min;
    u16 max;
};

void func_ov021_021c9924(GameSystem *gsys);
void func_ov021_021c9b78(BSubwayScrWork *bsw);
void func_ov021_021c9bcc(BSubwayScrWork *bsw);
void func_ov021_021c9c54(BSubwayScrWork *bsw, GameSystem *gsys);
void func_ov021_021c9cac(BSubwayScrWork *bsw);
void func_ov021_021c9d70(GameSystem *gsys, BSubwayScrWork *bsw);
void func_ov021_021c9e8c(BSubwayScrWork *bsw);
u16 func_ov021_021c9f44(BSubwayScrWork *bsw, GameSystem *gsys);
BOOL func_ov021_021c9fd0(BSubwayScrWork *bsw);
void func_ov021_021c9ffc(BSubwayScrWork *bsw, GameSystem *gsys);
void func_ov021_021ca048(BSubwayScrWork *bsw, GameSystem *gsys);
void func_ov021_021ca05c(BSubwayScrWork *bsw);
u32 func_ov021_021ca150(BSubwayScrWork *bsw, u16 a1);
u16 func_ov021_021ca164(BSubwayScoreData *score, GameSystem *gsys, u32 a2);
void func_ov021_021ca23c(BSubwayScrWork *bsw);
void func_ov021_021ca250(BSubwayScrWork *bsw);
void func_ov021_021ca258(BSubwayScrWork *bsw, u32 count, u32 extra);
u16 func_ov021_021ca270(u16 mode);
BOOL func_ov021_021ca2a4(const u16 *list, u16 value, u16 count);
void func_ov021_021ca2dc(BSubwayScrWork *bsw, SaveControl *save, u32 a2, u32 a3);
void func_ov021_021ca338(BSubwayScrWork *bsw, u8 a1);
void func_ov021_021ca3b4(BSubwayPokemon *dest, PartyPkm *pkm);
BOOL func_ov021_021cbed8(BSubwayScrWork *bsw, BSubwayTrainer *trainer, u16 trainerId, u32 count, const u16 *species,
                         const u16 *items, BSubwayTeamConfig *config, HeapID heapId);
u8 func_ov021_021cbefc(u32 value);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void *func_ov010_021688bc(BSubwayTrainer *trainer, u16 trainerId, HeapID heapId);
// Makes a Pokémon from the file of the Battle Subway's Pokémon arc, with the personality, or one made from id when it
// is 0, the IVs and, when rentalItem is set, the rental item of the index. Returns the personality
u32 func_ov010_021684e8(BSubwayPokemon *pkm, u16 file, u32 id, u32 pid, u8 iv, u8 index, BOOL rentalItem,
                        HeapID heapId);
PokeParty *func_ov021_021ca210(BSubwayScrWork *bsw);
u16 func_ov021_021ca234(BSubwayScrWork *bsw);
void func_ov010_02167b24(u8 timing);
BOOL func_ov010_02167b34(u8 timing);
GameEvent *func_ov010_0216c538(BSubwayScrWork *bsw, GameSystem *gsys, u8 rental);
GameEvent *func_ov010_0216c640(BSubwayScrWork *bsw, GameSystem *gsys, Field *field);
// The message of a trainer of the train in a balloon over the actor
GameEvent *func_ov010_0216c6e8(BSubwayScrWork *bsw, GameSystem *gsys, u32 index, u16 actorId, u8 winPos);
// The message of a saved leader in a balloon over the actor
GameEvent *func_ov010_0216cb48(GameSystem *gsys, u16 index, u16 actorId);
extern const u8 data_ov021_021dacd0[];
BSubwayScrWork *func_ov021_021c9934(GameSystem *gsys, u16 a1, u16 a2);
void func_ov021_021c9b24(GameSystem *gsys, BSubwayScrWork *bsw);
u16 func_ov021_021c9d30(GameSystem *gsys);
u16 func_ov021_021c9db0(BSubwayScrWork *bsw);
void func_ov021_021ca1e4(BSubwayScrWork *bsw);
void func_ov021_021ca238(BSubwayScrWork *bsw, u32 score);

#endif // BWPORT_FIELD_BSUBWAY_SCR_H
