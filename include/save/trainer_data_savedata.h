// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/save/trainer_data_savedata.h, the lines in
// use so far.
#ifndef BWPORT_SAVE_TRAINER_DATA_SAVEDATA_H
#define BWPORT_SAVE_TRAINER_DATA_SAVEDATA_H

#include "types.h"
#include "struct_decls.h"
#include "save/config.h"
#include "save/player_info.h"
#include "save/playtime.h"

// pokebw2's SaveControl_GetPlayerInfo
PlayerInfo *func_02008730(SaveControl *save);
TrainerDataSave *getTrainerDataBlkAddress(SaveControl *save);

#define TRAINER_DATA_MARKER_COUNT 10

#define TRAINER_DATA_MARKER_SET 0xc21e

// Black 2's; matching code confirms the offsets of config, playerInfo, playTime, unk2C, markers and unk44, and its size
// in Black
// The save block of the options, the player's info and the play time (trainer_data_savedata.c, a guessed name)
struct TrainerDataSave {
    Config config;
    PlayerInfo playerInfo;
    PlayTime playTime;
    u8 unk2C;
    u8 unk2D[3];
    // Each is TRAINER_DATA_MARKER_SET once something has happened
    u16 markers[TRAINER_DATA_MARKER_COUNT];
    u32 unk44[9];
};

// The play time: hours and minutes
PlayTime *func_02008748(SaveControl *save);
u8 func_02008754(SaveControl *save);
void func_02008764(SaveControl *save);
void func_02008774(SaveControl *save);
BOOL func_02008784(SaveControl *save, u32 index);
void func_020087a8(SaveControl *save, u32 index);
void func_020087c0(SaveControl *save, u32 index);
// Copy the player's info from the player state, and back
void func_020087e8(SaveControl *save, PlayerState *state);
void func_02008808(SaveControl *save, PlayerState *state);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
u32 getSizeofTrainerData(void);
void func_02008704(TrainerDataSave *data);
u32 *func_020087d4(SaveControl *save, u32 index);

#endif // BWPORT_SAVE_TRAINER_DATA_SAVEDATA_H
