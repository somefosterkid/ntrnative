// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/worldtrade.h. The proc table's name,
// WORLDTRADE_PROC_FUNCTIONS, is pokebw2's guess.
// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/worldtrade.h, the lines in use so far.
#ifndef BWPORT_APP_WORLDTRADE_H
#define BWPORT_APP_WORLDTRADE_H

#include "types.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "struct_decls.h"

// The Global Trade Station
#define OVERLAY_WORLDTRADE OVERLAY_ID(135)
#include "struct_decls.h"

// The Global Trade Station, overlay 135

// worldtrade_data, systemdata, myparty, mybox, wifilist, wifihistory, mystatus, config, record and savedata keep the
// names that worldtrade.c's asserts print. config is the options save block, which TrainerDataSave names here
typedef struct {
    WorldTradeData *worldtrade_data;
    AdventureSave *systemdata;
    PokeParty *myparty;
    BoxSaveAccessor *mybox;
    PokeDexSave *pokedex;
    WifiList *wifilist;
    UnityTowerSurveySave *wifihistory;
    PlayerInfo *mystatus;
    TrainerDataSave *config;
    GameRecords *record;
    BagSave *bag;
    BOOL isNationalDex;
    s32 profileId;
    u32 unk34;
    u32 unk38;
    SaveControl *savedata;
    GameSystem *gsys;
} WorldTradeParam;

extern const GameProcFunctions data_ov135_02202bac;

#endif // BWPORT_APP_WORLDTRADE_H
