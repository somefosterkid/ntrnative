// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/zukan_info.h: the lines in use so far.
#ifndef BWPORT_APP_ZUKAN_INFO_H
#define BWPORT_APP_ZUKAN_INFO_H

#include "types.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "system/printsys.h"
#include "struct_decls.h"
#include "gfl/overlay.h"

typedef struct ZukanInfo ZukanInfo;

// The same for a Pokémon of the party. national is whether the national Pokédex numbers it, caught whether its entry
// is shown or only its sprite, mode 0, 1 or 2 how it is shown (2 by the detail screen), engine 0 for the main screen
// and 1 for the sub screen
ZukanInfo *func_ov206_021b9de0(HeapID heapId, PartyPkm *pkm, BOOL national, BOOL caught, u32 mode, u32 engine,
                               u8 bgPriority, ClActUnit *unit, Font *font, PrintQueue *printQueue, MsgData *msgData0,
                               MsgData *msgData1, MsgData *msgData2);
void func_ov206_021ba2cc(ZukanInfo *info);
void func_ov206_021ba374(ZukanInfo *info);
// Whether the sprite is sliding in, and whether the entry has finished its animation
BOOL func_ov206_021ba524(ZukanInfo *info);
void func_ov206_021ba4cc(ZukanInfo *info);

BOOL func_ov206_021ba534(ZukanInfo *info);
// Starts mode 0's animation
void func_ov206_021ba544(ZukanInfo *info);
// Shows another Pokémon
void func_ov206_021ba550(ZukanInfo *info, u16 species, u16 form, u16 sex, u16 rare, u32 personality, BOOL caught);
// Shows the entry in another language
void func_ov206_021ba64c(ZukanInfo *info, u32 language);
void func_ov206_021ba6b0(ZukanInfo *info, u16 species, u16 form, u16 sex, u16 rare, u32 personality, BOOL caught,
                         u32 language);
void func_ov206_021ba7c8(ZukanInfo *info);
void func_ov206_021ba814(ZukanInfo *info);

// Black 2's, not checked against Black
#define OVERLAY_ZUKAN_INFO OVERLAY_ID(296)

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void func_ov206_021ba51c(ZukanInfo *info);
ZukanInfo *func_ov206_021b9e68(HeapID heapId, u16 species, u16 form, u16 sex, u16 rare, u32 personality, BOOL national,
                               BOOL caught, u32 mode, u32 engine, u8 bgPriority, ClActUnit *unit, Font *font,
                               PrintQueue *printQueue, MsgData *msgData0, MsgData *msgData1, MsgData *msgData2);
// Mode 0 waits for the caller to say that it is ready before the sprite slides in, and mode 1 for it to ask for the
// Pokémon's cry
void func_ov206_021ba514(ZukanInfo *info);

#endif // BWPORT_APP_ZUKAN_INFO_H
