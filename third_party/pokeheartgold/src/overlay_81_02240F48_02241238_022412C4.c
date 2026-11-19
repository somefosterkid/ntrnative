#include "global.h"
#include "overlay_81_battle_factory_app.h"

s32 ov80_02237254(u8);

s32 ov81_02240F08(void *, s32);

BattleFactoryAppPokeballSprite *ov81_02242E50(BattleFactoryAppSpriteManager *spriteMan, int x, int y, enum HeapID heapID);

extern s32 ov81_0224345C;

extern s32 ov81_0224345E;

extern s32 ov81_02243464;

extern s32 ov81_02243466;

extern s32 ov81_02243470;

extern s32 ov81_02243472;

extern s32 ov81_02243490;

extern s32 ov81_02243492;

BattleFactoryAppPokeballSprite *ov81_022412C4(BattleFactoryApp *arg0, s32 arg1);

BattleFactoryAppPokeballSprite *ov81_022412C4(BattleFactoryApp *arg0, s32 arg1) {
    s32 temp_r2;
    s32 temp_r2_2;
    s32 temp_r2_3;
    s32 temp_r2_4;
    u16 var_r1;
    u16 var_r2;

    if (ov81_02240F08(arg0, 0) == 1) {
        temp_r2 = arg1 * 4;
        var_r1 = *(u16 *)(((u8 *)&ov81_02243490) + temp_r2);
        var_r2 = *(u16 *)(((u8 *)&ov81_02243492) + temp_r2);
    } else if (ov80_02237254(arg0->challengeType) == 0) {
        temp_r2_2 = arg1 * 4;
        var_r1 = *(u16 *)(((u8 *)&ov81_02243464) + temp_r2_2);
        var_r2 = *(u16 *)(((u8 *)&ov81_02243466) + temp_r2_2);
    } else if (arg0->numPokeballs == 2) {
        temp_r2_3 = arg1 * 4;
        var_r1 = *(u16 *)(((u8 *)&ov81_0224345C) + temp_r2_3);
        var_r2 = *(u16 *)(((u8 *)&ov81_0224345E) + temp_r2_3);
    } else {
        temp_r2_4 = arg1 * 4;
        var_r1 = *(u16 *)(((u8 *)&ov81_02243470) + temp_r2_4);
        var_r2 = *(u16 *)(((u8 *)&ov81_02243472) + temp_r2_4);
    }
    return ov81_02242E50(&arg0->spriteMan, var_r1, var_r2, HEAP_ID_100);
}
