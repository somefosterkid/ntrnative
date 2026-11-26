#include "global.h"

#include "constants/heap.h"

#include "struct_defs/resdat_narc_structs.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_3d_loader.h"
#include "item.h"
#include "message_format.h"
#include "message_printer.h"
#include "msgdata.h"
#include "palette.h"
#include "pm_string.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_02020A0C.h"
#include "gf_gfx_planes.h"
#include "overlay_08_battle_party.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "bag_types_def.h"
#include "battle/battle_bag_context.h"
#include "overlay_08_battle_bag.h"
#include "constants/items.h"

#if defined(SDK_PORT) || defined(NONMATCHING) // as defined in src/bag.c (tools/pokehg_abi.py)
#if defined(SDK_PORT) || defined(NONMATCHING)
ItemSlot *Bag_GetPocketSlotN(Bag *bag, u8 pocketID, u32 slot);
#else
ItemSlot *Bag_GetPocketSlotN(Bag *bag, u16 pocketID, u32 slot);
#endif
#else
ItemSlot *Bag_GetPocketSlotN(Bag *bag, u16 pocketID, u16 slot);
#endif

extern const u8 ov08_02225CE0[5];

void ov08_02223BF4(BattleBag *battleBag);

u16 ov08_02223CD4(BattleBag *battleBag, u32 pagePosition);

s32 ov08_02223D34(BattleBag *battleBag);
s32 ov08_02223D80(void *);
s32 ov08_02223F14(void *);
s32 ov08_022240A8(void *);
s32 ov08_0222419C(void *);
void ov08_02223D08(void *arg0);

void ov08_02223BF4(BattleBag *battleBag) {
    for (u32 i = 0; i < 8; i++) {
        u32 currentPocketSlot = 0;

        while (1) {
            ItemSlot *bagItem = Bag_GetPocketSlotN(battleBag->context->bag, i, currentPocketSlot);

            if (bagItem == NULL) {
                break;
            }

            if (!(bagItem->id == ITEM_NONE || bagItem->quantity == 0)) {
                s32 bagItemBattlePocketMask = GetItemAttr(bagItem->id, ITEMATTR_BATTLE_POCKET, battleBag->context->heapID);

                for (u32 l = 0; l < 5; l++) {
                    if ((bagItemBattlePocketMask & (1 << l)) == 0) {
                        continue;
                    }

                    battleBag->items[ov08_02225CE0[l]][battleBag->numBattlePocketItems[ov08_02225CE0[l]]] = *bagItem;
                    battleBag->numBattlePocketItems[ov08_02225CE0[l]]++;
                }
            }

            currentPocketSlot++;
        }
    }

    for (u32 i = 0; i < 5; i++) {
        if (battleBag->numBattlePocketItems[i] == 0) {
            battleBag->numBattlePocketPages[i] = 0;
        } else {
            battleBag->numBattlePocketPages[i] = (battleBag->numBattlePocketItems[i] - 1) / 6;
        }

        if (battleBag->numBattlePocketPages[i] < battleBag->context->pocketCurrentPages[i]) {
            battleBag->context->pocketCurrentPages[i] = battleBag->numBattlePocketPages[i];
        }
    }
}

u16 ov08_02223CD4(BattleBag *battleBag, u32 pagePosition) {
    if (battleBag->items[battleBag->currentBattlePocket][battleBag->context->pocketCurrentPages[battleBag->currentBattlePocket] * 6 + pagePosition].id != ITEM_NONE && battleBag->items[battleBag->currentBattlePocket][battleBag->context->pocketCurrentPages[battleBag->currentBattlePocket] * 6 + pagePosition].quantity != 0) {
        return battleBag->items[battleBag->currentBattlePocket][battleBag->context->pocketCurrentPages[battleBag->currentBattlePocket] * 6 + pagePosition].id;
    }

    return ITEM_NONE;
}

void ov08_02223D08(void *arg0) {
    ov08_02223D34(arg0);
    ov08_02223D80(arg0);
    ov08_02223F14(arg0);
    ov08_022240A8(arg0);
    ov08_0222419C(arg0);
    GfGfx_EngineBTogglePlanes(0x10U, 1U);
}
