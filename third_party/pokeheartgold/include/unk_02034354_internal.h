#ifndef POKEHEARTGOLD_UNK_02034354_INTERNAL_H
#define POKEHEARTGOLD_UNK_02034354_INTERNAL_H

#include <dwc.h>

#include "global.h"

#include "player_data.h"
#include "save_link_ruleset.h"

enum InfoState {
    INFO_STATE_EMPTY = 0,
    INFO_STATE_BEGIN_RECIEVE,
    INFO_STATE_RECIEVE,
    INFO_STATE_END_RECIEVE,
    INFO_STATE_MAX
};

typedef union CommRegulationBuffer {
    LinkBattleRuleset ruleset;
    u8 bytes[32];
} CommRegulationBuffer;

typedef struct CommPlayerInfo {
    CommRegulationBuffer regulationBuffer; // 0x0
    PlayerProfile trainerInfoBuffer; // 0x20
    DWCFriendData friendData; // 0x40
    u16 unk_4C[8]; // 0x4C
    u8 macAddress[6]; // 0x5C
    u8 netId; // 0x62
    u8 country; // 0x63
    u8 region; // 0x64
    u8 hasGiftPenalty; // 0x65
} CommPlayerInfo; // 0x68

typedef struct CommPlayerRecord {
    u16 win;
    u16 lose;
    u16 trades;
} CommPlayerRecord;

typedef struct CommunicationInformation {
    PlayerProfile *personalTrainerInfo; // 0x0
    const LinkBattleRuleset *regulation; // 0x4
    SaveData *saveData; // 0x8
    CommPlayerInfo playerInfo[8]; // 0xC
    PlayerProfile *trainerInfo[8]; // 0x34C
    CommPlayerRecord playerRecord[8]; // 0x36C
    u8 infoState[8]; // 0x39C
    u8 dataFinishedReading; // 0x3A4
    u8 dataRecvFlag; // 0x3A5
    u8 curNetId; // 0x3A6
} CommunicationInformation; // 0x3A8

extern CommunicationInformation *_021D4130;

#endif // POKEHEARTGOLD_UNK_02034354_INTERNAL_H
