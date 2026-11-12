#ifndef BWPORT_SYSTEM_TRAINER_GAME_INFO_SAVEDATA_H
#define BWPORT_SYSTEM_TRAINER_GAME_INFO_SAVEDATA_H

#include "types.h"
#include "struct_decls.h"

// The survey rank of the trainer card, from src/system/trainer_game_info_savedata.c
void func_0200c22c(TrainerCardSave *trainerCard, int rank);

void func_0200c1f8(TrainerGameInfoSave *info, StrBuf *strBuf);
void func_0200c210(TrainerGameInfoSave *info, StrBuf *strBuf);

#endif
