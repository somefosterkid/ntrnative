#ifndef POKEHEARTGOLD_COMM_SERVER_CLIENT_H
#define POKEHEARTGOLD_COMM_SERVER_CLIENT_H

#include "global.h"

#include "player_data.h"
#include "pokemon_types_def.h"
#include "save_link_ruleset.h"

typedef struct CommGameInfo {
    u32 unk_00;
    u8 unk_04;
    u8 unk_05;
    u8 unk_06;
    u8 unk_07;
    MailMessage unk_08;
    PlayerProfile unk_10;
    u8 unk_30[0x20];
    u8 unk_50[4];
    u8 unk_54;
    u8 unk_55[7];
} CommGameInfo;

// The wireless parent/child work (0xD98 bytes on the DS), allocated by sub_02034B0C and held by _021D4134.
typedef struct CommServerClient {
    u8 unk_00[0x54];
    WMBssDesc unk_54;
    WMBssDesc unk_114[16];
    u8 unk_D14[8][6];
    u16 unk_D44[16];
    void *unk_D64;
    MailMessage unk_D68;
    u32 unk_D70;
    u8 unk_D74;
    u8 unk_D75[3];
    PlayerProfile *unk_D78;
    LinkBattleRuleset *unk_D7C;
    u32 unk_D80;
    void *unk_D84;
    CommGameInfo *unk_D88;
    u16 unk_D8C;
    u16 unk_D8E;
    u8 unk_D90;
    u8 unk_D91;
    u8 unk_D92;
    u8 unk_D93;
    u8 unk_D94;
    u8 unk_D95_0 : 1;
    u8 unk_D95_1 : 1;
    u8 unk_D95_2 : 1;
    u8 unk_D95_3 : 1;
    u8 unk_D95_4 : 1;
    u8 unk_D95_5 : 1;
    u8 unk_D95_6 : 1;
    u8 unk_D95_7 : 1;
    u8 unk_D96[2];
} CommServerClient;

#ifndef SDK_PORT
// DS layout check: the pointer fields are 4 bytes there (the PC build's pointers are wider)
typedef char CommServerClient_size_check[(sizeof(CommServerClient) == 0xD98) ? 1 : -1];
#endif

typedef struct CommServerClientStatics {
    u16 tgid;
    u16 unk_02;
    u32 status;
    CommServerClient *sCommServerClient;
} CommServerClientStatics;

extern CommServerClientStatics _021D4134;

#endif // POKEHEARTGOLD_COMM_SERVER_CLIENT_H
