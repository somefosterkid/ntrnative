#include "global.h"
struct BattleBag;

#include "battle/battle_cursor.h"
#include "constants/heap.h"
#include "struct_defs/resdat_narc_structs.h"
#include "battle/battle.h"
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
#include "overlay_08_battle_party.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "battle/battle_bag_context.h"
#include "overlay_08_battle_bag.h"

BattleCursor *ov08_02224B84(void *);

s32 ov08_02224B90(void *, s32);

s32 ov08_02224BC0(void *);

void ov08_0222417C(struct BattleBag *arg0);

struct BattleBagContext;
struct BattleSubMenuCursor;
BattleFinger *BattleFinger_New(SpriteSystem *spriteSys, SpriteManager *spriteMan, enum HeapID heapID, u32 charResID, u32 plttResID, u32 cellResID, u32 animResID, u32 priority, u32 bgPriority);
typedef struct BagItem {
    u16 item;
    u16 quantity;
} BagItem;
typedef u16 charcode_t;
void ov08_0222419C(BattleBag *battleBag);
void ov08_022241F4(BattleBag *battleBag);

extern u32 ov08_02225D2C;
extern u32 ov08_02225D30;
void ov08_0222421C(BattleBag *battleBag, s32 param_2);

void ov08_0222417C(BattleBag *arg0) {
    ov08_02224B90(*((void **)&arg0->cursor), 0);
    ov08_02224BC0(*((void **)&arg0->cursor));
    BattleCursor_Disable(ov08_02224B84(*((void **)&arg0->cursor)));
}

void ov08_0222419C(BattleBag *battleBag) {
    SpriteSystem *spriteSystem = BattleSystem_GetSpriteSystem(battleBag->context->battleSys);

    BattleFinger_LoadResources(spriteSystem, battleBag->spriteManager, battleBag->context->heapID, battleBag->palette, 46269, 46269, 46264, 46264);
    battleBag->catchTutorialCursor = BattleFinger_New(spriteSystem, battleBag->spriteManager, battleBag->context->heapID, 46269, 46269, 46264, 46264, 0, 0);
}

void ov08_022241F4(BattleBag *battleBag) {
    BattleFinger_Delete(battleBag->catchTutorialCursor);
    BattleFinger_FreeResources(battleBag->spriteManager, 46269, 46269, 46264, 46264);
}

void ov08_0222421C(BattleBag *battleBag, s32 param_2) {
    if (battleBag->context->isInCatchTutorial == 1) {
        ov12_0226BD2C(battleBag->catchTutorialCursor, *(s32 *)(&ov08_02225D2C + param_2 * 2), *(s32 *)(&ov08_02225D30 + param_2 * 2));
        ov12_0226BD4C(battleBag->catchTutorialCursor, 0x3c);
        return;
    }
    BattleFinger_Disable(battleBag->catchTutorialCursor);
    return;
}
