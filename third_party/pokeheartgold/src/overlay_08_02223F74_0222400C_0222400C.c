#include "global.h"

#include "constants/heap.h"

#include "battle/battle.h"
#include "battle/battle_cursor.h"
#include "battle/battle_finger.h"

#include "bag_types_def.h"
#include "bg_window.h"
#include "filesystem.h"
#include "message_format.h"
#include "message_printer.h"
#include "msgdata.h"
#include "palette.h"
#include "pm_string.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "unk_02020A0C.h"
#include "overlay_08_battle_bag.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "overlay_08_battle_party.h"
#include "battle/battle_bag_context.h"

typedef struct BagItem BagItem;

typedef u16 charcode_t;

struct BagItem {
    u16 item;
    u16 quantity;
};

u16 ov08_02223CD4(BattleBag *, u32);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov08_02223E3C(BattleBag *, u16, u32);
#else
void ov08_02223E3C(BattleBag *, s32, u32);
#endif

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov08_02223E74(BattleBag *, u16, s16, u32);
#else
void ov08_02223E74(BattleBag *, s32, u32, u32);
#endif

void ov08_02223F74(ManagedSprite *, u32, u32);

extern u32 ov08_02225D74;

void ov08_0222400C(BattleBag *battleBag);

void ov08_0222400C(BattleBag *battleBag) {
    s32 iVar1;
    u32 uVar2;
    u32 *puVar3;
    BattleBag *pBVar4;

    puVar3 = (u32 *)&ov08_02225D74;
    uVar2 = 0;
    pBVar4 = battleBag;
    do {
        iVar1 = ov08_02223CD4(battleBag, uVar2);
        if (iVar1 != 0) {
            ov08_02223E3C(battleBag, iVar1, uVar2 + 0xb4b7);
            ov08_02223E74(battleBag, iVar1, uVar2 & 0xffff, uVar2 + 0xb4b7);
            ov08_02223F74(pBVar4->sprites[0], *puVar3, puVar3[1]);
        }
        uVar2 = uVar2 + 1;
        puVar3 = puVar3 + 2;
        pBVar4 = (BattleBag *)&pBVar4->background;
    } while (uVar2 < 6);
    return;
}
