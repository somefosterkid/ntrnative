// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/battle_rec_ov.h: the lines in use so
// far.
#ifndef BWPORT_APP_BATTLE_REC_OV_H
#define BWPORT_APP_BATTLE_REC_OV_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"
#include "save/battle_rec.h"

// pokebw2's BattleRecOv_SaveVideo
// Saves the loaded video to a slot, a step each call: 0 while it runs, then the save's result (2 or 3). The first
// step encrypts the body again, after marking the video as one that may be uploaded if secure is set
u32 func_ov125_021d7540(GameData *gameData, u32 slot, BOOL secure, u16 *seq, u16 *saveSeq, HeapID heapId);

// pokebw2's BattleRecOv_EncryptBody
// Encrypts the loaded video's body
void func_ov125_021d75c0(SaveControl *save);
// pokebw2's BattleRecOv_SaveDataNumber
// Gives the loaded video the number that the server gave it, and saves it to the player's own slot, a step each call
u32 func_ov125_021d75e8(GameData *gameData, u64 dataNumber, u16 *seq, u16 *saveSeq, HeapID heapId);
// pokebw2's BattleRecOv_SaveSecureVideo
// Saves the loaded video to a slot as one that may be uploaded, a step each call
u32 func_ov125_021d7630(GameData *gameData, u32 slot, u16 *seq, u16 *saveSeq, HeapID heapId);
// pokebw2's BattleRecOv_StartDeleteVideo
// Starts clearing the video of an extra save block, and runs the save, a step each call: the save's result
void func_ov125_021d76a8(GameData *gameData, HeapID heapId, u32 block);
// pokebw2's BattleRecOv_DeleteVideoMain
u32 func_ov125_021d76d8(GameData *gameData, u32 block);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's BattleRecOv_GetVideo
// The loaded video
BattleRecVideo *func_ov125_021d7700(void);

#endif // BWPORT_APP_BATTLE_REC_OV_H
