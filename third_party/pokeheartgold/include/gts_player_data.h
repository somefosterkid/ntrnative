#ifndef POKEHEARTGOLD_GTS_PLAYER_DATA_H
#define POKEHEARTGOLD_GTS_PLAYER_DATA_H

#include "global.h"

struct JournalEntry;
struct WiFiHistory;
struct GlobalTrade;
struct PCBoxes;
struct SysInfo;
struct Party;
struct Pokedex;
struct WiFiList;
struct PlayerProfile;
struct SaveData;
struct Options;
struct GameStats;
struct Bag;

typedef struct GTSPlayerData {
    struct GlobalTrade *globalTrade;
    struct SysInfo *systemData;
    struct Party *party;
    struct PCBoxes *pcBoxes;
    struct Pokedex *pokedex;
    struct WiFiList *wiFiList;
    struct WiFiHistory *wiFiHistory;
    struct PlayerProfile *trainerInfo;
    struct SaveData *saveData;
    struct Options *options;
    struct GameStats *records;
    struct JournalEntry *journalEntry;
    struct Bag *bag;
    int dexMode;
    int dwcProfileId;
    int showContestData;
    BOOL connectToWiFi;
} GTSPlayerData;

#endif // POKEHEARTGOLD_GTS_PLAYER_DATA_H
