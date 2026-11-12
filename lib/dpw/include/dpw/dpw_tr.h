// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dpw/include/dpw/dpw_tr.h: its types and
// constants, and the prototypes of Black's named functions.
#ifndef BWPORT_DPW_DPW_TR_H
#define BWPORT_DPW_DPW_TR_H

#include "types.h"

// What a player looks for, or wants for the Pokémon they deposit
typedef struct {
    s16 characterNo;
    s8 gender;
    s8 level_min;
    s8 level_max;
    s8 unused;
} Dpw_Tr_PokemonSearchData;

// A search with a country, and how many results to return
typedef struct {
    s16 characterNo;
    s8 gender;
    s8 level_min;
    s8 level_max;
    s8 unused;
    s8 maxNum;
    u8 countryCode;
} Dpw_Tr_PokemonSearchDataEx;

// What a deposited Pokémon is, for searches
typedef struct {
    s16 characterNo;
    s8 gender;
    s8 level;
} Dpw_Tr_PokemonDataSimple;

// A deposited Pokémon on the server
typedef struct {
    // The Pokémon, a party Pokémon
    u8 postData[0xec];
    Dpw_Tr_PokemonDataSimple postSimple;
    Dpw_Tr_PokemonSearchData wantSimple;
    // The trainer's
    u8 gender;
    u8 unkF7;
    u8 postDate[8];
    u8 tradeDate[8];
    s32 id;
    u32 trainerID;
    u16 name[8];
    u8 countryCode;
    u8 localCode;
    u8 trainerType;
    // Whether the Pokémon was traded
    s8 isTrade;
    u8 versionCode;
    u8 langCode;
    u8 unk126;
    u8 unk127;
} Dpw_Tr_Data;

// The player's profile, which the server keeps with their trades
typedef struct {
    u8 version;
    u8 language;
    u8 country;
    u8 region;
    u32 playerId;
    u16 playerName[8];
    u32 unk18;
    u8 unk1C[8];
    // An e-mail address, which the server confirms (see Dpw_Common_ProfileResult)
    char mailAddr[56];
    u32 unk5C;
    u8 unk60[4];
} Dpw_Common_Profile;

typedef struct {
    int code;
    int mailAddrAuthResult;
} Dpw_Common_ProfileResult;

// pokebw2's DpwTr_Init
// Starts and ends the library, for the player's ID and friend key
void func_ov114_021c7ba0(s32 pid, u64 friendKey, int a2);
// pokebw2's DpwTr_Process
// Runs the library's requests, every frame
void func_ov114_021c7c1c(void);
// pokebw2's DpwTr_End
void func_ov114_021c8658(void);
// pokebw2's DpwTr_IsFinished
// Whether the last request has ended, and its result
BOOL func_ov114_021c866c(void);
// pokebw2's DpwTr_GetResult
s32 func_ov114_021c86a8(void);
// pokebw2's DpwTr_Upload
// Deposits a Pokémon with its signature, and ends the deposit
void func_ov114_021c86b8(Dpw_Tr_Data *data, const void *sign, u32 size);
// pokebw2's DpwTr_UploadFinish
void func_ov114_021c8770(void);
// pokebw2's DpwTr_Download
// Asks for the player's deposited Pokémon, and whether it was traded
void func_ov114_021c87fc(Dpw_Tr_Data *result);
// pokebw2's DpwTr_GetTradeResult
void func_ov114_021c887c(Dpw_Tr_Data *result);
// pokebw2's DpwTr_DeleteFinish
// End the taking back of a traded Pokémon, and of a deposited one
void func_ov114_021c88fc(void);
// pokebw2's DpwTr_ReturnFinish
void func_ov114_021c8978(void);
// pokebw2's DpwTr_Cancel
// Cancels the request
void func_ov114_021c89f0(void);
// pokebw2's DpwTr_Search
// Searches the server for up to maxNum Pokémon, without and with a country
void func_ov114_021c8b18(const Dpw_Tr_PokemonSearchData *search, s32 maxNum, Dpw_Tr_Data *result);
// pokebw2's DpwTr_SearchEx
void func_ov114_021c8bc4(const Dpw_Tr_PokemonSearchDataEx *search, Dpw_Tr_Data *result);
// pokebw2's DpwTr_Exchange
// Trades for the Pokémon of the given ID, and ends the trade
void func_ov114_021c8c68(s32 id, Dpw_Tr_Data *upload, Dpw_Tr_Data *result, const void *sign, u32 size);
// pokebw2's DpwTr_ExchangeFinish
void func_ov114_021c8d14(void);
// pokebw2's DpwTr_GetServerState
// Requests the server's state, and sends the player's profile
void func_ov114_021c8da0(void);
// pokebw2's DpwTr_SetProfile
void func_ov114_021c8e18(Dpw_Common_Profile *profile, Dpw_Common_ProfileResult *result);

#endif // BWPORT_DPW_DPW_TR_H
