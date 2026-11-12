// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov201/battle_rec_ov.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// What the Global Data Server client and the Battle Recorder do with the loaded battle video on its way to the server
// and into the save. The ROM does not name the file: the name is a guess, after the Generation 4 games' battle_rec_ov.c,
// which holds the same functions after the GDS client's files.

#include "types.h"
#include "app/battle_rec_ov.h"
#include "gfl/std.h"
#include "save/battle_rec.h"
#include "save/save_control.h"
#include "system/game_data.h"

// The key that a body is encrypted with, of its CRC
#define BATTLE_REC_KEY(body) ((body)->crc + (((body)->crc ^ 0xffff) << 16))

// The loaded battle video, in battle_rec.c
extern BattleRecVideo *data_0214616c;

// pokebw2's BattleRecOv_SaveVideo
u32 func_ov125_021d7540(GameData *gameData, u32 slot, BOOL secure, u16 *seq, u16 *saveSeq, HeapID heapId) {
    switch (*seq) {
    case 0:
        if (secure == TRUE) {
            data_0214616c->header.unk2A = secure;
            data_0214616c->header.magic = BATTLE_REC_MAGIC;
            data_0214616c->header.crc = getCRC16(&data_0214616c->header, BATTLE_REC_HEADER_CRC_SIZE);
        }
        decryptVSPlayer(&data_0214616c->body, BATTLE_REC_BODY_CRC_SIZE, BATTLE_REC_KEY(&data_0214616c->body));
        (*seq)++;
        break;
    case 1:
        return func_0200b67c(gameData, data_0214616c, slot, saveSeq, heapId);
    }
    return 0;
}

// pokebw2's BattleRecOv_EncryptBody
// Encrypts the loaded video's body
void func_ov125_021d75c0(SaveControl *save) {
    decryptVSPlayer(&data_0214616c->body, BATTLE_REC_BODY_CRC_SIZE, BATTLE_REC_KEY(&data_0214616c->body));
}

// pokebw2's BattleRecOv_SaveDataNumber
u32 func_ov125_021d75e8(GameData *gameData, u64 dataNumber, u16 *seq, u16 *saveSeq, HeapID heapId) {
    switch (*seq) {
    case 0:
        data_0214616c->header.dataNumber = dataNumber;
        (*seq)++;
        break;
    case 1:
        return func_0200b67c(gameData, data_0214616c, 0, saveSeq, heapId);
    }
    return 0;
}

// pokebw2's BattleRecOv_SaveSecureVideo
u32 func_ov125_021d7630(GameData *gameData, u32 slot, u16 *seq, u16 *saveSeq, HeapID heapId) {
    switch (*seq) {
    case 0:
        data_0214616c->header.unk2A = TRUE;
        data_0214616c->header.magic = BATTLE_REC_MAGIC;
        data_0214616c->header.crc = getCRC16(&data_0214616c->header, BATTLE_REC_HEADER_CRC_SIZE);
        decryptVSPlayer(&data_0214616c->body, BATTLE_REC_BODY_CRC_SIZE, BATTLE_REC_KEY(&data_0214616c->body));
        (*seq)++;
        break;
    case 1:
        return func_0200b67c(gameData, data_0214616c, slot, saveSeq, heapId);
    }
    return 0;
}

// pokebw2's BattleRecOv_StartDeleteVideo
void func_ov125_021d76a8(GameData *gameData, HeapID heapId, u32 block) {
    SaveControl *save = GameData_GetSaveControl(gameData);

    func_0200725c(save, block, heapId);
    clearVSPlayerBlk(getAddressOfExtraSaveBlk(save, block, 0));
    func_02012e4c(gameData, block);
}

// pokebw2's BattleRecOv_DeleteVideoMain
u32 func_ov125_021d76d8(GameData *gameData, u32 block) {
    SaveControl *save = GameData_GetSaveControl(gameData);
    u32 result = func_02012e7c(gameData, block);

    if (result == 2 || result == 3) {
        freeIntermediateSaveExtraBlksAfterLoad(save, block);
    }
    return result;
}

// pokebw2's BattleRecOv_GetVideo
// The loaded video
BattleRecVideo *func_ov125_021d7700(void) {
    return data_0214616c;
}

