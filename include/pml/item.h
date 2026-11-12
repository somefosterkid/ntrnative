// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/pml/item.h, the lines in use so far.
#ifndef BWPORT_PML_ITEM_H
#define BWPORT_PML_ITEM_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0), except ItemData

// Reads one of an item's files into a new allocation: 0 is its data
void *PML_ItemReadDataFile(u16 item, u32 type, HeapID heapId);
s32 PML_ItemGetParam(ItemData *data, u32 param);
// The move a TM or HM teaches, or 0
u16 PML_ItemGetTMWazaID(u16 item);

// An ITEM_PARAM_* of an item, read from its file
s32 GetItemParam(u16 item, u16 param, HeapID heapId);
BOOL PML_ItemIsTMHM(u16 item);
BOOL PML_MoveIsHM(u16 move);
BOOL PML_ItemIsMail(u16 item);
BOOL PML_ItemIsBerry(u16 item);

// Black 2's; twinned code confirms the values of ITEM_PARAM_PRICE, ITEM_PARAM_FIELD_POCKET, ITEM_PARAM_FIELD_FUNC,
// ITEM_PARAM_FLING_POWER, ITEM_PARAM_KIND, ITEM_PARAM_PP_RESTORE and ITEM_PARAM_PP_RESTORE_ALL in Black
// PML_ItemGetParam's parameters: the item's fields, then its work's
enum {
    ITEM_PARAM_PRICE,
    ITEM_PARAM_HOLD_EFFECT,
    ITEM_PARAM_HOLD_PARAM,
    ITEM_PARAM_IMPORTANT,
    ITEM_PARAM_REGISTRABLE,
    ITEM_PARAM_FIELD_POCKET,
    ITEM_PARAM_FIELD_FUNC,
    ITEM_PARAM_BATTLE_FUNC,
    ITEM_PARAM_PLUCK_EFFECT,
    ITEM_PARAM_FLING_EFFECT,
    ITEM_PARAM_FLING_POWER,
    ITEM_PARAM_NATURAL_GIFT_POWER,
    ITEM_PARAM_NATURAL_GIFT_TYPE,
    ITEM_PARAM_BATTLE_POCKET,
    ITEM_PARAM_WORK_TYPE,
    ITEM_PARAM_KIND,
    ITEM_PARAM_UNK_E,
    ITEM_PARAM_SORT_INDEX,
    ITEM_PARAM_SLEEP_HEAL,
    ITEM_PARAM_POISON_HEAL,
    ITEM_PARAM_BURN_HEAL,
    ITEM_PARAM_FREEZE_HEAL,
    ITEM_PARAM_PARALYSIS_HEAL,
    ITEM_PARAM_CONFUSION_HEAL,
    ITEM_PARAM_INFATUATION_HEAL,
    ITEM_PARAM_GUARD_SPEC,
    ITEM_PARAM_REVIVE,
    ITEM_PARAM_REVIVE_ALL,
    ITEM_PARAM_LEVEL_UP,
    ITEM_PARAM_EVOLVE,
    ITEM_PARAM_ATTACK_STAGES,
    ITEM_PARAM_DEFENSE_STAGES,
    ITEM_PARAM_SP_ATTACK_STAGES,
    ITEM_PARAM_SP_DEFENSE_STAGES,
    ITEM_PARAM_SPEED_STAGES,
    ITEM_PARAM_ACCURACY_STAGES,
    ITEM_PARAM_CRIT_STAGES,
    ITEM_PARAM_PP_UP,
    ITEM_PARAM_PP_MAX,
    ITEM_PARAM_PP_RESTORE,
    ITEM_PARAM_PP_RESTORE_ALL,
    ITEM_PARAM_HP_RESTORE,
    ITEM_PARAM_HP_EV_UP,
    ITEM_PARAM_ATTACK_EV_UP,
    ITEM_PARAM_DEFENSE_EV_UP,
    ITEM_PARAM_SPEED_EV_UP,
    ITEM_PARAM_SP_ATTACK_EV_UP,
    ITEM_PARAM_SP_DEFENSE_EV_UP,
    ITEM_PARAM_FRIENDSHIP_UP_1,
    ITEM_PARAM_FRIENDSHIP_UP_2,
    ITEM_PARAM_FRIENDSHIP_UP_3,
    ITEM_PARAM_UNK_51,
    ITEM_PARAM_HP_EV,
    ITEM_PARAM_ATTACK_EV,
    ITEM_PARAM_DEFENSE_EV,
    ITEM_PARAM_SPEED_EV,
    ITEM_PARAM_SP_ATTACK_EV,
    ITEM_PARAM_SP_DEFENSE_EV,
    ITEM_PARAM_HP_RESTORE_AMOUNT,
    ITEM_PARAM_PP_RESTORE_AMOUNT,
    ITEM_PARAM_FRIENDSHIP_1,
    ITEM_PARAM_FRIENDSHIP_2,
    ITEM_PARAM_FRIENDSHIP_3,
};

// How many of an item the bag can hold: one of a TM or HM, 999 of others
u32 PML_ItemGetMaxStorageCount(u16 item);

// Black 2's, not checked against Black
// What an item's work holds
#define ITEM_WORK_VALUE 0

// Black 2's, not checked against Black
#define ITEM_WORK_PARAMS 1
// The files of an item that GetItemGraphicsDatID and PML_ItemReadDataFile give: its data in ARCID_ITEMINFO, its icon's
// characters and palette in ARCID_ITEMGRA, and the characters and palette of the icon of the Wonder Launcher's
// items, in ShooterItem_GetIndex's order
#define ITEM_FILE_DATA 0

#define ITEM_FILE_ICON_CHAR 1
#define ITEM_FILE_ICON_PLTT 2
#define ITEM_FILE_LIST_ICON_CHAR 3
#define ITEM_FILE_LIST_ICON_PLTT 4

ArcTool *PML_ItemArcHandleCreate(HeapID heapId);
// A TM's or HM's bit in a species' TM flags: TM01 to TM92, TM93 to TM95, then HM01 to HM06. 0xff for other items
u8 PML_ItemGetTMBitMask(u16 item);
// An HM's number from 0, or 0xff
u8 PML_ItemGetHMID(u16 item);
// The mail of a mail item, and the item of a mail
u8 PML_ItemGetMailID(u16 item);
u16 PML_ItemGetMailItemID(u8 mail);
// pokebw2's PML_ItemGetIconArcID
// ARCID_ITEMGRA, and the files of the item icons' cells and animations in it
u32 func_02020f08(void);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
u16 GetItemGraphicsDatID(u16 item, u32 type);
ItemData *PML_ItemArcHandleReadFile(ArcTool *handle, u16 item, HeapID heapId);
void setItemDescriptionTextToStrbuf(StrBuf *strbuf, u16 item, HeapID heapId);
// The ball ID of a ball item, or 0
u16 PML_ItemGetMonsBallID(u16 item);
void setItemNameToStrbuf(StrBuf *strbuf, u16 item, HeapID heapId);
// Whether the item isn't one of HGSS's special balls, from the Fast Ball to the Park Ball
BOOL PML_ItemIsNotSpecialMonsball(u16 item);
// pokebw2's PML_ItemGetIconCellDatID
u32 func_02020f0c(void);
// pokebw2's PML_ItemGetIconAnimDatID
u32 func_02020f10(void);

#endif // BWPORT_PML_ITEM_H
