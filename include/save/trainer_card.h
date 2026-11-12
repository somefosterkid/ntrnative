// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/save/trainer_card.h, the lines in use so far.
#ifndef BWPORT_SAVE_TRAINER_CARD_H
#define BWPORT_SAVE_TRAINER_CARD_H

#include "types.h"
#include "struct_decls.h"

// Overlay 10's oneshot_dr.c: the gifts given once, kept in the trainer card's table keyed to the player's ID
void setOneShotDRObtained(TrainerCardSave *trainerCard, int flag, PlayerInfo *playerInfo);
BOOL isOneShotDRObtained(TrainerCardSave *trainerCard, int flag, PlayerInfo *playerInfo);
void setToTrainerCardDRTable(TrainerCardSave *trainerCard, int flag, u32 key);
u32 getOneShotDRKeyFromSave(TrainerCardSave *trainerCard, int flag);
// pokebw2's func_0200c924 and func_0200c90c
u32 func_0200c1dc(TrainerCardSave *trainerCard);
// Counts one more thanks, up to 99999, and returns the count
u32 func_0200c1c8(TrainerCardSave *trainerCard);
u32 func_0200c1c4(TrainerCardSave *trainerCard);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void setSecondsCurrentTimeInTrainerCard(TrainerCardSave *trainerCard, s64 seconds);
TrainerCardSave *getTrainerCardData_wrapper(SaveControl *save);
// When the survey started, in seconds since 2000
s64 getSecondsFromTrainerCardData(TrainerGameInfoSave *info);
BOOL isBadgeObtained(TrainerCardSave *trainerCard, u32 badgeId);
void addBadge(TrainerCardSave *trainerCard, u32 badgeId);
// Takes minutes from the counter at 0xe0, down to 0
void func_0200c3c0(TrainerGameInfoSave *info, u16 minutes);
u16 func_0200c3b0(TrainerGameInfoSave *info);
void func_0200c3b8(TrainerGameInfoSave *info, u16 value);
// The color of the Battle Recorder
u8 func_0200c3e0(TrainerCardSave *trainerCard);
void func_0200c3ec(TrainerCardSave *trainerCard, u8 color);
u32 func_0200c3f4(void);
void func_0200c3f8(void *data);

#endif // BWPORT_SAVE_TRAINER_CARD_H
