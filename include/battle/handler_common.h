// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/handler_common.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_BATTLE_HANDLER_COMMON_H
#define BWPORT_BATTLE_HANDLER_COMMON_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"

BOOL GiratinaArceusGenesectItemCheck(u16 species, u16 item);
void CommonMagicCoatCheckMoveEffect(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
void MultiplyBasePower(u32 factor);
BOOL CommonCheckRunMessage(BattleEventItem *item, BtlServerFlow *flow, u8 monId);

void CommonMagicCoatWait(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
void CommonRunCalcSkip(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
BOOL func_ov093_021e85f8(u32 monId);
BOOL func_ov093_021e869c(BtlServerFlow *flow, u8 monId);
BOOL func_ov093_021e86e8(BtlServerFlow *flow, u8 monId, u8 otherId);
void func_ov093_021e8804(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
BOOL IsMonLastInTurnOrder(BtlServerFlow *flow, u8 monId);
BOOL func_ov093_021e86c8(BtlServerFlow *flow, u8 monId);

#endif // BWPORT_BATTLE_HANDLER_COMMON_H
