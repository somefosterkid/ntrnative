#include "global.h"
#include "constants/heap.h"
#include "struct_defs/resdat_narc_structs.h"
#include "battle/battle.h"
#include "battle/battle_cursor.h"
#include "battle/battle_finger.h"
#include "battle/battle_system.h"
#include "bag_types_def.h"
#include "bg_window.h"
#include "filesystem.h"
#include "gf_3d_loader.h"
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
#include "overlay_08_battle_bag.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "overlay_08_battle_party.h"
#include "battle/battle_bag_context.h"

ManagedSprite *ov08_02223EA4(void *, u32);

void ov08_02223F14(void *arg0);

typedef struct BagItem {
    u16 item;
    u16 quantity;
} BagItem;
typedef u16 charcode_t;
void ov08_02224108(BattleBag *battleBag);
void ov08_022241F4(BattleBag *battleBag);
void ov08_02223F34(BattleBag *battleBag);

void ov08_02223F14(void *arg0) {
    u32 v0;
    struct ManagedSprite **v1;

    v0 = 0;
    v1 = (struct ManagedSprite **)arg0;
    do {
        *(void **)((u8 *)v1 + offsetof(BattleBag, sprites)) = ov08_02223EA4(arg0, v0);
        v0 += 1;
        v1 += 1;
    } while (v0 < 6U);
}

void ov08_02223F34(BattleBag *battleBag) {
    SpriteSystem *spriteSystem = BattleSystem_GetSpriteSystem(battleBag->context->battleSys);

    for (u32 i = 0; i < 6; i++) {
        Sprite_DeleteAndFreeResources(battleBag->sprites[i]);
    }

    ov08_02224108(battleBag);
    ov08_022241F4(battleBag);
    SpriteSystem_FreeResourcesAndManager(spriteSystem, battleBag->spriteManager);
}
