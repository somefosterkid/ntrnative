#include "global.h"
struct BattleBag;

#include "battle/battle_cursor.h"
#include "battle/battle_system.h"
#include "battle/battle_bag_context.h"
#include "overlay_08_battle_bag.h"

u16 ov08_02223CD4(void *, u32);

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov08_02223E3C(void *, u16, s32);
#else
s32 ov08_02223E3C(void *, s32, s32);
#endif

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov08_02223E74(void *, u16, s16, s32);
#else
s32 ov08_02223E74(void *, s32, s32, s32);
#endif

s32 ov08_02223F74(ManagedSprite *, s32, s32);

void ov08_02224064(void *arg0);

void ov08_02224064(void *arg0) {
    s32 v0;

    v0 = ov08_02223CD4(arg0, ((BattleBag *)arg0)->context->pocketCurrentPagePositions[((BattleBag *)arg0)->currentBattlePocket]);
    ov08_02223E3C(arg0, v0, 0xB4B7);
    ov08_02223E74(arg0, v0, 0, 0xB4B7);
    ov08_02223F74(*((void **)&((BattleBag *)arg0)->sprites[0]), 0x28, 0x2C);
}

s32 ov08_02224B94(void *, BattleCursor *);

void ov08_022240A8(void *arg0);

void ov08_022240A8(void *arg0) {
    SpriteSystem *renderer;

    renderer = BattleSystem_GetSpriteSystem(((BattleBag *)arg0)->context->battleSys);
    BattleCursor_LoadResources(renderer, ((BattleBag *)arg0)->spriteManager, ((BattleBag *)arg0)->palette, ((BattleBag *)arg0)->context->heapID, 0xB4BEU, 0xB4BEU, 0xB4B9U, 0xB4B9U);
    ov08_02224B94(*((void **)&((BattleBag *)arg0)->cursor), BattleCursor_New(renderer, ((BattleBag *)arg0)->spriteManager, ((BattleBag *)arg0)->context->heapID, 0xB4BEU, 0xB4BEU, 0xB4B9U, 0xB4B9U, 0U, 1U));
}

BattleCursor *ov08_02224B84(void *);

void ov08_02224108(struct BattleBag *arg0);

void ov08_02224108(BattleBag *arg0) {
    BattleCursor_Delete(ov08_02224B84(*((void **)&arg0->cursor)));
    BattleCursor_FreeResources(arg0->spriteManager, 0xB4BEU, 0xB4BEU, 0xB4B9U, 0xB4B9U);
}
