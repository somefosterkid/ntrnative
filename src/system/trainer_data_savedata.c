// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/trainer_data_savedata.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "save/trainer_data_savedata.h"
#include "field/player_state.h"
#include "save/save_control.h"
#include "nitro/mi.h"

// The save block with the options, the player's info and the play time. The ROM has no string for the file, so the
// name is a guess. Block 0x1b.

#define SAVE_BLOCK_TRAINER_DATA 0x1b

u32 getSizeofTrainerData(void) {
    return sizeof(TrainerDataSave);
}

void func_02008704(TrainerDataSave *data) {
    MIi_CpuClear32(0, data, sizeof(TrainerDataSave));
    func_02008320(&data->config);
    func_020084a0(&data->playerInfo);
    func_020085a0(&data->playTime);
    data->unk2C = 0;
}

// pokebw2's SaveControl_GetPlayerInfo
PlayerInfo *func_02008730(SaveControl *save) {
    TrainerDataSave *data = SaveControl_GetBlockPtr(save, SAVE_BLOCK_TRAINER_DATA);
    return &data->playerInfo;
}

TrainerDataSave *getTrainerDataBlkAddress(SaveControl *save) {
    return SaveControl_GetBlockPtr(save, SAVE_BLOCK_TRAINER_DATA);
}

PlayTime *func_02008748(SaveControl *save) {
    TrainerDataSave *data = SaveControl_GetBlockPtr(save, SAVE_BLOCK_TRAINER_DATA);
    return &data->playTime;
}

u8 func_02008754(SaveControl *save) {
    TrainerDataSave *data = SaveControl_GetBlockPtr(save, SAVE_BLOCK_TRAINER_DATA);
    return data->unk2C;
}

void func_02008764(SaveControl *save) {
    TrainerDataSave *data = SaveControl_GetBlockPtr(save, SAVE_BLOCK_TRAINER_DATA);
    data->unk2C = 1;
}

void func_02008774(SaveControl *save) {
    TrainerDataSave *data = SaveControl_GetBlockPtr(save, SAVE_BLOCK_TRAINER_DATA);
    data->unk2C = 0;
}

BOOL func_02008784(SaveControl *save, u32 index) {
    TrainerDataSave *data = SaveControl_GetBlockPtr(save, SAVE_BLOCK_TRAINER_DATA);
    return data->markers[index] == TRAINER_DATA_MARKER_SET;
}

void func_020087a8(SaveControl *save, u32 index) {
    TrainerDataSave *data = SaveControl_GetBlockPtr(save, SAVE_BLOCK_TRAINER_DATA);
    data->markers[index] = TRAINER_DATA_MARKER_SET;
}

void func_020087c0(SaveControl *save, u32 index) {
    TrainerDataSave *data = SaveControl_GetBlockPtr(save, SAVE_BLOCK_TRAINER_DATA);
    data->markers[index] = 0;
}

u32 *func_020087d4(SaveControl *save, u32 index) {
    TrainerDataSave *data = SaveControl_GetBlockPtr(save, SAVE_BLOCK_TRAINER_DATA);
    return &data->unk44[index];
}

void func_020087e8(SaveControl *save, PlayerState *state) {
    TrainerDataSave *data = SaveControl_GetBlockPtr(save, SAVE_BLOCK_TRAINER_DATA);
    data->playerInfo = state->playerInfo;
}

void func_02008808(SaveControl *save, PlayerState *state) {
    TrainerDataSave *data = SaveControl_GetBlockPtr(save, SAVE_BLOCK_TRAINER_DATA);
    state->playerInfo = data->playerInfo;
}
