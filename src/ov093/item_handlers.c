// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov167/item_handlers.c, ported by tools/port_file.py.
#include "types.h"
#include "battle/btl_ability.h"
#include "battle/btl_action.h"
#include "battle/btl_calc.h"
#include "battle/btl_event.h"
#include "battle/btl_field.h"
#include "battle/btl_handler.h"
#include "battle/btl_handler_work.h"
#include "battle/btl_item.h"
#include "battle/btl_main.h"
#include "battle/btl_pokeparam.h"
#include "battle/btl_server_flow.h"
#include "battle/btl_tables.h"
#include "battle/handler_common.h"
#include "constants/battle.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/species.h"
#include "constants/types.h"
#include "pml/item.h"
#include "pml/poke_party.h"
#include "pml/waza.h"
#include "nitro/fx/fx_const.h"

// One entry of the item lookup table at the end of the file: an item and the function that returns its handlers
// Black 2's, not checked against Black
typedef struct ItemEventAddEntry {
    u32 item;
    const BattleEventHandlerEntry *(*eventAdd)(u32 *numHandlers);
} ItemEventAddEntry;

static BattleEventItem *ItemEvent_AddItemCore(BattleMon *mon, u16 itemId);
static BOOL ItemEvent_RollEffectChance(BtlServerFlow *flow, u8 chance);
static u32 CommonGetItemParam(BattleEventItem *item, u8 param);
static fx32 ItemAttackValueToRatio(BattleEventItem *item);
static void ItemEvent_PushRun(BattleEventItem *item, BtlServerFlow *flow, u8 monId);
static void HandlerCheriBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerCherryBerryUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddCheriBerry(u32 *numHandlers);
static void HandlerChestoBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerChestoBerryUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddChestoBerry(u32 *numHandlers);
static void HandlerRawstBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerRawstBerryUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddRawstBerry(u32 *numHandlers);
static void HandlerAspearBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerAspearBerryUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddAspearBerry(u32 *numHandlers);
static void HandlerPersimBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerPersimBerryUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddPersimBerry(u32 *numHandlers);
static void HandlerPechaBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerPechaBerryUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddPechaBerry(u32 *numHandlers);
static void HandlerLumBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerLumBerryUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddLumBerry(u32 *numHandlers);
static void CommonStatusReaction(BattleEventItem *item, BtlServerFlow *flow, u8 monId, u32 condition);
static void CommonUseForStatus(BattleEventItem *item, BtlServerFlow *flow, u8 monId, u32 condition);
static BOOL CommonConditionCodeMatch(BtlServerFlow *flow, u8 monId, u32 condition);
static const BattleEventHandlerEntry *EventAddLeppaBerry(u32 *numHandlers);
static void HandlerLeppaBerryMoveEnd(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerLeppaBerryReaction(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerLeppaBerryGet(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerLeppaBerryPPUsed(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerLeppaBerryUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerLeppaBerryUseTemp(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static u8 CommonLeppaBerryLastMoveID(BtlServerFlow *flow, u8 monId);
static u8 CommonLeppaBerryEnableMoveID(BtlServerFlow *flow, u8 monId);
static BOOL HandlerLeppaBerryCommon(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work, BOOL anyUsed);
static const BattleEventHandlerEntry *EventAddOranBerry(u32 *numHandlers);
static void HandlerOranBerryReaction(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerOranBerryMemberIn(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerOranBerryCheckActivation(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerOranBerryUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddBerryJuice(u32 *numHandlers);
static const BattleEventHandlerEntry *EventAddSitrusBerry(u32 *numHandlers);
static void HandlerSitrusBerryUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddFigyBerry(u32 *numHandlers);
static void HandlerFigyBerryUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerFigyBerryUseTemp(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddWikiBerry(u32 *numHandlers);
static void HandlerWikiBerryUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerWikiBerryUseTemp(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddMagoBerry(u32 *numHandlers);
static void HandlerMagoBerryUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerMagoBerryUseTemp(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddAguavBerry(u32 *numHandlers);
static void HandlerAguavBerryUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerAguavBerryUseTemp(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddIapapaBerry(u32 *numHandlers);
static void HandlerIapapaBerryUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerIapapaBerryUseTemp(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerCommonPinchBerryReaction(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerCommonPinchBerryMemberIn(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerCommonPinchBerryCheckActivation(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void CommonPinchBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work, u32 flavor);
static const BattleEventHandlerEntry *EventAddLiechiBerry(u32 *numHandlers);
static void HandlerLiechiBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddGanlonBerry(u32 *numHandlers);
static void HandlerGanlonBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddSalacBerry(u32 *numHandlers);
static void HandlerSalacBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddPetayaBerry(u32 *numHandlers);
static void HandlerPetayaBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddApicotBerry(u32 *numHandlers);
static void HandlerApicotBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddLansatBerry(u32 *numHandlers);
static void HandlerLansatBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddStarfBerry(u32 *numHandlers);
static void HandlerStarfBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void CommonStatBoostBerry(BtlServerFlow *flow, u8 monId, u32 stat, s8 change);
static const BattleEventHandlerEntry *EventAddEnigmaBerry(u32 *numHandlers);
static void HandlerEnigmaBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerEnigmaBerryUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddOccaBerry(u32 *numHandlers);
static void HandlerOccaBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddPasshoBerry(u32 *numHandlers);
static void HandlerPasshoBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddWacanBerry(u32 *numHandlers);
static void HandlerWacanBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddRindoBerry(u32 *numHandlers);
static void HandlerRindoBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddYacheBerry(u32 *numHandlers);
static void HandlerYacheBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddChopleBerry(u32 *numHandlers);
static void HandlerChopleBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddKebiaBerry(u32 *numHandlers);
static void HandlerKebiaBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddShucaBerry(u32 *numHandlers);
static void HandlerShucaBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddCobaBerry(u32 *numHandlers);
static void HandlerCobaBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddPayapaBerry(u32 *numHandlers);
static void HandlerPayapaBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddTangaBerry(u32 *numHandlers);
static void HandlerTangaBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddChartiBerry(u32 *numHandlers);
static void HandlerChartiBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddKasibBerry(u32 *numHandlers);
static void HandlerKasibBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddHabanBerry(u32 *numHandlers);
static void HandlerHabanBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddColburBerry(u32 *numHandlers);
static void HandlerColburBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddBabiriBerry(u32 *numHandlers);
static void HandlerBabiriBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddChilanBerry(u32 *numHandlers);
static void HandlerChilanBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static BOOL CommonResistBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work, u8 type,
                              BOOL anyEffectiveness);
static void HandlerCommonResistBerryDamageAfter(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerPinchReactionCommon(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerPinchReactionMemberIn(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void CommonDamageReact(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void CommonDamageReactCheck(BattleEventItem *item, BtlServerFlow *flow, u8 monId, u32 ratio, BOOL checkContext);
static BOOL func_ov093_021dde74(BattleEventItem *item, BtlServerFlow *flow, u8 monId, u32 ratio);
static BOOL CommonDamageReactCheckCore(BtlServerFlow *flow, u8 monId, u32 ratio);
static const BattleEventHandlerEntry *EventAddCustapBerry(u32 *numHandlers);
static void HandlerCustapBerryPriorityCheck(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerCustapBerryUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddMicleBerry(u32 *numHandlers);
static void HandlerMicleBerryReaction(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerMicleBerryActProcEnd(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerMicleBerryActionEnd(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerMicleBerryMemberIn(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerMicleBerryUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerMicleBerryUseTemp(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddJabocaBerry(u32 *numHandlers);
static void HandlerJabocaBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddRowapBerry(u32 *numHandlers);
static void HandlerRowapBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void CommonJabocaRowapBerryReaction(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work,
                                           u32 category);
static void HandlerJabocaRowapBerryUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddWhiteHerb(u32 *numHandlers);
static void HandlerWhiteHerbReaction(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void func_ov093_021de24c(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerWhiteHerbTurnCheck(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerWhiteHerbUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerWhiteHerbUseTemp(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddMentalHerb(u32 *numHandlers);
static void HandlerMentalHerbReaction(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerMentalHerbUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerMentalHerbUseTemp(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddBrightPowder(u32 *numHandlers);
static void HandlerBrightPowder(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddMachoBrace(u32 *numHandlers);
static void HandlerMachoBrace(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddQuickClaw(u32 *numHandlers);
static void HandlerQuickClawPriorityCheck(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerQuickClawUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddLaggingTail(u32 *numHandlers);
static void HandlerLaggingTail(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddKingsRock(u32 *numHandlers);
static void HandlerKingsRock(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerKingsRockUseTemp(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddRazorFang(u32 *numHandlers);
static const BattleEventHandlerEntry *EventAddWideLens(u32 *numHandlers);
static void HandlerWideLens(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddScopeLens(u32 *numHandlers);
static void HandlerScopeLens(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddLuckyPunch(u32 *numHandlers);
static void HandlerLuckyPunch(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddStick(u32 *numHandlers);
static void HandlerStick(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddZoomLens(u32 *numHandlers);
static void HandlerZoomLens(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddLaxIncense(u32 *numHandlers);
static void HandlerLaxIncense(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddMuscleBand(u32 *numHandlers);
static void HandlerMuscleBand(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddWiseGlasses(u32 *numHandlers);
static void HandlerWiseGlasses(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddDeepSeaTooth(u32 *numHandlers);
static void HandlerDeepSeaTooth(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddDeepSeaScale(u32 *numHandlers);
static void HandlerDeepSeaScale(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddMetalPowder(u32 *numHandlers);
static void HandlerMetalPowder(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddQuickPowder(u32 *numHandlers);
static void HandlerQuickPowder(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddSoulDew(u32 *numHandlers);
static void HandlerSoulDewAttacker(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerSoulDewDefender(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddThickClub(u32 *priority);
static void HandlerThickClub(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddChoiceBand(u32 *priority);
static void HandlerChoiceBandPower(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddChoiceSpecs(u32 *priority);
static void HandlerChoiceSpecsPower(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddChoiceScarf(u32 *priority);
static void HandlerChoiceScarf(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerChoiceItemCommonMoveLock(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerChoiceItemCommonItemChange(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddFocusSash(u32 *priority);
static void HandlerFocusSash(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerFocusSashUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddFocusBand(u32 *priority);
static void HandlerFocusBandCheck(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerFocusBand(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerFocusBandUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddExpertBelt(u32 *priority);
static void HandlerExpertBelt(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddLifeOrb(u32 *priority);
static void HandlerLifeOrbReaction(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerLifeOrbPower(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddMetronomeItem(u32 *priority);
static void HandlerMetronomeItem(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddGripClaw(u32 *priority);
static void HandlerGripClaw(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddShellBell(u32 *priority);
static void HandlerShellBell(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddLightClay(u32 *priority);
static void HandlerLightClay(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddPowerHerb(u32 *priority);
static void HandlerPowerHerbCheckChargeSkip(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerPowerHerbFixChargeSkip(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerPowerHerbUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddLeftovers(u32 *priority);
static void HandlerLeftoversReaction(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerLeftoversUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddBlackSludge(u32 *priority);
static void HandlerBlackSludge(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddDestinyKnot(u32 *priority);
static void HandlerDestinyKnot(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddStickyBarb(u32 *priority);
static void HandlerStickyBarbDamageReaction(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerStickyBarbTurnCheck(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddPowerBracer(u32 *priority);
static const BattleEventHandlerEntry *EventAddPowerBelt(u32 *priority);
static const BattleEventHandlerEntry *EventAddPowerLens(u32 *priority);
static const BattleEventHandlerEntry *EventAddPowerBand(u32 *priority);
static const BattleEventHandlerEntry *EventAddPowerAnklet(u32 *priority);
static const BattleEventHandlerEntry *EventAddPowerWeight(u32 *priority);
static void HandlerPowerItemCalcSpeed(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddFlamePlate(u32 *priority);
static const BattleEventHandlerEntry *EventAddSplashPlate(u32 *priority);
static const BattleEventHandlerEntry *EventAddZapPlate(u32 *priority);
static const BattleEventHandlerEntry *EventAddMeadowPlate(u32 *priority);
static const BattleEventHandlerEntry *EventAddIciclePlate(u32 *priority);
static const BattleEventHandlerEntry *EventAddFistPlate(u32 *priority);
static const BattleEventHandlerEntry *EventAddToxicPlate(u32 *priority);
static const BattleEventHandlerEntry *EventAddEarthPlate(u32 *priority);
static const BattleEventHandlerEntry *EventAddSkyPlate(u32 *priority);
static const BattleEventHandlerEntry *EventAddMindPlate(u32 *priority);
static const BattleEventHandlerEntry *EventAddInsectPlate(u32 *priority);
static const BattleEventHandlerEntry *EventAddStonePlate(u32 *priority);
static const BattleEventHandlerEntry *EventAddSpookyPlate(u32 *priority);
static const BattleEventHandlerEntry *EventAddDracoPlate(u32 *priority);
static const BattleEventHandlerEntry *EventAddDreadPlate(u32 *priority);
static const BattleEventHandlerEntry *EventAddIronPlate(u32 *priority);
static const BattleEventHandlerEntry *func_ov093_021df364(u32 *priority);
static void HandlerPlate(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddSmokeBall(u32 *priority);
static void HandlerSmokeBall(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerSmokeBallMessage(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddAmuletCoin(u32 *priority);
static void HandlerAmuletCoin(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void func_ov093_021df420(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddGriseousOrb(u32 *priority);
static void HandlerGriseousOrb(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddIcyRock(u32 *priority);
static void HandlerIcyRock(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddSmoothRock(u32 *priority);
static void HandlerSmoothRock(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddHeatRock(u32 *priority);
static void HandlerHeatRock(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddDampRock(u32 *priority);
static void HandlerDampRock(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void CommonWeatherMoveIncreaseTurns(BattleEventItem *item, BtlServerFlow *flow, u8 monId, u8 weather);
static const BattleEventHandlerEntry *EventAddLightBall(u32 *priority);
static void HandlerLightBall(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerLightBallUseTemp(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddToxicOrb(u32 *priority);
static void HandlerToxicOrb(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerToxicOrbUseTemp(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddFlameOrb(u32 *priority);
static void HandlerFlameOrb(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerFlameOrbUseTemp(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddSilverPowder(u32 *priority);
static void HandlerSilverPowder(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddSoftSand(u32 *priority);
static void HandlerSoftSand(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddHardStone(u32 *priority);
static void HandlerHardStone(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddMiracleSeed(u32 *priority);
static void HandlerMiracleSeed(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddBlackGlasses(u32 *priority);
static void HandlerBlackGlasses(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddBlackBelt(u32 *priority);
static void HandlerBlackBelt(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddMagnet(u32 *priority);
static void HandlerMagnet(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddMetalCoat(u32 *priority);
static void HandlerMetalCoat(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddMysticWater(u32 *priority);
static void HandlerMysticWater(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddSharpBeak(u32 *priority);
static void HandlerSharpBeak(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddPoisonBarb(u32 *priority);
static void HandlerPoisonBarb(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerPoisonBarbUseTemp(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddNeverMeltIce(u32 *priority);
static void HandlerNeverMeltIce(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddSpellTag(u32 *priority);
static void HandlerSpellTag(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddTwistedSpoon(u32 *priority);
static void HandlerTwistedSpoon(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddCharcoal(u32 *priority);
static void HandlerCharcoal(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddDragonFang(u32 *priority);
static void HandlerDragonFang(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddSilkScarf(u32 *priority);
static void HandlerSilkScarf(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddOddIncense(u32 *priority);
static void HandlerOddIncense(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddRockIncense(u32 *priority);
static void HandlerRockIncense(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddWaveIncense(u32 *priority);
static void HandlerWaveIncense(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddSeaIncense(u32 *priority);
static void HandlerSeaIncense(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddRoseIncense(u32 *priority);
static void HandlerRoseIncense(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void CommonTypeBoostingItem(BattleEventItem *item, BtlServerFlow *flow, u8 monId, u8 type);
static const BattleEventHandlerEntry *EventAddLustrousOrb(u32 *priority);
static void HandlerLustrousOrb(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddAdamantOrb(u32 *priority);
static void HandlerAdamantOrb(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddIronBall(u32 *priority);
static void HandlerIronBallCalcSpeed(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerIronBallCheckFly(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddFloatStone(u32 *priority);
static void HandlerFloatStone(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
const BattleEventHandlerEntry *EventAddEviolite(u32 *priority);
static void HandlerEviolite(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddRockyHelmet(u32 *priority);
static void HandlerRockyHelmet(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddAirBalloon(u32 *priority);
static void HandlerAirBalloonMemberIn(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerAirBalloonCheckFlying(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerAirBalloonDamageReaction(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddRedCard(u32 *priority);
static void HandlerRedCard(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddRingTarget(u32 *priority);
static void HandlerRingTarget(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddBindingBand(u32 *priority);
static void HandlerBindingBand(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddAbsorbBulb(u32 *priority);
static void HandlerAbsorbBulbDamageReaction(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerAbsorbBulbUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddCellBattery(u32 *priority);
static void HandlerCellBatteryDamageReaction(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerCellBatteryUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddEjectButton(u32 *priority);
static void HandlerEjectButtonReaction(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerEjectButtonUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddFireGem(u32 *priority);
static void HandlerFireGemPower(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerFireGemDecide(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddWaterGem(u32 *priority);
static void HandlerWaterGemPower(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerWaterGemDecide(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddElectricGem(u32 *priority);
static void HandlerElectricGemPower(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerElectricGemDecide(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddGrassGem(u32 *priority);
static void HandlerGrassGemPower(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerGrassGemDecide(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddIceGem(u32 *priority);
static void HandlerIceGemPower(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerIceGemDecide(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddFightingGem(u32 *priority);
static void HandlerFightingGemPower(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerFightingGemDecide(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddPoisonGem(u32 *priority);
static void HandlerPoisonGemPower(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerPoisonGemDecide(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddGroundGem(u32 *priority);
static void HandlerGroundGemPower(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerGroundGemDecide(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddFlyingGem(u32 *priority);
static void HandlerFlyingGemPower(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerFlyingGemDecide(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddPsychicGem(u32 *priority);
static void HandlerPsychicGemPower(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerPsychicGemDecide(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddBugGem(u32 *priority);
static void HandlerBugGemPower(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerBugGemDecide(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddRockGem(u32 *priority);
static void HandlerRockGemPower(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerRockGemDecide(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddGhostGem(u32 *priority);
static void HandlerGhostGemPower(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerGhostGemDecide(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddDragonGem(u32 *priority);
static void HandlerDragonGemPower(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerDragonGemDecide(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddDarkGem(u32 *priority);
static void HandlerDarkGemPower(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerDarkGemDecide(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddSteelGem(u32 *priority);
static void HandlerSteelGemPower(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerSteelGemDecide(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddNormalGem(u32 *priority);
static void HandlerNormalGemPower(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerNormalGemDecide(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void CommonGemDecide(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work, u8 type);
static void CommonGemPower(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work, u8 type);
static void HandlerGemEnd(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);

// pokebw2's sItemEventAddTable
static const ItemEventAddEntry data_ov093_021f1ea4[] = {
    { ITEM_CHERI_BERRY, EventAddCheriBerry },
    { ITEM_CHESTO_BERRY, EventAddChestoBerry },
    { ITEM_RAWST_BERRY, EventAddRawstBerry },
    { ITEM_ASPEAR_BERRY, EventAddAspearBerry },
    { ITEM_PERSIM_BERRY, EventAddPersimBerry },
    { ITEM_PECHA_BERRY, EventAddPechaBerry },
    { ITEM_LUM_BERRY, EventAddLumBerry },
    { ITEM_LEPPA_BERRY, EventAddLeppaBerry },
    { ITEM_ORAN_BERRY, EventAddOranBerry },
    { ITEM_BERRY_JUICE, EventAddBerryJuice },
    { ITEM_SITRUS_BERRY, EventAddSitrusBerry },
    { ITEM_FIGY_BERRY, EventAddFigyBerry },
    { ITEM_WIKI_BERRY, EventAddWikiBerry },
    { ITEM_MAGO_BERRY, EventAddMagoBerry },
    { ITEM_AGUAV_BERRY, EventAddAguavBerry },
    { ITEM_IAPAPA_BERRY, EventAddIapapaBerry },
    { ITEM_LIECHI_BERRY, EventAddLiechiBerry },
    { ITEM_GANLON_BERRY, EventAddGanlonBerry },
    { ITEM_SALAC_BERRY, EventAddSalacBerry },
    { ITEM_PETAYA_BERRY, EventAddPetayaBerry },
    { ITEM_APICOT_BERRY, EventAddApicotBerry },
    { ITEM_LANSAT_BERRY, EventAddLansatBerry },
    { ITEM_STARF_BERRY, EventAddStarfBerry },
    { ITEM_ENIGMA_BERRY, EventAddEnigmaBerry },
    { ITEM_OCCA_BERRY, EventAddOccaBerry },
    { ITEM_PASSHO_BERRY, EventAddPasshoBerry },
    { ITEM_WACAN_BERRY, EventAddWacanBerry },
    { ITEM_RINDO_BERRY, EventAddRindoBerry },
    { ITEM_YACHE_BERRY, EventAddYacheBerry },
    { ITEM_CHOPLE_BERRY, EventAddChopleBerry },
    { ITEM_KEBIA_BERRY, EventAddKebiaBerry },
    { ITEM_SHUCA_BERRY, EventAddShucaBerry },
    { ITEM_COBA_BERRY, EventAddCobaBerry },
    { ITEM_PAYAPA_BERRY, EventAddPayapaBerry },
    { ITEM_TANGA_BERRY, EventAddTangaBerry },
    { ITEM_CHARTI_BERRY, EventAddChartiBerry },
    { ITEM_KASIB_BERRY, EventAddKasibBerry },
    { ITEM_HABAN_BERRY, EventAddHabanBerry },
    { ITEM_COLBUR_BERRY, EventAddColburBerry },
    { ITEM_BABIRI_BERRY, EventAddBabiriBerry },
    { ITEM_CHILAN_BERRY, EventAddChilanBerry },
    { ITEM_CUSTAP_BERRY, EventAddCustapBerry },
    { ITEM_MICLE_BERRY, EventAddMicleBerry },
    { ITEM_JABOCA_BERRY, EventAddJabocaBerry },
    { ITEM_ROWAP_BERRY, EventAddRowapBerry },
    { ITEM_WHITE_HERB, EventAddWhiteHerb },
    { ITEM_MENTAL_HERB, EventAddMentalHerb },
    { ITEM_BRIGHT_POWDER, EventAddBrightPowder },
    { ITEM_MACHO_BRACE, EventAddMachoBrace },
    { ITEM_QUICK_CLAW, EventAddQuickClaw },
    { ITEM_LAGGING_TAIL, EventAddLaggingTail },
    { ITEM_FULL_INCENSE, EventAddLaggingTail },
    { ITEM_KINGS_ROCK, EventAddKingsRock },
    { ITEM_RAZOR_CLAW, EventAddScopeLens },
    { ITEM_WIDE_LENS, EventAddWideLens },
    { ITEM_SCOPE_LENS, EventAddScopeLens },
    { ITEM_ZOOM_LENS, EventAddZoomLens },
    { ITEM_LAX_INCENSE, EventAddLaxIncense },
    { ITEM_MUSCLE_BAND, EventAddMuscleBand },
    { ITEM_WISE_GLASSES, EventAddWiseGlasses },
    { ITEM_DEEP_SEA_TOOTH, EventAddDeepSeaTooth },
    { ITEM_DEEP_SEA_SCALE, EventAddDeepSeaScale },
    { ITEM_METAL_POWDER, EventAddMetalPowder },
    { ITEM_QUICK_POWDER, EventAddQuickPowder },
    { ITEM_LIGHT_BALL, EventAddLightBall },
    { ITEM_LUCKY_PUNCH, EventAddLuckyPunch },
    { ITEM_STICK, EventAddStick },
    { ITEM_SOUL_DEW, EventAddSoulDew },
    { ITEM_THICK_CLUB, EventAddThickClub },
    { ITEM_CHOICE_BAND, EventAddChoiceBand },
    { ITEM_BLACK_SLUDGE, EventAddBlackSludge },
    { ITEM_CHOICE_SPECS, EventAddChoiceSpecs },
    { ITEM_CHOICE_SCARF, EventAddChoiceScarf },
    { ITEM_SILVER_POWDER, EventAddSilverPowder },
    { ITEM_SOFT_SAND, EventAddSoftSand },
    { ITEM_HARD_STONE, EventAddHardStone },
    { ITEM_MIRACLE_SEED, EventAddMiracleSeed },
    { ITEM_BLACK_GLASSES, EventAddBlackGlasses },
    { ITEM_BLACK_BELT, EventAddBlackBelt },
    { ITEM_MAGNET, EventAddMagnet },
    { ITEM_METAL_COAT, EventAddMetalCoat },
    { ITEM_MYSTIC_WATER, EventAddMysticWater },
    { ITEM_SHARP_BEAK, EventAddSharpBeak },
    { ITEM_RAZOR_FANG, EventAddRazorFang },
    { ITEM_POISON_BARB, EventAddPoisonBarb },
    { ITEM_NEVER_MELT_ICE, EventAddNeverMeltIce },
    { ITEM_SPELL_TAG, EventAddSpellTag },
    { ITEM_TWISTED_SPOON, EventAddTwistedSpoon },
    { ITEM_CHARCOAL, EventAddCharcoal },
    { ITEM_DRAGON_FANG, EventAddDragonFang },
    { ITEM_SILK_SCARF, EventAddSilkScarf },
    { ITEM_ODD_INCENSE, EventAddOddIncense },
    { ITEM_ROCK_INCENSE, EventAddRockIncense },
    { ITEM_WAVE_INCENSE, EventAddWaveIncense },
    { ITEM_SEA_INCENSE, EventAddSeaIncense },
    { ITEM_ROSE_INCENSE, EventAddRoseIncense },
    { ITEM_FOCUS_SASH, EventAddFocusSash },
    { ITEM_FOCUS_BAND, EventAddFocusBand },
    { ITEM_EXPERT_BELT, EventAddExpertBelt },
    { ITEM_LIFE_ORB, EventAddLifeOrb },
    { ITEM_METRONOME, EventAddMetronomeItem },
    { ITEM_GRIP_CLAW, EventAddGripClaw },
    { ITEM_SHELL_BELL, EventAddShellBell },
    { ITEM_LIGHT_CLAY, EventAddLightClay },
    { ITEM_POWER_HERB, EventAddPowerHerb },
    { ITEM_LEFTOVERS, EventAddLeftovers },
    { ITEM_TOXIC_ORB, EventAddToxicOrb },
    { ITEM_FLAME_ORB, EventAddFlameOrb },
    { ITEM_LUSTROUS_ORB, EventAddLustrousOrb },
    { ITEM_ADAMANT_ORB, EventAddAdamantOrb },
    { ITEM_IRON_BALL, EventAddIronBall },
    { ITEM_DESTINY_KNOT, EventAddDestinyKnot },
    { ITEM_ICY_ROCK, EventAddIcyRock },
    { ITEM_SMOOTH_ROCK, EventAddSmoothRock },
    { ITEM_HEAT_ROCK, EventAddHeatRock },
    { ITEM_DAMP_ROCK, EventAddDampRock },
    { ITEM_STICKY_BARB, EventAddStickyBarb },
    { ITEM_POWER_BRACER, EventAddPowerBracer },
    { ITEM_POWER_BELT, EventAddPowerBelt },
    { ITEM_POWER_LENS, EventAddPowerLens },
    { ITEM_POWER_BAND, EventAddPowerBand },
    { ITEM_POWER_ANKLET, EventAddPowerAnklet },
    { ITEM_POWER_WEIGHT, EventAddPowerWeight },
    { ITEM_FLAME_PLATE, EventAddFlamePlate },
    { ITEM_SPLASH_PLATE, EventAddSplashPlate },
    { ITEM_ZAP_PLATE, EventAddZapPlate },
    { ITEM_MEADOW_PLATE, EventAddMeadowPlate },
    { ITEM_ICICLE_PLATE, EventAddIciclePlate },
    { ITEM_FIST_PLATE, EventAddFistPlate },
    { ITEM_TOXIC_PLATE, EventAddToxicPlate },
    { ITEM_EARTH_PLATE, EventAddEarthPlate },
    { ITEM_SKY_PLATE, EventAddSkyPlate },
    { ITEM_MIND_PLATE, EventAddMindPlate },
    { ITEM_INSECT_PLATE, EventAddInsectPlate },
    { ITEM_STONE_PLATE, EventAddStonePlate },
    { ITEM_SPOOKY_PLATE, EventAddSpookyPlate },
    { ITEM_DRACO_PLATE, EventAddDracoPlate },
    { ITEM_DREAD_PLATE, EventAddDreadPlate },
    { ITEM_IRON_PLATE, EventAddIronPlate },
    { ITEM_BIG_ROOT, func_ov093_021df364 },
    { ITEM_SMOKE_BALL, EventAddSmokeBall },
    { ITEM_AMULET_COIN, EventAddAmuletCoin },
    { ITEM_LUCK_INCENSE, EventAddAmuletCoin },
    { ITEM_GRISEOUS_ORB, EventAddGriseousOrb },
    { ITEM_FLOAT_STONE, EventAddFloatStone },
    { ITEM_EVIOLITE, EventAddEviolite },
    { ITEM_ROCKY_HELMET, EventAddRockyHelmet },
    { ITEM_AIR_BALLOON, EventAddAirBalloon },
    { ITEM_RED_CARD, EventAddRedCard },
    { ITEM_RING_TARGET, EventAddRingTarget },
    { ITEM_BINDING_BAND, EventAddBindingBand },
    { ITEM_ABSORB_BULB, EventAddAbsorbBulb },
    { ITEM_CELL_BATTERY, EventAddCellBattery },
    { ITEM_EJECT_BUTTON, EventAddEjectButton },
    { ITEM_FIRE_GEM, EventAddFireGem },
    { ITEM_WATER_GEM, EventAddWaterGem },
    { ITEM_ELECTRIC_GEM, EventAddElectricGem },
    { ITEM_GRASS_GEM, EventAddGrassGem },
    { ITEM_ICE_GEM, EventAddIceGem },
    { ITEM_FIGHTING_GEM, EventAddFightingGem },
    { ITEM_POISON_GEM, EventAddPoisonGem },
    { ITEM_GROUND_GEM, EventAddGroundGem },
    { ITEM_FLYING_GEM, EventAddFlyingGem },
    { ITEM_PSYCHIC_GEM, EventAddPsychicGem },
    { ITEM_BUG_GEM, EventAddBugGem },
    { ITEM_ROCK_GEM, EventAddRockGem },
    { ITEM_GHOST_GEM, EventAddGhostGem },
    { ITEM_DRAGON_GEM, EventAddDragonGem },
    { ITEM_DARK_GEM, EventAddDarkGem },
    { ITEM_STEEL_GEM, EventAddSteelGem },
    { ITEM_NORMAL_GEM, EventAddNormalGem },
    { ITEM_NONE, NULL },
};

// Function names from swan.

static BattleEventItem *ItemEvent_AddItemCore(BattleMon *mon, u16 itemId) {
    u32 i;
    u16 subPriority;
    u8 monId;
    const BattleEventHandlerEntry *handlers;
    u32 numHandlers;

    for (i = 0; i < 0xac; i++) {
        if (itemId == data_ov093_021f1ea4[i].item) {
            subPriority = RawBattleMonStat(mon, 12);
            monId = GetMonID(mon);
            handlers = data_ov093_021f1ea4[i].eventAdd(&numHandlers);
            return BattleEvent_AddItem(5, itemId, 6, subPriority, monId, handlers, numHandlers);
        }
    }
    return NULL;
}

void ItemEvent_RemoveItem(BattleMon *mon) {
    BattleEventItem *item;
    BattleEventItem *next;

    item = BattleEvent_SeekItem(5, GetMonID(mon));
    while (item != NULL) {
        next = BattleEvent_GetNextItem(item);
        if (BattleEventItem_GetWorkValue(item, 6) == 0) {
            BattleEventItem_Remove(item);
        }
        item = next;
    }
}

BattleEventItem *ItemEvent_AddItem(BattleMon *mon) {
    u32 itemId;

    itemId = GetBattleMonHeldItem(mon);
    if (itemId != 0) {
        return ItemEvent_AddItemCore(mon, itemId);
    }
    return NULL;
}

BattleEventItem *ItemEvent_TempAdd(BattleMon *mon, u16 itemId) {
    BattleEventItem *item;

    if (itemId != 0) {
        item = ItemEvent_AddItemCore(mon, itemId);
        if (item != NULL) {
            BattleEventItem_SetTempItemFlag(item);
            BattleEventItem_SetWorkValue(item, 6, 1);
            return item;
        }
    }
    return NULL;
}

void func_ov093_021dcf9c(void *temp) {
    BattleEventItem_Remove(temp);
}

void ItemEvent_ItemRotationSleep(BattleMon *mon) {
    BattleEvent_ItemRotationSleep(GetMonID(mon), 5);
}

void ItemEvent_ItemRotationWake(BattleMon *mon) {
    if (!BattleEvent_ItemRotationWake(GetMonID(mon), 5)) {
        ItemEvent_AddItem(mon);
    }
}

static BOOL ItemEvent_RollEffectChance(BtlServerFlow *flow, u8 chance) {
    if (RollEffectChance(chance)) {
        return TRUE;
    }
    if (func_ov093_021c82b8(flow, 2)) {
        return TRUE;
    }
    return FALSE;
}

static u32 CommonGetItemParam(BattleEventItem *item, u8 param) {
    return ItemGetParam(BattleEventItem_GetSubID(item), param);
}

static fx32 ItemAttackValueToRatio(BattleEventItem *item) {
    s32 value;

    value = CommonGetItemParam(item, 2) + 100;
    return FX32_CONST(value) / 100;
}

static void ItemEvent_PushRun(BattleEventItem *item, BtlServerFlow *flow, u8 monId) {
    BattleEventItem_SetRecallEnable(item);
    BattleHandler_PushRun(flow, 0, monId);
}

static void HandlerCheriBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonStatusReaction(item, flow, monId, 1);
}

static void HandlerCherryBerryUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonUseForStatus(item, flow, monId, 1);
}

// pokebw2's sHandlersCheriBerry
static const BattleEventHandlerEntry data_ov093_021f1ac4[] = {
    { 0x55, HandlerCheriBerry },     { 0x91, HandlerCheriBerry },     { 0x2, HandlerCheriBerry },
    { 0x72, HandlerCherryBerryUse }, { 0x73, HandlerCherryBerryUse },
};

static const BattleEventHandlerEntry *EventAddCheriBerry(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f1ac4);
    return data_ov093_021f1ac4;
}

static void HandlerChestoBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonStatusReaction(item, flow, monId, 2);
}

static void HandlerChestoBerryUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonUseForStatus(item, flow, monId, 2);
}

// pokebw2's sHandlersChestoBerry
static const BattleEventHandlerEntry data_ov093_021f1aec[] = {
    { 0x55, HandlerChestoBerry },    { 0x91, HandlerChestoBerry },    { 0x2, HandlerChestoBerry },
    { 0x72, HandlerChestoBerryUse }, { 0x73, HandlerChestoBerryUse },
};

static const BattleEventHandlerEntry *EventAddChestoBerry(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f1aec);
    return data_ov093_021f1aec;
}

static void HandlerRawstBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonStatusReaction(item, flow, monId, 4);
}

static void HandlerRawstBerryUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonUseForStatus(item, flow, monId, 4);
}

// pokebw2's sHandlersRawstBerry
static const BattleEventHandlerEntry data_ov093_021f1de4[] = {
    { 0x55, HandlerRawstBerry },    { 0x91, HandlerRawstBerry },    { 0x2, HandlerRawstBerry },
    { 0x72, HandlerRawstBerryUse }, { 0x73, HandlerRawstBerryUse },
};

static const BattleEventHandlerEntry *EventAddRawstBerry(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f1de4);
    return data_ov093_021f1de4;
}

static void HandlerAspearBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonStatusReaction(item, flow, monId, 3);
}

static void HandlerAspearBerryUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonUseForStatus(item, flow, monId, 3);
}

// pokebw2's sHandlersAspearBerry
static const BattleEventHandlerEntry data_ov093_021f1b14[] = {
    { 0x55, HandlerAspearBerry },    { 0x91, HandlerAspearBerry },    { 0x2, HandlerAspearBerry },
    { 0x72, HandlerAspearBerryUse }, { 0x73, HandlerAspearBerryUse },
};

static const BattleEventHandlerEntry *EventAddAspearBerry(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f1b14);
    return data_ov093_021f1b14;
}

static void HandlerPersimBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonStatusReaction(item, flow, monId, 6);
}

static void HandlerPersimBerryUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonUseForStatus(item, flow, monId, 6);
}

// pokebw2's sHandlersPersimBerry
static const BattleEventHandlerEntry data_ov093_021f1b3c[] = {
    { 0x55, HandlerPersimBerry },    { 0x91, HandlerPersimBerry },    { 0x2, HandlerPersimBerry },
    { 0x72, HandlerPersimBerryUse }, { 0x73, HandlerPersimBerryUse },
};

static const BattleEventHandlerEntry *EventAddPersimBerry(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f1b3c);
    return data_ov093_021f1b3c;
}

static void HandlerPechaBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonStatusReaction(item, flow, monId, 5);
}

static void HandlerPechaBerryUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonUseForStatus(item, flow, monId, 5);
}

// pokebw2's sHandlersPechaBerry
const BattleEventHandlerEntry data_ov093_021f1b64[] = {
    { 0x55, HandlerPechaBerry },    { 0x91, HandlerPechaBerry },    { 0x2, HandlerPechaBerry },
    { 0x72, HandlerPechaBerryUse }, { 0x73, HandlerPechaBerryUse },
};

static const BattleEventHandlerEntry *EventAddPechaBerry(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f1b64);
    return data_ov093_021f1b64;
}

static void HandlerLumBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonStatusReaction(item, flow, monId, 0x25);
}

static void HandlerLumBerryUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonUseForStatus(item, flow, monId, 0x25);
}

// pokebw2's sHandlersLumBerry
static const BattleEventHandlerEntry data_ov093_021f1b8c[] = {
    { 0x55, HandlerLumBerry },    { 0x91, HandlerLumBerry },    { 0x2, HandlerLumBerry },
    { 0x72, HandlerLumBerryUse }, { 0x73, HandlerLumBerryUse },
};

static const BattleEventHandlerEntry *EventAddLumBerry(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f1b8c);
    return data_ov093_021f1b8c;
}

static void CommonStatusReaction(BattleEventItem *item, BtlServerFlow *flow, u8 monId, u32 condition) {
    s32 context;

    if (BattleEventVar_GetValue(2) == monId) {
        if (!func_ov093_021d778c(0x2e, &context) || context == 3 || context == 0) {
            if (CommonConditionCodeMatch(flow, monId, condition)) {
                ItemEvent_PushRun(item, flow, monId);
            }
        }
    }
}

static void CommonUseForStatus(BattleEventItem *item, BtlServerFlow *flow, u8 monId, u32 condition) {
    BattleHandlerCureConditionParam *param;

    if (BattleEventVar_GetValue(2) == monId && CommonConditionCodeMatch(flow, monId, condition)) {
        param = BattleHandler_PushWork(flow, 0xb, monId);
        param->condition = condition;
        param->count = 1;
        param->monIds[0] = monId;
        BattleHandler_PopWork(flow, param);
    }
}

static BOOL CommonConditionCodeMatch(BtlServerFlow *flow, u8 monId, u32 condition) {
    BattleMon *mon;

    mon = GetBattleMon(flow, monId);
    switch (condition) {
    case 0x25:
        if (GetBattleMonStatus(mon) != 0 || CheckCondition(mon, 6)) {
            return TRUE;
        }
        break;
    case 0x24:
        if (GetBattleMonStatus(mon) != 0) {
            return TRUE;
        }
        return FALSE;
    case 0x26:
        if (func_ov093_021d7e10(mon)) {
            return TRUE;
        }
        return FALSE;
    default:
        return CheckCondition(mon, condition);
    }
    return FALSE;
}

// pokebw2's sHandlersLeppaBerry
static const BattleEventHandlerEntry data_ov093_021f1e6c[] = {
    { 0x4, HandlerLeppaBerryMoveEnd },  { 0x91, HandlerLeppaBerryReaction }, { 0x55, HandlerLeppaBerryGet },
    { 0x9d, HandlerLeppaBerryGet },     { 0x4f, HandlerLeppaBerryPPUsed },   { 0x72, HandlerLeppaBerryUse },
    { 0x73, HandlerLeppaBerryUseTemp },
};

static const BattleEventHandlerEntry *EventAddLeppaBerry(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f1e6c);
    return data_ov093_021f1e6c;
}

static void HandlerLeppaBerryMoveEnd(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (BattleEventVar_GetValue(2) == monId) {
        if (CommonLeppaBerryLastMoveID(flow, monId) != 4) {
            ItemEvent_PushRun(item, flow, monId);
        }
    }
}

static void HandlerLeppaBerryReaction(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    u32 context;

    if (BattleEventVar_GetValue(2) == monId) {
        context = BattleEventVar_GetValue(0x2e);
        if (context == 2 || context == 0) {
            if (CommonLeppaBerryEnableMoveID(flow, monId) != 4) {
                ItemEvent_PushRun(item, flow, monId);
            }
        }
    }
}

static void HandlerLeppaBerryGet(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (BattleEventVar_GetValue(2) == monId) {
        if (CommonLeppaBerryEnableMoveID(flow, monId) != 4) {
            ItemEvent_PushRun(item, flow, monId);
        }
    }
}

static void HandlerLeppaBerryPPUsed(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (BattleEventVar_GetValue(2) == monId) {
        if (CommonLeppaBerryLastMoveID(flow, monId) != 4) {
            BattleEventVar_RewriteValue(0x51, 1);
        }
    }
}

static void HandlerLeppaBerryUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (BattleEventVar_GetValue(2) == monId) {
        HandlerLeppaBerryCommon(item, flow, monId, work, FALSE);
    }
}

static void HandlerLeppaBerryUseTemp(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (work[6] != 0) {
        HandlerLeppaBerryCommon(item, flow, monId, work, TRUE);
    }
}

static u8 CommonLeppaBerryLastMoveID(BtlServerFlow *flow, u8 monId) {
    BattleMon *mon;
    u8 moveIndex;

    mon = GetBattleMon(flow, monId);
    moveIndex = func_ov093_021d5778(mon, GetPreviousMoveUsed(mon));
    if (moveIndex != 4 && GetMovePP(mon, moveIndex) == 0) {
        return moveIndex;
    }
    return 4;
}

static u8 CommonLeppaBerryEnableMoveID(BtlServerFlow *flow, u8 monId) {
    BattleMon *mon;
    u32 count;
    u32 i;

    mon = GetBattleMon(flow, monId);
    count = GetBattleMonMoveCount(mon);
    for (i = 0; i < count; i++) {
        if (GetMovePP(mon, i) == 0) {
            return i;
        }
    }
    return 4;
}

static BOOL HandlerLeppaBerryCommon(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work, BOOL anyUsed) {
    BattleMon *mon;
    BattleHandlerPPParam *param;
    u32 count;
    u8 moveIndex;
    u32 i;

    mon = GetBattleMon(flow, monId);
    moveIndex = CommonLeppaBerryLastMoveID(flow, monId);
    if (moveIndex == 4) {
        moveIndex = CommonLeppaBerryEnableMoveID(flow, monId);
        if (moveIndex == 4 && anyUsed) {
            count = GetBattleMonMoveCount(mon);
            for (i = 0; i < count; i++) {
                if (func_ov093_021d554c(mon, i) != 0) {
                    moveIndex = i;
                    break;
                }
            }
        }
    }
    if (moveIndex != 4) {
        param = BattleHandler_PushWork(flow, 9, monId);
        param->amount = 10;
        param->monIndex = monId;
        param->moveIndex = moveIndex;
        param->currentMoves = 1;
        BattleHandler_StrSetup(&param->string, 2, 0x38f);
        BattleHandler_AddArg(&param->string, monId);
        BattleHandler_AddArg(&param->string, BattleEventItem_GetSubID(item));
        BattleHandler_AddArg(&param->string, MoveGetID(mon, moveIndex));
        BattleHandler_PopWork(flow, param);
        return TRUE;
    }
    return FALSE;
}

// pokebw2's sHandlersOranBerry
static const BattleEventHandlerEntry data_ov093_021f1bb4[] = {
    { 0x91, HandlerOranBerryReaction }, { 0x55, HandlerOranBerryMemberIn }, { 0x58, HandlerOranBerryCheckActivation },
    { 0x72, HandlerOranBerryUse },      { 0x73, HandlerOranBerryUse },
};

static const BattleEventHandlerEntry *EventAddOranBerry(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f1bb4);
    return data_ov093_021f1bb4;
}

static void HandlerOranBerryReaction(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (!CheckCondition(GetBattleMon(flow, monId), 0xf)) {
        CommonDamageReactCheck(item, flow, monId, 2, TRUE);
    }
}

static void HandlerOranBerryMemberIn(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (!CheckCondition(GetBattleMon(flow, monId), 0xf)) {
        CommonDamageReactCheck(item, flow, monId, 2, FALSE);
    }
}

static void HandlerOranBerryCheckActivation(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (!CheckCondition(GetBattleMon(flow, monId), 0xf) && CommonDamageReactCheckCore(flow, monId, 2)) {
        ItemEvent_PushRun(item, flow, monId);
    }
}

static void HandlerOranBerryUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    BattleHandlerRecoverHPParam *param;

    if (BattleEventVar_GetValue(2) == monId) {
        param = BattleHandler_PushWork(flow, 5, monId);
        param->targetIndex = monId;
        param->amount = CommonGetItemParam(item, 2);
        BattleHandler_StrSetup(&param->string, 2, 0x38c);
        BattleHandler_AddArg(&param->string, monId);
        BattleHandler_AddArg(&param->string, BattleEventItem_GetSubID(item));
        BattleHandler_PopWork(flow, param);
    }
}

// pokebw2's sHandlersBerryJuice
static const BattleEventHandlerEntry data_ov093_021f1aa4[] = {
    { 0x91, HandlerOranBerryReaction },
    { 0x55, HandlerOranBerryMemberIn },
    { 0x58, HandlerOranBerryCheckActivation },
    { 0x72, HandlerOranBerryUse },
};

static const BattleEventHandlerEntry *EventAddBerryJuice(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f1aa4);
    return data_ov093_021f1aa4;
}

// pokebw2's sHandlersSitrusBerry
static const BattleEventHandlerEntry data_ov093_021f1bdc[] = {
    { 0x91, HandlerOranBerryReaction }, { 0x55, HandlerOranBerryMemberIn }, { 0x58, HandlerOranBerryCheckActivation },
    { 0x72, HandlerSitrusBerryUse },    { 0x73, HandlerSitrusBerryUse },
};

static const BattleEventHandlerEntry *EventAddSitrusBerry(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f1bdc);
    return data_ov093_021f1bdc;
}

static void HandlerSitrusBerryUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    BattleHandlerRecoverHPParam *param;
    u32 maxHp;
    u8 ratio;

    if (BattleEventVar_GetValue(2) == monId) {
        maxHp = GetBattleMonStat(GetBattleMon(flow, monId), 0xe);
        ratio = CommonGetItemParam(item, 2);
        param = BattleHandler_PushWork(flow, 5, monId);
        param->targetIndex = monId;
        param->amount = maxHp * ratio / 100;
        BattleHandler_StrSetup(&param->string, 2, 0x38c);
        BattleHandler_AddArg(&param->string, monId);
        BattleHandler_AddArg(&param->string, BattleEventItem_GetSubID(item));
        BattleHandler_PopWork(flow, param);
    }
}

// pokebw2's sHandlersFigyBerry
static const BattleEventHandlerEntry data_ov093_021f1c04[] = {
    { 0x91, HandlerCommonPinchBerryReaction },
    { 0x55, HandlerCommonPinchBerryMemberIn },
    { 0x58, HandlerCommonPinchBerryCheckActivation },
    { 0x72, HandlerFigyBerryUse },
    { 0x73, HandlerFigyBerryUseTemp },
};

static const BattleEventHandlerEntry *EventAddFigyBerry(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f1c04);
    return data_ov093_021f1c04;
}

static void HandlerFigyBerryUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonPinchBerry(item, flow, monId, work, 0);
}

static void HandlerFigyBerryUseTemp(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (work[6] != 0) {
        CommonPinchBerry(item, flow, monId, work, 0);
    }
}

// pokebw2's sHandlersWikiBerry
static const BattleEventHandlerEntry data_ov093_021f1c2c[] = {
    { 0x91, HandlerCommonPinchBerryReaction },
    { 0x55, HandlerCommonPinchBerryMemberIn },
    { 0x58, HandlerCommonPinchBerryCheckActivation },
    { 0x72, HandlerWikiBerryUse },
    { 0x73, HandlerWikiBerryUseTemp },
};

static const BattleEventHandlerEntry *EventAddWikiBerry(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f1c2c);
    return data_ov093_021f1c2c;
}

static void HandlerWikiBerryUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonPinchBerry(item, flow, monId, work, 1);
}

static void HandlerWikiBerryUseTemp(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (work[6] != 0) {
        CommonPinchBerry(item, flow, monId, work, 1);
    }
}

// pokebw2's sHandlersMagoBerry
static const BattleEventHandlerEntry data_ov093_021f1c54[] = {
    { 0x91, HandlerCommonPinchBerryReaction },
    { 0x55, HandlerCommonPinchBerryMemberIn },
    { 0x58, HandlerCommonPinchBerryCheckActivation },
    { 0x72, HandlerMagoBerryUse },
    { 0x73, HandlerMagoBerryUseTemp },
};

static const BattleEventHandlerEntry *EventAddMagoBerry(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f1c54);
    return data_ov093_021f1c54;
}

static void HandlerMagoBerryUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonPinchBerry(item, flow, monId, work, 2);
}

static void HandlerMagoBerryUseTemp(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (work[6] != 0) {
        CommonPinchBerry(item, flow, monId, work, 2);
    }
}

// pokebw2's sHandlersAguavBerry
static const BattleEventHandlerEntry data_ov093_021f1c7c[] = {
    { 0x91, HandlerCommonPinchBerryReaction },
    { 0x55, HandlerCommonPinchBerryMemberIn },
    { 0x58, HandlerCommonPinchBerryCheckActivation },
    { 0x72, HandlerAguavBerryUse },
    { 0x73, HandlerAguavBerryUseTemp },
};

static const BattleEventHandlerEntry *EventAddAguavBerry(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f1c7c);
    return data_ov093_021f1c7c;
}

static void HandlerAguavBerryUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonPinchBerry(item, flow, monId, work, 3);
}

static void HandlerAguavBerryUseTemp(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (work[6] != 0) {
        CommonPinchBerry(item, flow, monId, work, 3);
    }
}

// pokebw2's sHandlersIapapaBerry
static const BattleEventHandlerEntry data_ov093_021f1ca4[] = {
    { 0x91, HandlerCommonPinchBerryReaction },
    { 0x55, HandlerCommonPinchBerryMemberIn },
    { 0x58, HandlerCommonPinchBerryCheckActivation },
    { 0x72, HandlerIapapaBerryUse },
    { 0x73, HandlerIapapaBerryUseTemp },
};

static const BattleEventHandlerEntry *EventAddIapapaBerry(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f1ca4);
    return data_ov093_021f1ca4;
}

static void HandlerIapapaBerryUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonPinchBerry(item, flow, monId, work, 4);
}

static void HandlerIapapaBerryUseTemp(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (work[6] != 0) {
        CommonPinchBerry(item, flow, monId, work, 4);
    }
}

static void HandlerCommonPinchBerryReaction(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (work[0] == 0 && !CheckCondition(GetBattleMon(flow, monId), 0xf)) {
        CommonDamageReactCheck(item, flow, monId, 2, TRUE);
    }
}

static void HandlerCommonPinchBerryMemberIn(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (work[0] == 0 && !CheckCondition(GetBattleMon(flow, monId), 0xf)) {
        CommonDamageReactCheck(item, flow, monId, 2, FALSE);
    }
}

static void HandlerCommonPinchBerryCheckActivation(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (work[0] == 0 && !CheckCondition(GetBattleMon(flow, monId), 0xf) && CommonDamageReactCheckCore(flow, monId, 2)) {
        ItemEvent_PushRun(item, flow, monId);
    }
}

static void CommonPinchBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work, u32 flavor) {
    BattleMon *mon;
    BattleHandlerRecoverHPParam *param;
    BattleHandlerAddConditionParam *confuse;

    if (work[0] == 0 && BattleEventVar_GetValue(2) == monId) {
        mon = GetBattleMon(flow, monId);
        param = BattleHandler_PushWork(flow, 5, monId);
        param->targetIndex = monId;
        param->amount = DivideMaxHPZeroCheck(mon, CommonGetItemParam(item, 2));
        BattleHandler_StrSetup(&param->string, 2, 0x38c);
        BattleHandler_AddArg(&param->string, monId);
        BattleHandler_AddArg(&param->string, BattleEventItem_GetSubID(item));
        BattleHandler_PopWork(flow, param);
        if (doesNatureAffectStat(GetSrcData(mon), flavor) == -1) {
            confuse = BattleHandler_PushWork(flow, 0xc, monId);
            confuse->targetIndex = monId;
            confuse->condition = 6;
            func_ov093_021d7dc0(6, mon, &confuse->value);
            BattleHandler_PopWork(flow, confuse);
        }
        work[0] = 1;
    }
}

// pokebw2's sHandlersLiechiBerry
static const BattleEventHandlerEntry data_ov093_021f1ccc[] = {
    { 0x91, HandlerPinchReactionCommon }, { 0x55, HandlerPinchReactionMemberIn }, { 0x58, CommonDamageReact },
    { 0x72, HandlerLiechiBerry },         { 0x73, HandlerLiechiBerry },
};

static const BattleEventHandlerEntry *EventAddLiechiBerry(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f1ccc);
    return data_ov093_021f1ccc;
}

static void HandlerLiechiBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonStatBoostBerry(flow, monId, 1, 1);
}

// pokebw2's sHandlersGanlonBerry
static const BattleEventHandlerEntry data_ov093_021f1cf4[] = {
    { 0x91, HandlerPinchReactionCommon }, { 0x55, HandlerPinchReactionMemberIn }, { 0x58, CommonDamageReact },
    { 0x72, HandlerGanlonBerry },         { 0x73, HandlerGanlonBerry },
};

static const BattleEventHandlerEntry *EventAddGanlonBerry(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f1cf4);
    return data_ov093_021f1cf4;
}

static void HandlerGanlonBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonStatBoostBerry(flow, monId, 2, 1);
}

// pokebw2's sHandlersSalacBerry
static const BattleEventHandlerEntry data_ov093_021f1d1c[] = {
    { 0x91, HandlerPinchReactionCommon }, { 0x55, HandlerPinchReactionMemberIn },
    { 0x58, CommonDamageReact },          { 0x72, HandlerSalacBerry },
    { 0x73, HandlerSalacBerry },
};

static const BattleEventHandlerEntry *EventAddSalacBerry(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f1d1c);
    return data_ov093_021f1d1c;
}

static void HandlerSalacBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonStatBoostBerry(flow, monId, 5, 1);
}

// pokebw2's sHandlersPetayaBerry
static const BattleEventHandlerEntry data_ov093_021f1d44[] = {
    { 0x91, HandlerPinchReactionCommon }, { 0x55, HandlerPinchReactionMemberIn }, { 0x58, CommonDamageReact },
    { 0x72, HandlerPetayaBerry },         { 0x73, HandlerPetayaBerry },
};

static const BattleEventHandlerEntry *EventAddPetayaBerry(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f1d44);
    return data_ov093_021f1d44;
}

static void HandlerPetayaBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonStatBoostBerry(flow, monId, 3, 1);
}

// pokebw2's sHandlersApicotBerry
static const BattleEventHandlerEntry data_ov093_021f1d6c[] = {
    { 0x91, HandlerPinchReactionCommon }, { 0x55, HandlerPinchReactionMemberIn }, { 0x58, CommonDamageReact },
    { 0x72, HandlerApicotBerry },         { 0x73, HandlerApicotBerry },
};

static const BattleEventHandlerEntry *EventAddApicotBerry(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f1d6c);
    return data_ov093_021f1d6c;
}

static void HandlerApicotBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonStatBoostBerry(flow, monId, 4, 1);
}

// pokebw2's sHandlersLansatBerry
static const BattleEventHandlerEntry data_ov093_021f1d94[] = {
    { 0x91, HandlerPinchReactionCommon }, { 0x55, HandlerPinchReactionMemberIn }, { 0x58, CommonDamageReact },
    { 0x72, HandlerLansatBerry },         { 0x73, HandlerLansatBerry },
};

static const BattleEventHandlerEntry *EventAddLansatBerry(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f1d94);
    return data_ov093_021f1d94;
}

static void HandlerLansatBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    BattleHandlerFlagParam *flag;
    BattleHandlerMessageParam *message;

    if (BattleEventVar_GetValue(2) == monId && !GetAdditionalConditionFlag(GetBattleMon(flow, monId), 9)) {
        flag = BattleHandler_PushWork(flow, 0x17, monId);
        flag->monIndex = monId;
        flag->flag = 9;
        BattleHandler_PopWork(flow, flag);
        message = BattleHandler_PushWork(flow, 4, monId);
        BattleHandler_StrSetup(&message->string, 2, 0x3e9);
        BattleHandler_AddArg(&message->string, monId);
        BattleHandler_AddArg(&message->string, BattleEventItem_GetSubID(item));
        BattleHandler_PopWork(flow, message);
    }
}

// pokebw2's sHandlersStarfBerry
static const BattleEventHandlerEntry data_ov093_021f1dbc[] = {
    { 0x91, HandlerPinchReactionCommon }, { 0x55, HandlerPinchReactionMemberIn },
    { 0x58, CommonDamageReact },          { 0x72, HandlerStarfBerry },
    { 0x73, HandlerStarfBerry },
};

static const BattleEventHandlerEntry *EventAddStarfBerry(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f1dbc);
    return data_ov093_021f1dbc;
}

// pokebw2's sStarfBerryStats
static const u8 data_ov093_021f1364[] = { 1, 2, 3, 4, 5 };

static void HandlerStarfBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    BattleMon *mon;
    u8 *stats;
    u32 i;
    u32 count;
    u8 stat;

    mon = GetBattleMon(flow, monId);
    stats = func_ov093_021c8120(flow, 5);
    for (i = 0, count = 0; i < 5; i++) {
        stat = data_ov093_021f1364[i];
        if (IsStatChangeValid(mon, stat, 1)) {
            stats[count++] = stat;
        }
    }
    if (count != 0) {
        CommonStatBoostBerry(flow, monId, stats[BattleRandom(count)], 2);
    }
}

static void CommonStatBoostBerry(BtlServerFlow *flow, u8 monId, u32 stat, s8 change) {
    BattleHandlerStatChangeParam *param;

    if (BattleEventVar_GetValue(2) == monId) {
        param = BattleHandler_PushWork(flow, 0xe, monId);
        param->stat = stat;
        param->change = change;
        param->count = 1;
        param->monIds[0] = monId;
        BattleHandler_PopWork(flow, param);
    }
}

// pokebw2's sHandlersEnigmaBerry
static const BattleEventHandlerEntry data_ov093_021f16cc[] = {
    { 0x4b, HandlerEnigmaBerry },
    { 0x72, HandlerEnigmaBerryUse },
};

static const BattleEventHandlerEntry *EventAddEnigmaBerry(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f16cc);
    return data_ov093_021f16cc;
}

static void HandlerEnigmaBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (BattleEventVar_GetValue(4) == monId && BattleEventVar_GetValue(0x46) == 0 &&
        (s32)BattleEventVar_GetValue(0x38) > 3 && !CheckCondition(GetBattleMon(flow, monId), 0xf)) {
        ItemEvent_PushRun(item, flow, monId);
    }
}

static void HandlerEnigmaBerryUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    BattleMon *mon;
    u8 ratio;
    BattleHandlerRecoverHPParam *param;

    if (BattleEventVar_GetValue(2) == monId) {
        mon = GetBattleMon(flow, monId);
        ratio = CommonGetItemParam(item, 2);
        param = BattleHandler_PushWork(flow, 5, monId);
        param->targetIndex = monId;
        param->amount = DivideMaxHPZeroCheck(mon, ratio);
        BattleHandler_PopWork(flow, param);
    }
}

// pokebw2's sHandlersOccaBerry
static const BattleEventHandlerEntry data_ov093_021f175c[] = {
    { 0x47, HandlerOccaBerry },
    { 0x44, HandlerCommonResistBerryDamageAfter },
};

static const BattleEventHandlerEntry *EventAddOccaBerry(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f175c);
    return data_ov093_021f175c;
}

static void HandlerOccaBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonResistBerry(item, flow, monId, work, 9, FALSE);
}

// pokebw2's sHandlersPasshoBerry
static const BattleEventHandlerEntry data_ov093_021f177c[] = {
    { 0x47, HandlerPasshoBerry },
    { 0x44, HandlerCommonResistBerryDamageAfter },
};

static const BattleEventHandlerEntry *EventAddPasshoBerry(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f177c);
    return data_ov093_021f177c;
}

static void HandlerPasshoBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonResistBerry(item, flow, monId, work, 10, FALSE);
}

// pokebw2's sHandlersWacanBerry
static const BattleEventHandlerEntry data_ov093_021f178c[] = {
    { 0x47, HandlerWacanBerry },
    { 0x44, HandlerCommonResistBerryDamageAfter },
};

static const BattleEventHandlerEntry *EventAddWacanBerry(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f178c);
    return data_ov093_021f178c;
}

static void HandlerWacanBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonResistBerry(item, flow, monId, work, 12, FALSE);
}

// pokebw2's sHandlersRindoBerry
static const BattleEventHandlerEntry data_ov093_021f179c[] = {
    { 0x47, HandlerRindoBerry },
    { 0x44, HandlerCommonResistBerryDamageAfter },
};

static const BattleEventHandlerEntry *EventAddRindoBerry(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f179c);
    return data_ov093_021f179c;
}

static void HandlerRindoBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonResistBerry(item, flow, monId, work, 11, FALSE);
}

// pokebw2's sHandlersYacheBerry
static const BattleEventHandlerEntry data_ov093_021f17ac[] = {
    { 0x47, HandlerYacheBerry },
    { 0x44, HandlerCommonResistBerryDamageAfter },
};

static const BattleEventHandlerEntry *EventAddYacheBerry(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f17ac);
    return data_ov093_021f17ac;
}

static void HandlerYacheBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonResistBerry(item, flow, monId, work, 14, FALSE);
}

// pokebw2's sHandlersChopleBerry
static const BattleEventHandlerEntry data_ov093_021f171c[] = {
    { 0x47, HandlerChopleBerry },
    { 0x44, HandlerCommonResistBerryDamageAfter },
};

static const BattleEventHandlerEntry *EventAddChopleBerry(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f171c);
    return data_ov093_021f171c;
}

static void HandlerChopleBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonResistBerry(item, flow, monId, work, 1, FALSE);
}

// pokebw2's sHandlersKebiaBerry
static const BattleEventHandlerEntry data_ov093_021f17bc[] = {
    { 0x47, HandlerKebiaBerry },
    { 0x44, HandlerCommonResistBerryDamageAfter },
};

static const BattleEventHandlerEntry *EventAddKebiaBerry(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f17bc);
    return data_ov093_021f17bc;
}

static void HandlerKebiaBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonResistBerry(item, flow, monId, work, 3, FALSE);
}

// pokebw2's sHandlersShucaBerry
static const BattleEventHandlerEntry data_ov093_021f17cc[] = {
    { 0x47, HandlerShucaBerry },
    { 0x44, HandlerCommonResistBerryDamageAfter },
};

static const BattleEventHandlerEntry *EventAddShucaBerry(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f17cc);
    return data_ov093_021f17cc;
}

static void HandlerShucaBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonResistBerry(item, flow, monId, work, 4, FALSE);
}

// pokebw2's sHandlersCobaBerry
static const BattleEventHandlerEntry data_ov093_021f17dc[] = {
    { 0x47, HandlerCobaBerry },
    { 0x44, HandlerCommonResistBerryDamageAfter },
};

static const BattleEventHandlerEntry *EventAddCobaBerry(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f17dc);
    return data_ov093_021f17dc;
}

static void HandlerCobaBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonResistBerry(item, flow, monId, work, 2, FALSE);
}

// pokebw2's sHandlersPayapaBerry
static const BattleEventHandlerEntry data_ov093_021f172c[] = {
    { 0x47, HandlerPayapaBerry },
    { 0x44, HandlerCommonResistBerryDamageAfter },
};

static const BattleEventHandlerEntry *EventAddPayapaBerry(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f172c);
    return data_ov093_021f172c;
}

static void HandlerPayapaBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonResistBerry(item, flow, monId, work, 13, FALSE);
}

// pokebw2's sHandlersTangaBerry
static const BattleEventHandlerEntry data_ov093_021f17ec[] = {
    { 0x47, HandlerTangaBerry },
    { 0x44, HandlerCommonResistBerryDamageAfter },
};

static const BattleEventHandlerEntry *EventAddTangaBerry(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f17ec);
    return data_ov093_021f17ec;
}

static void HandlerTangaBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonResistBerry(item, flow, monId, work, 6, FALSE);
}

// pokebw2's sHandlersChartiBerry
static const BattleEventHandlerEntry data_ov093_021f17fc[] = {
    { 0x47, HandlerChartiBerry },
    { 0x44, HandlerCommonResistBerryDamageAfter },
};

static const BattleEventHandlerEntry *EventAddChartiBerry(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f17fc);
    return data_ov093_021f17fc;
}

static void HandlerChartiBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonResistBerry(item, flow, monId, work, 5, FALSE);
}

// pokebw2's sHandlersKasibBerry
static const BattleEventHandlerEntry data_ov093_021f180c[] = {
    { 0x47, HandlerKasibBerry },
    { 0x44, HandlerCommonResistBerryDamageAfter },
};

static const BattleEventHandlerEntry *EventAddKasibBerry(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f180c);
    return data_ov093_021f180c;
}

static void HandlerKasibBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonResistBerry(item, flow, monId, work, 7, FALSE);
}

// pokebw2's sHandlersHabanBerry
static const BattleEventHandlerEntry data_ov093_021f181c[] = {
    { 0x47, HandlerHabanBerry },
    { 0x44, HandlerCommonResistBerryDamageAfter },
};

static const BattleEventHandlerEntry *EventAddHabanBerry(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f181c);
    return data_ov093_021f181c;
}

static void HandlerHabanBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonResistBerry(item, flow, monId, work, 15, FALSE);
}

// pokebw2's sHandlersColburBerry
static const BattleEventHandlerEntry data_ov093_021f182c[] = {
    { 0x47, HandlerColburBerry },
    { 0x44, HandlerCommonResistBerryDamageAfter },
};

static const BattleEventHandlerEntry *EventAddColburBerry(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f182c);
    return data_ov093_021f182c;
}

static void HandlerColburBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonResistBerry(item, flow, monId, work, 16, FALSE);
}

// pokebw2's sHandlersBabiriBerry
static const BattleEventHandlerEntry data_ov093_021f183c[] = {
    { 0x47, HandlerBabiriBerry },
    { 0x44, HandlerCommonResistBerryDamageAfter },
};

static const BattleEventHandlerEntry *EventAddBabiriBerry(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f183c);
    return data_ov093_021f183c;
}

static void HandlerBabiriBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonResistBerry(item, flow, monId, work, 8, FALSE);
}

// pokebw2's sHandlersChilanBerry
static const BattleEventHandlerEntry data_ov093_021f184c[] = {
    { 0x47, HandlerChilanBerry },
    { 0x44, HandlerCommonResistBerryDamageAfter },
};

static const BattleEventHandlerEntry *EventAddChilanBerry(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f184c);
    return data_ov093_021f184c;
}

static void HandlerChilanBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonResistBerry(item, flow, monId, work, 0, TRUE);
}

static BOOL CommonResistBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work, u8 type,
                              BOOL anyEffectiveness) {
    if (BattleEventVar_GetValue(4) == monId && BattleEventVar_GetValue(0x16) == type) {
        if (anyEffectiveness || (s32)BattleEventVar_GetValue(0x38) > 3) {
            if (!IsSubstituteActive(GetBattleMon(flow, monId))) {
                BattleEventVar_MulValue(0x35, FX32_CONST(0.5));
                if (!func_ov093_021c7ec4(flow)) {
                    work[0] = 1;
                }
                return TRUE;
            }
        }
    }
    return FALSE;
}

static void HandlerCommonResistBerryDamageAfter(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    BattleHandlerConsumeItemParam *param;

    if (work[0] != 0 && func_ov093_021e85f8(monId)) {
        param = BattleHandler_PushWork(flow, 0x23, monId);
        BattleHandler_StrSetup(&param->string, 2, 0xdb);
        BattleHandler_AddArg(&param->string, monId);
        BattleHandler_AddArg(&param->string, BattleEventItem_GetSubID(item));
        BattleHandler_PopWork(flow, param);
    }
}

static void HandlerPinchReactionCommon(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    BattleMon *mon;

    if (BattleEventVar_GetValue(2) == monId) {
        mon = GetBattleMon(flow, monId);
        CommonDamageReactCheck(item, flow, monId, CommonGetItemParam(item, 2), TRUE);
    }
}

static void HandlerPinchReactionMemberIn(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    BattleMon *mon;

    if (BattleEventVar_GetValue(2) == monId) {
        mon = GetBattleMon(flow, monId);
        CommonDamageReactCheck(item, flow, monId, CommonGetItemParam(item, 2), FALSE);
    }
}

static void CommonDamageReact(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    BattleMon *mon;

    mon = GetBattleMon(flow, monId);
    if (CommonDamageReactCheckCore(flow, monId, CommonGetItemParam(item, 2))) {
        ItemEvent_PushRun(item, flow, monId);
    }
}

static void CommonDamageReactCheck(BattleEventItem *item, BtlServerFlow *flow, u8 monId, u32 ratio, BOOL checkContext) {
    BOOL react;

    if (BattleEventVar_GetValue(2) == monId) {
        if (checkContext) {
            react = func_ov093_021dde74(item, flow, monId, ratio);
        } else {
            react = CommonDamageReactCheckCore(flow, monId, ratio);
        }
        if (react) {
            ItemEvent_PushRun(item, flow, monId);
        }
    }
}

static BOOL func_ov093_021dde74(BattleEventItem *item, BtlServerFlow *flow, u8 monId, u32 ratio) {
    u8 context;

    if (BattleEventVar_GetValue(2) == monId) {
        context = BattleEventVar_GetValue(0x2e);
        if (context == 0 || context == 1) {
            return CommonDamageReactCheckCore(flow, monId, ratio);
        }
    }
    return FALSE;
}

static BOOL CommonDamageReactCheckCore(BtlServerFlow *flow, u8 monId, u32 ratio) {
    BattleMon *mon;
    u32 hp;

    mon = GetBattleMon(flow, monId);
    if (GetBattleMonStat(mon, 0x11) == 0x52 && ratio > 2) {
        ratio /= 2;
    }
    if (ratio == 0) {
        ratio = 1;
    }
    if (GetBattleMonStat(mon, 0xe) > 1) {
        hp = GetBattleMonStat(mon, 0xd);
        if (hp <= DivideMaxHp(mon, ratio)) {
            return TRUE;
        }
    }
    return FALSE;
}

// pokebw2's sHandlersCustapBerry
static const BattleEventHandlerEntry data_ov093_021f15ec[] = {
    { 0xf, HandlerCustapBerryPriorityCheck },
    { 0x72, HandlerCustapBerryUse },
};

static const BattleEventHandlerEntry *EventAddCustapBerry(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f15ec);
    return data_ov093_021f15ec;
}

static void HandlerCustapBerryPriorityCheck(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (BattleEventVar_GetValue(2) == monId && CommonDamageReactCheckCore(flow, monId, 4) &&
        BattleEventVar_RewriteValue(0x11, 2)) {
        ItemEvent_PushRun(item, flow, monId);
    }
}

static void HandlerCustapBerryUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    BattleHandlerMessageParam *param;

    if (BattleEventVar_GetValue(2) == monId) {
        param = BattleHandler_PushWork(flow, 4, monId);
        BattleHandler_StrSetup(&param->string, 2, 0x401);
        BattleHandler_AddArg(&param->string, monId);
        BattleHandler_AddArg(&param->string, BattleEventItem_GetSubID(item));
        BattleHandler_PopWork(flow, param);
        work[0] = 0;
    }
}

// pokebw2's sHandlersMicleBerry
static const BattleEventHandlerEntry data_ov093_021f1e0c[] = {
    { 0x91, HandlerMicleBerryReaction }, { 0x55, HandlerMicleBerryActProcEnd }, { 0x2, HandlerMicleBerryActionEnd },
    { 0x58, HandlerMicleBerryMemberIn }, { 0x72, HandlerMicleBerryUse },        { 0x73, HandlerMicleBerryUseTemp },
};

static const BattleEventHandlerEntry *EventAddMicleBerry(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f1e0c);
    return data_ov093_021f1e0c;
}

static void HandlerMicleBerryReaction(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    BattleMon *mon;
    u8 ratio;

    if (BattleEventVar_GetValue(2) == monId) {
        mon = GetBattleMon(flow, monId);
        ratio = CommonGetItemParam(item, 2);
        if (func_ov093_021dde74(item, flow, monId, ratio)) {
            ItemEvent_PushRun(item, flow, monId);
        }
    }
}

static void HandlerMicleBerryActProcEnd(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (BattleEventVar_GetValue(2) == monId) {
        HandlerMicleBerryMemberIn(item, flow, monId, work);
    }
}

static void HandlerMicleBerryActionEnd(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (BattleEventVar_GetValue(0xc) != 6) {
        HandlerMicleBerryActProcEnd(item, flow, monId, work);
    }
}

static void HandlerMicleBerryMemberIn(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    BattleMon *mon;
    u8 ratio;

    mon = GetBattleMon(flow, monId);
    ratio = CommonGetItemParam(item, 2);
    if (CommonDamageReactCheckCore(flow, monId, ratio)) {
        ItemEvent_PushRun(item, flow, monId);
    }
}

static void HandlerMicleBerryUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    BattleMon *mon;
    BattleHandlerAddConditionParam *param;

    if (BattleEventVar_GetValue(2) == monId) {
        mon = GetBattleMon(flow, monId);
        param = BattleHandler_PushWork(flow, 0xc, monId);
        param->targetIndex = monId;
        param->condition = 0x22;
        param->value = MakeConditionParamPermanent(0x78);
        BattleHandler_StrSetup(&param->string, 2, 0x404);
        BattleHandler_AddArg(&param->string, monId);
        BattleHandler_AddArg(&param->string, BattleEventItem_GetSubID(item));
        BattleHandler_PopWork(flow, param);
    }
}

static void HandlerMicleBerryUseTemp(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (work[6] != 0) {
        HandlerMicleBerryUse(item, flow, monId, work);
    }
}

// pokebw2's sHandlersJabocaBerry
static const BattleEventHandlerEntry data_ov093_021f160c[] = {
    { 0x4b, HandlerJabocaBerry },
    { 0x72, HandlerJabocaRowapBerryUse },
};

static const BattleEventHandlerEntry *EventAddJabocaBerry(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f160c);
    return data_ov093_021f160c;
}

static void HandlerJabocaBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonJabocaRowapBerryReaction(item, flow, monId, work, 1);
}

// pokebw2's sHandlersRowapBerry
static const BattleEventHandlerEntry data_ov093_021f162c[] = {
    { 0x4b, HandlerRowapBerry },
    { 0x72, HandlerJabocaRowapBerryUse },
};

static const BattleEventHandlerEntry *EventAddRowapBerry(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f162c);
    return data_ov093_021f162c;
}

static void HandlerRowapBerry(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonJabocaRowapBerryReaction(item, flow, monId, work, 2);
}

static void CommonJabocaRowapBerryReaction(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work,
                                           u32 category) {
    BattleHandlerUseHeldItemParam *param;

    if (BattleEventVar_GetValue(4) == monId && BattleEventVar_GetValue(0x46) == 0 &&
        !data_ov095_0689c8cd(BattleEventVar_GetValue(0x12)) && BattleEventVar_GetValue(0x1a) == category) {
        BattleEventItem_SetRecallEnable(item);
        work[0] = BattleEventVar_GetValue(3) + 1;
        param = BattleHandler_PushWork(flow, 0, monId);
        param->allowFainted = 1;
        BattleHandler_PopWork(flow, param);
    }
}

static void HandlerJabocaRowapBerryUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    u8 attackerId;
    BattleMon *attacker;
    u8 ratio;
    BattleHandlerDamageParam *param;

    if (BattleEventVar_GetValue(2) == monId && work[0] != 0) {
        attackerId = work[0] - 1;
        attacker = GetBattleMon(flow, attackerId);
        if (!IsFainted(attacker)) {
            ratio = CommonGetItemParam(item, 2);
            param = BattleHandler_PushWork(flow, 7, monId);
            param->targetIndex = attackerId;
            param->amount = DivideMaxHPZeroCheck(attacker, ratio);
            BattleHandler_StrSetup(&param->string, 2, 0x407);
            BattleHandler_AddArg(&param->string, param->targetIndex);
            BattleHandler_AddArg(&param->string, monId);
            BattleHandler_AddArg(&param->string, BattleEventItem_GetSubID(item));
            BattleHandler_PopWork(flow, param);
        }
    }
}

// pokebw2's sHandlersWhiteHerb
static const BattleEventHandlerEntry data_ov093_021f1e3c[] = {
    { 0x57, HandlerWhiteHerbReaction },  { 0x2, func_ov093_021de24c },  { 0x58, HandlerWhiteHerbReaction },
    { 0x77, HandlerWhiteHerbTurnCheck }, { 0x72, HandlerWhiteHerbUse }, { 0x73, HandlerWhiteHerbUseTemp },
};

static const BattleEventHandlerEntry *EventAddWhiteHerb(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f1e3c);
    return data_ov093_021f1e3c;
}

static void HandlerWhiteHerbReaction(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (AreStatsLowered(GetBattleMon(flow, monId))) {
        ItemEvent_PushRun(item, flow, monId);
    }
}

static void func_ov093_021de24c(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (BattleEventVar_GetValue(0xc) != 6) {
        HandlerWhiteHerbReaction(item, flow, monId, work);
    }
}

static void HandlerWhiteHerbTurnCheck(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (BattleEventVar_GetValue(2) == monId && AreStatsLowered(GetBattleMon(flow, monId))) {
        ItemEvent_PushRun(item, flow, monId);
    }
}

static void HandlerWhiteHerbUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    BattleHandlerRecoverStatStageParam *recover;
    BattleHandlerMessageParam *message;

    if (BattleEventVar_GetValue(2) == monId) {
        recover = BattleHandler_PushWork(flow, 0x12, monId);
        recover->monIndex = monId;
        BattleHandler_PopWork(flow, recover);
        message = BattleHandler_PushWork(flow, 4, monId);
        BattleHandler_StrSetup(&message->string, 2, 0x3f2);
        BattleHandler_AddArg(&message->string, monId);
        BattleHandler_AddArg(&message->string, BattleEventItem_GetSubID(item));
        BattleHandler_PopWork(flow, message);
    }
}

static void HandlerWhiteHerbUseTemp(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (work[6] != 0) {
        HandlerWhiteHerbUse(item, flow, monId, work);
    }
}

// pokebw2's sHandlersMentalHerb
static const BattleEventHandlerEntry data_ov093_021f1a84[] = {
    { 0x91, HandlerMentalHerbReaction },
    { 0x55, HandlerMentalHerbReaction },
    { 0x72, HandlerMentalHerbUse },
    { 0x73, HandlerMentalHerbUseTemp },
};

static const BattleEventHandlerEntry *EventAddMentalHerb(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f1a84);
    return data_ov093_021f1a84;
}

static void HandlerMentalHerbReaction(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (BattleEventVar_GetValue(2) == monId && func_ov093_021d7e10(GetBattleMon(flow, monId))) {
        ItemEvent_PushRun(item, flow, monId);
    }
}

static void HandlerMentalHerbUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonUseForStatus(item, flow, monId, 0x26);
}

static void HandlerMentalHerbUseTemp(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (work[6] != 0) {
        HandlerMentalHerbUse(item, flow, monId, work);
    }
}

// pokebw2's sHandlersBrightPowder
static const BattleEventHandlerEntry data_ov093_021f1564[] = {
    { 0x34, HandlerBrightPowder },
};

static const BattleEventHandlerEntry *EventAddBrightPowder(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f1564);
    return data_ov093_021f1564;
}

static void HandlerBrightPowder(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    u32 ratio;

    if (BattleEventVar_GetValue(4) == monId) {
        ratio = 100 - CommonGetItemParam(item, 2);
        BattleEventVar_MulValue(0x35, FX32_CONST(ratio) / 100);
    }
}

// pokebw2's sHandlersMachoBrace
static const BattleEventHandlerEntry data_ov093_021f156c[] = {
    { 0x13, HandlerMachoBrace },
};

static const BattleEventHandlerEntry *EventAddMachoBrace(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f156c);
    return data_ov093_021f156c;
}

static void HandlerMachoBrace(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (BattleEventVar_GetValue(2) == monId) {
        BattleEventVar_MulValue(0x35, FX32_CONST(0.5));
    }
}

// pokebw2's sHandlersQuickClaw
static const BattleEventHandlerEntry data_ov093_021f163c[] = {
    { 0xf, HandlerQuickClawPriorityCheck },
    { 0x72, HandlerQuickClawUse },
};

static const BattleEventHandlerEntry *EventAddQuickClaw(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f163c);
    return data_ov093_021f163c;
}

static void HandlerQuickClawPriorityCheck(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    u8 chance;

    if (BattleEventVar_GetValue(2) == monId) {
        chance = CommonGetItemParam(item, 2);
        if (ItemEvent_RollEffectChance(flow, chance) && BattleEventVar_RewriteValue(0x11, 2)) {
            ItemEvent_PushRun(item, flow, monId);
        }
    }
}

static void HandlerQuickClawUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    BattleHandlerMessageParam *param;

    if (BattleEventVar_GetValue(2) == monId) {
        param = BattleHandler_PushWork(flow, 4, monId);
        BattleHandler_StrSetup(&param->string, 2, 0x401);
        BattleHandler_AddArg(&param->string, monId);
        BattleHandler_AddArg(&param->string, BattleEventItem_GetSubID(item));
        BattleHandler_PopWork(flow, param);
        work[0] = 0;
    }
}

// pokebw2's sHandlersLaggingTail
static const BattleEventHandlerEntry data_ov093_021f1394[] = {
    { 0xf, HandlerLaggingTail },
};

static const BattleEventHandlerEntry *EventAddLaggingTail(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f1394);
    return data_ov093_021f1394;
}

static void HandlerLaggingTail(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (BattleEventVar_GetValue(2) == monId) {
        BattleEventVar_RewriteValue(0x11, 0);
    }
}

// pokebw2's sHandlersKingsRock
static const BattleEventHandlerEntry data_ov093_021f164c[] = {
    { 0x6c, HandlerKingsRock },
    { 0x73, HandlerKingsRockUseTemp },
};

static const BattleEventHandlerEntry *EventAddKingsRock(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f164c);
    return data_ov093_021f164c;
}

static void HandlerKingsRock(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    u8 chance;

    if (BattleEventVar_GetValue(3) == monId && BattleEventVar_GetValue(0x25) == 0) {
        chance = CommonGetItemParam(item, 2);
        BattleEventVar_RewriteValue(0x26, chance);
    }
}

static void HandlerKingsRockUseTemp(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    BattleHandlerFlinchParam *param;

    if (BattleEventVar_GetValue(2) == monId) {
        param = BattleHandler_PushWork(flow, 0x2b, monId);
        param->monIndex = monId;
        param->flag = 100;
        BattleHandler_PopWork(flow, param);
    }
}

// pokebw2's sHandlersRazorFang
static const BattleEventHandlerEntry data_ov093_021f165c[] = {
    { 0x6c, HandlerKingsRock },
    { 0x73, HandlerKingsRockUseTemp },
};

static const BattleEventHandlerEntry *EventAddRazorFang(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f165c);
    return data_ov093_021f165c;
}

// pokebw2's sHandlersWideLens
static const BattleEventHandlerEntry data_ov093_021f1584[] = {
    { 0x34, HandlerWideLens },
};

static const BattleEventHandlerEntry *EventAddWideLens(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f1584);
    return data_ov093_021f1584;
}

static void HandlerWideLens(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (BattleEventVar_GetValue(3) == monId) {
        BattleEventVar_MulValue(0x35, ItemAttackValueToRatio(item));
    }
}

// pokebw2's sHandlersScopeLens
static const BattleEventHandlerEntry data_ov093_021f158c[] = {
    { 0x36, HandlerScopeLens },
};

static const BattleEventHandlerEntry *EventAddScopeLens(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f158c);
    return data_ov093_021f158c;
}

static void HandlerScopeLens(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    u8 rank;

    if (BattleEventVar_GetValue(3) == monId) {
        rank = BattleEventVar_GetValue(0x2c);
        BattleEventVar_RewriteValue(0x2c, (u8)(rank + 1));
    }
}

// pokebw2's sHandlersLuckyPunch
static const BattleEventHandlerEntry data_ov093_021f146c[] = {
    { 0x36, HandlerLuckyPunch },
};

static const BattleEventHandlerEntry *EventAddLuckyPunch(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f146c);
    return data_ov093_021f146c;
}

static void HandlerLuckyPunch(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    u8 rank;

    if (BattleEventVar_GetValue(3) == monId && GetBattleMonSpecies(GetBattleMon(flow, monId)) == 0x71) {
        rank = BattleEventVar_GetValue(0x2c);
        BattleEventVar_RewriteValue(0x2c, (u8)(rank + 2));
    }
}

// pokebw2's sHandlersStick
static const BattleEventHandlerEntry data_ov093_021f13dc[] = {
    { 0x36, HandlerStick },
};

static const BattleEventHandlerEntry *EventAddStick(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f13dc);
    return data_ov093_021f13dc;
}

static void HandlerStick(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    u8 rank;

    if (BattleEventVar_GetValue(3) == monId && GetBattleMonSpecies(GetBattleMon(flow, monId)) == 0x53) {
        rank = BattleEventVar_GetValue(0x2c);
        BattleEventVar_RewriteValue(0x2c, (u8)(rank + 2));
    }
}

// pokebw2's sHandlersZoomLens
static const BattleEventHandlerEntry data_ov093_021f13d4[] = {
    { 0x34, HandlerZoomLens },
};

static const BattleEventHandlerEntry *EventAddZoomLens(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f13d4);
    return data_ov093_021f13d4;
}

static void HandlerZoomLens(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    u8 targetId;

    if (BattleEventVar_GetValue(3) == monId) {
        targetId = BattleEventVar_GetValue(4);
        if (GetTurnFlag(GetBattleMon(flow, targetId), 1)) {
            BattleEventVar_MulValue(0x35, ItemAttackValueToRatio(item));
        }
    }
}

// pokebw2's sHandlersLaxIncense
static const BattleEventHandlerEntry data_ov093_021f1474[] = {
    { 0x34, HandlerLaxIncense },
};

static const BattleEventHandlerEntry *EventAddLaxIncense(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f1474);
    return data_ov093_021f1474;
}

static void HandlerLaxIncense(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    u16 ratio;

    if (BattleEventVar_GetValue(4) == monId) {
        ratio = 100 - CommonGetItemParam(item, 2);
        BattleEventVar_MulValue(0x35, FX32_CONST(ratio) / 100);
    }
}

// pokebw2's sHandlersMuscleBand
static const BattleEventHandlerEntry data_ov093_021f13e4[] = {
    { 0x38, HandlerMuscleBand },
};

static const BattleEventHandlerEntry *EventAddMuscleBand(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f13e4);
    return data_ov093_021f13e4;
}

static void HandlerMuscleBand(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (BattleEventVar_GetValue(3) == monId && PML_MoveGetCategory(BattleEventVar_GetValue(0x12)) == 1) {
        BattleEventVar_MulValue(0x31, ItemAttackValueToRatio(item));
    }
}

// pokebw2's sHandlersWiseGlasses
static const BattleEventHandlerEntry data_ov093_021f148c[] = {
    { 0x38, HandlerWiseGlasses },
};

static const BattleEventHandlerEntry *EventAddWiseGlasses(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f148c);
    return data_ov093_021f148c;
}

static void HandlerWiseGlasses(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (BattleEventVar_GetValue(3) == monId && PML_MoveGetCategory(BattleEventVar_GetValue(0x12)) == 2) {
        BattleEventVar_MulValue(0x31, ItemAttackValueToRatio(item));
    }
}

// pokebw2's sHandlersDeepSeaTooth
static const BattleEventHandlerEntry data_ov093_021f13ec[] = {
    { 0x3b, HandlerDeepSeaTooth },
};

static const BattleEventHandlerEntry *EventAddDeepSeaTooth(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f13ec);
    return data_ov093_021f13ec;
}

static void HandlerDeepSeaTooth(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (BattleEventVar_GetValue(3) == monId && GetBattleMonSpecies(GetBattleMon(flow, monId)) == 0x16e &&
        PML_MoveGetCategory(BattleEventVar_GetValue(0x12)) == 2) {
        BattleEventVar_MulValue(0x35, FX32_CONST(2));
    }
}

// pokebw2's sHandlersDeepSeaScale
static const BattleEventHandlerEntry data_ov093_021f139c[] = {
    { 0x3c, HandlerDeepSeaScale },
};

static const BattleEventHandlerEntry *EventAddDeepSeaScale(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f139c);
    return data_ov093_021f139c;
}

static void HandlerDeepSeaScale(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (BattleEventVar_GetValue(4) == monId && GetBattleMonSpecies(GetBattleMon(flow, monId)) == 0x16e &&
        BattleEventVar_GetValue(0x1a) == 2) {
        BattleEventVar_MulValue(0x35, FX32_CONST(2));
    }
}

// pokebw2's sHandlersMetalPowder
static const BattleEventHandlerEntry data_ov093_021f1454[] = {
    { 0x3c, HandlerMetalPowder },
};

static const BattleEventHandlerEntry *EventAddMetalPowder(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f1454);
    return data_ov093_021f1454;
}

static void HandlerMetalPowder(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    BattleMon *mon;

    if (BattleEventVar_GetValue(4) == monId) {
        mon = GetBattleMon(flow, monId);
        if (GetBattleMonSpecies(mon) == 0x84 && !TransformCheck(mon) && BattleEventVar_GetValue(0x1a) == 1) {
            BattleEventVar_MulValue(0x35, FX32_CONST(2));
        }
    }
}

// pokebw2's sHandlersQuickPowder
static const BattleEventHandlerEntry data_ov093_021f14ac[] = {
    { 0x13, HandlerQuickPowder },
};

static const BattleEventHandlerEntry *EventAddQuickPowder(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f14ac);
    return data_ov093_021f14ac;
}

static void HandlerQuickPowder(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    BattleMon *mon;

    if (BattleEventVar_GetValue(2) == monId) {
        mon = GetBattleMon(flow, monId);
        if (GetBattleMonSpecies(mon) == 0x84 && !TransformCheck(mon)) {
            BattleEventVar_MulValue(0x35, FX32_CONST(2));
        }
    }
}

// pokebw2's sHandlersSoulDew
static const BattleEventHandlerEntry data_ov093_021f16ac[] = {
    { 0x3b, HandlerSoulDewAttacker },
    { 0x3c, HandlerSoulDewDefender },
};

static const BattleEventHandlerEntry *EventAddSoulDew(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov093_021f16ac);
    return data_ov093_021f16ac;
}

static void HandlerSoulDewAttacker(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    u32 species;

    if (BattleEventVar_GetValue(3) == monId) {
        species = GetBattleMonSpecies(GetBattleMon(flow, monId));
        if ((species == 0x17c || species == 0x17d) && BattleEventVar_GetValue(0x1a) == 2) {
            BattleEventVar_MulValue(0x35, FX32_CONST(1.5));
        }
    }
}

static void HandlerSoulDewDefender(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    u32 species;

    if (BattleEventVar_GetValue(4) == monId) {
        species = GetBattleMonSpecies(GetBattleMon(flow, monId));
        if ((species == 0x17c || species == 0x17d) && BattleEventVar_GetValue(0x1a) == 2) {
            BattleEventVar_MulValue(0x35, FX32_CONST(1.5));
        }
    }
}

// pokebw2's sHandlersThickClub
static const BattleEventHandlerEntry data_ov093_021f13a4[] = {
    { 0x3b, HandlerThickClub },
};

static const BattleEventHandlerEntry *EventAddThickClub(u32 *priority) {
    *priority = NELEMS(data_ov093_021f13a4);
    return data_ov093_021f13a4;
}

static void HandlerThickClub(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    u32 species;

    if (BattleEventVar_GetValue(3) == monId) {
        species = GetBattleMonSpecies(GetBattleMon(flow, monId));
        if (species == SPECIES_CUBONE || species == SPECIES_MAROWAK) {
            if (PML_MoveGetCategory(BattleEventVar_GetValue(0x12)) == MOVE_CATEGORY_PHYSICAL) {
                BattleEventVar_MulValue(0x35, FX32_CONST(2));
            }
        }
    }
}

// pokebw2's sHandlersChoiceBand
static const BattleEventHandlerEntry data_ov093_021f18d4[] = {
    { 0x22, HandlerChoiceItemCommonMoveLock },
    { 0x9c, HandlerChoiceItemCommonItemChange },
    { 0x3b, HandlerChoiceBandPower },
};

static const BattleEventHandlerEntry *EventAddChoiceBand(u32 *priority) {
    *priority = NELEMS(data_ov093_021f18d4);
    return data_ov093_021f18d4;
}

static void HandlerChoiceBandPower(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (BattleEventVar_GetValue(3) == monId) {
        if (PML_MoveGetCategory(BattleEventVar_GetValue(0x12)) == MOVE_CATEGORY_PHYSICAL) {
            BattleEventVar_MulValue(0x35, FX32_CONST(1.5));
        }
    }
}

// pokebw2's sHandlersChoiceSpecs
static const BattleEventHandlerEntry data_ov093_021f18ec[] = {
    { 0x22, HandlerChoiceItemCommonMoveLock },
    { 0x9c, HandlerChoiceItemCommonItemChange },
    { 0x3b, HandlerChoiceSpecsPower },
};

static const BattleEventHandlerEntry *EventAddChoiceSpecs(u32 *priority) {
    *priority = NELEMS(data_ov093_021f18ec);
    return data_ov093_021f18ec;
}

static void HandlerChoiceSpecsPower(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (BattleEventVar_GetValue(3) == monId) {
        if (PML_MoveGetCategory(BattleEventVar_GetValue(0x12)) == MOVE_CATEGORY_SPECIAL) {
            BattleEventVar_MulValue(0x35, FX32_CONST(1.5));
        }
    }
}

// pokebw2's sHandlersChoiceScarf
static const BattleEventHandlerEntry data_ov093_021f1904[] = {
    { 0x22, HandlerChoiceItemCommonMoveLock },
    { 0x9c, HandlerChoiceItemCommonItemChange },
    { 0x13, HandlerChoiceScarf },
};

static const BattleEventHandlerEntry *EventAddChoiceScarf(u32 *priority) {
    *priority = NELEMS(data_ov093_021f1904);
    return data_ov093_021f1904;
}

static void HandlerChoiceScarf(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (BattleEventVar_GetValue(2) == monId) {
        BattleEventVar_MulValue(0x35, FX32_CONST(1.5));
    }
}

// Locks the holder into the move it used
static void HandlerChoiceItemCommonMoveLock(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    u16 move;
    BattleHandlerAddConditionParam *param;

    if (BattleEventVar_GetValue(3) == monId) {
        move = BattleEventVar_GetValue(0x12);
        if (move != MOVE_STRUGGLE) {
            param = BattleHandler_PushWork(flow, 0xc, monId);
            param->targetIndex = monId;
            param->condition = 0x1b;
            param->value = MakeConditionParamPermanent(move);
            param->overwrite = 2;
            BattleHandler_PopWork(flow, param);
        }
    }
}

// Ends the move lock when the item changes
static void HandlerChoiceItemCommonItemChange(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    BattleHandlerCureConditionParam *param;

    if (BattleEventVar_GetValue(2) == monId) {
        param = BattleHandler_PushWork(flow, 0xb, monId);
        param->condition = 0x1b;
        param->monIds[0] = monId;
        param->count = 1;
        param->useString = 1;
        BattleHandler_PopWork(flow, param);
    }
}

// pokebw2's sHandlersFocusSash
static const BattleEventHandlerEntry data_ov093_021f16bc[] = {
    { 0x74, HandlerFocusSash },
    { 0x75, HandlerFocusSashUse },
};

static const BattleEventHandlerEntry *EventAddFocusSash(u32 *priority) {
    *priority = NELEMS(data_ov093_021f16bc);
    return data_ov093_021f16bc;
}

static void HandlerFocusSash(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (BattleEventVar_GetValue(4) == monId) {
        if (IsMonFullHP(GetBattleMon(flow, monId))) {
            *work = BattleEventVar_RewriteValue(0x3a, 3);
        } else {
            *work = 0;
        }
    }
}

static void HandlerFocusSashUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    BattleHandlerConsumeItemParam *param;

    if (BattleEventVar_GetValue(2) == monId && *work) {
        param = BattleHandler_PushWork(flow, 0x23, monId);
        BattleHandler_StrSetup(&param->string, 2, 0x3ef);
        BattleHandler_AddArg(&param->string, monId);
        BattleHandler_AddArg(&param->string, BattleEventItem_GetSubID(item));
        BattleHandler_PopWork(flow, param);
        *work = 0;
    }
}

// pokebw2's sHandlersFocusBand
static const BattleEventHandlerEntry data_ov093_021f191c[] = {
    { 0x74, HandlerFocusBandCheck },
    { 0x75, HandlerFocusBand },
    { 0x72, HandlerFocusBandUse },
};

static const BattleEventHandlerEntry *EventAddFocusBand(u32 *priority) {
    *priority = NELEMS(data_ov093_021f191c);
    return data_ov093_021f191c;
}

static void HandlerFocusBandCheck(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    u8 chance;
    if (BattleEventVar_GetValue(4) == monId) {
        chance = CommonGetItemParam(item, 2);
        if (ItemEvent_RollEffectChance(flow, chance)) {
            *work = BattleEventVar_RewriteValue(0x3a, 3);
        } else {
            *work = 0;
        }
    }
}

static void HandlerFocusBand(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (BattleEventVar_GetValue(2) == monId && *work) {
        ItemEvent_PushRun(item, flow, monId);
        *work = 0;
    }
}

static void HandlerFocusBandUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    BattleHandlerMessageParam *param;

    if (BattleEventVar_GetValue(2) == monId) {
        param = BattleHandler_PushWork(flow, 4, monId);
        BattleHandler_StrSetup(&param->string, 2, 0x3ef);
        BattleHandler_AddArg(&param->string, monId);
        BattleHandler_AddArg(&param->string, BattleEventItem_GetSubID(item));
        BattleHandler_PopWork(flow, param);
    }
}

// pokebw2's sHandlersExpertBelt
static const BattleEventHandlerEntry data_ov093_021f150c[] = {
    { 0x47, HandlerExpertBelt },
};

static const BattleEventHandlerEntry *EventAddExpertBelt(u32 *priority) {
    *priority = NELEMS(data_ov093_021f150c);
    return data_ov093_021f150c;
}

static void HandlerExpertBelt(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (BattleEventVar_GetValue(3) == monId) {
        if ((s32)BattleEventVar_GetValue(0x38) > TYPE_EFFECTIVENESS_NORMAL) {
            BattleEventVar_MulValue(0x35, ItemAttackValueToRatio(item));
        }
    }
}

// pokebw2's sHandlersLifeOrb
static const BattleEventHandlerEntry data_ov093_021f16dc[] = {
    { 0x85, HandlerLifeOrbReaction },
    { 0x47, HandlerLifeOrbPower },
};

static const BattleEventHandlerEntry *EventAddLifeOrb(u32 *priority) {
    *priority = NELEMS(data_ov093_021f16dc);
    return data_ov093_021f16dc;
}

static void HandlerLifeOrbReaction(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    BattleMon *mon;
    BattleHandlerDamageParam *param;

    if (BattleEventVar_GetValue(3) == monId) {
        mon = GetBattleMon(flow, monId);
        param = BattleHandler_PushWork(flow, 7, monId);
        param->targetIndex = monId;
        param->amount = DivideMaxHPZeroCheck(mon, 10);
        BattleHandler_StrSetup(&param->string, 2, 0x3f5);
        BattleHandler_AddArg(&param->string, monId);
        BattleHandler_PopWork(flow, param);
    }
}

static void HandlerLifeOrbPower(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (BattleEventVar_GetValue(3) == monId) {
        BattleEventVar_MulValue(0x35, ItemAttackValueToRatio(item));
    }
}

// pokebw2's sHandlersMetronomeItem
static const BattleEventHandlerEntry data_ov093_021f13bc[] = {
    { 0x47, HandlerMetronomeItem },
};

static const BattleEventHandlerEntry *EventAddMetronomeItem(u32 *priority) {
    *priority = NELEMS(data_ov093_021f13bc);
    return data_ov093_021f13bc;
}

static void HandlerMetronomeItem(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    BattleMon *mon;
    u16 count;
    u16 move;
    u16 ratio;

    if (BattleEventVar_GetValue(3) == monId) {
        if (!data_ov095_0689c8cd(BattleEventVar_GetValue(0x12))) {
            mon = GetBattleMon(flow, monId);
            count = GetConsecutiveMoveCount(mon);
            if (count >= 1) {
                move = BattleEventVar_GetValue(0x12);
                if (move == GetPreviousMoveID(mon)) {
                    ratio = 100 + CommonGetItemParam(item, 2) * count;
                    if (ratio > 200) {
                        ratio = 200;
                    }
                    BattleEventVar_MulValue(0x35, FX32_CONST(ratio) / 100);
                }
            }
        }
    }
}

// pokebw2's sHandlersGripClaw
static const BattleEventHandlerEntry data_ov093_021f1384[] = {
    { 0x62, HandlerGripClaw },
};

static const BattleEventHandlerEntry *EventAddGripClaw(u32 *priority) {
    *priority = NELEMS(data_ov093_021f1384);
    return data_ov093_021f1384;
}

static void HandlerGripClaw(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    BattleCondition condition;

    if (BattleEventVar_GetValue(3) == monId && BattleEventVar_GetValue(0x1d) == 8) {
        condition.raw = BattleEventVar_GetValue(0x1e);
        SetTurns(&condition, 8);
        BattleEventVar_RewriteValue(0x1e, condition.raw);
    }
}

// pokebw2's sHandlersShellBell
static const BattleEventHandlerEntry data_ov093_021f152c[] = {
    { 0x85, HandlerShellBell },
};

static const BattleEventHandlerEntry *EventAddShellBell(u32 *priority) {
    *priority = NELEMS(data_ov093_021f152c);
    return data_ov093_021f152c;
}

static void HandlerShellBell(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    u32 damage;
    u32 amount;
    BattleHandlerRecoverHPParam *param;

    if (BattleEventVar_GetValue(3) == monId && BattleEventVar_GetValue(0x4d) == 0) {
        damage = BattleEventVar_GetValue(0x32);
        if (damage != 0) {
            amount = damage / CommonGetItemParam(item, 2);
            if (amount == 0) {
                amount = 1;
            }
            param = BattleHandler_PushWork(flow, 5, monId);
            param->targetIndex = monId;
            param->amount = amount;
            BattleHandler_StrSetup(&param->string, 2, 0x392);
            BattleHandler_AddArg(&param->string, monId);
            BattleHandler_AddArg(&param->string, BattleEventItem_GetSubID(item));
            BattleHandler_PopWork(flow, param);
        }
    }
}

// pokebw2's sHandlersLightClay
static const BattleEventHandlerEntry data_ov093_021f143c[] = {
    { 0x9f, HandlerLightClay },
};

static const BattleEventHandlerEntry *EventAddLightClay(u32 *priority) {
    *priority = NELEMS(data_ov093_021f143c);
    return data_ov093_021f143c;
}

static void HandlerLightClay(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    BattleCondition condition;
    u8 turns;

    if (BattleEventVar_GetValue(2) == monId && BattleEventVar_GetValue(0x53) <= 1) {
        condition.raw = BattleEventVar_GetValue(0x1e);
        turns = CommonGetItemParam(item, 2);
        IncrementTurn(&condition, turns);
        BattleEventVar_RewriteValue(0x1e, condition.raw);
    }
}

// pokebw2's sHandlersPowerHerb
static const BattleEventHandlerEntry data_ov093_021f197c[] = {
    { 0x94, HandlerPowerHerbCheckChargeSkip },
    { 0x97, HandlerPowerHerbFixChargeSkip },
    { 0x72, HandlerPowerHerbUse },
};

static const BattleEventHandlerEntry *EventAddPowerHerb(u32 *priority) {
    *priority = NELEMS(data_ov093_021f197c);
    return data_ov093_021f197c;
}

static void HandlerPowerHerbCheckChargeSkip(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (BattleEventVar_GetValue(3) == monId && BattleEventVar_GetValue(0x12) != MOVE_SKY_DROP) {
        if (BattleEventVar_RewriteValue(0x51, 1)) {
            *work = 1;
        }
    }
}

static void HandlerPowerHerbFixChargeSkip(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (BattleEventVar_GetValue(3) == monId && *work) {
        ItemEvent_PushRun(item, flow, monId);
    }
}

static void HandlerPowerHerbUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    BattleHandlerMessageParam *param;

    if (BattleEventVar_GetValue(2) == monId) {
        param = BattleHandler_PushWork(flow, 4, monId);
        BattleHandler_StrSetup(&param->string, 2, 0x3fe);
        BattleHandler_AddArg(&param->string, monId);
        BattleHandler_AddArg(&param->string, BattleEventItem_GetSubID(item));
        BattleHandler_PopWork(flow, param);
    }
}

// pokebw2's sHandlersLeftovers
static const BattleEventHandlerEntry data_ov093_021f16ec[] = {
    { 0x76, HandlerLeftoversReaction },
    { 0x72, HandlerLeftoversUse },
};

static const BattleEventHandlerEntry *EventAddLeftovers(u32 *priority) {
    *priority = NELEMS(data_ov093_021f16ec);
    return data_ov093_021f16ec;
}

static void HandlerLeftoversReaction(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    BattleHandlerUseHeldItemParam *param;

    if (BattleEventVar_GetValue(2) == monId && !IsMonFullHP(GetBattleMon(flow, monId))) {
        param = BattleHandler_PushWork(flow, 0, monId);
        param->checkFullHp = 1;
        BattleEventItem_SetRecallEnable(item);
        BattleHandler_PopWork(flow, param);
    }
}

static void HandlerLeftoversUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    BattleMon *mon;
    BattleHandlerRecoverHPParam *param;

    if (BattleEventVar_GetValue(2) == monId) {
        mon = GetBattleMon(flow, monId);
        if (!IsMonFullHP(mon)) {
            param = BattleHandler_PushWork(flow, 5, monId);
            param->targetIndex = monId;
            param->amount = DivideMaxHPZeroCheck(mon, 16);
            BattleHandler_StrSetup(&param->string, 2, 0x392);
            BattleHandler_AddArg(&param->string, monId);
            BattleHandler_AddArg(&param->string, BattleEventItem_GetSubID(item));
            BattleHandler_PopWork(flow, param);
        }
    }
}

// pokebw2's sHandlersBlackSludge
static const BattleEventHandlerEntry data_ov093_021f14b4[] = {
    { 0x76, HandlerBlackSludge },
};

static const BattleEventHandlerEntry *EventAddBlackSludge(u32 *priority) {
    *priority = NELEMS(data_ov093_021f14b4);
    return data_ov093_021f14b4;
}

static void HandlerBlackSludge(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    BattleMon *mon;
    BattleHandlerRecoverHPParam *recoverParam;
    BattleHandlerDamageParam *damageParam;

    if (BattleEventVar_GetValue(2) == monId && !func_ov093_021c83d4(flow)) {
        mon = GetBattleMon(flow, monId);
        if (func_ov093_021e8d24(GetPokeType(mon), TYPE_POISON)) {
            recoverParam = BattleHandler_PushWork(flow, 5, monId);
            recoverParam->targetIndex = monId;
            recoverParam->amount = DivideMaxHPZeroCheck(mon, CommonGetItemParam(item, 2));
            BattleHandler_StrSetup(&recoverParam->string, 2, 0x392);
            BattleHandler_AddArg(&recoverParam->string, monId);
            BattleHandler_AddArg(&recoverParam->string, BattleEventItem_GetSubID(item));
            BattleHandler_PopWork(flow, recoverParam);
        } else {
            damageParam = BattleHandler_PushWork(flow, 7, monId);
            damageParam->targetIndex = monId;
            damageParam->amount = DivideMaxHPZeroCheck(mon, 8);
            damageParam->showViewEffect = 1;
            damageParam->effect = 0x25f;
            damageParam->effectArg1 = func_ov093_021c8010(flow, monId);
            damageParam->effectArg2 = 6;
            BattleHandler_StrSetup(&damageParam->string, 2, 0x40e);
            BattleHandler_AddArg(&damageParam->string, monId);
            BattleHandler_AddArg(&damageParam->string, BattleEventItem_GetSubID(item));
            BattleHandler_PopWork(flow, damageParam);
        }
    }
}

// pokebw2's sHandlersDestinyKnot
static const BattleEventHandlerEntry data_ov093_021f154c[] = {
    { 0x69, HandlerDestinyKnot },
};

static const BattleEventHandlerEntry *EventAddDestinyKnot(u32 *priority) {
    *priority = NELEMS(data_ov093_021f154c);
    return data_ov093_021f154c;
}

static void HandlerDestinyKnot(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    u8 attackerId;
    BattleHandlerAddConditionParam *param;

    if (BattleEventVar_GetValue(4) == monId && BattleEventVar_GetValue(0x1d) == 7) {
        attackerId = BattleEventVar_GetValue(3);
        if (attackerId != 0x1f) {
            param = BattleHandler_PushWork(flow, 0xc, monId);
            param->targetIndex = attackerId;
            param->condition = 7;
            param->value = func_ov093_021e899c(monId);
            param->noMessage = 1;
            BattleHandler_StrSetup(&param->string, 2, 0x14a);
            BattleHandler_AddArg(&param->string, attackerId);
            BattleHandler_AddArg(&param->string, BattleEventItem_GetSubID(item));
            BattleHandler_PopWork(flow, param);
        }
    }
}

// pokebw2's sHandlersStickyBarb
static const BattleEventHandlerEntry data_ov093_021f16fc[] = {
    { 0x4b, HandlerStickyBarbDamageReaction },
    { 0x77, HandlerStickyBarbTurnCheck },
};

static const BattleEventHandlerEntry *EventAddStickyBarb(u32 *priority) {
    *priority = NELEMS(data_ov093_021f16fc);
    return data_ov093_021f16fc;
}

static void HandlerStickyBarbDamageReaction(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    u8 attackerId;
    BattleHandlerSwapItemParam *param;

    if (BattleEventVar_GetValue(4) == monId && BattleEventVar_GetValue(0x46) == 0 &&
        getMoveFlag(BattleEventVar_GetValue(0x12), 0)) {
        attackerId = BattleEventVar_GetValue(3);
        if (GetBattleMonHeldItem(GetBattleMon(flow, attackerId)) == ITEM_NONE) {
            param = BattleHandler_PushWork(flow, 0x24, monId);
            param->otherIndex = attackerId;
            BattleHandler_PopWork(flow, param);
        }
    }
}

static void HandlerStickyBarbTurnCheck(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    BattleMon *mon;
    BattleHandlerDamageParam *param;

    if (BattleEventVar_GetValue(2) == monId) {
        mon = GetBattleMon(flow, monId);
        param = BattleHandler_PushWork(flow, 7, monId);
        param->targetIndex = monId;
        param->amount = DivideMaxHPZeroCheck(mon, CommonGetItemParam(item, 2));
        param->showViewEffect = 1;
        param->effect = 0x25f;
        param->effectArg1 = func_ov093_021c8010(flow, monId);
        param->effectArg2 = 6;
        BattleHandler_StrSetup(&param->string, 2, 0x40e);
        BattleHandler_AddArg(&param->string, monId);
        BattleHandler_AddArg(&param->string, BattleEventItem_GetSubID(item));
        BattleHandler_PopWork(flow, param);
    }
}

// pokebw2's sHandlersPowerBracer
static const BattleEventHandlerEntry data_ov093_021f138c[] = {
    { 0x13, HandlerPowerItemCalcSpeed },
};

static const BattleEventHandlerEntry *EventAddPowerBracer(u32 *priority) {
    *priority = NELEMS(data_ov093_021f138c);
    return data_ov093_021f138c;
}

// pokebw2's sHandlersPowerBelt
static const BattleEventHandlerEntry data_ov093_021f1434[] = {
    { 0x13, HandlerPowerItemCalcSpeed },
};

static const BattleEventHandlerEntry *EventAddPowerBelt(u32 *priority) {
    *priority = NELEMS(data_ov093_021f1434);
    return data_ov093_021f1434;
}

// pokebw2's sHandlersPowerLens
static const BattleEventHandlerEntry data_ov093_021f15a4[] = {
    { 0x13, HandlerPowerItemCalcSpeed },
};

static const BattleEventHandlerEntry *EventAddPowerLens(u32 *priority) {
    *priority = NELEMS(data_ov093_021f15a4);
    return data_ov093_021f15a4;
}

// pokebw2's sHandlersPowerBand
static const BattleEventHandlerEntry data_ov093_021f1554[] = {
    { 0x13, HandlerPowerItemCalcSpeed },
};

static const BattleEventHandlerEntry *EventAddPowerBand(u32 *priority) {
    *priority = NELEMS(data_ov093_021f1554);
    return data_ov093_021f1554;
}

// pokebw2's sHandlersPowerAnklet
static const BattleEventHandlerEntry data_ov093_021f15b4[] = {
    { 0x13, HandlerPowerItemCalcSpeed },
};

static const BattleEventHandlerEntry *EventAddPowerAnklet(u32 *priority) {
    *priority = NELEMS(data_ov093_021f15b4);
    return data_ov093_021f15b4;
}

// pokebw2's sHandlersPowerWeight
static const BattleEventHandlerEntry data_ov093_021f15c4[] = {
    { 0x13, HandlerPowerItemCalcSpeed },
};

static const BattleEventHandlerEntry *EventAddPowerWeight(u32 *priority) {
    *priority = NELEMS(data_ov093_021f15c4);
    return data_ov093_021f15c4;
}

static void HandlerPowerItemCalcSpeed(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (BattleEventVar_GetValue(2) == monId) {
        BattleEventVar_MulValue(0x35, FX32_CONST(0.5));
    }
}

// pokebw2's sHandlersFlamePlate
static const BattleEventHandlerEntry data_ov093_021f15cc[] = {
    { 0x38, HandlerCharcoal },
};

static const BattleEventHandlerEntry *EventAddFlamePlate(u32 *priority) {
    *priority = NELEMS(data_ov093_021f15cc);
    return data_ov093_021f15cc;
}

// pokebw2's sHandlersSplashPlate
static const BattleEventHandlerEntry data_ov093_021f147c[] = {
    { 0x38, HandlerMysticWater },
};

static const BattleEventHandlerEntry *EventAddSplashPlate(u32 *priority) {
    *priority = NELEMS(data_ov093_021f147c);
    return data_ov093_021f147c;
}

// pokebw2's sHandlersZapPlate
static const BattleEventHandlerEntry data_ov093_021f136c[] = {
    { 0x38, HandlerMagnet },
};

static const BattleEventHandlerEntry *EventAddZapPlate(u32 *priority) {
    *priority = NELEMS(data_ov093_021f136c);
    return data_ov093_021f136c;
}

// pokebw2's sHandlersMeadowPlate
static const BattleEventHandlerEntry data_ov093_021f15d4[] = {
    { 0x38, HandlerMiracleSeed },
};

static const BattleEventHandlerEntry *EventAddMeadowPlate(u32 *priority) {
    *priority = NELEMS(data_ov093_021f15d4);
    return data_ov093_021f15d4;
}

// pokebw2's sHandlersIciclePlate
static const BattleEventHandlerEntry data_ov093_021f13cc[] = {
    { 0x38, HandlerNeverMeltIce },
};

static const BattleEventHandlerEntry *EventAddIciclePlate(u32 *priority) {
    *priority = NELEMS(data_ov093_021f13cc);
    return data_ov093_021f13cc;
}

// pokebw2's sHandlersFistPlate
static const BattleEventHandlerEntry data_ov093_021f1494[] = {
    { 0x38, HandlerBlackBelt },
};

static const BattleEventHandlerEntry *EventAddFistPlate(u32 *priority) {
    *priority = NELEMS(data_ov093_021f1494);
    return data_ov093_021f1494;
}

// pokebw2's sHandlersToxicPlate
static const BattleEventHandlerEntry data_ov093_021f149c[] = {
    { 0x38, HandlerPoisonBarb },
};

static const BattleEventHandlerEntry *EventAddToxicPlate(u32 *priority) {
    *priority = NELEMS(data_ov093_021f149c);
    return data_ov093_021f149c;
}

// pokebw2's sHandlersEarthPlate
static const BattleEventHandlerEntry data_ov093_021f14e4[] = {
    { 0x38, HandlerSoftSand },
};

static const BattleEventHandlerEntry *EventAddEarthPlate(u32 *priority) {
    *priority = NELEMS(data_ov093_021f14e4);
    return data_ov093_021f14e4;
}

// pokebw2's sHandlersSkyPlate
static const BattleEventHandlerEntry data_ov093_021f1594[] = {
    { 0x38, HandlerSharpBeak },
};

static const BattleEventHandlerEntry *EventAddSkyPlate(u32 *priority) {
    *priority = NELEMS(data_ov093_021f1594);
    return data_ov093_021f1594;
}

// pokebw2's sHandlersMindPlate
static const BattleEventHandlerEntry data_ov093_021f13f4[] = {
    { 0x38, HandlerTwistedSpoon },
};

static const BattleEventHandlerEntry *EventAddMindPlate(u32 *priority) {
    *priority = NELEMS(data_ov093_021f13f4);
    return data_ov093_021f13f4;
}

// pokebw2's sHandlersInsectPlate
static const BattleEventHandlerEntry data_ov093_021f14a4[] = {
    { 0x38, HandlerSilverPowder },
};

static const BattleEventHandlerEntry *EventAddInsectPlate(u32 *priority) {
    *priority = NELEMS(data_ov093_021f14a4);
    return data_ov093_021f14a4;
}

// pokebw2's sHandlersStonePlate
static const BattleEventHandlerEntry data_ov093_021f1574[] = {
    { 0x38, HandlerHardStone },
};

static const BattleEventHandlerEntry *EventAddStonePlate(u32 *priority) {
    *priority = NELEMS(data_ov093_021f1574);
    return data_ov093_021f1574;
}

// pokebw2's sHandlersSpookyPlate
static const BattleEventHandlerEntry data_ov093_021f1374[] = {
    { 0x38, HandlerSpellTag },
};

static const BattleEventHandlerEntry *EventAddSpookyPlate(u32 *priority) {
    *priority = NELEMS(data_ov093_021f1374);
    return data_ov093_021f1374;
}

// pokebw2's sHandlersDracoPlate
static const BattleEventHandlerEntry data_ov093_021f14d4[] = {
    { 0x38, HandlerDragonFang },
};

static const BattleEventHandlerEntry *EventAddDracoPlate(u32 *priority) {
    *priority = NELEMS(data_ov093_021f14d4);
    return data_ov093_021f14d4;
}

// pokebw2's sHandlersDreadPlate
static const BattleEventHandlerEntry data_ov093_021f14ec[] = {
    { 0x38, HandlerBlackGlasses },
};

static const BattleEventHandlerEntry *EventAddDreadPlate(u32 *priority) {
    *priority = NELEMS(data_ov093_021f14ec);
    return data_ov093_021f14ec;
}

// pokebw2's sHandlersIronPlate
static const BattleEventHandlerEntry data_ov093_021f1424[] = {
    { 0x38, HandlerMetalCoat },
};

static const BattleEventHandlerEntry *EventAddIronPlate(u32 *priority) {
    *priority = NELEMS(data_ov093_021f1424);
    return data_ov093_021f1424;
}

// pokebw2's sHandlersBigRoot
static const BattleEventHandlerEntry data_ov093_021f153c[] = {
    { 0x8c, HandlerPlate },
};

static const BattleEventHandlerEntry *func_ov093_021df364(u32 *priority) {
    *priority = NELEMS(data_ov093_021f153c);
    return data_ov093_021f153c;
}

static void HandlerPlate(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (BattleEventVar_GetValue(3) == monId) {
        BattleEventVar_MulValue(0x35, ItemAttackValueToRatio(item));
    }
}

// pokebw2's sHandlersSmokeBall
static const BattleEventHandlerEntry data_ov093_021f15fc[] = {
    { 0xb, HandlerSmokeBall },
    { 0xd, HandlerSmokeBallMessage },
};

static const BattleEventHandlerEntry *EventAddSmokeBall(u32 *priority) {
    *priority = NELEMS(data_ov093_021f15fc);
    return data_ov093_021f15fc;
}

static void HandlerSmokeBall(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonRunCalcSkip(item, flow, monId, work);
}

static void HandlerSmokeBallMessage(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    BattleHandlerMessageParam *param;

    if (CommonCheckRunMessage(item, flow, monId)) {
        param = BattleHandler_PushWork(flow, 4, monId);
        BattleHandler_StrSetup(&param->string, 2, 0x3ec);
        BattleHandler_AddArg(&param->string, monId);
        BattleHandler_AddArg(&param->string, BattleEventItem_GetSubID(item));
        BattleHandler_AddSoundEffect(&param->string, 0x56a);
        BattleHandler_PopWork(flow, param);
    }
}

// pokebw2's sHandlersAmuletCoin
static const BattleEventHandlerEntry data_ov093_021f161c[] = {
    { 0x55, HandlerAmuletCoin },
    { 0x58, func_ov093_021df420 },
};

static const BattleEventHandlerEntry *EventAddAmuletCoin(u32 *priority) {
    *priority = NELEMS(data_ov093_021f161c);
    return data_ov093_021f161c;
}

static void HandlerAmuletCoin(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (BattleEventVar_GetValue(2) == monId) {
        func_ov093_021c8408(flow, monId);
    }
}

static void func_ov093_021df420(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    func_ov093_021c8408(flow, monId);
}

// pokebw2's sHandlersGriseousOrb
static const BattleEventHandlerEntry data_ov093_021f1534[] = {
    { 0x38, HandlerGriseousOrb },
};

static const BattleEventHandlerEntry *EventAddGriseousOrb(u32 *priority) {
    *priority = NELEMS(data_ov093_021f1534);
    return data_ov093_021f1534;
}

static void HandlerGriseousOrb(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    u8 type;

    if (BattleEventVar_GetValue(3) == monId && GetBattleMonSpecies(GetBattleMon(flow, monId)) == SPECIES_GIRATINA) {
        type = BattleEventVar_GetValue(0x16);
        if (type == TYPE_DRAGON || type == TYPE_GHOST) {
            BattleEventVar_MulValue(0x31, FX32_CONST(1.2));
        }
    }
}

// pokebw2's sHandlersIcyRock
static const BattleEventHandlerEntry data_ov093_021f145c[] = {
    { 0x7c, HandlerIcyRock },
};

static const BattleEventHandlerEntry *EventAddIcyRock(u32 *priority) {
    *priority = NELEMS(data_ov093_021f145c);
    return data_ov093_021f145c;
}

static void HandlerIcyRock(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonWeatherMoveIncreaseTurns(item, flow, monId, BTL_WEATHER_HAIL);
}

// pokebw2's sHandlersSmoothRock
static const BattleEventHandlerEntry data_ov093_021f1464[] = {
    { 0x7c, HandlerSmoothRock },
};

static const BattleEventHandlerEntry *EventAddSmoothRock(u32 *priority) {
    *priority = NELEMS(data_ov093_021f1464);
    return data_ov093_021f1464;
}

static void HandlerSmoothRock(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonWeatherMoveIncreaseTurns(item, flow, monId, BTL_WEATHER_SANDSTORM);
}

// pokebw2's sHandlersHeatRock
static const BattleEventHandlerEntry data_ov093_021f151c[] = {
    { 0x7c, HandlerHeatRock },
};

static const BattleEventHandlerEntry *EventAddHeatRock(u32 *priority) {
    *priority = NELEMS(data_ov093_021f151c);
    return data_ov093_021f151c;
}

static void HandlerHeatRock(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonWeatherMoveIncreaseTurns(item, flow, monId, BTL_WEATHER_SUN);
}

// pokebw2's sHandlersDampRock
static const BattleEventHandlerEntry data_ov093_021f1514[] = {
    { 0x7c, HandlerDampRock },
};

static const BattleEventHandlerEntry *EventAddDampRock(u32 *priority) {
    *priority = NELEMS(data_ov093_021f1514);
    return data_ov093_021f1514;
}

static void HandlerDampRock(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonWeatherMoveIncreaseTurns(item, flow, monId, BTL_WEATHER_RAIN);
}

static void CommonWeatherMoveIncreaseTurns(BattleEventItem *item, BtlServerFlow *flow, u8 monId, u8 weather) {
    if (BattleEventVar_GetValue(3) == monId && BattleEventVar_GetValue(0x39) == weather) {
        BattleEventVar_RewriteValue(0x24, CommonGetItemParam(item, 2));
    }
}

// pokebw2's sHandlersLightBall
static const BattleEventHandlerEntry data_ov093_021f167c[] = {
    { 0x3b, HandlerLightBall },
    { 0x73, HandlerLightBallUseTemp },
};

static const BattleEventHandlerEntry *EventAddLightBall(u32 *priority) {
    *priority = NELEMS(data_ov093_021f167c);
    return data_ov093_021f167c;
}

static void HandlerLightBall(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (BattleEventVar_GetValue(3) == monId && GetBattleMonSpecies(GetBattleMon(flow, monId)) == SPECIES_PIKACHU) {
        BattleEventVar_MulValue(0x35, FX32_CONST(2));
    }
}

static void HandlerLightBallUseTemp(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    u8 attackerId;
    BattleHandlerAddConditionParam *param;

    if (BattleEventVar_GetValue(2) == monId) {
        attackerId = BattleEventVar_GetValue(3);
        param = BattleHandler_PushWork(flow, 0xc, attackerId);
        param->targetIndex = monId;
        param->condition = 1;
        param->value = func_ov093_021d7d18(1);
        BattleHandler_PopWork(flow, param);
    }
}

// pokebw2's sHandlersToxicOrb
static const BattleEventHandlerEntry data_ov093_021f168c[] = {
    { 0x77, HandlerToxicOrb },
    { 0x73, HandlerToxicOrbUseTemp },
};

static const BattleEventHandlerEntry *EventAddToxicOrb(u32 *priority) {
    *priority = NELEMS(data_ov093_021f168c);
    return data_ov093_021f168c;
}

static void HandlerToxicOrb(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    BattleHandlerAddConditionParam *param;

    if (BattleEventVar_GetValue(2) == monId) {
        param = BattleHandler_PushWork(flow, 0xc, monId);
        param->condition = 5;
        param->value = func_ov093_021e8a58();
        param->showFail = 0;
        param->targetIndex = monId;
        BattleHandler_StrSetup(&param->string, 2, 0xf0);
        BattleHandler_AddArg(&param->string, monId);
        BattleHandler_AddArg(&param->string, BattleEventItem_GetSubID(item));
        BattleHandler_PopWork(flow, param);
    }
}

static void HandlerToxicOrbUseTemp(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    u8 attackerId;
    BattleHandlerAddConditionParam *param;

    if (BattleEventVar_GetValue(2) == monId) {
        attackerId = BattleEventVar_GetValue(3);
        param = BattleHandler_PushWork(flow, 0xc, attackerId);
        param->targetIndex = monId;
        param->condition = 5;
        param->value = func_ov093_021e8a58();
        BattleHandler_PopWork(flow, param);
    }
}

// pokebw2's sHandlersFlameOrb
static const BattleEventHandlerEntry data_ov093_021f169c[] = {
    { 0x77, HandlerFlameOrb },
    { 0x73, HandlerFlameOrbUseTemp },
};

static const BattleEventHandlerEntry *EventAddFlameOrb(u32 *priority) {
    *priority = NELEMS(data_ov093_021f169c);
    return data_ov093_021f169c;
}

static void HandlerFlameOrb(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    BattleHandlerAddConditionParam *param;

    if (BattleEventVar_GetValue(2) == monId) {
        param = BattleHandler_PushWork(flow, 0xc, monId);
        param->condition = 4;
        param->value = func_ov093_021d7d18(4);
        param->showFail = 0;
        param->targetIndex = monId;
        param->string.args[1] = BattleEventItem_GetSubID(item);
        BattleHandler_StrSetup(&param->string, 2, 0x102);
        BattleHandler_AddArg(&param->string, monId);
        BattleHandler_AddArg(&param->string, BattleEventItem_GetSubID(item));
        BattleHandler_PopWork(flow, param);
    }
}

static void HandlerFlameOrbUseTemp(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    u8 attackerId;
    BattleHandlerAddConditionParam *param;

    if (BattleEventVar_GetValue(2) == monId) {
        attackerId = BattleEventVar_GetValue(3);
        param = BattleHandler_PushWork(flow, 0xc, attackerId);
        param->targetIndex = monId;
        param->condition = 4;
        param->value = func_ov093_021d7d18(4);
        BattleHandler_PopWork(flow, param);
    }
}

// pokebw2's sHandlersSilverPowder
static const BattleEventHandlerEntry data_ov093_021f14f4[] = {
    { 0x38, HandlerSilverPowder },
};

static const BattleEventHandlerEntry *EventAddSilverPowder(u32 *priority) {
    *priority = NELEMS(data_ov093_021f14f4);
    return data_ov093_021f14f4;
}

static void HandlerSilverPowder(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonTypeBoostingItem(item, flow, monId, TYPE_BUG);
}

// pokebw2's sHandlersSoftSand
static const BattleEventHandlerEntry data_ov093_021f13fc[] = {
    { 0x38, HandlerSoftSand },
};

static const BattleEventHandlerEntry *EventAddSoftSand(u32 *priority) {
    *priority = NELEMS(data_ov093_021f13fc);
    return data_ov093_021f13fc;
}

static void HandlerSoftSand(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonTypeBoostingItem(item, flow, monId, TYPE_GROUND);
}

// pokebw2's sHandlersHardStone
static const BattleEventHandlerEntry data_ov093_021f13ac[] = {
    { 0x38, HandlerHardStone },
};

static const BattleEventHandlerEntry *EventAddHardStone(u32 *priority) {
    *priority = NELEMS(data_ov093_021f13ac);
    return data_ov093_021f13ac;
}

static void HandlerHardStone(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonTypeBoostingItem(item, flow, monId, TYPE_ROCK);
}

// pokebw2's sHandlersMiracleSeed
static const BattleEventHandlerEntry data_ov093_021f137c[] = {
    { 0x38, HandlerMiracleSeed },
};

static const BattleEventHandlerEntry *EventAddMiracleSeed(u32 *priority) {
    *priority = NELEMS(data_ov093_021f137c);
    return data_ov093_021f137c;
}

static void HandlerMiracleSeed(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonTypeBoostingItem(item, flow, monId, TYPE_GRASS);
}

// pokebw2's sHandlersBlackGlasses
static const BattleEventHandlerEntry data_ov093_021f13b4[] = {
    { 0x38, HandlerBlackGlasses },
};

static const BattleEventHandlerEntry *EventAddBlackGlasses(u32 *priority) {
    *priority = NELEMS(data_ov093_021f13b4);
    return data_ov093_021f13b4;
}

static void HandlerBlackGlasses(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonTypeBoostingItem(item, flow, monId, TYPE_DARK);
}

// pokebw2's sHandlersBlackBelt
static const BattleEventHandlerEntry data_ov093_021f142c[] = {
    { 0x38, HandlerBlackBelt },
};

static const BattleEventHandlerEntry *EventAddBlackBelt(u32 *priority) {
    *priority = NELEMS(data_ov093_021f142c);
    return data_ov093_021f142c;
}

static void HandlerBlackBelt(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonTypeBoostingItem(item, flow, monId, TYPE_FIGHTING);
}

// pokebw2's sHandlersMagnet
static const BattleEventHandlerEntry data_ov093_021f14c4[] = {
    { 0x38, HandlerMagnet },
};

static const BattleEventHandlerEntry *EventAddMagnet(u32 *priority) {
    *priority = NELEMS(data_ov093_021f14c4);
    return data_ov093_021f14c4;
}

static void HandlerMagnet(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonTypeBoostingItem(item, flow, monId, TYPE_ELECTRIC);
}

// pokebw2's sHandlersMetalCoat
static const BattleEventHandlerEntry data_ov093_021f13c4[] = {
    { 0x38, HandlerMetalCoat },
};

static const BattleEventHandlerEntry *EventAddMetalCoat(u32 *priority) {
    *priority = NELEMS(data_ov093_021f13c4);
    return data_ov093_021f13c4;
}

static void HandlerMetalCoat(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonTypeBoostingItem(item, flow, monId, TYPE_STEEL);
}

// pokebw2's sHandlersMysticWater
static const BattleEventHandlerEntry data_ov093_021f1444[] = {
    { 0x38, HandlerMysticWater },
};

static const BattleEventHandlerEntry *EventAddMysticWater(u32 *priority) {
    *priority = NELEMS(data_ov093_021f1444);
    return data_ov093_021f1444;
}

static void HandlerMysticWater(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonTypeBoostingItem(item, flow, monId, TYPE_WATER);
}

// pokebw2's sHandlersSharpBeak
static const BattleEventHandlerEntry data_ov093_021f144c[] = {
    { 0x38, HandlerSharpBeak },
};

static const BattleEventHandlerEntry *EventAddSharpBeak(u32 *priority) {
    *priority = NELEMS(data_ov093_021f144c);
    return data_ov093_021f144c;
}

static void HandlerSharpBeak(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonTypeBoostingItem(item, flow, monId, TYPE_FLYING);
}

// pokebw2's sHandlersPoisonBarb
static const BattleEventHandlerEntry data_ov093_021f170c[] = {
    { 0x38, HandlerPoisonBarb },
    { 0x73, HandlerPoisonBarbUseTemp },
};

static const BattleEventHandlerEntry *EventAddPoisonBarb(u32 *priority) {
    *priority = NELEMS(data_ov093_021f170c);
    return data_ov093_021f170c;
}

static void HandlerPoisonBarb(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonTypeBoostingItem(item, flow, monId, TYPE_POISON);
}

static void HandlerPoisonBarbUseTemp(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    u8 attackerId;
    BattleHandlerAddConditionParam *param;

    if (BattleEventVar_GetValue(2) == monId) {
        attackerId = BattleEventVar_GetValue(3);
        param = BattleHandler_PushWork(flow, 0xc, attackerId);
        param->targetIndex = monId;
        param->condition = 5;
        param->value = func_ov093_021d7d18(5);
        BattleHandler_PopWork(flow, param);
    }
}

// pokebw2's sHandlersNeverMeltIce
static const BattleEventHandlerEntry data_ov093_021f15ac[] = {
    { 0x38, HandlerNeverMeltIce },
};

static const BattleEventHandlerEntry *EventAddNeverMeltIce(u32 *priority) {
    *priority = NELEMS(data_ov093_021f15ac);
    return data_ov093_021f15ac;
}

static void HandlerNeverMeltIce(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonTypeBoostingItem(item, flow, monId, TYPE_ICE);
}

// pokebw2's sHandlersSpellTag
static const BattleEventHandlerEntry data_ov093_021f15bc[] = {
    { 0x38, HandlerSpellTag },
};

static const BattleEventHandlerEntry *EventAddSpellTag(u32 *priority) {
    *priority = NELEMS(data_ov093_021f15bc);
    return data_ov093_021f15bc;
}

static void HandlerSpellTag(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonTypeBoostingItem(item, flow, monId, TYPE_GHOST);
}

// pokebw2's sHandlersTwistedSpoon
static const BattleEventHandlerEntry data_ov093_021f1484[] = {
    { 0x38, HandlerTwistedSpoon },
};

static const BattleEventHandlerEntry *EventAddTwistedSpoon(u32 *priority) {
    *priority = NELEMS(data_ov093_021f1484);
    return data_ov093_021f1484;
}

static void HandlerTwistedSpoon(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonTypeBoostingItem(item, flow, monId, TYPE_PSYCHIC);
}

// pokebw2's sHandlersCharcoal
static const BattleEventHandlerEntry data_ov093_021f15e4[] = {
    { 0x38, HandlerCharcoal },
};

static const BattleEventHandlerEntry *EventAddCharcoal(u32 *priority) {
    *priority = NELEMS(data_ov093_021f15e4);
    return data_ov093_021f15e4;
}

static void HandlerCharcoal(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonTypeBoostingItem(item, flow, monId, TYPE_FIRE);
}

// pokebw2's sHandlersDragonFang
static const BattleEventHandlerEntry data_ov093_021f15dc[] = {
    { 0x38, HandlerDragonFang },
};

static const BattleEventHandlerEntry *EventAddDragonFang(u32 *priority) {
    *priority = NELEMS(data_ov093_021f15dc);
    return data_ov093_021f15dc;
}

static void HandlerDragonFang(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonTypeBoostingItem(item, flow, monId, TYPE_DRAGON);
}

// pokebw2's sHandlersSilkScarf
static const BattleEventHandlerEntry data_ov093_021f159c[] = {
    { 0x38, HandlerSilkScarf },
};

static const BattleEventHandlerEntry *EventAddSilkScarf(u32 *priority) {
    *priority = NELEMS(data_ov093_021f159c);
    return data_ov093_021f159c;
}

static void HandlerSilkScarf(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonTypeBoostingItem(item, flow, monId, TYPE_NORMAL);
}

// pokebw2's sHandlersOddIncense
static const BattleEventHandlerEntry data_ov093_021f155c[] = {
    { 0x38, HandlerOddIncense },
};

static const BattleEventHandlerEntry *EventAddOddIncense(u32 *priority) {
    *priority = NELEMS(data_ov093_021f155c);
    return data_ov093_021f155c;
}

static void HandlerOddIncense(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonTypeBoostingItem(item, flow, monId, TYPE_PSYCHIC);
}

// pokebw2's sHandlersRockIncense
static const BattleEventHandlerEntry data_ov093_021f1404[] = {
    { 0x38, HandlerRockIncense },
};

static const BattleEventHandlerEntry *EventAddRockIncense(u32 *priority) {
    *priority = NELEMS(data_ov093_021f1404);
    return data_ov093_021f1404;
}

static void HandlerRockIncense(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonTypeBoostingItem(item, flow, monId, TYPE_ROCK);
}

// pokebw2's sHandlersWaveIncense
static const BattleEventHandlerEntry data_ov093_021f1544[] = {
    { 0x38, HandlerWaveIncense },
};

static const BattleEventHandlerEntry *EventAddWaveIncense(u32 *priority) {
    *priority = NELEMS(data_ov093_021f1544);
    return data_ov093_021f1544;
}

static void HandlerWaveIncense(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonTypeBoostingItem(item, flow, monId, TYPE_WATER);
}

// pokebw2's sHandlersSeaIncense
static const BattleEventHandlerEntry data_ov093_021f14dc[] = {
    { 0x38, HandlerSeaIncense },
};

static const BattleEventHandlerEntry *EventAddSeaIncense(u32 *priority) {
    *priority = NELEMS(data_ov093_021f14dc);
    return data_ov093_021f14dc;
}

static void HandlerSeaIncense(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonTypeBoostingItem(item, flow, monId, TYPE_WATER);
}

// pokebw2's sHandlersRoseIncense
static const BattleEventHandlerEntry data_ov093_021f1414[] = {
    { 0x38, HandlerRoseIncense },
};

static const BattleEventHandlerEntry *EventAddRoseIncense(u32 *priority) {
    *priority = NELEMS(data_ov093_021f1414);
    return data_ov093_021f1414;
}

static void HandlerRoseIncense(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonTypeBoostingItem(item, flow, monId, TYPE_GRASS);
}

static void CommonTypeBoostingItem(BattleEventItem *item, BtlServerFlow *flow, u8 monId, u8 type) {
    if (BattleEventVar_GetValue(3) == monId && BattleEventVar_GetValue(0x16) == type) {
        BattleEventVar_MulValue(0x31, ItemAttackValueToRatio(item));
    }
}

// pokebw2's sHandlersLustrousOrb
static const BattleEventHandlerEntry data_ov093_021f141c[] = {
    { 0x38, HandlerLustrousOrb },
};

static const BattleEventHandlerEntry *EventAddLustrousOrb(u32 *priority) {
    *priority = NELEMS(data_ov093_021f141c);
    return data_ov093_021f141c;
}

static void HandlerLustrousOrb(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    u8 type;

    if (BattleEventVar_GetValue(3) == monId && GetBattleMonSpecies(GetBattleMon(flow, monId)) == SPECIES_PALKIA) {
        type = BattleEventVar_GetValue(0x16);
        if (type == TYPE_DRAGON || type == TYPE_WATER) {
            BattleEventVar_MulValue(0x31, ItemAttackValueToRatio(item));
        }
    }
}

// pokebw2's sHandlersAdamantOrb
static const BattleEventHandlerEntry data_ov093_021f157c[] = {
    { 0x38, HandlerAdamantOrb },
};

static const BattleEventHandlerEntry *EventAddAdamantOrb(u32 *priority) {
    *priority = NELEMS(data_ov093_021f157c);
    return data_ov093_021f157c;
}

static void HandlerAdamantOrb(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    u8 type;

    if (BattleEventVar_GetValue(3) == monId && GetBattleMonSpecies(GetBattleMon(flow, monId)) == SPECIES_DIALGA) {
        type = BattleEventVar_GetValue(0x16);
        if (type == TYPE_DRAGON || type == TYPE_STEEL) {
            BattleEventVar_MulValue(0x31, ItemAttackValueToRatio(item));
        }
    }
}

// pokebw2's sHandlersIronBall
static const BattleEventHandlerEntry data_ov093_021f166c[] = {
    { 0x13, HandlerIronBallCalcSpeed },
    { 0x12, HandlerIronBallCheckFly },
};

static const BattleEventHandlerEntry *EventAddIronBall(u32 *priority) {
    *priority = NELEMS(data_ov093_021f166c);
    return data_ov093_021f166c;
}

static void HandlerIronBallCalcSpeed(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (BattleEventVar_GetValue(2) == monId) {
        BattleEventVar_MulValue(0x35, FX32_CONST(0.5));
    }
}

static void HandlerIronBallCheckFly(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (BattleEventVar_GetValue(2) == monId) {
        BattleEventVar_RewriteValue(0x41, 1);
    }
}

// pokebw2's sHandlersFloatStone
static const BattleEventHandlerEntry data_ov093_021f1504[] = {
    { 0x7b, HandlerFloatStone },
};

static const BattleEventHandlerEntry *EventAddFloatStone(u32 *priority) {
    *priority = NELEMS(data_ov093_021f1504);
    return data_ov093_021f1504;
}

static void HandlerFloatStone(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (BattleEventVar_GetValue(2) == monId) {
        BattleEventVar_MulValue(0x35, FX32_CONST(0.5));
    }
}

// pokebw2's sHandlersEviolite
static const BattleEventHandlerEntry data_ov093_021f14fc[] = {
    { 0x3c, HandlerEviolite },
};

const BattleEventHandlerEntry *EventAddEviolite(u32 *priority) {
    *priority = NELEMS(data_ov093_021f14fc);
    return data_ov093_021f14fc;
}

static void HandlerEviolite(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (BattleEventVar_GetValue(4) == monId && CheckEvolution(flow, monId)) {
        BattleEventVar_MulValue(0x35, FX32_CONST(1.5));
    }
}

// pokebw2's sHandlersRockyHelmet
static const BattleEventHandlerEntry data_ov093_021f140c[] = {
    { 0x4b, HandlerRockyHelmet },
};

static const BattleEventHandlerEntry *EventAddRockyHelmet(u32 *priority) {
    *priority = NELEMS(data_ov093_021f140c);
    return data_ov093_021f140c;
}

static void HandlerRockyHelmet(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    BattleHandlerDamageParam *param;
    BattleMon *attacker;
    u8 divisor;

    if (BattleEventVar_GetValue(4) == monId && BattleEventVar_GetValue(0x46) == 0 &&
        getMoveFlag(BattleEventVar_GetValue(0x12), 0)) {
        param = BattleHandler_PushWork(flow, 7, monId);
        param->targetIndex = BattleEventVar_GetValue(3);
        attacker = GetBattleMon(flow, param->targetIndex);
        divisor = CommonGetItemParam(item, 2);
        param->amount = DivideMaxHPZeroCheck(attacker, divisor);
        BattleHandler_StrSetup(&param->string, 2, 0x1a8);
        BattleHandler_AddArg(&param->string, param->targetIndex);
        BattleHandler_PopWork(flow, param);
    }
}

// pokebw2's sHandlersAirBalloon
static const BattleEventHandlerEntry data_ov093_021f1934[] = {
    { 0x55, HandlerAirBalloonMemberIn },
    { 0x12, HandlerAirBalloonCheckFlying },
    { 0x4b, HandlerAirBalloonDamageReaction },
};

static const BattleEventHandlerEntry *EventAddAirBalloon(u32 *priority) {
    *priority = NELEMS(data_ov093_021f1934);
    return data_ov093_021f1934;
}

static void HandlerAirBalloonMemberIn(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    BattleHandlerMessageParam *param;

    if (BattleEventVar_GetValue(2) == monId && !func_ov093_021ef948(FIELD_CONDITION_GRAVITY)) {
        param = BattleHandler_PushWork(flow, 4, monId);
        BattleHandler_StrSetup(&param->string, 2, 0x198);
        BattleHandler_AddArg(&param->string, monId);
        BattleHandler_PopWork(flow, param);
    }
}

static void HandlerAirBalloonCheckFlying(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (BattleEventVar_GetValue(2) == monId) {
        BattleEventVar_RewriteValue(0x51, 1);
    }
}

static void HandlerAirBalloonDamageReaction(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    BattleHandlerSetItemParam *param;

    if (BattleEventVar_GetValue(4) == monId) {
        param = BattleHandler_PushWork(flow, 0x20, monId);
        param->targetIndex = monId;
        param->item = ITEM_NONE;
        BattleHandler_StrSetup(&param->string, 2, 0x19b);
        BattleHandler_AddArg(&param->string, monId);
        BattleHandler_PopWork(flow, param);
    }
}

// pokebw2's sHandlersRedCard
static const BattleEventHandlerEntry data_ov093_021f14cc[] = {
    { 0x83, HandlerRedCard },
};

static const BattleEventHandlerEntry *EventAddRedCard(u32 *priority) {
    *priority = NELEMS(data_ov093_021f14cc);
    return data_ov093_021f14cc;
}

static void HandlerRedCard(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    u8 attackerId;
    BattleHandlerConsumeItemParam *consumeParam;
    BattleHandlerForceSwitchParam *switchParam;

    if (func_ov093_021e85f8(monId)) {
        attackerId = BattleEventVar_GetValue(3);
        if (func_ov093_021c7d00(flow, attackerId) != 6 && BattleEventVar_GetValue(0x4d) == 0 &&
            func_ov093_021c7f24(flow, attackerId) && !func_ov093_021c83d4(flow)) {
            consumeParam = BattleHandler_PushWork(flow, 0x23, monId);
            BattleHandler_StrSetup(&consumeParam->string, 2, 0x1a1);
            BattleHandler_AddArg(&consumeParam->string, monId);
            BattleHandler_AddArg(&consumeParam->string, attackerId);
            BattleHandler_PopWork(flow, consumeParam);
            switchParam = BattleHandler_PushWork(flow, 0x2e, monId);
            switchParam->targetIndex = BattleEventVar_GetValue(3);
            switchParam->ignoreLevel = 1;
            switchParam->effect = 0x28c;
            BattleHandler_PopWork(flow, switchParam);
        }
    }
}

// pokebw2's sHandlersRingTarget
static const BattleEventHandlerEntry data_ov093_021f14bc[] = {
    { 0x3e, HandlerRingTarget },
};

static const BattleEventHandlerEntry *EventAddRingTarget(u32 *priority) {
    *priority = NELEMS(data_ov093_021f14bc);
    return data_ov093_021f14bc;
}

static void HandlerRingTarget(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (!func_ov093_021c7ec4(flow) && BattleEventVar_GetValue(4) == monId) {
        BattleEventVar_RewriteValue(0x4b, 1);
    }
}

// pokebw2's sHandlersBindingBand
static const BattleEventHandlerEntry data_ov093_021f1524[] = {
    { 0x62, HandlerBindingBand },
};

static const BattleEventHandlerEntry *EventAddBindingBand(u32 *priority) {
    *priority = NELEMS(data_ov093_021f1524);
    return data_ov093_021f1524;
}

static void HandlerBindingBand(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    BattleCondition condition;

    if (BattleEventVar_GetValue(3) == monId) {
        condition.raw = BattleEventVar_GetValue(0x1e);
        SetConditionFlag(&condition, 1);
        BattleEventVar_RewriteValue(0x1e, condition.raw);
    }
}

// pokebw2's sHandlersAbsorbBulb
static const BattleEventHandlerEntry data_ov093_021f173c[] = {
    { 0x4b, HandlerAbsorbBulbDamageReaction },
    { 0x72, HandlerAbsorbBulbUse },
};

static const BattleEventHandlerEntry *EventAddAbsorbBulb(u32 *priority) {
    *priority = NELEMS(data_ov093_021f173c);
    return data_ov093_021f173c;
}

static void HandlerAbsorbBulbDamageReaction(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (BattleEventVar_GetValue(4) == monId && BattleEventVar_GetValue(0x46) == 0 &&
        BattleEventVar_GetValue(0x16) == TYPE_WATER &&
        IsStatChangeValid(GetBattleMon(flow, monId), BATTLEMON_SP_ATTACK_STAGE, 1)) {
        ItemEvent_PushRun(item, flow, monId);
    }
}

static void HandlerAbsorbBulbUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    BattleHandlerStatChangeParam *param;

    if (BattleEventVar_GetValue(2) == monId) {
        param = BattleHandler_PushWork(flow, 0xe, monId);
        param->count = 1;
        param->monIds[0] = monId;
        param->stat = BATTLEMON_SP_ATTACK_STAGE;
        param->change = 1;
        BattleHandler_PopWork(flow, param);
    }
}

// pokebw2's sHandlersCellBattery
static const BattleEventHandlerEntry data_ov093_021f174c[] = {
    { 0x4b, HandlerCellBatteryDamageReaction },
    { 0x72, HandlerCellBatteryUse },
};

static const BattleEventHandlerEntry *EventAddCellBattery(u32 *priority) {
    *priority = NELEMS(data_ov093_021f174c);
    return data_ov093_021f174c;
}

static void HandlerCellBatteryDamageReaction(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (BattleEventVar_GetValue(4) == monId && BattleEventVar_GetValue(0x46) == 0 &&
        BattleEventVar_GetValue(0x16) == TYPE_ELECTRIC &&
        IsStatChangeValid(GetBattleMon(flow, monId), BATTLEMON_ATTACK_STAGE, 1)) {
        ItemEvent_PushRun(item, flow, monId);
    }
}

static void HandlerCellBatteryUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    BattleHandlerStatChangeParam *param;

    if (BattleEventVar_GetValue(2) == monId) {
        param = BattleHandler_PushWork(flow, 0xe, monId);
        param->count = 1;
        param->monIds[0] = monId;
        param->stat = BATTLEMON_ATTACK_STAGE;
        param->change = 1;
        BattleHandler_PopWork(flow, param);
    }
}

// pokebw2's sHandlersEjectButton
static const BattleEventHandlerEntry data_ov093_021f176c[] = {
    { 0x83, HandlerEjectButtonReaction },
    { 0x72, HandlerEjectButtonUse },
};

static const BattleEventHandlerEntry *EventAddEjectButton(u32 *priority) {
    *priority = NELEMS(data_ov093_021f176c);
    return data_ov093_021f176c;
}

static void HandlerEjectButtonReaction(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (func_ov093_021e85f8(monId) && BattleEventVar_GetValue(0x4d) == 0 && !func_ov093_021c8374(flow, monId) &&
        !func_ov093_021c83d4(flow) && !IsMonSwitchingOut(flow) && func_ov093_021c7f24(flow, monId) &&
        func_ov093_021c7f4c(flow)) {
        ItemEvent_PushRun(item, flow, monId);
    }
}

static void HandlerEjectButtonUse(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    BattleHandlerMessageParam *messageParam;
    BattleHandlerSwitchParam *switchParam;

    if (BattleEventVar_GetValue(2) == monId && func_ov093_021c7f24(flow, monId)) {
        messageParam = BattleHandler_PushWork(flow, 4, monId);
        BattleHandler_StrSetup(&messageParam->string, 2, 0x19e);
        BattleHandler_AddArg(&messageParam->string, monId);
        BattleHandler_PopWork(flow, messageParam);
        switchParam = BattleHandler_PushWork(flow, 0x29, monId);
        switchParam->monIndex = monId;
        BattleHandler_PopWork(flow, switchParam);
    }
}

// pokebw2's sHandlersFireGem
static const BattleEventHandlerEntry data_ov093_021f1a3c[] = {
    { 0x81, HandlerFireGemDecide },
    { 0x38, HandlerFireGemPower },
    { 0x88, HandlerGemEnd },
};

static const BattleEventHandlerEntry *EventAddFireGem(u32 *priority) {
    *priority = NELEMS(data_ov093_021f1a3c);
    return data_ov093_021f1a3c;
}

static void HandlerFireGemPower(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonGemPower(item, flow, monId, work, TYPE_FIRE);
}

static void HandlerFireGemDecide(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonGemDecide(item, flow, monId, work, TYPE_FIRE);
}

// pokebw2's sHandlersWaterGem
static const BattleEventHandlerEntry data_ov093_021f1a54[] = {
    { 0x81, HandlerWaterGemDecide },
    { 0x38, HandlerWaterGemPower },
    { 0x88, HandlerGemEnd },
};

static const BattleEventHandlerEntry *EventAddWaterGem(u32 *priority) {
    *priority = NELEMS(data_ov093_021f1a54);
    return data_ov093_021f1a54;
}

static void HandlerWaterGemPower(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonGemPower(item, flow, monId, work, TYPE_WATER);
}

static void HandlerWaterGemDecide(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonGemDecide(item, flow, monId, work, TYPE_WATER);
}

// pokebw2's sHandlersElectricGem
static const BattleEventHandlerEntry data_ov093_021f1874[] = {
    { 0x81, HandlerElectricGemDecide },
    { 0x38, HandlerElectricGemPower },
    { 0x88, HandlerGemEnd },
};

static const BattleEventHandlerEntry *EventAddElectricGem(u32 *priority) {
    *priority = NELEMS(data_ov093_021f1874);
    return data_ov093_021f1874;
}

static void HandlerElectricGemPower(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonGemPower(item, flow, monId, work, TYPE_ELECTRIC);
}

static void HandlerElectricGemDecide(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonGemDecide(item, flow, monId, work, TYPE_ELECTRIC);
}

// pokebw2's sHandlersGrassGem
static const BattleEventHandlerEntry data_ov093_021f18a4[] = {
    { 0x81, HandlerGrassGemDecide },
    { 0x38, HandlerGrassGemPower },
    { 0x88, HandlerGemEnd },
};

static const BattleEventHandlerEntry *EventAddGrassGem(u32 *priority) {
    *priority = NELEMS(data_ov093_021f18a4);
    return data_ov093_021f18a4;
}

static void HandlerGrassGemPower(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonGemPower(item, flow, monId, work, TYPE_GRASS);
}

static void HandlerGrassGemDecide(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonGemDecide(item, flow, monId, work, TYPE_GRASS);
}

// pokebw2's sHandlersIceGem
static const BattleEventHandlerEntry data_ov093_021f18bc[] = {
    { 0x81, HandlerIceGemDecide },
    { 0x38, HandlerIceGemPower },
    { 0x88, HandlerGemEnd },
};

static const BattleEventHandlerEntry *EventAddIceGem(u32 *priority) {
    *priority = NELEMS(data_ov093_021f18bc);
    return data_ov093_021f18bc;
}

static void HandlerIceGemPower(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonGemPower(item, flow, monId, work, TYPE_ICE);
}

static void HandlerIceGemDecide(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonGemDecide(item, flow, monId, work, TYPE_ICE);
}

// pokebw2's sHandlersFightingGem
static const BattleEventHandlerEntry data_ov093_021f194c[] = {
    { 0x81, HandlerFightingGemDecide },
    { 0x38, HandlerFightingGemPower },
    { 0x88, HandlerGemEnd },
};

static const BattleEventHandlerEntry *EventAddFightingGem(u32 *priority) {
    *priority = NELEMS(data_ov093_021f194c);
    return data_ov093_021f194c;
}

static void HandlerFightingGemPower(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonGemPower(item, flow, monId, work, TYPE_FIGHTING);
}

static void HandlerFightingGemDecide(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonGemDecide(item, flow, monId, work, TYPE_FIGHTING);
}

// pokebw2's sHandlersPoisonGem
static const BattleEventHandlerEntry data_ov093_021f1994[] = {
    { 0x81, HandlerPoisonGemDecide },
    { 0x38, HandlerPoisonGemPower },
    { 0x88, HandlerGemEnd },
};

static const BattleEventHandlerEntry *EventAddPoisonGem(u32 *priority) {
    *priority = NELEMS(data_ov093_021f1994);
    return data_ov093_021f1994;
}

static void HandlerPoisonGemPower(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonGemPower(item, flow, monId, work, TYPE_POISON);
}

static void HandlerPoisonGemDecide(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonGemDecide(item, flow, monId, work, TYPE_POISON);
}

// pokebw2's sHandlersGroundGem
static const BattleEventHandlerEntry data_ov093_021f19ac[] = {
    { 0x81, HandlerGroundGemDecide },
    { 0x38, HandlerGroundGemPower },
    { 0x88, HandlerGemEnd },
};

static const BattleEventHandlerEntry *EventAddGroundGem(u32 *priority) {
    *priority = NELEMS(data_ov093_021f19ac);
    return data_ov093_021f19ac;
}

static void HandlerGroundGemPower(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonGemPower(item, flow, monId, work, TYPE_GROUND);
}

static void HandlerGroundGemDecide(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonGemDecide(item, flow, monId, work, TYPE_GROUND);
}

// pokebw2's sHandlersFlyingGem
static const BattleEventHandlerEntry data_ov093_021f1a24[] = {
    { 0x81, HandlerFlyingGemDecide },
    { 0x38, HandlerFlyingGemPower },
    { 0x88, HandlerGemEnd },
};

static const BattleEventHandlerEntry *EventAddFlyingGem(u32 *priority) {
    *priority = NELEMS(data_ov093_021f1a24);
    return data_ov093_021f1a24;
}

static void HandlerFlyingGemPower(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonGemPower(item, flow, monId, work, TYPE_FLYING);
}

static void HandlerFlyingGemDecide(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonGemDecide(item, flow, monId, work, TYPE_FLYING);
}

// pokebw2's sHandlersPsychicGem
static const BattleEventHandlerEntry data_ov093_021f1a6c[] = {
    { 0x81, HandlerPsychicGemDecide },
    { 0x38, HandlerPsychicGemPower },
    { 0x88, HandlerGemEnd },
};

static const BattleEventHandlerEntry *EventAddPsychicGem(u32 *priority) {
    *priority = NELEMS(data_ov093_021f1a6c);
    return data_ov093_021f1a6c;
}

static void HandlerPsychicGemPower(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonGemPower(item, flow, monId, work, TYPE_PSYCHIC);
}

static void HandlerPsychicGemDecide(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonGemDecide(item, flow, monId, work, TYPE_PSYCHIC);
}

// pokebw2's sHandlersBugGem
static const BattleEventHandlerEntry data_ov093_021f185c[] = {
    { 0x81, HandlerBugGemDecide },
    { 0x38, HandlerBugGemPower },
    { 0x88, HandlerGemEnd },
};

static const BattleEventHandlerEntry *EventAddBugGem(u32 *priority) {
    *priority = NELEMS(data_ov093_021f185c);
    return data_ov093_021f185c;
}

static void HandlerBugGemPower(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonGemPower(item, flow, monId, work, TYPE_BUG);
}

static void HandlerBugGemDecide(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonGemDecide(item, flow, monId, work, TYPE_BUG);
}

// pokebw2's sHandlersRockGem
static const BattleEventHandlerEntry data_ov093_021f1964[] = {
    { 0x81, HandlerRockGemDecide },
    { 0x38, HandlerRockGemPower },
    { 0x88, HandlerGemEnd },
};

static const BattleEventHandlerEntry *EventAddRockGem(u32 *priority) {
    *priority = NELEMS(data_ov093_021f1964);
    return data_ov093_021f1964;
}

static void HandlerRockGemPower(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonGemPower(item, flow, monId, work, TYPE_ROCK);
}

static void HandlerRockGemDecide(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonGemDecide(item, flow, monId, work, TYPE_ROCK);
}

// pokebw2's sHandlersGhostGem
static const BattleEventHandlerEntry data_ov093_021f19dc[] = {
    { 0x81, HandlerGhostGemDecide },
    { 0x38, HandlerGhostGemPower },
    { 0x88, HandlerGemEnd },
};

static const BattleEventHandlerEntry *EventAddGhostGem(u32 *priority) {
    *priority = NELEMS(data_ov093_021f19dc);
    return data_ov093_021f19dc;
}

static void HandlerGhostGemPower(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonGemPower(item, flow, monId, work, TYPE_GHOST);
}

static void HandlerGhostGemDecide(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonGemDecide(item, flow, monId, work, TYPE_GHOST);
}

// pokebw2's sHandlersDragonGem
static const BattleEventHandlerEntry data_ov093_021f1a0c[] = {
    { 0x81, HandlerDragonGemDecide },
    { 0x38, HandlerDragonGemPower },
    { 0x88, HandlerGemEnd },
};

static const BattleEventHandlerEntry *EventAddDragonGem(u32 *priority) {
    *priority = NELEMS(data_ov093_021f1a0c);
    return data_ov093_021f1a0c;
}

static void HandlerDragonGemPower(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonGemPower(item, flow, monId, work, TYPE_DRAGON);
}

static void HandlerDragonGemDecide(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonGemDecide(item, flow, monId, work, TYPE_DRAGON);
}

// pokebw2's sHandlersDarkGem
static const BattleEventHandlerEntry data_ov093_021f188c[] = {
    { 0x81, HandlerDarkGemDecide },
    { 0x38, HandlerDarkGemPower },
    { 0x88, HandlerGemEnd },
};

static const BattleEventHandlerEntry *EventAddDarkGem(u32 *priority) {
    *priority = NELEMS(data_ov093_021f188c);
    return data_ov093_021f188c;
}

static void HandlerDarkGemPower(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonGemPower(item, flow, monId, work, TYPE_DARK);
}

static void HandlerDarkGemDecide(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonGemDecide(item, flow, monId, work, TYPE_DARK);
}

// pokebw2's sHandlersSteelGem
static const BattleEventHandlerEntry data_ov093_021f19f4[] = {
    { 0x81, HandlerSteelGemDecide },
    { 0x38, HandlerSteelGemPower },
    { 0x88, HandlerGemEnd },
};

static const BattleEventHandlerEntry *EventAddSteelGem(u32 *priority) {
    *priority = NELEMS(data_ov093_021f19f4);
    return data_ov093_021f19f4;
}

static void HandlerSteelGemPower(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonGemPower(item, flow, monId, work, TYPE_STEEL);
}

static void HandlerSteelGemDecide(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonGemDecide(item, flow, monId, work, TYPE_STEEL);
}

// pokebw2's sHandlersNormalGem
static const BattleEventHandlerEntry data_ov093_021f19c4[] = {
    { 0x81, HandlerNormalGemPower },
    { 0x38, HandlerNormalGemDecide },
    { 0x88, HandlerGemEnd },
};

static const BattleEventHandlerEntry *EventAddNormalGem(u32 *priority) {
    *priority = NELEMS(data_ov093_021f19c4);
    return data_ov093_021f19c4;
}

// swan's names for Normal Gem's handlers are swapped: this one decides, the next one powers up
static void HandlerNormalGemPower(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonGemDecide(item, flow, monId, work, TYPE_NORMAL);
}

static void HandlerNormalGemDecide(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    CommonGemPower(item, flow, monId, work, TYPE_NORMAL);
}

static void CommonGemDecide(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work, u8 type) {
    u16 move;
    BattleHandlerConsumeItemParam *param;

    if (BattleEventVar_GetValue(3) == monId && BattleEventVar_GetValue(0x16) == type) {
        move = BattleEventVar_GetValue(0x12);
        if (PML_MoveIsDamaging(move) && !data_ov095_0689c8ed(move) && !BattleEventItem_IsIsolated(item) &&
            !func_ov093_021c7ec4(flow)) {
            BattleEventItem_ConvertToIsolated(item);
            param = BattleHandler_PushWork(flow, 0x23, monId);
            BattleHandler_StrSetup(&param->string, 1, 0xb6);
            BattleHandler_AddArg(&param->string, BattleEventItem_GetSubID(item));
            BattleHandler_AddArg(&param->string, BattleEventVar_GetValue(0x12));
            BattleHandler_PopWork(flow, param);
            *work = 1;
        }
    }
}

static void CommonGemPower(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work, u8 type) {
    if (BattleEventVar_GetValue(3) == monId && BattleEventVar_GetValue(0x16) == type) {
        if (*work == 1 || func_ov093_021c7ec4(flow)) {
            BattleEventVar_MulValue(0x31, FX32_CONST(1.5));
        }
    }
}

static void HandlerGemEnd(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (BattleEventVar_GetValue(3) == monId && *work == 1) {
        *work = 2;
    }
}
