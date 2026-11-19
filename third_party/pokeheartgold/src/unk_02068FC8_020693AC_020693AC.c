#include "global.h"

#include "trainer_case.h"

void sub_020693AC(u32 timesLinked, u32 linkBattleWins, u32 linkBattleLosses, u32 linkTrades, u8 signatureExists, const u8 *signature, TrainerCase *trainerCase);

void sub_020693AC(u32 timesLinked, u32 linkBattleWins, u32 linkBattleLosses, u32 linkTrades, u8 signatureExists, const u8 *signature, TrainerCase *trainerCase) {
    trainerCase->timesLinked = timesLinked;
    if (timesLinked > 999999) {
        trainerCase->timesLinked = 999999;
    }
    trainerCase->linkBattleWins = linkBattleWins;
    trainerCase->linkBattleLosses = linkBattleLosses;
    if (trainerCase->linkBattleWins > 9999) {
        trainerCase->linkBattleWins = 9999;
    }
    if (trainerCase->linkBattleLosses > 9999) {
        trainerCase->linkBattleLosses = 9999;
    }
    trainerCase->linkTrades = linkTrades;
    if (linkTrades > 99999) {
        trainerCase->linkTrades = 99999;
    }
    trainerCase->signatureExists = signatureExists;
    MI_CpuCopy8(signature, trainerCase->signature, sizeof(trainerCase->signature));
}
